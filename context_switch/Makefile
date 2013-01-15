CC = gcc
CFLAGS = -Wall -O2
LFLAGS =

all:	plot_results

plot_results:	plot_results.o
	$(CC) $(LFLAGS) -o plot_results plot_results.o

plot_results.o:	plot_results.c
	$(CC) $(CFLAGS) -c plot_results.c

clean:	
	rm -f *.o *~ plot_results

