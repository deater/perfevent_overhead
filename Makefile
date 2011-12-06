CC	= gcc
CC_R	= $(CC) -pthread
CFLAGS	= -g -O2 -Wall
LFLAGS  = -lrt -lm

#PAPI_INCLUDE = -I/home/vweaver1/research/papi/papi.cvs.perfctr/src
#PAPI_LIB = /home/vweaver1/research/papi/papi.cvs.perfctr/src/libpapi.a

#PAPI_INCLUDE = -I/home/vweaver1/research/papi/papi.cvs.libpfm4/src
#PAPI_LIB = /home/vweaver1/research/papi/papi.cvs.libpfm4/src/libpapi.a

#PAPI_INCLUDE = -I/home/vweaver1/research/papi/papi.cvs/src
#PAPI_LIB = /home/vweaver1/research/papi/papi.cvs/src/libpapi.a

PAPI_INCLUDE = -I/usr/local/lib
PAPI_LIB = /usr/local/lib/libpapi.a


all:	hpca_null_papi hpca_multiplex_papi hpca_overflow_papi \
	hpca_null_pe \
	hpca_null_perfctr_slow \
	hpca_null_perfctr \
	hpca_null_perfmon2 \
	hpca_read_perfmon2 \
	hpca_read_pe \
	hpca_read_perfctr \
	rdtsc_null_pe \
	make_papi_null_results \
	make_pe_null_results \
	make_rdtsc_pe_null_results \
	make_pe_read_results \
	make_perfctr_null_results make_perfctr_null_slow_results \
	make_perfctr_read_results \
	make_perfmon2_read_results \
	make_perfmon2_null_results \
	make_mpx_results make_oflo_results 


###

hpca_null_papi:	hpca_null_papi.o
	$(CC) $(LFLAGS) -o hpca_null_papi hpca_null_papi.o \
		$(PAPI_LIB)

hpca_null_papi.o:	hpca_null_papi.c
	$(CC) $(CFLAGS) $(PAPI_INCLUDE) -c hpca_null_papi.c

###

hpca_multiplex_papi:	hpca_multiplex_papi.o
	$(CC) $(LFLAGS) -o hpca_multiplex_papi hpca_multiplex_papi.o \
		$(PAPI_LIB)

hpca_multiplex_papi.o:	hpca_multiplex_papi.c
	$(CC) $(CFLAGS) $(PAPI_INCLUDE) -c hpca_multiplex_papi.c


###

hpca_overflow_papi:	hpca_overflow_papi.o matrix_multiply.o
	$(CC) $(LFLAGS) -o hpca_overflow_papi hpca_overflow_papi.o \
		matrix_multiply.o $(PAPI_LIB)

hpca_overflow_papi.o:	hpca_overflow_papi.c
	$(CC) $(CFLAGS) $(PAPI_INCLUDE) -c hpca_overflow_papi.c


###

make_papi_null_results:	make_papi_null_results.o
	$(CC) $(LFLAGS) -o make_papi_null_results make_papi_null_results.o

make_papi_null_results.o:	make_papi_null_results.c
	$(CC) $(CFLAGS) -c make_papi_null_results.c

###

make_pe_null_results:	make_pe_null_results.o
	$(CC) $(LFLAGS) -o make_pe_null_results make_pe_null_results.o

make_pe_null_results.o:	make_pe_null_results.c
	$(CC) $(CFLAGS) -c make_pe_null_results.c

###

make_rdtsc_pe_null_results:	make_rdtsc_pe_null_results.o
	$(CC) $(LFLAGS) -o make_rdtsc_pe_null_results make_rdtsc_pe_null_results.o

make_rdtsc_pe_null_results.o:	make_rdtsc_pe_null_results.c
	$(CC) $(CFLAGS) -c make_rdtsc_pe_null_results.c

###

make_pe_read_results:	make_pe_read_results.o
	$(CC) $(LFLAGS) -o make_pe_read_results make_pe_read_results.o

make_pe_read_results.o:	make_pe_read_results.c
	$(CC) $(CFLAGS) -c make_pe_read_results.c

###

make_perfmon2_null_results:	make_perfmon2_null_results.o
	$(CC) $(LFLAGS) -o make_perfmon2_null_results make_perfmon2_null_results.o

make_perfmon2_null_results.o:	make_perfmon2_null_results.c
	$(CC) $(CFLAGS) -c make_perfmon2_null_results.c

###

make_perfmon2_read_results:	make_perfmon2_read_results.o
	$(CC) $(LFLAGS) -o make_perfmon2_read_results make_perfmon2_read_results.o

make_perfmon2_read_results.o:	make_perfmon2_read_results.c
	$(CC) $(CFLAGS) -c make_perfmon2_read_results.c

###

make_perfctr_read_results:	make_perfctr_read_results.o
	$(CC) $(LFLAGS) -o make_perfctr_read_results make_perfctr_read_results.o

make_perfctr_read_results.o:	make_perfctr_read_results.c
	$(CC) $(CFLAGS) -c make_perfctr_read_results.c


###

make_perfctr_null_results:	make_perfctr_null_results.o
	$(CC) $(LFLAGS) -o make_perfctr_null_results make_perfctr_null_results.o

make_perfctr_null_results.o:	make_perfctr_null_results.c
	$(CC) $(CFLAGS) -c make_perfctr_null_results.c

###

make_perfctr_null_slow_results:	make_perfctr_null_slow_results.o
	$(CC) $(LFLAGS) -o make_perfctr_null_slow_results make_perfctr_null_slow_results.o

make_perfctr_null_slow_results.o:	make_perfctr_null_slow_results.c
	$(CC) $(CFLAGS) -I./perfctr -c make_perfctr_null_slow_results.c

###

make_mpx_results:	make_mpx_results.o
	$(CC) $(LFLAGS) -o make_mpx_results make_mpx_results.o

make_mpx_results.o:	make_mpx_results.c
	$(CC) $(CFLAGS) -c make_mpx_results.c

###

make_oflo_results:	make_oflo_results.o
	$(CC) $(LFLAGS) -o make_oflo_results make_oflo_results.o

make_oflo_results.o:	make_oflo_results.c
	$(CC) $(CFLAGS) -c make_oflo_results.c

###

matrix_multiply.o:	matrix_multiply.c
	$(CC) $(CFLAGS) -c matrix_multiply.c


####

hpca_read_pe:	hpca_read_pe.o
	$(CC) $(LFLAGS) -o hpca_read_pe hpca_read_pe.o

hpca_read_pe.o:	hpca_read_pe.c
	$(CC) $(CFLAGS) -c hpca_read_pe.c

####

hpca_null_pe:	hpca_null_pe.o
	$(CC) $(LFLAGS) -o hpca_null_pe hpca_null_pe.o

hpca_null_pe.o:	hpca_null_pe.c
	$(CC) $(CFLAGS) -c hpca_null_pe.c

####

rdtsc_null_pe:	rdtsc_null_pe.o
	$(CC) $(LFLAGS) -o rdtsc_null_pe rdtsc_null_pe.o

rdtsc_null_pe.o:	rdtsc_null_pe.c
	$(CC) $(CFLAGS) -c rdtsc_null_pe.c


####

hpca_null_perfmon2:	hpca_null_perfmon2.o ./perfmon2/libpfm.a
	$(CC) $(LFLAGS) -o hpca_null_perfmon2 hpca_null_perfmon2.o ./perfmon2/libpfm.a

hpca_null_perfmon2.o:	hpca_null_perfmon2.c
	$(CC) $(CFLAGS) -I./perfmon2 -c hpca_null_perfmon2.c

####

hpca_read_perfmon2:	hpca_read_perfmon2.o ./perfmon2/libpfm.a
	$(CC) $(LFLAGS) -o hpca_read_perfmon2 hpca_read_perfmon2.o ./perfmon2/libpfm.a

hpca_read_perfmon2.o:	hpca_read_perfmon2.c
	$(CC) $(CFLAGS) -I./perfmon2 -c hpca_read_perfmon2.c


####

hpca_null_perfctr:	hpca_null_perfctr.o perfctr/libperfctr.a
	$(CC) $(LFLAGS) -o hpca_null_perfctr hpca_null_perfctr.o perfctr/libperfctr.a

hpca_null_perfctr.o:	hpca_null_perfctr.c perfctr/perfctr.h perfctr/libperfctr.h
	$(CC) $(CFLAGS) -I./perfctr -c hpca_null_perfctr.c


####

hpca_read_perfctr:	hpca_read_perfctr.o perfctr/libperfctr.a
	$(CC) $(LFLAGS) -o hpca_read_perfctr hpca_read_perfctr.o perfctr/libperfctr.a

hpca_read_perfctr.o:	hpca_read_perfctr.c perfctr/perfctr.h perfctr/libperfctr.h
	$(CC) $(CFLAGS) -I./perfctr -c hpca_read_perfctr.c

####

hpca_null_perfctr_slow:	hpca_null_perfctr_slow.o perfctr/libperfctr.a
	$(CC) $(LFLAGS) -o hpca_null_perfctr_slow hpca_null_perfctr_slow.o perfctr/libperfctr.a

hpca_null_perfctr_slow.o:	hpca_null_perfctr.c perfctr/perfctr.h perfctr/libperfctr.h
	$(CC) $(CFLAGS) -DUSE_SLOW -c hpca_null_perfctr.c -o hpca_null_perfctr_slow.o

#####

clean:
	rm -f *.o core *~ \
	hpca_null_papi \
	hpca_null_pe \
	hpca_null_perfctr \
	hpca_null_perfctr_slow \
	hpca_null_perfmon2 \
	hpca_read_perfmon2 \
	hpca_read_pe \
	hpca_read_perfctr \
	rdtsc_null_pe \
	hpca_multiplex_papi \
	hpca_overflow_papi \
	make_papi_null_results \
	make_pe_null_results \
	make_rdtsc_pe_null_results \
	make_pe_read_results \
	make_perfmon2_null_results \
	make_perfmon2_read_results \
	make_perfctr_null_results \
	make_perfctr_null_slow_results \
	make_perfctr_read_results \
	make_raw_null_results \
	make_mpx_results \
	make_oflo_results 

install:	all
