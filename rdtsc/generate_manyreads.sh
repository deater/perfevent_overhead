#!/bin/sh
./generate_results_many perf_event_rdpmc-touch_static
./generate_results_many perf_event
./generate_results_many perf_event-static

