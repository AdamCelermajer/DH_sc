#!/usr/bin/env python3
"""Verify recovered Zone local-box arithmetic against the original cache."""

import argparse
import math
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET


HERE = Path(__file__).resolve().parent
PORT = HERE.parent.parent
REPO = HERE.parents[2]


def close(actual: float, expected: float) -> bool:
    return math.isclose(actual, expected, rel_tol=0.0, abs_tol=1e-4)


def validate_cache(cache: Path) -> None:
    level = ET.parse(cache / "data" / "scene" / "005_infectedvillage.mlx").getroot()
    modules = [node for node in level.findall("GameObject")
               if node.attrib.get("name") == "_module_infectedvillage_01_001"]
    if len(modules) != 1:
        raise SystemExit(f"expected one module 01 placement, found {len(modules)}")
    module = modules[0]
    if not all(close(float(a), b) for a, b in zip(
            module.attrib["position"].split(","), (-3448.5, 3000.0, 0.0))):
        raise SystemExit("module placement differs from the audited source values")
    if not all(close(float(a), 1.0) for a in module.attrib["scale"].split(",")):
        raise SystemExit("module 01 scale is no longer unit scale")
    if not all(close(float(a), 0.0) for a in module.attrib["rotation"].split(",")):
        raise SystemExit("module 01 rotation is no longer zero")

    relative = module.attrib["mgp"].replace("data/iphone/", "data/").lower()
    objects = ET.parse(cache / relative).getroot().findall("GameObject")
    zones = [node.attrib for node in objects
             if node.attrib.get("name") == "_prim_TriggerZone_ambush"]
    if len(zones) != 1:
        raise SystemExit(f"expected one module 01 Ambush Zone, found {len(zones)}")
    zone = zones[0]
    if zone.get("gametype") != "TriggerZone" or "dimensions" in zone:
        raise SystemExit("Ambush Zone type/dimension override changed")
    scale = tuple(float(a) for a in zone["scale"].split(","))
    expected_scale = (1.92682, 1.92682, 1.0)
    if not all(close(a, b) for a, b in zip(scale, expected_scale)):
        raise SystemExit(f"unexpected Ambush Zone scale: {scale}")
    local = tuple(float(a) for a in zone["position"].split(","))
    world = tuple(float(a) + float(b) for a, b in
                  zip(module.attrib["position"].split(","), local))
    expected_world = (2545.3, 1110.83, 1313.39)
    if not all(close(a, b) for a, b in zip(world, expected_world)):
        raise SystemExit(f"unexpected translation-only world center: {world}")


def compiler() -> str:
    requested = os.environ.get("CXX", "c++")
    found = shutil.which(requested)
    if found:
        return found
    if Path(requested).is_file():
        return str(Path(requested).resolve())
    raise SystemExit(f"C++ compiler not found: {requested}; set CXX to override")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path,
                        default=REPO.parent / "cache" / "files")
    args = parser.parse_args()
    cache = args.cache.resolve()
    validate_cache(cache)
    with tempfile.TemporaryDirectory(prefix="dh2-zone-geometry-") as temp:
        executable = Path(temp) / "zone_geometry_test"
        command = [compiler(), "-std=c++11", "-O2", "-Wall", "-Wextra",
                   "-Werror", "-fno-exceptions", "-fno-rtti",
                   str(HERE / "zone_geometry_test.cpp"),
                   str(HERE.parent / "zone_geometry.cpp"), "-o", str(executable)]
        subprocess.run(command, check=True)
        subprocess.run([str(executable)], check=True)
    print("verified cache-backed Zone local-box construction")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
