#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../version.h"

#define FONTSIZE 16

#define NUM_EVENTS 16
#define NUM_RUNS 1024

#define NUM_KERNELS 16

#define EVENT_TO_PLOT 1

char kernel_names[NUM_KERNELS][64]={
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
   "2.6.30-perfmon2",
   "2.6.32-perfctr",
};

char colors[NUM_KERNELS][64]={
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
  "0.0 0.0   1.0",   /* blue */
  "1.0 0.0   0.0",   /* red */
};

long long times[NUM_KERNELS][NUM_EVENTS][NUM_RUNS];
long long times_start,times_stop,times_read;

struct freq_list {
  long long value;
  long long count;
  struct freq_list *next;
};

#define PLOT_TYPE_START 0
#define PLOT_TYPE_STOP  1
#define PLOT_TYPE_READ  2
#define PLOT_TYPE_TOTAL 3

#define INTERFACE_PERF_EVENT 0
#define INTERFACE_PERFMON2   1
#define INTERFACE_PERFCTR    2
#define INTERFACE_PERF_EVENT_RDPMC 3
#define INTERFACE_UNKNOWN    99

static int debug=0;

struct cpuinfo_t {
   int vendor;
   int family;
   int model;
   int stepping;
   char modelname[BUFSIZ];
   char generic_modelname[BUFSIZ];
   int num_cpus;
} cpuinfo;


int main(int argc, char **argv) {

  int events,run,kernel,runs;
  char filename[BUFSIZ];
  FILE *fff;
  char string[BUFSIZ];
  char *machine,*plot_name,*result;
  int plot_type=PLOT_TYPE_START;
  long long maxy,minx,maxx,threshold;

  struct freq_list *head[NUM_KERNELS],*tmp,*last;

  char hostname[BUFSIZ]="Unknown",kernel_name[BUFSIZ]="Unknown";
  char gathered_version[BUFSIZ]="Unknown";
  int interface=INTERFACE_PERF_EVENT;


  if (argc<3) {
     printf("Must specify machine and start/stop/read/total\n");
     exit(1);
  }

  machine=strdup(argv[1]);

  plot_name=strdup(argv[2]);

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

  fprintf(stderr,"Reading in data for %s...\n",argv[1]);

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


  events=EVENT_TO_PLOT;

  /********************************/
  /* Put info in the linked lists */
  /********************************/

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {

     head[kernel]=calloc(1,sizeof(struct freq_list));

     for(run=0;run<NUM_RUNS;run++) {

        tmp=head[kernel];
	last=head[kernel];

	while(1) {

	   if (tmp->value==times[kernel][events][run]) {
              /* 0 shouldn't count */
	      if (tmp->value!=0) tmp->count++;
	      break;
	   }

	   if (times[kernel][events][run] < tmp->value) {
	      last->next=calloc(1,sizeof(struct freq_list));
	      last->next->count=1;
	      last->next->value=times[kernel][events][run];
	      last->next->next=tmp;
	      break;
	   }

	   if (tmp->next==NULL) {
	      tmp->next=calloc(1,sizeof(struct freq_list));
	      tmp->next->count=1;
	      tmp->next->value=times[kernel][events][run];
	      tmp->next->next=NULL;
	      break;
	   }

	   last=tmp;
	   tmp=tmp->next;
	}

     }
  }

  /* calculate mins and maxes */

  minx=1000000000;
  maxx=0;
  maxy=0;
  threshold=5;

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     for(tmp=head[kernel];tmp!=NULL;tmp=tmp->next) {
       if (tmp->count!=0) {
	 if (tmp->value<minx) minx=tmp->value;
	 if ((tmp->value>maxx) && (tmp->count>5)) maxx=tmp->value;
	 if (tmp->count>maxy) { 
            maxy=tmp->count;
            //printf("Setting maxy to %lld\n",maxy);
	 }
       }
     }
  }

  if (maxy<21) threshold=2;

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     for(tmp=head[kernel];tmp!=NULL;tmp=tmp->next) {
       if (tmp->count!=0) {
	 if ((tmp->value>maxx) && (tmp->count>threshold)) maxx=tmp->value;
       }
     }
  }



  minx=(minx/1000)*1000;
  maxx=((maxx/1000)+1)*1000;
  maxy=((maxy/10)+1)*10;

  printf("(* Begin Graph *)\n");
  printf("newgraph\n");
  printf("\n");
  printf("X 7\n");
  printf("Y 5\n");
  printf("clip\n");
  printf("\n");
  printf("(* Legend *)\n");
  printf("legend custom\n");
  printf("\n");
  printf("(* Y-Axis *)\n");
  printf("yaxis size 4 min 0 max %lld\n",maxy);
  printf("grid_gray 0.9 grid_lines\n");
  printf("label font Helvetica fontsize %d  : Times Observed (out of %d)\n",
         FONTSIZE,NUM_RUNS);
  printf("hash_labels font Helvetica fontsize %d\n",FONTSIZE);
  printf("\n");
  printf("(* X-Axis *)\n");
  printf("xaxis size 6 min %lld max %lld\n",0LL,maxx); //minx,maxx);
  printf("hash_labels font Helvetica fontsize %d\n",FONTSIZE);
  printf("label font Helvetica fontsize %d  : Overhead (cycles)\n",
	 FONTSIZE);
  printf("\n");
  printf("(* Title *)\n");
  printf("title font Helvetica fontsize %d y %lf : "
	 "%s Overall Overhead of ",
	 FONTSIZE,
	 (double)maxy+((double)maxy/8.0),machine);

  if (plot_type==PLOT_TYPE_START) printf("Start");
  if (plot_type==PLOT_TYPE_STOP) printf("Stop");
  if (plot_type==PLOT_TYPE_READ) printf("Read");
  if (plot_type==PLOT_TYPE_TOTAL) printf("Start/Stop/Read");

  printf(" with %d Event%s\n",events,events==1?"":"s");
  printf("\n");


  for(kernel=0;kernel<NUM_KERNELS;kernel++) {

     int xsize=maxx-minx;

     printf("newcurve marksize 1 marktype xbar linetype none color %s\n",
            colors[kernel]);

     printf("(* label hjl vjc fontsize 14 font Helvetica x %lld y %.2lf "
            ": %s *)\n",
	    maxx-(xsize/5),
	    (double)maxy-(((double)maxy/20.0)*(double)kernel),
	    kernel_names[kernel]);
     printf("pts\n");

     for(tmp=head[kernel];tmp!=NULL;tmp=tmp->next) {
       if (tmp->count!=0) 
        printf("\t%lld %lld\n",tmp->value,tmp->count);
     }
  }

  return 0;
}


