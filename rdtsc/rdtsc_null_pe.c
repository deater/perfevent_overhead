/* by Vince Weaver, vincent.weaver@maine.edu                   */
/* Compile with gcc -O2 -o rdtsc_null_pe rdtsc_null_pe.c       */

#ifndef NUM_READS
#define NUM_READS 1 
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/utsname.h>

#include <time.h>

#include "include/perf_event.h"
#include <unistd.h>
#include <asm/unistd.h>

#include <sys/ioctl.h>
#include <errno.h>

#ifndef __NR_perf_event_open

#if defined(__i386__)
#define __NR_perf_event_open    336
#elif defined(__x86_64__)
#define __NR_perf_event_open    298
#elif defined __powerpc__
#define __NR_perf_event_open    319
#elif defined __arm__
#define __NR_perf_event_open    364
#endif
#endif


int perf_event_open(struct perf_event_attr *hw_event_uptr,
                    pid_t pid, int cpu, int group_fd, unsigned long flags) {

  return syscall(__NR_perf_event_open,hw_event_uptr, pid, cpu,
                 group_fd, flags);
}

#define MAX_EVENTS 16

int events[MAX_EVENTS];

unsigned long long rdtsc(void) {
	unsigned a,d;

	__asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

	return ((unsigned long long)a) | (((unsigned long long)d) << 32);
}

int main(int argc, char **argv) {

   int i;

   long long before,after;
   long long start_before,start_after;
   long long stop_before,stop_after;
   long long read_before,read_after;

   long long init_ns=0,eventset_ns=0;

   struct perf_event_attr pe;
   int fd[MAX_EVENTS],ret1,ret2,ret3;

   int count=0;

	if (argc < 2) {
		fprintf(stderr,"Usage: %s count event0 ... eventN\n",argv[0]);
		return -1;
	}

	if (argc>2) {
		count=atoi(argv[1]);
	}

	if (count!=argc-2) {
		fprintf(stderr,"Error!  Count mismatch!\n");
		return -1;
	}

	for(i=0;i<count;i++) {
		events[i]=strtol(argv[2+i],NULL,0);
	}


   /* measure init latency */

   before=rdtsc();

   after=rdtsc();

   init_ns=after-before;

   printf("init_latency: %lld cycles\n",init_ns);

   /* measure eventset creation */

   before=rdtsc();

   memset(&pe,0,sizeof(struct perf_event_attr));

   pe.type=PERF_TYPE_RAW;
   pe.size=sizeof(struct perf_event_attr);
   pe.read_format=PERF_FORMAT_GROUP|PERF_FORMAT_ID;

   fd[0]=-1;

   for(i=0;i<count;i++) {
     pe.config=events[i];

     if (i==0) {
        pe.disabled=1;
        pe.pinned=1;
     }
     else {
        pe.disabled=0;
        pe.pinned=0;
     }

     fd[i]=perf_event_open(&pe,0,-1,fd[0],0);
     if (fd[i]<0) {
	 count=i;
	 printf("Eventset_creation_%d: %lld cycles\n",i,0LL);
	 printf("start/stop/read latency: %lld cycles\n",0LL);
	 exit(1);
      }
   }

   after=rdtsc();

   eventset_ns=after-before;

   printf("Eventset_creation_%d: %lld cycles\n",i,eventset_ns);

   start_before=rdtsc();

   /* start */
   ret1=ioctl(fd[0], PERF_EVENT_IOC_ENABLE,0);

   start_after=rdtsc();
	// 10?

	#define BUFFER_SIZE 256
	long long buffer[NUM_READS][BUFFER_SIZE];
	long long read_times[NUM_READS];

	int krg;

	for(krg=0;krg<NUM_READS;krg++) {

#ifdef USE_SYSCALL
	ret3=syscall(__NR_read,fd[0],buffer[krg],BUFFER_SIZE*sizeof(long long));
#else
	ret3=read(fd[0],buffer[krg],BUFFER_SIZE*sizeof(long long));
#endif
		read_times[krg]=rdtsc();
	}

	read_after=rdtsc();

   /* stop */
   ret2=ioctl(fd[0], PERF_EVENT_IOC_DISABLE,0);

   stop_after=rdtsc();



	stop_before=read_after;
	read_before=start_after;

	if (ret3<0) {
		printf("start latency: %lld cycles\n",0LL);
		printf("stop latency: %lld cycles\n",0LL);
		printf("read latency: %lld cycles\n",0LL);
		printf("Unexpected read result %d %s\n",ret3,strerror(errno));
		exit(0);
	}

	/* Can indiate an error */
	/* Also can indicate we've hacked the syscall to return early */
	/* for timing purposes.                                       */
	buffer[0][0]=count;

	if (buffer[0][0]!=count){
		printf("start latency: %lld cycles\n",0LL);
		printf("stop/read latency: %lld cycles\n",0LL);
		printf("read latency: %lld cycles\n",0LL);
		printf("Error!  buffer count is %lld!\n",buffer[0][0]);
		exit(0);
	}

	printf("start latency: %lld cycles\n",start_after-start_before);
	printf("stop latency: %lld cycles\n",stop_after-stop_before);
	printf("read latency: %lld cycles\n",read_after-read_before);

	if (ret1<0) {
		printf("Error starting!\n");
		exit(1);
	}

	if (ret2<0) {
		printf("Error stopping!\n");
		exit(1);
	}

	for(krg=0;krg<NUM_READS;krg++) {
	for(i=0;i<buffer[0][0];i++) {
		printf("%d %x %lld time %lld\n",
			krg,events[i],buffer[krg][1+(i*2)],
			krg==0?read_times[krg]-read_before:
				read_times[krg]-read_times[krg-1]);
	}
	}

	for(i=0;i<count;i++) {
		close(fd[i]);
	}

	return 0;
}
