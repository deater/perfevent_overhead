#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../version.h"

#include "read_data.h"

#define FONTSIZE 16

#define EVENT_TO_PLOT 1


int main(int argc, char **argv) {

	int events,kernel,i;
	int plot_type=PLOT_TYPE_START;
	int machine_num=0;
	double maxy;
	double average[NUM_FINAL_KERNELS],deviation[NUM_FINAL_KERNELS];
	double median[NUM_FINAL_KERNELS],twentyfive[NUM_FINAL_KERNELS],
		seventyfive[NUM_FINAL_KERNELS];

	long long *times=NULL;

	/* parse command line args */
	if (argc<4) {
		printf("Must specify machine type, machine num, "
			"and start/stop/read/total\n");
		exit(1);
	}

	machine_num=atoi(argv[2]);

	/* allocate memory */
	times=calloc(NUM_FINAL_KERNELS*NUM_EVENTS*NUM_RUNS,sizeof(long long));
	if (times==NULL) {
		fprintf(stderr,"Error allocating!\n");
		return -1;
	}


	/* read in data */
	plot_type=read_data(argv[1],machine_num,argv[3],FINAL_KERNELS,
			times);
	if (plot_type<0) {
		fprintf(stderr,"Some sort of error reading!\n");
		return -1;
	}

	events=EVENT_TO_PLOT;

  	/* sort data */
	sort_data(times,events,FINAL_KERNELS);


	/* calculate median, twentyfive, seventyfive */
	calculate_boxplot_data(times,events,
                        median, twentyfive,
                        seventyfive,
			FINAL_KERNELS);

	/* Calculate averages and deviation */
	calculate_deviation(times,events,average,deviation,FINAL_KERNELS);


	/* calculate mins and maxes */
	maxy=calculate_maxy(average,deviation,FINAL_KERNELS);

	/* Make graph */
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
	printf("xaxis size 6.5 min %d max %d\n",-1,NUM_FINAL_KERNELS); //minx,maxx);
	printf("grid_gray 0.9 grid_lines\n");
	printf("hash_labels font Helvetica fontsize %d  vjc hjr rotate 45\n",
		FONTSIZE);
	printf("(* label font Helvetica fontsize %d  : Kernels *)\n",
		FONTSIZE);
	printf("no_auto_hash_marks\n");
	for(i=0;i<NUM_FINAL_KERNELS;i++) {
		printf("hash_at %d\n",i);
	}
	printf("no_auto_hash_labels\n");
	for(i=0;i<NUM_FINAL_KERNELS;i++) {
		printf("hash_label at %d : %s\n",i,final_kernels[i].name);
	}

	printf("\n");
	printf("(* Title *)\n");
	printf("title font Helvetica fontsize %d y %lf : "
		"%s Overhead of ", FONTSIZE,
		(double)maxy+((double)maxy/8.0),argv[1]);

	if (plot_type==PLOT_TYPE_START) printf("Start");
	if (plot_type==PLOT_TYPE_STOP) printf("Stop");
	if (plot_type==PLOT_TYPE_READ) printf("Read");
	if (plot_type==PLOT_TYPE_TOTAL) printf("Start/Stop/Read");

	printf(" with %d Event%s\n",events,events==1?"":"s");
	printf("\n");

	for(kernel=0;kernel<NUM_FINAL_KERNELS;kernel++) {

		/* plot standard deviation */
		printf("newcurve ");
		if (final_kernels[kernel].type==INTERFACE_PERFCTR) {
			printf("marktype x linetype none color 1.0 0.0 0.0\n");
		}
		else if (final_kernels[kernel].type==INTERFACE_PERFMON2) {
			printf("marktype x linetype none color 0.0 0.0 1.0\n");
		}
		else if (final_kernels[kernel].type==INTERFACE_PERF_EVENT) {
			printf("marktype x linetype none color 0.0 0.0 0.0\n");
     		}
		else if (final_kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
			printf("marktype x linetype none color 0.3 0.3 0.3\n");
		}
		else {
			printf("marktype x linetype none color 0.3 0.3 0.0\n");
		}

		printf("y_epts\n");

		printf("\t%d %.2f %.2f %.2f (* %s standard dev *)\n",
			kernel,average[kernel],
			average[kernel]-deviation[kernel],
			average[kernel]+deviation[kernel],
			final_kernels[kernel].name);

		/* plot 25th to 75th box */
		printf("newcurve ");
		printf("marktype box marksize 0.5 %lf linetype none ",
			seventyfive[kernel]-twentyfive[kernel]);
		if (final_kernels[kernel].type==INTERFACE_PERFCTR) {
			printf("color 1.0 0.0 0.0\n");
		}
		else if (final_kernels[kernel].type==INTERFACE_PERFMON2) {
			printf("color 0.0 0.0 1.0\n");
		}
		else if (final_kernels[kernel].type==INTERFACE_PERF_EVENT) {
			printf("color 0.0 0.0 0.0\n");
		}
		else if (final_kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
			printf("color 0.3 0.3 0.3\n");
		}
		else {
			printf("color 0.3 0.3 0.0\n");
		}
		printf("pts\n");
		printf("\t%d %lf\n",kernel,
			(twentyfive[kernel]+seventyfive[kernel])/2.0);
		printf("\t(* %lf %lf %lf *)\n",median[kernel],twentyfive[kernel],seventyfive[kernel]);

		/* plot median */

		printf("newcurve ");
		printf("marktype box marksize 0.5 1 linetype none color 1.0 1.0 1.0\n");
		printf("pts\n");
		printf("\t%d %lf\n",kernel,median[kernel]);

		/* plot outliers */

		printf("newcurve ");
		if (final_kernels[kernel].type==INTERFACE_PERFCTR) {
			printf("marktype x linetype none color 1.0 0.0 0.0\n");
		}
		else if (final_kernels[kernel].type==INTERFACE_PERFMON2) {
			printf("marktype x linetype none color 0.0 0.0 1.0\n");
		}
		else if (final_kernels[kernel].type==INTERFACE_PERF_EVENT) {
			printf("marktype x linetype none color 0.0 0.0 0.0\n");
		}
		else if (final_kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
			printf("marktype x linetype none color 0.3 0.3 0.3\n");
		}
		else {
			printf("marktype x linetype none color 0.3 0.3 0.0\n");
		}

		printf("pts\n");

		for(i=0;i<NUM_RUNS;i++) {
			if (*(get_runs(times,kernel,events)+i) >
				average[kernel] + deviation[kernel]) {
				printf("\t%d %lld\n",kernel,
					*(get_runs(times,kernel,events)+i));
			}
			if (*(get_runs(times,kernel,events)+i) <
				average[kernel]-deviation[kernel]) {
				printf("\t%d %lld\n",kernel,
					*(get_runs(times,kernel,events)+i));
			}
		}

		printf("\n");

	}

	return 0;
}


