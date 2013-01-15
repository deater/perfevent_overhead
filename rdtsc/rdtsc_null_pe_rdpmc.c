/* by Vince Weaver, vweaver1@eecs.utk.edu                                  */
/* Compile with gcc -O2 -o rdtsc_null_pe_rdpmc rdtsc_null_pe_rdpmc.c       */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/utsname.h>

#include <time.h>

#include "include/perf_event.h"
#include <unistd.h>
#include <asm/unistd.h>

#include <sys/ioctl.h>
#include <errno.h>

#include <sys/mman.h>

#ifndef __NR_perf_event_open

#if defined(__i386__)
#define __NR_perf_event_open    336
#elif defined(__x86_64__) 
#define __NR_perf_event_open    298
#elif defined __powerpc__
#define __NR_perf_event_open    319
#elif defined __arm__
#define __NR_perf_event_open    364
#endif
#endif


int perf_event_open(struct perf_event_attr *hw_event_uptr,
                    pid_t pid, int cpu, int group_fd, unsigned long flags) {
   
  return syscall(__NR_perf_event_open,hw_event_uptr, pid, cpu,
                 group_fd, flags);
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

static unsigned long long rdtsc(void) {
  unsigned a,d;

  __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

  return ((unsigned long long)a) | (((unsigned long long)d) << 32);
}

static unsigned long long rdpmc(unsigned int counter) {
  unsigned int low, high;

  __asm__ volatile("rdpmc" : "=a" (low), "=d" (high) : "c" (counter));

  return (unsigned long long)low | ((unsigned long long)high) <<32;
}

#define barrier() __asm__ volatile("" ::: "memory")

#if 0
/* From Peter Zjilstra's demo code */
static unsigned long long mmap_read_self(void *addr, 
					 unsigned long long *enabled,
					 unsigned long long *running)
{
  struct perf_event_mmap_page *pc = addr;
  unsigned int seq;
  unsigned long long count, delta;

  do {
  again:
    seq=pc->lock;
    barrier();
    if (seq&1) goto again;

    if ((enabled || running) && pc->time_mult) {
      unsigned long long cyc = rdtsc();
      unsigned long long rem, quot;

      quot=(cyc >> pc->time_shift);
      rem = cyc & ((1 << pc->time_shift) -1);
      delta=pc->time_offset + 
	quot*pc->time_mult +
	((rem * pc->time_mult) >> pc->time_shift);
    }
    if (enabled) *enabled=pc->time_enabled+delta;
    if (running) *running=pc->time_running+delta;

    if (pc->index) {
      count=rdpmc(pc->index-1);
      count+=pc->offset;
    }
    else goto fail;

    barrier();
  } while (pc->lock != seq);

  return count;

 fail:
  /* should do slow read here */
  printf("FAIL FAIL FAIL\n");
  return 0;

}

#endif

/* Ingo's code */
static inline unsigned long long mmap_read_self(void *addr) {
  
    struct perf_event_mmap_page *pc = addr;
    unsigned int seq,idx;

    unsigned long long count;

    do {
      seq=pc->lock;
      barrier();

      idx=pc->index;
      count=pc->offset;

      if (idx) {
	count+=rdpmc(pc->index-1);
      }
      barrier();
    } while (pc->lock != seq);
  

  return count;
}



int main(int argc, char **argv) {

   int i;
   static long page_size=4096;
   
   long long before,after;
   long long start_before,start_after;
   long long stop_before,stop_after;
   long long read_before,read_after;

   long long init_ns=0,eventset_ns=0;
   void *addr[MAX_EVENTS];

   struct perf_event_attr pe;
   int fd[MAX_EVENTS],ret1,ret2,ret3;

   int count;

   char *machine_name;
   char kernel_name[BUFSIZ];
   struct utsname uname_info;
   unsigned long long now[MAX_EVENTS],stamp[MAX_EVENTS];
   unsigned long long now2[MAX_EVENTS],stamp2[MAX_EVENTS];

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

   uname(&uname_info);
   strncpy(kernel_name,uname_info.release,BUFSIZ);

   printf("Kernel: %s\n",kernel_name);
   
   /* measure init latency */

   before=rdtsc();

   after=rdtsc();

   init_ns=after-before;

   printf("init_latency: %lld cycles\n",init_ns);

   /* measure eventset creation */

   before=rdtsc();

   memset(&pe,0,sizeof(struct perf_event_attr));

   pe.type=PERF_TYPE_RAW;
   pe.size=sizeof(struct perf_event_attr);
   pe.read_format=PERF_FORMAT_GROUP|PERF_FORMAT_ID;

   fd[0]=-1;

   for(i=0;i<count;i++) {
     pe.config=events[i];

     if (i==0) {
        pe.disabled=1;
        pe.pinned=1;
     }
     else {
        pe.disabled=0;
        pe.pinned=0;
     }

     fd[i]=perf_event_open(&pe,0,-1,fd[0],0);
     if (fd[i]<0) {
	 count=i;
	 printf("Eventset_creation_%d: %lld cycles\n",i,0LL);
	 printf("start/stop/read latency: %lld cycles\n",0LL);
	 exit(1);
      }
   

      addr[i]=mmap(NULL,page_size, PROT_READ, MAP_SHARED,fd[i],0);
      if (addr[i] == (void *)(-1)) {
        printf("Error mmaping!\n");
        exit(1);
      }
   }

   after=rdtsc();

   eventset_ns=after-before;

   printf("Eventset_creation_%d: %lld cycles\n",i,eventset_ns);


   /* start */
   start_before=rdtsc();

   ret1=ioctl(fd[0], PERF_EVENT_IOC_ENABLE,0);

   start_after=rdtsc();

   /* read */


   for(i=0;i<count;i++) {
     stamp[i] = mmap_read_self(addr[i]);
   }

   /* NULL */

   for(i=0;i<count;i++) {
     now[i] = mmap_read_self(addr[i]);
   }

   read_after=rdtsc();

   /* stop */
   ret2=ioctl(fd[0], PERF_EVENT_IOC_DISABLE,0);

   stop_after=rdtsc();

   stop_before=read_after;
   read_before=start_after;

   if (ret3<0) {
      printf("start latency: %lld cycles\n",0LL);
      printf("stop latency: %lld cycles\n",0LL);
      printf("read latency: %lld cycles\n",0LL);
      printf("Unexpected read result %d %s\n",ret3,strerror(errno));
      exit(0);
   }
   
   printf("start latency: %lld cycles\n",start_after-start_before);
   printf("stop latency: %lld cycles\n",stop_after-stop_before);
   printf("read latency: %lld cycles\n",read_after-read_before);

   if (ret1<0) {
      printf("Error starting!\n");
      exit(1);
   }
   
   if (ret2<0) {
      printf("Error stopping!\n");
      exit(1);
   }
   
   
   for(i=0;i<count;i++) {
     printf("%x %lld\n",
	    events[i],now[i]-stamp[i]);
   }

   for(i=0;i<count;i++) {
      munmap(addr[i],page_size);
   }

   for(i=0;i<count;i++) {
      close(fd[i]);
   }

   return 0;
}
