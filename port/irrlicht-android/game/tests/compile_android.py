#!/usr/bin/env python3
"""Compile the isolated SceneMesh adapter against pinned Irrlicht r6038."""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys


TEST_DIR = Path(__file__).resolve().parent
GAME_DIR = TEST_DIR.parent
IRRLICHT_ROOT = GAME_DIR.parent
REPO = IRRLICHT_ROOT.parents[1]
UPSTREAM = IRRLICHT_ROOT / "upstream"
MANIFEST = IRRLICHT_ROOT / "upstream-source-manifest.json"
SOURCE = GAME_DIR / "scene_mesh_adapter.cpp"
OUTPUT = GAME_DIR / "build-compile"
DEFAULT_NDK = REPO.parent / "emulator-test" / "sdk" / "ndk" / "29.0.14206865"

TARGETS = (
    ("arm64-v8a", "aarch64-linux-android26"),
    ("x86_64", "x86_64-linux-android26"),
)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def main() -> int:
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    canonical = json.dumps(manifest["files"], sort_keys=True,
                           separators=(",", ":")).encode()
    if hashlib.sha256(canonical).hexdigest() != manifest["tree_manifest_sha256"]:
        raise RuntimeError("pinned Irrlicht source manifest is invalid")
    header_record = next(record for record in manifest["files"]
                         if record["path"] == "include/irrlicht.h")
    if sha256(UPSTREAM / "include/irrlicht.h") != header_record["sha256"]:
        raise RuntimeError("Irrlicht include/irrlicht.h differs from pinned r6038")

    ndk = Path(os.environ.get("ANDROID_NDK_HOME") or
               os.environ.get("ANDROID_NDK_ROOT") or DEFAULT_NDK).resolve()
    host_tag = "windows-x86_64" if os.name == "nt" else "linux-x86_64"
    toolchain = ndk / "toolchains" / "llvm" / "prebuilt" / host_tag
    compiler = toolchain / "bin" / ("clang++.exe" if os.name == "nt" else "clang++")
    readelf = toolchain / "bin" / ("llvm-readelf.exe" if os.name == "nt"
                                    else "llvm-readelf")
    if not compiler.is_file():
        raise FileNotFoundError(f"NDK clang++ not found: {compiler}")
    if not readelf.is_file():
        raise FileNotFoundError(f"NDK llvm-readelf not found: {readelf}")
    if not SOURCE.is_file():
        raise FileNotFoundError(SOURCE)

    OUTPUT.mkdir(parents=True, exist_ok=True)
    records = []
    for abi, target in TARGETS:
        object_file = OUTPUT / f"scene_mesh_adapter-{abi}.o"
        linked_library = OUTPUT / f"libscene-mesh-adapter-{abi}.so"
        irrlicht_library = IRRLICHT_ROOT / "build" / "static" / abi / "libIrrlicht.a"
        if not irrlicht_library.is_file():
            raise FileNotFoundError(
                f"Irrlicht {abi} static library not found: {irrlicht_library}; "
                "run python port/irrlicht-android/build.py --no-apk first")
        command = [
            str(compiler), f"--target={target}", "-std=c++17", "-fPIC",
            "-fno-exceptions", "-Wall", "-Wextra", "-Werror",
            "-Wno-unused-parameter", f"--sysroot={toolchain / 'sysroot'}",
            "-I", str(UPSTREAM / "include"),
            "-I", str(REPO / "port" / "android-app"),
            "-c", str(SOURCE), "-o", str(object_file),
        ]
        print(f"[{abi}] compiling adapter against pinned Irrlicht headers")
        completed = subprocess.run(command, text=True, stdout=subprocess.PIPE,
                                   stderr=subprocess.STDOUT, check=False)
        if completed.stdout:
            print(completed.stdout, end="")
        if completed.returncode:
            return completed.returncode

        link_command = [
            str(compiler), f"--target={target}", "-std=c++17", "-fPIC",
            "-fno-exceptions", "-shared", "-Wl,--no-undefined",
            "-Wl,-z,max-page-size=16384", "-Wl,-z,common-page-size=16384",
            str(object_file), str(irrlicht_library), "-llog", "-lEGL",
            "-lGLESv1_CM", "-lGLESv2", "-lz", "-landroid",
            "-static-libstdc++", "-o", str(linked_library),
        ]
        print(f"[{abi}] linking against the r6038 Irrlicht static library")
        linked = subprocess.run(link_command, text=True, stdout=subprocess.PIPE,
                                stderr=subprocess.STDOUT, check=False)
        if linked.stdout:
            print(linked.stdout, end="")
        if linked.returncode:
            return linked.returncode

        headers = subprocess.run(
            [str(readelf), "--program-headers", "--wide", str(linked_library)],
            text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            check=False)
        if headers.returncode:
            print(headers.stdout, end="")
            return headers.returncode
        load_alignments = [int(match.group(1), 16) for line in headers.stdout.splitlines()
                           if (match := re.match(r"^\s*LOAD\s+.*\s+(0x[0-9a-fA-F]+)\s*$", line))]
        if not load_alignments or any(value < 16384 for value in load_alignments):
            raise RuntimeError(f"linked {abi} adapter lacks 16 KiB PT_LOAD alignment")
        records.append({
            "abi": abi,
            "target": target,
            "object": object_file.name,
            "bytes": object_file.stat().st_size,
            "sha256": sha256(object_file),
            "irrlicht_static_library_sha256": sha256(irrlicht_library),
            "linked_shared_library": linked_library.name,
            "linked_bytes": linked_library.stat().st_size,
            "linked_sha256": sha256(linked_library),
            "pt_load_alignments": load_alignments,
        })

    report = {
        "result": "compile and link pass",
        "upstream_svn_revision": manifest["svn_revision"],
        "tree_manifest_sha256": manifest["tree_manifest_sha256"],
        "compiler": str(compiler),
        "objects": records,
        "note": "This proves compile/link compatibility, not rendering or app integration.",
    }
    report_path = OUTPUT / "compile-report.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: {len(records)} Android ABI compile targets; report: {report_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, StopIteration, RuntimeError) as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        raise SystemExit(1)
