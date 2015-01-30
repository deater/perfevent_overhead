#!/bin/sh
./generate_manyreads perf_event_rdpmc-touch_static
./generate_manyreads perf_event
./generate_manyreads perf_event-static

