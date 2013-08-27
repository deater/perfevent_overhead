#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../version.h"

#include "read_data.h"

#define FONTSIZE 16

#define MAX_EVENTS 8

#define KERNELS_TO_PLOT 5

static int kernels_to_plot[KERNELS_TO_PLOT]={
  KERNEL_2_6_30_PERFMON2,
  KERNEL_2_6_32_PERFCTR,
  KERNEL_2_6_32,
  KERNEL_3_5_0,
  KERNEL_3_5_0_RDPMC,
};

long long *times=NULL;


static long long *get_runs(long long *pointer,int kernel, int event) {

        return pointer+ (kernel*NUM_EVENTS*NUM_RUNS)+(event*NUM_RUNS);

}


int main(int argc, char **argv) {

  int events,run,kernel,i,k;
  int plot_type=PLOT_TYPE_START;
  int minx,maxx;
  double maxy;
  double total;
  double average[NUM_KERNELS][MAX_EVENTS];

  if (argc<3) {
     printf("Must specify machine and start/stop/read/total\n");
     exit(1);
  }

        /* allocate memory */
        times=calloc(NUM_KERNELS*NUM_EVENTS*NUM_RUNS,sizeof(long long));
        if (times==NULL) {
                fprintf(stderr,"Error allocating!\n");
                return -1;
        }


  if (!strncmp(argv[1],"core2",4)) {
     minx=1; maxx=4;
  }

  if (!strncmp(argv[1],"nehalem",7)) {
     minx=1; maxx=6;
  }

  if (!strncmp(argv[1],"amd0fh",6)) {
     minx=1; maxx=4;
  }

  if (!strncmp(argv[1],"atom",4)) {
     minx=1; maxx=4;
  }

	/* read data */
  plot_type=read_data(argv[1],0,argv[2],STANDARD_KERNELS,times);
  if (plot_type<0) {
     fprintf(stderr,"Some sort of error reading!\n");
     return -1;
  }

  /* Calculate averages */
  double deviation[NUM_KERNELS][NUM_EVENTS],dev,temp;

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
    for(events=0;events<MAX_EVENTS;events++) {
       total=0.0;
       for(run=0;run<NUM_RUNS;run++) {
          total+=(double)*(get_runs(times,kernel,events)+run);
       }
       average[kernel][events]=total/(double)NUM_RUNS;
//     printf("%s: avg=%.2f\n",kernel_names[kernel],average[kernel]);
    }
  }

  /* Calculate Deviation */
  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     for(events=0;events<MAX_EVENTS;events++) {
        dev=0.0;
        for(run=0;run<NUM_RUNS;run++) {
           temp=(double)*(get_runs(times,kernel,events)+run)-average[kernel][events];
	   temp*=temp;
           dev+=temp;
        }
        deviation[kernel][events]=dev/(double)(NUM_RUNS-1);
        deviation[kernel][events]=sqrt(deviation[kernel][events]);
     }
  }


  /* calculate mins and maxes */
  maxy=0;

  for(k=0;k<KERNELS_TO_PLOT;k++) {
     kernel=kernels_to_plot[k];
     for(events=0;events<MAX_EVENTS;events++) {
        if (average[kernel][events]+deviation[kernel][events]>maxy) {
           maxy=average[kernel][events]+deviation[kernel][events];
        }
     }
  }

  maxy=((((long long)maxy)/10000)+1)*10000;

  printf("(* Begin Graph *)\n");
  printf("newgraph\n");
  printf("\n");
  printf("X 7\n");
  printf("Y 6\n");
  printf("clip\n");
  printf("\n");
  printf("(* Legend *)\n");
  printf("legend custom\n");
  printf("\n");
  printf("(* Y-Axis *)\n");
  printf("yaxis size 4 min 0 max %.0f\n",maxy);
  printf("(* grid_gray 0.9 grid_lines *)\n");
  printf("label font Helvetica fontsize %d  : Average Overhead (Cycles)\n", 
          FONTSIZE);
  printf("hash_labels font Helvetica fontsize %d\n",FONTSIZE);
  printf("\n");
  printf("(* X-Axis *)\n");
  printf("xaxis size 5.5 min %f max %f\n",(double)minx-0.5,(double)maxx+0.5);
  printf("grid_gray 0.9 grid_lines\n");
  printf("hash_labels font Helvetica fontsize %d\n",
	 FONTSIZE);
  printf("label font Helvetica fontsize %d  : Simultaneous Events Being Measured\n",
	 FONTSIZE);

  printf("no_auto_hash_marks\n");
  for(i=minx;i<maxx+1;i++) {
     printf("hash_at %d\n",i);
  }

  printf("no_auto_hash_labels\n");
  for(i=minx;i<maxx+1;i++) {
     printf("hash_label at %d : %d\n",i,i);
  }


  printf("\n");
  printf("(* Title *)\n");
  printf("title font Helvetica fontsize %d y %lf : "
	 "%s Overall ",
	 FONTSIZE,
	 (double)maxy+((double)maxy/8.0),argv[1]);

  if (plot_type==PLOT_TYPE_START) printf("Start");
  if (plot_type==PLOT_TYPE_STOP) printf("Stop");
  if (plot_type==PLOT_TYPE_READ) printf("Read");
  if (plot_type==PLOT_TYPE_TOTAL) printf("Start/Stop/Read");

  printf(" Overhead \n");

  for(k=0;k<KERNELS_TO_PLOT;k++) {
     kernel=kernels_to_plot[k];

//     printf("newcurve marksize 1 marktype xbar linetype none color %s\n",
  //          colors[kernel]);

     printf("newcurve ");
     if (kernels[kernel].type==INTERFACE_PERFCTR) {
        printf("marktype diamond linetype dashed color 1.0 0.0 0.0\n");
     }
     else if (kernels[kernel].type==INTERFACE_PERFMON2) {
        printf("marktype circle linetype dotted color 0.0 0.0 1.0\n");
     }
     else if (kernels[kernel].type==INTERFACE_PERF_EVENT) {
        if (kernel==KERNEL_2_6_32) {
           printf("marktype triangle linetype solid color 0.3 0.5 0.3\n");
        }
        else {
           printf("marktype box linetype solid color 0.0 0.0 0.0\n");
	}
     }
     else if (kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
        printf("marktype x linetype dashed color 0.3 0.3 0.3\n");
     }
     else {
       printf("marktype box linetype none color 0.3 0.3 0.0\n");
     }
     printf("label font Helvetica fontsize 14 "
            "x 1.0 y %lld vjc hjl : %s\n",
	    (long long)maxy-(((long long)maxy)/15*k),
	    kernels[kernel].name);

     printf("y_epts\n");

     for(events=1;events<=maxx;events++) {
       if (average[kernel][events]!=0) 
        printf("\t%d %.2f %.2f %.2f (* %s *)\n",
            events,
	    average[kernel][events],
	    average[kernel][events]-deviation[kernel][events],
            average[kernel][events]+deviation[kernel][events],
            kernels[kernel].name);
     }
  }

  return 0;
}


