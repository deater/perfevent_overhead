#!/bin/sh

mkdir -p ./graphs/oflo_papi

./make_graphs_oflo core2 > ./graphs/oflo_papi/core2_oflo.jgr
jgraph < ./graphs/oflo_papi/core2_oflo.jgr > ./graphs/oflo_papi/core2_oflo.eps
convert ./graphs/oflo_papi/core2_oflo.eps ./graphs/oflo_papi/core2_oflo.png

./make_graphs_oflo nehalem > ./graphs/oflo_papi/nehalem_oflo.jgr
jgraph < ./graphs/oflo_papi/nehalem_oflo.jgr > ./graphs/oflo_papi/nehalem_oflo.eps
convert ./graphs/oflo_papi/nehalem_oflo.eps ./graphs/oflo_papi/nehalem_oflo.png

./make_graphs_oflo atom > ./graphs/oflo_papi/atom_oflo.jgr
jgraph < ./graphs/oflo_papi/atom_oflo.jgr > ./graphs/oflo_papi/atom_oflo.eps
convert ./graphs/oflo_papi/atom_oflo.eps ./graphs/oflo_papi/atom_oflo.png

./make_graphs_oflo amd10h > ./graphs/oflo_papi/amd10h_oflo.jgr
jgraph < ./graphs/oflo_papi/amd10h_oflo.jgr > ./graphs/oflo_papi/amd10h_oflo.eps
convert ./graphs/oflo_papi/amd10h_oflo.eps ./graphs/oflo_papi/amd10h_oflo.png

./make_graphs_oflo power6 > ./graphs/oflo_papi/power6_oflo.jgr
jgraph < ./graphs/oflo_papi/power6_oflo.jgr > ./graphs/oflo_papi/power6_oflo.eps
convert ./graphs/oflo_papi/power6_oflo.eps ./graphs/oflo_papi/power6_oflo.png

./make_graphs_oflo cortexA9 > ./graphs/oflo_papi/cortexA9_oflo.jgr
jgraph < ./graphs/oflo_papi/cortexA9_oflo.jgr > ./graphs/oflo_papi/cortexA9_oflo.eps
convert ./graphs/oflo_papi/cortexA9_oflo.eps ./graphs/oflo_papi/cortexA9_oflo.png
