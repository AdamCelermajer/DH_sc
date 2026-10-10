import json
import pathlib
import subprocess

root = pathlib.Path.cwd()
own = root / "port/windows-foundation/features/actor_frame"
build = own / "source_backend_script_host_build"
build.mkdir(exist_ok=True)
compiler = root / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
flags = ["-std=c++20", "-Dfinite=_finite", "-include", "exception", "-Wall", "-Wextra", "-Werror", "-Wno-unused-value"]
for path in (root / "port").iterdir():
    if path.is_dir():
        flags.append("-I" + str(path))
flags += [
    "-I" + str(root / "port/android-native/app/src/main/cpp"),
    "-I" + str(root / "port/engine-audio/integration-v42"),
    "-I" + str(root / ".local-inputs/runtime-test-20261009/windows-include"),
    "-isystem", str(root / "port/physics-backend/box2d-2.0.1/Include"),
]
results = {}
for source in (own / "source_backend_script_host_v1.cpp", own / "source_backend_script_host_contract_test.cpp"):
    output = build / (source.stem + ".o")
    run = subprocess.run([str(compiler), *flags, "-c", str(source), "-o", str(output)], text=True, capture_output=True)
    results[source.name] = {"exit": run.returncode, "stderr": run.stderr}
    if run.stderr:
        print(source.name, run.stderr)
report = own / "source_backend_script_host_compile_report.json"
report.write_text(json.dumps({
    "purpose": "Windows x64 COFF compile and typed contract check for the additive backend script-host provider",
    "runtime_claim": False,
    "native_runtime_acceptance": {
        "status": "not_run",
        "reason": "No genuine integrated fixture currently publishes the same Application PM plus same constructed SourceCurrentLevelBackendV1 under one source World lease. The test intentionally does not substitute a fake manager, GS slot, current Level, or debug owner.",
        "existing_component_coverage": "source_current_level_backend_native_test.cpp exercises actual GS Level C1/current slot/stale-borrow checks; character_host_context.cpp exercises native host ABI and ordered range rereads/tier boundaries; application_player_manager_bootstrap_v59.cpp exercises actual PM owner/PlayerInfo managed-field producer. A composed SourceBackendScriptHostV1 execution remains a root integration test requirement.",
    },
    "provider_requirements": [
        "same retained ApplicationServicesOwnerV5 published on SourceCurrentLevelGraphV1.application",
        "actual Application PlayerManager and online host selector whenever online byte5 is nonzero",
        "selected PlayerInfo identity borrowed from that PM network and managed_fields_produced_v70",
        "same loaded LevelTables with LevelProjection72 words 12..17 readable as LevelRangeRow24",
        "same Application DebugSwitches and retained DebugFileServices24 callback owner",
    ],
    "results": results,
}, indent=2))
raise SystemExit(0 if all(value["exit"] == 0 for value in results.values()) else 1)
