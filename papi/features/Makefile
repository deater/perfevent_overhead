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

#PAPI_INCLUDE = -I/usr/local/lib
#PAPI_LIB = /usr/local/lib/libpapi.a

PAPI_INCLUDE = -I/home/vince/research/papi/papi.git/src
PAPI_LIB = /home/vince/research/papi/papi.git/src/libpapi.a



all:	hpca_null_papi hpca_multiplex_papi hpca_overflow_papi \
	make_papi_null_results \
	make_mpx_results make_oflo_results \
        make_graphs_papi_null \
        make_graphs_mpx make_graphs_oflo 



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

matrix_multiply.o:	matrix_multiply.c
	$(CC) $(CFLAGS) -c matrix_multiply.c


###

make_papi_null_results:	make_papi_null_results.o
	$(CC) $(LFLAGS) -o make_papi_null_results make_papi_null_results.o

make_papi_null_results.o:	make_papi_null_results.c
	$(CC) $(CFLAGS) -c make_papi_null_results.c

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

make_graphs_papi_null:	make_graphs_papi_null.o
	$(CC) -g $(LFLAGS) -o make_graphs_papi_null make_graphs_papi_null.o

make_graphs_papi_null.o:	make_graphs_papi_null.c
	$(CC) $(CFLAGS) -c make_graphs_papi_null.c

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



######


clean:
	rm -f *.o core *~ \
	hpca_null_papi \
	hpca_multiplex_papi \
	hpca_overflow_papi \
	make_papi_null_results \
	make_mpx_results \
	make_oflo_results \
        make_graphs_papi_null \
        make_graphs_mpx \
        make_graphs_oflo 


