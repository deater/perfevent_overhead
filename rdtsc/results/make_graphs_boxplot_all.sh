#!/bin/sh

DIR=./graphs/boxplots

mkdir -p $DIR

for MACHINE in amd0fh nehalem core2 atom; do
for TYPE in start stop read total; do
./make_graphs_boxplot $MACHINE $TYPE > "$DIR"/"$MACHINE"_boxplot_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_boxplot_"$TYPE".jgr > "$DIR"/"$MACHINE"_boxplot_"$TYPE".eps
convert "$DIR"/"$MACHINE"_boxplot_"$TYPE".eps "$DIR"/"$MACHINE"_boxplot_"$TYPE".png

done

done
