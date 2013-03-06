#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../version.h"

#include "read_data.h"

#define FONTSIZE 16

#define EVENT_TO_PLOT 1

int main(int argc, char **argv) {

  int events,run,kernel,i;
  int plot_type=PLOT_TYPE_START;
  double maxy;
  double total;
  double average[NUM_KERNELS];

  if (argc<3) {
     printf("Must specify machine and start/stop/read/total\n");
     exit(1);
  }

  plot_type=read_data(argv[1],0,argv[2]);
  if (plot_type<0) {
     fprintf(stderr,"Some sort of error reading!\n");
     return -1;
  }

  events=EVENT_TO_PLOT;

  /* Calculate averages */
  double deviation[NUM_KERNELS],dev,temp;

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     total=0.0;
     for(run=0;run<NUM_RUNS;run++) {
        total+=(double)times[kernel][events][run];
     }
     average[kernel]=total/(double)NUM_RUNS;
//     printf("%s: avg=%.2f\n",kernel_names[kernel],average[kernel]);
  }

  /* Calculate Deviation */
  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     dev=0.0;
     for(run=0;run<NUM_RUNS;run++) {
        temp=(double)times[kernel][events][run]-average[kernel];
	temp*=temp;
        dev+=temp;
     }
     deviation[kernel]=dev/(double)(NUM_RUNS-1);
     deviation[kernel]=sqrt(deviation[kernel]);
  }


  /* calculate mins and maxes */
  maxy=0;

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     if (average[kernel]+deviation[kernel]>maxy) {
        maxy=average[kernel]+deviation[kernel];
     }
  }

  maxy=((((long long)maxy)/10000)+1)*10000;

  printf("(* Begin Graph *)\n");
  printf("newgraph\n");
  printf("\n");
  printf("X 8.5\n");
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
  printf("xaxis size 6.5 min %d max %d\n",-1,NUM_KERNELS); //minx,maxx);
  printf("grid_gray 0.9 grid_lines\n");
  printf("hash_labels font Helvetica fontsize %d  vjc hjr rotate 45\n",
	 FONTSIZE);
  printf("(* label font Helvetica fontsize %d  : Kernels *)\n",
	 FONTSIZE);
  printf("no_auto_hash_marks\n");
  for(i=0;i<NUM_KERNELS;i++) {
     printf("hash_at %d\n",i);
  }
  printf("no_auto_hash_labels\n");
  for(i=0;i<NUM_KERNELS;i++) {
     printf("hash_label at %d : %s\n",i,kernels[i].name);
  }

  printf("\n");
  printf("(* Title *)\n");
  printf("title font Helvetica fontsize %d y %lf : "
	 "%s Overall Overhead of ",
	 FONTSIZE,
	 (double)maxy+((double)maxy/8.0),argv[1]);

  if (plot_type==PLOT_TYPE_START) printf("Start");
  if (plot_type==PLOT_TYPE_STOP) printf("Stop");
  if (plot_type==PLOT_TYPE_READ) printf("Read");
  if (plot_type==PLOT_TYPE_TOTAL) printf("Start/Stop/Read");

  printf(" with %d Event%s\n",events,events==1?"":"s");
  printf("\n");

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {

//     printf("newcurve marksize 1 marktype xbar linetype none color %s\n",
  //          colors[kernel]);

     printf("newcurve ");
     if (kernels[kernel].type==INTERFACE_PERFCTR) {
        printf("marktype diamond linetype none color 1.0 0.0 0.0\n");
     }
     else if (kernels[kernel].type==INTERFACE_PERFMON2) {
        printf("marktype circle linetype none color 0.0 0.0 1.0\n");
     }
     else if (kernels[kernel].type==INTERFACE_PERF_EVENT) {
        printf("marktype box linetype none color 0.0 0.0 0.0\n");
     }
     else if (kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
        printf("marktype x linetype none color 0.3 0.3 0.3\n");
     }
     else {
       printf("marktype box linetype none color 0.3 0.3 0.0\n");
     }

     printf("y_epts\n");

     printf("\t%d %.2f %.2f %.2f (* %s *)\n",
            kernel,average[kernel],
	    average[kernel]-deviation[kernel],
            average[kernel]+deviation[kernel],
            kernels[kernel].name);

  }

  return 0;
}


