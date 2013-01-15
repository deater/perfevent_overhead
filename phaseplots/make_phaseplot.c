#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FONTSIZE 10

#define MAX_INTERVALS 6000

int main(int argc, char **argv) {

  FILE *fff;
  char string[BUFSIZ];

  long long instructions[MAX_INTERVALS],
    branches[MAX_INTERVALS],taken_branches[MAX_INTERVALS];
  int intervals=0,i;
  char filename[BUFSIZ];

  if (argc < 6 ) {
    fprintf(stderr,"Usage: %s benchname runname Y-axis first-label second-label\n",argv[0]);
    exit(1);
  }

  sprintf(filename,"./results/%s/%s.gcc.sse3.perf",
	  argv[2],argv[1]);

  fff=fopen(filename,"r");
  if (fff==NULL) {
    fprintf(stderr,"Could not open %s\n",filename);
    exit(1);
  }

  instructions[0]=0;
  branches[0]=0;
  taken_branches[0]=0;

  intervals=1;

  while(1) {

    if (fgets(string,BUFSIZ,fff)==NULL) break;
    
    if (!strncmp(string,"IIP:",4)) {
       if (fgets(string,BUFSIZ,fff)==NULL) break;
       sscanf(string,"%lld",&instructions[intervals]);

       if (fgets(string,BUFSIZ,fff)==NULL) break;
       sscanf(string,"%lld",&branches[intervals]);

       if (fgets(string,BUFSIZ,fff)==NULL) break;
       sscanf(string,"%lld",&taken_branches[intervals]);

       intervals++;
       if (intervals>=MAX_INTERVALS) {
	 fprintf(stderr,"Too many intervals\n");
	 exit(1);
       }
    }
  }

  long long  maxy=0;

    for(i=1;i<intervals;i++) {
 
      if (branches[i]-branches[i-1] > maxy) maxy=branches[i]-branches[i-1];

      if (taken_branches[i]-taken_branches[i-1] > maxy) maxy=
	 taken_branches[i]-taken_branches[i-1];
    }

  printf("newgraph\n");
  printf("X 4\n");
  printf("Y 1.25\n");
  printf("title font Helvetica fontsize %d : %s\n",FONTSIZE+2,argv[1]);

  printf("xaxis\n");
  printf("size 3.0 min 0 max %d label font Helvetica fontsize %d : "
	 "Instruction Interval (100M)\n",intervals,FONTSIZE);
  printf("hash_labels fontsize %d\n",FONTSIZE);

  printf("yaxis\n");
  printf("size 0.75 min 0 max %lld label font Helvetica fontsize %d : %s\n",
	 maxy,FONTSIZE,argv[3]);
  printf("hash_labels fontsize %d\n",FONTSIZE);


  if (maxy>5000000) {
    int i;
    long long rounded_max;

    printf("no_auto_hash_labels\n");
    printf("no_auto_hash_marks\n");

    rounded_max=maxy/5000000;
    rounded_max++;
    rounded_max*=5000000;

    for(i=0;i<rounded_max;i+=rounded_max/5) {
      printf("hash_at %d\n",i);
      printf("hash_label at %d : %dM\n",i,i/1000000);
    }
  }

    printf("legend on defaults font Helvetica fontsize %d\n",FONTSIZE);

  printf("newcurve marktype none linetype solid color 1.0 0.0 0.0\n");
  printf("label font Helvetica fontsize %d : %s\n",FONTSIZE,argv[4]);
  printf("pts\n");

    for(i=1;i<intervals;i++) {
      printf("%d\t%lld\n",i,
	     branches[i]-branches[i-1]);
    }

  printf("newcurve marktype none linetype solid color 0.0 0.55 1.0\n");
  printf("label font Helvetica fontsize %d : %s\n",FONTSIZE,argv[5]);
  printf("pts\n");

    for(i=1;i<intervals;i++) {
      printf("%d\t%lld\n",i,
	     taken_branches[i]-taken_branches[i-1]);
    }

  return 0;
}