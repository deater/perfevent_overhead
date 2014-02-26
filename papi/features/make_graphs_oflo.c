#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#define FONTSIZE 10

#define NUM_OFLOS 16

#define NUM_RUNS 1024

#define NUM_KERNELS 5

char kernel_names[NUM_KERNELS][64]={
   "2.6.32-pe-papi-libpfm4",
   "3.0-pe-papi-libpfm3",
   "3.0-pe-papi-libpfm4",
   "2.6.30-perfmon2-papi",
   "2.6.32-perfctr-papi",
};

char marktype[6][64]={
   "circle",
   "box",
   "diamond",
   "triangle",
   "x",
   "cross",
};

char linetype[6][64]={
  "dotted",
  "dashed",
  "solid",
  "longdash",
  "dotdash",
  "dotdotdash",
};


char colors[6][64]={
  "1.0 0.0 0.0",  /* red */
  "0.0 0.5 0.0",  /* green */
  "0.0 0.0 1.0",  /* blue */
  "1.0 0.5 0.0",  /* orange */
  "0.0 0.7 0.7",  /* cyan */
  "1.0 0.0 1.0",  /* magenta */
};

long long times[NUM_KERNELS][NUM_OFLOS][NUM_RUNS];
double average[NUM_KERNELS][NUM_OFLOS],stdev[NUM_KERNELS][NUM_OFLOS];
long long  max[NUM_KERNELS][NUM_OFLOS],min[NUM_KERNELS][NUM_OFLOS];
int num_runs[NUM_KERNELS][NUM_OFLOS];

long long overflow_values[NUM_OFLOS]={
  5000000000LL,
  1000000000LL,
  500000000,
  100000000,
  50000000,
  10000000,
  5000000,
  1000000,
  500000,
  100000,
  50000,
  10000,
  5000,
  1000,
  500,
  100,
};

struct overflow_label_info {
  long long value;
  char name[20];
};

#define NUM_OFLO_LABELS 8

struct overflow_label_info overflow_labels[NUM_OFLO_LABELS]={
  {1000000000LL,"1B"},
  {100000000,   "100M"},
  {10000000,    "10M"},
  {1000000,     "1M"},
  {100000,      "100k"},
  {10000,       "10k"},
  {1000,        "1k"},
  {100,        "100"},
};



int main(int argc, char **argv) {

  int events,run,kernel,i;
  char filename[BUFSIZ];
  FILE *fff;
  char string[BUFSIZ];
  char *machine;
  long long maxy;
   
  if (argc<2) {
     printf("Must specify machine\n");
     exit(1);
  }
  
  machine=strdup(argv[1]);
   
  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     for(events=0;events<NUM_OFLOS;events++) {
        for(run=0;run<NUM_RUNS;run++) {
	   times[kernel][events][run]=0LL;
	}
     }
  }

  fprintf(stderr,"Reading in data for %s...\n",argv[1]);

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     fprintf(stderr,"\tReading data for kernel %s: ",kernel_names[kernel]); 
     for(events=0;events<NUM_OFLOS;events++) {
        fprintf(stderr,"%d ",events);
        for(run=0;run<NUM_RUNS;run++) {
           sprintf(filename,"results/oflo_papi/%s/%s/%d/oflo_papi.%d",
	           machine,kernel_names[kernel],events,run);
           fff=fopen(filename,"r");
           if (fff==NULL) {
	      fprintf(stderr,"Can't open %s\n",filename);
	      break;
           }
           if (fgets(string,BUFSIZ,fff)==NULL) break;
           if (fgets(string,BUFSIZ,fff)==NULL) break;
	   if (fgets(string,BUFSIZ,fff)==NULL) break;
           sscanf(string,"%*s %*s %*d %*s %lld",&times[kernel][events][run]);
           fclose(fff);
	}
        num_runs[kernel][events]=run;
	//	fprintf(stderr,"%d %d\n",run,NUM_RUNS);
     }

     fprintf(stderr,"\n");
  }

  long long total;
  double stdev_f,diffs;

  /* calculate average, stdev */
  /* do this first, as we need it to set y axis properly */

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     for(events=0;events<NUM_OFLOS;events++) {
       if (num_runs[kernel][events]==0) break;

        total=0;
        average[kernel][events]=0.0;
        diffs=0.0;
        max[kernel][events]=times[kernel][events][0];
        min[kernel][events]=times[kernel][events][0];
        for(run=0;run<num_runs[kernel][events];run++) {
           total+=times[kernel][events][run];
           if (times[kernel][events][run]>max[kernel][events]) {
	     max[kernel][events]=times[kernel][events][run];
	   }
           if (times[kernel][events][run]<min[kernel][events]) {
	      min[kernel][events]=times[kernel][events][run];
	   }
	}
        average[kernel][events]=(double)total/(double)num_runs[kernel][events];
        //printf("avg = %lf diffs=%lf\n",average[kernel][events],diffs);
        for(run=0;run<num_runs[kernel][events];run++) {
	  diffs+=((double)times[kernel][events][run]-average[kernel][events])*
	    ((double)times[kernel][events][run]-average[kernel][events]);
	}
	//printf("%lf %d\n",diffs,num_runs[kernel][events]);
        diffs/=(double)num_runs[kernel][events];

        stdev_f=sqrt(diffs);
        stdev[kernel][events]=(long long)stdev_f;
	//	printf("%lf %lf %lf\n",diffs,stdev_f,stdev[kernel][events]);
     }
  }


  maxy=0;
  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     for(events=0;events<NUM_OFLOS;events++) {
       if (((average[kernel][events]+stdev[kernel][events])) > maxy) {
	 maxy=average[kernel][events]+stdev[kernel][events];
       }
     }
  }
  maxy=( ((maxy/10)+1)*10)/1000000000;

    
  printf("(* Begin Graph *)\n");
  printf("newgraph\n");
  printf("\n");
  printf("X 4\n");
  printf("Y 1\n");
  printf("\n");
  printf("(* Legend *)\n");
  printf("legend custom\n");
  printf("\n");
  printf("(* Y-Axis *)\n");
  printf("yaxis size 0.75 max %lld log\n",maxy);
  printf("grid_gray 0.9 grid_lines\n");
  //  printf("mgrid_gray 0.9 mgrid_lines\n");
  printf("label font Helvetica fontsize %d  : Time (s)\n",FONTSIZE);
  printf("hash_labels font Helvetica fontsize %d\n",FONTSIZE);
  printf("\n");

  printf("(* X-Axis *)\n");
  printf("xaxis size 3 min 100 log\n");
  printf("hash_labels font Helvetica fontsize %d\n",FONTSIZE);
  printf("label font Helvetica fontsize %d  : "
	 "Retired Instruction Overflow Threshold\n",FONTSIZE);
  printf("no_auto_hash_labels\n");
  for(i=0;i<NUM_OFLO_LABELS;i++) {
    printf("hash_label at %lld : %s\n",overflow_labels[i].value,
	   overflow_labels[i].name);
  }
  printf("\n");
  printf("(* Title *)\n");
  printf("title font Helvetica fontsize %d y %lf : %s\n",
	 FONTSIZE+2,
	 (double)maxy*exp10(1.25/4.0),machine);
  //(double)maxy/exp10((double)kernel/4.0),
  printf("\n");
   
  for(kernel=0;kernel<NUM_KERNELS;kernel++) {

    if (times[kernel][0][0]==0) continue;

     printf("newcurve marktype %s linetype %s color %s\n",
	   marktype[kernel],
	    linetype[kernel],
	   colors[kernel]);
     //     printf("label hjl vjc fontsize 16 font Helvetica x %lf y %lf : %s\n",
     //	    10000000.0,
	    //	    (double)maxy-((double)kernel*((double)maxy/8.0)),
	    //	    Before:   
	    //         100 - 0 = 100         log10 100 = 2 
	    //         100 - 10 = 90          log10 90 = 1.95
	    //         100 - 20 = 80       1 5 10 50
            //                            10^0 10^0.5 10^1  10^2
	    //                             1    10    100   
     //	    (double)maxy/exp10((double)kernel/4.0),
     //	   kernel_names[kernel]);
     printf("y_epts\n");

     for(events=0;events<NUM_OFLOS;events++) {
       if(num_runs[kernel][events]==0) continue;

       if (average[kernel][events]!=0)
        printf("%lld %lf %lf %lf (* stdev=%lf min=%lf max=%lf runs=%d *)\n",
	   overflow_values[events],
	   (double)average[kernel][events]/1000000000.0,
	   (double)(average[kernel][events]-stdev[kernel][events])/1000000000.0,
	   (double)(average[kernel][events]+stdev[kernel][events])/1000000000.0,
	   (double)stdev[kernel][events]/1000000000.0,
	   (double)min[kernel][events]/1000000000.0,
	       (double)max[kernel][events]/1000000000.0,
	       num_runs[kernel][events]);

     }
  }

  return 0;
}


