"""Replay the native owned stack against preserved original-derived fixtures."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import subprocess

ROOT = Path(__file__).resolve().parents[3]
SOURCES = [
    "port/engine-ui/menu_stack_v1.hpp",
    "port/engine-ui/menu_stack_v1.cpp",
    "port/engine-ui/menu_stack_owner_v1.hpp",
    "port/engine-ui/menu_stack_owner_v1.cpp",
    "port/engine-ui/tests/menu_stack_v1.cpp",
    "port/engine-ui/tests/menu_stack_v1_differential.py",
    "port/engine-ui/tools/run_menu_stack_v1_host.py",
]

def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()

def repo_path(path):
    p = Path(path)
    return p if p.is_absolute() else ROOT / p

def wsl_path(path):
    p = str(Path(path).resolve()).replace("\\", "/")
    if len(p) < 3 or p[1:3] != ":/":
        raise ValueError("expected an absolute Windows host path")
    return "/mnt/" + p[0].lower() + p[2:]

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--executable", required=True)
    ap.add_argument("--gold", default="port/engine-ui/reference/menu-stack-v1/stack-fixtures.bin")
    ap.add_argument("--output", required=True)
    args = ap.parse_args()
    exe, gold, out = map(repo_path, (args.executable, args.gold, args.output))
    before = {p: digest(ROOT / p) for p in SOURCES}
    binaries = {"executable_sha256": digest(exe), "gold_sha256": digest(gold)}
    if os.name == "nt":
        command = ["wsl.exe", "-e", "env", "ASAN_OPTIONS=detect_leaks=1", wsl_path(exe), wsl_path(gold)]
    else:
        command = ["env", "ASAN_OPTIONS=detect_leaks=1", str(exe), str(gold)]
    run = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    after = {p: digest(ROOT / p) for p in SOURCES}
    if before != after or binaries != {"executable_sha256": digest(exe), "gold_sha256": digest(gold)}:
        raise RuntimeError("source or binary changed during replay")
    if run.returncode or run.stderr:
        raise RuntimeError(f"host audit failed ({run.returncode}): {run.stdout}\n{run.stderr}")
    audit = json.loads(run.stdout)
    expected = {"validation": "PASS", "gold_cases": 4800, "ordered_services": 27099,
                "failure_prefixes": 27099, "owned_nested_checks": 10, "guards": 6,
                "sanitizer_findings": 0}
    if audit != expected:
        raise RuntimeError(f"unexpected host result: {audit}")
    original = ROOT / ".local-inputs/libDungeonHunter2.so"
    arm_report = ROOT / "port/engine-ui/reports/menu-stack-v1-arm64-differential.json"
    arm = json.loads(arm_report.read_text())
    if arm["validation"] != "PASS" or arm["comparisons"] != 4800 or arm["mismatches"]:
        raise RuntimeError("original/O2 proof is not complete")
    for p, h in arm["source_sha256"].items():
        if digest(ROOT / p) != h:
            raise RuntimeError(f"ARM64 source proof is stale: {p}")
    if arm["gold_sha256"] != binaries["gold_sha256"] or arm["original_sha256"] != digest(original):
        raise RuntimeError("original/gold proof binding mismatch")
    build = "g++ -std=c++17 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off port/engine-ui/menu_stack_v1.cpp port/engine-ui/menu_stack_owner_v1.cpp port/engine-ui/tests/menu_stack_v1.cpp -o .local-inputs/ingame-menu-stack-v1/menu_stack_audit"
    report = {
        "validation": "PASS", "host_audit": audit,
        "sanitizers": {"address": "PASS", "undefined": "PASS", "leak": "PASS", "findings": 0},
        "source_sha256": before, "original_sha256": digest(original), **binaries,
        "executable": str(exe.relative_to(ROOT)).replace("\\", "/"),
        "gold": str(gold.relative_to(ROOT)).replace("\\", "/"),
        "original_arm64_report": str(arm_report.relative_to(ROOT)).replace("\\", "/"),
        "original_arm64_report_sha256": digest(arm_report),
        "build_command": build, "run_command": command, "return_code": run.returncode,
        "stdout": run.stdout, "stderr": run.stderr,
        "scope": "Owned native64 projection and original-derived ordered service replay; required SWF/MenuBase/world/touch/AS services are controlled fixtures. No packaged or live menu parity claim.",
    }
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "report": str(out), "report_sha256": digest(out), **audit}))

if __name__ == "__main__":
    main()
