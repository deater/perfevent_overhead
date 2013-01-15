#!/bin/sh

mkdir -p graphs/null_raw

./make_graphs_raw_null core2 > ./graphs/null_raw/core2_raw.jgr
jgraph < ./graphs/null_raw/core2_raw.jgr > ./graphs/null_raw/core2_raw.eps
convert ./graphs/null_raw/core2_raw.eps ./graphs/null_raw/core2_raw.png

./make_graphs_raw_null nehalem > ./graphs/null_raw/nehalem_raw.jgr
jgraph < ./graphs/null_raw/nehalem_raw.jgr > ./graphs/null_raw/nehalem_raw.eps
convert ./graphs/null_raw/nehalem_raw.eps ./graphs/null_raw/nehalem_raw.png

./make_graphs_raw_null atom > ./graphs/null_raw/atom_raw.jgr
jgraph < ./graphs/null_raw/atom_raw.jgr > ./graphs/null_raw/atom_raw.eps
convert ./graphs/null_raw/atom_raw.eps ./graphs/null_raw/atom_raw.png

./make_graphs_raw_null amd10h > ./graphs/null_raw/amd10h_raw.jgr
jgraph < ./graphs/null_raw/amd10h_raw.jgr > ./graphs/null_raw/amd10h_raw.eps
convert ./graphs/null_raw/amd10h_raw.eps ./graphs/null_raw/amd10h_raw.png

./make_graphs_raw_null amd0fh > ./graphs/null_raw/amd0fh_raw.jgr
jgraph < ./graphs/null_raw/amd0fh_raw.jgr > ./graphs/null_raw/amd0fh_raw.eps
convert ./graphs/null_raw/amd0fh_raw.eps ./graphs/null_raw/amd0fh_raw.png

./make_graphs_raw_null power6 > ./graphs/null_raw/power6_raw.jgr
jgraph < ./graphs/null_raw/power6_raw.jgr > ./graphs/null_raw/power6_raw.eps
convert ./graphs/null_raw/power6_raw.eps ./graphs/null_raw/power6_raw.png

./make_graphs_raw_null cortexA9 > ./graphs/null_raw/cortexA9_raw.jgr
jgraph < ./graphs/null_raw/cortexA9_raw.jgr > ./graphs/null_raw/cortexA9_raw.eps
convert ./graphs/null_raw/cortexA9_raw.eps ./graphs/null_raw/cortexA9_raw.png
