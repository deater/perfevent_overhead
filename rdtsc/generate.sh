#!/bin/sh
./generate_results perf_event_rdpmc-touch_static
./generate_results perf_event_rdpmc
./generate_results perf_event_rdpmc-map_populate
./generate_results perf_event_rdpmc-touch
./generate_results perf_event-syscall_static
./generate_results perf_event-syscall
./generate_results perf_event-static
./generate_results perf_event
