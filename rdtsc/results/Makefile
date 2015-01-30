CC	= gcc
CC_R	= $(CC) -pthread
CFLAGS	= -g -O2 -Wall -I..
LFLAGS  = -lrt -lm

all:	\
	make_graphs_boxplot \
	make_graphs_boxplot_gcc \
	make_graphs_boxplot_many_read \
	make_graphs_boxplot_rdpmc \
	make_graphs_boxplot_breakdown \
	make_graphs_boxplot_final \
	make_graphs_colorful \
	make_graphs_errorbars \
	make_graphs_varying \
	make_graphs_varying_final
#	rdtsc_null_pe \
#	rdtsc_null_pe_rdpmc \
#	rdtsc_null_perfctr \
#	rdtsc_null_perfmon2 \
#	make_rdtsc_pe_null_results \
#	make_rdtsc_pe_rdpmc_null_results \
#	make_rdtsc_perfctr_null_results \
#	make_rdtsc_perfmon2_null_results \
#       make_graphs_kernel_rdtsc \
#        make_graphs_kernel_rdtsc_vary_events


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
	$(CC) $(LFLAGS) -o rdtsc_null_perfmon2 rdtsc_null_perfmon2.o ../perfmon/libpfm.a

rdtsc_null_perfmon2.o:	rdtsc_null_perfmon2.c
	$(CC) $(CFLAGS) -I. -I./perfmon2 -c rdtsc_null_perfmon2.c

####

make_graphs_varying:		make_graphs_varying.o read_data.o
	$(CC) $(LFLAGS) -o make_graphs_varying 	make_graphs_varying.o read_data.o

make_graphs_varying.o:		make_graphs_varying.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_varying.c

####

make_graphs_varying_final:		make_graphs_varying_final.o read_data.o
	$(CC) $(LFLAGS) -o make_graphs_varying_final 	make_graphs_varying_final.o read_data.o

make_graphs_varying_final.o:		make_graphs_varying_final.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_varying_final.c


###

make_graphs_kernel_rdtsc:	make_graphs_kernel_rdtsc.o
	$(CC) -g $(LFLAGS) -o make_graphs_kernel_rdtsc make_graphs_kernel_rdtsc.o

make_graphs_kernel_rdtsc.o:	make_graphs_kernel_rdtsc.c
	$(CC) $(CFLAGS) -c make_graphs_kernel_rdtsc.c

#####

make_graphs_colorful:	make_graphs_colorful.o read_data.o
	$(CC) -g $(LFLAGS) -o make_graphs_colorful make_graphs_colorful.o read_data.o

make_graphs_colorful.o:	make_graphs_colorful.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_colorful.c

#####

make_graphs_errorbars:	make_graphs_errorbars.o read_data.o
	$(CC) -g $(LFLAGS) -o make_graphs_errorbars make_graphs_errorbars.o read_data.o

make_graphs_errorbars.o:	make_graphs_errorbars.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_errorbars.c


#####

make_graphs_boxplot:	make_graphs_boxplot.o read_data.o
	$(CC) -g $(LFLAGS) -o make_graphs_boxplot make_graphs_boxplot.o read_data.o

make_graphs_boxplot.o:	make_graphs_boxplot.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_boxplot.c

#####

make_graphs_boxplot_breakdown:	make_graphs_boxplot_breakdown.o read_data.o
	$(CC) -g $(LFLAGS) -o make_graphs_boxplot_breakdown make_graphs_boxplot_breakdown.o read_data.o

make_graphs_boxplot_breakdown.o:	make_graphs_boxplot_breakdown.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_boxplot_breakdown.c


#####

make_graphs_boxplot_final:	make_graphs_boxplot_final.o read_data.o
	$(CC) -g $(LFLAGS) -o make_graphs_boxplot_final make_graphs_boxplot_final.o read_data.o

make_graphs_boxplot_final.o:	make_graphs_boxplot_final.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_boxplot_final.c

#####

make_graphs_boxplot_gcc:	make_graphs_boxplot_gcc.o read_data.o
	$(CC) -g $(LFLAGS) -o make_graphs_boxplot_gcc make_graphs_boxplot_gcc.o read_data.o

make_graphs_boxplot_gcc.o:	make_graphs_boxplot_gcc.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_boxplot_gcc.c

#####

make_graphs_boxplot_many_read:	make_graphs_boxplot_many_read.o read_data_many.o
	$(CC) -g $(LFLAGS) -o make_graphs_boxplot_many_read make_graphs_boxplot_many_read.o read_data_many.o

make_graphs_boxplot_many_read.o:	make_graphs_boxplot_many_read.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_boxplot_many_read.c

#####

make_graphs_boxplot_rdpmc:	make_graphs_boxplot_rdpmc.o read_data.o
	$(CC) -g $(LFLAGS) -o make_graphs_boxplot_rdpmc make_graphs_boxplot_rdpmc.o read_data.o

make_graphs_boxplot_rdpmc.o:	make_graphs_boxplot_rdpmc.c read_data.h
	$(CC) $(CFLAGS) -c make_graphs_boxplot_rdpmc.c


#####

read_data.o:	read_data.c read_data.h
	$(CC) $(CFLAGS) -c read_data.c

#####

read_data_many.o:	read_data_many.c read_data.h
	$(CC) $(CFLAGS) -c read_data_many.c

#####

clean:
	rm -f *.o core *~ \
	rdtsc_null_pe \
	rdtsc_null_pe_rdpmc \
	rdtsc_null_perfctr \
	rdtsc_null_perfmon2 \
	make_rdtsc_pe_null_results \
	make_rdtsc_pe_rdpmc_null_results \
	make_rdtsc_perfctr_null_results \
	make_rdtsc_perfmon2_null_results \
        make_graphs_kernel_rdtsc \
	make_graphs_boxplot \
	make_graphs_boxplot_breakdown \
	make_graphs_boxplot_final \
	make_graphs_boxplot_gcc \
	make_graphs_boxplot_many_read \
	make_graphs_boxplot_rdpmc \
        make_graphs_colorful \
	make_graphs_errorbars \
        make_graphs_varying \
        make_graphs_varying_final
	cd graphs && make clean



