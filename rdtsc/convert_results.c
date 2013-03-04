/* Convert results from separate files (huge diskspace) */
/* to all-in-one-file (much less disk space)            */

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

static char dirname[BUFSIZ];

static int create_output_dir(char *modelname) {

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

  sprintf(dirname,"results/%s",modelname);

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

    sprintf(dirname,"results/%s/%d",modelname,next_avail);

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

#define KERNEL_PERF_EVENT 0
#define KERNEL_PERFMON2   1
#define KERNEL_PERFCTR    2
#define KERNEL_PERF_EVENT_RDPMC 3

static int generate_results(char *directory, 
			    char *hostname,
			    char *kernel_name,
			    int kernel_type,
			    char *modelname,
			    int cpuinfo_family,
			    int cpuinfo_model,
			    int cpuinfo_stepping,
			    char *cpuinfo_modelname,
			    int num) {

   int i,result;
   char dirname[BUFSIZ],filename[BUFSIZ],temp_string[BUFSIZ];
   char *fresult;
   FILE *fff,*ggg;

   sprintf(dirname,"%s/%s",directory,kernel_name);
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
   fprintf(fff,"Kernel:    %s %s\n","Linux",kernel_name);
   fprintf(fff,"Interface: ");
   if (kernel_type==KERNEL_PERF_EVENT) fprintf(fff,"perf_event\n");
   else if (kernel_type==KERNEL_PERFMON2) fprintf(fff,"perfmon2\n");
   else if (kernel_type==KERNEL_PERFCTR) fprintf(fff,"perfctr\n");
   else if (kernel_type==KERNEL_PERF_EVENT_RDPMC) fprintf(fff,"perf_event_rdpmc\n");
   else fprintf(fff,"Unknown\n");
   fprintf(fff,"Hostname:  %s\n",hostname);
   fprintf(fff,"Family:    %d\n",cpuinfo_family);
   fprintf(fff,"Model:     %d\n",cpuinfo_model);
   fprintf(fff,"Stepping:  %d\n",cpuinfo_stepping);
   fprintf(fff,"Modelname: %s\n",cpuinfo_modelname);
   fprintf(fff,"Generic:   %s\n",modelname);
   fprintf(fff,"generate_results version: %s\n","0.5");
   fprintf(fff,"Runs:      %d\n",NUM_RUNS);

   for(i=0;i<NUM_RUNS;i++) {
      fprintf(fff,"### Perf Results %d\n",i);
      sprintf(temp_string,"old/results/rdtsc_null_raw/%s/%s/%d/null_test.%d",
	      modelname,kernel_name,num,i);
      ggg=fopen(temp_string,"r");
      if (ggg==NULL) {
	 fprintf(stderr,"Error opening %s!\n",temp_string);
	 continue;
      }
      while(1) {
	fresult=fgets(temp_string,BUFSIZ,ggg);
	if (fresult==NULL) break;
	fprintf(fff,"%s",temp_string);
      }

      fclose(ggg);
   }

   fclose(fff);
   return 0;
}


#define NUM_KERNELS 18

struct kernel_info {
  char name[64];
  int type;
} kernels[NUM_KERNELS]={
  {"2.6.32", KERNEL_PERF_EVENT},
  {"2.6.33", KERNEL_PERF_EVENT},
  {"2.6.34", KERNEL_PERF_EVENT},
  {"2.6.35", KERNEL_PERF_EVENT},
  {"2.6.36", KERNEL_PERF_EVENT},
  {"2.6.37", KERNEL_PERF_EVENT},
  {"2.6.38", KERNEL_PERF_EVENT},
  {"2.6.39", KERNEL_PERF_EVENT},
  {"3.0.0", KERNEL_PERF_EVENT},
  {"3.1.0", KERNEL_PERF_EVENT},
  {"3.2.0", KERNEL_PERF_EVENT},
  {"3.3.0", KERNEL_PERF_EVENT},
  {"3.4.0", KERNEL_PERF_EVENT},
  {"3.4.0-rdpmc", KERNEL_PERF_EVENT_RDPMC},
  {"3.5.0", KERNEL_PERF_EVENT},
  {"3.5.0-rdpmc", KERNEL_PERF_EVENT_RDPMC},
  {"2.6.30-perfmon2", KERNEL_PERFMON2},
  {"2.6.32-perfctr", KERNEL_PERFCTR},
};


int main(int argc, char **argv) {

  int i,k;

#if 0
  create_output_dir("core2");

  for(k=0;k<NUM_KERNELS;k++) {
     for(i=0;i<NUM_EVENTS;i++) {
        generate_results(dirname,
		      "deater.macbook",
		      kernels[k].name,
		      kernels[k].type,
		      "core2",
		      6,23,10,
		      "Intel(R) Core(TM)2 Duo CPU     T9900  @ 3.06GHz",
		      i);
     }
  }
#endif

#if 0
  create_output_dir("amd0fh");

  for(k=0;k<NUM_KERNELS;k++) {
     for(i=0;i<NUM_EVENTS;i++) {
        generate_results(dirname,
		      "old_amd",
		      kernels[k].name,
		      kernels[k].type,
		      "amd0fh",
		      15,4,21,
		      "AMD Athlon(tm) 64 Processor 2000+",
		      i);
     }
  }
#endif

#if 0
  create_output_dir("atom");

  for(k=0;k<NUM_KERNELS;k++) {
     for(i=0;i<NUM_EVENTS;i++) {
        generate_results(dirname,
		      "server",
		      kernels[k].name,
		      kernels[k].type,
		      "atom",
		      6,28,2,
		      "Intel(R) Atom(TM) CPU  230   @ 1.60GHz",
		      i);
     }
  }
#endif

  create_output_dir("nehalem");

  for(k=0;k<NUM_KERNELS;k++) {
     for(i=0;i<NUM_EVENTS;i++) {
        generate_results(dirname,
		      "dell_laptop",
		      kernels[k].name,
		      kernels[k].type,
		      "nehalem",
		      6,30,5,
		      "Intel(R) Core(TM) i? 2GHz",
		      i);
     }
  }

  return 0;
}
