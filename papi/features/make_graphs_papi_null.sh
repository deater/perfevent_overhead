#!/bin/sh

./make_graphs_papi_null core2 > ./graphs/null_papi/core2_null.jgr
jgraph < ./graphs/null_papi/core2_null.jgr > ./graphs/null_papi/core2_null.eps
convert ./graphs/null_papi/core2_null.eps ./graphs/null_papi/core2_null.png

./make_graphs_papi_null nehalem > ./graphs/null_papi/nehalem_null.jgr
jgraph < ./graphs/null_papi/nehalem_null.jgr > ./graphs/null_papi/nehalem_null.eps
convert ./graphs/null_papi/nehalem_null.eps ./graphs/null_papi/nehalem_null.png

./make_graphs_papi_null atom > ./graphs/null_papi/atom_null.jgr
jgraph < ./graphs/null_papi/atom_null.jgr > ./graphs/null_papi/atom_null.eps
convert ./graphs/null_papi/atom_null.eps ./graphs/null_papi/atom_null.png

./make_graphs_papi_null amd10h > ./graphs/null_papi/amd10h_null.jgr
jgraph < ./graphs/null_papi/amd10h_null.jgr > ./graphs/null_papi/amd10h_null.eps
convert ./graphs/null_papi/amd10h_null.eps ./graphs/null_papi/amd10h_null.png

./make_graphs_papi_null power6 > ./graphs/null_papi/power6_null.jgr
jgraph < ./graphs/null_papi/power6_null.jgr > ./graphs/null_papi/power6_null.eps
convert ./graphs/null_papi/power6_null.eps ./graphs/null_papi/power6_null.png

./make_graphs_papi_null cortexA9 > ./graphs/null_papi/cortexA9_null.jgr
jgraph < ./graphs/null_papi/cortexA9_null.jgr > ./graphs/null_papi/cortexA9_null.eps
convert ./graphs/null_papi/cortexA9_null.eps ./graphs/null_papi/cortexA9_null.png
