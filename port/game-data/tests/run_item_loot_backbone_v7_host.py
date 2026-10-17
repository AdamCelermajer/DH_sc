"""Verify item presentation/powered loot through the selected game-data DSO.

Requires the existing canonical pydata cache; never copies cache bytes into Git.
Text/debug providers in composition are explicitly bounded fixtures. The test
does not establish native loot, pickup, localization, or the full AddLoot caller.
"""
import argparse
import hashlib
import json
import os
import re
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parents[3]
REF = ROOT / "port/game-data/reference/adam-791e961-item-loot"


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True)
    parser.add_argument("--build", type=Path, required=True)
    parser.add_argument("--compiler", type=Path, required=True)
    parser.add_argument("--report", type=Path, required=True)
    args = parser.parse_args()
    args.build.mkdir(parents=True, exist_ok=True)
    env = os.environ.copy()
    env["PATH"] = str(args.compiler.parent) + os.pathsep + env.get("PATH", "")
    units = ["item_presentation_v5.cpp", "loot_power_creation_v7.cpp"]
    logs = []

    def run(command):
        result = subprocess.run([str(x) for x in command], cwd=ROOT, env=env, capture_output=True, text=True)
        logs.append({"command": [str(x) for x in command], "returncode": result.returncode,
                     "stdout": result.stdout, "stderr": result.stderr})
        if result.returncode:
            (args.build / "gate-command-outputs.json").write_text(json.dumps(logs, indent=2) + "\n")
            raise RuntimeError(result.stderr or result.stdout)
        return result.stdout

    run(["cmake", "-S", ROOT / "port/game-data/tests/item-loot-backbone-v7", "-B", args.build,
         "-G", "Ninja", "-DCMAKE_BUILD_TYPE=RelWithDebInfo", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
         "-DCMAKE_CXX_COMPILER=" + str(args.compiler),
         "-DCMAKE_C_COMPILER=" + str(args.compiler.with_name("gcc.exe"))])
    database = json.loads((args.build / "compile_commands.json").read_text())
    selected = [entry for entry in database if "CMakeFiles/dh2_game_data.dir" in entry["command"].replace("\\", "/")]
    # Record the actual selected DSO/test source closure, excluding unrelated
    # drafts and unbuilt audit targets also listed by central CMake.
    reached = [entry for entry in database if "CMakeFiles/dh2_game_data.dir" in entry["command"].replace("\\", "/")
               or "CMakeFiles/item_presentation_v5_selected.dir" in entry["command"].replace("\\", "/")
               or "CMakeFiles/item_loot_backbone_v7_selected.dir" in entry["command"].replace("\\", "/")]
    sources = set()
    pending = [Path(entry["file"]).resolve() for entry in reached]
    while pending:
        path = pending.pop()
        if path in sources:
            continue
        path.relative_to(ROOT)
        sources.add(path)
        for include in re.findall(r'^\s*#\s*include\s*"([^"]+)"', path.read_text(), re.MULTILINE):
            dependency = (path.parent / include).resolve()
            if dependency.is_file():
                pending.append(dependency)
    sources.update([Path(__file__).resolve(), ROOT / "port/game-data/CMakeLists.txt",
                    ROOT / "port/game-data/tests/item-loot-backbone-v7/CMakeLists.txt"])
    before = {p.relative_to(ROOT).as_posix(): digest(p) for p in sorted(sources)}
    run(["cmake", "--build", args.build, "--parallel", "1", "--target",
         "item_presentation_v5_selected", "item_loot_backbone_v7_selected",
         "item_loot_random_table_v1_selected"])
    for name in units:
        matches = [entry for entry in selected if Path(entry["file"]).name == name]
        assert len(matches) == 1, (name, matches)
    # On Windows, the DLL is produced under the shared-library subdirectory.
    library = args.build / "game-data/libdh2_game_data.dll"
    env["PATH"] = str(library.parent) + os.pathsep + env["PATH"]
    source_loot_fixtures = [
        ROOT / "port/game-data/reference/player-creation-v2/powered-addloot-v7.bin",
        ROOT / "port/game-data/reference/player-creation-v2/powered-random-addloot-v7.bin",
        ROOT / "port/game-data/reference/player-creation-v2/powered-nested-addloot-v7.bin",
        ROOT / "port/game-data/reference/player-creation-v2/powered-gold-addloot-v7.bin",
    ]
    commands = [
        [args.build / "item_presentation_v5_selected.exe", REF / "player-item-effects-v5/presentation-fixtures.bin",
         REF / "player-item-effects-v5/power-instance-fixtures.bin", args.cache],
        [args.build / "item_loot_backbone_v7_selected.exe", REF / "loot-power-creation-v7/fixtures.bin", args.cache,
         source_loot_fixtures[0]],
        [args.build / "item_loot_random_table_v1_selected.exe",
             source_loot_fixtures[1], args.cache],
        [args.build / "item_loot_random_table_v1_selected.exe",
             source_loot_fixtures[2], args.cache],
        [args.build / "item_loot_random_table_v1_selected.exe",
             source_loot_fixtures[3], args.cache],
    ]
    tests = [{"command": [str(x) for x in command], "result": json.loads(run(command))} for command in commands]
    assert all(test["result"]["validation"] == "PASS" for test in tests)
    after = {p.relative_to(ROOT).as_posix(): digest(p) for p in sources}
    assert before == after, "Source changed during the gate; rerun the frozen selection"
    report = {
        "validation": "PASS", "upstream_commit": "791e961b12233100b303038c961666834f4beb9d",
        "scope": "Candidate through selected shared library. Original-derived fixed/random/nested AddLoot snapshots and RNG are compared, including LootTable row 124 (Gold_01, Type 13); pickup converts its source-valued world item into wallet gold. Text/debug/AddPower are explicit fixtures; this does not claim Android runtime gameplay parity or full AddLoot coverage.",
        "source_before_after_equal": True, "source_sha256": before,
        "selected_library": {"path": str(library), "sha256": digest(library)},
        "selected_commands": selected, "tests": tests,
        "gold_sha256": {p.relative_to(REF).as_posix(): digest(p) for p in REF.rglob("*.bin")},
        "original_addloot_fixtures_sha256": {p.name: digest(p) for p in source_loot_fixtures},
        "cache_sha256": {p.name: digest(p) for p in args.cache.glob("*.bin")
                         if p.name.startswith(("loot_table_", "item_powers_"))},
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n")
    (args.build / "gate-command-outputs.json").write_text(json.dumps(logs, indent=2) + "\n")
    print(json.dumps({"validation": "PASS", "tests": [test["result"] for test in tests], "report": str(args.report)}))


if __name__ == "__main__":
    main()
