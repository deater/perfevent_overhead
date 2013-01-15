CC	= gcc
CFLAGS	= -g -O2 -Wall
LFLAGS  = -lm

all:	\
	make_graphs_papi_null make_graphs_raw_null \
	make_graphs_raw_read \
	make_graphs_mpx make_graphs_oflo \
	make_branchplot make_phaseplot \
	make_graphs_kernel_null \
	make_graphs_kernel_3events \
	make_graphs_kernel_rdtsc \
	make_graphs_kernel_rdtsc_all \
	make_graphs_kernel_rdtsc_vary_events



make_graphs_kernel_rdtsc_vary_events:		make_graphs_kernel_rdtsc_vary_events.o
	$(CC) $(LFLAGS) -o make_graphs_kernel_rdtsc_vary_events 	make_graphs_kernel_rdtsc_vary_events.o

make_graphs_kernel_rdtsc_vary_events.o:		make_graphs_kernel_rdtsc_vary_events.c
	$(CC) $(CFLAGS) -c make_graphs_kernel_rdtsc_vary_events.c

###

make_graphs_papi_null:	make_graphs_papi_null.o
	$(CC) -g $(LFLAGS) -o make_graphs_papi_null make_graphs_papi_null.o

make_graphs_papi_null.o:	make_graphs_papi_null.c
	$(CC) $(CFLAGS) -c make_graphs_papi_null.c

###

make_graphs_raw_null:	make_graphs_raw_null.o
	$(CC) -g $(LFLAGS) -o make_graphs_raw_null make_graphs_raw_null.o

make_graphs_raw_null.o:	make_graphs_raw_null.c
	$(CC) $(CFLAGS) -c make_graphs_raw_null.c

###

make_graphs_raw_read:	make_graphs_raw_read.o
	$(CC) -g $(LFLAGS) -o make_graphs_raw_read make_graphs_raw_read.o

make_graphs_raw_read.o:	make_graphs_raw_read.c
	$(CC) $(CFLAGS) -c make_graphs_raw_read.c


###

make_graphs_mpx:	make_graphs_mpx.o
	$(CC) -g $(LFLAGS) -o make_graphs_mpx make_graphs_mpx.o

make_graphs_mpx.o:	make_graphs_mpx.c
	$(CC) $(CFLAGS) -c make_graphs_mpx.c

###

make_graphs_oflo:	make_graphs_oflo.o
	$(CC) -g $(LFLAGS) -o make_graphs_oflo make_graphs_oflo.o -lm

make_graphs_oflo.o:	make_graphs_oflo.c
	$(CC) $(CFLAGS) -c make_graphs_oflo.c


###

make_branchplot:	make_branchplot.o
	$(CC) -g $(LFLAGS) -o make_branchplot make_branchplot.o

make_branchplot.o:	make_branchplot.c
	$(CC) $(CFLAGS) -c make_branchplot.c


###

make_phaseplot:	make_phaseplot.o
	$(CC) -g $(LFLAGS) -o make_phaseplot make_phaseplot.o

make_phaseplot.o:	make_phaseplot.c
	$(CC) $(CFLAGS) -c make_phaseplot.c

###

make_graphs_kernel_null:	make_graphs_kernel_null.o
	$(CC) -g $(LFLAGS) -o make_graphs_kernel_null make_graphs_kernel_null.o

make_graphs_kernel_null.o:	make_graphs_kernel_null.c
	$(CC) $(CFLAGS) -c make_graphs_kernel_null.c

###

make_graphs_kernel_3events:	make_graphs_kernel_3events.o
	$(CC) -g $(LFLAGS) -o make_graphs_kernel_3events make_graphs_kernel_3events.o

make_graphs_kernel_3events.o:	make_graphs_kernel_3events.c
	$(CC) $(CFLAGS) -c make_graphs_kernel_3events.c

###

make_graphs_kernel_rdtsc:	make_graphs_kernel_rdtsc.o
	$(CC) -g $(LFLAGS) -o make_graphs_kernel_rdtsc make_graphs_kernel_rdtsc.o

make_graphs_kernel_rdtsc.o:	make_graphs_kernel_rdtsc.c
	$(CC) $(CFLAGS) -c make_graphs_kernel_rdtsc.c

###

make_graphs_kernel_rdtsc_all:	make_graphs_kernel_rdtsc_all.o
	$(CC) -g $(LFLAGS) -o make_graphs_kernel_rdtsc make_graphs_kernel_rdtsc_all.o

make_graphs_kernel_rdtsc_all.o:	make_graphs_kernel_rdtsc_all.c
	$(CC) $(CFLAGS) -c make_graphs_kernel_rdtsc_all.c

#####

clean:
	rm -f *.o core *~ \
	make_graphs_papi_null \
	make_graphs_raw_null \
	make_graphs_raw_read \
	make_graphs_mpx \
	make_graphs_oflo \
	make_branchplot \
	make_phaseplot \
	make_graphs_kernel_null \
	make_graphs_kernel_3events \
	make_graphs_kernel_rdtsc \
	make_graphs_kernel_rdtsc_all


install:	all
