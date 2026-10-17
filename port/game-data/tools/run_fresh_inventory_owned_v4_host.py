"""Sanitized host replay for the imported FreshInventoryOwnedV4 corpus."""
import argparse
import hashlib
import json
import shlex
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
CACHE_FILES = [
    ".local-inputs/items-discovery/loot_table_pyarray.bin",
    ".local-inputs/items-discovery/loot_table_pyarraynames.bin",
    ".local-inputs/items-discovery/loot_table_pystructnames.bin",
]
SOURCE_FILES = [
    "port/game-data/fresh_inventory_owned_v4.hpp",
    "port/game-data/fresh_inventory_owned_v4.cpp",
    "port/game-data/loot_tables_v2.hpp",
    "port/game-data/loot_tables_v2.cpp",
    "port/game-data/item_instance.hpp",
    "port/game-data/item_instance.cpp",
    "port/game-data/items.hpp",
    "port/game-data/items.cpp",
    "port/game-data/item_power_tables_v5.cpp",
    "port/game-data/loot_power_resources_v7.cpp",
    "port/game-data/loot_power_creation_v7.cpp",
    "port/game-data/loot_entry_selection_v1.cpp",
    "port/game-data/item_presentation_v5.cpp",
    "port/game-data/tests/fresh_inventory_owned_v4.cpp",
    "port/game-data/tests/fresh_inventory_owned_v4_original.py",
    "port/game-data/tests/fresh_inventory_v2_original.py",
    "port/game-data/tests/item_inventory_v1_original.py",
    "port/game-data/tests/player_savegame_v1_original.py",
    "port/game-data/tests/items_differential_v4_original.py",
    "port/game-data/tests/navigation_differential_v4_original.py",
    "port/game-data/tools/run_fresh_inventory_owned_v4_host.py",
    "port/random/random.h",
    "port/random/random.c",
]


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--distro", default="Ubuntu-22.04")
    parser.add_argument("--random-only", action="store_true")
    parser.add_argument("--world-transfer-only", action="store_true")
    parser.add_argument("--cache", type=Path, default=ROOT / ".local-inputs/items-discovery")
    parser.add_argument(
        "--output",
        type=Path,
        default=ROOT / "port/game-data/reference/player-inventory-owned-v4/host-validation.json",
    )
    args = parser.parse_args()
    cache_dir = args.cache.resolve()
    sources = {path: sha(ROOT / path) for path in SOURCE_FILES}
    inputs = {}
    if not args.random_only:
        inputs = {
            path: sha(ROOT / path)
            for path in [
                "port/game-data/reference/player-inventory-owned-v4/fixtures.bin",
                "port/game-data/reference/player-creation-v2/fresh-fixtures.bin",
                *[str(cache_dir / Path(path).name) for path in CACHE_FILES],
                ".local-inputs/libDungeonHunter2.so",
            ]
        }

    output_dir = ROOT / ".local-inputs/player-inventory-owned-v4"
    output_dir.mkdir(parents=True, exist_ok=True)
    executable = ".local-inputs/player-inventory-owned-v4/host-audit"
    random_object = ".local-inputs/player-inventory-owned-v4/random.o"
    wsl_root = "/mnt/" + ROOT.drive[0].lower() + "/" + ROOT.as_posix()[3:]
    wsl_cache = "/mnt/" + cache_dir.drive[0].lower() + "/" + cache_dir.as_posix()[3:]
    prefix = "cd " + shlex.quote(wsl_root) + " && "
    random_build = prefix + shlex.join([
        "gcc", "-std=c11", "-O1", "-g", "-Wall", "-Wextra", "-Werror",
        "-fno-fast-math", "-ffp-contract=off", "-fsanitize=address,undefined",
        "-fno-omit-frame-pointer", "-c", "port/random/random.c", "-o", random_object,
    ])
    subprocess.run(["wsl", "-d", args.distro, "-e", "bash", "-lc", random_build], check=True)
    build = prefix + shlex.join([
        "g++", "-std=c++17", "-O1", "-g", "-Wall", "-Wextra", "-Werror",
        "-Wno-misleading-indentation", "-fno-fast-math", "-ffp-contract=off",
        "-fsanitize=address,undefined", "-fno-omit-frame-pointer",
        "port/game-data/fresh_inventory_owned_v4.cpp",
        "port/game-data/loot_tables_v2.cpp",
        "port/game-data/item_instance.cpp",
        "port/game-data/items.cpp",
        "port/game-data/item_power_tables_v5.cpp",
        "port/game-data/loot_power_resources_v7.cpp",
        "port/game-data/loot_power_creation_v7.cpp",
        "port/game-data/loot_entry_selection_v1.cpp",
        "port/game-data/item_presentation_v5.cpp",
        "port/game-data/tests/fresh_inventory_owned_v4.cpp",
        random_object, "-o", executable,
    ])
    subprocess.run(["wsl", "-d", args.distro, "-e", "bash", "-lc", build], check=True)
    if args.random_only:
        run_args = [executable, "--random-only"]
    elif args.world_transfer_only:
        run_args = [executable, "--world-transfer-only", wsl_cache]
    else:
        run_args = [
            executable,
            "port/game-data/reference/player-inventory-owned-v4/fixtures.bin",
            "port/game-data/reference/player-creation-v2/fresh-fixtures.bin",
            wsl_cache,
        ]
    command = prefix + "ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 " + shlex.join(run_args)
    result = subprocess.run(
        ["wsl", "-d", args.distro, "-e", "bash", "-lc", command], capture_output=True, text=True
    )
    (output_dir / "host.stdout").write_text(result.stdout)
    (output_dir / "host.stderr").write_text(result.stderr)
    result.check_returncode()
    audit = json.loads(result.stdout)
    if audit.get("validation") != "PASS" or audit.get("mismatches", 0) != 0 or result.stderr:
        raise RuntimeError("FreshInventoryOwnedV4 host audit did not pass cleanly")
    if sources != {path: sha(ROOT / path) for path in SOURCE_FILES}:
        raise RuntimeError("FreshInventoryOwnedV4 source changed during host replay")
    if inputs and inputs != {path: sha(ROOT / path) for path in inputs}:
        raise RuntimeError("FreshInventoryOwnedV4 original inputs changed during host replay")
    report = {
        "validation": "PASS",
        "host_audit": audit,
        "source_sha256": sources,
        "input_sha256": inputs,
        "random_object_sha256": sha(ROOT / random_object),
        "executable_sha256": sha(ROOT / executable),
        "sanitizers": ["address", "undefined", "leak"],
        "sanitizer_findings": 0,
        "commands": [random_build, build, command],
        "scope": "V4 source inventory/equipment graph and cached table parsing against imported original gold. Live mode borrows PropertyState and a caller RNG callback; the legacy LootRandom8V2 constructor is fixture-only. This does not prove native runtime RNG has been unified or bound.",
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report))


if __name__ == "__main__":
    main()
