#!/bin/sh

mkdir -p graphs/kernel_rdtsc

# Core2

./make_graphs_kernel_rdtsc core2 start > ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_start.jgr
jgraph < ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_start.jgr > ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_start.eps
convert ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_start.eps ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_start.png

./make_graphs_kernel_rdtsc core2 stop > ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_stop.jgr
jgraph < ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_stop.jgr > ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_stop.eps
convert ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_stop.eps ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_stop.png
 
./make_graphs_kernel_rdtsc core2 read > ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_read.jgr
jgraph < ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_read.jgr > ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_read.eps
convert ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_read.eps ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_read.png

./make_graphs_kernel_rdtsc core2 total > ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_total.jgr
jgraph < ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_total.jgr > ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_total.eps
convert ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_total.eps ./graphs/kernel_rdtsc/core2_raw_null_kernel_rdtsc_total.png

# Nehalem

./make_graphs_kernel_rdtsc nehalem start > ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_start.jgr
jgraph < ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_start.jgr > ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_start.eps
convert ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_start.eps ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_start.png

./make_graphs_kernel_rdtsc nehalem stop > ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_stop.jgr
jgraph < ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_stop.jgr > ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_stop.eps
convert ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_stop.eps ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_stop.png
 
./make_graphs_kernel_rdtsc nehalem read > ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_read.jgr
jgraph < ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_read.jgr > ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_read.eps
convert ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_read.eps ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_read.png

./make_graphs_kernel_rdtsc nehalem total > ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_total.jgr
jgraph < ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_total.jgr > ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_total.eps
convert ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_total.eps ./graphs/kernel_rdtsc/nehalem_raw_null_kernel_rdtsc_total.png

# AMD 0fh

./make_graphs_kernel_rdtsc amd0fh start > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.jgr
jgraph < ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.jgr > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.eps
convert ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.eps ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.png

./make_graphs_kernel_rdtsc amd0fh stop > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.jgr
jgraph < ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.jgr > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.eps
convert ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.eps ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.png
 
./make_graphs_kernel_rdtsc amd0fh read > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.jgr
jgraph < ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.jgr > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.eps
convert ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.eps ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.png

./make_graphs_kernel_rdtsc amd0fh total > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.jgr
jgraph < ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.jgr > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.eps
convert ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.eps ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.png


