CC	= gcc
CC_R	= $(CC) -pthread
CFLAGS	= -g -O2 -Wall -I..
LFLAGS_STATIC  = -lrt -lm -static
LFLAGS  = -lrt -lm
LFLAGS_BORING =

all:	\
	generate_results \
	generate_manyreads \
	rdtsc_null_pe \
	rdtsc_null_pe.10 \
	rdtsc_null_pe_syscall \
	rdtsc_null_pe_rdpmc \
	rdtsc_null_pe_rdpmc-map_populate \
	rdtsc_null_pe_rdpmc-touch \
	rdtsc_null_pe_static \
	rdtsc_null_pe_syscall_static \
	rdtsc_null_pe_rdpmc_static \
	rdtsc_null_pe_rdpmc-map_populate_static \
	rdtsc_null_pe_rdpmc-touch_static.10 \
	rdtsc_null_perfctr \
	rdtsc_null_perfmon2 


convert_results:	convert_results.o
	$(CC) $(LFLAGS) -o convert_results convert_results.o

convert_results.o:	convert_results.c
	$(CC) $(CFLAGS) -c convert_results.c


###

generate_results:	generate_results.o
	$(CC) $(LFLAGS_BORING) -o generate_results generate_results.o

generate_results.o:	generate_results.c
	$(CC) $(CFLAGS) -c generate_results.c

####

generate_manyreads:	generate_manyreads.o
	$(CC) $(LFLAGS_BORING) -o generate_manyreads generate_manyreads.o

generate_manyreads.o:	generate_manyreads.c
	$(CC) $(CFLAGS) -c generate_manyreads.c

####

rdtsc_null_pe:	rdtsc_null_pe.o
	$(CC) $(LFLAGS) -o rdtsc_null_pe rdtsc_null_pe.o

rdtsc_null_pe.o:	rdtsc_null_pe.c
	$(CC) $(CFLAGS) -c rdtsc_null_pe.c
####

rdtsc_null_pe_static:	rdtsc_null_pe.o
	$(CC) $(LFLAGS_STATIC) -o rdtsc_null_pe_static rdtsc_null_pe.o

####

rdtsc_null_pe_syscall:	rdtsc_null_pe_syscall.o
	$(CC) $(LFLAGS) -o rdtsc_null_pe_syscall rdtsc_null_pe_syscall.o

rdtsc_null_pe_syscall.o:	rdtsc_null_pe.c
	$(CC) $(CFLAGS) -DUSE_SYSCALL=1 -o rdtsc_null_pe_syscall.o -c rdtsc_null_pe.c

#####

rdtsc_null_pe_syscall_static:	rdtsc_null_pe_syscall.o
	$(CC) $(LFLAGS_STATIC) -o rdtsc_null_pe_syscall_static rdtsc_null_pe_syscall.o

#####

rdtsc_null_pe_rdpmc:	rdtsc_null_pe_rdpmc.o
	$(CC) $(LFLAGS) -o rdtsc_null_pe_rdpmc rdtsc_null_pe_rdpmc.o

rdtsc_null_pe_rdpmc.o:	rdtsc_null_pe_rdpmc.c
	$(CC) $(CFLAGS) -c rdtsc_null_pe_rdpmc.c
#####

rdtsc_null_pe_rdpmc_static:	rdtsc_null_pe_rdpmc.o
	$(CC) $(LFLAGS_STATIC) -o rdtsc_null_pe_rdpmc_static rdtsc_null_pe_rdpmc.o

#####

rdtsc_null_pe_rdpmc-map_populate:	rdtsc_null_pe_rdpmc-map_populate.o
	$(CC) $(LFLAGS) -o rdtsc_null_pe_rdpmc-map_populate rdtsc_null_pe_rdpmc-map_populate.o

rdtsc_null_pe_rdpmc-map_populate.o:	rdtsc_null_pe_rdpmc.c
	$(CC) $(CFLAGS) -DMAPPOPULATE -o rdtsc_null_pe_rdpmc-map_populate.o -c rdtsc_null_pe_rdpmc.c

#####

rdtsc_null_pe_rdpmc-map_populate_static:	rdtsc_null_pe_rdpmc-map_populate.o
	$(CC) $(LFLAGS_STATIC) -o rdtsc_null_pe_rdpmc-map_populate_static rdtsc_null_pe_rdpmc-map_populate.o

#####

rdtsc_null_pe_rdpmc-touch:	rdtsc_null_pe_rdpmc-touch.o
	$(CC) $(LFLAGS) -o rdtsc_null_pe_rdpmc-touch rdtsc_null_pe_rdpmc-touch.o

rdtsc_null_pe_rdpmc-touch.o:	rdtsc_null_pe_rdpmc.c
	$(CC) $(CFLAGS) -DTOUCH -o rdtsc_null_pe_rdpmc-touch.o -c rdtsc_null_pe_rdpmc.c

####

rdtsc_null_pe_rdpmc-touch_static:	rdtsc_null_pe_rdpmc-touch.o
	$(CC) $(LFLAGS_STATIC) -o rdtsc_null_pe_rdpmc-touch_static rdtsc_null_pe_rdpmc-touch.o

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



####
# HACK
####

###

rdtsc_null_pe.10:	rdtsc_null_pe.10.o
	$(CC) $(LFLAGS) -o rdtsc_null_pe.10 rdtsc_null_pe.10.o

rdtsc_null_pe.10.o:	rdtsc_null_pe.c
	$(CC) $(CFLAGS) -DNUM_READS=10 -o rdtsc_null_pe.10.o -c rdtsc_null_pe.c
####


####

rdtsc_null_pe_rdpmc-touch_static.10:	rdtsc_null_pe_rdpmc-touch.10.o
	$(CC) $(LFLAGS_STATIC) -o rdtsc_null_pe_rdpmc-touch_static.10 rdtsc_null_pe_rdpmc-touch.10.o

rdtsc_null_pe_rdpmc-touch.10.o:	rdtsc_null_pe_rdpmc.c
	$(CC) $(CFLAGS) -DTOUCH -DNUM_READS=10 -o rdtsc_null_pe_rdpmc-touch.10.o -c rdtsc_null_pe_rdpmc.c

#####



clean:
	cd results && make clean
	rm -f *.o core *~ \
	convert_results \
	generate_results \
	generate_manyreads \
	rdtsc_null_pe \
	rdtsc_null_pe.1 \
	rdtsc_null_pe.2 \
	rdtsc_null_pe.3 \
	rdtsc_null_pe.4 \
	rdtsc_null_pe.5 \
	rdtsc_null_pe.6 \
	rdtsc_null_pe.7 \
	rdtsc_null_pe.8 \
	rdtsc_null_pe.9 \
	rdtsc_null_pe.10 \
	rdtsc_null_static \
	rdtsc_null_syscall \
	rdtsc_null_syscall_static \
	rdtsc_null_pe_rdpmc \
	rdtsc_null_pe_rdpmc-map_populate \
	rdtsc_null_pe_rdpmc-touch \
	rdtsc_null_perfctr \
	rdtsc_null_perfmon2 \
	rdtsc_null_pe_static \
	rdtsc_null_pe_syscall \
	rdtsc_null_pe_syscall_static  \
	rdtsc_null_pe_rdpmc-map_populate_static \
	rdtsc_null_pe_rdpmc-touch_static




