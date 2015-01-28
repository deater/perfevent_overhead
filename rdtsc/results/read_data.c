#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../version.h"

#include "read_data.h"

struct kernel_info kernels[NUM_KERNELS]={
   {"2.6.30-perfmon2",	INTERFACE_PERFMON2,},
   {"2.6.32-perfctr",	INTERFACE_PERFCTR,},
   {"2.6.32",		INTERFACE_PERF_EVENT,},
   {"2.6.33",		INTERFACE_PERF_EVENT,},
   {"2.6.34",		INTERFACE_PERF_EVENT,},
   {"2.6.35",		INTERFACE_PERF_EVENT,},
   {"2.6.36",		INTERFACE_PERF_EVENT,},
   {"2.6.37",		INTERFACE_PERF_EVENT,},
   {"2.6.38",		INTERFACE_PERF_EVENT,},
   {"2.6.39",		INTERFACE_PERF_EVENT,},
   {"3.0.0",		INTERFACE_PERF_EVENT,},
   {"3.1.0",		INTERFACE_PERF_EVENT,},
   {"3.2.0",		INTERFACE_PERF_EVENT,},
   {"3.3.0",		INTERFACE_PERF_EVENT,},
   {"3.4.0",		INTERFACE_PERF_EVENT,},
   {"3.5.0",		INTERFACE_PERF_EVENT,},
   {"3.6.0",		INTERFACE_PERF_EVENT,},
   {"3.7.0",		INTERFACE_PERF_EVENT,},
   {"3.8.0",		INTERFACE_PERF_EVENT,},
   {"3.9.0",		INTERFACE_PERF_EVENT,},
   {"3.10.0",		INTERFACE_PERF_EVENT,},
   {"3.11.0",		INTERFACE_PERF_EVENT,},
   {"3.12.0",		INTERFACE_PERF_EVENT,},
   {"3.13.0",		INTERFACE_PERF_EVENT,},
   {"3.14.0",		INTERFACE_PERF_EVENT,},
   {"3.15.0",		INTERFACE_PERF_EVENT,},
   {"3.16.0",		INTERFACE_PERF_EVENT,},
   {"3.17.0",		INTERFACE_PERF_EVENT,},
   {"3.18.0",		INTERFACE_PERF_EVENT,},
};

struct kernel_info rdpmc_kernels[NUM_RDPMC_KERNELS]={
   {"3.4.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.5.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.6.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.7.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.8.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.9.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.10.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.11.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.12.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.13.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.14.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.15.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.16.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.17.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.18.0-rdpmc",	INTERFACE_PERF_EVENT_RDPMC,},
};

struct kernel_info gcc_kernels[NUM_GCC_KERNELS]={
   {"3.10.0gcc44",	INTERFACE_PERF_EVENT,},
   {"3.10.0gcc45",	INTERFACE_PERF_EVENT,},
   {"3.10.0gcc46",	INTERFACE_PERF_EVENT,},
   {"3.10.0gcc47",	INTERFACE_PERF_EVENT,},
   {"3.10.0gcc48",	INTERFACE_PERF_EVENT,},
};

struct kernel_info manyread_kernels[NUM_MANYREAD_KERNELS]={
   {"3.14.0-read_many",	INTERFACE_PERF_EVENT,},
   {"3.14.0-rdpmc-map_populate-read_many",	INTERFACE_PERF_EVENT,},
};


struct kernel_info breakdown_kernels[NUM_BREAKDOWN_KERNELS]={
   {"3.10.0-static-perf_read",	INTERFACE_PERF_EVENT,},
   {"3.10.0-static-perf_read_hw",	INTERFACE_PERF_EVENT,},
   {"3.10.0-static-perf_read_value",	INTERFACE_PERF_EVENT,},
   {"3.10.0-static-perf_event_read",	INTERFACE_PERF_EVENT,},
   {"3.10.0-static-__perf_event_read",	INTERFACE_PERF_EVENT,},
   {"3.10.0-static-x86_pmu_read",	INTERFACE_PERF_EVENT,},
   {"3.10.0-static-rdpmcl",	INTERFACE_PERF_EVENT,},
   {"3.10.0-static",	INTERFACE_PERF_EVENT,},
};


struct kernel_info final_kernels[NUM_FINAL_KERNELS]={
   {"3.10.0",				INTERFACE_PERF_EVENT,},
   {"3.10.0-syscall",			INTERFACE_PERF_EVENT,},
   {"3.10.0-static",			INTERFACE_PERF_EVENT,},
   {"3.10.0-syscall_static",		INTERFACE_PERF_EVENT,},
   {"2.6.30-perfmon2",			INTERFACE_PERFMON2,},
   {"3.10.0-rdpmc",			INTERFACE_PERF_EVENT_RDPMC,},
   {"3.10.0-rdpmc-map_populate",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.10.0-rdpmc-touch",		INTERFACE_PERF_EVENT_RDPMC,},
   {"3.10.0-rdpmc_static",			INTERFACE_PERF_EVENT_RDPMC,},
   {"3.10.0-rdpmc-map_populate_static",	INTERFACE_PERF_EVENT_RDPMC,},
   {"3.10.0-rdpmc-touch_static",		INTERFACE_PERF_EVENT_RDPMC,},
   {"2.6.32-perfctr",			INTERFACE_PERFCTR,},
};

struct kernel_info varying_kernels[NUM_VARYING_KERNELS]={
   {"3.10.0-static",			INTERFACE_PERF_EVENT,},
   {"3.10.0-rdpmc-touch_static",	INTERFACE_PERF_EVENT_RDPMC,},
   {"2.6.30-perfmon2",			INTERFACE_PERFMON2,},
   {"2.6.32-perfctr",			INTERFACE_PERFCTR,},
};



char colors[NUM_KERNELS][64]={
  "0.0 0.0   1.0",   /* blue */
  "1.0 0.0   0.0",   /* red */
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
  "0.25 1.0   0.0",   /* ??? */
  "0.5 1.0   0.0",   /* ??? */
};

//long long times[NUM_KERNELS][NUM_EVENTS][NUM_RUNS];
//long long rdpmc_times[NUM_RDPMC_KERNELS][NUM_EVENTS][NUM_RUNS];

static int debug=0;

struct cpuinfo_t cpuinfo;

long long *get_runs(long long *pointer,int kernel, int event) {

        return pointer+ (kernel*NUM_EVENTS*NUM_RUNS)+(event*NUM_RUNS);

}

static int get_num_kernels(int type) {

	int num_kernels;

	if (type==STANDARD_KERNELS) {
		num_kernels=NUM_KERNELS;
	}
	else if (type==RDPMC_KERNELS) {
		num_kernels=NUM_RDPMC_KERNELS;
	}
	else if (type==GCC_KERNELS) {
		num_kernels=NUM_GCC_KERNELS;
	}
	else if (type==BREAKDOWN_KERNELS) {
		num_kernels=NUM_BREAKDOWN_KERNELS;
	}
	else if (type==FINAL_KERNELS) {
		num_kernels=NUM_FINAL_KERNELS;
	}
	else if (type==VARYING_KERNELS) {
		num_kernels=NUM_VARYING_KERNELS;
	}
	else if (type==MANYREAD_KERNELS) {
		num_kernels=NUM_MANYREAD_KERNELS;
	}
	else {
		fprintf(stderr,"UNKNOWN TYPE!\n");
		exit(1);
	}
	return num_kernels;
}

int read_data(char *machine,
		int which,
		char *plot_name,
		int type,
		long long *times) {

	int events,run,kernel,runs;
	char filename[BUFSIZ];
	FILE *fff;
	char string[BUFSIZ];
	char *result;
	int plot_type=PLOT_TYPE_START;
	long long times_start,times_stop,times_read;
	int num_kernels;

	char hostname[BUFSIZ]="Unknown",kernel_name[BUFSIZ]="Unknown";
	char gathered_version[BUFSIZ]="Unknown";
	int interface=INTERFACE_PERF_EVENT;

	num_kernels=get_num_kernels(type);

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

	for(kernel=0;kernel<num_kernels;kernel++) {
		for(events=0;events<NUM_EVENTS;events++) {
			for(run=0;run<NUM_RUNS;run++) {
				*(get_runs(times,kernel,events)+run)=0LL;
			}
     		}
  	}

	fprintf(stderr,"Reading in data for %s...\n",machine);

	for(kernel=0;kernel<num_kernels;kernel++) {
		if (type==STANDARD_KERNELS) {
			fprintf(stderr,"\tReading data for kernel %s\n",
				kernels[kernel].name);
		}
		else if (type==RDPMC_KERNELS) {
			fprintf(stderr,"\tReading data for kernel %s\n",
				rdpmc_kernels[kernel].name);
		} else if (type==GCC_KERNELS) {
			fprintf(stderr,"\tReading data for kernel %s\n",
				gcc_kernels[kernel].name);
		} else if (type==BREAKDOWN_KERNELS) {
			fprintf(stderr,"\tReading data for kernel %s\n",
				breakdown_kernels[kernel].name);
		} else if (type==FINAL_KERNELS) {
			fprintf(stderr,"\tReading data for kernel %s\n",
				final_kernels[kernel].name);
		} else if (type==VARYING_KERNELS) {
			fprintf(stderr,"\tReading data for kernel %s\n",
				varying_kernels[kernel].name);
		} else if (type==MANYREAD_KERNELS) {
			fprintf(stderr,"\tReading data for kernel %s\n",
				manyread_kernels[kernel].name);
		}


     		for(events=0;events<NUM_EVENTS;events++) {
			fprintf(stderr,"%d ",events);

			if (type==STANDARD_KERNELS) {
				sprintf(filename,"%s/%d/%s/%d/results",
					machine,which,
					kernels[kernel].name,
					events);
			}
			else if (type==RDPMC_KERNELS) {
				sprintf(filename,"%s/%d/%s/%d/results",
					machine,which,
					rdpmc_kernels[kernel].name,
					events);
			} else if (type==GCC_KERNELS) {
				sprintf(filename,"%s/%d/%s/%d/results",
					machine,which,
					gcc_kernels[kernel].name,
					events);
			} else if (type==BREAKDOWN_KERNELS) {
				sprintf(filename,"%s/%d/%s/%d/results",
					machine,which,
					breakdown_kernels[kernel].name,
					events);
			} else if (type==FINAL_KERNELS) {
				sprintf(filename,"%s/%d/%s/%d/results",
					machine,which,
					final_kernels[kernel].name,
					events);
			} else if (type==VARYING_KERNELS) {
				sprintf(filename,"%s/%d/%s/%d/results",
					machine,which,
					varying_kernels[kernel].name,
					events);
			} else if (type==MANYREAD_KERNELS) {
				sprintf(filename,"%s/%d/%s/%d/results",
					machine,which,
					manyread_kernels[kernel].name,
					events);
			}

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

			// printf("%d %lld\n",run,times_start);

			if (plot_type==PLOT_TYPE_START) {
				*(get_runs(times,kernel,events)+run)=times_start;
			}
			if (plot_type==PLOT_TYPE_STOP) {
				*(get_runs(times,kernel,events)+run)=times_stop;
			}
			if (plot_type==PLOT_TYPE_READ) {
				*(get_runs(times,kernel,events)+run)=times_read;
			}
			if (plot_type==PLOT_TYPE_TOTAL) {
				*(get_runs(times,kernel,events)+run)=times_start+times_stop+times_read;
			}
			run++;

			if (run<runs) goto loop;

			fclose(fff);
		}
		fprintf(stderr,"\n");
	}

	return plot_type;
}


//#include <string.h>

static int comp(const void *av,const void *bv) {

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

int sort_data(long long *times, int events, int type) {

	int kernel,num_kernels;

	num_kernels=get_num_kernels(type);

  	/* sort data */

	for(kernel=0;kernel<num_kernels;kernel++) {
		qsort(get_runs(times,kernel,events),NUM_RUNS,
			sizeof(long long),comp);
  	}
	return 0;
}

int calculate_boxplot_data(long long *times, int events,
			double *median, double *twentyfive,
			double *seventyfive,
			int type) {


	int kernel,num_kernels;

	num_kernels=get_num_kernels(type);


	/* calculate median, twentyfive, seventyfive */
	for(kernel=0;kernel<num_kernels;kernel++) {
		median[kernel]=*(get_runs(times,kernel,events)+(NUM_RUNS/2));
		twentyfive[kernel]=*(get_runs(times,kernel,events)+(NUM_RUNS/4));
		seventyfive[kernel]=*(get_runs(times,kernel,events)+((NUM_RUNS*3)/4));
	}

	return 0;

}

int calculate_deviation(long long *times, int events,
			double *average,double *deviation,
			int type) {

	/* Calculate averages */
	double dev,temp;
	int kernel,run;
	double total;
	int num_kernels;

	num_kernels=get_num_kernels(type);

	for(kernel=0;kernel<num_kernels;kernel++) {
		total=0.0;
		for(run=0;run<NUM_RUNS;run++) {
			total+=(double)*(get_runs(times,kernel,events)+run);
		}
		average[kernel]=total/(double)NUM_RUNS;
		// printf("%s: avg=%.2f\n",kernel_names[kernel],average[kernel]);
	}

	/* Calculate Deviation */
	for(kernel=0;kernel<num_kernels;kernel++) {
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
	return 0;
}


int calculate_maxy(double *average, double *deviation, int type) {

	/* calculate mins and maxes */
	int maxy=0;
	int kernel;
	int num_kernels;

	num_kernels=get_num_kernels(type);

	for(kernel=0;kernel<num_kernels;kernel++) {
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


	return maxy;
}


