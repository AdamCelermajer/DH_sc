from pathlib import Path
import hashlib
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[4]
FEATURE = ROOT / "port/windows-foundation/features/effects"
BUILD = ROOT / ".local-inputs/runtime-source-fx-asset-v1"
COMPILER = ROOT / ".local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe"
EFFECT = (ROOT / "port/level-world/reference/shared-target-facing-v1/cache" /
          "skill_dh2_prince_warrior_bash_down.bdae")
ANIMATION = (ROOT / "port/level-world/reference/shared-target-facing-v1/cache/animations" /
             "skill_dh2_prince_warrior_bash_down.bdae")
OUTPUT = BUILD / "runtime-source-fx-asset-v1-tests.exe"
REL = Path("data/3d/interface/skill_dh2_prince_warrior_bash_down.bdae")

EXPECTED_EFFECT = "21a31d374a80b9b6f7d1f9b7f273c62459dde9bfd9a2fabfa629fc2fae83a483"
EXPECTED_ANIMATION = "087068efba8eb4626510292c4cf9257e119e209965e7ab591e5250ea2609be0b"


def require(ok, message):
    if not ok:
        raise SystemExit(f"FAIL | {message}")


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def place(source, destination):
    destination.parent.mkdir(parents=True, exist_ok=True)
    if destination.exists():
        destination.unlink()
    try:
        destination.hardlink_to(source)
    except OSError:
        shutil.copyfile(source, destination)
    require(sha(source) == sha(destination), f"staged resource changed: {destination}")


require(COMPILER.exists(), f"LLVM-MinGW compiler missing: {COMPILER}")
require(EFFECT.is_file() and ANIMATION.is_file(), "Both authored BDAE resources must exist")
require(EFFECT.name == ANIMATION.name, "Fixture no longer exercises a same-basename collision")
require(EFFECT.stat().st_size == 34420 and sha(EFFECT) == EXPECTED_EFFECT,
        "Actual authored BashDown effect identity changed")
require(ANIMATION.stat().st_size == 15528 and sha(ANIMATION) == EXPECTED_ANIMATION,
        "Mistaken same-basename animation identity changed")

BUILD.mkdir(parents=True, exist_ok=True)
good = BUILD / "good-assets"
missing = BUILD / "missing-assets"
wrong = BUILD / "wrong-assets"
cache = BUILD / "cache-assets"
for root in (good, missing, wrong, cache):
    root.mkdir(parents=True, exist_ok=True)
place(EFFECT, good / REL)
place(ANIMATION, good / "animations" / EFFECT.name)
place(ANIMATION, missing / "animations" / EFFECT.name)
place(ANIMATION, wrong / REL)
place(EFFECT, cache / "original-cache" / REL)

sources = [
    FEATURE / "runtime_source_fx_asset_v1_tests.cpp",
    FEATURE / "runtime_source_fx_asset_v1.cpp",
    ROOT / "port/windows-foundation/asset_catalog.cpp",
    ROOT / "port/windows-foundation/content_paths.cpp",
    ROOT / "port/asset-payloads/sha256.cpp",
    ROOT / "port/asset-payloads/payloads.cpp",
    ROOT / "port/engine-resources/resources.cpp",
    ROOT / "port/scene-materials/particle_scene_v1.cpp",
    ROOT / "port/scene-materials/scene.cpp",
    ROOT / "port/engine-math/math.cpp",
]
for path in sources:
    require(path.is_file(), f"test source missing: {path}")

includes = []
for directory in [
    "port/windows-foundation", "port/engine-resources", "port/scene-materials",
    "port/asset-payloads", "port/engine-math",
]:
    includes += ["-I", str(ROOT / directory)]

command = [
    str(COMPILER), "-std=c++17", "-O1", "-static", "-Wall", "-Wextra", "-Werror",
    "-Wno-missing-field-initializers", "-Wno-misleading-indentation", *includes,
    *map(str, sources), "-o", str(OUTPUT),
]
build = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
(BUILD / "build.log").write_text(build.stdout + build.stderr)
require(build.returncode == 0, build.stdout + build.stderr)
run = subprocess.run([str(OUTPUT), str(good), str(missing), str(wrong), str(cache)],
                     cwd=ROOT, text=True, capture_output=True)
(BUILD / "run.log").write_text(run.stdout + run.stderr)
require(run.returncode == 0, run.stdout + run.stderr)
print(run.stdout, end="")
print(f"Evidence | effect={EFFECT} | bytes={EFFECT.stat().st_size} | sha256={EXPECTED_EFFECT}")
print(f"Evidence | animation={ANIMATION} | bytes={ANIMATION.stat().st_size} | sha256={EXPECTED_ANIMATION}")
