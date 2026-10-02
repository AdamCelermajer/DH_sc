#!/usr/bin/env python3
"""Build the checked skin component on a host; optionally cross-compile Android."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parent
SOURCES = [ROOT/'skin.cpp', ROOT/'../asset-payloads/payloads.cpp',
           ROOT/'../engine-resources/resources.cpp', ROOT/'../scene-payloads/scene.cpp',
           ROOT/'../engine-math/math.cpp']

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--output', type=Path, default=ROOT/'build')
    p.add_argument('--ndk', type=Path)
    p.add_argument('--report', type=Path)
    a = p.parse_args(); a.output.mkdir(parents=True, exist_ok=True)
    flags = ['-std=c++17', '-O2', '-fPIC', '-fno-exceptions', '-fno-rtti',
             '-fno-fast-math', '-ffp-contract=off', '-Wall', '-Wextra', '-Werror']
    host = a.output/('skin-host.dll' if os.name == 'nt' else 'skin-host.so')
    subprocess.run([os.environ.get('CXX', 'c++'), *flags, '-shared', *map(str,SOURCES),
                    '-lm', '-o', str(host)], check=True)
    artifacts = [host]
    if a.ndk:
        cc = a.ndk/'toolchains/llvm/prebuilt'/('windows-x86_64' if os.name=='nt' else 'linux-x86_64')/'bin'/('clang++.exe' if os.name=='nt' else 'clang++')
        for abi, target in [('arm64', 'aarch64-linux-android35'), ('x86_64', 'x86_64-linux-android35')]:
            path = a.output/f'skin-{abi}.so'
            subprocess.run([str(cc), f'--target={target}', *flags, '-shared', *map(str,SOURCES),
                            '-nostdlib++', '-lm', '-Wl,-z,max-page-size=16384',
                            '-Wl,--no-undefined', '-o', str(path)], check=True)
            artifacts.append(path)
    sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    result = {'complete_engine': False, 'host_build': True, 'android_build': bool(a.ndk),
              'source_sha256': {str(f.relative_to(ROOT.parent)):sha(f) for f in [*SOURCES,ROOT/'skin.hpp']},
              'artifact_sha256': {f.name:sha(f) for f in artifacts}}
    if a.report: a.report.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result))

if __name__ == '__main__': main()
