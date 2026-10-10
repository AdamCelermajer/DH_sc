from pathlib import Path
import subprocess


ROOT = Path(__file__).resolve().parents[4]
FEATURE = ROOT / "port/windows-foundation/features/effects"
HEADER = FEATURE / "runtime_combat_effects_v1.hpp"
SOURCE = FEATURE / "runtime_combat_effects_v1.cpp"


def require(condition, message):
    if not condition:
        raise SystemExit(f"FAIL {message}")


header = HEADER.read_text()
source = SOURCE.read_text()
require("CombatSession&" in header and "RetainedEffectsAdapter" in header,
        "runtime must borrow the same CombatSession and retained event adapter")
require("retained_actor_pose(actor)" in source and
        "retained_actor_visual_borrow(actor)" in source,
        "actor visual must come from the Entry paired with the same live retained pose")
require("same_owner(current_lease, binding_lease_)" in source,
        "visual/pose borrows must be guarded by the exact current session lease")
require("event(ActorId" in header and "events_->event(static_cast<std::uintptr_t>(actor)" in source,
        "source event route must preserve stable ActorId through the retained adapter")
require("event.name.compare(0, 3, \"fx_\")" in source,
        "only original source fx_ events may enter this adapter")
require(source.index("executor.scene_phase(absolute_ms, app_dt") <
        source.index("executor.manager_phase(app_dt"),
        "source Scene sampling must precede manager update with the same App dt")
require("frame_loans_" in source and "Drain root FX render-frame loans" in source,
        "session lease renewal must wait for actual frame-loan drain")
require("visual->socket_world(name, local_offset, output, error)" in source,
        "authored socket must use the same retained CharacterVisual as its pose")
require("events_->release_actor(static_cast<std::uintptr_t>(actor))" in source,
        "actor/session renewal must detach old retained FX anchors and marker tokens")

compiler = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
require(compiler.exists(), "configured LLVM-MinGW compiler is unavailable")
command = [str(compiler), "-std=c++17", "-Wall", "-Wextra", "-pedantic",
           "-Wno-missing-field-initializers", "-Werror",
           "-I", str(ROOT / "port/windows-foundation"),
           "-I", str(ROOT / "port/level-world"),
           "-I", str(ROOT / "port/scene-materials"),
           "-I", str(ROOT / "port/engine-resources"),
           "-fsyntax-only", str(SOURCE)]
result = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
require(result.returncode == 0, result.stdout + result.stderr)
print("PASS runtime combat effects source contract and strict Windows COFF syntax compile")
