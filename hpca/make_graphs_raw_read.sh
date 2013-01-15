#!/bin/sh

mkdir -p graphs/read_raw

./make_graphs_raw_read core2 > ./graphs/read_raw/core2_raw_read.jgr
jgraph < ./graphs/read_raw/core2_raw_read.jgr > ./graphs/read_raw/core2_raw_read.eps
convert ./graphs/read_raw/core2_raw_read.eps ./graphs/read_raw/core2_raw_read.png

./make_graphs_raw_read nehalem > ./graphs/read_raw/nehalem_raw_read.jgr
jgraph < ./graphs/read_raw/nehalem_raw_read.jgr > ./graphs/read_raw/nehalem_raw_read.eps
convert ./graphs/read_raw/nehalem_raw_read.eps ./graphs/read_raw/nehalem_raw_read.png

./make_graphs_raw_read atom > ./graphs/read_raw/atom_raw_read.jgr
jgraph < ./graphs/read_raw/atom_raw_read.jgr > ./graphs/read_raw/atom_raw_read.eps
convert ./graphs/read_raw/atom_raw_read.eps ./graphs/read_raw/atom_raw_read.png

./make_graphs_raw_read amd10h > ./graphs/read_raw/amd10h_raw_read.jgr
jgraph < ./graphs/read_raw/amd10h_raw_read.jgr > ./graphs/read_raw/amd10h_raw_read.eps
convert ./graphs/read_raw/amd10h_raw_read.eps ./graphs/read_raw/amd10h_raw_read.png

./make_graphs_raw_read amd0fh > ./graphs/read_raw/amd0fh_raw_read.jgr
jgraph < ./graphs/read_raw/amd0fh_raw_read.jgr > ./graphs/read_raw/amd0fh_raw_read.eps
convert ./graphs/read_raw/amd0fh_raw_read.eps ./graphs/read_raw/amd0fh_raw_read.png

./make_graphs_raw_read power6 > ./graphs/read_raw/power6_raw_read.jgr
jgraph < ./graphs/read_raw/power6_raw_read.jgr > ./graphs/read_raw/power6_raw_read.eps
convert ./graphs/read_raw/power6_raw_read.eps ./graphs/read_raw/power6_raw_read.png

./make_graphs_raw_read cortexA9 > ./graphs/read_raw/cortexA9_raw_read.jgr
jgraph < ./graphs/read_raw/cortexA9_raw_read.jgr > ./graphs/read_raw/cortexA9_raw_read.eps
convert ./graphs/read_raw/cortexA9_raw_read.eps ./graphs/read_raw/cortexA9_raw_read.png
