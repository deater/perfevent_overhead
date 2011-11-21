/* by Vince Weaver, vweaver1@eecs.utk.edu                      */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <time.h>

#include "papiStdEventDefs.h"
#include "papi.h"

#include "matrix_multiply.h"

long long convert_to_ns(struct timespec *before,
			struct timespec *after) {

  long long seconds;
  long long ns;

  seconds=after->tv_sec - before->tv_sec;
  ns = after->tv_nsec - before->tv_nsec;

  ns = (seconds*1000000000ULL)+ns;

  return ns;
}


static int total = 0;                              /* total overflows */


void
handler( int EventSet, void *address, long long overflow_vector, void *context )
{

  ( void ) context;

  total++;
}


char core2_event[64]="INSTRUCTIONS_RETIRED";
char atom_event[64]="INSTRUCTIONS_RETIRED";
char amd10h_event[64]="RETIRED_INSTRUCTIONS";
char nehalem_event[64]="INSTRUCTIONS_RETIRED";
char power6_event[64]="PM_INST_CMPL";
char cortexA9_event[64]="INST_OUT_OF_RENAME_STAGE";

char event_name[64];

int main(int argc, char **argv) {

   int retval;

   int EventSet=PAPI_NULL;

   int event;
   long long *counts;
   
   struct timespec before,after;
   struct timespec start_before;
   struct timespec stop_after;

   long long init_ns=0,eventset_ns=0;
   long long ns=0;

   int count=1;
   long long oflo_value=10000;

   char *machine_name;

   if (argc>2) {
     oflo_value=atoll(argv[2]);
     machine_name=strdup(argv[1]);
   }
   else {
     count=1;
     machine_name=strdup("core2");
   }
   counts=calloc(count,sizeof(long long));

   if (!strncmp(machine_name,"core2",5)) {
      strncpy(event_name,core2_event,64);
   }
   else if (!strncmp(machine_name,"nehalem",7)) {
      strncpy(event_name,nehalem_event,64);
   }
   else if (!strncmp(machine_name,"atom",4)) {
      strncpy(event_name,atom_event,64);
   }
   else if (!strncmp(machine_name,"amd10h",6)) {
      strncpy(event_name,amd10h_event,64);
   }
   else if (!strncmp(machine_name,"amd0fh",6)) {
      strncpy(event_name,amd10h_event,64);
   }
   else if (!strncmp(machine_name,"power6",6)) {
      strncpy(event_name,power6_event,64);
   }
   else if (!strncmp(machine_name,"cortexA9",8)) {
      strncpy(event_name,cortexA9_event,64);
   }
   else {
      fprintf(stderr,"Unknown machine name %s\n",machine_name);
      exit(0);
   }
   
   /* measure init latency */

   clock_gettime(CLOCK_REALTIME,&before);

   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT) {
      fprintf(stderr,"ERROR: PAPI_library_init %d\n", retval);
   }

   clock_gettime(CLOCK_REALTIME,&after);

   init_ns=convert_to_ns(&before,&after);

   printf("PAPI_library_init_latency: %lld ns\n",init_ns);

   /* measure eventset creation */

   clock_gettime(CLOCK_REALTIME,&before);

   retval = PAPI_create_eventset(&EventSet);
   if (retval!=PAPI_OK) {
     fprintf(stderr,"PAPI_create_eventset failed\n");
   }

   retval=PAPI_event_name_to_code(event_name,&event);
   if (retval!=PAPI_OK) {
      fprintf(stderr,"PAPI_event_name_to_code failed %s\n",event_name);
   }

   retval=PAPI_add_event(EventSet,event);
   if (retval!=PAPI_OK) {
     fprintf(stderr,"Could not add event!\n");
	 exit(1);
   }

   retval = PAPI_overflow( EventSet, event, 
			      oflo_value, 0, handler );
   

   clock_gettime(CLOCK_REALTIME,&after);

   eventset_ns=convert_to_ns(&before,&after);

   printf("Eventset_creation_%lld: %lld ns\n",oflo_value,eventset_ns);

   clock_gettime(CLOCK_REALTIME,&start_before);
   
   PAPI_start(EventSet);
   
   naive_matrix_multiply(1);

   PAPI_stop(EventSet,counts);

   clock_gettime(CLOCK_REALTIME,&stop_after);

   ns=convert_to_ns(&start_before,&stop_after);

   retval = PAPI_overflow( EventSet, event, 0, 0, handler );

   printf("Overflow %lld %d times: %lld ns\n",oflo_value,total,ns);

   PAPI_shutdown();
   
   return 0;
}
