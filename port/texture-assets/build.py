#!/usr/bin/env python3
"""Build the portable parser and run its independent fixture/cache checks."""

import argparse
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cache-zip', type=Path, help='Complete owner-supplied cache ZIP')
    p.add_argument('--report', type=Path, default=ROOT / 'validation.json')
    p.add_argument('--cxx', default=os.environ.get('CXX', 'g++'))
    p.add_argument('--ndk', type=Path, default=os.environ.get('ANDROID_NDK_HOME'),
                   help='Also compile Android ARM64 with an Android NDK')
    a = p.parse_args()
    output = ROOT / 'build' / ('dh2_texture_assets.dll' if os.name == 'nt' else 'libdh2_texture_assets.so')
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [a.cxx, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC',
               str(ROOT / 'texture.cpp'), '-o', str(output)]
    subprocess.run(command, check=True)
    if a.ndk:
        prebuilt = a.ndk / 'toolchains' / 'llvm' / 'prebuilt'
        hosts = ('windows-x86_64', 'linux-x86_64', 'darwin-arm64', 'darwin-x86_64')
        compilers = [prebuilt / host / 'bin' / ('clang++.exe' if host.startswith('windows') else 'clang++')
                     for host in hosts]
        compiler = next((path for path in compilers if path.is_file()), None)
        if compiler is None:
            raise SystemExit(f'No Android NDK clang++ found under {prebuilt}')
        arm64 = output.parent / 'libdh2_texture_assets_arm64.so'
        subprocess.run([str(compiler), '--target=aarch64-linux-android26',
                        '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror', '-fPIC',
                        '-fno-exceptions', '-fno-rtti', '-shared', str(ROOT / 'texture.cpp'),
                        '-nostdlib++', '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
                        '-o', str(arm64)], check=True)
        print(f'Android ARM64 build: {arm64}')
    check = [sys.executable, str(ROOT / 'tests' / 'check.py'), '--library', str(output)]
    if a.cache_zip:
        check += ['--cache-zip', str(a.cache_zip), '--report', str(a.report)]
    subprocess.run(check, check=True)


if __name__ == '__main__':
    main()
