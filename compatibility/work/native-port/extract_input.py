#!/usr/bin/env python3
"""Extract the exact engine used by this porting probe from the supplied APK."""
import argparse
import hashlib
import zipfile
from pathlib import Path

APK_SHA256 = '32c2d027b585a42547311cd95da6a3975fdb3174e663e513d42a7f49d1a4c200'
LIB_SHA256 = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'

parser = argparse.ArgumentParser()
parser.add_argument('apk', type=Path)
parser.add_argument('--output', type=Path, default=Path('input/libDungeonHunter2.so'))
args = parser.parse_args()
if hashlib.sha256(args.apk.read_bytes()).hexdigest() != APK_SHA256:
    raise SystemExit('APK SHA-256 differs from the exact input audited for this prototype.')
with zipfile.ZipFile(args.apk) as apk:
    data = apk.read('lib/armeabi-v7a/libDungeonHunter2.so')
if hashlib.sha256(data).hexdigest() != LIB_SHA256:
    raise SystemExit('Engine SHA-256 mismatch.')
if args.output.exists():
    raise SystemExit('Output already exists; choose a new output path.')
args.output.parent.mkdir(parents=True, exist_ok=True)
args.output.write_bytes(data)
print('Extracted verified ARM32 input:', args.output)
