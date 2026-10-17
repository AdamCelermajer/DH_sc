#!/usr/bin/env python3
"""Build the portable parser and run its independent fixture/cache checks."""

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import struct
import sys

ROOT = Path(__file__).resolve().parent


def verify_arm64_elf(path):
    elf = path.read_bytes()
    if len(elf) < 64 or elf[:6] != b'\x7fELF\x02\x01':
        raise ValueError('Android build is not a little-endian ELF64')
    kind, machine = struct.unpack_from('<HH', elf, 16)
    if (kind, machine) != (3, 183):
        raise ValueError(f'expected AArch64 shared object, got ELF type/machine {(kind, machine)}')
    phoff = struct.unpack_from('<Q', elf, 32)[0]
    phsize, phcount = struct.unpack_from('<HH', elf, 54)
    if phsize < 56 or phoff + phsize * phcount > len(elf):
        raise ValueError('invalid ELF program-header bounds')
    loads = []
    for i in range(phcount):
        ptype, _, offset, vaddr, _, file_size, _, alignment = struct.unpack_from(
            '<IIQQQQQQ', elf, phoff + i * phsize)
        if ptype == 1:
            if offset + file_size > len(elf) or alignment != 16384 \
                    or offset % alignment != vaddr % alignment:
                raise ValueError(f'PT_LOAD {i} is not a valid 16 KiB-aligned segment')
            loads.append(alignment)
    if not loads:
        raise ValueError('Android build has no PT_LOAD segments')
    report = {
        'android_api': 26,
        'elf_class': 'ELF64',
        'elf_machine': 'EM_AARCH64',
        'pt_load_alignments': loads,
        'pt_load_offset_vaddr_congruent': True,
        'artifact_bytes': len(elf),
        'artifact_sha256': hashlib.sha256(elf).hexdigest(),
        'source_sha256': {name: hashlib.sha256((ROOT / name).read_bytes()).hexdigest()
                          for name in ('texture.cpp', 'texture.hpp', 'decode.cpp')},
        'runtime_tested': False,
    }
    (ROOT / 'arm64-build-validation.json').write_text(
        json.dumps(report, indent=2) + '\n', encoding='utf-8')
    return report


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--cache-zip', type=Path, help='Complete owner-supplied cache ZIP')
    p.add_argument('--cache-root', type=Path, help='Extracted complete cache root')
    p.add_argument('--report', type=Path, default=ROOT / 'validation.json')
    p.add_argument('--cxx', default=os.environ.get('CXX', 'g++'))
    p.add_argument('--ndk', type=Path, default=os.environ.get('ANDROID_NDK_HOME'),
                   help='Also compile Android ARM64 with an Android NDK')
    a = p.parse_args()
    output = ROOT / 'build' / ('dh2_texture_assets.dll' if os.name == 'nt' else 'libdh2_texture_assets.so')
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [a.cxx, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC',
               str(ROOT / 'texture.cpp'), str(ROOT / 'decode.cpp'), '-o', str(output)]
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
                        str(ROOT / 'decode.cpp'),
                        '-nostdlib++', '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
                        '-o', str(arm64)], check=True)
        report = verify_arm64_elf(arm64)
        print(f'Android ARM64 build: {arm64}')
        print(f'PT_LOAD alignments: {report["pt_load_alignments"]}')
    check = [sys.executable, str(ROOT / 'tests' / 'check.py'), '--library', str(output)]
    if a.cache_zip:
        check += ['--cache-zip', str(a.cache_zip), '--report', str(a.report)]
    subprocess.run(check, check=True)
    pixels = [sys.executable, str(ROOT / 'tests' / 'decode.py'),
              '--library', str(output)]
    cache_source = a.cache_root or a.cache_zip
    if cache_source:
        pixels += ['--cache-source', str(cache_source),
                   '--report', str(ROOT / 'pixel-validation.json')]
    subprocess.run(pixels, check=True)


if __name__ == '__main__':
    main()
