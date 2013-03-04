#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#define NUM_EVENTS 8

#include "version.h"

#define VENDOR_UNKNOWN 0
#define VENDOR_AMD     1
#define VENDOR_INTEL   2

#define KERNEL_PERF_EVENT       0
#define KERNEL_PERF_EVENT_RDPMC 1
#define KERNEL_PERFCTR          2
#define KERNEL_PERFMON2         3
#define KERNEL_UNKNOWN         99

static char dirname[BUFSIZ];

struct cpuinfo_t {
  int vendor;
  int family;
  int model;
  int stepping;
  char modelname[BUFSIZ];
  char generic_modelname[BUFSIZ];
} cpuinfo;

struct event_type_t {
  unsigned int event;
  char *event_name;
  char *papi_name;
};

#define MAX_EVENTS 16

struct event_table_t {
  struct event_type_t event[MAX_EVENTS];
} *event_table=NULL;

struct event_table_t fam10h_event_table = { {
  { 0x530076, "CPU_CLK_UNHALTED",		"PAPI_TOT_CYC", },
  { 0x5300c0, "RETIRED_INSTRUCTIONS",		"PAPI_TOT_INS", },
  { 0x5300c2, "RETIRED_BRANCH_INSTRUCTIONS",	"PAPI_BR_INS"  },
  { 0x5300c3, "RETIRED_MISPREDICTED_BRANCH_INSTRUCTIONS", "PAPI_BR_MSP" },
  { 0x530385, "L1_ITLB_MISS_AND_L2_ITLB_MISS:ALL", "PAPI_TLB_IM" },
  { 0x530746, "L1_DTLB_AND_L2_DTLB_MISS",	"PAPI_TLB_DM" },
  { 0x530080, "INSTRUCTION_CACHE_FETCHES",	"PAPI_L1_ICA" },
  { 0x530081, "INSTRUCTION_CACHE_MISSES",	"PAPI_L1_ICM" },
  { 0x530040, "DATA_CACHE_ACCESSES",		"PAPI_L1_DCA" },
  { 0x530041, "DATA_CACHE_MISSES",		"PAPI_L1_DCM" },
  { 0x5300cf, "INTERRUPTS_TAKEN",		"PAPI_HW_INT" },
  { 0x530300, "DISPATCHED_FPU:OPS_MULTIPLY:OPS_ADD", "PAPI_FP_OPS" },
  { 0x5300d0, "DECODER_EMPTY",			"PAPI_LD_INS" }, /* nope */
  { 0x5300d1, "DISPATCH_STALLS",		"PAPI_SR_INS" }, /* nope */
  { 0x533f7d, "REQUESTS_TO_L2:ALL",		"PAPI_L2_TCA" },
  { 0x53037e, "L2_CACHE_MISS:INSTRUCTIONS:DATA","PAPI_L2_TCM" },
}};

struct event_table_t atom_event_table = {{
  { 0x53003c, "UNHALTED_CORE_CYCLES",		"PAPI_TOT_CYC", },
  { 0x5300c0, "INSTRUCTIONS_RETIRED",		"PAPI_TOT_INS", },
  { 0x5300c4, "BRANCH_INSTRUCTIONS_RETIRED",	"PAPI_BR_INS",  },
  { 0x5300c5, "MISPREDICTED_BRANCH_RETIRED",	"PAPI_BR_MSP",  },
  { 0x530282, "ITLB:MISSES",			"PAPI_TLB_IM",  },
  { 0x530708, "DATA_TLB_MISSES:DTLB_MISS",	"PAPI_TLB_DM",  },
  { 0x530380, "ICACHE:ACCESSES",		"PAPI_L1_ICA",  },
  { 0x530280, "ICACHE:MISSES",			"PAPI_L1_ICM",  },
  { 0x532140, "L1D_CACHE:LD",			"PAPI_L1_DCA",  },
  { 0x537f2e, "L2_RQSTS:SELF",			"PAPI_L1_DCM",  },
  { 0x5301c8, "HW_INT_RCV",			"PAPI_HW_INT",  },
  { 0x531fc7, "SIMD_INST_RETIRED:ANY",		"PAPI_FP_OPS",  },
  { 0x537f29, "L2_LD:SELF:ANY:MESI",		"PAPI_LD_INS",  },
  { 0x534f2a, "L2_ST:SELF:MESI",		"PAPI_SR_INS",  },
  { 0x537f29, "L2_LD:SELF:ANY:MESI",		"PAPI_L2_TCA",  },
  { 0x537024, "L2_LINES_IN:SELF:ANY",		"PAPI_L2_TCM",  },
}};

struct event_table_t core2_event_table = {{
  { 0x53003c, "UNHALTED_CORE_CYCLES",		"PAPI_TOT_CYC", },
  { 0x5300c0, "INSTRUCTIONS_RETIRED",		"PAPI_TOT_INS", },
  { 0x5300c4, "BRANCH_INSTRUCTIONS_RETIRED",	"PAPI_BR_INS",  },
  { 0x5300c5, "MISPREDICTED_BRANCH_RETIRED",	"PAPI_BR_MSP",  },
  { 0x531282, "ITLB:MISSES",			"PAPI_TLB_IM",  },
  { 0x530108, "DTLB_MISSES:ANY",		"PAPI_TLB_DM",  },
  { 0x530080, "L1I_READS",			"PAPI_L1_ICA",  },
  { 0x530081, "L1I_MISSES",			"PAPI_L1_ICM",  },
  { 0x530143, "L1D_ALL_REF",			"PAPI_L1_DCA",  },
  { 0x530f45, "L1D_REPL",			"PAPI_L1_DCM",  },
  { 0x5300c8, "HW_INT_RCV",			"PAPI_HW_INT",  },
  { 0x530010, "FP_COMP_OPS_EXE",		"PAPI_FP_OPS",  },
  { 0x5301c0, "INST_RETIRED:LOADS",		"PAPI_LD_INS",  },
  { 0x5302c0, "INST_RETIRED:STORES",		"PAPI_SR_INS",  },
  { 0x537f2e, "L2_RQSTS:SELF:ANY:MESI",		"PAPI_L2_TCA",  },
  { 0x537024, "L2_LINES_IN:SELF:ANY",		"PAPI_L2_TCM",  },
}};

struct event_table_t nhm_event_table = {{
  { 0x53003c, "UNHALTED_CORE_CYCLES",		"PAPI_TOT_CYC", },
  { 0x5300c0, "INSTRUCTIONS_RETIRED",		"PAPI_TOT_INS", },
  { 0x537f88, "BR_INST_EXEC:ANY",		"PAPI_BR_INS",  },
  { 0x537f89, "BR_MISP_EXEC:ANY",		"PAPI_BR_MSP",  },
  { 0x530185, "ITLB_MISSES:ANY",		"PAPI_TLB_IM",  },
  { 0x530149, "DTLB_MISSES:ANY",		"PAPI_TLB_DM",  },
  { 0x530380, "L1I:READS",			"PAPI_L1_ICA",  },
  { 0x530280, "L1I:MISSES",			"PAPI_L1_ICM",  },
  { 0x530143, "L1D_ALL_REF:ANY",		"PAPI_L1_DCA",  },
  { 0x530151, "L1D:REPL",			"PAPI_L1_DCM",  },
  { 0x53011d, "HW_INT:RCV",			"PAPI_HW_INT",  },
  { 0x530410, "FP_COMP_OPS_EXE:SSE_FP",		"PAPI_FP_OPS",  },
  { 0x53010b, "MEM_INST_RETIRED:LOADS",		"PAPI_LD_INS",  },
  { 0x53020b, "MEM_INST_RETIRED:STORES",	"PAPI_SR_INS",  },
  { 0x53ff24, "L2_RQSTS:REFERENCES",		"PAPI_L2_TCA",  },
  { 0x53c024, "L2_RQSTS:PREFETCHES",		"PAPI_L2_TCM",  },
}};

struct event_table_t ivb_event_table = {{
  { 0x53003c, "UNHALTED_CORE_CYCLES",		"PAPI_TOT_CYC", },
  { 0x5300c0, "INSTRUCTION_RETIRED",		"PAPI_TOT_INS", },
  { 0x5300c4, "BR_INST_RETIRED:ALL_BRANCHES",	"PAPI_BR_INS",  },
  { 0x5300c5, "BR_MISP_RETIRED:ALL_BRANCHES",	"PAPI_BR_MSP",  },
  { 0x530185, "ITLB_MISSES:CAUSES_A_WALK",	"PAPI_TLB_IM",  },
  { 0x538108, "DTLB_LOAD_MISSES:DEMAND_LD_MISS_CAUSES_A_WALK","PAPI_TLB_DM",  },
  { 0x530f27, "L2_STORE_LOCK_RQSTS:ALL",	"PAPI_L1_ICA",  }, /* nope */
  { 0x530280, "ICACHE:MISSES",			"PAPI_L1_ICM",  },
  { 0x530324, "L2_RQSTS:ALL_DEMAND_DATA_RD",	"PAPI_L1_DCA",  }, /* nope */
  { 0x530151, "L1D:REPLACEMENT",		"PAPI_L1_DCM",  },
  { 0x53011d, "HW_INTERRUPTS",			"PAPI_HW_INT",  }, /* nope */
  { 0x1570114,"ARITH:FPU_DIV",			"PAPI_FP_OPS",  }, /* nope */
  { 0x5381d0, "MEM_UOP_RETIRED:ANY_LOADS",	"PAPI_LD_INS",  },
  { 0x5382d0, "MEM_UOP_RETIRED:ANY_STORES",	"PAPI_SR_INS",  },
  { 0x530151, "L1D:REPLACEMENT",		"PAPI_L2_TCA",  },
  { 0x534f2e, "LAST_LEVEL_CACHE_REFERENCES",	"PAPI_L2_TCM",  },
}};


#if 0
struct event_table_t power6_event_table = {{
  0x10000a, // "PM_RUN_CYC",            /* PAPI_TOT_CYC */
  0x2,      // "PM_INST_CMPL",          /* PAPI_TOT_INS */
  0x430e6,  // "PM_BRU_FIN",                    /* PAPI_BR_INS  */
  0x400052, // "PM_BR_MPRED",                   /* PAPI_BR_MSP  */
  0x2000fe, // "PM_DATA_FROM_L2MISS",        /* PAPI_TLB_IM  */  /* nope */
  0x2000fe, // "PM_DATA_FROM_L2MISS",        /* PAPI_TLB_DM  */  /* nope */
  0x80086,  // "PM_ST_REF_L1",               /* PAPI_L1_ICA  */ /* nope */
  0x100056, // "PM_L1_ICACHE_MISS",          /* PAPI_L1_ICM  */
  0x80082,  // "PM_LD_REF_L1",               /* PAPI_L1_DCA  */  /* nope */
  0x80080, // "PM_LD_MISS_L1",              /* PAPI_L1_DCM  */  /* nope */
  0x2000f8, // "PM_EXT_INT",                 /* PAPI_HW_INT  */
  0x1c0032, // "PM_FPU_FLOP",                   /* PAPI_FP_OPS  */
  0x80082,  // "PM_LD_REF_L1",          /* PAPI_LD_INS  */
  0x80086,   // "PM_ST_REF_L1",                 /* PAPI_SR_INS  */
  0x80082,  // "PM_LD_REF_L1",                  /* PAPI_L2_TCA  */ /* nope */
  0x2000fe, // "PM_DATA_FROM_L2MISS",        /* PAPI_L2_TCM  */ /* nope */
}};

struct event_table_t cortexA9_event_table = {{
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
}};
#endif

static int set_generic_modelname(int vendor, int family, int model) {

  if (vendor==VENDOR_AMD) {
    /* 0fh */
    if (family==15) {
      strcpy(cpuinfo.generic_modelname,"opteron");
      event_table=&fam10h_event_table;
    } 
    /* 10h */
    else if (family==16) {
      strcpy(cpuinfo.generic_modelname,"fam10h");
      event_table=&fam10h_event_table;
    }
    /* 14h */
    else if (family==20) {
      strcpy(cpuinfo.generic_modelname,"bobcat");
      //event_table=&bobcat_event_table;
    }
    /* 15h */
    else if (family==21) {
      strcpy(cpuinfo.generic_modelname,"bulldozer");
      event_table=NULL; /* TODO */
    }
    /* 16h */
    else if (family==22) {
      strcpy(cpuinfo.generic_modelname,"piledriver");
      event_table=NULL; /* TODO */
    } else {
      strcpy(cpuinfo.generic_modelname,"UNKNOWN");
    }
    return 0;
  }

  if (vendor==VENDOR_INTEL) {
    if (family==15) {
      switch(model) {
      case 0:
      case 1:
      case 2:
	strcpy(cpuinfo.generic_modelname,"pentium4");
	//event_table=&pentium4_event_table;
	break;
      case 3:
      case 4:
      case 6:
	strcpy(cpuinfo.generic_modelname,"pentiumd");
	//event_table=&pentium4_event_table;
	break;
      default:
	strcpy(cpuinfo.generic_modelname,"UNKNOWN");
	break;
      }
    }
    else if (family==11) {
      /* MIC */
      strcpy(cpuinfo.generic_modelname,"UNKNOWN");
    }
    else if (family==6) {
      switch(model) {

      case 14: /* yonah */
	strcpy(cpuinfo.generic_modelname,"UNKNOWN");
	break;

      case 15: /* Memrom/Conroe */
      case 22: /* Memrom-L/Conroe-L */
      case 23: /* Penryn/Wolfdale */
      case 29: /* Dunnington */
	strcpy(cpuinfo.generic_modelname,"core2");
	event_table=&core2_event_table;
	break;

      case 26: /* Bloomfield */
      case 30: /* Lynnfield */
	/* Nehalem */
	strcpy(cpuinfo.generic_modelname,"nehalem");
	event_table=&nhm_event_table;
	break;

      case 46: /* Beckton */
	/* Nehalem-EX */
	strcpy(cpuinfo.generic_modelname,"nehalem-ex");
	event_table=&nhm_event_table;
	break;

      case 28: /* Atom */
      case 38: /* Lincroft */
      case 39: /* Penwell */
      case 53: /* Cloverview */
	strcpy(cpuinfo.generic_modelname,"atom");
	event_table=&atom_event_table;
	break;

      case 54: /* Cedarview */
	strcpy(cpuinfo.generic_modelname,"atom-cedarview");
	event_table=&atom_event_table;
	break;

      case 37: /* Clarkdale */
      case 44: /* Gulftown */
	strcpy(cpuinfo.generic_modelname,"westmere");
	//event_table=&wsm_event_table;
	break;

      case 47: /* Xeon E7 */
	strcpy(cpuinfo.generic_modelname,"westmere-ex");
	//event_table=&wsm_event_table;
	break;

      case 42: /* Sandybridge */
	strcpy(cpuinfo.generic_modelname,"sandybridge");
	//event_table=&snb_event_table;
	break;
      case 45: /* Sandybridge EP (Romley) */
	strcpy(cpuinfo.generic_modelname,"sandybridge-ep");
	//event_table=&wsm_event_table;
	break;

      case 58: /* Ivy Bridge */
	strcpy(cpuinfo.generic_modelname,"ivybridge");
	event_table=&ivb_event_table;
	break;
      case 62: /* Ivy Bridge EP */
	strcpy(cpuinfo.generic_modelname,"ivybridge-ep");
	event_table=&ivb_event_table;
	break;

      default: strcpy(cpuinfo.generic_modelname,"UNKNOWN"); break;
      }
      return 0;
    } else {
      strcpy(cpuinfo.generic_modelname,"UNKNOWN");
    }
    return 0;
  }

  strcpy(cpuinfo.generic_modelname,"UNKNOWN");
  return -1;
}


static int get_cpuinfo(void) {

  FILE *fff;
  char temp_string[BUFSIZ],temp_string2[BUFSIZ];
  char *result;

  fff=fopen("/proc/cpuinfo","r");
  if (fff==NULL) return -1;

  while(1) {
    result=fgets(temp_string,BUFSIZ,fff);
    if (result==NULL) break;

    if (!strncmp(temp_string,"vendor_id",9)) {
      sscanf(temp_string,"%*s%*s%s",temp_string2);

      if (!strncmp(temp_string2,"GenuineIntel",12)) {
	cpuinfo.vendor=VENDOR_INTEL;
      } else
	if (!strncmp(temp_string2,"AuthenticAMD",12)) {
	  cpuinfo.vendor=VENDOR_AMD;
	}
	else {
	  cpuinfo.vendor=VENDOR_UNKNOWN;
	}
    }
    if (!strncmp(temp_string,"cpu family",10)) {
      sscanf(temp_string,"%*s%*s%*s%d",&cpuinfo.family);
    }

    if (!strncmp(temp_string,"model",5)) {
      sscanf(temp_string,"%*s%s",temp_string2);
      if (temp_string2[0]==':') {
	sscanf(temp_string,"%*s%*s%d",&cpuinfo.model);
      } else {
	result=strstr(temp_string,":");
	strcpy(cpuinfo.modelname,result+2);
	cpuinfo.modelname[strlen(cpuinfo.modelname)-1]=0;
      }

    }
    if (!strncmp(temp_string,"stepping",8)) {
      sscanf(temp_string,"%*s%*s%d",&cpuinfo.stepping);
    }

  }


  set_generic_modelname(cpuinfo.vendor,cpuinfo.family,cpuinfo.model);

  printf("Family:    %d\n",cpuinfo.family);
  printf("Model:     %d\n",cpuinfo.model);
  printf("Stepping:  %d\n",cpuinfo.stepping);
  printf("Modelname: %s\n",cpuinfo.modelname);
  printf("Generic:   %s\n",cpuinfo.generic_modelname);

  fclose(fff);

  return 0;
}


static int create_output_dir(void) {

  int result,next_avail;

  result=mkdir("results",0755);
  if (result<0) {
    if (errno==EEXIST) {
      /* this is OK */
    }
    else {
      fprintf(stderr,"ERROR creating results dir!\n");
      return -1;
    }
  }

  get_cpuinfo();

  sprintf(dirname,"results/%s",cpuinfo.generic_modelname);

  result=mkdir(dirname,0755);
  if (result<0) {
    if (errno==EEXIST) {
      /* this is OK */
    }
    else {
      fprintf(stderr,"ERROR creating %s dir!\n",dirname);
      return -1;
    }
  }
  next_avail=0;
  while(1) {

    sprintf(dirname,"results/%s/%d",cpuinfo.generic_modelname,next_avail);

    result=mkdir(dirname,0755);
    if (result<0) {
      if (errno==EEXIST) {
	/* already there move on */
      }
      else {
	fprintf(stderr,"ERROR creating %s dir!\n",dirname);
	return -1;
      }

    } else {
      printf("Found available directory: %s\n",dirname);
      return 0;
    }

    next_avail++;
  }

  return 0;
}

#define NUM_RUNS 1024

static int generate_results(char *directory, int type, int num) {

   int i,result;
   char dirname[BUFSIZ],filename[BUFSIZ],temp_string[BUFSIZ],temp_string2[BUFSIZ];
   FILE *fff;
   struct utsname uname_buf;

   uname(&uname_buf);

   sprintf(dirname,"%s/%s",directory,uname_buf.release);
   result=mkdir(dirname,0755);
   if (result<0) {
      if (errno==EEXIST) {
         /* this is OK */
      }
      else {
         fprintf(stderr,"Can't create directory %s\n",dirname);
         return -1;
      }
   }

   sprintf(dirname,"%s/%d",dirname,num);
   result=mkdir(dirname,0755);
   if (result<0) {
      fprintf(stderr,"Can't create directory %s\n",dirname);
      return -1;
   }

   sprintf(filename,"%s/results",dirname);
   printf("Generating file %s\n",filename);

   fff=fopen(filename,"w");
   if (fff==NULL) return -1;
   fprintf(fff,"### System info\n");
   fprintf(fff,"Kernel:    %s %s\n",uname_buf.sysname,uname_buf.release);
   fprintf(fff,"Interface: perf_event\n");
   fprintf(fff,"Hostname:  %s\n",uname_buf.nodename);
   fprintf(fff,"Family:    %d\n",cpuinfo.family);
   fprintf(fff,"Model:     %d\n",cpuinfo.model);
   fprintf(fff,"Stepping:  %d\n",cpuinfo.stepping);
   fprintf(fff,"Modelname: %s\n",cpuinfo.modelname);
   fprintf(fff,"Generic:   %s\n",cpuinfo.generic_modelname);
   for(i=0;i<num;i++) {
      fprintf(fff,"Event %d %x %s\n",i,
             event_table->event[i].event,
             event_table->event[i].event_name);
   }
   fprintf(fff,"generate_results version: %s\n",PERF_EVENT_OVERHEAD_VERSION);
   fprintf(fff,"Runs:      %d\n",NUM_RUNS);
   fclose(fff);

   if (type==KERNEL_PERF_EVENT) {
      sprintf(temp_string2,"taskset 0x1 ./rdtsc_null_pe %d ",num);
      for(i=0;i<num;i++) {
         sprintf(temp_string,"0x%x ",event_table->event[i].event);
         strcat(temp_string2,temp_string);
      }
   }
   else if (type==KERNEL_PERF_EVENT_RDPMC) {
      sprintf(temp_string2,"taskset 0x1 ./rdtsc_null_pe_rdpmc %d ",num);
      for(i=0;i<num;i++) {
         sprintf(temp_string,"0x%x ",event_table->event[i].event);
         strcat(temp_string2,temp_string);
      }
   }
   else if (type==KERNEL_PERFCTR) {
      sprintf(temp_string2,"taskset 0x1 ./rdtsc_null_perfctr %d ",num);
      for(i=0;i<num;i++) {
         sprintf(temp_string,"0x%x ",event_table->event[i].event);
         strcat(temp_string2,temp_string);
      }
   }
   else if (type==KERNEL_PERFMON2) {
      sprintf(temp_string2,"taskset 0x1 ./rdtsc_null_perfmon2 %d ",num);
      for(i=0;i<num;i++) {
         sprintf(temp_string,"%s ",event_table->event[i].event_name);
         strcat(temp_string2,temp_string);
      }
   }

   sprintf(temp_string2,"%s >> %s",temp_string2,filename);

   for(i=0;i<NUM_RUNS;i++) {
      sprintf(temp_string,"echo \"### Perf Results %d\">> %s",
                           i,filename);
      system(temp_string);
 //printf("Running %s\n",temp_string2);
      system(temp_string2);
   }
   printf("\n");

   return 0;
}



int main(int argc, char **argv) {

  int i;
  int type=KERNEL_PERF_EVENT;

  create_output_dir();

  if (event_table==NULL) {
     fprintf(stderr,"Error!  Unknown machine type!\n");
     return -1;
  }

  if (argc>1) {
     if (!strncmp(argv[1],"perf_event_rdpmc",16)) {
        type=KERNEL_PERF_EVENT_RDPMC;
     }
     else if (!strncmp(argv[1],"perf_event",10)) {
        type=KERNEL_PERF_EVENT;
     }
     else if (!strncmp(argv[1],"perfctr",7)) {
        type=KERNEL_PERFCTR;
     }
     else if (!strncmp(argv[1],"perfmon",7)) {
        type=KERNEL_PERFMON2;
     }
     else {
        type=KERNEL_UNKNOWN;
	fprintf(stderr,"Error!  Unknown kernel type %s\n",argv[1]);
	return -1;
     }
  }

  for(i=0;i<NUM_EVENTS;i++) {
     generate_results(dirname,type,i);
  }

  return 0;
}
