"""Build the bounded external monster session with the existing Adam Lua VM."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
RUNTIME = ROOT / "port/adam-script-runtime"
CORE = "lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib".split()


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", help="C++ compiler")
    parser.add_argument("--c-compiler", help="matching C compiler")
    parser.add_argument("--output", type=Path, default=MODULE / "build/monster-external-session/host")
    parser.add_argument("--report", type=Path, default=MODULE / "build/monster-external-session/validation.json")
    args = parser.parse_args()
    cxx = args.compiler or os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
    android = False
    android_sdk = None
    android_target_flags: list[str] = []
    adb = None
    if not cxx:
        sdk_value = os.environ.get("ANDROID_HOME") or os.environ.get("ANDROID_SDK_ROOT") or str(
            Path(os.environ.get("LOCALAPPDATA", "")) / "Android" / "Sdk")
        android_sdk = Path(sdk_value)
        ndks = sorted((android_sdk / "ndk").glob("*"), reverse=True)
        if not ndks:
            parser.error("pass --compiler, set CXX, or install Android NDK")
        prebuilt = ndks[0] / "toolchains" / "llvm" / "prebuilt" / "windows-x86_64"
        cxx = str(prebuilt / "bin" / "clang++.exe")
        cc = str(prebuilt / "bin" / "clang.exe")
        adb = shutil.which("adb") or str(android_sdk / "platform-tools" / "adb.exe")
        if not Path(cxx).is_file() or not Path(cc).is_file() or not Path(adb).is_file():
            parser.error("Android NDK compiler or adb unavailable")
        android_target_flags = ["--target=x86_64-linux-android24",
                                f"--sysroot={prebuilt / 'sysroot'}"]
        android = True
    else:
        cc = args.c_compiler or str(Path(cxx).with_name(Path(cxx).name.replace("g++", "gcc").replace("clang++", "clang")))
    output = args.output.resolve()
    if os.name == "nt" and not output.suffix and not android:
        output = output.with_suffix(".exe")
    output.parent.mkdir(parents=True, exist_ok=True)
    objects = output.parent / "objects"
    objects.mkdir(exist_ok=True)
    original = ROOT / "recovered/scripts/original/data/scripts/ai"
    commons, monster = original / "_commons.luac", original / "monster.luac"
    follower = original / "follower.luac"
    assert digest(commons) == "20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c"
    assert digest(monster) == "84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d"
    assert digest(follower) == "2d281c3bc784bbc12356e37cff9e7bdcd44a13e1d45da1fcb21069b4fef48d9d"
    c_sources = [RUNTIME / "lua" / (name + ".c") for name in CORE] + [
        RUNTIME / "script_runtime.c", ROOT / "port/lua-numeric/numeric.c"]
    cpp_sources = [RUNTIME / "script_function_alias.cpp", MODULE / "ais_native_bindings.cpp",
                   MODULE / "ais_external_init_callbacks.cpp",
                   MODULE / "lua_script_load_once.cpp",
                   MODULE / "ais_state_callbacks.cpp",
                   MODULE / "character_oid_cache_v1.cpp",
                   MODULE / "monster_external_script_session.cpp",
                   MODULE / "tests/monster_external_script_session.cpp"]
    compiled = []
    commands = []
    warnings = []
    for path in c_sources + cpp_sources:
        obj = objects / (path.parent.name + "-" + path.stem + ".o")
        own_cpp = path in cpp_sources[1:]
        command = [cxx if path.suffix == ".cpp" else cc, *android_target_flags,
                   "-std=c++17" if path.suffix == ".cpp" else "-std=c99", "-O1",
                   "-fno-fast-math", "-ffp-contract=off", "-I", str(RUNTIME / "lua"),
                   "-Wall", *( ["-Wextra", "-Werror", "-pedantic"] if own_cpp else []),
                   "-c", str(path), "-o", str(obj)]
        result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
        if result.returncode:
            sys.stderr.write(result.stderr)
            return result.returncode
        if result.stderr:
            warnings.append({"source": path.relative_to(ROOT).as_posix(), "diagnostics": result.stderr})
        commands.append(command)
        compiled.append(obj)
    command = [cxx, *android_target_flags, *(str(path) for path in compiled), "-lm",
               *( ["-static-libstdc++"] if android else [] ), "-o", str(output)]
    result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    commands.append(command)
    if result.returncode:
        sys.stderr.write(result.stderr)
        return result.returncode
    if android:
        serial = os.environ.get("ANDROID_SERIAL") or "emulator-5554"
        remote = "/data/local/tmp/dh2-monster-external-session-host"
        remote_commons = "/data/local/tmp/dh2-monster-external-session-commons.luac"
        remote_monster = "/data/local/tmp/dh2-monster-external-session-monster.luac"
        remote_follower = "/data/local/tmp/dh2-monster-external-session-follower.luac"
        subprocess.run([adb, "-s", serial, "push", str(output), remote], check=True,
                       capture_output=True, text=True)
        subprocess.run([adb, "-s", serial, "push", str(commons), remote_commons], check=True,
                       capture_output=True, text=True)
        subprocess.run([adb, "-s", serial, "push", str(monster), remote_monster], check=True,
                       capture_output=True, text=True)
        subprocess.run([adb, "-s", serial, "push", str(follower), remote_follower], check=True,
                       capture_output=True, text=True)
        try:
            subprocess.run([adb, "-s", serial, "shell", "chmod", "700", remote], check=True,
                           capture_output=True, text=True)
            result = subprocess.run([adb, "-s", serial, "shell", remote,
                                     remote_commons, remote_monster], cwd=ROOT,
                                    capture_output=True, text=True)
            follower_result = subprocess.run([adb, "-s", serial, "shell", remote,
                                              "--follower-only", remote_commons,
                                              remote_follower], cwd=ROOT,
                                             capture_output=True, text=True)
        finally:
            subprocess.run([adb, "-s", serial, "shell", "rm", "-f", remote,
                            remote_commons, remote_monster, remote_follower], check=False,
                           capture_output=True, text=True)
    else:
        result = subprocess.run([str(output), str(commons), str(monster)], cwd=ROOT,
                                capture_output=True, text=True)
        follower_result = subprocess.run([str(output), "--follower-only", str(commons),
                                          str(follower)], cwd=ROOT, capture_output=True,
                                         text=True)
    sys.stdout.write(result.stdout)
    sys.stderr.write(result.stderr)
    if result.returncode:
        return result.returncode
    sys.stdout.write(follower_result.stdout)
    sys.stderr.write(follower_result.stderr)
    if follower_result.returncode:
        return follower_result.returncode
    assert follower_result.stdout.strip() == "PASS: recovered follower AISExternal callbacks on one VM"
    host = json.loads(result.stdout)
    assert host["monster_external_session_cases"] == 41 and host["mismatches"] == 0, host
    for key in ("unchanged_original_scripts_executed", "spotted_callback_order", "idle_path_short_circuit",
                "fresh_target_after_path_query", "opaque_64bit_identity_tables", "service_lifetime_and_reentry",
                "failure_preserves_prior_effects", "unknown_callbacks_rejected"):
        assert host[key] is True, (key, host)
    for key in ("staged_same_vm_lifecycle", "staged_errors_stop_without_fallback"):
        assert host[key] is True, (key, host)
    for key in ("source_libraries_and_35_bindings", "unsupported_globals_fail_closed"):
        assert host[key] is True, (key, host)
    assert host["numeric_result_arity"] is True, host
    assert host["same_vm_post_final_callbacks"] is True and host["post_discarded_return_updates_final_alias"] is True
    assert host["death_callback_same_vm"] is True
    assert host["death_killer_identity_and_nil"] is True
    assert host["combat_callback_two_character_abi"] is True
    assert host["stop_attack_callbacks"] is True
    assert host["native_wired"] is False
    dependencies = c_sources + cpp_sources + [MODULE / "monster_external_script_session.hpp",
        ROOT / "port/android-native/app/src/main/cpp/player_gameplay_audio.hpp",
        MODULE / "ais_external_init_callbacks.hpp",
        MODULE / "ais_native_bindings.hpp", MODULE / "character_oid_cache_v1.hpp",
        ROOT / "port/adam-script-runtime/script_runtime.h",
        MODULE / "lua_script_load_once.hpp",
        MODULE / "ais_state_callbacks.hpp",
        RUNTIME / "script_runtime.h", RUNTIME / "script_function_alias.h", ROOT / "port/lua-numeric/numeric.h",
        Path(__file__).resolve()] + list((RUNTIME / "lua").glob("*.h"))
    evidence = {"validation": "PASS", "host_report": host, "compiler_commands": commands,
        "compiled_source_and_header_sha256": {p.relative_to(ROOT).as_posix(): digest(p) for p in dependencies},
        "unchanged_script_sha256": {p.relative_to(ROOT).as_posix(): digest(p) for p in (commons, monster, follower)},
        "executable_sha256": digest(output), "dependency_warnings": warnings,
        "native_wired": False, "whole_original_vm_parity": False,
        "scope": "Original monster callbacks plus the recovered generic follower AISExternal callback entrypoints on one VM; follower FriendSpotted/master-range dispatch preserves callback alias and object identity with same-session SetMaster/MoveTo/Stop/WarpBehind/ClearTarget providers; live app Record wiring remains pending"}
    report = args.report.resolve()
    report.parent.mkdir(parents=True, exist_ok=True)
    report.write_text(json.dumps(evidence, indent=2) + "\n", encoding="utf8")
    print("report:", report)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
