import json
import pathlib
import subprocess

root = pathlib.Path.cwd()
own = root / "port/windows-foundation/features/actor_frame"
build = own / "source_backend_npc_script_bootstrap_build"
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
for source in (own / "source_backend_npc_script_bootstrap_v1.cpp", own / "source_backend_npc_script_bootstrap_contract_test.cpp"):
    output = build / (source.stem + ".o")
    run = subprocess.run([str(compiler), *flags, "-c", str(source), "-o", str(output)], text=True, capture_output=True)
    results[source.name] = {"exit": run.returncode, "stderr": run.stderr}
    if run.stderr:
        print(source.name, run.stderr)
report = own / "source_backend_npc_script_bootstrap_report.json"
report.write_text(json.dumps({
    "purpose": "Windows x64 COFF compile and typed contract validation for the connected actual NPC script bootstrap",
    "runtime_claim": False,
    "results": results,
    "runtime_boundary": "The current owned native prefix fixture deliberately lacks a published CampaignFsmV101, same-world SourceCurrentLevelBackendV1, loaded CharacterScriptAssetsV1 snapshot, and same CharacterScriptObjects. Therefore original record.load_script reaches the exact existing missing npc_script service; this lane did not fabricate those owners or edit the root-owned fixture/build closure.",
    "required_caller_providers": [
        "same source World lease used by the canonical candidate record and backend current-Level graph",
        "same-world CharacterScriptObjects and its published canonical ScriptCharacterObject",
        "loaded actual CharacterScriptAssetsV1 binding backed by the same SkillTables/cache",
        "same SourceBackendScriptHostV1 from the retained Application/current-Level backend",
        "record's already bound CampaignFsmV101; record-based path borrow is checked before event binding",
    ],
    "session_and_fsm_policy": "The service only creates CharacterScriptSessionInput; CanonicalCharacterCandidateRecordV60::load_script calls construct_script on its existing actor. No Session/FSM/World/Character is constructed by this bootstrap.",
}, indent=2))
raise SystemExit(0 if all(value["exit"] == 0 for value in results.values()) else 1)
