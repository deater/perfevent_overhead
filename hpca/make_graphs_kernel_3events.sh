#!/bin/sh

mkdir -p graphs/kernel_null

./make_graphs_kernel_3events core2 > ./graphs/kernel_null/core2_raw_null_kernel_3events.jgr
jgraph < ./graphs/kernel_null/core2_raw_null_kernel_3events.jgr > ./graphs/kernel_null/core2_raw_null_kernel_3events.eps
convert ./graphs/kernel_null/core2_raw_null_kernel_3events.eps ./graphs/kernel_null/core2_raw_null_kernel_3events.png

./make_graphs_kernel_3events nehalem > ./graphs/kernel_null/nehalem_raw_null_kernel_3events.jgr
jgraph < ./graphs/kernel_null/nehalem_raw_null_kernel_3events.jgr > ./graphs/kernel_null/nehalem_raw_null_kernel_3events.eps
convert ./graphs/kernel_null/nehalem_raw_null_kernel_3events.eps ./graphs/kernel_null/nehalem_raw_null_kernel_3events.png

./make_graphs_kernel_3events atom > ./graphs/kernel_null/atom_raw_null_kernel_3events.jgr
jgraph < ./graphs/kernel_null/atom_raw_null_kernel_3events.jgr > ./graphs/kernel_null/atom_raw_null_kernel_3events.eps
convert ./graphs/kernel_null/atom_raw_null_kernel_3events.eps ./graphs/kernel_null/atom_raw_null_kernel_3events.png

./make_graphs_kernel_3events amd10h > ./graphs/kernel_null/amd10h_raw_null_kernel_3events.jgr
jgraph < ./graphs/kernel_null/amd10h_raw_null_kernel_3events.jgr > ./graphs/kernel_null/amd10h_raw_null_kernel_3events.eps
convert ./graphs/kernel_null/amd10h_raw_null_kernel_3events.eps ./graphs/kernel_null/amd10h_raw_null_kernel_3events.png

./make_graphs_kernel_3events amd0fh > ./graphs/kernel_null/amd0fh_raw_null_kernel_3events.jgr
jgraph < ./graphs/kernel_null/amd0fh_raw_null_kernel_3events.jgr > ./graphs/kernel_null/amd0fh_raw_null_kernel_3events.eps
convert ./graphs/kernel_null/amd0fh_raw_null_kernel_3events.eps ./graphs/kernel_null/amd0fh_raw_null_kernel_3events.png

./make_graphs_kernel_3events power6 > ./graphs/kernel_null/power6_raw_null_kernel_3events.jgr
jgraph < ./graphs/kernel_null/power6_raw_null_kernel_3events.jgr > ./graphs/kernel_null/power6_raw_null_kernel_3events.eps
convert ./graphs/kernel_null/power6_raw_null_kernel_3events.eps ./graphs/kernel_null/power6_raw_null_kernel_3events.png

./make_graphs_kernel_3events cortexA9 > ./graphs/kernel_null/cortexA9_raw_null_kernel_3events.jgr
jgraph < ./graphs/kernel_null/cortexA9_raw_null_kernel_3events.jgr > ./graphs/kernel_null/cortexA9_raw_null_kernel_3events.eps
convert ./graphs/kernel_null/cortexA9_raw_null_kernel_3events.eps ./graphs/kernel_null/cortexA9_raw_null_kernel_3events.png
