#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FONTSIZE 11

#define NUM_EVENTS 16
#define NUM_RUNS 1024

#define NUM_KERNELS 5

char kernel_names[NUM_KERNELS][64]={
   "2.6.32",
   "3.4.0",
   "3.4.0-rdpmc",
   "2.6.30-perfmon2",
   "2.6.32-perfctr",
};

char colors[NUM_KERNELS][64]={
  "1.0 0.75   0.75", /* pink-red */
  "0.625 1.0 0.5",   /* ??? */
  "0.0 0.0   1.0",   /* blue */
  "0.0 1.0   0.0",   /* green */
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

int main(int argc, char **argv) {

  int events,run,kernel;
  char filename[BUFSIZ];
  FILE *fff;
  char string[BUFSIZ];
  char *machine,*plot_name;
  int plot_type=PLOT_TYPE_START;
  
     
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
     fprintf(stderr,"\tReading data for kernel %s: ",kernel_names[kernel]); 
     for(events=0;events<NUM_EVENTS;events++) {
        fprintf(stderr,"%d ",events);
        for(run=0;run<NUM_RUNS;run++) {
           sprintf(filename,"results/rdtsc_null_raw/%s/%s/%d/null_test.%d",
	           machine,kernel_names[kernel],events,run);
           fff=fopen(filename,"r");
           if (fff==NULL) {
	      fprintf(stderr,"Can't open %s\n",filename);
	      break;
           }
           if (fgets(string,BUFSIZ,fff)==NULL) break;
           if (fgets(string,BUFSIZ,fff)==NULL) break;
	   if (fgets(string,BUFSIZ,fff)==NULL) break;
	   if (fgets(string,BUFSIZ,fff)==NULL) break;
           sscanf(string,"%*s %*s %lld",&times_start);
	   if (fgets(string,BUFSIZ,fff)==NULL) break;
	   sscanf(string,"%*s %*s %lld",&times_stop);
	   if (fgets(string,BUFSIZ,fff)==NULL) break;
	   sscanf(string,"%*s %*s %lld",&times_read);

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


           fclose(fff);
	}
     }
     fprintf(stderr,"\n");
  }

  FILE *ggg;

  for(events=1;events<8;events++) {

     /* Put info in the linked lists */
     for(kernel=0;kernel<NUM_KERNELS;kernel++) {

       sprintf(filename,"%s.%s.%d",machine,kernel_names[kernel],events);
	ggg=fopen(filename,"w");

        for(run=0;run<NUM_RUNS;run++) {
	  fprintf(ggg,"%lld\n",times[kernel][events][run]);
	}
	fclose(ggg);
     }
  }


  return 0;
}


