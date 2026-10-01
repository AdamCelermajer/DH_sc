#!/usr/bin/env python3
"""Build and validate a source-only Android x86_64 asset-reader smoke test."""

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
    HERE / 'smoke.cpp',
    REPO / 'port/texture-assets/texture.cpp',
    REPO / 'port/texture-assets/decode.cpp',
    REPO / 'port/material-bindings/bindings.cpp',
    REPO / 'port/engine-resources/resources.cpp',
)


def sha256(path: Path) -> str:
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def verify_elf(path: Path) -> dict:
    raw = path.read_bytes()
    if len(raw) < 64 or raw[:6] != b'\x7fELF\x02\x01':
        raise ValueError('expected little-endian ELF64')
    kind, machine = struct.unpack_from('<HH', raw, 16)
    if (kind, machine) != (3, 62):
        raise ValueError(f'expected PIE x86_64 ELF, got {(kind, machine)}')
    phoff = struct.unpack_from('<Q', raw, 32)[0]
    phsize, phcount = struct.unpack_from('<HH', raw, 54)
    if phsize < 56 or phoff + phsize * phcount > len(raw):
        raise ValueError('invalid ELF program-header bounds')
    segments = []
    interpreter = None
    for index in range(phcount):
        ptype, _, offset, vaddr, _, filesz, _, align = struct.unpack_from(
            '<IIQQQQQQ', raw, phoff + index * phsize)
        if offset + filesz > len(raw):
            raise ValueError(f'program header {index} exceeds file')
        if ptype == 1:
            if align < 16384 or offset % 16384 != vaddr % 16384:
                raise ValueError(f'PT_LOAD {index} violates 16 KiB page alignment')
            segments.append({'alignment': align,
                             'offset_vaddr_congruent_16k': True})
        elif ptype == 3:
            interpreter = raw[offset:offset + filesz].rstrip(b'\0').decode('ascii')
    if not segments or interpreter != '/system/bin/linker64':
        raise ValueError('missing Android 64-bit interpreter or PT_LOAD')
    return {
        'elf_class': 'ELF64', 'elf_type': 'ET_DYN_PIE',
        'machine': 'EM_X86_64', 'interpreter': interpreter,
        'pt_load_segments': segments, 'bytes': len(raw), 'sha256': sha256(path),
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--ndk', type=Path, required=True)
    parser.add_argument('--host-cxx', default=os.environ.get('CXX', 'g++'))
    parser.add_argument('--texture', type=Path, help='optional private BTEX/PVR fixture for host check')
    parser.add_argument('--bres', type=Path, help='optional private BRES fixture for host check')
    parser.add_argument('--report', type=Path, default=HERE / 'build-validation.json')
    args = parser.parse_args()
    if args.bres and not args.texture:
        parser.error('--bres requires --texture')
    build = HERE / 'build'
    build.mkdir(exist_ok=True)
    host_output = build / ('dh2-asset-smoke-host.exe' if os.name == 'nt' else 'dh2-asset-smoke-host')
    android_output = build / 'dh2-asset-smoke-android-x86_64'
    common = ['-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
              *map(str, SOURCES)]
    subprocess.run([args.host_cxx, *common, '-o', str(host_output)], check=True)
    host_run = [str(host_output)]
    if args.texture:
        host_run.append(str(args.texture))
    if args.bres:
        host_run.append(str(args.bres))
    subprocess.run(host_run, check=True)

    prebuilt = args.ndk / 'toolchains/llvm/prebuilt'
    host = 'windows-x86_64' if os.name == 'nt' else 'linux-x86_64'
    compiler = prebuilt / host / 'bin' / ('clang++.exe' if os.name == 'nt' else 'clang++')
    if not compiler.is_file():
        raise FileNotFoundError(compiler)
    # NDK r29 provides API 35 headers; the executable is intended for an API 37 AVD.
    subprocess.run([
        str(compiler), '--target=x86_64-linux-android35',
        '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror',
        '-fPIC', '-fPIE', '-pie', '-fno-exceptions', '-fno-rtti', '-nostdlib++',
        '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
        *map(str, SOURCES), '-o', str(android_output),
    ], check=True)
    elf = verify_elf(android_output)
    runtime_path = HERE / 'runtime-validation.json'
    runtime_match = False
    if runtime_path.is_file():
        recorded = json.loads(runtime_path.read_text(encoding='utf-8'))
        runtime_match = (recorded.get('android_runtime_tested') is True and
                         recorded.get('executable_sha256') == elf['sha256'])
    report = {
        'scope': 'standalone source-only texture and BRES/material reader smoke harness; no game renderer or APK',
        'ndk_api': 35,
        'intended_avd_api': 37,
        'host_synthetic_checks_passed': True,
        'host_private_fixture_checks_passed': bool(args.texture and args.bres),
        'android_runtime_tested': runtime_match,
        'runtime_report': 'runtime-validation.json' if runtime_match else None,
        'elf': elf,
        'source_sha256': {
            path.relative_to(REPO).as_posix(): sha256(path) for path in SOURCES
        },
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(f'Android executable: {android_output}')
    print(f'16 KiB PT_LOAD segments: {len(report["elf"]["pt_load_segments"])}')


if __name__ == '__main__':
    main()
