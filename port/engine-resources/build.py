#!/usr/bin/env python3
"""Build the resource component and validation fixtures, not a game APK."""
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
    p.add_argument('--output', type=Path, default=ROOT / 'build')
    p.add_argument('--sanitize', type=Path, help='Run host ASan/UBSan input checks using this recovered BRES file')
    p.add_argument('--report', type=Path, help='Write build/sanitizer evidence as JSON')
    a = p.parse_args()
    a.output.mkdir(parents=True, exist_ok=True)
    common = ['-std=c++17', '-O2', '-fPIC', '-fno-builtin', '-fno-exceptions',
              '-fno-rtti', '-Wall', '-Wextra', '-Werror']
    sources = [str(ROOT / 'resources.cpp')]
    subprocess.run([os.environ.get('CXX', 'c++'), *common, '-shared', *sources,
                    '-Wl,-Bsymbolic', '-o', str(a.output / 'libdh2_engine_resources_host.so')], check=True)
    fixture = str(ROOT / 'tests/fixtures.cpp')
    if Path(fixture).exists():
        subprocess.run([os.environ.get('CXX', 'c++'), *common, '-shared', *sources, fixture,
                        '-Wl,-Bsymbolic', '-o', str(a.output / 'libresources_fixture_host.so')], check=True)
    if a.ndk:
        compiler = a.ndk / 'toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android26-clang++'
        for name, files in [('libdh2_engine_resources_arm64.so', sources),
                            ('libresources_fixture_arm64.so', sources + [fixture])]:
            if not all(Path(f).exists() for f in files):
                continue
            subprocess.run([str(compiler), *common, '-shared', *files, '-nostdlib++',
                            '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
                            '-Wl,-Bsymbolic', '-o', str(a.output / name)], check=True)
    if a.sanitize:
        target = a.output / 'resources-safety'
        subprocess.run([os.environ.get('CXX', 'c++'), '-std=c++17', '-O1', '-g',
                        '-fsanitize=address,undefined', '-fno-omit-frame-pointer',
                        *sources, str(ROOT/'tests/safety.cpp'), '-o', str(target)], check=True)
        subprocess.run([str(target.resolve()), str(a.sanitize.resolve())], check=True)
    report = {'host_build': True, 'arm64_build': bool(a.ndk),
              'sanitizer_passed': bool(a.sanitize),
              'sanitizer_cases': 10000 if a.sanitize else 0,
              'asan_options': os.environ.get('ASAN_OPTIONS', ''),
              'complete_engine': False,
              'source_sha256': {str(f.relative_to(ROOT)): hashlib.sha256(f.read_bytes()).hexdigest()
                                for f in [ROOT/'resources.cpp', ROOT/'resources.hpp',
                                          ROOT/'tests/safety.cpp', ROOT/'build.py']},
              'artifact_sha256': {f.name: hashlib.sha256(f.read_bytes()).hexdigest()
                                  for f in sorted(a.output.glob('*.so'))}}
    if a.sanitize:
        report['sanitizer_sample_sha256'] = hashlib.sha256(a.sanitize.read_bytes()).hexdigest()
    if a.report:
        a.report.parent.mkdir(parents=True, exist_ok=True)
        a.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps({**report, 'output': str(a.output.resolve())}))

if __name__ == '__main__':
    main()
