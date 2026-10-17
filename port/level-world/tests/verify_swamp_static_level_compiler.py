#!/usr/bin/env python3
"""Minimal cache-backed verification of SWAMP row, modules, and entrypoint."""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import tempfile
import xml.etree.ElementTree as ET
import zipfile


ROOT = Path(__file__).resolve().parents[1]
COMPILER_PATH = ROOT / "tools" / "compile_swamp_static_level.py"
spec = importlib.util.spec_from_file_location("dh2_swamp_compiler_verify", COMPILER_PATH)
if spec is None or spec.loader is None:
    raise RuntimeError(f"cannot load compiler {COMPILER_PATH}")
compiler = importlib.util.module_from_spec(spec)
import sys
sys.modules[spec.name] = compiler
spec.loader.exec_module(compiler)


def require(condition: bool, detail: str) -> None:
    if not condition:
        raise AssertionError(detail)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True)
    args = parser.parse_args()

    with tempfile.TemporaryDirectory(prefix="dh2-swamp-compiler-test-") as temp:
        output = Path(temp) / "bundle"
        manifest = compiler.compile_swamp(args.cache, output)
        require(manifest["level_list"] == {
            "row": 41, "name": "SWAMP", "level_file": "001_swamp.mlx"},
            "LevelList row 41 no longer selects static SWAMP MLX")

        with zipfile.ZipFile(args.cache) as archive:
            index = compiler._load_source_index(archive)
            levels_raw, _ = compiler._read_source(archive, index,
                "data/pydata/levels_pyarray.bin", limit=8 * 1024 * 1024)
            names_raw, _ = compiler._read_source(archive, index,
                "data/pydata/levels_pyarraynames.bin", limit=8 * 1024 * 1024)
            catalogue = compiler.load_catalogue_module().decode_catalogue(levels_raw, names_raw)
            declaration = catalogue.levels[41]
            require(declaration.name == "SWAMP" and declaration.level_file == "001_swamp.mlx",
                    "decoded LevelList row differs from compiled manifest")

            mlx_raw, _ = compiler._read_source(archive, index,
                "data/scene/001_swamp.mlx", limit=compiler.MAX_XML_BYTES)
            mlx = compiler.parse_xml(mlx_raw, "Level", "001_swamp.mlx")
            modules = [row for row in mlx.findall("GameObject") if row.get("gametype") == "Module"]

            dwld = (output / "001_swamp.dwld").read_bytes()
            magic, version, room_count, *entry_zero = struct.unpack_from("<4sII3f", dwld)
            require((magic, version, room_count) == (b"DWLD", 1, len(modules)),
                    "DWLD header does not match source MLX module count")
            require(len(dwld) == 24 + 128 * len(modules), "DWLD v1 byte length changed")
            for module_index, row in enumerate(modules):
                offset = 24 + module_index * 128
                node_id, *tail = struct.unpack_from("<112s3fI", dwld, offset)
                expected_node = (row.get("xrefobject", "") + "-node").encode("utf-8")
                expected_position = compiler.vector(row.get("position"), "MLX module position")
                require(node_id.split(b"\0", 1)[0] == expected_node,
                        f"DWLD module {module_index} node ID differs from MLX")
                require(tail[:3] == [struct.unpack("<f", struct.pack("<f", x))[0]
                                     for x in expected_position] and tail[3] == 0,
                        f"DWLD module {module_index} position/reserved field differs from MLX")

            spwn = (output / "001_swamp.spwn").read_bytes()
            spawn_magic, spawn_version, spawn_count, reserved = struct.unpack_from("<4sIII", spwn)
            require((spawn_magic, spawn_version, reserved) == (b"SPWN", 1, 0),
                    "SPWN header is not v1")
            require(len(spwn) == 16 + 144 * spawn_count, "SPWN v1 byte length changed")
            entry_zero_rows = []
            for index_in_spawn in range(spawn_count):
                row = struct.unpack_from("<iI64s18f", spwn, 16 + index_in_spawn * 144)
                if row[0] == 0:
                    entry_zero_rows.append(row)
            require(len(entry_zero_rows) == 1, "compiled SPWN must expose entrypoint zero once")
            expected_start = manifest["entrypoint_zero"]["world_position"]
            require(tuple(entry_zero) == tuple(struct.unpack("<f", struct.pack("<f", x))[0]
                for x in expected_start), "DWLD start position differs from compiled entrypoint zero")
            require(tuple(entry_zero_rows[0][12:15]) == tuple(entry_zero),
                    "SPWN world position for entrypoint zero differs from DWLD header")
            require(manifest["selectable_entrypoints"] == [0, 3, 13],
                    "conditional/duplicate Swamp entrypoints became selectable")
            require({row["entrypoint_id"] for row in manifest["unsupported_entrypoints"]} == {1, 4},
                    "conditional/duplicate source spawn IDs were not reported unsupported")

            # Every copied source asset is byte-preserved from the pinned archive.
            for asset in manifest["source_assets"]:
                copied = (output / "assets" / Path(*asset["path"].split("/"))).read_bytes()
                require(hashlib.sha256(copied).hexdigest() == asset["sha256"],
                        f"source asset was altered: {asset['path']}")

        print(json.dumps({"validation": "PASS", "row": 41, "modules": len(modules),
            "spawn_points": manifest["spawn_point_count"],
            "selectable_entrypoints": manifest["selectable_entrypoints"],
            "unsupported_entrypoint_rows": len(manifest["unsupported_entrypoints"]),
            "preserved_assets": len(manifest["source_assets"]),
            "dwld_sha256": manifest["generated"]["001_swamp.dwld"]["sha256"]},
            sort_keys=True))


if __name__ == "__main__":
    main()
