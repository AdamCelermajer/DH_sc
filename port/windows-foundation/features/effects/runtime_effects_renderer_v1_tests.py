from pathlib import Path
import hashlib
import json
import subprocess

ROOT = Path(__file__).resolve().parents[4]
FEATURE = ROOT / "port/windows-foundation/features/effects"
BUILD = ROOT / ".local-inputs/windows-effects-renderer-v1"
BUILD.mkdir(parents=True, exist_ok=True)
COMPILER = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
SOURCES = [
    FEATURE / "runtime_effects_renderer_v1_tests.cpp",
    FEATURE / "runtime_effects_renderer_v1.cpp",
    ROOT / "port/windows-foundation/renderer.cpp",
]
OUTPUT = BUILD / "runtime-effects-renderer-v1-tests.exe"


def require(value, message):
    if not value:
        raise SystemExit(f"FAIL {message}")


require(COMPILER.is_file(), "LLVM-MinGW compiler missing")
for source in SOURCES:
    require(source.is_file(), f"source file missing: {source}")

command = [str(COMPILER), "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
           "-Wno-missing-field-initializers", "-I", str(ROOT / "port/windows-foundation"),
           "-I", str(ROOT / "port/level-world"), "-I", str(ROOT / "port/scene-materials"),
           "-I", str(ROOT / "port/engine-resources"), "-I", str(ROOT / "port/game-data"),
           "-I", str(ROOT / "port/engine-animation"), "-I", str(ROOT / "port/engine-skinning"),
           "-I", str(ROOT / "port/engine-math"), "-I", str(ROOT / "port/engine-textures"),
           *map(str, SOURCES), "-static", "-lopengl32", "-luser32", "-lgdi32",
           "-o", str(OUTPUT)]
build = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
(BUILD / "build.log").write_text(build.stdout + build.stderr)
require(build.returncode == 0, build.stdout + build.stderr)

run = subprocess.run([str(OUTPUT)], cwd=ROOT, text=True, capture_output=True)
(BUILD / "run.log").write_text(run.stdout + run.stderr)
require(run.returncode == 0, run.stdout + run.stderr)
try:
    result = json.loads(run.stdout.strip().splitlines()[-1])
except Exception as error:
    raise SystemExit(f"FAIL CPU queue test emitted no JSON result: {error}\n{run.stdout}")
require(result.get("validation") == "PASS" and result.get("gpu_calls") == 0,
        f"renderer queue contract test failed: {result}")

report = {
    "feature": "RuntimeEffectsRendererV1",
    "validation": "PASS",
    "runner": "port/windows-foundation/features/effects/runtime_effects_renderer_v1_tests.py",
    "test": "port/windows-foundation/features/effects/runtime_effects_renderer_v1_tests.cpp",
    "source_result": result,
    "source_sha256": {str(path.relative_to(ROOT)).replace("\\", "/"):
                      hashlib.sha256(path.read_bytes()).hexdigest() for path in SOURCES},
    "claims": {
        "actual_root_renderer_callbacks_bound": True,
        "exact_shared_frames_retained_until_drain": True,
        "invalid_source_packets_rejected": True,
        "gpu_calls": False,
        "wgl_context_created": False,
        "draw_and_finish_and_drain_runtime_path_executed": False,
        "global_scene_manager_ordering": "caller inserts draw_queued at its established render slot",
    },
    "remaining_proof": "Requires a root-cleared WGL smoke to exercise draw_queued and finish_and_drain against the actual source packet.",
}
wgl_report_path = FEATURE / "runtime-effects-renderer-wgl-report.json"
if wgl_report_path.is_file():
    wgl_report = json.loads(wgl_report_path.read_text())
    if wgl_report.get("validation") == "PASS":
        report["claims"]["draw_and_finish_and_drain_runtime_path_executed"] = True
        report["wgl_integration_report"] = str(wgl_report_path.relative_to(ROOT)).replace("\\", "/")
        report.pop("remaining_proof")
(FEATURE / "runtime-effects-renderer-v1-report.json").write_text(json.dumps(report, indent=2) + "\n")
print(json.dumps(report, indent=2))
