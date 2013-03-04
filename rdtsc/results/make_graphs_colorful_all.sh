#!/bin/sh

DIR=./graphs/colorful

mkdir -p $DIR

for MACHINE in amd0fh nehalem core2 atom; do
for TYPE in start stop read total; do
./make_graphs_colorful $MACHINE $TYPE > "$DIR"/"$MACHINE"_colorful_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_colorful_"$TYPE".jgr > "$DIR"/"$MACHINE"_colorful_"$TYPE".eps
convert "$DIR"/"$MACHINE"_colorful_"$TYPE".eps "$DIR"/"$MACHINE"_colorful_"$TYPE".png

done

done