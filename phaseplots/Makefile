CC	= gcc
CFLAGS	= -g -O2 -Wall
LFLAGS  = -lm

all:	\
	make_branchplot make_phaseplot

###

make_branchplot:	make_branchplot.o
	$(CC) -g $(LFLAGS) -o make_branchplot make_branchplot.o

make_branchplot.o:	make_branchplot.c
	$(CC) $(CFLAGS) -c make_branchplot.c

###

make_phaseplot:	make_phaseplot.o
	$(CC) -g $(LFLAGS) -o make_phaseplot make_phaseplot.o

make_phaseplot.o:       make_phaseplot.c
	$(CC) $(CFLAGS) -c make_phaseplot.c

###

clean:
	rm -f *.o core *~ \
	make_branchplot make_phaseplot
