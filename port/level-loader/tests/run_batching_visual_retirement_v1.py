"""Compile/run the actual Stage24 compiler fixture and check original IDA evidence.

Uses an existing g++ (WSL on Windows). No emulator, APK launch or dependency
installation. Resource primitives are fixture services; the original APK is
not executed. IDA checks skip explicitly when the private export is absent.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--output", type=Path, default=ROOT / ".local-inputs/stage24-visual-retirement-v1")
args = parser.parse_args()
out = args.output.resolve()
out.mkdir(parents=True, exist_ok=True)
if os.name == "nt":
    wsl = shutil.which("wsl.exe")
    if not wsl:
        raise SystemExit("Existing WSL g++ is required on Windows")

    def unix(path):
        path = str(path.resolve())
        return "/mnt/" + path[0].lower() + path[2:].replace("\\", "/")

    prefix = [wsl, "--cd", unix(ROOT), "--exec"]
    executable = unix(out / "stage24-visual-retirement-host")
else:
    prefix = []
    executable = str(out / "stage24-visual-retirement-host")

modules = ["level-loader", "level-world", "game-data", "engine-ui", "engine-math",
           "scene-materials", "asset-payloads", "engine-objects", "engine-animation",
           "script-runtime", "engine-effects", "engine-skinning"]
sources = ["port/level-loader/tests/level_batching_visual_retirement_v1.cpp",
           "port/level-loader/level_batching_source_v96.cpp"]
compile_args = ["g++", "-std=c++17", "-O2", "-g", "-Wall", "-Wextra", "-Werror",
                "-Wno-misleading-indentation", "-ffunction-sections", "-fdata-sections",
                "-fno-fast-math", "-ffp-contract=off"]
compile_args += ["-Iport/" + module for module in modules]
compile_args += sources + ["-Wl,--gc-sections", "-o", executable]
commands = [("compile", prefix + compile_args),
            ("original-ida-evidence", [sys.executable, "-B", str(Path(__file__).with_name("test_batching_visual_retirement_arm_v1.py"))]),
            ("current-host-compiler", prefix + [executable])]
receipt = {"scope": __doc__, "original_apk_executed": False, "cases": [],
           "source_sha256": {path: hashlib.sha256((ROOT / path).read_bytes()).hexdigest()
                             for path in sources + ["port/level-loader/level_batching_source_v96.hpp",
                                                    "port/level-loader/tests/test_batching_visual_retirement_arm_v1.py"]}}
for name, command in commands:
    result = subprocess.run(command, cwd=ROOT, text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, timeout=60)
    (out / (name + ".log")).write_text(result.stdout, encoding="utf-8")
    receipt["cases"].append({"name": name, "command": command,
                             "exit_code": result.returncode, "output": result.stdout})
    receipt["validation"] = "PASS" if result.returncode == 0 else "FAIL"
    (out / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    print(name + ": " + str(result.returncode), flush=True)
    print(result.stdout, end="", flush=True)
    if result.returncode:
        raise SystemExit(result.returncode)
