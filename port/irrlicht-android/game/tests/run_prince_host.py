#!/usr/bin/env python3
"""Build and run the renderer-neutral Prince Character/BlendedPlayback proof."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import sys


HERE = Path(__file__).resolve().parent
GAME = HERE.parent
IRRLICHT = GAME.parent
PORT = IRRLICHT.parent
REPO = PORT.parent
DEFAULT_ASSETS = REPO / "port" / "android-native" / "app" / "src" / "main" / "assets"
ANIMATION_DATA = (
    "animations_pyarray.bin",
    "animations_pyarraynames.bin",
    "animations_pystructnames.bin",
    "animations_dictionary_pyarraynames.bin",
    "animations_dictionary_pyarray.bin",
    "prince-animation-bank.bin",
    "character_properties_pyarray.bin",
    "character_properties_pyarraynames.bin",
    "character_properties_pystructnames.bin",
    "character_classes_pyarray.bin",
    "character_classes_pyarraynames.bin",
    "character_classes_pystructnames.bin",
)
SOURCES = (
    HERE / "prince_actor_host.cpp",
    GAME / "prince_actor.cpp",
    GAME / "prince_character_runtime.cpp",
    PORT / "level-world" / "visual_motion.cpp",
    PORT / "level-world" / "physical_controls.cpp",
    PORT / "level-world" / "actor_playback.cpp",
    PORT / "level-world" / "actor_blended_playback.cpp",
    PORT / "level-world" / "animation_blender.cpp",
    PORT / "level-world" / "visual_timeline.cpp",
    PORT / "level-world" / "character_coordinator.cpp",
    PORT / "level-world" / "character_state.cpp",
    PORT / "level-world" / "character_timers.cpp",
    PORT / "level-world" / "character_stance.cpp",
    PORT / "level-world" / "character_scene.cpp",
    PORT / "level-world" / "move_state.cpp",
    PORT / "level-world" / "decor_scene.cpp",
    PORT / "level-world" / "decor_body_config.cpp",
    PORT / "engine-skinning" / "skinning.cpp",
    PORT / "scene-materials" / "scene.cpp",
    PORT / "game-data" / "data.cpp",
    PORT / "game-data" / "animation_bank.cpp",
    PORT / "game-data" / "animation_tables.cpp",
    PORT / "game-data" / "animation_scheduler.cpp",
    PORT / "game-data" / "animation_selection.cpp",
    PORT / "game-data" / "class_tables.cpp",
    PORT / "game-data" / "properties.cpp",
    PORT / "engine-animation" / "animation_registration.cpp",
    PORT / "engine-animation" / "animation.cpp",
    PORT / "engine-animation" / "animation_blend.cpp",
    PORT / "engine-animation" / "component_applicator.cpp",
    PORT / "engine-animation" / "angle_interpreter.cpp",
    PORT / "engine-animation" / "events.cpp",
    PORT / "engine-animation" / "event_track.cpp",
    PORT / "asset-payloads" / "payloads.cpp",
    PORT / "engine-resources" / "resources.cpp",
    PORT / "engine-math" / "math.cpp",
)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def resolve_compiler(requested: str | None) -> Path:
    choices = [requested] if requested else []
    if not requested and os.environ.get("CXX"):
        choices.append(os.environ["CXX"])
    choices.extend(("g++", "c++", "clang++"))
    for choice in choices:
        if not choice:
            continue
        found = shutil.which(choice)
        if found:
            return Path(found).resolve()
        candidate = Path(choice).expanduser()
        if candidate.is_file():
            return candidate.resolve()
    if os.name == "nt":
        for root in (Path.home() / ".local" / "mingw" / "mingw64",
                     Path("C:/msys64/mingw64"), Path("C:/msys64/ucrt64"),
                     Path("C:/mingw64")):
            candidate = root / "bin" / "g++.exe"
            if candidate.is_file():
                return candidate.resolve()
    raise SystemExit("C++ compiler not found; install MinGW-w64 or set CXX/--compiler")


def run(command: list[str | Path], cwd: Path) -> str:
    args = [str(value) for value in command]
    result = subprocess.run(args, cwd=cwd, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, text=True, check=False)
    quoted = subprocess.list2cmdline(args) if os.name == "nt" else shlex.join(args)
    print(f"+ {quoted}", flush=True)
    if result.stdout:
        print(result.stdout, end="" if result.stdout.endswith("\n") else "\n")
    if result.returncode:
        raise RuntimeError(f"command exited {result.returncode}: {quoted}\n{result.stdout[-12000:]}")
    return result.stdout


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--assets", type=Path, default=DEFAULT_ASSETS,
                        help="app asset tree with the source model, tables and complete bank clips")
    parser.add_argument("--compiler", "--cxx", dest="compiler", default=None)
    parser.add_argument("--output", type=Path,
                        default=IRRLICHT / "build" / "prince-character-host-checks")
    args = parser.parse_args()
    source_root = args.assets.expanduser().resolve(strict=True)
    output = args.output.expanduser().resolve()
    output.mkdir(parents=True, exist_ok=True)
    compiler = resolve_compiler(args.compiler)
    bank_json = source_root / "data/prince-animation-bank.json"
    bank = json.loads(bank_json.read_text(encoding="utf-8"))
    if (bank.get("character") != "KnightPlayerBase" or
            bank.get("animation_table") != 48 or
            bank.get("animation_set_id") != 12302 or
            bank.get("template_clip_id") != 1111 or
            len(bank.get("resources", [])) != 116 or
            len(bank.get("registration_requests", [])) != 158):
        raise SystemExit("authored Prince bank metadata differs from the checked 116/158 source bank")

    missing_sources = [path for path in SOURCES if not path.is_file()]
    if missing_sources:
        raise SystemExit("missing runtime host source(s):\n" + "\n".join(map(str, missing_sources)))
    input_paths = [source_root / "models/prince_modular.bdae"]
    input_paths.extend(source_root / "data" / name for name in ANIMATION_DATA)
    input_paths.extend(source_root / resource["asset"] for resource in bank["resources"])
    missing_assets = [path for path in input_paths if not path.is_file()]
    if missing_assets:
        raise SystemExit("missing checked Prince input(s):\n" + "\n".join(map(str, missing_assets[:30])))

    staged_root = output / "prince-input"
    staged: dict[str, Path] = {}
    def copy_checked(source: Path, relative: str, expected_size: int | None = None,
                     expected_hash: str | None = None) -> None:
        target = staged_root / Path(relative)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
        identity = {"bytes": source.stat().st_size, "sha256": sha256(source)}
        if expected_size is not None and identity["bytes"] != expected_size:
            raise RuntimeError(f"asset size differs from its bank metadata: {source}")
        if expected_hash is not None and identity["sha256"] != expected_hash:
            raise RuntimeError(f"asset hash differs from its bank metadata: {source}")
        if {"bytes": target.stat().st_size, "sha256": sha256(target)} != identity:
            raise RuntimeError(f"staged asset changed: {source}")
        staged[relative] = source

    copy_checked(input_paths[0], "models/prince_modular.bdae")
    for name in ANIMATION_DATA:
        copy_checked(source_root / "data" / name,
                     f"dh2/prince-animation/data/{name}")
    for resource in bank["resources"]:
        copy_checked(source_root / resource["asset"],
                     f"dh2/prince-animation/{resource['asset']}",
                     resource["bytes"], resource["sha256"])

    executable = output / ("prince-actor-host.exe" if os.name == "nt"
                           else "prince-actor-host")
    command: list[str | Path] = [
        compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror",
        "-Wno-misleading-indentation", "-Wno-unused-parameter",
        "-fexceptions", "-fno-rtti", "-I", GAME,
        "-I", IRRLICHT / "upstream" / "include", "-I", PORT,
        "-I", PORT / "android-app", *SOURCES, "-o", executable,
    ]
    compiler_version = run([compiler, "--version"], cwd=REPO)
    compile_output = run(command, cwd=REPO)
    test_output = run([executable, staged_root], cwd=REPO)
    required = (
        "controllers=4", "bank_resources=116", "registration_occurrences=158",
        "timeline_less_resources=", "walk_clip=1126",
        "state_idle=", "state_walk=", "current_state=3", "current_sequence=",
        "idle_release=pass", "source_fsm=pass",
        "owner_translation_rms=", "full_game_ai_physics_combat=not_implemented",
    )
    if not all(token in test_output for token in required):
        raise RuntimeError("source Character host assertions did not report all expected checks")

    report = {
        "validation": "PASS",
        "scope": (
        "Host integration check loads Prince's complete authored AnimationBank and source tables, "
        "routes Idle/Move/release through the shared Character Coordinator and two-slot "
        "BlendedPlayback, retains no-payload registration identities, CPU-deforms the four source "
        "skins, and checks one owner translation. "
            "It does not validate Irrlicht GPU rendering, APK behavior, full actor physics, AI or combat."
        ),
        "compiler": str(compiler),
        "compiler_version": compiler_version.splitlines()[0] if compiler_version else "",
        "compile_command": [str(item) for item in command],
        "test_output": test_output.strip(),
        "bank": {
            "character": bank["character"],
            "animation_table": bank["animation_table"],
            "animation_set_id": bank["animation_set_id"],
            "template_clip_id": bank["template_clip_id"],
            "unique_resources": len(bank["resources"]),
            "registration_requests": len(bank["registration_requests"]),
        },
        "asset_inputs": {
            relative: {"path": str(source), "bytes": source.stat().st_size,
                       "sha256": sha256(source)}
            for relative, source in sorted(
                (relative, source) for relative, source in staged.items())
        },
        "source_sha256": {
            source.relative_to(REPO).as_posix(): sha256(source)
            for source in SOURCES
        },
        "executable": {"path": str(executable), "bytes": executable.stat().st_size,
                       "sha256": sha256(executable)},
        "generated_output": compile_output.strip(),
    }
    report_path = output / "validation.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"PASS: source Prince Character host assertions; report={report_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, RuntimeError) as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        raise SystemExit(1)
