#!/usr/bin/env python3
"""Build and run the bounded host scheduler against the owner's SWAMP cache."""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET


HERE = Path(__file__).resolve().parent
RUNTIME = HERE.parent
NATIVE = RUNTIME.parent / "pydata-scripts" / "native"


def compiler() -> str:
    requested = os.environ.get("CXX", "c++")
    found = shutil.which(requested)
    if found:
        return found
    if Path(requested).is_file():
        return str(Path(requested).resolve())
    raise SystemExit(f"C++ compiler not found: {requested}; set CXX to override")


def source_seed(cache: Path, path: Path) -> tuple[int, int]:
    level_file = cache / "data" / "scene" / "001_swamp.mlx"
    level = ET.parse(level_file).getroot()
    modules = [obj for obj in level.findall("GameObject")
               if obj.attrib.get("gametype") == "Module"]
    if len(modules) != 9:
        raise SystemExit(f"expected 9 original SWAMP modules; found {len(modules)}")

    objects: list[dict[str, str]] = []
    trigger = None
    for module in modules:
        relative = module.attrib["mgp"].replace("data/iphone/", "data/").lower()
        mgp = ET.parse(cache / relative).getroot()
        for obj in mgp.findall("GameObject"):
            attrs = obj.attrib
            objects.append(attrs)
            if attrs.get("name") == "_prim_TriggerZone_LizManIntro":
                if trigger is not None:
                    raise SystemExit("duplicate Lizard intro trigger in source MGPs")
                trigger = attrs

    if len(objects) != 148:
        raise SystemExit(f"expected 148 original MGP records; found {len(objects)}")
    if trigger is None:
        raise SystemExit("original Lizard intro TriggerZone record is missing")
    if (trigger.get("gametype") != "TriggerZone" or
            trigger.get("script") != "LizardMan_Intro" or
            trigger.get("triggercount") != "1" or
            trigger.get("triggerdelay") != "0"):
        raise SystemExit(f"unexpected original trigger properties: {trigger}")

    found: dict[str, dict[str, str]] = {}
    for name in ("_prim_Monster_LizManIntro1",
                 "_prim_Monster_LizManIntro2",
                 "_prim_Waypoint_NewCamSpot"):
        matches = [obj for obj in objects if obj.get("name") == name]
        if len(matches) != 1:
            raise SystemExit(f"expected exactly one preloaded MGP object {name}")
        found[name] = matches[0]
    for name in ("_prim_Monster_LizManIntro1", "_prim_Monster_LizManIntro2"):
        attrs = found[name]
        if (attrs.get("gametype") != "Character" or
                attrs.get("ai_state") != "Limbus" or
                attrs.get("auto_spawn") != "0"):
            raise SystemExit(f"unexpected original preloaded actor record: {attrs}")
    if found["_prim_Waypoint_NewCamSpot"].get("gametype") != "Dummy":
        raise SystemExit("intro camera waypoint is not the expected Dummy record")

    with path.open("w", encoding="utf-8", newline="\n") as output:
        output.write("\t".join(("TRIGGER", trigger["name"], trigger["script"],
                                  trigger["triggercount"], trigger["triggerdelay"])) + "\n")
        for attrs in objects:
            output.write("\t".join(("OBJECT", attrs.get("name", ""),
                                      attrs.get("gametype", ""),
                                      attrs.get("ai_state", ""),
                                      attrs.get("auto_spawn", "0"))) + "\n")
    return len(modules), len(objects)


def run(command: list[str]) -> None:
    print("+", subprocess.list2cmdline(command), flush=True)
    subprocess.run(command, check=True)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--cache",
        type=Path,
        default=HERE.parents[3] / "cache" / "files",
        help="cache/files directory containing the recovered game data",
    )
    args = parser.parse_args()
    cache = args.cache.resolve()
    data = cache / "data" / "pydata"
    common_names = data / "scripts_pyscriptnames.bin"
    common_programs = data / "scripts_pyscripts.bin"
    swamp_names = data / "scripts" / "001_swamp_pyscriptnames.bin"
    swamp_programs = data / "scripts" / "001_swamp_pyscripts.bin"
    required = (common_names, common_programs, swamp_names, swamp_programs,
                cache / "data" / "scene" / "001_swamp.mlx")
    for path in required:
        if not path.is_file():
            raise SystemExit(f"missing original cache file: {path}")

    cxx = compiler()
    with tempfile.TemporaryDirectory(prefix="dh2-script-runtime-") as temp:
        temp_dir = Path(temp)
        seed_path = temp_dir / "swamp-source-seed.tsv"
        modules, records = source_seed(cache, seed_path)
        executable = temp_dir / "script_runtime_test"
        run([
            cxx, "-std=c++11", "-Wall", "-Wextra", "-Werror",
            "-fno-exceptions", "-fno-rtti",
            str(HERE / "script_runtime_test.cpp"),
            str(RUNTIME / "script_runtime.cpp"),
            str(NATIVE / "pydata_scripts.cpp"),
            "-o", str(executable),
        ])
        run([
            str(executable), str(common_names), str(common_programs),
            str(swamp_names), str(swamp_programs), str(seed_path),
        ])
        print(f"verified {modules} original modules and {records} source records")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
