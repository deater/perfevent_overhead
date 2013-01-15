#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  int events,run;
  char command[BUFSIZ];

  //#define machine "power6"   
  //  #define machine "amd0fh"
  #define machine "nehalem"
  //#define kernel "3.0-pe-papi-libpfm3"
  //#define kernel "3.0-pe-papi-libpfm4"
  //#define kernel "2.6.32-pe-papi-libpfm4"
//#define kernel "2.6.32-perfctr-papi"
#define kernel "2.6.30-perfmon2-papi"

  fprintf(stderr,"papi_null_results for %s %s: ",machine,kernel);
   
  for(events=0;events<16;events++) {
    fprintf(stderr,"%d ",events);
    sprintf(command,"mkdir -p results/null_papi/%s/%s/%d/",
	    machine,kernel,events);
    system(command);
    for(run=0;run<1024;run++) {
      sprintf(command,"./hpca_null_papi %s %d > results/null_papi/%s/%s/%d/null_test.%d",
	      machine,events,
	      machine,kernel,
	      events,run);
      system(command);
    }
  }
  fprintf(stderr,"\n");

  return 0;
}
