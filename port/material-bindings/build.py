#!/usr/bin/env python3
"""Build the checked BRES image/material record views for a host compiler."""

import argparse
import os
from pathlib import Path
import subprocess


ROOT = Path(__file__).resolve().parent


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--cxx', default=os.environ.get('CXX', 'g++'))
    parser.add_argument('--ndk', type=Path, help='Android NDK r29 for an ARM64 build')
    parser.add_argument('--arm64-output', type=Path)
    args = parser.parse_args()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    command = [
        args.cxx, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror', '-fPIC',
        '-shared', str(ROOT / 'bindings.cpp'),
        str(ROOT / '../engine-resources/resources.cpp'),
        '-o', str(args.output),
    ]
    if os.name == 'nt':
        command.extend(['-static-libgcc', '-static-libstdc++'])
    subprocess.run(command, check=True)
    print(args.output.resolve())
    if args.ndk:
        if not args.arm64_output:
            parser.error('--arm64-output is required with --ndk')
        prebuilt = args.ndk / 'toolchains/llvm/prebuilt'
        host = 'windows-x86_64' if os.name == 'nt' else 'linux-x86_64'
        compiler = prebuilt / host / 'bin' / ('aarch64-linux-android26-clang++.cmd' if os.name == 'nt' else 'aarch64-linux-android26-clang++')
        args.arm64_output.parent.mkdir(parents=True, exist_ok=True)
        subprocess.run([
            str(compiler), '-std=c++17', '-O2', '-fPIC', '-fno-exceptions',
            '-fno-rtti', '-Wall', '-Wextra', '-Werror', '-shared', '-nostdlib++',
            str(ROOT / 'bindings.cpp'), str(ROOT / '../engine-resources/resources.cpp'),
            '-Wl,-Bsymbolic', '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
            '-o', str(args.arm64_output),
        ], check=True)
        print(args.arm64_output.resolve())


if __name__ == '__main__':
    main()
