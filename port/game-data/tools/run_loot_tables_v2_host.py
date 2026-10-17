"""Sanitized host replay for the imported LootTablesV2 reader corpus."""
import argparse
import hashlib
import json
import shlex
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--distro", default="Ubuntu-22.04")
    parser.add_argument("--output", type=Path, default=ROOT / "port/game-data/reference/player-inventory-owned-v4/loot-tables-host-validation.json")
    args = parser.parse_args()
    sources = [
        "port/game-data/loot_tables_v2.hpp",
        "port/game-data/loot_tables_v2.cpp",
        "port/game-data/items.hpp",
        "port/game-data/items.cpp",
        "port/game-data/tests/loot_tables_v2.cpp",
        "port/game-data/tests/loot_tables_v2_original.py",
        "port/game-data/tests/items_differential_v4_original.py",
        "port/game-data/tests/navigation_differential_v4_original.py",
        "port/game-data/tools/run_loot_tables_v2_host.py",
    ]
    inputs = [
        "port/game-data/reference/player-creation-v2/fixtures.bin",
        ".local-inputs/items-discovery/loot_table_pyarray.bin",
        ".local-inputs/items-discovery/loot_table_pyarraynames.bin",
        ".local-inputs/items-discovery/loot_table_pystructnames.bin",
    ]
    before = {path: sha(ROOT / path) for path in sources + inputs}
    directory = ROOT / ".local-inputs/player-inventory-owned-v4"
    directory.mkdir(parents=True, exist_ok=True)
    executable = ".local-inputs/player-inventory-owned-v4/loot-tables-host"
    root_wsl = "/mnt/" + ROOT.drive[0].lower() + "/" + ROOT.as_posix()[3:]
    prefix = "cd " + shlex.quote(root_wsl) + " && "
    build = prefix + shlex.join([
        "g++", "-std=c++17", "-O1", "-g", "-Wall", "-Wextra", "-Werror",
        "-Wno-misleading-indentation", "-fno-fast-math", "-ffp-contract=off",
        "-fsanitize=address,undefined", "-fno-omit-frame-pointer",
        "port/game-data/loot_tables_v2.cpp", "port/game-data/items.cpp",
        "port/game-data/tests/loot_tables_v2.cpp", "-o", executable,
    ])
    subprocess.run(["wsl", "-d", args.distro, "-e", "bash", "-lc", build], check=True)
    run = prefix + "ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 " + shlex.join([
        executable,
        "port/game-data/reference/player-creation-v2/fixtures.bin",
        ".local-inputs/items-discovery",
    ])
    result = subprocess.run(["wsl", "-d", args.distro, "-e", "bash", "-lc", run], capture_output=True, text=True)
    (directory / "loot-tables.stdout").write_text(result.stdout)
    (directory / "loot-tables.stderr").write_text(result.stderr)
    result.check_returncode()
    audit = json.loads(result.stdout)
    if audit.get("validation") != "PASS" or audit.get("mismatches") != 0 or result.stderr:
        raise RuntimeError("LootTablesV2 host audit did not pass cleanly")
    if before != {path: sha(ROOT / path) for path in sources + inputs}:
        raise RuntimeError("LootTablesV2 source or input changed during host replay")
    report = {
        "validation": "PASS",
        "host_audit": audit,
        "source_sha256": {path: before[path] for path in sources},
        "input_sha256": before,
        "executable_sha256": sha(ROOT / executable),
        "runner_sha256": sha(__file__),
        "sanitizers": ["address", "undefined", "leak"],
        "sanitizer_findings": 0,
        "commands": [build, run],
        "scope": "Original Loot record readers, LootTable cache projection, and the frozen Loot RNG helper; does not claim the live V4 inventory RNG has been wired into the app-wide stream.",
    }
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps(report))


if __name__ == "__main__":
    main()
