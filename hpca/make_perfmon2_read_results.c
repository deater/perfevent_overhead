#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  int events,run;
  char command[BUFSIZ];

  //#define machine "core2"
  //#define machine "nehalem"
  #define machine "atom"
  //#define machine "cortexA9"
  //#define machine "power6"
//  #define machine "amd10h"
//     #define machine "amd0fh"

//   #define kernel "3.0-pe-raw"
  //#define kernel "2.6.32-pe-raw"
//#define kernel "2.6.32-perfctr-raw"
#define kernel "2.6.30-perfmon2-raw"

  fprintf(stderr,"Working on %s %s: ",machine,kernel);
  for(events=0;events<16;events++) {
    fprintf(stderr,"%d ",events);
    sprintf(command,"mkdir -p results/read_raw/%s/%s/%d/",
	    machine,kernel,events);
    system(command);
    for(run=0;run<128;run++) {
      sprintf(command,"./hpca_read_perfmon2 %s %d > results/read_raw/%s/%s/%d/null_test.%d",
	      machine,events,
	      machine,kernel,
	      events,run);
      system(command);
    }
  }
  fprintf(stderr,"\n");

  return 0;
}
