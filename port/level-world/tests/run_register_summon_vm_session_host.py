#!/usr/bin/env python3
"""Build and execute the RegisterSummon operation through its Lua VM session."""
from __future__ import annotations

import os
from pathlib import Path
import shutil
import subprocess
import tempfile

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
RUNTIME = ROOT / "port/adam-script-runtime"
CORE = "lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib".split()


def main() -> None:
    cxx = os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    android = False
    target_flags: list[str] = []
    adb = None
    if not cxx:
        sdk = Path(os.environ.get("ANDROID_HOME") or os.environ.get("ANDROID_SDK_ROOT") or
                   Path(os.environ.get("LOCALAPPDATA", "")) / "Android" / "Sdk")
        ndks = sorted((sdk / "ndk").glob("*"), reverse=True)
        if not ndks:
            raise SystemExit("No host C++ compiler or Android NDK found")
        prebuilt = ndks[0] / "toolchains/llvm/prebuilt/windows-x86_64"
        cxx = str(prebuilt / "bin/clang++.exe")
        cc = str(prebuilt / "bin/clang.exe")
        adb = shutil.which("adb") or str(sdk / "platform-tools/adb.exe")
        target_flags = ["--target=x86_64-linux-android24", f"--sysroot={prebuilt / 'sysroot'}"]
        android = True
    else:
        cc = os.environ.get("CC") or shutil.which("gcc") or shutil.which("clang")
        if not cc:
            raise SystemExit("No C compiler found; set CC")

    c_sources = [RUNTIME / "lua" / f"{name}.c" for name in CORE] + [
        RUNTIME / "script_runtime.c", ROOT / "port/lua-numeric/numeric.c"]
    cpp_sources = [RUNTIME / "script_function_alias.cpp", MODULE / "ais_native_bindings.cpp",
                   MODULE / "ais_external_init_callbacks.cpp", MODULE / "ais_state_callbacks.cpp",
                   MODULE / "lua_script_load_once.cpp", MODULE / "character_oid_cache_v1.cpp",
                   MODULE / "monster_external_script_session.cpp",
                   MODULE / "tests/register_summon_vm_session.cpp"]
    with tempfile.TemporaryDirectory(prefix="dh2-register-summon-vm-") as temporary:
        temp = Path(temporary)
        objects = []
        for index, source in enumerate(c_sources + cpp_sources):
            obj = temp / f"{index:02d}-{source.stem}.o"
            command = [cxx if source.suffix == ".cpp" else cc, *target_flags,
                       "-std=c++17" if source.suffix == ".cpp" else "-std=c99",
                       "-O1", "-fno-fast-math", "-ffp-contract=off", "-I", str(RUNTIME / "lua"),
                       "-Wall"]
            if source in cpp_sources:
                command += ["-Wextra", "-Werror", "-pedantic"]
            subprocess.run([*command, "-c", str(source), "-o", str(obj)], cwd=ROOT, check=True)
            objects.append(obj)
        executable = temp / "register-summon-vm"
        link = [cxx, *target_flags, *(str(obj) for obj in objects), "-lm"]
        if android:
            link.append("-static-libstdc++")
        subprocess.run([*link, "-o", str(executable)], cwd=ROOT, check=True)
        if not android:
            subprocess.run([str(executable)], cwd=ROOT, check=True)
            return
        serial = os.environ.get("ANDROID_SERIAL") or "emulator-5554"
        remote = "/data/local/tmp/dh2-register-summon-vm"
        subprocess.run([adb, "-s", serial, "push", str(executable), remote], check=True,
                       capture_output=True, text=True)
        try:
            subprocess.run([adb, "-s", serial, "shell", "chmod", "700", remote], check=True,
                           capture_output=True, text=True)
            result = subprocess.run([adb, "-s", serial, "shell", remote], check=False,
                                    capture_output=True, text=True)
            print(result.stdout, end="")
            if result.returncode:
                raise SystemExit(result.stderr or f"emulator returned {result.returncode}")
        finally:
            subprocess.run([adb, "-s", serial, "shell", "rm", "-f", remote], check=False,
                           capture_output=True, text=True)


if __name__ == "__main__":
    main()
