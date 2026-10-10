from pathlib import Path
import hashlib
import json
import subprocess

ROOT = Path(__file__).resolve().parents[4]
FEATURE = ROOT / "port/windows-foundation/features/effects"
SOURCE = FEATURE / "runtime_effects_factory_v1.cpp"
HEADER = FEATURE / "runtime_effects_factory_v1.hpp"
SOURCE_KERNEL = ROOT / "port/level-world/character_fx_state_v1.cpp"
FIXTURE = ROOT / "port/level-world/reference/character-fx-owner-v1/freeze-manifest-v2.json"


def require(value, message):
    if not value:
        raise SystemExit(f"FAIL {message}")


header = HEADER.read_text()
source = SOURCE.read_text()
kernel = SOURCE_KERNEL.read_text()
require("retained_actor_pose(scene_actor)" in source and
        "retained_actor_visual_borrow(scene_actor)" in source,
        "manager scene must be the visual paired with the same session pose")
require("visual->retained_scene_borrow()" in source and
        "*impl->scene" in source,
        "manager and particle resource owner must share the actual retained Scene")
require("same_owner(session.actor_binding_lease(), impl->lease)" in source and
        "q.live_scene != self.scene" in source,
        "factory must reject scene/session lease replacement")
require("state->alive()" in source and "policy.disabled" in source,
        "dead and bound-but-disabled are separate source semantics")
require("q.result = 0;" in source and "anchor_stationary" in source and
        "output-equivalent" in source,
        "stationary optimization must resample transforms without inventing native state")
require("character_fx_floor_query_v3" in source and
        "same_pf_world" in header,
        "FX floor source query must use the caller's same PFWorld")
require("read_content(*self.bindings.assets, uri)" in source and
        "OriginalEffectMaterialBinding" in source,
        "original FX assets and source material decoder must remain in the factory path")
require("debug_load" in kernel and "anchor_dead" in kernel and
        "anchor_disabled" in kernel and "anchor_stationary" in kernel,
        "policy claims must remain grounded in the source state update branches")
require("source_flags520" not in source and "source_movement_type" not in source,
        "do not infer disabled/stationary from unrelated native-state projections")

manifest = json.loads(FIXTURE.read_text())
table_hashes = {}
for relative, expected in manifest["input_sha256"].items():
    if "effects_" not in relative:
        continue
    path = ROOT / relative
    require(path.exists(), f"missing actual source EffectsTables input: {relative}")
    actual = hashlib.sha256(path.read_bytes()).hexdigest()
    require(actual == expected, f"source EffectsTables input changed: {relative}")
    table_hashes[relative.replace("\\", "/")] = actual
require(len(table_hashes) == 5, "all five original EffectsTables files must be present")

compiler = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
require(compiler.exists(), "configured LLVM-MinGW compiler is unavailable")
includes = ["-I", str(ROOT / "port/windows-foundation"),
            "-I", str(ROOT / "port/level-world"),
            "-I", str(ROOT / "port/scene-materials"),
            "-I", str(ROOT / "port/engine-resources")]
compiled = []
for path in (SOURCE, FEATURE / "runtime_combat_effects_v1.cpp"):
    command = [str(compiler), "-std=c++17", "-Wall", "-Wextra", "-pedantic",
               "-Wno-missing-field-initializers", "-Werror", *includes,
               "-fsyntax-only", str(path)]
    result = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
    require(result.returncode == 0, result.stdout + result.stderr)
    compiled.append(str(path.relative_to(ROOT)).replace("\\", "/"))

report = {
    "feature": "RuntimeEffectsFactoryV1",
    "validation": "PASS_SOURCE_CONTRACT_AND_ACTUAL_ASSET_INTEGRITY",
    "same_session_scene": "CombatSession retained pose + exact Entry.visual + retained Scene; manager and particle factory borrow the same Scene",
    "source_policy": {
        "dead": "same-session ActorState::alive()",
        "disabled": "optional authored FxActorPolicyV1 override; default enabled while actor remains in the bound session",
        "stationary": "resample transform every frame; source stationary branch only skips resync and this is output-equivalent",
        "debug": "diagnostic-only calls are no-op; effects module is explicitly enabled by constructing this owner",
        "floor": "character_fx_floor_query_v3 over caller's same PFWorld",
    },
    "changed_files": [
        "port/windows-foundation/features/effects/runtime_effects_factory_v1.hpp",
        "port/windows-foundation/features/effects/runtime_effects_factory_v1.cpp",
        "port/windows-foundation/features/effects/runtime_effects_factory_v1_tests.py",
        "port/windows-foundation/features/effects/runtime-effects-factory-v1-report.json",
    ],
    "verification": {
        "strict_windows_syntax": compiled,
        "actual_original_effects_table_inputs_sha256": table_hashes,
        "source_policy_contract": "PASS",
    },
    "explicit_root_services": [
        "source-table borrow and AssetCatalog",
        "same PFWorld",
        "camera for the exact retained Scene and actual renderer driver type",
        "original texture upload/release while the matching GL context is current",
        "root render-queue submission and frame-loan drain",
        "optional authored bound-but-disabled ActorId policy",
    ],
    "not_claimed": [
        "No actor policy is inferred from idle/action or native offsets.",
        "No camera matrix, floor, effect, marker, texture, or render packet is synthesized.",
        "The runner verifies source contract and original input integrity; live CombatSession factory creation and GL submission remain root integration work.",
    ],
}
(FEATURE / "runtime-effects-factory-v1-report.json").write_text(json.dumps(report, indent=2) + "\n")
print(json.dumps({"validation": report["validation"],
                   "strict_windows_syntax_units": len(compiled),
                   "actual_effects_table_inputs": len(table_hashes)}, indent=2))
