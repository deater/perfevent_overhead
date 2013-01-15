#!/bin/sh

mkdir -p graphs/kernel_rdtsc

# atom

./make_graphs_kernel_rdtsc atom start > ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_start.jgr
jgraph < ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_start.jgr > ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_start.eps
convert ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_start.eps ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_start.png

./make_graphs_kernel_rdtsc atom stop > ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_stop.jgr
jgraph < ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_stop.jgr > ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_stop.eps
convert ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_stop.eps ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_stop.png
 
./make_graphs_kernel_rdtsc atom read > ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_read.jgr
jgraph < ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_read.jgr > ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_read.eps
convert ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_read.eps ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_read.png

./make_graphs_kernel_rdtsc atom total > ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_total.jgr
jgraph < ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_total.jgr > ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_total.eps
convert ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_total.eps ./graphs/kernel_rdtsc/atom_raw_null_kernel_rdtsc_all_total.png
