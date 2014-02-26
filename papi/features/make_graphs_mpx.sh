#!/bin/sh

mkdir -p ./graphs/mpx_papi

./make_graphs_mpx core2 > ./graphs/mpx_papi/core2_mpx.jgr
jgraph < ./graphs/mpx_papi/core2_mpx.jgr > ./graphs/mpx_papi/core2_mpx.eps
convert ./graphs/mpx_papi/core2_mpx.eps ./graphs/mpx_papi/core2_mpx.png

./make_graphs_mpx nehalem > ./graphs/mpx_papi/nehalem_mpx.jgr
jgraph < ./graphs/mpx_papi/nehalem_mpx.jgr > ./graphs/mpx_papi/nehalem_mpx.eps
convert ./graphs/mpx_papi/nehalem_mpx.eps ./graphs/mpx_papi/nehalem_mpx.png

./make_graphs_mpx atom > ./graphs/mpx_papi/atom_mpx.jgr
jgraph < ./graphs/mpx_papi/atom_mpx.jgr > ./graphs/mpx_papi/atom_mpx.eps
convert ./graphs/mpx_papi/atom_mpx.eps ./graphs/mpx_papi/atom_mpx.png

./make_graphs_mpx amd10h > ./graphs/mpx_papi/amd10h_mpx.jgr
jgraph < ./graphs/mpx_papi/amd10h_mpx.jgr > ./graphs/mpx_papi/amd10h_mpx.eps
convert ./graphs/mpx_papi/amd10h_mpx.eps ./graphs/mpx_papi/amd10h_mpx.png

./make_graphs_mpx power6 > ./graphs/mpx_papi/power6_mpx.jgr
jgraph < ./graphs/mpx_papi/power6_mpx.jgr > ./graphs/mpx_papi/power6_mpx.eps
convert ./graphs/mpx_papi/power6_mpx.eps ./graphs/mpx_papi/power6_mpx.png

./make_graphs_mpx cortexA9 > ./graphs/mpx_papi/cortexA9_mpx.jgr
jgraph < ./graphs/mpx_papi/cortexA9_mpx.jgr > ./graphs/mpx_papi/cortexA9_mpx.eps
convert ./graphs/mpx_papi/cortexA9_mpx.eps ./graphs/mpx_papi/cortexA9_mpx.png
