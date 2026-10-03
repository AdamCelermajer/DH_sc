#!/usr/bin/env python3
"""Build the source renderer APK with the installed Android SDK and NDK."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import zipfile

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent
SOURCES = [
    HERE / 'native.cpp',
    HERE / 'world_renderer.cpp',
    HERE / 'gameplay.cpp',
    HERE / 'scene_buffers.cpp',
    HERE / 'scripts.cpp',
    REPO / 'port/skin-payloads/skin.cpp',
    REPO / 'port/animation-pose/pose.cpp',
    REPO / 'port/animation-values/values.cpp',
    REPO / 'port/animation-timeline/timeline.cpp',
    REPO / 'port/animation-mixing/mixing.cpp',
    REPO / 'port/animation-layers/layers.cpp',
    REPO / 'port/scene-draw/draw.cpp',
    REPO / 'port/scene-payloads/scene.cpp',
    REPO / 'port/asset-payloads/payloads.cpp',
    REPO / 'port/engine-resources/resources.cpp',
    REPO / 'port/engine-math/math.cpp',
    REPO / 'port/material-bindings/bindings.cpp',
    REPO / 'port/texture-assets/texture.cpp',
    REPO / 'port/texture-assets/decode.cpp',
]


def run(*argv: object) -> None:
    subprocess.run([str(a) for a in argv], check=True)


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def check_elf(path: Path, expected_machine: int) -> dict:
    data = path.read_bytes()
    if data[:6] != b'\x7fELF\x02\x01':
        raise ValueError(f'{path}: expected little-endian ELF64')
    kind, machine = struct.unpack_from('<HH', data, 16)
    if kind != 3 or machine != expected_machine:
        raise ValueError(f'{path}: unexpected ELF type/machine {(kind, machine)}')
    phoff = struct.unpack_from('<Q', data, 32)[0]
    phsize, phcount = struct.unpack_from('<HH', data, 54)
    segments = []
    for i in range(phcount):
        ptype, _, offset, vaddr, _, filesz, _, align = struct.unpack_from(
            '<IIQQQQQQ', data, phoff + i * phsize)
        if ptype == 1:
            if align < 16384 or offset % 16384 != vaddr % 16384:
                raise ValueError(f'{path}: PT_LOAD {i} is not 16 KiB aligned')
            segments.append({'alignment': align, 'offset': offset})
    if not segments:
        raise ValueError(f'{path}: no PT_LOAD segments')
    return {'machine': machine, 'sha256': sha(path), 'bytes': len(data), 'pt_load': segments}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk', required=True, type=Path)
    parser.add_argument('--ndk', required=True, type=Path)
    parser.add_argument('--cache', type=Path, default=REPO.parent / 'cache/files',
                        help='Unpacked original cache; only encounter assets are bundled')
    args = parser.parse_args()
    sdk, ndk = args.sdk.resolve(), args.ndk.resolve()
    build = HERE / 'build'
    classes, dex, lib = build / 'classes', build / 'dex', build / 'lib'
    for path in (classes, dex, lib):
        path.mkdir(parents=True, exist_ok=True)
    cache = args.cache.resolve()
    encounter_paths = {
        'room.bdae': 'data/3d/modules/void_maze/void_maze.bdae',
        'floor.tga': 'data/3d/textures/env_voidmaze.tga',
        'hero.bdae': 'data/3d/characters/prince/prince_low_poly_warrior.bdae',
        'hero.tga': 'data/3d/textures/prince-warrior.tga',
        'walk.bdae': 'data/3d/characters/prince/animations/prince_walk_1hand.bdae',
        'idle.bdae': 'data/3d/characters/prince/animations/prince_idle_shield.bdae',
        'attack.bdae': 'data/3d/characters/prince/animations/prince_1hand_combo_01.bdae',
        'properties.bin': 'data/pydata/character_properties_pyarray.bin',
        'classes.bin': 'data/pydata/character_classes_pyarray.bin',
        'loot.bin': 'data/pydata/loot_table_pyarray.bin',
        'powers.bin': 'data/pydata/item_powers_pyarray.bin',
        'quests.bin': 'data/pydata/v2quests_pyarray.bin',
    }
    encounter_assets = {}
    encounter_manifest = {}
    for name, relative in encounter_paths.items():
        source = cache / relative
        if not source.is_file():
            raise FileNotFoundError(f'Supply --cache with the unpacked original cache: missing {relative}')
        encounter_assets['assets/dh2/encounter/' + name] = source
        encounter_manifest[name] = {'source': relative, 'sha256': sha(source),
                                    'bytes': source.stat().st_size}
    constant_sources = [cache / 'data/pydata' / name for name in
                        ('ai_pycst.bin', 'design_pycst.bin', 'v2quests_pycst.bin')]
    constant_payloads = [path.read_bytes() for path in constant_sources]
    if any(len(data) < 4 for data in constant_payloads):
        raise ValueError('Original encounter constants are incomplete')
    group_count = sum(struct.unpack_from('<I', data)[0] for data in constant_payloads)
    merged = build / 'encounter-constants.bin'
    merged.write_bytes(struct.pack('<I', group_count) + b''.join(data[4:] for data in constant_payloads))
    encounter_assets['assets/dh2/encounter/constants.bin'] = merged
    encounter_manifest['constants.bin'] = {'sha256': sha(merged), 'bytes': merged.stat().st_size,
        'derivation': 'sum little-endian group counts, concatenate complete original group payloads',
        'sources': [{'source': path.relative_to(cache).as_posix(), 'sha256': sha(path)}
                    for path in constant_sources]}
    bundle_manifest = build / 'encounter-assets.json'
    bundle_manifest.write_text(json.dumps(encounter_manifest, indent=2) + '\n')
    encounter_assets['assets/dh2/encounter/manifest.json'] = bundle_manifest
    jar = sdk / 'platforms/android-37.2/android.jar'
    if not jar.is_file():
        jar = sdk / 'platforms/android-37.0/android.jar'
    tools = sdk / 'build-tools/35.0.0'
    clang = ndk / 'toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe'
    java_home = Path(os.environ.get('JAVA_HOME', 'C:/Program Files/Java/jdk-23'))
    javac = java_home / 'bin/javac.exe'
    keytool = java_home / 'bin/keytool.exe'
    if not javac.is_file() or not keytool.is_file():
        javac, keytool = shutil.which('javac'), shutil.which('keytool')
    if not javac or not keytool:
        raise FileNotFoundError('JDK javac/keytool required; set JAVA_HOME')
    result = {}
    lua_root = REPO / 'port/lua-runtime'
    run(sys.executable, lua_root / 'build.py', '--ndk', ndk,
        '--report', build / 'lua-build-validation.json')
    manifest = json.loads((REPO / 'recovered/scripts/manifest.json').read_text())
    known = {row['path']: row for row in manifest['files']}
    script_assets = {}
    for asset, source in [('ai-commons.lua', 'ai/_commons.luac'),
                          ('skills-commons.lua', 'skills/_commons.luac'),
                          ('combat-formulas.lua', 'level/combat_formulas.luac')]:
        path = REPO / 'recovered/scripts/original/data/scripts' / source
        row = known[path.relative_to(REPO).as_posix()]
        if sha(path) != row['sha256']: raise ValueError('Changed original script: ' + source)
        script_assets['assets/dh2/scripts/' + asset] = path
    for abi, target, machine in (
        ('arm64-v8a', 'aarch64-linux-android35', 183),
        ('x86_64', 'x86_64-linux-android35', 62),
    ):
        directory = lib / abi
        directory.mkdir(exist_ok=True)
        output = directory / 'libdh2source.so'
        lua_library = directory / 'libdh2lua.so'
        variant = 'arm64' if abi == 'arm64-v8a' else 'x86_64'
        shutil.copyfile(lua_root / ('build/lua-' + variant + '.so'), lua_library)
        run(clang, f'--target={target}', '-std=c++17', '-O2', '-Wall', '-Wextra',
            '-Werror', '-fPIC', '-shared', '-fno-exceptions', '-fno-rtti',
            '-fno-fast-math', '-ffp-contract=off',
            '-nostdlib++', '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
            *SOURCES, '-L', directory, '-ldh2lua', '-llog', '-lGLESv2', '-lEGL', '-landroid', '-o', output)
        result[abi] = check_elf(output, machine)
        result[abi]['lua_library'] = check_elf(lua_library, machine)
    java_sources = sorted((HERE / 'src/local/dh2/sourceviewer').glob('*.java'))
    run(javac, '-Xlint:-options', '-source', '8', '-target', '8', '-classpath', jar,
        '-d', classes, *java_sources)
    for source in java_sources:
        if (classes / 'local/dh2/sourceviewer' / (source.stem + '.class')).stat().st_mtime_ns < source.stat().st_mtime_ns:
            raise RuntimeError('javac did not update ' + source.stem + '.class')
    run(tools / 'd8.bat', '--min-api', '26', '--output', dex,
        *sorted((classes / 'local/dh2/sourceviewer').glob('*.class')))
    base = build / 'base.apk'
    run(tools / 'aapt2.exe', 'link', '--manifest', HERE / 'AndroidManifest.xml',
        '-I', jar, '--min-sdk-version', '26', '--target-sdk-version', '37',
        '-o', base)
    with zipfile.ZipFile(base, 'a') as apk:
        apk.write(dex / 'classes.dex', 'classes.dex', compress_type=zipfile.ZIP_DEFLATED)
        for abi in result:
            for name in ('libdh2source.so', 'libdh2lua.so'):
                apk.write(lib / abi / name, f'lib/{abi}/{name}', compress_type=zipfile.ZIP_STORED)
        for name, path in {**script_assets, **encounter_assets}.items():
            apk.write(path, name, compress_type=zipfile.ZIP_DEFLATED)
    aligned = build / 'aligned.apk'
    run(tools / 'zipalign.exe', '-f', '-P', '16', '4', base, aligned)
    key = build / 'debug.jks'
    if not key.is_file():
        run(keytool, '-genkeypair', '-keystore', key, '-storepass', 'android',
            '-keypass', 'android', '-alias', 'debug', '-keyalg', 'RSA', '-keysize',
            '2048', '-validity', '3650', '-dname', 'CN=DH2 Local Debug')
    signed = build / 'dh2-source-renderer-debug.apk'
    run(tools / 'apksigner.bat', 'sign', '--ks', key, '--ks-key-alias', 'debug',
        '--ks-pass', 'pass:android', '--key-pass', 'pass:android',
        '--out', signed, aligned)
    run(tools / 'apksigner.bat', 'verify', '--verbose', signed)
    run(tools / 'zipalign.exe', '-c', '-P', '16', '4', signed)
    report = {'scope': 'source-built authored development encounter and asset diagnostics; not the complete original game',
              'target_sdk': 37, 'min_sdk': 26, 'abi': result,
              'apk': {'sha256': sha(signed), 'bytes': signed.stat().st_size},
              'lua_build_sha256': sha(build / 'lua-build-validation.json'),
              'lua_build': json.loads((build / 'lua-build-validation.json').read_text()),
              'script_assets': {name: {'sha256': sha(path), 'bytes': path.stat().st_size}
                                for name, path in script_assets.items()},
              'java_source_sha256': {p.relative_to(HERE).as_posix(): sha(p)
                                     for p in java_sources},
              'encounter_assets': encounter_manifest,
              'source_sha256': {str(p.relative_to(REPO)).replace('\\', '/'): sha(p)
                                for p in SOURCES}}
    (HERE / 'build-validation.json').write_text(json.dumps(report, indent=2) + '\n')
    print(signed)


if __name__ == '__main__':
    main()
