CC	= gcc
CC_R	= $(CC) -pthread
CFLAGS	= -g -O2 -Wall -I..
LFLAGS  = -lrt -lm

all:	hpca_null_pe \
	hpca_null_perfctr_slow \
	hpca_null_perfctr \
	hpca_null_perfmon2 \
	hpca_read_perfmon2 \
	hpca_read_pe \
	hpca_read_perfctr \
	make_pe_null_results \
	make_pe_read_results \
	make_perfctr_null_results make_perfctr_null_slow_results \
	make_perfctr_read_results \
	make_perfmon2_read_results \
	make_perfmon2_null_results 



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
	$(CC) $(CFLAGS) -I. -I./perfctr -c make_perfctr_null_slow_results.c

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

hpca_null_perfmon2:	hpca_null_perfmon2.o ../perfmon/libpfm.a
	$(CC) $(LFLAGS) -o hpca_null_perfmon2 hpca_null_perfmon2.o ../perfmon/libpfm.a

hpca_null_perfmon2.o:	hpca_null_perfmon2.c
	$(CC) $(CFLAGS) -I. -I./perfmon2 -c hpca_null_perfmon2.c

####

hpca_read_perfmon2:	hpca_read_perfmon2.o ../perfmon/libpfm.a
	$(CC) $(LFLAGS) -o hpca_read_perfmon2 hpca_read_perfmon2.o ../perfmon/libpfm.a

hpca_read_perfmon2.o:	hpca_read_perfmon2.c
	$(CC) $(CFLAGS) -I. -I./perfmon2 -c hpca_read_perfmon2.c


####

hpca_null_perfctr:	hpca_null_perfctr.o ../perfctr/libperfctr.a
	$(CC) $(LFLAGS) -o hpca_null_perfctr hpca_null_perfctr.o ../perfctr/libperfctr.a

hpca_null_perfctr.o:	hpca_null_perfctr.c ../perfctr/perfctr.h ../perfctr/libperfctr.h
	$(CC) $(CFLAGS) -I./perfctr -c hpca_null_perfctr.c


####

hpca_read_perfctr:	hpca_read_perfctr.o ../perfctr/libperfctr.a
	$(CC) $(LFLAGS) -o hpca_read_perfctr hpca_read_perfctr.o ../perfctr/libperfctr.a

hpca_read_perfctr.o:	hpca_read_perfctr.c ../perfctr/perfctr.h ../perfctr/libperfctr.h
	$(CC) $(CFLAGS) -I./perfctr -c hpca_read_perfctr.c

####

hpca_null_perfctr_slow:	hpca_null_perfctr_slow.o ../perfctr/libperfctr.a
	$(CC) $(LFLAGS) -o hpca_null_perfctr_slow hpca_null_perfctr_slow.o ../perfctr/libperfctr.a

hpca_null_perfctr_slow.o:	hpca_null_perfctr.c ../perfctr/perfctr.h ../perfctr/libperfctr.h
	$(CC) $(CFLAGS) -DUSE_SLOW -c hpca_null_perfctr.c -o hpca_null_perfctr_slow.o

#####

clean:
	rm -f *.o core *~ \
	hpca_null_pe \
	hpca_null_perfctr \
	hpca_null_perfctr_slow \
	hpca_null_perfmon2 \
	hpca_read_perfmon2 \
	hpca_read_pe \
	hpca_read_perfctr \
	make_pe_null_results \
	make_pe_read_results \
	make_perfmon2_null_results \
	make_perfmon2_read_results \
	make_perfctr_null_results \
	make_perfctr_null_slow_results \
	make_perfctr_read_results \
	make_raw_null_results 

install:	all
