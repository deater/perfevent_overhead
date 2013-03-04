#!/bin/sh

DIR=./graphs/errorbars

mkdir -p $DIR

for MACHINE in amd0fh nehalem core2 atom; do
for TYPE in start stop read total; do
./make_graphs_errorbars $MACHINE $TYPE > "$DIR"/"$MACHINE"_errorbars_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_errorbars_"$TYPE".jgr > "$DIR"/"$MACHINE"_errorbars_"$TYPE".eps
convert "$DIR"/"$MACHINE"_errorbars_"$TYPE".eps "$DIR"/"$MACHINE"_errorbars_"$TYPE".png

done

done
