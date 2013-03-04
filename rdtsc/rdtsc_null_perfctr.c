/* by Vince Weaver, vincent.weaver@maine.edu                      */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/utsname.h>

#include <fcntl.h>
#include <sys/mman.h>

#include <time.h>

#include <unistd.h>
#include <asm/unistd.h>

#include <sys/ioctl.h>
#include <errno.h>

#include <stddef.h> /* offsetof() */

#include "perfctr/libperfctr.h"

#define PERF_CTR_MASK     0xff000000
#define PERF_INV_CTR_MASK 0x00800000
#define PERF_ENABLE       0x00400000
#define PERF_INT_ENABLE   0x00100000
#define PERF_PIN_CONTROL  0x00080000
#define PERF_EDGE_DETECT  0x00040000
#define PERF_OS           0x00020000
#define PERF_USR          0x00010000
#define PERF_UNIT_MASK    0x0000ff00
#define PERF_EVENT_MASK   0x000000ff

#define PAGE_SIZE 4096

#define rdtscl(low) __asm__ __volatile__("rdtsc" : "=a"(low) : : "edx")
#define rdpmcl(ctr,low) __asm__ __volatile__("rdpmc" : "=a"(low) : "c"(ctr) : "edx")


#define MAX_EVENTS 16

int events[MAX_EVENTS];

unsigned long long rdtsc(void) {
  unsigned a,d;

  __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));
  return ((unsigned long long)a) | (((unsigned long long)d) << 32);
}

int main(int argc, char **argv) {

   int i,fd,dev_perfctr_fd;
 
   long long before,after;
   long long start_before,start_after;
   long long stop_before,stop_after;
   long long read_before,read_after;

   long long init_ns=0,eventset_ns=0;

   int ret1,ret2;

   int count=0;

   volatile const struct vperfctr_state *kstate;

   struct perfctr_sum_ctrs sum;
   struct vperfctr_control control,control_stop;

   if (argc < 2) {
     fprintf(stderr,"Usage: %s count event0 ... eventN\n",argv[0]);
     return -1;
   }

   if (argc>2) {
     count=atoi(argv[1]);
   }

   if (count!=argc-2) {
     fprintf(stderr,"Error!  Count mismatch!\n");
     return -1;
   }

   for(i=0;i<count;i++) {
     events[i]=strtol(argv[2+i],NULL,0);
   }

   /* measure init latency */

   before=rdtsc();

   /* init */

   after=rdtsc();

   init_ns=after-before;

   printf("init_latency: %lld cycles\n",init_ns);

   /*****************************/
   /* measure eventset creation */
   /*****************************/

   before=rdtsc();
	       
   /* result=vperfctr_open_pid(0,self,VPERFCTR_OPEN_CREAT_EXCL); */
   dev_perfctr_fd=open("/dev/perfctr",O_RDONLY);
   if (dev_perfctr_fd<1) {
      fprintf(stderr,"Error opening /dev/perfctr!\n");
      exit(1);
   }
   
   /* pid 0 = self? */
   fd=ioctl(dev_perfctr_fd, VPERFCTR_CREAT,0);
   
   close(dev_perfctr_fd);
   
   if (fcntl(fd,F_SETFD, FD_CLOEXEC)<0) {
      fprintf(stderr,"Error running fcntl\n");
      exit(1);
   }
   
   kstate=mmap(NULL, PAGE_SIZE, PROT_READ, MAP_SHARED, fd, 0);
   if (kstate==MAP_FAILED) {
      fprintf(stderr,"Error mmaping() buffer\n");	
   }

   memset(&control,0,sizeof control);
   
   for(i=0;i<count;i++) {
     control.cpu_control.evntsel[i]=PERF_ENABLE|PERF_USR|(events[i]&0xffff);
     control.cpu_control.pmc_map[i]=i;//|FAST_RDPMC;
   }

   /* accumulating counters */
   control.cpu_control.nractrs=count;
   /* interrupting counters */
   control.cpu_control.nrictrs=0;
   control.cpu_control.tsc_on=1;
   
   /* tsc */
   control.cpu_control.tsc_on=1;

   memset(&control_stop,0,sizeof control_stop);

   after=rdtsc();

   eventset_ns=after-before;

   printf("Eventset_creation_%d: %lld cycles\n",i,eventset_ns);

   start_before=rdtsc();

   /*********/
   /* start */
   /*********/

   ret1=_vperfctr_control(fd, &control); 

   /* ret1=perfctr_ioctl_w(fd,VPERFCTR_CONTROL, &control, &vperfctr_control_sdesc);*/
   /* Trying to unpack this more leads to realizing that perfctr */
   /* does some sort of crazy structure packing/unpacking at ioctl() */
   /* time, which probably has a lot of overhead */
   /* tried to replicate it here, too complex! */

   start_after=rdtsc();

   /********/
   /* read */
   /********/

   unsigned int cstatus,nrctrs;
   unsigned int tsc0, now;
   
   cstatus=kstate->cpu_state.cstatus;
   nrctrs=perfctr_cstatus_nrctrs(cstatus);

#ifdef USE_SLOW
   if (0) {
#else
   if (perfctr_cstatus_has_tsc(cstatus) ) {
#endif   

retry:
   tsc0=kstate->cpu_state.tsc_start;
   rdtscl(now);
   sum.tsc = kstate->cpu_state.tsc_sum+(now-tsc0);
   for(i = nrctrs; --i >=0 ;) {
      rdpmcl(kstate->cpu_state.pmc[i].map, now);
      sum.pmc[i] = kstate->cpu_state.pmc[i].sum+(now-kstate->cpu_state.pmc[i].start);
   }
   if (tsc0!=kstate->cpu_state.tsc_start) {
     goto retry;
   }

   }
   else {

          _vperfctr_read_sum(fd,&sum);
     //     perfctr_ioctl_r(fd,VPERFCTR_READ_SUM, &sum, &perfctr_sum_ctrs_sdesc);

   }

   read_after=rdtsc();

   /********/
   /* stop */
   /********/
   
   ret2=_vperfctr_control(fd, &control_stop);
   
   stop_after=rdtsc();

   stop_before=read_after;
   read_before=start_after;

   /*   if (ret3<0) {
      printf("start/stop/read latency: %lld ns\n",0LL);
      printf("Unexpected read result %d %s\n",ret3,strerror(errno));
      exit(0);
   }
   */

   if (ret1<0) {
      printf("start latency: %lld cycles\n",0LL);
      printf("stop latency: %lld cycles\n",0LL);
      printf("read latency: %lld cycles\n",0LL);
      printf("Error starting!\n");
      exit(1);
   }
   
   
   printf("start latency: %lld cycles\n",start_after-start_before);
   printf("stop latency: %lld cycles\n",stop_after-stop_before);
   printf("read latency: %lld cycles\n",read_after-read_before);

   if (ret2<0) {
      printf("Error stopping!\n");
      exit(1);
   }
   
   for(i=0;i<count;i++) {

     printf("%x %lld\n",events[i],sum.pmc[i]);
   }

   /* close */

   return 0;
}
