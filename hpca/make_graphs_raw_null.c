#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FONTSIZE 11

#define NUM_EVENTS 16
#define NUM_RUNS 1024

#define NUM_KERNELS 4

char kernel_names[NUM_KERNELS][64]={
   "3.0-pe-raw",
   "2.6.32-pe-raw",
   "2.6.32-perfctr-raw",
   "2.6.30-perfmon2-raw",
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

char dim_colors[6][64]={
  "1.0 0.75 0.75",  /* red */
  "0.63 0.82 0.62",  /* green */
  "0.60 0.64 0.80",  /* blue */
  "1.0 0.85 0.75",  /* orange */
  "0.66 0.89 0.84",  /* cyan */
  "0.89 0.66 0.88",  /* magenta */
};

long long times[NUM_KERNELS][NUM_EVENTS][NUM_RUNS];
long long average[NUM_KERNELS][NUM_EVENTS],stdev[NUM_KERNELS][NUM_EVENTS],
          max[NUM_KERNELS][NUM_EVENTS],min[NUM_KERNELS][NUM_EVENTS];

int main(int argc, char **argv) {

  int events,run,kernel;
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
     for(events=0;events<NUM_EVENTS;events++) {
        for(run=0;run<NUM_RUNS;run++) {
	   times[kernel][events][run]=0LL;
	}
     }
  }

  fprintf(stderr,"Reading in data for %s...\n",argv[1]);

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     fprintf(stderr,"\tReading data for kernel %s: ",kernel_names[kernel]); 
     for(events=0;events<NUM_EVENTS;events++) {
        fprintf(stderr,"%d ",events);
        for(run=0;run<NUM_RUNS;run++) {
           sprintf(filename,"results/null_raw/%s/%s/%d/null_test.%d",
	           machine,kernel_names[kernel],events,run);
           fff=fopen(filename,"r");
           if (fff==NULL) {
	      fprintf(stderr,"Can't open %s\n",filename);
	      break;
           }
           if (fgets(string,BUFSIZ,fff)==NULL) break;
	   if (fgets(string,BUFSIZ,fff)==NULL) break;
	   if (fgets(string,BUFSIZ,fff)==NULL) break;
           sscanf(string,"%*s %*s %lld",&times[kernel][events][run]);
           fclose(fff);
	}
     }
     fprintf(stderr,"\n");
  }

  long long total,diffs;
  double stdev_f;

  /* calculate average, stdev */
  /* do this first, as we need it to set y axis properly */

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     for(events=0;events<NUM_EVENTS;events++) {
        total=0;
        average[kernel][events]=0;
        diffs=0;
        max[kernel][events]=times[kernel][events][0];
        min[kernel][events]=times[kernel][events][0];
        for(run=0;run<NUM_RUNS;run++) {
           total+=times[kernel][events][run];
           if (times[kernel][events][run]>max[kernel][events]) {
	     max[kernel][events]=times[kernel][events][run];
	   }
           if (times[kernel][events][run]<min[kernel][events]) {
	      min[kernel][events]=times[kernel][events][run];
	   }
	}
        average[kernel][events]=total/NUM_RUNS;
        for(run=0;run<NUM_RUNS;run++) {
           diffs+=(times[kernel][events][run]-average[kernel][events])*
	          (times[kernel][events][run]-average[kernel][events]);
	}
        diffs/=NUM_RUNS;

        stdev_f=sqrt((double)diffs);
        stdev[kernel][events]=(long long)stdev_f;
     }
  }


  maxy=0;
  for(kernel=0;kernel<NUM_KERNELS;kernel++) {
     for(events=0;events<NUM_EVENTS;events++) {
       if (((average[kernel][events]+stdev[kernel][events])) > maxy) {
	 maxy=average[kernel][events]+stdev[kernel][events];
       }
     }
  }
  maxy=( ((maxy/10)+1)*10)/1000;

    
  printf("(* Begin Graph *)\n");
  printf("newgraph\n");
  printf("\n");
  printf("X 4\n");
  printf("Y 3\n");
  printf("\n");
  printf("(* Legend *)\n");
  printf("legend custom\n");
  printf("\n");
  printf("(* Y-Axis *)\n");
  printf("yaxis size 2 min 0 max %lld\n",maxy);
  printf("grid_gray 0.9 grid_lines\n");
  //printf("mgrid_gray 0.9 mgrid_lines\n");
  printf("label font Helvetica fontsize %d  : Time (us)\n",FONTSIZE);
  printf("hash_labels font Helvetica fontsize %d\n",FONTSIZE);
  printf("\n");
  printf("(* X-Axis *)\n");
  printf("xaxis size 3 min 0 max 6\n");
  printf("hash_labels font Helvetica fontsize %d\n",FONTSIZE);
  printf("label font Helvetica fontsize %d  : Events being Measured\n",
	 FONTSIZE);
  printf("\n");
  printf("(* Title *)\n");
  printf("title font Helvetica fontsize %d y %lf : "
	 "%s - Start/Stop/Read\n",
	 FONTSIZE+2,
	 (double)maxy+((double)maxy/8.0),machine);
  printf("\n");

  /* error bars */
   
  for(kernel=0;kernel<NUM_KERNELS;kernel++) {

    //if (times[kernel][0][0]==0) continue;

     printf("newcurve marktype none linetype none color %s\n",
	   dim_colors[kernel]);
          printf("(* label hjl vjc fontsize 16 font Helvetica x 6.2 y %lf : %s *)\n",
	    (double)maxy-((double)kernel*((double)maxy/8.0)),
	    kernel_names[kernel]);

     printf("y_epts\n");

     for(events=0;events<NUM_EVENTS;events++) {

       if (average[kernel][events]!=0)
        printf("%d %lf %lf %lf (* stdev=%lf min=%lf max=%lf *)\n",events,
	   (double)average[kernel][events]/1000.0,
	   (double)(average[kernel][events]-stdev[kernel][events])/1000.0,
	   (double)(average[kernel][events]+stdev[kernel][events])/1000.0,
	   (double)stdev[kernel][events]/1000.0,
	   (double)min[kernel][events]/1000.0,
	   (double)max[kernel][events]/1000.0);

     }
  }

  double marksize_x,marksize_y;

  marksize_x=0.2;
  marksize_y=(double)maxy/25.0;

  /* points */

  for(kernel=0;kernel<NUM_KERNELS;kernel++) {

     printf("newcurve marktype %s linetype %s color %s marksize %.2f %.2f\n",
	   marktype[kernel],
	    linetype[kernel],
	    colors[kernel],
	    marksize_x,marksize_y);

          printf("(* label hjl vjc fontsize 16 font Helvetica x 6.2 y %lf : %s *)\n",
	    (double)maxy-((double)kernel*((double)maxy/8.0)),
	    kernel_names[kernel]);

     printf("pts\n");

     for(events=0;events<NUM_EVENTS;events++) {

       if (average[kernel][events]!=0)
	 printf("%d %lf\n",events,
		(double)average[kernel][events]/1000.0);
     }
  }

  return 0;
}


