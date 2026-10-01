#!/usr/bin/env python3
"""Build a freestanding ARM64 probe; no Android framework or engine integration."""
import subprocess
import sys
from pathlib import Path

root = Path(__file__).resolve().parent
(root / 'build').mkdir(exist_ok=True)
subprocess.run([
    sys.executable, '-m', 'ziglang', 'cc', '-target', 'aarch64-linux-musl',
    '-nostdlib', '-shared', '-Wl,-soname,libdh2_port_prototype.so',
    '-Wl,-z,max-page-size=16384', '-Wl,-z,common-page-size=16384', '-Wl,--build-id=sha1',
    str(root / 'ported_arm64.S'), '-o', str(root / 'build/libdh2_port_prototype.so')
], check=True)
print('Built freestanding AArch64 ELF probe; this is not libDungeonHunter2.so.')
