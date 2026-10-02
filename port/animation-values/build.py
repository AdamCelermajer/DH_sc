#!/usr/bin/env python3
"""Build isolated float scene-track calculations for host and Android ARM64."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess

ROOT = Path(__file__).resolve().parent
SOURCES = [ROOT/'values.cpp', ROOT/'../asset-payloads/payloads.cpp',
           ROOT/'../engine-resources/resources.cpp', ROOT/'../engine-math/math.cpp']

def arm64_alignment(binary):
    raw = binary.read_bytes()
    assert raw[:6] == b'\x7fELF\x02\x01' and struct.unpack_from('<H',raw,18)[0] == 183
    phoff = struct.unpack_from('<Q',raw,32)[0]
    stride,count = struct.unpack_from('<HH',raw,54)
    alignment = []
    for i in range(count):
        typ,_,offset,address,_,_,_,align = struct.unpack_from('<IIQQQQQQ',raw,phoff+i*stride)
        if typ == 1:
            assert align >= 16384 and offset % 16384 == address % 16384
            alignment.append({'alignment':align,'offset':offset})
    assert alignment
    return alignment

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--ndk', type=Path)
    p.add_argument('--arm64-only', action='store_true')
    p.add_argument('--output', type=Path, default=ROOT/'build')
    p.add_argument('--report', type=Path, default=ROOT/'build-validation.json')
    p.add_argument('--sanitize', type=Path, help='Run host safety checks on a privately supplied animation BRES')
    a = p.parse_args(); a.output.mkdir(parents=True, exist_ok=True)
    flags = ['-std=c++17', '-O2', '-fPIC', '-shared', '-fno-builtin', '-fno-fast-math',
             '-ffp-contract=off', '-fno-exceptions', '-fno-rtti', '-Wall', '-Wextra', '-Werror']
    artifacts = []; alignment = []
    if not a.arm64_only:
        binary = a.output/'values-host.so'
        subprocess.run([os.environ.get('CXX', 'c++'), *flags, *map(str,SOURCES), '-lm',
                        '-Wl,-Bsymbolic', '-o', str(binary)], check=True)
        artifacts.append(binary)
    if a.ndk:
        platform = 'windows-x86_64' if os.name == 'nt' else 'linux-x86_64'
        compiler = a.ndk/f'toolchains/llvm/prebuilt/{platform}/bin/clang++'
        if os.name == 'nt': compiler = compiler.with_suffix('.exe')
        binary = a.output/'values-arm64.so'
        subprocess.run([str(compiler), '--target=aarch64-linux-android26', *flags,
                        *map(str,SOURCES), '-lm', '-nostdlib++', '-Wl,-Bsymbolic',
                        '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined', '-o', str(binary)], check=True)
        artifacts.append(binary)
        alignment = arm64_alignment(binary)
    if not artifacts: p.error('No build selected; --arm64-only requires --ndk')
    if a.sanitize:
        if a.arm64_only: p.error('--sanitize requires a host compiler')
        binary = a.output/'values-safety'
        subprocess.run([os.environ.get('CXX','c++'), '-std=c++17', '-O1', '-g',
                        '-fno-fast-math', '-ffp-contract=off', '-fsanitize=address,undefined',
                        '-fno-omit-frame-pointer', *map(str,SOURCES),
                        str(ROOT/'tests/safety.cpp'), '-o',str(binary)],check=True)
        subprocess.run([str(binary.resolve()),str(a.sanitize.resolve())],check=True)
    sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    result = {'complete_engine': False, 'host_build': not a.arm64_only, 'arm64_build': bool(a.ndk),
              'source_sha256': {str(p.resolve().relative_to(ROOT.parent)).replace('\\','/'):sha(p)
                                for p in [*SOURCES, *[p.with_suffix('.hpp') for p in SOURCES]]},
              'artifact_sha256': {p.name:sha(p) for p in artifacts}}
    if alignment: result['arm64_pt_load'] = alignment
    if a.sanitize:
        result['safety'] = {'address_sanitizer':True,'undefined_behavior_sanitizer':True,
                            'cases':3000,'seed':20261002,'sample_sha256':sha(a.sanitize),
                            'test_sha256':sha(ROOT/'tests/safety.cpp')}
    a.report.write_text(json.dumps(result, indent=2)+'\n'); print(json.dumps(result))
if __name__ == '__main__': main()
