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
import zipfile

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent
SOURCES = [
    HERE / 'native.cpp',
    HERE / 'scene_buffers.cpp',
    REPO / 'port/skin-payloads/skin.cpp',
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
    args = parser.parse_args()
    sdk, ndk = args.sdk.resolve(), args.ndk.resolve()
    build = HERE / 'build'
    classes, dex, lib = build / 'classes', build / 'dex', build / 'lib'
    for path in (classes, dex, lib):
        path.mkdir(parents=True, exist_ok=True)
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
    for abi, target, machine in (
        ('arm64-v8a', 'aarch64-linux-android35', 183),
        ('x86_64', 'x86_64-linux-android35', 62),
    ):
        directory = lib / abi
        directory.mkdir(exist_ok=True)
        output = directory / 'libdh2source.so'
        run(clang, f'--target={target}', '-std=c++17', '-O2', '-Wall', '-Wextra',
            '-Werror', '-fPIC', '-shared', '-fno-exceptions', '-fno-rtti',
            '-fno-fast-math', '-ffp-contract=off',
            '-nostdlib++', '-Wl,-z,max-page-size=16384', '-Wl,--no-undefined',
            *SOURCES, '-llog', '-lGLESv2', '-landroid', '-o', output)
        result[abi] = check_elf(output, machine)
    java_source = HERE / 'src/local/dh2/sourceviewer/MainActivity.java'
    run(javac, '-Xlint:-options', '-source', '8', '-target', '8', '-classpath', jar,
        '-d', classes, java_source)
    if (classes / 'local/dh2/sourceviewer/MainActivity.class').stat().st_mtime_ns < java_source.stat().st_mtime_ns:
        raise RuntimeError('javac did not update MainActivity.class')
    run(tools / 'd8.bat', '--min-api', '26', '--output', dex,
        classes / 'local/dh2/sourceviewer/MainActivity.class',
        classes / 'local/dh2/sourceviewer/MainActivity$1.class')
    base = build / 'base.apk'
    run(tools / 'aapt2.exe', 'link', '--manifest', HERE / 'AndroidManifest.xml',
        '-I', jar, '--min-sdk-version', '26', '--target-sdk-version', '37',
        '-o', base)
    with zipfile.ZipFile(base, 'a') as apk:
        apk.write(dex / 'classes.dex', 'classes.dex', compress_type=zipfile.ZIP_DEFLATED)
        for abi in result:
            item = lib / abi / 'libdh2source.so'
            apk.write(item, f'lib/{abi}/libdh2source.so', compress_type=zipfile.ZIP_STORED)
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
    report = {'scope': 'source-based Android asset renderer; not a playable game',
              'target_sdk': 37, 'min_sdk': 26, 'abi': result,
              'apk': {'sha256': sha(signed), 'bytes': signed.stat().st_size},
              'source_sha256': {str(p.relative_to(REPO)).replace('\\', '/'): sha(p)
                                for p in SOURCES}}
    (HERE / 'build-validation.json').write_text(json.dumps(report, indent=2) + '\n')
    print(signed)


if __name__ == '__main__':
    main()
