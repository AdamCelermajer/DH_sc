"""Focused host checks only; no APK/JVM, emulator, game-data or save writes."""
import json
import os
from pathlib import Path
import shlex
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / ".local-inputs/campaign-cancellation-v135"
OUT.mkdir(parents=True, exist_ok=True)


def host_path(path):
    path = str(path.resolve()).replace("\\", "/")
    return "/mnt/" + path[0].lower() + path[2:] if os.name == "nt" else path


sources = [
    "port/android-native/tests/source_campaign_cancel_unload_v135.cpp",
    "port/level-loader/lifecycle_v36.cpp",
    "port/level-loader/canonical_level_context_v1.cpp",
    "port/level-loader/level_constructor_v3.cpp",
    "port/level-world/event_manager_owner_v12.cpp",
    "port/level-world/player_manager_owner_v1.cpp",
    "port/level-world/player_level_quicksave_v29.cpp",
    "port/level-world/object_manager_language_registry_v1.cpp",
]
includes = ["-I" + host_path(path) for path in (ROOT / "port").iterdir() if path.is_dir()]
includes += ["-I" + host_path(ROOT / "port/physics-backend/box2d-2.0.1/Include")]
binary = OUT / "campaign-cancellation-v135"
prefix = ["wsl.exe", "--exec"] if os.name == "nt" else []
compile_command = prefix + [
    "/usr/bin/c++", "-std=c++17", "-O0", "-g1", "-Wall", "-Wextra", "-Werror",
    "-Wno-misleading-indentation", "-ffunction-sections", "-fdata-sections",
] + includes + [host_path(ROOT / source) for source in sources] + [
    "-Wl,--gc-sections", "-pthread", "-o", host_path(binary),
]
rows = []
commands = [("compile", compile_command), ("run", prefix + ["/usr/bin/timeout", "20s", host_path(binary)])]
if "--production-syntax" in sys.argv:
    build = ROOT / ".local-inputs/native-host-v128/build"
    for filename in ("source_campaign_release_v88.cpp", "source_campaign_runtime_v61.cpp"):
        target = "native/CMakeFiles/dh2_native.dir/" + filename + ".o"
        generated = subprocess.check_output(prefix + ["/usr/bin/ninja", "-C", host_path(build), "-t", "commands", target], text=True).splitlines()[-1]
        compiler = shlex.split(generated)
        compiler = compiler[:compiler.index("-MD")]
        commands.append(("syntax-" + filename, prefix + compiler + ["-fsyntax-only", host_path(ROOT / "port/android-native/app/src/main/cpp" / filename)]))
for name, command in commands:
    result = subprocess.run(command, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, timeout=120)
    (OUT / (name + ".log")).write_text(result.stdout, encoding="utf-8")
    rows.append({"name": name, "command": command, "exit_code": result.returncode, "output": result.stdout})
    (OUT / "receipt.json").write_text(json.dumps({"scope": "Focused current-source Level C1/Stage0/dispatcher/Unload/GS and admission; resource/file/script providers are explicit fixtures; no emulator", "runs": rows}, indent=2), encoding="utf-8")
    print(name, result.returncode, "log:", OUT / (name + ".log"))
    if name == "run" or result.returncode:
        print(result.stdout[-8000:], end="\n")
    if result.returncode:
        raise SystemExit(result.returncode)
