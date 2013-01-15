CC	= gcc
CFLAGS	= -g -O2 -Wall
LFLAGS  = -lm

all:	\
	make_branchplot

###

make_branchplot:	make_branchplot.o
	$(CC) -g $(LFLAGS) -o make_branchplot make_branchplot.o

make_branchplot.o:	make_branchplot.c
	$(CC) $(CFLAGS) -c make_branchplot.c


###

clean:
	rm -f *.o core *~ \
	make_branchplot 

install:	all
