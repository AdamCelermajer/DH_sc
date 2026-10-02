#!/usr/bin/env python3
"""Build the static scene draw bridge for the host and Android ABIs."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import struct
import subprocess


HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent
SOURCES = (
    HERE / 'draw.cpp',
    REPO / 'port/scene-payloads/scene.cpp',
    REPO / 'port/asset-payloads/payloads.cpp',
    REPO / 'port/material-bindings/bindings.cpp',
    REPO / 'port/engine-resources/resources.cpp',
    REPO / 'port/engine-math/math.cpp',
)
SMOKE = HERE / 'tests/android_smoke.cpp'
RUNNER = HERE / 'tests/run_android.py'


def digest(path: Path) -> str:
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def elf(path: Path, machine: int) -> dict:
    raw = path.read_bytes()
    if len(raw) < 64 or raw[:6] != b'\x7fELF\x02\x01':
        raise ValueError(f'{path}: expected little-endian ELF64')
    kind, observed = struct.unpack_from('<HH', raw, 16)
    if (kind, observed) != (3, machine):
        raise ValueError(f'{path}: unexpected ELF type/machine {(kind, observed)}')
    phoff = struct.unpack_from('<Q', raw, 32)[0]
    phsize, phcount = struct.unpack_from('<HH', raw, 54)
    if phsize < 56 or phoff + phsize * phcount > len(raw):
        raise ValueError(f'{path}: invalid program headers')
    loads = []
    for index in range(phcount):
        kind, _, offset, virtual, _, count, _, alignment = struct.unpack_from(
            '<IIQQQQQQ', raw, phoff + index * phsize)
        if kind == 1:
            if (offset + count > len(raw) or alignment < 16384 or
                    offset % 16384 != virtual % 16384):
                raise ValueError(f'{path}: invalid 16 KiB PT_LOAD {index}')
            loads.append(alignment)
    if not loads:
        raise ValueError(f'{path}: no PT_LOAD segments')
    return {'bytes': len(raw), 'sha256': digest(path),
            'machine': 'EM_AARCH64' if machine == 183 else 'EM_X86_64',
            'pt_load_alignments': loads}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ndk', type=Path, help='Android NDK r29')
    parser.add_argument('--cxx', default=os.environ.get('CXX', 'g++'))
    parser.add_argument('--sample', type=Path, help='optional private BRES for host smoke')
    parser.add_argument('--report', type=Path, default=HERE / 'build-validation.json')
    args = parser.parse_args()
    output = HERE / 'build'
    output.mkdir(exist_ok=True)
    flags = ['-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
             '-fPIC', '-fno-exceptions', '-fno-rtti', '-fno-fast-math',
             '-ffp-contract=off']
    host = output / ('libdh2_scene_draw_host.dll' if os.name == 'nt'
                     else 'libdh2_scene_draw_host.so')
    subprocess.run([args.cxx, *flags, '-shared', *map(str, SOURCES),
                    '-o', str(host)], check=True)
    host_smoke = output / ('dh2-scene-draw-host.exe' if os.name == 'nt'
                           else 'dh2-scene-draw-host')
    subprocess.run([args.cxx, *flags, str(SMOKE), *map(str, SOURCES),
                    '-o', str(host_smoke)], check=True)
    if args.sample:
        subprocess.run([str(host_smoke), str(args.sample)], check=True)
    report = {
        'scope': 'new static BRES scene-to-draw bridge; no original renderer or gameplay',
        'host': {'bytes': host.stat().st_size, 'sha256': digest(host)},
        'host_smoke': {'bytes': host_smoke.stat().st_size,
                       'sha256': digest(host_smoke),
                       'private_sample_tested': bool(args.sample)},
        'android': {},
        'android_runtime_tested': False,
        'source_sha256': {path.relative_to(REPO).as_posix(): digest(path)
                          for path in (*SOURCES, HERE / 'draw.hpp', SMOKE, RUNNER)},
    }
    if args.ndk:
        folder = 'windows-x86_64' if os.name == 'nt' else 'linux-x86_64'
        compiler = (args.ndk / 'toolchains/llvm/prebuilt' / folder / 'bin' /
                    ('clang++.exe' if os.name == 'nt' else 'clang++'))
        if not compiler.is_file():
            raise FileNotFoundError(compiler)
        for abi, target, machine in (
            ('x86_64', 'x86_64-linux-android35', 62),
            ('arm64-v8a', 'aarch64-linux-android35', 183),
        ):
            binary = output / f'libdh2_scene_draw_{abi}.so'
            subprocess.run([str(compiler), f'--target={target}', *flags,
                            '-shared', '-nostdlib++',
                            '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
                            *map(str, SOURCES), '-o', str(binary)], check=True)
            report['android'][abi] = elf(binary, machine)
            if abi == 'x86_64':
                smoke = output / 'dh2-scene-draw-android-x86_64'
                subprocess.run([str(compiler), f'--target={target}', *flags,
                                '-fPIE', '-pie', '-nostdlib++',
                                '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
                                str(SMOKE), *map(str, SOURCES), '-o', str(smoke)],
                               check=True)
                report['android_smoke_executable'] = elf(smoke, machine)
                runtime_path = HERE / 'runtime-validation.json'
                if runtime_path.is_file():
                    runtime = json.loads(runtime_path.read_text(encoding='utf-8'))
                    if (runtime.get('android_runtime_tested') is True and
                            runtime.get('binary_sha256') ==
                            report['android_smoke_executable']['sha256']):
                        report['android_runtime_tested'] = True
                        report['runtime_report'] = 'runtime-validation.json'
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
