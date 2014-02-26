CC	= gcc
CC_R	= $(CC) -pthread
CFLAGS	= -g -O2 -Wall -I../include

PAPI_INCLUDE = -I/home/vince/research/papi/papi.git/src/

PAPI_LIB_DYNAMIC = -lpapi
PAPI_LIB_STATIC = ~/research/papi/papi.git/src/libpapi.a

DYNAMIC_LFLAGS = 
STATIC_LFLAGS = -static

all:	papi_tot_cyc.dynamic \
	papi_tot_cyc.static \
	papi_tot_cyc_ins.dynamic \
	papi_tot_cyc_ins.static \
	papi_tot_cyc_ins_cache.dynamic \
	papi_tot_cyc_ins_cache.static 


###

instructions_testcode.o:	instructions_testcode.c
	$(CC) $(CFLAGS) -c instructions_testcode.c

###

papi_tot_cyc.dynamic:	papi_tot_cyc.dynamic.o instructions_testcode.o 
	$(CC) $(DYNAMIC_LFLAGS) -o papi_tot_cyc.dynamic papi_tot_cyc.dynamic.o \
		instructions_testcode.o $(PAPI_LIB_DYNAMIC)

papi_tot_cyc.dynamic.o:	papi_tot_cyc.c
	$(CC) $(CFLAGS) $(PAPI_INCLUDE) -o papi_tot_cyc.dynamic.o -c papi_tot_cyc.c

###

papi_tot_cyc.static:	papi_tot_cyc.static.o instructions_testcode.o 
	$(CC) $(STATIC_LFLAGS) -o papi_tot_cyc.static papi_tot_cyc.static.o \
		instructions_testcode.o $(PAPI_LIB_STATIC)

papi_tot_cyc.static.o:	papi_tot_cyc.c
	$(CC) $(CFLAGS) $(PAPI_INCLUDE) -o papi_tot_cyc.static.o -c papi_tot_cyc.c

###

papi_tot_cyc_ins.dynamic:	papi_tot_cyc_ins.dynamic.o instructions_testcode.o 
	$(CC) $(DYNAMIC_LFLAGS) -o papi_tot_cyc_ins.dynamic papi_tot_cyc_ins.dynamic.o \
		instructions_testcode.o $(PAPI_LIB_DYNAMIC)

papi_tot_cyc_ins.dynamic.o:	papi_tot_cyc_ins.c
	$(CC) $(CFLAGS) $(PAPI_INCLUDE) -o papi_tot_cyc_ins.dynamic.o -c papi_tot_cyc_ins.c

###

papi_tot_cyc_ins.static:	papi_tot_cyc_ins.static.o instructions_testcode.o 
	$(CC) $(STATIC_LFLAGS) -o papi_tot_cyc_ins.static papi_tot_cyc_ins.static.o \
		instructions_testcode.o $(PAPI_LIB_STATIC)

papi_tot_cyc_ins.static.o:	papi_tot_cyc_ins.c
	$(CC) $(CFLAGS) $(PAPI_INCLUDE) -o papi_tot_cyc_ins.static.o -c papi_tot_cyc_ins.c

###

papi_tot_cyc_ins_cache.dynamic:	papi_tot_cyc_ins_cache.dynamic.o instructions_testcode.o 
	$(CC) $(DYNAMIC_LFLAGS) -o papi_tot_cyc_ins_cache.dynamic papi_tot_cyc_ins_cache.dynamic.o \
		instructions_testcode.o $(PAPI_LIB_DYNAMIC)

papi_tot_cyc_ins_cache.dynamic.o:	papi_tot_cyc_ins_cache.c
	$(CC) $(CFLAGS) $(PAPI_INCLUDE) -o papi_tot_cyc_ins_cache.dynamic.o -c papi_tot_cyc_ins_cache.c

###

papi_tot_cyc_ins_cache.static:	papi_tot_cyc_ins_cache.static.o instructions_testcode.o 
	$(CC) $(STATIC_LFLAGS) -o papi_tot_cyc_ins_cache.static papi_tot_cyc_ins_cache.static.o \
		instructions_testcode.o $(PAPI_LIB_STATIC)

papi_tot_cyc_ins_cache.static.o:	papi_tot_cyc_ins_cache.c
	$(CC) $(CFLAGS) $(PAPI_INCLUDE) -o papi_tot_cyc_ins_cache.static.o -c papi_tot_cyc_ins_cache.c


###

clean:
	rm -f *.o core *~ \
	papi_tot_cyc papi_tot_ins papi_tot_cyc_ins \
	papi_tot_cyc.dynamic papi_tot_cyc.static \
	papi_tot_cyc_ins.dynamic papi_tot_cyc_ins.static \
	papi_tot_cyc_ins_cache.dynamic papi_tot_cyc_ins_cache.static 
