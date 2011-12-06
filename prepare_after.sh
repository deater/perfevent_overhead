#!/bin/sh

# test with cpufreq-info
cpufreq-set -g ondemand -c 0 -r
cpufreq-set -g ondemand -c 1 -r
cpufreq-set -g ondemand -c 2 -r
cpufreq-set -g ondemand -c 3 -r
cpufreq-set -g ondemand -c 4 -r
cpufreq-set -g ondemand -c 5 -r
cpufreq-set -g ondemand -c 6 -r
cpufreq-set -g ondemand -c 7 -r
cpufreq-set -g ondemand -c 8 -r


echo "1" > /proc/sys/kernel/nmi_watchdog

