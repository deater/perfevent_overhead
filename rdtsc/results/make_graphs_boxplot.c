#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../version.h"

#include "read_data.h"

#define FONTSIZE 16

#define EVENT_TO_PLOT 1

int comp(const void *av,const void *bv) {

	const long long *a,*b;

	a=av; b=bv;

	if (*a==*b) {
		return 0;
	}

	if (*a < *b) {
		return -1;
	}

	return 1;
}

long long *times=NULL;


static long long *get_runs(long long *pointer,int kernel, int event) {

	return pointer+ (kernel*NUM_EVENTS*NUM_RUNS)+(event*NUM_RUNS);

}

int main(int argc, char **argv) {

	int events,run,kernel,i;
	int plot_type=PLOT_TYPE_START;
	int machine_num=0;
	double maxy;
	double total;
	double average[NUM_KERNELS];

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


	/* read in data */
	plot_type=read_data(argv[1],machine_num,argv[3],STANDARD_KERNELS,
			times);
	if (plot_type<0) {
		fprintf(stderr,"Some sort of error reading!\n");
		return -1;
	}

	events=EVENT_TO_PLOT;

  	/* sort data */

	for(kernel=0;kernel<NUM_KERNELS;kernel++) {
		qsort(get_runs(times,kernel,events),NUM_RUNS,sizeof(long long),comp);
  	}

	double median[NUM_KERNELS],twentyfive[NUM_KERNELS],
		seventyfive[NUM_KERNELS];

	/* calculate median, twentyfive, seventyfive */
	for(kernel=0;kernel<NUM_KERNELS;kernel++) {
		median[kernel]=*(get_runs(times,kernel,events)+(NUM_RUNS/2));
		twentyfive[kernel]=*(get_runs(times,kernel,events)+(NUM_RUNS/4));
		seventyfive[kernel]=*(get_runs(times,kernel,events)+((NUM_RUNS*3)/4));
	}


	/* Calculate averages */
	double deviation[NUM_KERNELS],dev,temp;

	for(kernel=0;kernel<NUM_KERNELS;kernel++) {
		total=0.0;
		for(run=0;run<NUM_RUNS;run++) {
			total+=(double)*(get_runs(times,kernel,events)+run);
		}
		average[kernel]=total/(double)NUM_RUNS;
		// printf("%s: avg=%.2f\n",kernel_names[kernel],average[kernel]);
	}

	/* Calculate Deviation */
	for(kernel=0;kernel<NUM_KERNELS;kernel++) {
		dev=0.0;
		for(run=0;run<NUM_RUNS;run++) {
			temp=(double)(*(get_runs(times,kernel,events)+run))-
				average[kernel];
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

	if (maxy>10000) {
		maxy=((((long long)maxy)/10000)+1)*10000;
	}
	else if (maxy>1000) {
		maxy=((((long long)maxy)/1000)+1)*1000;
	}
	else {
		maxy=((((long long)maxy)/100)+1)*100;
	}

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
		"%s Overhead of ", FONTSIZE,
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

	return 0;
}


