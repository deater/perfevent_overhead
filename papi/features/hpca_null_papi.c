/* by Vince Weaver, vweaver1@eecs.utk.edu                      */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <time.h>

#include "papiStdEventDefs.h"
#include "papi.h"


long long convert_to_ns(struct timespec *before,
			struct timespec *after) {

  long long seconds;
  long long ns;

  seconds=after->tv_sec - before->tv_sec;
  ns = after->tv_nsec - before->tv_nsec;

  ns = (seconds*1000000000ULL)+ns;

  return ns;
}


char core2_events[16][64]={
  "UNHALTED_CORE_CYCLES",         /* PAPI_TOT_CYC */
  "INSTRUCTIONS_RETIRED",         /* PAPI_TOT_INS */
  "BRANCH_INSTRUCTIONS_RETIRED",  /* PAPI_BR_INS  */
  "MISPREDICTED_BRANCH_RETIRED",  /* PAPI_BR_MSP  */
  "ITLB:MISSES",                  /* PAPI_TLB_IM  */
  "DTLB_MISSES:ANY",              /* PAPI_TLB_DM  */
  "L1I_READS",                    /* PAPI_L1_ICA  */
  "L1I_MISSES",                   /* PAPI_L1_ICM  */
  "L1D_ALL_REF",                  /* PAPI_L1_DCA  */
  "L1D_REPL",                     /* PAPI_L1_DCM  */
  "HW_INT_RCV",                   /* PAPI_HW_INT  */
  "FP_COMP_OPS_EXE",              /* PAPI_FP_OPS  */
  "INST_RETIRED:LOADS",           /* PAPI_LD_INS  */
  "INST_RETIRED:STORES",          /* PAPI_SR_INS  */
  "L2_RQSTS:SELF:ANY:MESI",       /* PAPI_L2_TCA  */
  "L2_LINES_IN:SELF:ANY",         /* PAPI_L2_TCM  */
};

char atom_events[16][64]={
  "UNHALTED_CORE_CYCLES",         /* PAPI_TOT_CYC */
  "INSTRUCTIONS_RETIRED",         /* PAPI_TOT_INS */
  "BRANCH_INSTRUCTIONS_RETIRED",  /* PAPI_BR_INS  */
  "MISPREDICTED_BRANCH_RETIRED",  /* PAPI_BR_MSP  */
  "ITLB:MISSES",                  /* PAPI_TLB_IM  */
  "DATA_TLB_MISSES:DTLB_MISS",    /* PAPI_TLB_DM  */
  "ICACHE:ACCESSES",              /* PAPI_L1_ICA  */
  "ICACHE:MISSES",                /* PAPI_L1_ICM  */
  "L1D_CACHE:LD",                 /* PAPI_L1_DCA  */
  "L2_RQSTS:SELF",                /* PAPI_L1_DCM  */
  "HW_INT_RCV",                   /* PAPI_HW_INT  */
  "SIMD_INST_RETIRED:ANY",        /* PAPI_FP_OPS  */
  "L2_LD:SELF:ANY:MESI",          /* PAPI_LD_INS  */
  "L2_ST:SELF:MESI",              /* PAPI_SR_INS  */
  "L2_LD:SELF:ANY:MESI",          /* PAPI_L2_TCA  */
  "L2_LINES_IN:SELF:ANY",         /* PAPI_L2_TCM  */
};

char amd10h_events[16][64]={
  "CPU_CLK_UNHALTED",             /* PAPI_TOT_CYC */
  "RETIRED_INSTRUCTIONS",         /* PAPI_TOT_INS */
  "RETIRED_BRANCH_INSTRUCTIONS",  /* PAPI_BR_INS  */
  "RETIRED_MISPREDICTED_BRANCH_INSTRUCTIONS",  /* PAPI_BR_MSP  */
  "L1_ITLB_MISS_AND_L2_ITLB_MISS:ALL",                  /* PAPI_TLB_IM  */
  "L1_DTLB_AND_L2_DTLB_MISS",    /* PAPI_TLB_DM  */
  "INSTRUCTION_CACHE_FETCHES",              /* PAPI_L1_ICA  */
  "INSTRUCTION_CACHE_MISSES",                /* PAPI_L1_ICM  */
  "DATA_CACHE_ACCESSES",                 /* PAPI_L1_DCA  */
  "DATA_CACHE_MISSES",                /* PAPI_L1_DCM  */
  "INTERRUPTS_TAKEN",                   /* PAPI_HW_INT  */
  "DISPATCHED_FPU:OPS_MULTIPLY:OPS_ADD",        /* PAPI_FP_OPS  */
  "DECODER_EMPTY",          /* PAPI_LD_INS  */ /* nope */
  "DISPATCH_STALLS",              /* PAPI_SR_INS  */ /* nope */
  "REQUESTS_TO_L2:ALL",          /* PAPI_L2_TCA  */
  "L2_CACHE_MISS:INSTRUCTIONS:DATA",         /* PAPI_L2_TCM  */
};

char amd0fh_events[16][64]={
  "CPU_CLK_UNHALTED",             /* PAPI_TOT_CYC */
  "RETIRED_INSTRUCTIONS",         /* PAPI_TOT_INS */
  "RETIRED_BRANCH_INSTRUCTIONS",  /* PAPI_BR_INS  */
  "RETIRED_MISPREDICTED_BRANCH_INSTRUCTIONS",  /* PAPI_BR_MSP  */
  "L1_ITLB_MISS_AND_L2_ITLB_MISS",                  /* PAPI_TLB_IM  */
  "L1_DTLB_AND_L2_DTLB_MISS",    /* PAPI_TLB_DM  */
  "INSTRUCTION_CACHE_FETCHES",              /* PAPI_L1_ICA  */
  "INSTRUCTION_CACHE_MISSES",                /* PAPI_L1_ICM  */
  "DATA_CACHE_ACCESSES",                 /* PAPI_L1_DCA  */
  "DATA_CACHE_MISSES",                /* PAPI_L1_DCM  */
  "INTERRUPTS_TAKEN",                   /* PAPI_HW_INT  */
  "DISPATCHED_FPU:OPS_MULTIPLY:OPS_ADD",        /* PAPI_FP_OPS  */
  "DECODER_EMPTY",          /* PAPI_LD_INS  */ /* nope */
  "DISPATCH_STALLS",              /* PAPI_SR_INS  */ /* nope */
  "REQUESTS_TO_L2:ALL",          /* PAPI_L2_TCA  */
  "L2_CACHE_MISS:INSTRUCTIONS:DATA",         /* PAPI_L2_TCM  */
};

char nehalem_events[16][64]={
  "UNHALTED_CORE_CYCLES",         /* PAPI_TOT_CYC */
  "INSTRUCTIONS_RETIRED",         /* PAPI_TOT_INS */
  "BR_INST_EXEC:ANY",             /* PAPI_BR_INS  */
  "BR_MISP_EXEC:ANY",             /* PAPI_BR_MSP  */
  "ITLB_MISSES:ANY",              /* PAPI_TLB_IM  */
  "DTLB_MISSES:ANY",              /* PAPI_TLB_DM  */
  "L1I:READS",                    /* PAPI_L1_ICA  */
  "L1I:MISSES",                   /* PAPI_L1_ICM  */
  "L1D_ALL_REF:ANY",              /* PAPI_L1_DCA  */
  "L1D:REPL",                     /* PAPI_L1_DCM  */
  "HW_INT:RCV",                   /* PAPI_HW_INT  */
  "FP_COMP_OPS_EXE:SSE_FP",       /* PAPI_FP_OPS  */
  "MEM_INST_RETIRED:LOADS",       /* PAPI_LD_INS  */
  "MEM_INST_RETIRED:STORES",      /* PAPI_SR_INS  */
  "L2_RQSTS:REFERENCES",          /* PAPI_L2_TCA  */
  "L2_RQSTS:PREFETCHES",          /* PAPI_L2_TCM  */
};

char power6_events[16][64]={
  "PM_RUN_CYC",         /* PAPI_TOT_CYC */
  "PM_INST_CMPL",         /* PAPI_TOT_INS */
  "PM_BRU_FIN",             /* PAPI_BR_INS  */
  "PM_BR_MPRED",             /* PAPI_BR_MSP  */
  "PM_DATA_FROM_L2MISS",              /* PAPI_TLB_IM  */  /* nope */
  "PM_DATA_FROM_L2MISS",              /* PAPI_TLB_DM  */  /* nope */
  "PM_ST_REF_L1",                    /* PAPI_L1_ICA  */ /* nope */
  "PM_L1_ICACHE_MISS",                   /* PAPI_L1_ICM  */
  "PM_LD_REF_L1",              /* PAPI_L1_DCA  */  /* nope */
  "PM_LD_MISS_L1",                     /* PAPI_L1_DCM  */  /* nope */
  "PM_EXT_INT",                   /* PAPI_HW_INT  */
  "PM_FPU_FLOP",       /* PAPI_FP_OPS  */
  "PM_LD_REF_L1",       /* PAPI_LD_INS  */
  "PM_ST_REF_L1",      /* PAPI_SR_INS  */
  "PM_LD_REF_L1",          /* PAPI_L2_TCA  */ /* nope */
  "PM_DATA_FROM_L2MISS",          /* PAPI_L2_TCM  */ /* nope */
};

char cortexA9_events[16][64]={
  "CPU_CYCLES",                    /* PAPI_TOT_CYC */
  "INST_OUT_OF_RENAME_STAGE",      /* PAPI_TOT_INS */
  "PC_WRITE",                      /* PAPI_BR_INS  */
  "PC_BRANCH_MIS_PRED",            /* PAPI_BR_MSP  */
  "ITLB_MISS",                     /* PAPI_TLB_IM  */
  "DTLB_REFILL",                   /* PAPI_TLB_DM  */
  "MAIN_UNIT_EXECUTED_INST",       /* PAPI_L1_ICA  */ /* nope */
  "IFETCH_MISS",                   /* PAPI_L1_ICM  */
  "DCACHE_ACCESS",                 /* PAPI_L1_DCA  */
  "DCACHE_REFILL",                 /* PAPI_L1_DCM  */
  "EXT_INTERRUPTS",                /* PAPI_HW_INT  */
  "FP_EXECUTED_INST",              /* PAPI_FP_OPS  */
  "DREAD",                         /* PAPI_LD_INS  */
  "DWRITE",                        /* PAPI_SR_INS  */
  "DCACHE_ACCESS",                 /* PAPI_L2_TCA  */  /* nope */
  "DCACHE_REFILL",                 /* PAPI_L2_TCM  */  /* nope */
};


char event_names[16][64];

int main(int argc, char **argv) {

   int retval;

   int i;

   int EventSet=PAPI_NULL;

   int event;
   long long *counts;
   
   struct timespec before,after;
   struct timespec start_before;
   struct timespec stop_after;

   long long init_ns=0,eventset_ns=0;
   long long ns=0;

   int count;

   char *machine_name;

   if (argc>2) {
     count=atoi(argv[2]);
     machine_name=strdup(argv[1]);
   }
   else {
     count=1;
     machine_name=strdup("core2");
   }
   counts=calloc(count,sizeof(long long));

   if (!strncmp(machine_name,"core2",5)) {
      memcpy(event_names,core2_events,16*64);
   }
   else if (!strncmp(machine_name,"nehalem",7)) {
      memcpy(event_names,nehalem_events,16*64);
   }
   else if (!strncmp(machine_name,"atom",4)) {
      memcpy(event_names,atom_events,16*64);
   }
   else if (!strncmp(machine_name,"amd0fh",6)) {
      memcpy(event_names,amd0fh_events,16*64);
   }
   else if (!strncmp(machine_name,"amd10h",6)) {
      memcpy(event_names,amd10h_events,16*64);
   }
   else if (!strncmp(machine_name,"power6",6)) {
      memcpy(event_names,power6_events,16*64);
   }
   else if (!strncmp(machine_name,"cortexA9",8)) {
     memcpy(event_names,cortexA9_events,16*64);
   }
   else {
      fprintf(stderr,"Unknown machine name %s\n",machine_name);
      exit(0);
   }
   
   /* measure init latency */

   clock_gettime(CLOCK_REALTIME,&before);

   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT) {
      fprintf(stderr,"ERROR: PAPI_library_init %d\n", retval);
   }

   clock_gettime(CLOCK_REALTIME,&after);

   init_ns=convert_to_ns(&before,&after);

   printf("PAPI_library_init_latency: %lld ns\n",init_ns);

   /* measure eventset creation */

   clock_gettime(CLOCK_REALTIME,&before);

   retval = PAPI_create_eventset(&EventSet);
   if (retval!=PAPI_OK) {
     fprintf(stderr,"PAPI_create_eventset failed\n");
   }

   for(i=0;i<count;i++) {
      retval=PAPI_event_name_to_code(event_names[i],&event);
      if (retval!=PAPI_OK) {
	fprintf(stderr,"PAPI_event_name_to_code failed %s\n",event_names[i]);
      }

      retval=PAPI_add_event(EventSet,event);
      if (retval!=PAPI_OK) {
	//fprintf(stderr,"PAPI_add_event failed %x\n",event);
	 count=i;
	 printf("Eventset_creation_%d: %lld ns\n",i,0LL);
	 printf("PAPI_start/stop/read latency: %lld ns\n",0LL);
	 exit(1);
      }
   }

   clock_gettime(CLOCK_REALTIME,&after);

   eventset_ns=convert_to_ns(&before,&after);

   printf("Eventset_creation_%d: %lld ns\n",i,eventset_ns);

   clock_gettime(CLOCK_REALTIME,&start_before);
   PAPI_start(EventSet);
   
   PAPI_stop(EventSet,counts);
   clock_gettime(CLOCK_REALTIME,&stop_after);

   ns=convert_to_ns(&start_before,&stop_after);

   printf("PAPI_start/stop/read latency: %lld ns\n",ns);

   for(i=0;i<count;i++) {
     printf("%s %lld\n",event_names[i],counts[i]);
   }

   PAPI_shutdown();
   
   return 0;
}
