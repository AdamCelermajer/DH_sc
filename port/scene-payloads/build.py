#!/usr/bin/env python3
"""Build the checked scene views for the host and optionally Android ARM64."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--ndk', type=Path, default=os.environ.get('ANDROID_NDK_HOME'))
    parser.add_argument('--output', type=Path, default=ROOT/'build')
    parser.add_argument('--report', type=Path)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    sources = [ROOT/'scene.cpp', ROOT/'../engine-resources/resources.cpp',
               ROOT/'../engine-math/math.cpp']
    flags = ['-std=c++17', '-O2', '-fPIC', '-fno-exceptions', '-fno-rtti',
             '-fno-fast-math', '-ffp-contract=off', '-fno-builtin',
             '-Wall', '-Wextra', '-Werror']
    host = args.output/('libdh2_scene_host.dll' if os.name == 'nt' else 'libdh2_scene_host.so')
    subprocess.run([os.environ.get('CXX', 'c++'), *flags, '-shared',
                    *(str(p) for p in sources), '-lm', '-o', str(host)], check=True)
    artifacts = {host.name: digest(host)}
    if args.ndk:
        folder = 'windows-x86_64' if os.name == 'nt' else 'linux-x86_64'
        suffix = '.cmd' if os.name == 'nt' else ''
        compiler = args.ndk/'toolchains/llvm/prebuilt'/folder/'bin'/('aarch64-linux-android26-clang++'+suffix)
        arm64 = args.output/'libdh2_scene_arm64.so'
        subprocess.run([str(compiler), *flags, '-shared',
                        *(str(p) for p in sources), '-lm', '-nostdlib++',
                        '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
                        '-o', str(arm64)], check=True)
        artifacts[arm64.name] = digest(arm64)
    inputs = {
        'scene.cpp': ROOT/'scene.cpp', 'scene.hpp': ROOT/'scene.hpp',
        '../engine-resources/resources.cpp': ROOT/'../engine-resources/resources.cpp',
        '../engine-resources/resources.hpp': ROOT/'../engine-resources/resources.hpp',
        '../engine-math/math.cpp': ROOT/'../engine-math/math.cpp',
        '../engine-math/math.hpp': ROOT/'../engine-math/math.hpp',
    }
    report = {
        'host_build': True, 'arm64_build': bool(args.ndk),
        'complete_engine': False,
        'source_sha256': {name: digest(path) for name, path in inputs.items()},
        'artifact_sha256': artifacts,
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
