#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../version.h"

#include "read_data.h"

#define FONTSIZE 16

#define PLOT_EVENTS 8

long long *times=NULL;

int main(int argc, char **argv) {

	int events,i,kernel;
	int machine_num;
	int plot_type=PLOT_TYPE_START;
	int minx=1,maxx=4;

        double maxy,maxys[PLOT_EVENTS];
        double average[PLOT_EVENTS][NUM_KERNELS],deviation[PLOT_EVENTS][NUM_KERNELS];
        double median[PLOT_EVENTS][NUM_KERNELS],twentyfive[PLOT_EVENTS][NUM_KERNELS],
                seventyfive[PLOT_EVENTS][NUM_KERNELS];


	if (argc<4) {
		printf("Must specify machine type, num, and start/stop/read/total\n");
		exit(1);
	}

	machine_num=atoi(argv[2]);


        /* allocate memory */
        times=calloc(NUM_KERNELS*PLOT_EVENTS*NUM_RUNS,sizeof(long long));
        if (times==NULL) {
                fprintf(stderr,"Error allocating!\n");
                return -1;
        }

	/* read data */
	plot_type=read_data(argv[1],machine_num,argv[3],VARYING_KERNELS,times);
	if (plot_type<0) {
		fprintf(stderr,"Some sort of error reading!\n");
		return -1;
	}

	for(i=0;i<PLOT_EVENTS;i++) {
		/* sort data */
		sort_data(times,i,VARYING_KERNELS);

		/* calculate median, twentyfive, seventyfive */
		calculate_boxplot_data(times,i,
			median[i], twentyfive[i],
			seventyfive[i],
			VARYING_KERNELS);

		/* Calculate averages and deviation */
		calculate_deviation(times,i,
				average[i],deviation[i],VARYING_KERNELS);

		/* calculate mins and maxes */
		maxys[i]=calculate_maxy(average[i],deviation[i],VARYING_KERNELS);

	}


	/* calculate mins and maxes */
	maxy=0;
	for(events=0;events<PLOT_EVENTS;events++) {
		if (maxys[events]>maxy) maxy=maxys[events];
	}

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
	printf("label font Helvetica fontsize %d  : "
		"Average Overhead (Cycles)\n", FONTSIZE);
	printf("hash_labels font Helvetica fontsize %d\n",FONTSIZE);
	printf("\n");
	printf("(* X-Axis *)\n");
	printf("xaxis size 5.5 min %f max %f\n",
		(double)minx-0.5,(double)maxx+0.5);
	printf("grid_gray 0.9 grid_lines\n");
	printf("hash_labels font Helvetica fontsize %d\n",
		FONTSIZE);
	printf("label font Helvetica fontsize %d  : "
		"Simultaneous Events Being Measured\n",FONTSIZE);

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
		"%s Overall ",FONTSIZE,
		(double)maxy+((double)maxy/8.0),argv[1]);

	if (plot_type==PLOT_TYPE_START) printf("Start");
	if (plot_type==PLOT_TYPE_STOP) printf("Stop");
	if (plot_type==PLOT_TYPE_READ) printf("Read");
	if (plot_type==PLOT_TYPE_TOTAL) printf("Start/Stop/Read");

	printf(" Overhead \n");

	for(kernel=0;kernel<NUM_VARYING_KERNELS;kernel++) {

		printf("(* varying_kernels[%d].type=%d *)\n",
			kernel,varying_kernels[kernel].type);
		printf("newcurve ");
		switch(varying_kernels[kernel].type) {
			case INTERFACE_PERFCTR:
				printf("marktype diamond linetype dashed color 1.0 0.0 0.0\n");
				break;
			case INTERFACE_PERFMON2:
				printf("marktype circle linetype dotted color 0.0 0.0 1.0\n");
				break;
			case INTERFACE_PERF_EVENT:
				printf("marktype box linetype solid color 0.0 0.0 0.0\n");
				break;
			case INTERFACE_PERF_EVENT_RDPMC:
				printf("marktype x linetype dashed color 0.11 0.40 0.11\n");
				break;
			default:
				printf("marktype box linetype none color 0.3 0.3 0.0\n");
		}

		printf("label font Helvetica fontsize 14 "
			"x 1.0 y %lld vjc hjl : %s\n",
			(long long)maxy-(((long long)maxy)/15*kernel),
			varying_kernels[kernel].name);

		printf("y_epts\n");

		for(events=1;events<=maxx;events++) {
			if (average[events][kernel]!=0) {
				printf("\t%d %.2f %.2f %.2f (* %s *)\n",
					events,
					average[events][kernel],
					average[events][kernel]-
					deviation[events][kernel],
					average[events][kernel]+
					deviation[events][kernel],
					varying_kernels[kernel].name);
     			}
  		}
	}
	return 0;
}


