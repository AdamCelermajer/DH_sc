#!/usr/bin/env python3
"""Compile and run the source ObjectManager factory-map check."""
from __future__ import annotations

import os
from pathlib import Path
import shutil
import subprocess
import tempfile

TESTS = Path(__file__).resolve().parent
PORT = TESTS.parents[1]


def compiler() -> str:
    configured = os.environ.get("CXX")
    if configured:
        return configured
    for candidate in ("clang++", "g++"):
        found = shutil.which(candidate)
        if found:
            return found
    sdk_value = os.environ.get("ANDROID_HOME") or os.environ.get("ANDROID_SDK_ROOT")
    if not sdk_value and os.environ.get("LOCALAPPDATA"):
        sdk_value = str(Path(os.environ["LOCALAPPDATA"]) / "Android" / "Sdk")
    sdk = Path(sdk_value or "")
    ndk = sdk / "ndk"
    if ndk.is_dir():
        versions = sorted((item for item in ndk.iterdir() if item.is_dir()), reverse=True)
        for version in versions:
            found = version / "toolchains" / "llvm" / "prebuilt" / "windows-x86_64" / "bin" / "clang++.exe"
            if found.is_file():
                return str(found)
    raise RuntimeError("No C++17 compiler found; set CXX or install an Android NDK")


def sdk_path() -> Path:
    value = os.environ.get("ANDROID_HOME") or os.environ.get("ANDROID_SDK_ROOT")
    if not value and os.environ.get("LOCALAPPDATA"):
        value = str(Path(os.environ["LOCALAPPDATA"]) / "Android" / "Sdk")
    return Path(value or "")


with tempfile.TemporaryDirectory(prefix="dh2-object-creation-map-") as temporary:
    temporary = Path(temporary)
    source = PORT / "level-world" / "object_creation_map_v1.cpp"
    test = TESTS / "object_creation_map_v1.cpp"
    host_compiler = next((shutil.which(name) for name in ("clang++", "g++")
                          if shutil.which(name)), None)
    if os.environ.get("CXX") or host_compiler:
        executable = temporary / ("object-creation-map.exe" if os.name == "nt" else "object-creation-map")
        command = [compiler(), "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
                   str(source), str(test), "-o", str(executable)]
        subprocess.run(command, check=True)
        subprocess.run([str(executable)], check=True)
    else:
        sdk = sdk_path()
        ndk = sdk / "ndk"
        versions = sorted((item for item in ndk.iterdir() if item.is_dir()), reverse=True) if ndk.is_dir() else []
        if not versions:
            raise RuntimeError("No host compiler or Android NDK found")
        prebuilt = versions[0] / "toolchains" / "llvm" / "prebuilt" / "windows-x86_64"
        clang = prebuilt / "bin" / "clang++.exe"
        adb = shutil.which("adb") or str(sdk / "platform-tools" / "adb.exe")
        if not clang.is_file() or not Path(adb).is_file():
            raise RuntimeError("Android NDK clang++ or adb is unavailable")
        executable = temporary / "object-creation-map"
        command = [str(clang), "--target=x86_64-linux-android24",
                   f"--sysroot={prebuilt / 'sysroot'}", "-std=c++17", "-O2",
                   "-Wall", "-Wextra", "-Werror", "-static-libstdc++",
                   str(source), str(test), "-o", str(executable)]
        subprocess.run(command, check=True)
        serial = os.environ.get("ANDROID_SERIAL")
        devices = subprocess.run([adb, "devices"], check=True, capture_output=True, text=True).stdout
        online = [line.split()[0] for line in devices.splitlines()[1:]
                  if len(line.split()) >= 2 and line.split()[1] == "device"]
        if not serial:
            if len(online) != 1:
                raise RuntimeError("Set ANDROID_SERIAL or connect exactly one emulator to run this focused check")
            serial = online[0]
        remote = "/data/local/tmp/dh2-object-creation-map-test"
        subprocess.run([adb, "-s", serial, "push", str(executable), remote], check=True)
        try:
            subprocess.run([adb, "-s", serial, "shell", "chmod", "700", remote], check=True)
            subprocess.run([adb, "-s", serial, "shell", remote], check=True)
        finally:
            subprocess.run([adb, "-s", serial, "shell", "rm", "-f", remote], check=False)
