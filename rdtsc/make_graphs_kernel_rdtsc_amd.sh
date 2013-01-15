#!/bin/sh

mkdir -p graphs/kernel_rdtsc

# AMD 0fh

./make_graphs_kernel_rdtsc amd0fh total > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.jgr
jgraph < ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.jgr > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.eps
convert ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.eps ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_total.png

./make_graphs_kernel_rdtsc amd0fh start > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.jgr
jgraph < ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.jgr > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.eps
convert ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.eps ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_start.png

./make_graphs_kernel_rdtsc amd0fh stop > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.jgr
jgraph < ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.jgr > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.eps
convert ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.eps ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_stop.png
 
./make_graphs_kernel_rdtsc amd0fh read > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.jgr
jgraph < ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.jgr > ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.eps
convert ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.eps ./graphs/kernel_rdtsc/amd0fh_raw_null_kernel_rdtsc_read.png




