#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <string.h>

int main(int argc, char **argv) {

  int events,run;
  char command[BUFSIZ];

#define NUM_EVENTS 8

#define machine "core2"
  //#define machine "nehalem"
  //  #define machine "atom"
  //#define machine "cortexA9"
  //#define machine "power6"
//  #define machine "amd10h"
//     #define machine "amd0fh"

//#define kernel "2.6.32-pe"
//#define kernel "2.6.33-pe"
//#define kernel "2.6.34-pe"
//#define kernel "2.6.35-pe"
//#define kernel "2.6.36-pe"
//#define kernel "2.6.37-pe"
//#define kernel "2.6.38-pe"
//#define kernel "2.6.39-pe"
//#define kernel "3.0-pe"
//#define kernel "3.1-pe"
//#define kernel "git-bisect-3.0-3.1-1"  
//#define kernel "git-bisect-3.0-3.1-2"  
//#define kernel "git-bisect-3.0-3.1-3"  
//#define kernel "git-bisect-3.0-3.1-4"
//#define kernel "git-bisect-3.0-3.1-5"
//#define kernel "git-bisect-3.0-3.1-6"
//#define kernel "git-bisect-3.0-3.1-7"
//#define kernel "git-bisect-3.0-3.1-8"
//#define kernel "git-bisect-3.0-3.1-9"
//#define kernel "git-bisect-3.0-3.1-10"
//#define kernel "git-bisect-3.0-3.1-11"
//#define kernel "git-bisect-3.0-3.1-12"
//#define kernel "git-bisect-3.0-3.1-g950d0a1"
//#define kernel "git-bisect-3.0-3.1-g1380516"
//#define kernel "git-bisect-3.0-3.1-ged9f373"
//#define kernel "git-bisect-3.0-3.1-g83c1b31"
//#define kernel "git-bisect-3.0-3.1-g69f1d1a"
//#define kernel "git-bisect-3.0-3.1-g07e729c"
//#define kernel "git-bisect-3.0-3.1-g3960ef3"
//#define kernel "git-bisect-3.0-3.1-g4d09a93"
//#define kernel "git-bisect-3.0-3.1-g605baf6"
//#define kernel "git-bisect-3.0-3.1-g1e09939"
//#define kernel "git-bisect-3.0-3.1-g1ad9205"
//#define kernel "git-bisect-3.0-3.1-g1a26bd7"
//#define kernel "git-bisect-3.0-3.1-g5ddac6b"
//#define kernel "git-bisect-3.0-3.1-g2d86a3f"
//#define kernel "git-bisect-3.0-3.1-gacac4e3"
//#define kernel "git-bisect-3.0-3.1-gd1a05b6"
//#define kernel "git-bisect-3.0-3.1-gfc92805"
//#define kernel "git-bisect-3.0-3.1-gbb2a0de"
//#define kernel "git-bisect-3.0-3.1-g4508378"
//#define kernel "git-bisect-3.0-3.1-g1af8efe"

//#define kernel "2.6.32-perfctr-raw"
//#define kernel "2.6.30-perfmon2-raw"

  char kernel[BUFSIZ];

  struct utsname uname_info;
  
  uname(&uname_info);
  
  sprintf(kernel,"%s-perfctr",uname_info.release);
  
  fprintf(stderr,"Working on %s %s: ",machine,kernel);
  for(events=0;events<NUM_EVENTS;events++) {
    fprintf(stderr,"%d ",events);
    sprintf(command,"mkdir -p results/rdtsc_null_raw/%s/%s/%d/",
	    machine,kernel,events);
    system(command);
    for(run=0;run<1024;run++) {
      sprintf(command,"taskset 0x1 ./rdtsc_null_perfctr %s %d > results/rdtsc_null_raw/%s/%s/%d/null_test.%d",
	      machine,events,
	      machine,kernel,
	      events,run);
      system(command);
    }
  }
  fprintf(stderr,"\n");

  return 0;
}
