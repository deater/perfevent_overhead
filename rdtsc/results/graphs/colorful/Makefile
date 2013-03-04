all:	kernel_key.png

kernel_key.png:	kernel_key.eps
	convert kernel_key.eps kernel_key.png

kernel_key.eps:	kernel_key.jgr
	jgraph < kernel_key.jgr > kernel_key.eps

install:	
	scp *.png vince@blah:

clean:	
	rm -f *~ *.eps *.png	\
		atom*jgr nehalem*jgr core2*jgr amd*jgr

