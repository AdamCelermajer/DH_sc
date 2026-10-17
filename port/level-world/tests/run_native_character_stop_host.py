"""Build and exercise the native Character Stop adapter on pinned Box2D."""
from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import subprocess
import sys
import time
from pathlib import Path

MODULE = Path(__file__).resolve().parents[1]
REPO = MODULE.parents[1]
NATIVE = REPO / "port/android-native/app/src/main/cpp"
BOX2D = REPO / "port/physics-backend/box2d-2.0.1"


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default=None)
    parser.add_argument("--build-dir", type=Path,
                        default=MODULE / "build/native-character-stop-host")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/native-character-stop-host/validation.json")
    args = parser.parse_args()
    for name in ("build_dir", "report"):
        path = getattr(args, name)
        if not path.is_absolute():
            setattr(args, name, (Path.cwd() / path).resolve())

    compiler = args.compiler or shutil.which("g++") or shutil.which("clang++")
    if not compiler:
        parser.error("pass --compiler or install a C++17 compiler")
    if not (BOX2D / "Include/Box2D.h").is_file():
        parser.error(f"pinned Box2D headers not found under {BOX2D}")

    started = time.monotonic()
    args.build_dir.mkdir(parents=True, exist_ok=True)
    production_sources = [
        NATIVE / "native_character_stop.cpp",
        MODULE / "game_object_stop.cpp",
        MODULE / "character_physics_position.cpp",
        MODULE / "move_state.cpp",
        MODULE / "physical_controls.cpp",
        MODULE / "native_body.cpp",
        MODULE / "navigation_path.cpp",
        MODULE / "navigation_controller.cpp",
        MODULE / "navigation_heading.cpp",
        MODULE / "character_coordinator.cpp",
        MODULE / "character_state.cpp",
        MODULE / "character_timers.cpp",
        MODULE / "character_skill_state_dispatch_v1.cpp",
        MODULE / "character_skill_fsm_callbacks_v1.cpp",
        MODULE / "tests/native_character_stop.cpp",
    ]
    box_sources = sorted((BOX2D / "Source").rglob("*.cpp"))
    if len(box_sources) != 31:
        raise AssertionError(f"pinned Box2D source count changed: {len(box_sources)}")

    include_args = ["-I", str(MODULE), "-I", str(NATIVE),
                    "-isystem", str(BOX2D / "Include")]
    objects: list[Path] = []
    commands: list[list[str]] = []
    tracked: list[Path] = []

    for index, source in enumerate(production_sources):
        if not source.is_file():
            raise AssertionError(f"required source is missing: {source}")
        obj = args.build_dir / f"native_character_stop_{index:02d}.o"
        warning_policy = ( ["-Wno-misleading-indentation"]
                          if source.name in {"native_body.cpp", "physical_controls.cpp",
                                             "navigation_path.cpp", "navigation_controller.cpp",
                                             "navigation_heading.cpp",
                                             "character_coordinator.cpp", "character_state.cpp",
                                             "character_timers.cpp"}
                          else [] )
        commands.append([compiler, "-std=c++17", "-O1", "-Wall", "-Wextra",
                         "-Werror", "-pedantic", "-ffunction-sections",
                         "-fdata-sections", "-fno-fast-math", "-ffp-contract=off",
                         "-include", str(MODULE / "tests/box2d_host_compat.hpp"),
                         *warning_policy, *include_args, "-c", str(source), "-o", str(obj)])
        objects.append(obj)
        tracked.append(source)

    for index, source in enumerate(box_sources):
        obj = args.build_dir / f"box2d_{index:02d}.o"
        # Legacy vendor warnings are kept out of our strict project-TU gate.
        commands.append([compiler, "-std=c++17", "-O1", "-w", "-fno-fast-math",
                         "-ffp-contract=off", "-include",
                         str(MODULE / "tests/box2d_host_compat.hpp"), *include_args,
                         "-c", str(source), "-o", str(obj)])
        objects.append(obj)
        tracked.append(source)

    executable = args.build_dir / "native_character_stop_host.exe"
    commands.append([compiler, *map(str, objects), "-Wl,--gc-sections", "-o",
                     str(executable)])
    for command in commands:
        subprocess.run(command, cwd=REPO, check=True)

    output = subprocess.check_output([str(executable)], cwd=REPO, text=True)
    replay = json.loads(output)
    assert replay["validation"] == "PASS" and replay["scenarios"] == 14
    assert replay["real_box2d"] and replay["source_path_drop"]
    assert replay["policy_fresh_read"] and replay["native_put_to_sleep"]
    assert replay["subsequent_empty_path_frame"]
    assert replay["synchronous_transform_reentry"]
    assert replay["callback_prefix_effects"]
    assert replay["fresh_final_owner"] and replay["owner_alias_rejection"]

    # Include headers which the actual host compiler read from the backend.
    box_headers = sorted(path for root in (BOX2D / "Include", BOX2D / "Source")
                         for path in root.rglob("*.h"))
    inputs = sorted(set(tracked + box_headers + [
        NATIVE / "native_character_stop.hpp",
        MODULE / "actor_runtime.hpp", MODULE / "character_coordinator.hpp",
        MODULE / "character_state.hpp", MODULE / "character_timers.hpp",
        MODULE / "character_skill_state_dispatch_v1.hpp",
        MODULE / "character_skill_fsm_callbacks_v1.hpp",
        MODULE / "character_physics_position.hpp", MODULE / "move_state.hpp",
        MODULE / "game_object_stop.hpp", MODULE / "physical_controls.hpp",
        MODULE / "native_body.hpp", MODULE / "navigation_controller.hpp",
        MODULE / "navigation_heading.hpp", MODULE / "navigation_world.hpp",
        MODULE / "navigation_motion.hpp", MODULE / "navigation_path.hpp",
        MODULE / "navigation_avoidance.hpp", MODULE / "navigation_objects.hpp",
        MODULE / "navigation_world.hpp", Path(__file__).resolve(),
        MODULE / "tests/box2d_host_compat.hpp",
    ]))
    report = {
        "validation": "PASS",
        "native_fixture": replay,
        "box2d_release": "2.0.1",
        "box2d_source_cpp_count": len(box_sources),
        "compiler": compiler,
        "compile_commands": commands,
        "source_sha256": {path.relative_to(REPO).as_posix(): sha(path)
                          for path in inputs},
        "scope": (
            "The typed native Character Stop adapter composes the frozen source "
            "caller and Character policy with live path/body projections over an "
            "actual pinned Box2D world. It verifies Stop ordering, route storage "
            "lifetime, live policy reads, native final sleep/force clearing, "
            "absent and mismatched bodies, ignored false SetXForm, partial errors, "
            "synchronous transform observers/reentry and failures after native "
            "effects, fresh final-owner resolution, owner/route alias rejection, "
            "and the next empty-path controller frame. No renderer, Android, "
            "emulator, Character AI, or autonomous pursuit wiring is exercised."
        ),
        "elapsed_seconds": round(time.monotonic() - started, 2),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "scenarios": replay["scenarios"],
                      "box2d_source_cpp_count": len(box_sources),
                      "elapsed_seconds": report["elapsed_seconds"]}))


if __name__ == "__main__":
    main()
