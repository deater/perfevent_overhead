/* by Vince Weaver, vweaver1@eecs.utk.edu                      */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
#define rdpmcl(ctr,low) __asm__ __volatile__("rdpmc" : "=a"(low) : "c"(ctr): "edx")

long long convert_to_ns(struct timespec *before,
			struct timespec *after) {

  long long seconds;
  long long ns;

  seconds=after->tv_sec - before->tv_sec;
  ns = after->tv_nsec - before->tv_nsec;

  ns = (seconds*1000000000ULL)+ns;

  return ns;
}

#define MAX_EVENTS 16

int core2_events[MAX_EVENTS]={
  0x53003c, //"UNHALTED_CORE_CYCLES",         /* PAPI_TOT_CYC */
  0x5300c0, //"INSTRUCTIONS_RETIRED",         /* PAPI_TOT_INS */
  0x5300c4, //"BRANCH_INSTRUCTIONS_RETIRED",  /* PAPI_BR_INS  */
  0x5300c5, //"MISPREDICTED_BRANCH_RETIRED",  /* PAPI_BR_MSP  */
  0x531282, //"ITLB:MISSES",                  /* PAPI_TLB_IM  */
  0x530108, //"DTLB_MISSES:ANY",              /* PAPI_TLB_DM  */
  0x530080, //"L1I_READS",                    /* PAPI_L1_ICA  */
  0x530081, //"L1I_MISSES",                   /* PAPI_L1_ICM  */
  0x530143, //"L1D_ALL_REF",                  /* PAPI_L1_DCA  */
  0x530f45, //"L1D_REPL",                     /* PAPI_L1_DCM  */
  0x5300c8, //"HW_INT_RCV",                   /* PAPI_HW_INT  */
  0x530010, //"FP_COMP_OPS_EXE",              /* PAPI_FP_OPS  */
  0x5301c0, //"INST_RETIRED:LOADS",           /* PAPI_LD_INS  */
  0x5302c0, //"INST_RETIRED:STORES",          /* PAPI_SR_INS  */
  0x537f2e, //"L2_RQSTS:SELF:ANY:MESI",       /* PAPI_L2_TCA  */
  0x537024, //"L2_LINES_IN:SELF:ANY",         /* PAPI_L2_TCM  */
};

int atom_events[MAX_EVENTS]={
  0x53003c, //"UNHALTED_CORE_CYCLES",         /* PAPI_TOT_CYC */
  0x5300c0, // "INSTRUCTIONS_RETIRED",         /* PAPI_TOT_INS */
  0x5300c4, // "BRANCH_INSTRUCTIONS_RETIRED",  /* PAPI_BR_INS  */
  0x5300c5, // "MISPREDICTED_BRANCH_RETIRED",  /* PAPI_BR_MSP  */
  0x530282, // "ITLB:MISSES",                  /* PAPI_TLB_IM  */
  0x530708, // "DATA_TLB_MISSES:DTLB_MISS",    /* PAPI_TLB_DM  */
  0x530380, //"ICACHE:ACCESSES",              /* PAPI_L1_ICA  */
  0x530280, // "ICACHE:MISSES",                /* PAPI_L1_ICM  */
  0x532140, // "L1D_CACHE:LD",                 /* PAPI_L1_DCA  */
  0x537f2e, // "L2_RQSTS:SELF",                /* PAPI_L1_DCM  */
  0x5301c8, //"HW_INT_RCV",                   /* PAPI_HW_INT  */
  0x531fc7, // "SIMD_INST_RETIRED:ANY",        /* PAPI_FP_OPS  */
  0x537f29, //"L2_LD:SELF:ANY:MESI",          /* PAPI_LD_INS  */
  0x534f2a, //"L2_ST:SELF:MESI",              /* PAPI_SR_INS  */
  0x537f29, //"L2_LD:SELF:ANY:MESI",          /* PAPI_L2_TCA  */
  0x537024, //"L2_LINES_IN:SELF:ANY",         /* PAPI_L2_TCM  */
};

int amd10h_events[MAX_EVENTS]={
  0x530076, // "CPU_CLK_UNHALTED",                         /* PAPI_TOT_CYC */
  0x5300c0, // "RETIRED_INSTRUCTIONS",                     /* PAPI_TOT_INS */
  0x5300c2, // "RETIRED_BRANCH_INSTRUCTIONS",              /* PAPI_BR_INS  */
  0x5300c3, // "RETIRED_MISPREDICTED_BRANCH_INSTRUCTIONS", /* PAPI_BR_MSP  */
  0x530385, // "L1_ITLB_MISS_AND_L2_ITLB_MISS:ALL",        /* PAPI_TLB_IM  */
  0x530746, // "L1_DTLB_AND_L2_DTLB_MISS",                 /* PAPI_TLB_DM  */
  0x530080, // "INSTRUCTION_CACHE_FETCHES",                /* PAPI_L1_ICA  */
  0x530081, // "INSTRUCTION_CACHE_MISSES",                 /* PAPI_L1_ICM  */
  0x530040, // "DATA_CACHE_ACCESSES",                      /* PAPI_L1_DCA  */
  0x530041, // "DATA_CACHE_MISSES",                        /* PAPI_L1_DCM  */
  0x5300cf, // "INTERRUPTS_TAKEN",                         /* PAPI_HW_INT  */
  0x530300, // "DISPATCHED_FPU:OPS_MULTIPLY:OPS_ADD",      /* PAPI_FP_OPS  */
  0x5300d0, // "DECODER_EMPTY",                            /* PAPI_LD_INS  */ /* nope */
  0x5300d1, // "DISPATCH_STALLS",                          /* PAPI_SR_INS  */ /* nope */
  0x533f7d, // "REQUESTS_TO_L2:ALL",                       /* PAPI_L2_TCA  */
  0x53037e, // "L2_CACHE_MISS:INSTRUCTIONS:DATA",          /* PAPI_L2_TCM  */
};

int nehalem_events[MAX_EVENTS]={
  0x53003c, // "UNHALTED_CORE_CYCLES",         /* PAPI_TOT_CYC */
  0x5300c0, // "INSTRUCTIONS_RETIRED",         /* PAPI_TOT_INS */
  0x537f88, // "BR_INST_EXEC:ANY",             /* PAPI_BR_INS  */
  0x537f89, // "BR_MISP_EXEC:ANY",             /* PAPI_BR_MSP  */
  0x530185, // "ITLB_MISSES:ANY",              /* PAPI_TLB_IM  */
  0x530149, // "DTLB_MISSES:ANY",              /* PAPI_TLB_DM  */
  0x530380, // "L1I:READS",                    /* PAPI_L1_ICA  */
  0x530280, // "L1I:MISSES",                   /* PAPI_L1_ICM  */
  0x530143, // "L1D_ALL_REF:ANY",              /* PAPI_L1_DCA  */
  0x530151, // "L1D:REPL",                     /* PAPI_L1_DCM  */
  0x53011d, // "HW_INT:RCV",                   /* PAPI_HW_INT  */
  0x530410, // "FP_COMP_OPS_EXE:SSE_FP",       /* PAPI_FP_OPS  */
  0x53010b, // "MEM_INST_RETIRED:LOADS",       /* PAPI_LD_INS  */
  0x53020b, // "MEM_INST_RETIRED:STORES",      /* PAPI_SR_INS  */
  0x53ff24, // "L2_RQSTS:REFERENCES",          /* PAPI_L2_TCA  */
  0x53c024, // "L2_RQSTS:PREFETCHES",          /* PAPI_L2_TCM  */
};

int power6_events[MAX_EVENTS]={
  0x10000a, // "PM_RUN_CYC",         	/* PAPI_TOT_CYC */
  0x2,      // "PM_INST_CMPL",         	/* PAPI_TOT_INS */
  0x430e6,  // "PM_BRU_FIN",             	/* PAPI_BR_INS  */
  0x400052, // "PM_BR_MPRED",             	/* PAPI_BR_MSP  */
  0x2000fe, // "PM_DATA_FROM_L2MISS",        /* PAPI_TLB_IM  */  /* nope */
  0x2000fe, // "PM_DATA_FROM_L2MISS",        /* PAPI_TLB_DM  */  /* nope */
  0x80086,  // "PM_ST_REF_L1",               /* PAPI_L1_ICA  */ /* nope */
  0x100056, // "PM_L1_ICACHE_MISS",          /* PAPI_L1_ICM  */
  0x80082,  // "PM_LD_REF_L1",               /* PAPI_L1_DCA  */  /* nope */
  0x80080, // "PM_LD_MISS_L1",              /* PAPI_L1_DCM  */  /* nope */
  0x2000f8, // "PM_EXT_INT",                 /* PAPI_HW_INT  */
  0x1c0032, // "PM_FPU_FLOP",       		/* PAPI_FP_OPS  */
  0x80082,  // "PM_LD_REF_L1",       	/* PAPI_LD_INS  */
  0x80086,   // "PM_ST_REF_L1",      		/* PAPI_SR_INS  */
  0x80082,  // "PM_LD_REF_L1",          	/* PAPI_L2_TCA  */ /* nope */
  0x2000fe, // "PM_DATA_FROM_L2MISS",        /* PAPI_L2_TCM  */ /* nope */
};

int cortexA9_events[MAX_EVENTS]={
  0xff, // "CPU_CYCLES",                    /* PAPI_TOT_CYC */
  0x68, // "INST_OUT_OF_RENAME_STAGE",      /* PAPI_TOT_INS */
  0xc,  //"PC_WRITE",                      /* PAPI_BR_INS  */
  0x10, // "PC_BRANCH_MIS_PRED",            /* PAPI_BR_MSP  */
  0x2,  // "ITLB_MISS",                     /* PAPI_TLB_IM  */
  0x5,  // "DTLB_REFILL",                   /* PAPI_TLB_DM  */
  0x70, // "MAIN_UNIT_EXECUTED_INST",       /* PAPI_L1_ICA  */ /* nope */
  0x1,  //"IFETCH_MISS",                   /* PAPI_L1_ICM  */
  0x4,  //"DCACHE_ACCESS",                  /* PAPI_L1_DCA  */
  0x3,  //"DCACHE_REFILL",                 /* PAPI_L1_DCM  */
  0x93, //"EXT_INTERRUPTS",                /* PAPI_HW_INT  */
  0x73, //"FP_EXECUTED_INST",              /* PAPI_FP_OPS  */
  0x6,  //"DREAD",                         /* PAPI_LD_INS  */
  0x7,  //"DWRITE",                        /* PAPI_SR_INS  */
  0x4,  //"DCACHE_ACCESS",                 /* PAPI_L2_TCA  */  /* nope */
  0x3,  //"DCACHE_REFILL",                 /* PAPI_L2_TCM  */  /* nope */
};

int events[MAX_EVENTS];

#define MATRIX_SIZE 128


int main(int argc, char **argv) {

   int i,fd,dev_perfctr_fd;
   
   struct timespec before,after;
   struct timespec start_before;
   struct timespec stop_after;

   long long init_ns=0,eventset_ns=0;
   long long ns=0;
   int ret1,ret2;

   int count;

   long long results[MATRIX_SIZE][MAX_EVENTS];

   int x,y,z,j;
   double s;

   char *machine_name;

   volatile const struct vperfctr_state *kstate;

   struct perfctr_sum_ctrs sum;
   struct vperfctr_control control,control_stop;

   if (argc>2) {
     count=atoi(argv[2]);
     machine_name=strdup(argv[1]);
   }
   else {
     count=1;
     machine_name=strdup("core2");
   }

   if (!strncmp(machine_name,"core2",5)) {
      memcpy(events,core2_events,MAX_EVENTS*sizeof(int));
   }
   else if (!strncmp(machine_name,"nehalem",7)) {
     memcpy(events,nehalem_events,MAX_EVENTS*sizeof(int));
   }
   else if (!strncmp(machine_name,"atom",4)) {
     memcpy(events,atom_events,MAX_EVENTS*sizeof(int));
   }
   else if (!strncmp(machine_name,"amd10h",6)) {
     memcpy(events,amd10h_events,MAX_EVENTS*sizeof(int));
   }
   else if (!strncmp(machine_name,"amd0fh",6)) {
     memcpy(events,amd10h_events,MAX_EVENTS*sizeof(int));
   }
   else if (!strncmp(machine_name,"power6",6)) {
     memcpy(events,power6_events,MAX_EVENTS*sizeof(int));
   }
   else if (!strncmp(machine_name,"cortexA9",8)) {
     memcpy(events,cortexA9_events,MAX_EVENTS*sizeof(int));
   }
   else {
      fprintf(stderr,"Unknown machine name %s\n",machine_name);
      exit(0);
   }

   
   /* measure init latency */

   clock_gettime(CLOCK_REALTIME,&before);

   /* init */

   clock_gettime(CLOCK_REALTIME,&after);

   init_ns=convert_to_ns(&before,&after);

   printf("init_latency: %lld ns\n",init_ns);

   /*****************************/
   /* measure eventset creation */
   /*****************************/

   clock_gettime(CLOCK_REALTIME,&before);
	       
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

   clock_gettime(CLOCK_REALTIME,&after);

   eventset_ns=convert_to_ns(&before,&after);

   printf("Eventset_creation_%d: %lld ns\n",i,eventset_ns);


   float a[MATRIX_SIZE][MATRIX_SIZE];
   float b[MATRIX_SIZE][MATRIX_SIZE];
   float c[MATRIX_SIZE][MATRIX_SIZE];

   for(x=0;x<MATRIX_SIZE;x++) {
     for(y=0;y<MATRIX_SIZE;y++) {
       a[x][y]=(float)x*(float)y;
       b[x][y]=(float)x/(float)(y+5);
     }
   }


   clock_gettime(CLOCK_REALTIME,&start_before);

   /*********/
   /* start */
   /*********/

   ret1=_vperfctr_control(fd, &control); 

   /* ret1=perfctr_ioctl_w(fd,VPERFCTR_CONTROL, &control, &vperfctr_control_sdesc);*/
   /* Trying to unpack this more leads to realizing that perfctr */
   /* does some sort of crazy structure packing/unpacking at ioctl() */
   /* time, which probably has a lot of overhead */
   /* tried to replicate it here, too complex! */



   
   for(x=0;x<MATRIX_SIZE;x++) {


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

   for(i=0;i<count;i++) {
     results[x][i]=sum.pmc[i];
   }

   /** END READ **/

   for(y=0;y<MATRIX_SIZE;y++) {

     s=0;
     for(z=0;z<MATRIX_SIZE;z++) {
       s+=a[y][z]*b[z][x];
     }
     c[y][x] = s;
   }
   }


   /********/
   /* stop */
   /********/

   ret2=_vperfctr_control(fd, &control_stop);
   
   clock_gettime(CLOCK_REALTIME,&stop_after);

   /*   if (ret3<0) {
      printf("start/stop/read latency: %lld ns\n",0LL);
      printf("Unexpected read result %d %s\n",ret3,strerror(errno));
      exit(0);
   }
   */

   if (ret1<0) {
      printf("start/stop/read latency: %lld ns\n",0LL);
      printf("Error starting!\n");
      exit(1);
   }
   
   ns=convert_to_ns(&start_before,&stop_after);
   
   printf("start/stop/read latency: %lld ns\n",ns);

   if (ret2<0) {
      printf("Error stopping!\n");
      exit(1);
   }

   s=0.0;
   for(i=0;i<MATRIX_SIZE;i++) {
     for(j=0;j<MATRIX_SIZE;j++) {
       s+=c[i][j];
     }
   }

   printf("Validation: s=%f\n",s);


   for(i=0;i<count;i++) {
     printf("%x: ",events[i]);
     for(j=0;j<MATRIX_SIZE;j++) {
       printf("%lld ",results[j][i]);
     }
     printf("\n");
   }

   /* close */

   return 0;
}
