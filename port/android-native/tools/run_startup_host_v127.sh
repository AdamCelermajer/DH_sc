#!/usr/bin/env bash
set -euo pipefail
if [[ $# != 3 ]]; then echo 'usage: run_startup_host_v127.sh repo cache.zip output-directory' >&2; exit 2; fi
task_repo=$1
task_cache=$2
task_output=$3
mkdir -p "$task_output"
#Bound each process to 2GiB of virtual address space; compile serially.
ulimit -v 2097152
cmake -S "$task_repo/port/android-native/tests/startup-host-v127" -B "$task_output/build" -G Ninja > "$task_output/build.log" 2>&1
timeout 120s cmake --build "$task_output/build" --parallel 1 >> "$task_output/build.log" 2>&1
timeout 180s /usr/bin/time -v -o "$task_output/resources.txt" "$task_output/build/startup_data_host_v127" "$task_cache" | tee "$task_output/results.jsonl"
