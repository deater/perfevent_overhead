/* by Vince Weaver, vincent.weaver@maine.edu                               */
/* Compile with gcc -O2 -o rdtsc_null_pe_rdpmc rdtsc_null_pe_rdpmc.c       */

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

#include <sys/mman.h>

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

static unsigned long long rdtsc(void) {
  unsigned a,d;

  __asm__ volatile("rdtsc" : "=a" (a), "=d" (d));

  return ((unsigned long long)a) | (((unsigned long long)d) << 32);
}

static unsigned long long rdpmc(unsigned int counter) {
  unsigned int low, high;

  __asm__ volatile("rdpmc" : "=a" (low), "=d" (high) : "c" (counter));

  return (unsigned long long)low | ((unsigned long long)high) <<32;
}

#define barrier() __asm__ volatile("" ::: "memory")

#if 0
/* From Peter Zjilstra's demo code */
static unsigned long long mmap_read_self(void *addr, 
					 unsigned long long *enabled,
					 unsigned long long *running)
{
  struct perf_event_mmap_page *pc = addr;
  unsigned int seq;
  unsigned long long count, delta;

  do {
  again:
    seq=pc->lock;
    barrier();
    if (seq&1) goto again;

    if ((enabled || running) && pc->time_mult) {
      unsigned long long cyc = rdtsc();
      unsigned long long rem, quot;

      quot=(cyc >> pc->time_shift);
      rem = cyc & ((1 << pc->time_shift) -1);
      delta=pc->time_offset + 
	quot*pc->time_mult +
	((rem * pc->time_mult) >> pc->time_shift);
    }
    if (enabled) *enabled=pc->time_enabled+delta;
    if (running) *running=pc->time_running+delta;

    if (pc->index) {
      count=rdpmc(pc->index-1);
      count+=pc->offset;
    }
    else goto fail;

    barrier();
  } while (pc->lock != seq);

  return count;

 fail:
  /* should do slow read here */
  printf("FAIL FAIL FAIL\n");
  return 0;

}

#endif

/* __perf_event_sync_stat()    */
/* or perf_mmap() */
/* or perf_event_reset() */
/*  |  */
/* \./ */
/* perf_event_update_userpage() */
/* arch_perf_update_userpage() */
/*	userpg->index = perf_event_index(event);
        userpg->offset = perf_event_count(event);
        if (userpg->index)
                userpg->offset -= local64_read(&event->hw.prev_count);
*/

/* Ingo's code */
/* also found in tools/perf/design.txt */

static inline unsigned long long mmap_read_self(void *addr) {

	struct perf_event_mmap_page *pc = addr;
	unsigned int seq;

	unsigned long long count=0;

	/* this spins 10,000+ times? */
	do {
		/* This attempts to avoid the problem */
		/* where the mmap() page was updated */
		/* while we were calculating values */
		/* it's unclear why this should happen so frequently? */
		seq=pc->lock;
//		printf("%d\n",seq);

		barrier();

		if (pc->index) {
			count = rdpmc(pc->index - 1);
			count += pc->offset;
		}
		else {
			/* can't use rdpmc */
		}

		barrier();

	} while (pc->lock != seq);


	return count;
}



int main(int argc, char **argv) {

   int i;
   static long page_size=4096;

   long long before,after;
   long long start_before,start_after;
   long long stop_before,stop_after;
   long long read_before,read_after;

   long long init_ns=0,eventset_ns=0;
   void *addr[MAX_EVENTS];

   struct perf_event_attr pe;
   int fd[MAX_EVENTS],ret1,ret2;

   int count=0;

   unsigned long long now[MAX_EVENTS],stamp[MAX_EVENTS];
   //   unsigned long long now2[MAX_EVENTS],stamp2[MAX_EVENTS];

   if (argc<2) {
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

#ifdef MAPPOPULATE

	addr[i]=mmap(NULL,page_size, PROT_READ, MAP_SHARED|MAP_POPULATE,fd[i],0);
	if (addr[i] == (void *)(-1)) {
		printf("Error mmaping!\n");
		exit(1);
	}
#else

	addr[i]=mmap(NULL,page_size, PROT_READ, MAP_SHARED,fd[i],0);
	if (addr[i] == (void *)(-1)) {
		printf("Error mmaping!\n");
		exit(1);
	}
#endif

#ifdef TOUCH

	/* touch mmap page to make sure in cache? */
	struct perf_event_mmap_page *pc;
	pc = addr[i];
	(void)pc->index;
	printf("%d\n",pc->index);
#endif
   }



   after=rdtsc();

   eventset_ns=after-before;

   printf("Eventset_creation_%d: %lld cycles\n",i,eventset_ns);


   /* start */
   start_before=rdtsc();

   ret1=ioctl(fd[0], PERF_EVENT_IOC_ENABLE,0);


   start_after=rdtsc();

   /* read */

	/* get initial values */
   for(i=0;i<count;i++) {
     stamp[i] = mmap_read_self(addr[i]);
   }
   /* NULL */

	/* get final values */
   for(i=0;i<count;i++) {
     now[i] = mmap_read_self(addr[i]);
   }

   read_after=rdtsc();

   /* stop */
   ret2=ioctl(fd[0], PERF_EVENT_IOC_DISABLE,0);

   stop_after=rdtsc();

   stop_before=read_after;
   read_before=start_after;

#if 0
   /* this was the check for read, but we're using rdpmc now */
   if (ret3<0) {
      printf("start latency: %lld cycles\n",0LL);
      printf("stop latency: %lld cycles\n",0LL);
      printf("read latency: %lld cycles\n",0LL);
      printf("Unexpected read result %d %s\n",ret3,strerror(errno));
      exit(0);
   }
#endif

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


   for(i=0;i<count;i++) {
     printf("%x %lld\n",
	    events[i],now[i]-stamp[i]);
   }

   for(i=0;i<count;i++) {
      munmap(addr[i],page_size);
   }

   for(i=0;i<count;i++) {
      close(fd[i]);
   }

   return 0;
}
