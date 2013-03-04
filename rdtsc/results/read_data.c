#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../version.h"

#include "read_data.h"

char kernel_names[NUM_KERNELS][64]={
   "2.6.30-perfmon2",
   "2.6.32-perfctr",
   "2.6.32",
   "2.6.33",
   "2.6.34",
   "2.6.35",
   "2.6.36",
   "2.6.37",
   "2.6.38",
   "2.6.39",
   "3.0.0",
   "3.1.0",
   "3.2.0",
   "3.3.0",
   "3.4.0",
   "3.4.0-rdpmc",
   "3.5.0",
   "3.5.0-rdpmc",
};

char colors[NUM_KERNELS][64]={
  "0.0 0.0   1.0",   /* blue */
  "1.0 0.0   0.0",   /* red */
  "1.0 0.75   0.75", /* pink-red */
  "1.0 0.625 0.0",   /* orange */
  "1.0 1.0   0.0",   /* yellow */
  "0.0 0.375 0.0",   /* dark green */
  "0.63 0.82 0.62",  /* light green */
  "0.0 1.0   1.0",   /* cyan */
  "0.375 0.0 1.0",   /* indigo */
  "1.0 0.0 1.0",     /* magenta */
  "0.625 0.0 0.5",   /* purple */
  "0.0 0.0 0.0",     /* black */
  "0.5 0.5 0.5",     /* grey */
  "1.0 0.5 0.5",     /* ??? */
  "0.625 1.0 0.5",   /* ??? */
  "0.0 1.0   0.0",   /* green */
  "0.25 1.0   0.0",   /* ??? */
  "0.5 1.0   0.0",   /* ??? */
};

long long times[NUM_KERNELS][NUM_EVENTS][NUM_RUNS];

static int debug=0;

struct cpuinfo_t cpuinfo;

int read_data(char *machine, int which, char *plot_name) {

  int events,run,kernel,runs;
  char filename[BUFSIZ];
  FILE *fff;
  char string[BUFSIZ];
  char *result;
  int plot_type=PLOT_TYPE_START;
  long long times_start,times_stop,times_read;

  char hostname[BUFSIZ]="Unknown",kernel_name[BUFSIZ]="Unknown";
  char gathered_version[BUFSIZ]="Unknown";
  int interface=INTERFACE_PERF_EVENT;

  if (!strcmp(plot_name,"start")) {
     plot_type=PLOT_TYPE_START;
  } else if (!strcmp(plot_name,"stop")) {
     plot_type=PLOT_TYPE_STOP;
  } else if (!strcmp(plot_name,"read")) {
     plot_type=PLOT_TYPE_READ;
  } else if (!strcmp(plot_name,"total")) {
     plot_type=PLOT_TYPE_TOTAL;
  }
  else {
    printf("Unknown plot type %s\n",plot_name);
    exit(1);
  }

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     for(events=0;events<NUM_EVENTS;events++) {
        for(run=0;run<NUM_RUNS;run++) {
	   times[kernel][events][run]=0LL;
	}
     }
  }

  fprintf(stderr,"Reading in data for %s...\n",machine);

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     fprintf(stderr,"\tReading data for kernel %s\n",kernel_names[kernel]); 
     for(events=0;events<NUM_EVENTS;events++) {
        fprintf(stderr,"%d ",events);

        sprintf(filename,"%s/0/%s/%d/results",
	           machine,kernel_names[kernel],events);
        fff=fopen(filename,"r");
        if (fff==NULL) {
	   fprintf(stderr,"Can't open %s\n",filename);
	   break;
        }

        /**********************/
        /* Find System Header */
        /**********************/
        while(1) {
           result=fgets(string,BUFSIZ,fff);
           if (result==NULL) return -1;

           if (!strncmp(string,"### System info",15)) {
              break;
           }
        }
        /**********************/
        /* Read System Header */
        /**********************/
        while(1) {
           result=fgets(string,BUFSIZ,fff);
           if (result==NULL) return -1;

           /* Break if reach end of header */
           if (!strncmp(string,"###",3)) {
              break;
           }
           if (!strncmp(string,"Kernel:",7)) {
              strcpy(kernel_name,string+8);
              kernel_name[strlen(kernel_name)-1]=0;
           }
           if (!strncmp(string,"Hostname:",9)) {
              sscanf(string,"%*s %s",hostname);
           }
           if (!strncmp(string,"Family:",7)) {
              sscanf(string,"%*s %d",&cpuinfo.family);
           }
           if (!strncmp(string,"Model:",6)) {
              sscanf(string,"%*s %d",&cpuinfo.model);
           }
           if (!strncmp(string,"Stepping:",9)) {
              sscanf(string,"%*s %d",&cpuinfo.stepping);
           }
           if (!strncmp(string,"Modelname:",10)) {
              strcpy(cpuinfo.modelname,string+11);
              cpuinfo.modelname[strlen(cpuinfo.modelname)-1]=0;
           }
           if (!strncmp(string,"Generic:",8)) {
              /* should check that this matches */
              strcpy(cpuinfo.generic_modelname,string+9);
              cpuinfo.generic_modelname[strlen(cpuinfo.generic_modelname)-1]=0;
           }
           if (!strncmp(string,"generate_results version:",25)) {
              sscanf(string,"%*s %*s %s",gathered_version);
           }
           if (!strncmp(string,"Interface:",10)) {
              if (strstr(string,"perf_event_rdpmc")) {
                 interface=INTERFACE_PERF_EVENT_RDPMC;
              } else if (strstr(string,"perf_event")) {
                 interface=INTERFACE_PERF_EVENT;
              } else if (strstr(string,"perfmon")) {
                 interface=INTERFACE_PERFMON2;
              } else if (strstr(string,"perfctr")) {
                 interface=INTERFACE_PERFCTR;
              } else {
                 fprintf(stderr,"Unknown interface!\n");
                 interface=INTERFACE_UNKNOWN;
              }
          }
          if (!strncmp(string,"Counters:",7)) {
          }
          if (!strncmp(string,"Runs:",4)) {
             sscanf(string,"%*s %d",&runs);
          }
       }

       /* Print header if first time through */
       if (debug) {
          printf("#### Events %d\n",events);
          printf("\tHostname:  %s\n",hostname);
          printf("\tKernel:    %s\n",kernel_name);
          printf("\tInterface: ");
          if (interface==INTERFACE_PERF_EVENT) printf("perf_event\n");
          else if (interface==INTERFACE_PERFMON2) printf("perfmon2\n");
          else if (interface==INTERFACE_PERFCTR) printf("perfctr\n");
          else if (interface==INTERFACE_PERF_EVENT_RDPMC) printf("perf_event_rdpmc\n");
          else printf("Unknown\n");
          printf("\tCPU:       %d/%d/%d\n",
             cpuinfo.family,cpuinfo.model,cpuinfo.stepping);
          printf("\tCPU type:  %s\n",cpuinfo.generic_modelname);
          printf("\tCPU name:  %s\n",cpuinfo.modelname);
          printf("\tdata gathered with tool version: %s\n",gathered_version);
          printf("\truns: %d\n",runs);
          printf("\tdata analyzed with tool version: %s\n",
             PERF_EVENT_OVERHEAD_VERSION);
       }

/*
### Perf Results 0
init_latency: 69 cycles
Eventset_creation_3: 150420 cycles
start latency: 34765 cycles
stop latency: 11431 cycles
read latency: 12282 cycles
53003c 2017
5300c0 1129
5300c4 356
### Perf Results 1
*/

       /* read data */
       run=0;
loop:
       while(1) {
	   if (fgets(string,BUFSIZ,fff)==NULL) break;

           if (!strncmp(string,"###",3)) break;

           if (!strncmp(string,"start latency",13)) {
              sscanf(string,"%*s %*s %lld",&times_start);
           }

           if (!strncmp(string,"stop latency",12)) {
              sscanf(string,"%*s %*s %lld",&times_stop);
           }

           if (!strncmp(string,"read latency",12)) {
              sscanf(string,"%*s %*s %lld",&times_read);
           }

       }

       //       printf("%d %lld\n",run,times_start);

       if (plot_type==PLOT_TYPE_START) {
	  times[kernel][events][run]=times_start;
       }
       if (plot_type==PLOT_TYPE_STOP) {
	  times[kernel][events][run]=times_stop;
       }
       if (plot_type==PLOT_TYPE_READ) {
	  times[kernel][events][run]=times_read;
       }
       if (plot_type==PLOT_TYPE_TOTAL) {
	  times[kernel][events][run]=times_start+times_stop+times_read;
       }
       run++;

       if (run<runs) goto loop;

       fclose(fff);
     }
     fprintf(stderr,"\n");
  }

  return plot_type;
}


