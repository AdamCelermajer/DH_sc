#!/usr/bin/env bash
set -euo pipefail
if [[ $# != 4 ]]; then echo 'usage: run_native_host_v128.sh repo cache.zip ndk-headers output-directory' >&2; exit 2; fi
task_repo=$1
task_cache=$2
task_ndk=$3
task_output=$4
mkdir -p "$task_output"
#One compiler at a time, finite address space and wall-clock bounds. The
#emulator must remain stopped while the integrator builds/runs this harness.
ulimit -v 3670016
cmake -S "$task_repo/port/android-native/tests/native-host-v128" -B "$task_output/build" -G Ninja \
 -DDH2_NDK_HEADERS="$task_ndk" > "$task_output/build.log" 2>&1
#Compile the JNI/renderer composition first, so portability errors are reported
#before the rest of the large first-time engine build.
timeout 300s cmake --build "$task_output/build" --target \
 native/CMakeFiles/dh2_native.dir/native_app.cpp.o \
 native/CMakeFiles/dh2_native.dir/model_renderer.cpp.o --parallel 1 >> "$task_output/build.log" 2>&1
timeout 1200s cmake --build "$task_output/build" --target dh2_native --parallel 1 >> "$task_output/build.log" 2>&1
mkdir -p "$task_output/java"
timeout 30s javac -J-Xmx128m -d "$task_output/java" "$task_repo/port/android-native/tests/native-host-v128/java/com/example/dh2/"*.java >> "$task_output/build.log" 2>&1
export EGL_PLATFORM=surfaceless LIBGL_ALWAYS_SOFTWARE=1 LP_NUM_THREADS=2
timeout 75s /usr/bin/time -v -o "$task_output/resources.txt" \
 java -Xms32m -Xmx256m -XX:CompressedClassSpaceSize=64m -XX:MaxMetaspaceSize=128m \
 -XX:ReservedCodeCacheSize=64m -XX:ActiveProcessorCount=2 \
 -Djava.library.path="$task_output/build/native" -cp "$task_output/java" com.example.dh2.HostRunner \
 "$task_repo/port/android-native/app/src/main/assets" "$task_cache" "$task_output" \
 > "$task_output/run.log" 2>&1
tail -n 10 "$task_output/run.log"
