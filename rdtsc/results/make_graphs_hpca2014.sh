#!/bin/sh

DIR=./graphs/hpca2014

mkdir -p $DIR

# Boxplot

for MACHINE in core2; do
for TYPE in start stop read total; do
./make_graphs_boxplot $MACHINE 0 $TYPE > "$DIR"/"$MACHINE"_boxplot_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_boxplot_"$TYPE".jgr > "$DIR"/"$MACHINE"_boxplot_"$TYPE".eps
convert "$DIR"/"$MACHINE"_boxplot_"$TYPE".eps "$DIR"/"$MACHINE"_boxplot_"$TYPE".png
done
done

# Boxplot RDPMC

for MACHINE in core2; do
for TYPE in start stop read total; do
./make_graphs_boxplot_rdpmc $MACHINE 0 $TYPE > "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".jgr > "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".eps
convert "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".eps "$DIR"/"$MACHINE"_boxplot_rdpmc_"$TYPE".png
done
done


# Boxplot gcc

for MACHINE in core2; do
for TYPE in start stop read total; do
./make_graphs_boxplot_gcc $MACHINE 0 $TYPE > "$DIR"/"$MACHINE"_boxplot_gcc_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_boxplot_gcc_"$TYPE".jgr > "$DIR"/"$MACHINE"_boxplot_gcc_"$TYPE".eps
convert "$DIR"/"$MACHINE"_boxplot_gcc_"$TYPE".eps "$DIR"/"$MACHINE"_boxplot_gcc_"$TYPE".png
done
done


# Boxplot final

for MACHINE in atom-cedarview bobcat core2 ivybridge; do
for TYPE in start stop read total; do
./make_graphs_boxplot_final $MACHINE 0 $TYPE > "$DIR"/"$MACHINE"_boxplot_final_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_boxplot_final_"$TYPE".jgr > "$DIR"/"$MACHINE"_boxplot_final_"$TYPE".eps
convert "$DIR"/"$MACHINE"_boxplot_final_"$TYPE".eps "$DIR"/"$MACHINE"_boxplot_final_"$TYPE".png
done
done

# Varying Plot
for MACHINE in bobcat core2 ivybridge atom-cedarview; do
for TYPE in start stop read total; do
./make_graphs_varying $MACHINE 0 $TYPE > "$DIR"/"$MACHINE"_varying_"$TYPE".jgr
jgraph < "$DIR"/"$MACHINE"_varying_"$TYPE".jgr > "$DIR"/"$MACHINE"_varying_"$TYPE".eps
convert "$DIR"/"$MACHINE"_varying_"$TYPE".eps "$DIR"/"$MACHINE"_varying_"$TYPE".png
done
done

