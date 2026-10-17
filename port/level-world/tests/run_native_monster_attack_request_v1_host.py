import json
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[3]
SOURCE = Path(__file__).with_name("native_monster_attack_request_v1.cpp")

compiler = os.environ.get("CXX") or shutil.which("g++") or shutil.which("clang++")
if not compiler:
    raise SystemExit("g++ or clang++ is required")
output = ROOT / "outputs" / "native-monster-attack-request-v1-host.exe"
output.parent.mkdir(parents=True, exist_ok=True)
build = subprocess.run([compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
                       str(SOURCE), "-o", str(output)], cwd=ROOT, capture_output=True, text=True)
if build.returncode:
    raise SystemExit(build.stderr)
run = subprocess.run([str(output)], cwd=ROOT, capture_output=True, text=True)
if run.returncode:
    raise SystemExit(run.stderr or run.stdout)
report = json.loads(run.stdout)
assert report == {"native_monster_attack_request_cases": 19,
                  "canonical_damage_deferred_to_animation_event": True,
                  "active_gated_source_vm_required": True,
                  "status": "PASS"}, report
print(run.stdout, end="")
