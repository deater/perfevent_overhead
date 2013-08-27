#!/bin/sh

DIR=./graphs/hpca2014

mkdir -p $DIR

for MACHINE in core2; do
for TYPE in start stop read total; do
./make_graphs_boxplot $MACHINE 0 $TYPE > "$DIR"/"$MACHINE"_boxplot_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_boxplot_"$TYPE".jgr > "$DIR"/"$MACHINE"_boxplot_"$TYPE".eps
convert "$DIR"/"$MACHINE"_boxplot_"$TYPE".eps "$DIR"/"$MACHINE"_boxplot_"$TYPE".png

done

done


for MACHINE in core2; do
for TYPE in start stop read total; do
./make_graphs_boxplot_rdpmc $MACHINE 0 $TYPE > "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".jgr > "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".eps
convert "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".eps "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".png

done

done
