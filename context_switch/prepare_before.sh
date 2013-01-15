#!/bin/sh

# test with cpufreq-info
cpufreq-set -g performance -c 0 -r
cpufreq-set -g performance -c 1 -r
cpufreq-set -g performance -c 2 -r
cpufreq-set -g performance -c 3 -r
cpufreq-set -g performance -c 4 -r
cpufreq-set -g performance -c 5 -r
cpufreq-set -g performance -c 6 -r
cpufreq-set -g performance -c 7 -r
cpufreq-set -g performance -c 8 -r

echo "0" > /proc/sys/kernel/nmi_watchdog

