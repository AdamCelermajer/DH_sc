#!/usr/bin/env python3
"""Build the immutable mesh/animation component for host and Android ARM64."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parent

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--ndk', type=Path, default=os.environ.get('ANDROID_NDK_HOME'))
    p.add_argument('--output', type=Path, default=ROOT/'build')
    p.add_argument('--sanitize', type=Path, help='Run 5,000 host corruption/truncation probes with this BRES image')
    p.add_argument('--report', type=Path)
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=True)
    flags = ['-std=c++17', '-O2', '-fPIC', '-fno-exceptions', '-fno-rtti',
             '-fno-fast-math', '-ffp-contract=off', '-Wall', '-Wextra', '-Werror']
    sources = [str(ROOT/'payloads.cpp'), str(ROOT/'../engine-resources/resources.cpp')]
    subprocess.run([os.environ.get('CXX', 'c++'), *flags, '-shared', *sources,
                    '-Wl,-Bsymbolic', '-o', str(a.output/'libdh2_asset_payloads_host.so')], check=True)
    if a.ndk:
        cc = a.ndk/'toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android26-clang++'
        subprocess.run([str(cc), *flags, '-shared', *sources, '-nostdlib++',
                        '-Wl,-Bsymbolic', '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
                        '-o', str(a.output/'libdh2_asset_payloads_arm64.so')], check=True)
    if a.sanitize:
        target = a.output/'payloads-safety'
        subprocess.run([os.environ.get('CXX', 'c++'), '-std=c++17', '-O1', '-g',
                        '-fno-fast-math', '-ffp-contract=off', '-fsanitize=address,undefined',
                        '-fno-omit-frame-pointer', *sources, str(ROOT/'tests/safety.cpp'),
                        '-o', str(target)], check=True)
        subprocess.run([str(target.resolve()), str(a.sanitize.resolve())], check=True)
    inputs = [ROOT/'payloads.cpp', ROOT/'payloads.hpp', ROOT/'build.py', ROOT/'tests/safety.cpp',
              ROOT/'../engine-resources/resources.cpp', ROOT/'../engine-resources/resources.hpp']
    report = {'host_build': True, 'arm64_build': bool(a.ndk),
              'sanitizer_passed': bool(a.sanitize), 'sanitizer_cases': 5000 if a.sanitize else 0,
              'asan_options': os.environ.get('ASAN_OPTIONS', ''), 'complete_engine': False,
              'source_sha256': {str(f.relative_to(ROOT)) if f.is_relative_to(ROOT) else '../engine-resources/'+f.name:
                                hashlib.sha256(f.read_bytes()).hexdigest() for f in inputs},
              'artifact_sha256': {f.name: hashlib.sha256(f.read_bytes()).hexdigest() for f in sorted(a.output.glob('libdh2_asset_payloads_*.so'))}}
    if a.sanitize:
        report['sanitizer_sample_sha256'] = hashlib.sha256(a.sanitize.read_bytes()).hexdigest()
    if a.report:
        a.report.parent.mkdir(parents=True, exist_ok=True)
        a.report.write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps({**report, 'output': str(a.output.resolve())}))

if __name__ == '__main__':
    main()
