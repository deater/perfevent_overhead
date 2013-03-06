#!/bin/sh

DIR=./graphs/varying

mkdir -p $DIR

for MACHINE in amd0fh nehalem core2 atom; do
for TYPE in start stop read total; do
./make_graphs_varying $MACHINE $TYPE > "$DIR"/"$MACHINE"_varying_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_varying_"$TYPE".jgr > "$DIR"/"$MACHINE"_varying_"$TYPE".eps
convert "$DIR"/"$MACHINE"_varying_"$TYPE".eps "$DIR"/"$MACHINE"_varying_"$TYPE".png

done

done
