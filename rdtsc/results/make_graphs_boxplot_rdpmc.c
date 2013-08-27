/* plot the rdpmc results */

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
	double maxy,maxy_rdpmc;
	double average[NUM_KERNELS],deviation[NUM_KERNELS];
	double median[NUM_KERNELS],twentyfive[NUM_KERNELS],
		seventyfive[NUM_KERNELS];


	double average_rdpmc[NUM_RDPMC_KERNELS],deviation_rdpmc[NUM_RDPMC_KERNELS];
	double median_rdpmc[NUM_RDPMC_KERNELS],twentyfive_rdpmc[NUM_RDPMC_KERNELS],
		seventyfive_rdpmc[NUM_RDPMC_KERNELS];

	long long *times=NULL,*times_rdpmc=NULL;

	/* parse command line args */
	if (argc<4) {
		printf("Must specify machine type, machine num, "
			"and start/stop/read/total\n");
		exit(1);
	}

	machine_num=atoi(argv[2]);

	/* allocate memory */
	times=calloc(NUM_KERNELS*NUM_EVENTS*NUM_RUNS,sizeof(long long));
	if (times==NULL) {
		fprintf(stderr,"Error allocating!\n");
		return -1;
	}
	times_rdpmc=calloc(NUM_RDPMC_KERNELS*NUM_EVENTS*NUM_RUNS,sizeof(long long));
	if (times_rdpmc==NULL) {
		fprintf(stderr,"Error allocating rdpmc!\n");
		return -1;
	}


	/* read in data full */
	plot_type=read_data(argv[1],machine_num,argv[3],
			STANDARD_KERNELS,times);
	if (plot_type<0) {
		fprintf(stderr,"Some sort of error reading!\n");
		return -1;
	}


	/* read in data rdpmc */
	read_data(argv[1],machine_num,argv[3],
			RDPMC_KERNELS,times_rdpmc);

	events=EVENT_TO_PLOT;

  	/* sort data */
	sort_data(times,events,STANDARD_KERNELS);
	sort_data(times_rdpmc,events,RDPMC_KERNELS);

	/* calculate median, twentyfive, seventyfive */
	calculate_boxplot_data(times,events,
                        median, twentyfive,
                        seventyfive,STANDARD_KERNELS);
	calculate_boxplot_data(times_rdpmc,events,
                        median_rdpmc, twentyfive_rdpmc,
                        seventyfive_rdpmc,RDPMC_KERNELS);

	/* Calculate averages and deviation */
	calculate_deviation(times,events,average,deviation,STANDARD_KERNELS);
	calculate_deviation(times_rdpmc,events,average_rdpmc,deviation_rdpmc,RDPMC_KERNELS);


	/* calculate mins and maxes */
	maxy=calculate_maxy(average,deviation,STANDARD_KERNELS);
	maxy_rdpmc=calculate_maxy(average_rdpmc,deviation_rdpmc,RDPMC_KERNELS);
	if (maxy_rdpmc>maxy) maxy=maxy_rdpmc;

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
		"%s rdpmc Overhead of ", FONTSIZE,
		(double)maxy+((double)maxy/8.0),argv[1]);

	if (plot_type==PLOT_TYPE_START) printf("Start");
	if (plot_type==PLOT_TYPE_STOP) printf("Stop");
	if (plot_type==PLOT_TYPE_READ) printf("Read");
	if (plot_type==PLOT_TYPE_TOTAL) printf("Start/Stop/Read");

	printf(" with %d Event%s\n",events,events==1?"":"s");
	printf("\n");

	for(kernel=0;kernel<NUM_KERNELS;kernel++) {

		/* plot standard deviation */
		printf("newcurve ");
		if (kernels[kernel].type==INTERFACE_PERFCTR) {
			printf("marktype x linetype none color 1.0 0.0 0.0\n");
		}
		else if (kernels[kernel].type==INTERFACE_PERFMON2) {
			printf("marktype x linetype none color 0.0 0.0 1.0\n");
		}
		else if (kernels[kernel].type==INTERFACE_PERF_EVENT) {
			printf("marktype x linetype none color 0.0 0.0 0.0\n");
     		}
		else if (kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
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
			kernels[kernel].name);

		/* plot 25th to 75th box */
		printf("newcurve ");
		printf("marktype box marksize 0.5 %lf linetype none ",
			seventyfive[kernel]-twentyfive[kernel]);
		if (kernels[kernel].type==INTERFACE_PERFCTR) {
			printf("color 1.0 0.0 0.0\n");
		}
		else if (kernels[kernel].type==INTERFACE_PERFMON2) {
			printf("color 0.0 0.0 1.0\n");
		}
		else if (kernels[kernel].type==INTERFACE_PERF_EVENT) {
			printf("color 0.0 0.0 0.0\n");
		}
		else if (kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
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
		if (kernels[kernel].type==INTERFACE_PERFCTR) {
			printf("marktype x linetype none color 1.0 0.0 0.0\n");
		}
		else if (kernels[kernel].type==INTERFACE_PERFMON2) {
			printf("marktype x linetype none color 0.0 0.0 1.0\n");
		}
		else if (kernels[kernel].type==INTERFACE_PERF_EVENT) {
			printf("marktype x linetype none color 0.0 0.0 0.0\n");
		}
		else if (kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
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

#define RDPMC_OFFSET 14

	/* RDPMC kernels */

	for(kernel=0;kernel<NUM_RDPMC_KERNELS;kernel++) {

		/* plot standard deviation */
		printf("newcurve ");
		if (rdpmc_kernels[kernel].type==INTERFACE_PERFCTR) {
			printf("marktype x linetype none color 1.0 0.0 0.0\n");
		}
		else if (rdpmc_kernels[kernel].type==INTERFACE_PERFMON2) {
			printf("marktype x linetype none color 0.0 0.0 1.0\n");
		}
		else if (rdpmc_kernels[kernel].type==INTERFACE_PERF_EVENT) {
			printf("marktype x linetype none color 0.0 0.0 0.0\n");
     		}
		else if (rdpmc_kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
			printf("marktype x linetype none color 0.3 0.3 0.3\n");
		}
		else {
			printf("marktype x linetype none color 0.3 0.3 0.0\n");
		}

		printf("y_epts\n");

		printf("\t%d %.2f %.2f %.2f (* %s standard dev *)\n",
			kernel+RDPMC_OFFSET,average_rdpmc[kernel],
			average_rdpmc[kernel]-deviation_rdpmc[kernel],
			average_rdpmc[kernel]+deviation_rdpmc[kernel],
			rdpmc_kernels[kernel].name);

		/* plot 25th to 75th box */
		printf("newcurve ");
		printf("marktype box marksize 0.5 %lf linetype none ",
			seventyfive_rdpmc[kernel]-twentyfive_rdpmc[kernel]);
		if (rdpmc_kernels[kernel].type==INTERFACE_PERFCTR) {
			printf("color 1.0 0.0 0.0\n");
		}
		else if (rdpmc_kernels[kernel].type==INTERFACE_PERFMON2) {
			printf("color 0.0 0.0 1.0\n");
		}
		else if (rdpmc_kernels[kernel].type==INTERFACE_PERF_EVENT) {
			printf("color 0.0 0.0 0.0\n");
		}
		else if (rdpmc_kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
			printf("color 0.3 0.3 0.3\n");
		}
		else {
			printf("color 0.3 0.3 0.0\n");
		}
		printf("pts\n");
		printf("\t%d %lf\n",kernel+RDPMC_OFFSET,
			(twentyfive_rdpmc[kernel]+seventyfive_rdpmc[kernel])/2.0);
		printf("\t(* %lf %lf %lf *)\n",median_rdpmc[kernel],twentyfive_rdpmc[kernel],seventyfive[kernel]);

		/* plot median */

		printf("newcurve ");
		printf("marktype box marksize 0.5 1 linetype none color 1.0 1.0 1.0\n");
		printf("pts\n");
		printf("\t%d %lf\n",kernel+RDPMC_OFFSET,median_rdpmc[kernel]);

		/* plot outliers */

		printf("newcurve ");
		if (rdpmc_kernels[kernel].type==INTERFACE_PERFCTR) {
			printf("marktype x linetype none color 1.0 0.0 0.0\n");
		}
		else if (rdpmc_kernels[kernel].type==INTERFACE_PERFMON2) {
			printf("marktype x linetype none color 0.0 0.0 1.0\n");
		}
		else if (rdpmc_kernels[kernel].type==INTERFACE_PERF_EVENT) {
			printf("marktype x linetype none color 0.0 0.0 0.0\n");
		}
		else if (rdpmc_kernels[kernel].type==INTERFACE_PERF_EVENT_RDPMC) {
			printf("marktype x linetype none color 0.3 0.3 0.3\n");
		}
		else {
			printf("marktype x linetype none color 0.3 0.3 0.0\n");
		}

		printf("pts\n");

		for(i=0;i<NUM_RUNS;i++) {
			if (*(get_runs(times_rdpmc,kernel,events)+i) >
				average_rdpmc[kernel] + deviation_rdpmc[kernel]) {
				printf("\t%d %lld\n",kernel+RDPMC_OFFSET,
					*(get_runs(times_rdpmc,kernel,events)+i));
			}
			if (*(get_runs(times_rdpmc,kernel,events)+i) <
				average_rdpmc[kernel]-deviation_rdpmc[kernel]) {
				printf("\t%d %lld\n",kernel+RDPMC_OFFSET,
					*(get_runs(times_rdpmc,kernel,events)+i));
			}
		}

		printf("\n");

	}


	return 0;
}


