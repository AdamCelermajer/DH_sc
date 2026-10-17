#!/usr/bin/env python3
"""Run the static draw walker on a 16 KiB Android emulator without an APK."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess


HERE = Path(__file__).resolve().parents[1]
BINARY = HERE / 'build/dh2-scene-draw-android-x86_64'
REMOTE_BINARY = '/data/local/tmp/dh2-scene-draw-smoke'
REMOTE_SAMPLE = '/data/local/tmp/dh2-scene-candle.bdae'
RESULT = re.compile(r'^scene draw: nodes=(\d+) commands=(\d+) triangles=(\d+) '
                    r'unresolved_geometry=(\d+) unresolved_materials=(\d+)$')


def digest(path: Path) -> str:
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def adb(command: Path, serial: str, *args: str) -> str:
    result = subprocess.run([str(command), '-s', serial, *args],
                            capture_output=True, text=True, check=True)
    return result.stdout.strip()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb', required=True, type=Path)
    parser.add_argument('--serial', required=True)
    parser.add_argument('--sample', required=True, type=Path,
                        help='private BRES sample; bytes are not committed')
    parser.add_argument('--report', type=Path, default=HERE / 'runtime-validation.json')
    args = parser.parse_args()
    version = adb(args.adb, args.serial, 'shell', 'getprop', 'ro.build.version.release')
    sdk = adb(args.adb, args.serial, 'shell', 'getprop', 'ro.build.version.sdk')
    page = adb(args.adb, args.serial, 'shell', 'getconf', 'PAGE_SIZE')
    abi = adb(args.adb, args.serial, 'shell', 'getprop', 'ro.product.cpu.abilist')
    qemu = adb(args.adb, args.serial, 'shell', 'getprop', 'ro.kernel.qemu')
    if (version, sdk, page, qemu) != ('17', '37', '16384', '1') or 'x86_64' not in abi.split(','):
        raise ValueError(f'expected Android 17/API 37 x86_64 16 KiB emulator; '
                         f'got {version=} {sdk=} {page=} {abi=} {qemu=}')
    adb(args.adb, args.serial, 'push', str(BINARY), REMOTE_BINARY)
    adb(args.adb, args.serial, 'push', str(args.sample), REMOTE_SAMPLE)
    adb(args.adb, args.serial, 'shell', 'chmod', '755', REMOTE_BINARY)
    hashes = adb(args.adb, args.serial, 'shell', 'sha256sum',
                 REMOTE_BINARY, REMOTE_SAMPLE)
    observed = dict(line.split()[:2][::-1] for line in hashes.splitlines())
    if observed.get(REMOTE_BINARY) != digest(BINARY) or \
       observed.get(REMOTE_SAMPLE) != digest(args.sample):
        raise ValueError('on-device bytes differ from host inputs')
    output = adb(args.adb, args.serial, 'shell', REMOTE_BINARY, REMOTE_SAMPLE)
    match = RESULT.fullmatch(output)
    if not match or int(match.group(2)) < 1:
        raise ValueError(f'unexpected draw-walk output: {output!r}')
    names = ('nodes', 'draw_commands', 'triangles',
             'unresolved_geometry', 'unresolved_materials')
    report = {
        'scope': 'isolated static BRES draw descriptors on an x86_64 Android emulator; no game renderer',
        'android_runtime_tested': True,
        'android_release': version,
        'android_api': int(sdk),
        'abi_list': abi,
        'page_size': int(page),
        'emulator': True,
        'remote_hashes_match_local': True,
        'binary_sha256': digest(BINARY),
        'sample_sha256': digest(args.sample),
        'sample_bytes': args.sample.stat().st_size,
        'result': dict(zip(names, map(int, match.groups()))),
        'exit_code': 0,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(output)
    print(f'Android runtime report: {args.report}')


if __name__ == '__main__':
    main()
