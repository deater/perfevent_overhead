#!/bin/sh

KERNEL=`uname -r`.perfmon
PERFCMD="/home/vince/research/perfmon/pfmon/pfmon/pfmon -e INSTRUCTIONS_RETIRED,UNHALTED_CORE_CYCLES "

echo $PERFCMD

mkdir -p $KERNEL

mkdir -p $KERNEL/0
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.0
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.1
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.2
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.3
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.4
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.5
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.6
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.7
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.8
$PERFCMD ./lat_ctx -s 0 2 4 8 16 32 64 96 2> $KERNEL/0/results.9

mkdir -p $KERNEL/8
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.0
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.1
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.2
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.3
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.4
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.5
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.6
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.7
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.8
$PERFCMD ./lat_ctx -s 8 2 4 8 16 32 64 96 2> $KERNEL/8/results.9

mkdir -p $KERNEL/16
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.0
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.1
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.2
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.3
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.4
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.5
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.6
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.7
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.8
$PERFCMD ./lat_ctx -s 16 2 4 8 16 32 64 96 2> $KERNEL/16/results.9

mkdir -p $KERNEL/32
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.0
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.1
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.2
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.3
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.4
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.5
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.6
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.7
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.8
$PERFCMD ./lat_ctx -s 32 2 4 8 16 32 64 96 2> $KERNEL/32/results.9

