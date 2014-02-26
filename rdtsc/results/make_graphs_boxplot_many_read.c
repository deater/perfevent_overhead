#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../version.h"

#include "read_data.h"

#define FONTSIZE 16

#define EVENT_TO_PLOT 1

#define NUM_READS 10

int main(int argc, char **argv) {

	int kernel,i,krg;
	int plot_type=PLOT_TYPE_START;
	int machine_num=0;
	double maxy;
	double average[NUM_READS][NUM_KERNELS],deviation[NUM_READS][NUM_KERNELS];
	double median[NUM_READS][NUM_KERNELS],twentyfive[NUM_READS][NUM_KERNELS],
		seventyfive[NUM_READS][NUM_KERNELS];

	long long *times=NULL;

	/* parse command line args */
	if (argc<4) {
		printf("Must specify machine type, machine num, "
			"and start/stop/read/total\n");
		exit(1);
	}

	machine_num=atoi(argv[2]);

	/* allocate memory */
	times=calloc(NUM_MANYREAD_KERNELS*NUM_EVENTS*NUM_RUNS,sizeof(long long));
	if (times==NULL) {
		fprintf(stderr,"Error allocating!\n");
		return -1;
	}


	/* read in data */
	plot_type=read_data(argv[1],machine_num,argv[3],MANYREAD_KERNELS,
			times);
	if (plot_type<0) {
		fprintf(stderr,"Some sort of error reading!\n");
		return -1;
	}

	for(krg=0;krg<10;krg++) {

		/* sort data */
		sort_data(times,krg,MANYREAD_KERNELS);


		/* calculate median, twentyfive, seventyfive */
		calculate_boxplot_data(times,krg,
			median[krg], twentyfive[krg],
			seventyfive[krg],
			MANYREAD_KERNELS);

		/* Calculate averages and deviation */
		calculate_deviation(times,krg,average[krg],deviation[krg],
				MANYREAD_KERNELS);

	}

	/* calculate mins and maxes */
	maxy=calculate_maxy(average[0],deviation[0],MANYREAD_KERNELS);


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
	printf("xaxis size 6.5 min %d max %d\n",0,10); //minx,maxx);
	printf("grid_gray 0.9 grid_lines\n");
	printf("hash_labels font Helvetica fontsize %d  vjc hjr rotate 45\n",
		FONTSIZE);
	printf("(* label font Helvetica fontsize %d  : Kernels *)\n",
		FONTSIZE);
	printf("no_auto_hash_marks\n");
	for(i=0;i<NUM_MANYREAD_KERNELS;i++) {
		printf("hash_at %d\n",i);
	}
	printf("no_auto_hash_labels\n");
	for(i=0;i<10;i++) {
		printf("hash_label at %d : %d\n",i,i+1);
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

	printf(" ");
	printf("\n");

	for(krg=0;krg<10;krg++) {

		for(kernel=0;kernel<NUM_MANYREAD_KERNELS;kernel++) {

			/* plot standard deviation */
			printf("newcurve ");
			if (kernel==0) {
				printf("marktype x linetype none color 0.0 0.0 0.0\n");
     			}
			else if (kernel==1) {
				printf("marktype x linetype none color 0.3 0.3 0.3\n");
			}
			else {
				printf("marktype x linetype none color 0.3 0.3 0.0\n");
			}

			printf("y_epts\n");

			printf("\t%d %.2f %.2f %.2f (* %s standard dev *)\n",
				krg,average[krg][kernel],
				average[krg][kernel]-deviation[krg][kernel],
				average[krg][kernel]+deviation[krg][kernel],
				manyread_kernels[kernel].name);

			/* plot 25th to 75th box */
			printf("newcurve ");
			printf("marktype box marksize 0.5 %lf linetype none ",
				seventyfive[krg][kernel]-twentyfive[krg][kernel]);
			if (kernel==0) {
				printf("color 0.0 0.0 0.0\n");
			}
			else if (kernel==1) {
				printf("color 0.3 0.3 0.3\n");
			}
			else {
				printf("color 0.3 0.3 0.0\n");
			}
			printf("pts\n");
			printf("\t%d %lf\n",krg,
				(twentyfive[krg][kernel]+seventyfive[krg][kernel])/2.0);
			printf("\t(* %lf %lf %lf *)\n",median[krg][kernel],twentyfive[krg][kernel],seventyfive[krg][kernel]);

			/* plot median */

			printf("newcurve ");
			printf("marktype box marksize 0.5 1 linetype none color 1.0 1.0 1.0\n");
			printf("pts\n");
			printf("\t%d %lf\n",krg,median[krg][kernel]);

			/* plot outliers */

			printf("newcurve ");

			if (kernel==0) {
				printf("marktype x linetype none color 0.0 0.0 0.0\n");
			}
			else if (kernel==1) {
				printf("marktype x linetype none color 0.3 0.3 0.3\n");
			}
			else {
				printf("marktype x linetype none color 0.3 0.3 0.0\n");
			}

			printf("pts\n");

			for(i=0;i<NUM_RUNS;i++) {
				if (*(get_runs(times,kernel,krg)+i) >
					average[krg][kernel] + deviation[krg][kernel]) {
					printf("\t%d %lld\n",krg,
						*(get_runs(times,kernel,krg)+i));
				}
				if (*(get_runs(times,kernel,krg)+i) <
					average[kernel]-deviation[kernel]) {
					printf("\t%d %lld\n",krg,
						*(get_runs(times,kernel,krg)+i));
				}
			}

			printf("\n");

		}

	}

	return 0;
}
