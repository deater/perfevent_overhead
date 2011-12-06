#include <stdio.h>
#include <stdlib.h>

#define NUM_EVENTS 8

int main(int argc, char **argv) {

  int events,run;
  char command[BUFSIZ];

#define machine "core2"
  //#define machine "nehalem"
  //  #define machine "atom"
  //#define machine "cortexA9"
  //#define machine "power6"
//  #define machine "amd10h"
//     #define machine "amd0fh"

//#define kernel "2.6.32-pe-raw"
//#define kernel "2.6.33-pe-raw"
//#define kernel "2.6.34-pe-raw"
//#define kernel "2.6.35-pe-raw"
//#define kernel "2.6.36-pe-raw"
//#define kernel "2.6.37-pe-raw"
//#define kernel "2.6.38-pe-raw"
//#define kernel "2.6.39-pe-raw"
#define kernel "3.0-pe-raw"
//#define kernel "3.1-pe-raw"
  
//#define kernel "2.6.32-perfctr-raw"
//#define kernel "2.6.30-perfmon2-raw"

  fprintf(stderr,"Working on %s %s: ",machine,kernel);
  for(events=0;events<NUM_EVENTS;events++) {
    fprintf(stderr,"%d ",events);
    sprintf(command,"mkdir -p results/read_raw/%s/%s/%d/",
	    machine,kernel,events);
    system(command);
    for(run=0;run<128;run++) {
      sprintf(command,"taskset 0x1 ./hpca_read_pe %s %d > results/read_raw/%s/%s/%d/null_test.%d",
	      machine,events,
	      machine,kernel,
	      events,run);
      system(command);
    }
  }
  fprintf(stderr,"\n");

  return 0;
}
