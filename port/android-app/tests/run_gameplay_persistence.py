#!/usr/bin/env python3
"""Compile and run the authored encounter persistence tests without an APK/UI."""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import time

HERE = Path(__file__).resolve().parent
REPO = HERE.parents[2]

def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--ndk', type=Path, required=True)
    p.add_argument('--adb', type=Path, required=True)
    p.add_argument('--serial', required=True)
    p.add_argument('--cache', type=Path, required=True)
    p.add_argument('--stage', type=Path, default=HERE.parent/'build/persistence-tests')
    args = p.parse_args()
    stage = args.stage.resolve(); stage.mkdir(parents=True, exist_ok=True)
    clang = args.ndk.resolve()/'toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
    library = stage/'libdh2lua.so'
    shutil.copyfile(REPO/'port/lua-runtime/build/lua-x86_64.so', library)
    runner = stage/'persistence-runner'
    command = [str(clang), '--target=x86_64-linux-android35', '-std=c++17', '-O2',
        '-Wall', '-Wextra', '-Werror', '-fno-fast-math', '-ffp-contract=off',
        '-fno-exceptions', '-fno-rtti', '-nostdlib++', '-Wl,-z,max-page-size=16384',
        '-Wl,--no-undefined', str(HERE/'gameplay_persistence.cpp'),
        str(HERE.parent/'gameplay.cpp'), '-L', str(stage), '-ldh2lua', '-lm', '-o', str(runner)]
    subprocess.run(command, check=True)
    originals = ['character_properties_pyarray.bin', 'character_classes_pyarray.bin',
        'loot_table_pyarray.bin', 'item_powers_pyarray.bin', 'v2quests_pyarray.bin']
    files = []
    for i, name in enumerate(originals):
        target = stage/f'input-{i}.bin'
        shutil.copyfile(args.cache.resolve()/'data/pydata'/name, target); files.append(target)
    constants = [(args.cache.resolve()/'data/pydata'/name).read_bytes() for name in
        ('ai_pycst.bin', 'design_pycst.bin', 'v2quests_pycst.bin')]
    target = stage/'input-5.bin'
    target.write_bytes(struct.pack('<I', sum(struct.unpack_from('<I', b)[0] for b in constants)) +
        b''.join(b[4:] for b in constants)); files.append(target)
    target = stage/'input-6.lua'
    shutil.copyfile(REPO/'recovered/scripts/original/data/scripts/level/combat_formulas.luac', target); files.append(target)
    remote = '/data/local/tmp/dh2-persistence-source-tests'
    adb = [str(args.adb.resolve()), '-s', args.serial]
    subprocess.run([*adb, 'shell', 'mkdir', '-p', remote], check=True, capture_output=True)
    for file in [runner, library, *files]:
        subprocess.run([*adb, 'push', str(file), remote+'/'+file.name], check=True, capture_output=True)
    subprocess.run([*adb, 'shell', 'chmod', '700', remote+'/'+runner.name], check=True, capture_output=True)
    start = time.monotonic()
    output = subprocess.run([*adb, 'shell', 'env', 'LD_LIBRARY_PATH='+remote,
        remote+'/'+runner.name, *[remote+'/'+f.name for f in files]], check=True,
        capture_output=True, text=True).stdout.strip()
    report = {'scope': 'Authored encounter exact-state persistence with original gameplay data; '
        'standalone Android x86_64 runner, simplified floor predicate, no APK/UI/original saves.',
        'serial': args.serial, 'passed': True, 'seconds': time.monotonic()-start,
        'stdout': output, 'compile_command': command,
        'sha256': {f.name: hashlib.sha256(f.read_bytes()).hexdigest() for f in [runner, library, *files]}}
    (stage/'result.json').write_text(json.dumps(report, indent=2)+'\n')
    print(output)
    print(stage/'result.json')

if __name__ == '__main__': main()
