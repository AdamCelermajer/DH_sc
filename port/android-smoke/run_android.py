#!/usr/bin/env python3
"""Run the standalone asset-reader smoke check on a 16 KiB Android x86_64 AVD."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import subprocess


HERE = Path(__file__).resolve().parent
EXECUTABLE = HERE / 'build/dh2-asset-smoke-android-x86_64'
REMOTE = '/data/local/tmp/dh2-asset-smoke'


def sha256(path: Path) -> str:
    with path.open('rb') as stream:
        return hashlib.file_digest(stream, 'sha256').hexdigest()


def run(adb: Path, serial: str, *arguments: str) -> str:
    result = subprocess.run([str(adb), '-s', serial, *arguments],
                            check=True, capture_output=True, text=True)
    return result.stdout.strip()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--adb', required=True, type=Path)
    parser.add_argument('--serial', required=True)
    parser.add_argument('--texture', type=Path, help='optional private cache texture')
    parser.add_argument('--bres', type=Path, help='optional private cache BRES')
    parser.add_argument('--report', type=Path, default=HERE / 'runtime-validation.json')
    args = parser.parse_args()
    if args.bres and not args.texture:
        parser.error('--bres requires --texture')
    if not EXECUTABLE.is_file():
        raise FileNotFoundError(EXECUTABLE)
    version = run(args.adb, args.serial, 'shell', 'getprop', 'ro.build.version.release')
    sdk = run(args.adb, args.serial, 'shell', 'getprop', 'ro.build.version.sdk')
    page = run(args.adb, args.serial, 'shell', 'getconf', 'PAGE_SIZE')
    abi = run(args.adb, args.serial, 'shell', 'getprop', 'ro.product.cpu.abilist')
    qemu = run(args.adb, args.serial, 'shell', 'getprop', 'ro.kernel.qemu')
    if (version, sdk, page, qemu) != ('17', '37', '16384', '1') or 'x86_64' not in abi.split(','):
        raise ValueError(f'expected Android 17/API 37 x86_64 emulator with 16 KiB pages; '
                         f'got version={version}, sdk={sdk}, page={page}, abi={abi}, qemu={qemu}')
    local = [(EXECUTABLE, REMOTE)]
    if args.texture:
        local.append((args.texture, '/data/local/tmp/dh2-smoke-texture.tga'))
    if args.bres:
        local.append((args.bres, '/data/local/tmp/dh2-smoke-bres.bdae'))
    for source, target in local:
        run(args.adb, args.serial, 'push', str(source), target)
    run(args.adb, args.serial, 'shell', 'chmod', '755', REMOTE)
    remote_hashes = run(args.adb, args.serial, 'shell', 'sha256sum', *(target for _, target in local))
    observed = dict(line.split()[:2][::-1] for line in remote_hashes.splitlines())
    for source, target in local:
        if observed.get(target) != sha256(source):
            raise ValueError(f'emulator file hash differs: {target}')
    synthetic = run(args.adb, args.serial, 'shell', REMOTE)
    if synthetic != 'synthetic texture and material checks passed':
        raise ValueError(f'unexpected synthetic output: {synthetic!r}')
    external = None
    if args.texture:
        external = run(args.adb, args.serial, 'shell', REMOTE,
                       *(target for _, target in local[1:]))
        if not external.startswith(synthetic + '\nexternal texture:'):
            raise ValueError(f'unexpected external fixture output: {external!r}')
        if args.bres and '\nexternal BRES:' not in external:
            raise ValueError(f'missing BRES fixture result: {external!r}')
    report = {
        'scope': 'standalone reconstructed texture and material readers, not original game engine or renderer',
        'android_runtime_tested': True,
        'emulator': True,
        'android_release': version,
        'android_api': int(sdk),
        'abi_list': abi,
        'page_size': int(page),
        'executable_sha256': sha256(EXECUTABLE),
        'executable_bytes': EXECUTABLE.stat().st_size,
        'remote_hashes_match_local': True,
        'synthetic_output': synthetic,
        'external_output': external,
        'private_fixtures': [
            {'kind': 'texture' if i == 1 else 'bres',
             'bytes': source.stat().st_size, 'sha256': sha256(source)}
            for i, (source, _) in enumerate(local) if i
        ],
        'exit_code': 0,
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(synthetic)
    if external:
        print(external)
    print(f'Android runtime report: {args.report}')


if __name__ == '__main__':
    main()
