#!/usr/bin/env python3
"""Cross-check the Crypt ambusher template projection against cache/MGP, then run C++ host tests."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import xml.etree.ElementTree as ET

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPO = HERE.parents[2]
sys.path.insert(0, str(REPO / "port" / "actor-spawn-runtime"))

from actor_spawn_runtime import (  # noqa: E402
    parse_character_templates,
    parse_name_file,
    parse_name_tables,
)


EXPECTED_SLOTS = (35, 35, 35, 35, 37)
EXPECTED_PROPERTIES = {35: "Crypt_Ghost", 37: "Crypt_Ghost_RE"}
AMBUSHER_NAMES = tuple(f"_prim_tmp_ambusher{i:02d}" for i in range(1, 5))
MAX_MGP_BYTES = 8 * 1024 * 1024


def require(condition: bool, message: str) -> None:
    if not condition:
        raise RuntimeError(message)


def digest(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def cache_fixture(cache: Path) -> dict:
    pydata = cache / "data" / "pydata"
    paths = {
        "templates": pydata / "character_templates_pyarray.bin",
        "template_names": pydata / "character_templates_pyarraynames.bin",
        "property_names": pydata / "character_properties_pyarraynames.bin",
        "property_rows": pydata / "character_properties_pyarray.bin",
        "property_fields": pydata / "character_properties_pystructnames.bin",
        "class_names": pydata / "character_classes_pyarraynames.bin",
    }
    for path in paths.values():
        require(path.is_file(), f"missing cache input: {path}")
    raw = {key: path.read_bytes() for key, path in paths.items()}

    property_tables = parse_name_tables(raw["property_names"])
    property_fields_tables = parse_name_tables(raw["property_fields"])
    require(len(property_tables) == 3,
            "Character properties names no longer contain the three known tables")
    require(len(property_fields_tables) == 1,
            "CharacterProperties field-name table count changed")
    property_names = property_tables[0]
    fields = property_fields_tables[0]
    require(len(fields) == 224, f"expected 224 CharacterProperties fields, got {len(fields)}")
    require(fields[26] == "ClassID", f"ClassID field moved: index 26 is {fields[26]!r}")

    templates = parse_character_templates(
        raw["templates"], raw["template_names"], raw["property_names"]
    )
    matches = [entry for entry in templates if entry.name == "GothicusCrypt_Ghosts"]
    require(len(matches) == 1, "GothicusCrypt_Ghosts must be a unique source template")
    require(matches[0].property_ids == EXPECTED_SLOTS,
            f"authored variant slots changed: {matches[0].property_ids!r}")

    row_data = raw["property_rows"]
    require(len(row_data) >= 4, "CharacterProperties rows are truncated")
    row_count = struct.unpack_from("<I", row_data)[0]
    require(row_count == len(property_names),
            "CharacterProperties row count differs from CharacterTable name count")
    row_stride = 4 * len(fields)
    character_rows_end = 4 + row_count * row_stride
    require(len(row_data) >= character_rows_end,
            "CharacterProperties source rows are truncated")
    class_names = parse_name_file(raw["class_names"])
    projections = {}
    for property_id, property_name in EXPECTED_PROPERTIES.items():
        require(property_id < len(property_names), "expected property ID is outside CharacterTable")
        require(property_names[property_id] == property_name,
                f"CharacterTable[{property_id}] changed to {property_names[property_id]!r}")
        values = struct.unpack_from(f"<{len(fields)}i", row_data, 4 + property_id * row_stride)
        class_id = values[26]
        require(class_id == 18, f"{property_name} ClassID changed from 18 to {class_id}")
        require(class_id < len(class_names) and class_names[class_id] == "BaseMonster",
                f"ClassTable[{class_id}] no longer names BaseMonster")
        projections[property_name] = {
            "property_id": property_id,
            "class_id": class_id,
            "class_name": class_names[class_id],
            "model_file_id": values[3],
            "animation_table_id": values[2],
            "ai_id": values[1],
        }

    mgp_path = cache / "data" / "3d" / "modules" / "crypt" / "mgp" / "crypt_straight_ns_01.mgp"
    require(mgp_path.is_file(), f"missing original Crypt MGP: {mgp_path}")
    mgp_bytes = mgp_path.read_bytes()
    require(0 < len(mgp_bytes) <= MAX_MGP_BYTES, "Crypt MGP size outside validation bound")
    root = ET.fromstring(mgp_bytes)
    require(root.tag == "Module", f"Crypt MGP root changed: {root.tag!r}")
    source_rows = [obj.attrib for obj in root.iter("GameObject")
                   if obj.attrib.get("name") in AMBUSHER_NAMES]
    by_name = {row.get("name"): row for row in source_rows}
    require(len(source_rows) == len(AMBUSHER_NAMES) and len(by_name) == len(AMBUSHER_NAMES),
            "source MGP must have one record for each of the four ambushers")
    for name in AMBUSHER_NAMES:
        row = by_name[name]
        require(row.get("gametype") == "Character", f"{name} no longer has gametype Character")
        require(row.get("char_template_pydata") == "Charater_Templates",
                f"{name} runtime template data class changed")
        require(row.get("char_template") == "GothicusCrypt_Ghosts",
                f"{name} runtime template key changed")
        require(row.get("_templateName") == "MonsterCommonType1",
                f"{name} editor template metadata changed")
        require(not row.get("charpropsname"),
                f"{name} now has explicit charpropsname; template precedence requires review")
    return {
        "cache_inputs": {key: {"path": str(paths[key].relative_to(cache)),
                               "sha256": digest(value), "bytes": len(value)}
                         for key, value in raw.items()},
        "template": {
            "name": matches[0].name,
            "ordered_property_ids": list(matches[0].property_ids),
            "selection_is_external": True,
        },
        "projections": projections,
        "character_properties_unparsed_suffix_bytes": len(row_data) - character_rows_end,
        "source_mgp": {
            "path": str(mgp_path.relative_to(cache)),
            "sha256": digest(mgp_bytes),
            "bytes": len(mgp_bytes),
            "objects": [{
                "name": name,
                "editor_template": by_name[name]["_templateName"],
                "template_data_class": by_name[name]["char_template_pydata"],
                "template_name": by_name[name]["char_template"],
                "explicit_property": by_name[name].get("charpropsname", ""),
            } for name in AMBUSHER_NAMES],
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, default=REPO.parent / "cache" / "files")
    parser.add_argument("--compiler", default="g++", help="C++17 host compiler (default: g++)")
    parser.add_argument(
        "--output", type=Path,
        default=MODULE / "build" / "character-template-factory" / "character-template-factory-host.exe",
        help="host test executable path",
    )
    parser.add_argument("--report", type=Path, help="optional JSON evidence report path")
    args = parser.parse_args()

    cache = args.cache.resolve()
    fixture_report = cache_fixture(cache)
    compiler = shutil.which(args.compiler)
    require(compiler is not None, f"C++ compiler not found: {args.compiler}")
    output = args.output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [
        compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
        str(MODULE / "character_template_factory.cpp"),
        str(HERE / "character_template_factory.cpp"),
        "-o", str(output),
    ]
    built = subprocess.run(command, cwd=REPO, text=True, capture_output=True)
    if built.returncode:
        raise RuntimeError(f"host compile failed ({built.returncode}):\n{built.stdout}{built.stderr}")
    tested = subprocess.run([str(output)], cwd=REPO, text=True, capture_output=True)
    if tested.returncode:
        raise RuntimeError(f"host checks failed ({tested.returncode}):\n{tested.stdout}{tested.stderr}")

    report = {
        "status": "passed",
        "compiler": compiler,
        "command": command,
        "test_output": tested.stdout.strip(),
        "fixture": fixture_report,
        "scope": "cache/MGP source validation plus host resolver tests; not a native game runtime integration",
    }
    report_path = args.report.resolve() if args.report else output.parent / "validation.json"
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"{tested.stdout.strip()}\ncache/MGP fixture checks passed ({len(fixture_report['source_mgp']['objects'])} ambushers)")
    print(f"report: {report_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ET.ParseError, RuntimeError, ValueError) as exc:
        raise SystemExit(f"ERROR: {exc}") from exc
