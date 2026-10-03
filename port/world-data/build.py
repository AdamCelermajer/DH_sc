#!/usr/bin/env python3
"""Build the owned world records and checked catalogue placement helpers."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

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
    sources = [ROOT/'world.cpp', ROOT/'world_scene.cpp',
               ROOT/'../scene-payloads/scene.cpp',
               ROOT/'../engine-resources/resources.cpp', ROOT/'../engine-math/math.cpp']
    flags = ['-std=c++17', '-O2', '-fPIC', '-fno-exceptions', '-fno-rtti',
             '-fno-fast-math', '-ffp-contract=off', '-fno-builtin',
             '-Wall', '-Wextra', '-Werror']
    host = args.output/('libdh2_world_host.dll' if os.name == 'nt' else 'libdh2_world_host.so')
    host_link = ['-Wl,--no-insert-timestamp'] if os.name == 'nt' else []
    subprocess.run([os.environ.get('CXX', 'c++'), *flags, '-shared', *host_link,
                    *(str(p) for p in sources), '-lm', '-o', str(host)], check=True)
    artifacts = {host.name: digest(host)}
    if args.ndk:
        folder = 'windows-x86_64' if os.name == 'nt' else 'linux-x86_64'
        suffix = '.cmd' if os.name == 'nt' else ''
        compiler = args.ndk/'toolchains/llvm/prebuilt'/folder/'bin'/('aarch64-linux-android26-clang++'+suffix)
        arm64 = args.output/'libdh2_world_arm64.so'
        subprocess.run([str(compiler), *flags, '-shared',
                        *(str(p) for p in sources), '-lm', '-nostdlib++',
                        '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
                        '-o', str(arm64)], check=True)
        artifacts[arm64.name] = digest(arm64)
    inputs = sources + [ROOT/'world.hpp', ROOT/'world_scene.hpp',
                       ROOT/'../scene-payloads/scene.hpp',
                       ROOT/'../engine-resources/resources.hpp', ROOT/'../engine-math/math.hpp']
    report = {
        'host_build': True, 'arm64_build': bool(args.ndk),
        'complete_engine': False, 'scene_rendered': False,
        'source_sha256': {os.path.relpath(p.resolve(), ROOT).replace('\\', '/') : digest(p)
                          for p in inputs},
        'artifact_sha256': artifacts,
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
