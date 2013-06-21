CC	= gcc
CC_R	= $(CC) -pthread
CFLAGS	= -g -O2 -Wall -I..
LFLAGS  = -lrt -lm
LFLAGS_BORING =

all:	\
	generate_results \
	rdtsc_null_pe \
	rdtsc_null_pe_rdpmc \
	rdtsc_null_perfctr \
	rdtsc_null_perfmon2 
#	make_rdtsc_pe_null_results \
#	make_rdtsc_pe_rdpmc_null_results \
#	make_rdtsc_perfctr_null_results \
#	make_rdtsc_perfmon2_null_results \
#       make_graphs_kernel_rdtsc \
#        make_graphs_kernel_rdtsc_all \
#        make_graphs_kernel_rdtsc_vary_events


convert_results:	convert_results.o
	$(CC) $(LFLAGS) -o convert_results convert_results.o

convert_results.o:	convert_results.c
	$(CC) $(CFLAGS) -c convert_results.c


###

generate_results:	generate_results.o
	$(CC) $(LFLAGS_BORING) -o generate_results generate_results.o

generate_results.o:	generate_results.c
	$(CC) $(CFLAGS) -c generate_results.c


###

make_rdtsc_pe_null_results:	make_rdtsc_pe_null_results.o
	$(CC) $(LFLAGS) -o make_rdtsc_pe_null_results make_rdtsc_pe_null_results.o

make_rdtsc_pe_null_results.o:	make_rdtsc_pe_null_results.c
	$(CC) $(CFLAGS) -c make_rdtsc_pe_null_results.c

###

make_rdtsc_pe_rdpmc_null_results:	make_rdtsc_pe_rdpmc_null_results.o
	$(CC) $(LFLAGS) -o make_rdtsc_pe_rdpmc_null_results make_rdtsc_pe_rdpmc_null_results.o

make_rdtsc_pe_rdpmc_null_results.o:	make_rdtsc_pe_rdpmc_null_results.c
	$(CC) $(CFLAGS) -c make_rdtsc_pe_rdpmc_null_results.c

###

make_rdtsc_perfctr_null_results:	make_rdtsc_perfctr_null_results.o
	$(CC) $(LFLAGS) -o make_rdtsc_perfctr_null_results make_rdtsc_perfctr_null_results.o

make_rdtsc_perfctr_null_results.o:	make_rdtsc_perfctr_null_results.c
	$(CC) $(CFLAGS) -c make_rdtsc_perfctr_null_results.c

###

make_rdtsc_perfmon2_null_results:	make_rdtsc_perfmon2_null_results.o
	$(CC) $(LFLAGS) -o make_rdtsc_perfmon2_null_results make_rdtsc_perfmon2_null_results.o

make_rdtsc_perfmon2_null_results.o:	make_rdtsc_perfmon2_null_results.c
	$(CC) $(CFLAGS) -c make_rdtsc_perfmon2_null_results.c

####

rdtsc_null_pe:	rdtsc_null_pe.o
	$(CC) $(LFLAGS) -o rdtsc_null_pe rdtsc_null_pe.o

rdtsc_null_pe.o:	rdtsc_null_pe.c
	$(CC) $(CFLAGS) -c rdtsc_null_pe.c

####

rdtsc_null_pe_rdpmc:	rdtsc_null_pe_rdpmc.o
	$(CC) $(LFLAGS) -o rdtsc_null_pe_rdpmc rdtsc_null_pe_rdpmc.o

rdtsc_null_pe_rdpmc.o:	rdtsc_null_pe_rdpmc.c
	$(CC) $(CFLAGS) -c rdtsc_null_pe_rdpmc.c

####

rdtsc_null_perfctr:	rdtsc_null_perfctr.o ../perfctr/libperfctr.a
	$(CC) $(LFLAGS) -o rdtsc_null_perfctr rdtsc_null_perfctr.o ../perfctr/libperfctr.a

rdtsc_null_perfctr.o:	rdtsc_null_perfctr.c ../perfctr/perfctr.h ../perfctr/libperfctr.h
	$(CC) $(CFLAGS) -I. -I./perfctr -c rdtsc_null_perfctr.c

####

rdtsc_null_perfmon2:	rdtsc_null_perfmon2.o ../perfmon/libpfm.a
	$(CC) $(LFLAGS_BORING) -o rdtsc_null_perfmon2 rdtsc_null_perfmon2.o ../perfmon/libpfm.a

rdtsc_null_perfmon2.o:	rdtsc_null_perfmon2.c
	$(CC) $(CFLAGS) -I. -I./perfmon2 -c rdtsc_null_perfmon2.c

####

make_graphs_kernel_rdtsc_vary_events:		make_graphs_kernel_rdtsc_vary_events.o
	$(CC) $(LFLAGS) -o make_graphs_kernel_rdtsc_vary_events 	make_graphs_kernel_rdtsc_vary_events.o

make_graphs_kernel_rdtsc_vary_events.o:		make_graphs_kernel_rdtsc_vary_events.c
	$(CC) $(CFLAGS) -c make_graphs_kernel_rdtsc_vary_events.c


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
	convert_results \
	generate_results \
	rdtsc_null_pe \
	rdtsc_null_pe_rdpmc \
	rdtsc_null_perfctr \
	rdtsc_null_perfmon2 \
	make_rdtsc_pe_null_results \
	make_rdtsc_pe_rdpmc_null_results \
	make_rdtsc_perfctr_null_results \
	make_rdtsc_perfmon2_null_results \
        make_graphs_kernel_rdtsc \
        make_graphs_kernel_rdtsc_all \
        make_graphs_kernel_rdtsc_vary_events



