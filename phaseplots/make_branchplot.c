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

  if (argc < 7 ) {
    fprintf(stderr,"Usage: %s benchname run1 run2 Y-axis first-label second-label\n",argv[0]);
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

  double  maxy=0.0;

      long long diff1,diff2;
    for(i=1;i<intervals;i++) {

      diff1=taken_branches[i]-taken_branches[i-1];
      diff2=branches[i]-branches[i-1];

      
      if ( ( (double)diff1/(double)diff2 )> maxy ) {
	maxy=(double)diff1/(double)diff2;
      }

    }
    maxy*=100.0;

  printf("newgraph\n");
  printf("X 5\n");
  printf("Y 1.25\n");
  printf("title font Helvetica fontsize %d y %lld : %s\n",
         FONTSIZE+2,(long long)((double)maxy*1.2),argv[1]);

  printf("xaxis\n");
  printf("size 3.0 min 0 max %d label font Helvetica fontsize %d : "
	 "Instruction Interval (100M)\n",intervals,FONTSIZE);
  printf("hash_labels fontsize %d\n",FONTSIZE);

  printf("yaxis\n");
  printf("size 0.70 min 0 max %lf label font Helvetica fontsize %d : %s\n",
	 15.0,FONTSIZE,argv[4]);
  printf("hash_labels fontsize %d\n",FONTSIZE);
  printf("legend on defaults font Helvetica fontsize %d\n",FONTSIZE);

  printf("newcurve marktype none linetype solid color 1.0 0.0 0.0\n");
  printf("label font Helvetica fontsize %d : %s\n",FONTSIZE,argv[5]);
  printf("pts\n");

    for(i=1;i<intervals;i++) {

      diff1=taken_branches[i]-taken_branches[i-1];
      diff2=branches[i]-branches[i-1];

      printf("%d\t%lf\n",i,
	     ((double)diff1/(double)diff2)*100.0);
    }

    fclose(fff);

    /* blah */

  sprintf(filename,"./results/%s/%s.gcc.sse3.perf",
	  argv[3],argv[1]);

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

  printf("newcurve marktype none linetype solid color 0.0 0.55 1.0\n");
  printf("label font Helvetica fontsize %d : %s\n",FONTSIZE,argv[6]);
  printf("pts\n");

    for(i=1;i<intervals;i++) {

      diff1=taken_branches[i]-taken_branches[i-1];
      diff2=branches[i]-branches[i-1];

      printf("%d\t%lf\n",i,
	     ((double)diff1/(double)diff2)*100.0);
    }

  return 0;
}
