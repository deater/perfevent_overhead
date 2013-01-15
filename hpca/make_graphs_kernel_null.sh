#!/bin/sh

mkdir -p graphs/kernel_null

./make_graphs_kernel_null core2 > ./graphs/kernel_null/core2_raw_null_kernel.jgr
jgraph < ./graphs/kernel_null/core2_raw_null_kernel.jgr > ./graphs/kernel_null/core2_raw_null_kernel.eps
convert ./graphs/kernel_null/core2_raw_null_kernel.eps ./graphs/kernel_null/core2_raw_null_kernel.png

./make_graphs_kernel_null nehalem > ./graphs/kernel_null/nehalem_raw_null_kernel.jgr
jgraph < ./graphs/kernel_null/nehalem_raw_null_kernel.jgr > ./graphs/kernel_null/nehalem_raw_null_kernel.eps
convert ./graphs/kernel_null/nehalem_raw_null_kernel.eps ./graphs/kernel_null/nehalem_raw_null_kernel.png

./make_graphs_kernel_null atom > ./graphs/kernel_null/atom_raw_null_kernel.jgr
jgraph < ./graphs/kernel_null/atom_raw_null_kernel.jgr > ./graphs/kernel_null/atom_raw_null_kernel.eps
convert ./graphs/kernel_null/atom_raw_null_kernel.eps ./graphs/kernel_null/atom_raw_null_kernel.png

./make_graphs_kernel_null amd10h > ./graphs/kernel_null/amd10h_raw_null_kernel.jgr
jgraph < ./graphs/kernel_null/amd10h_raw_null_kernel.jgr > ./graphs/kernel_null/amd10h_raw_null_kernel.eps
convert ./graphs/kernel_null/amd10h_raw_null_kernel.eps ./graphs/kernel_null/amd10h_raw_null_kernel.png

./make_graphs_kernel_null amd0fh > ./graphs/kernel_null/amd0fh_raw_null_kernel.jgr
jgraph < ./graphs/kernel_null/amd0fh_raw_null_kernel.jgr > ./graphs/kernel_null/amd0fh_raw_null_kernel.eps
convert ./graphs/kernel_null/amd0fh_raw_null_kernel.eps ./graphs/kernel_null/amd0fh_raw_null_kernel.png

./make_graphs_kernel_null power6 > ./graphs/kernel_null/power6_raw_null_kernel.jgr
jgraph < ./graphs/kernel_null/power6_raw_null_kernel.jgr > ./graphs/kernel_null/power6_raw_null_kernel.eps
convert ./graphs/kernel_null/power6_raw_null_kernel.eps ./graphs/kernel_null/power6_raw_null_kernel.png

./make_graphs_kernel_null cortexA9 > ./graphs/kernel_null/cortexA9_raw_null_kernel.jgr
jgraph < ./graphs/kernel_null/cortexA9_raw_null_kernel.jgr > ./graphs/kernel_null/cortexA9_raw_null_kernel.eps
convert ./graphs/kernel_null/cortexA9_raw_null_kernel.eps ./graphs/kernel_null/cortexA9_raw_null_kernel.png
