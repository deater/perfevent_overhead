/* by Vince Weaver, vincent.weaver@maine.edu */

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


#include "perfmon/pfmlib.h"
#include "perfmon/perfmon.h"

#define MAX_EVENTS 16

char events[MAX_EVENTS][64];

unsigned long long rdtsc(void) {
  unsigned a,d;

  __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

  return ((unsigned long long)a) | (((unsigned long long)d) <<32);
}

int main(int argc, char **argv) {

   int i,ctx_fd;
   
   long long before,after;
   long long start_before,start_after;
   long long stop_before,stop_after;
   long long read_before,read_after;

   long long init_ns=0,eventset_ns=0;
   int ret1,ret2,ret3;
   pfm_err_t ret;

   int count=0;

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
      strncpy(events[i],argv[2+i],64);
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

   pfmlib_options_t pfmlib_options;

   memset(&pfmlib_options,0,sizeof(pfmlib_options));
   pfm_set_options(&pfmlib_options);

   ret=pfm_initialize();
   if (ret != PFMLIB_SUCCESS) {
     fprintf(stderr,"Error starting pfmlib\n");
     exit(1);
   }

   unsigned int num_counters;
   pfmlib_input_param_t inp;
   pfmlib_output_param_t outp;

#define NUM_PMCS PFMLIB_MAX_PMCS
#define NUM_PMDS PFMLIB_MAX_PMDS
   pfarg_pmd_t pd[NUM_PMDS];
   pfarg_pmc_t pc[NUM_PMCS];
   pfarg_ctx_t ctx;
   pfarg_load_t load_args;
   

   pfm_get_num_counters(&num_counters);

   memset(pd,0,sizeof(pd));
   memset(pc,0,sizeof(pc));
   memset(&ctx,0,sizeof(ctx));
   memset(&load_args,0,sizeof(load_args));
   
   memset(&inp,0,sizeof(inp));
   memset(&outp,0,sizeof(outp));

   inp.pfp_dfl_plm = PFM_PLM3; /* user only */

   inp.pfp_event_count=count;

   for(i=0;i<count;i++) {
      if (i<num_counters) {  
	 pfm_find_full_event(events[i],&inp.pfp_events[i]);
      }
      
   }
   
   
   ctx_fd=pfm_create_context(&ctx,NULL,NULL,0);
   if (ctx_fd==-1) {
     fprintf(stderr,"Error in pfm_create()\n");
     exit(1);
   }
   
   //detect_unavail_pmcs(ctx_fd, &inp.pfp_unavail_pmcs);
   
   ret=pfm_dispatch_events(&inp,NULL,&outp,NULL);
   if (ret!=PFMLIB_SUCCESS) {
      printf("Eventset_creation_%d: %lld ns\n",i,0LL);
      printf("start/stop/read latency: %lld ns\n",0LL);
      printf("dispatch_events failed\n");
      exit(1);
   }
   
   for(i=0;i<outp.pfp_pmc_count;i++) {
      pc[i].reg_num=outp.pfp_pmcs[i].reg_num;
      pc[i].reg_value=outp.pfp_pmcs[i].reg_value;


   }
   for(i=0;i<outp.pfp_pmd_count;i++) {
      pd[i].reg_num=outp.pfp_pmds[i].reg_num;
   }
   
   ret=pfm_write_pmcs(ctx_fd,pc,outp.pfp_pmc_count);
   if (ret!=PFMLIB_SUCCESS) {
      fprintf(stderr,"pfm_write_pmcs() failed\n");
      exit(1);
   }
   
   ret=pfm_write_pmds(ctx_fd,pd,outp.pfp_pmd_count);
   if (ret!=PFMLIB_SUCCESS) {
      fprintf(stderr,"pfm_write_pmds() failed\n");
      exit(1);
   }
   
   load_args.load_pid=getpid();
   ret=pfm_load_context(ctx_fd, &load_args);
   if (ret!=PFMLIB_SUCCESS) {
      fprintf(stderr,"pfm_load_context() failed\n");
      exit(1);
   }
   
   after=rdtsc();

   eventset_ns=after-before;
   printf("Eventset_creation_%d: %lld cycles\n",i,eventset_ns);

   /*********/
   /* start */
   /*********/

   start_before=rdtsc();


   ret1=pfm_start(ctx_fd,NULL);
   /* syscall(PFM_pfm_start,fd,NULL) */

   start_after=rdtsc();

   /********/
   /* stop */
   /********/
   
   ret2=pfm_stop(ctx_fd);
   /* syscall(PFM_pfm_stop,fd); */

   stop_after=rdtsc();

   /********/
   /* read */
   /********/

   ret3=pfm_read_pmds(ctx_fd,pd,inp.pfp_event_count);
   /* syscall(PFM_pfm_read_pmds, fd, pmds, count); */

   read_after=rdtsc();

   stop_before=start_after;
   read_before=stop_after;

   if (ret3<0) {
      printf("start latency: %lld cycles\n",0LL);
      printf("stop latency: %lld cycles\n",0LL);
      printf("read latency: %lld cycles\n",0LL);
      printf("Unexpected read result %d %s\n",ret3,strerror(errno));
      exit(0);
   }


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
     printf("%llx %lld\n",outp.pfp_pmcs[i].reg_value,(long long)pd[i].reg_value);
   }

   /* close */

   return 0;
}
