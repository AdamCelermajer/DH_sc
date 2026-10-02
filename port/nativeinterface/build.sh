#!/usr/bin/env bash
set -euo pipefail
source_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
mode=${1:-host}
if [[ "$mode" == host ]]; then
  output_dir=${2:-"$source_dir/build/host"}
  mkdir -p "$output_dir/classes"
  cc=${CC:-gcc}
  flags=(-std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Werror -fstack-protector-strong)
  "$cc" "${flags[@]}" -fPIC -shared "$source_dir/nativeinterface.c" -o "$output_dir/libnativeinterface-core.so"
  "$cc" "${flags[@]}" -I "$source_dir" "$source_dir/nativeinterface.c" "$source_dir/tests/semantic_test.c" -o "$output_dir/nativeinterface-test"
  "$output_dir/nativeinterface-test"
  if [[ -n "${DH2_JDK_DIR:-}" ]]; then
    jdk_dir=$DH2_JDK_DIR
  else
    javac_path=$(readlink -f "$(command -v javac)")
    jdk_dir=$(dirname "$(dirname "$javac_path")")
  fi
  "$cc" "${flags[@]}" -fPIC -shared -I "$jdk_dir/include" -I "$jdk_dir/include/linux" \
      "-Wl,--version-script=$source_dir/exports.map" -Wl,--no-undefined \
      "$source_dir/nativeinterface.c" "$source_dir/nativeinterface_jni.c" -o "$output_dir/libnativeinterface.so"
  "$jdk_dir/bin/javac" -d "$output_dir/classes" "$source_dir/tests/NativeInterfaceSmokeTest.java"
  "$jdk_dir/bin/java" -cp "$output_dir/classes" com.samsung.zirconia.NativeInterfaceSmokeTest "$output_dir/libnativeinterface.so"
elif [[ "$mode" == android ]]; then
  ndk_dir=${2:?Usage: build.sh android /path/to/android-ndk [output_directory]}
  output_dir=${3:-"$source_dir/build/arm64-v8a"}
  mkdir -p "$output_dir"
  "$ndk_dir/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android26-clang" \
      -std=c11 -Wall -Wextra -Werror -fPIC -shared -fstack-protector-strong \
      "-Wl,--version-script=$source_dir/exports.map" -Wl,--no-undefined \
      -Wl,-z,max-page-size=16384 -Wl,-soname,libnativeinterface.so \
      "$source_dir/nativeinterface.c" "$source_dir/nativeinterface_jni.c" \
      -o "$output_dir/libnativeinterface.so"
  printf 'Built %s\n' "$output_dir/libnativeinterface.so"
else
  printf 'Usage: build.sh host [output_directory] | android /path/to/ndk [output_directory]\n' >&2
  exit 2
fi
