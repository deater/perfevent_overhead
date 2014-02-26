/* This file attempts to test the retired instruction */
/* performance counter on various architectures, as   */
/* implemented by the PAPI_TOT_INS counter.           */

/* by Vince Weaver, vweaver1@eecs.utk.edu             */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "papiStdEventDefs.h"
#include "papi.h"

#include "instructions_testcode.h"

#define NUM_RUNS 100


   /* Test a simple loop of 1 million instructions             */
   /* Most implementations should count be correct within 1%   */
   /* This loop in in assembly language, as compiler generated */
   /* code varies too much.                                    */

void test_million(int repeats) {

	int i,r,retval;

	int result=0;
	long long counts[8192],high=0,low=0,total=0,average=0,final_count;
	int EventSet = PAPI_NULL;

	retval = PAPI_create_eventset(&EventSet);
	if (retval!=PAPI_OK) {
		fprintf(stderr,"Error creating eventset!\n");
		return;
	}

	retval = PAPI_add_event(EventSet,PAPI_TOT_CYC);
	if (retval!=PAPI_OK) {
		fprintf(stderr,"Error adding event!\n");
		return;
	}

	printf("Testing a loop of 1 million instructions (%d times) %d:\n",
		NUM_RUNS,repeats);

	for(i=0;i<NUM_RUNS;i++) {

		PAPI_start(EventSet);

		for(r=0;r<repeats;r++) {

			result=instructions_million();

			PAPI_read(EventSet,&counts[r]);

		}

		PAPI_stop(EventSet,&final_count);

		if (result==CODE_UNIMPLEMENTED) {
			fprintf(stderr,"\tCode unimplemented\n");
			exit(1);
		}

		for(r=0;r<repeats;r++) {
			if (r==0) printf("%d %lld\n",r,counts[0]);
			else printf("%d %lld\n",r,counts[r]-counts[r-1]);
		}
		if (counts[0]>high) high=counts[0];
		if ((low==0) || (counts[0]<low)) low=counts[0];
		total+=counts[0];
	}

	average=total/NUM_RUNS;

	printf("average: %lld\n",average);

}


int main(int argc, char **argv) {

	int retval;

	retval = PAPI_library_init(PAPI_VER_CURRENT);
	if (retval != PAPI_VER_CURRENT) {
		fprintf(stderr, "Error! PAPI_library_init %d\n", retval);
		exit(1);
	}

	retval = PAPI_query_event(PAPI_TOT_INS);
	if (retval != PAPI_OK) {
		fprintf(stderr,"PAPI_TOT_INS not supported\n");
		exit(1);
	}

	printf("\n");

	test_million(10);

	PAPI_shutdown();

	return 0;
}
