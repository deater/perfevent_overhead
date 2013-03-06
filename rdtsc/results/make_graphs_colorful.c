#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../version.h"

#include "read_data.h"

#define FONTSIZE 16

#define EVENT_TO_PLOT 1

struct freq_list {
  long long value;
  long long count;
  struct freq_list *next;
};

int main(int argc, char **argv) {

  int events,run,kernel;
  int plot_type=PLOT_TYPE_START;
  long long maxy,minx,maxx,threshold;

  struct freq_list *head[NUM_KERNELS],*tmp,*last;

  if (argc<3) {
     printf("Must specify machine and start/stop/read/total\n");
     exit(1);
  }

  plot_type=read_data(argv[1],0,argv[2]);

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
	 (double)maxy+((double)maxy/8.0),argv[1]);

  if (plot_type==PLOT_TYPE_START) printf("Start");
  if (plot_type==PLOT_TYPE_STOP) printf("Stop");
  if (plot_type==PLOT_TYPE_READ) printf("Read");
  if (plot_type==PLOT_TYPE_TOTAL) printf("Start/Stop/Read");

  printf(" with %d Event%s\n",events,events==1?"":"s");
  printf("\n");


  for(kernel=0;kernel<NUM_COLORFUL_KERNELS;kernel++) {

     int xsize=maxx-minx;

     printf("newcurve marksize 1 marktype xbar linetype none color %s\n",
            colors[kernel]);

     printf("(* label hjl vjc fontsize 14 font Helvetica x %lld y %.2lf "
            ": %s *)\n",
	    maxx-(xsize/5),
	    (double)maxy-(((double)maxy/20.0)*(double)kernel),
	    kernels[kernel].name);
     printf("pts\n");

     for(tmp=head[kernel];tmp!=NULL;tmp=tmp->next) {
       if (tmp->count!=0)
        printf("\t%lld %lld\n",tmp->value,tmp->count);
     }
  }

  return 0;
}


