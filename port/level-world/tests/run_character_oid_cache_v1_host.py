#!/usr/bin/env python3
"""Build and run the bounded source Character OID cache regression."""
from __future__ import annotations

import os
from pathlib import Path
import shutil
import subprocess
import tempfile

TESTS = Path(__file__).resolve().parent
LEVEL_WORLD = TESTS.parent

compiler = os.environ.get("CXX") or shutil.which("clang++") or shutil.which("g++")
with tempfile.TemporaryDirectory(prefix="dh2-character-oid-cache-") as temporary:
    if compiler:
        executable = Path(temporary) / ("character-oid-cache.exe" if os.name == "nt"
                                        else "character-oid-cache")
        subprocess.run([
            compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
            "-fno-fast-math", "-ffp-contract=off",
            str(LEVEL_WORLD / "character_oid_cache_v1.cpp"),
            str(TESTS / "character_oid_cache_v1.cpp"), "-o", str(executable),
        ], check=True)
        subprocess.run([str(executable)], check=True)
    else:
        sdk_value = (os.environ.get("ANDROID_HOME") or os.environ.get("ANDROID_SDK_ROOT")
                     or str(Path(os.environ.get("LOCALAPPDATA", "")) / "Android" / "Sdk"))
        sdk = Path(sdk_value)
        ndks = sorted((sdk / "ndk").glob("*"), reverse=True)
        if not ndks:
            raise SystemExit("No host compiler or Android NDK found; set CXX")
        prebuilt = ndks[0] / "toolchains" / "llvm" / "prebuilt" / "windows-x86_64"
        clang = prebuilt / "bin" / "clang++.exe"
        adb = shutil.which("adb") or str(sdk / "platform-tools" / "adb.exe")
        if not clang.is_file() or not Path(adb).is_file():
            raise SystemExit("Android NDK clang++ or adb is unavailable")
        executable = Path(temporary) / "character-oid-cache"
        subprocess.run([
            str(clang), "--target=x86_64-linux-android24",
            f"--sysroot={prebuilt / 'sysroot'}", "-std=c++17", "-O2",
            "-Wall", "-Wextra", "-Werror", "-static-libstdc++",
            "-fno-fast-math", "-ffp-contract=off",
            str(LEVEL_WORLD / "character_oid_cache_v1.cpp"),
            str(TESTS / "character_oid_cache_v1.cpp"), "-o", str(executable),
        ], check=True)
        serial = os.environ.get("ANDROID_SERIAL")
        devices = subprocess.run([adb, "devices"], check=True, capture_output=True,
                                 text=True).stdout
        online = [line.split()[0] for line in devices.splitlines()[1:]
                  if len(line.split()) >= 2 and line.split()[1] == "device"]
        if not serial:
            if len(online) != 1:
                raise SystemExit("Set ANDROID_SERIAL or connect exactly one emulator")
            serial = online[0]
        remote = "/data/local/tmp/dh2-character-oid-cache-test"
        subprocess.run([adb, "-s", serial, "push", str(executable), remote], check=True)
        try:
            subprocess.run([adb, "-s", serial, "shell", "chmod", "700", remote], check=True)
            subprocess.run([adb, "-s", serial, "shell", remote], check=True)
        finally:
            subprocess.run([adb, "-s", serial, "shell", "rm", "-f", remote], check=False)
