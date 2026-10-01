#!/usr/bin/env python3
"""Build the reconstructed component, not a complete game library or APK."""
import argparse
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parent

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--ndk', type=Path, default=os.environ.get('ANDROID_NDK_HOME'))
    p.add_argument('--output', type=Path, default=ROOT / 'build')
    args = p.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    common = ['-std=c++17', '-O2', '-fPIC', '-shared', '-ffp-contract=off',
              '-fno-fast-math', '-fno-builtin', '-fno-exceptions', '-fno-rtti',
              '-Wall', '-Wextra', '-Werror']
    subprocess.run([os.environ.get('CXX', 'c++'), *common, str(ROOT / 'math.cpp'),
                    '-lm', '-o', str(args.output / 'libdh2_engine_math_host.so')],
                   check=True)
    subprocess.run([os.environ.get('CC', 'cc'), '-std=c11', '-O2', '-fPIC',
                    '-shared', '-ffp-contract=off', '-fno-fast-math', '-fno-builtin',
                    str(ROOT / 'tests/fp_oracle.c'), '-lm', '-o',
                    str(args.output / 'libfp_oracle.so')], check=True)
    if args.ndk:
        compiler = args.ndk / 'toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android26-clang++'
        subprocess.run([str(compiler), *common, str(ROOT / 'math.cpp'), '-lm',
                        '-nostdlib++',
                        '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
                        '-Wl,-Bsymbolic', '-o',
                        str(args.output / 'libdh2_engine_math_arm64.so')], check=True)
    print(json.dumps({'host_build': True, 'arm64_build': bool(args.ndk),
                      'output': str(args.output.resolve()),
                      'complete_engine': False}))

if __name__ == '__main__':
    main()
