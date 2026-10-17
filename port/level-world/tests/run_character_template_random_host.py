#!/usr/bin/env python3
"""Validate cached Crypt slot count, then compile and run the source RNG adapter tests."""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPO = HERE.parents[2]
RANDOM = REPO / "port" / "random"
SPAWN_RUNTIME = REPO / "port" / "actor-spawn-runtime"
sys.path.insert(0, str(SPAWN_RUNTIME))
from actor_spawn_runtime import parse_character_templates  # noqa: E402


def require(condition: bool, message: str) -> None:
    if not condition:
        raise RuntimeError(message)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def crypt_template_fixture(cache: Path) -> dict:
    pydata = cache / "data" / "pydata"
    array_path = pydata / "character_templates_pyarray.bin"
    names_path = pydata / "character_templates_pyarraynames.bin"
    character_names_path = pydata / "character_properties_pyarraynames.bin"
    for path in (array_path, names_path, character_names_path):
        require(path.is_file(), f"missing cache input: {path}")
    array_bytes, name_bytes, character_name_bytes = (
        array_path.read_bytes(), names_path.read_bytes(), character_names_path.read_bytes()
    )
    templates = parse_character_templates(array_bytes, name_bytes, character_name_bytes)
    matches = [row for row in templates if row.name == "GothicusCrypt_Ghosts"]
    require(len(matches) == 1, "expected one GothicusCrypt_Ghosts template")
    template = matches[0]
    require(template.property_ids == (35, 35, 35, 35, 37),
            f"Crypt ghost source slots changed: {template.property_ids!r}")

    mgp_path = cache / "data" / "3d" / "modules" / "crypt" / "mgp" / "crypt_straight_ns_01.mgp"
    require(mgp_path.is_file(), f"missing original Crypt MGP: {mgp_path}")
    mgp_bytes = mgp_path.read_bytes()
    root = ET.fromstring(mgp_bytes)
    ambushers = [obj.attrib for obj in root.iter("GameObject")
                 if obj.attrib.get("name", "").startswith("_prim_tmp_ambusher")]
    require(len(ambushers) == 4, f"expected four Crypt ambusher records, got {len(ambushers)}")
    require(all(row.get("char_template_pydata") == "Charater_Templates" and
                row.get("char_template") == template.name for row in ambushers),
            "Crypt ambushers no longer point to the cached runtime template")

    existing_report_path = RANDOM / "differential-validation.json"
    require(existing_report_path.is_file(), "missing existing Random source differential report")
    existing_report = json.loads(existing_report_path.read_text(encoding="utf-8"))
    random_evidence = [row for row in existing_report.get("function_evidence", [])
                       if row.get("elf_address") == "0x33ff90" and row.get("size") == 196]
    require(len(random_evidence) == 1 and
            random_evidence[0].get("sha256") ==
            "fc93b6feb58616e89b16d0bee33193af6b6cc36b6b29442fdca3e75c832ab665",
            "existing source-differential binding for Random::GetRandom changed")
    require(existing_report.get("core_comparisons") == 768 and
            existing_report.get("mismatches") == 0,
            "existing Random core differential summary changed")

    return {
        "template": {
            "name": template.name,
            "ordered_property_ids": list(template.property_ids),
            "alternative_count": len(template.property_ids),
        },
        "source_mgp": {
            "path": str(mgp_path.relative_to(cache)),
            "sha256": sha256(mgp_bytes),
            "ambusher_count": len(ambushers),
            "runtime_template_key": template.name,
        },
        "cache_sha256": {
            "template_array": sha256(array_bytes),
            "template_names": sha256(name_bytes),
            "character_table_names": sha256(character_name_bytes),
        },
        "existing_random_differential": {
            "report": str(existing_report_path.relative_to(REPO)),
            "original_random_elf_address": "0x33ff90",
            "original_function_bytes": 196,
            "core_comparisons": existing_report["core_comparisons"],
            "mismatches": existing_report["mismatches"],
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, default=REPO.parent / "cache" / "files")
    parser.add_argument("--seed", type=lambda value: int(value, 0), required=True,
                        help="explicit test-fixture ordinary seed; not a claimed original runtime seed")
    parser.add_argument("--compiler", default="g++", help="C++ compiler for adapter and test")
    parser.add_argument("--c-compiler", default="gcc", help="C compiler for the existing RNG kernel")
    parser.add_argument(
        "--output", type=Path,
        default=MODULE / "build" / "character-template-random" / "character-template-random-host.exe",
        help="host test executable path",
    )
    parser.add_argument("--report", type=Path, help="optional JSON evidence report path")
    args = parser.parse_args()
    require(0 <= args.seed <= 0xffffffff, "--seed must fit uint32")
    cache = args.cache.resolve()
    fixture = crypt_template_fixture(cache)

    cxx = shutil.which(args.compiler)
    cc = shutil.which(args.c_compiler)
    require(cxx is not None, f"C++ compiler not found: {args.compiler}")
    require(cc is not None, f"C compiler not found: {args.c_compiler}")
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    random_object = output.with_suffix(".random.o")
    c_command = [
        cc, "-std=c99", "-O2", "-Wall", "-Wextra", "-Werror", "-c",
        str(RANDOM / "random.c"), "-o", str(random_object),
    ]
    c_build = subprocess.run(c_command, cwd=REPO, text=True, capture_output=True)
    if c_build.returncode:
        raise RuntimeError(f"C RNG compile failed ({c_build.returncode}):\n{c_build.stdout}{c_build.stderr}")
    cxx_command = [
        cxx, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
        str(MODULE / "character_template_random.cpp"),
        str(HERE / "character_template_random.cpp"),
        str(random_object), "-o", str(output),
    ]
    cxx_build = subprocess.run(cxx_command, cwd=REPO, text=True, capture_output=True)
    if cxx_build.returncode:
        raise RuntimeError(f"C++ host compile failed ({cxx_build.returncode}):\n{cxx_build.stdout}{cxx_build.stderr}")
    tested = subprocess.run(
        [str(output), str(fixture["template"]["alternative_count"]), str(args.seed)],
        cwd=REPO, text=True, capture_output=True,
    )
    if tested.returncode:
        raise RuntimeError(f"host checks failed ({tested.returncode}):\n{tested.stdout}{tested.stderr}")

    report = {
        "status": "passed",
        "fixture_seed": args.seed,
        "fixture_seed_is_original_game_seed": False,
        "c_command": c_command,
        "cxx_command": cxx_command,
        "test_output": tested.stdout.strip(),
        "fixture": fixture,
        "scope": "uncached Character template slot selection using caller-owned ordinary RNG state; no startup seed or global stream ownership is reproduced",
    }
    report_path = args.report.resolve() if args.report else output.parent / "validation.json"
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(tested.stdout.strip())
    print(f"source template/MGP fixture checks passed ({fixture['source_mgp']['ambusher_count']} ambushers)")
    print(f"report: {report_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ET.ParseError, ValueError, RuntimeError) as exc:
        raise SystemExit(f"ERROR: {exc}") from exc
