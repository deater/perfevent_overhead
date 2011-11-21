#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  int events,run,num_runs;
  char command[BUFSIZ];

  //#define machine "core2"   
    #define machine "nehalem"
  //#define machine "atom"
  //#define machine "amd10h"
  //  #define machine "amd0fh"
  //#define machine "cortexA9"
  //#define machine "power6"

  //  #define kernel "3.0-pe-papi-libpfm3"
  //  #define kernel "3.0-pe-papi-libpfm4"
  //  #define kernel "2.6.32-pe-papi-libpfm4"
  //#define kernel "2.6.32-perfctr-papi"
#define kernel "2.6.30-perfmon2-papi"

#define NUM_RUNS 64
#define NUM_OVERFLOWS 16

  long long overflow_values[NUM_OVERFLOWS]={
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
    // too small    50,
    //10,
  };

  fprintf(stderr,"Making %s %s: ",machine,kernel);
   
  for(events=0;events<NUM_OVERFLOWS;events++) {
    fprintf(stderr,"%lld ",overflow_values[events]);
    sprintf(command,"mkdir -p results/oflo_papi/%s/%s/%d/",
	    machine,kernel,events);
    system(command);

    if (overflow_values[events]>1000) num_runs=NUM_RUNS;
    else num_runs=5;

    for(run=0;run<num_runs;run++) {
      sprintf(command,"./hpca_overflow_papi %s %lld > results/oflo_papi/%s/%s/%d/oflo_papi.%d",
	      machine,overflow_values[events],
	      machine,kernel,
	      events,run);
      system(command);
    }
  }
  fprintf(stderr,"\n");

  return 0;
}
