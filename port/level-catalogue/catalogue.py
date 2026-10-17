#!/usr/bin/env python3
"""Bounded host-side decoder and validator for DH2 level catalogues.

This module decodes only the recovered LevelList and FastTravelList pydata
tables, plus the authored exit records used to reference those tables. It does
not load a scene or perform a level transition.
"""
from __future__ import annotations

import argparse
from dataclasses import dataclass
import json
from pathlib import Path, PurePosixPath
import struct
import sys
import xml.etree.ElementTree as ET


EXPECTED_FAST_TRAVEL_COUNT = 33
EXPECTED_LEVEL_COUNT = 51
MAX_RECORD_COUNT = 4096
MAX_STRING_BYTES = 4096
MAX_BINARY_BYTES = 8 * 1024 * 1024
MAX_XML_BYTES = 16 * 1024 * 1024


class CatalogueError(ValueError):
    """Invalid or unsafe cache catalogue input."""


class Reader:
    """Little-endian reader whose every read is bounded by supplied bytes."""

    def __init__(self, data: bytes, label: str):
        if len(data) > MAX_BINARY_BYTES:
            raise CatalogueError(f"{label}: input exceeds {MAX_BINARY_BYTES} bytes")
        self.data = memoryview(data)
        self.label = label
        self.offset = 0

    def read(self, size: int) -> bytes:
        if size < 0 or size > len(self.data) - self.offset:
            raise CatalogueError(
                f"{self.label}: truncated at byte {self.offset} (need {size} bytes)"
            )
        start = self.offset
        self.offset += size
        return self.data[start:self.offset].tobytes()

    def u32(self) -> int:
        return struct.unpack("<I", self.read(4))[0]

    def i32(self) -> int:
        return struct.unpack("<i", self.read(4))[0]

    def boolean(self) -> bool:
        value = self.read(1)[0]
        if value not in (0, 1):
            raise CatalogueError(f"{self.label}: invalid bool {value} at byte {self.offset - 1}")
        return bool(value)

    def string(self) -> str:
        length = self.u32()
        if length > MAX_STRING_BYTES:
            raise CatalogueError(
                f"{self.label}: string length {length} exceeds {MAX_STRING_BYTES}"
            )
        raw = self.read(length)
        try:
            value = raw.decode("utf-8", errors="strict")
        except UnicodeDecodeError as exc:
            raise CatalogueError(f"{self.label}: invalid UTF-8 string at byte {self.offset - length}") from exc
        if "\x00" in value:
            raise CatalogueError(f"{self.label}: embedded NUL in string")
        return value

    def count(self, expected: int, table: str) -> int:
        value = self.u32()
        if value > MAX_RECORD_COUNT:
            raise CatalogueError(f"{self.label}: {table} count {value} exceeds safety limit")
        if value != expected:
            raise CatalogueError(f"{self.label}: expected {expected} {table} records, found {value}")
        return value

    def finish(self) -> None:
        if self.offset != len(self.data):
            raise CatalogueError(
                f"{self.label}: {len(self.data) - self.offset} trailing bytes at {self.offset}"
            )


def _read_bounded_file(path: Path, limit: int, label: str) -> bytes:
    """Read at most ``limit`` bytes, rejecting oversized cache sources first."""
    try:
        size = path.stat().st_size
    except OSError as exc:
        raise CatalogueError(f"cannot stat {label} {path}: {exc}") from exc
    if size > limit:
        raise CatalogueError(f"{label}: file size {size} exceeds {limit} bytes")

    # The capped read is also needed after stat: the file could grow between
    # the size check and open/read.  Never materialize an unbounded source.
    try:
        with path.open("rb") as stream:
            data = stream.read(limit + 1)
    except OSError as exc:
        raise CatalogueError(f"cannot read {label} {path}: {exc}") from exc
    if len(data) > limit:
        raise CatalogueError(f"{label}: file exceeds {limit} bytes")
    return data


@dataclass(frozen=True)
class FastTravelDestination:
    name: str
    description_id: int
    entrypoint_id: int
    level_name: str
    location_type: int
    string_id: int


@dataclass(frozen=True)
class LevelDeclaration:
    name: str
    dbg_is_stable: bool
    dynamic_bus_routing: str
    hub: int
    is_random: bool
    level_description: int
    level_file: str
    level_name_id: int
    level_state: int
    map_name: int
    monster_lvl_max: int
    monster_lvl_max_hard: int
    monster_lvl_max_nightmare: int
    monster_lvl_min: int
    monster_lvl_min_hard: int
    monster_lvl_min_nightmare: int

    @property
    def file_kind(self) -> str:
        return "procedural_rules" if self.level_file.endswith(".rule.xml") else "static_mlx"


@dataclass(frozen=True)
class Catalogue:
    fast_travel: tuple[FastTravelDestination, ...]
    levels: tuple[LevelDeclaration, ...]

    @property
    def fast_travel_by_name(self) -> dict[str, FastTravelDestination]:
        return {row.name: row for row in self.fast_travel}

    @property
    def levels_by_name(self) -> dict[str, LevelDeclaration]:
        return {row.name: row for row in self.levels}


@dataclass(frozen=True)
class ExitTarget:
    level_name: str
    entrypoint_id: int
    declaration: LevelDeclaration


@dataclass(frozen=True)
class ExitUnlock:
    fast_travel_name: str
    destination: FastTravelDestination


@dataclass(frozen=True)
class SwampExit:
    module_index: int
    module_path: str
    record_index: int
    object_name: str
    target: ExitTarget
    activation_condition: str | None
    unlock: ExitUnlock | None


def decode_names(data: bytes) -> tuple[tuple[str, ...], tuple[str, ...]]:
    """Read FastTravelList then LevelList names, in native registration order."""
    reader = Reader(data, "levels_pyarraynames.bin")
    tables: list[tuple[str, ...]] = []
    for label, expected in (("FastTravelList names", EXPECTED_FAST_TRAVEL_COUNT),
                            ("LevelList names", EXPECTED_LEVEL_COUNT)):
        count = reader.count(expected, label)
        names = tuple(reader.string() for _ in range(count))
        if any(not value for value in names):
            raise CatalogueError(f"{label}: empty member name")
        if len(set(names)) != len(names):
            raise CatalogueError(f"{label}: duplicate member name")
        tables.append(names)
    reader.finish()
    return tables[0], tables[1]


def decode_catalogue(data: bytes, names_data: bytes) -> Catalogue:
    """Decode both arrays using Structs::*::read and Arrays::*::read layouts."""
    fast_names, level_names = decode_names(names_data)
    reader = Reader(data, "levels_pyarray.bin")

    fast_count = reader.count(EXPECTED_FAST_TRAVEL_COUNT, "FastTravelList")
    fast_travel = tuple(
        FastTravelDestination(
            name=fast_names[index],
            description_id=reader.i32(),
            entrypoint_id=reader.i32(),
            level_name=reader.string(),
            location_type=reader.i32(),
            string_id=reader.i32(),
        )
        for index in range(fast_count)
    )

    level_count = reader.count(EXPECTED_LEVEL_COUNT, "LevelList")
    levels: list[LevelDeclaration] = []
    for index in range(level_count):
        # Structs::LevelDeclaration::read: bool, string, int, bool, int,
        # string, then nine int fields; order follows recovered struct names.
        stable = reader.boolean()
        routing = reader.string()
        hub = reader.i32()
        random = reader.boolean()
        description = reader.i32()
        filename = reader.string()
        values = tuple(reader.i32() for _ in range(9))
        levels.append(LevelDeclaration(
            name=level_names[index],
            dbg_is_stable=stable,
            dynamic_bus_routing=routing,
            hub=hub,
            is_random=random,
            level_description=description,
            level_file=filename,
            level_name_id=values[0],
            level_state=values[1],
            map_name=values[2],
            monster_lvl_max=values[3],
            monster_lvl_max_hard=values[4],
            monster_lvl_max_nightmare=values[5],
            monster_lvl_min=values[6],
            monster_lvl_min_hard=values[7],
            monster_lvl_min_nightmare=values[8],
        ))
    reader.finish()

    catalogue = Catalogue(fast_travel, tuple(levels))
    validate_catalogue(catalogue)
    return catalogue


def safe_level_path(level_file: str) -> PurePosixPath:
    """Validate LevelFile as a single cache-relative `data/scene` path."""
    normalized = level_file.replace("\\", "/")
    path = PurePosixPath(normalized)
    if (not normalized or path.is_absolute() or "\x00" in normalized
            or any(part in ("", ".", "..") for part in normalized.split("/"))):
        raise CatalogueError(f"unsafe LevelFile path {level_file!r}")
    if len(path.parts) != 1:
        raise CatalogueError(f"LevelFile must be a scene filename, found {level_file!r}")
    if not (normalized.endswith(".mlx") or normalized.endswith(".rule.xml")):
        raise CatalogueError(f"unsupported LevelFile format {level_file!r}")
    return path


def validate_catalogue(catalogue: Catalogue) -> None:
    """Check cross-table names and bounds independent of local file presence."""
    if len(catalogue.fast_travel) != EXPECTED_FAST_TRAVEL_COUNT:
        raise CatalogueError(f"expected {EXPECTED_FAST_TRAVEL_COUNT} FastTravelList rows")
    if len(catalogue.levels) != EXPECTED_LEVEL_COUNT:
        raise CatalogueError(f"expected {EXPECTED_LEVEL_COUNT} LevelList rows")
    fast_names = [row.name for row in catalogue.fast_travel]
    level_names = [row.name for row in catalogue.levels]
    if len(set(fast_names)) != len(fast_names):
        raise CatalogueError("duplicate FastTravelList member name")
    if len(set(level_names)) != len(level_names):
        raise CatalogueError("duplicate LevelList member name")
    levels = catalogue.levels_by_name
    for row in catalogue.levels:
        safe_level_path(row.level_file)
        if not row.name or not row.dynamic_bus_routing:
            raise CatalogueError(f"LevelList {row.name!r}: required name/routing field is empty")
    for row in catalogue.fast_travel:
        if not row.name or row.level_name not in levels:
            raise CatalogueError(
                f"FastTravelList {row.name!r}: unknown destination LevelName {row.level_name!r}"
            )
        if row.entrypoint_id < 0:
            raise CatalogueError(f"FastTravelList {row.name!r}: negative entrypoint id")


def _cache_path(cache_root: Path, relative: str) -> Path:
    """Resolve a normalized `data/...` cache reference without escaping root."""
    value = relative.replace("\\", "/")
    posix = PurePosixPath(value)
    if posix.is_absolute() or any(part in ("", ".", "..") for part in value.split("/")):
        raise CatalogueError(f"unsafe cache path {relative!r}")
    root = cache_root.resolve()
    result = (root.joinpath(*posix.parts)).resolve()
    if result != root and root not in result.parents:
        raise CatalogueError(f"cache path escapes root: {relative!r}")
    return result


def validate_cache_files(catalogue: Catalogue, cache_root: Path) -> None:
    for row in catalogue.levels:
        scene_path = _cache_path(cache_root, f"data/scene/{safe_level_path(row.level_file).name}")
        if not scene_path.is_file():
            raise CatalogueError(f"LevelList {row.name}: missing scene file {scene_path}")


def _normalized_asset_path(value: str) -> str:
    value = value.replace("\\", "/")
    if value.startswith("data/iphone/"):
        value = "data/" + value[len("data/iphone/"):]
    return value


def parse_swamp_exits(cache_root: Path, catalogue: Catalogue) -> tuple[SwampExit, ...]:
    """Extract SWAMP's authored exit objects and resolve target/unlock separately."""
    scene_path = _cache_path(cache_root, "data/scene/001_swamp.mlx")
    try:
        root = ET.fromstring(_read_bounded_file(scene_path, MAX_XML_BYTES, "SWAMP scene XML"))
    except ET.ParseError as exc:
        raise CatalogueError(f"cannot parse SWAMP scene {scene_path}: {exc}") from exc
    modules = [obj for obj in root.findall("GameObject") if obj.get("gametype") == "Module"]
    levels = catalogue.levels_by_name
    fast_travel = catalogue.fast_travel_by_name
    exits: list[SwampExit] = []
    for module_index, module in enumerate(modules):
        mgp = module.get("mgp")
        if not mgp:
            raise CatalogueError(f"SWAMP module {module_index}: missing mgp path")
        module_path = _normalized_asset_path(mgp)
        source = _cache_path(cache_root, module_path)
        try:
            objects = ET.fromstring(
                _read_bounded_file(source, MAX_XML_BYTES, "SWAMP module XML")
            ).findall("GameObject")
        except ET.ParseError as exc:
            raise CatalogueError(f"cannot parse SWAMP module {source}: {exc}") from exc
        for record_index, obj in enumerate(objects):
            if obj.get("gametype") != "TriggerZoneExitLevel":
                continue
            level_name = obj.get("levelName")
            if not level_name or level_name not in levels:
                raise CatalogueError(
                    f"{source} record {record_index}: unknown exit level {level_name!r}"
                )
            try:
                entrypoint = int(obj.attrib["entrypointID"], 10)
            except (KeyError, ValueError) as exc:
                raise CatalogueError(
                    f"{source} record {record_index}: invalid/missing entrypointID"
                ) from exc
            if entrypoint < 0:
                raise CatalogueError(f"{source} record {record_index}: negative entrypointID")
            declaration = levels[level_name]
            fast_name = obj.get("fasttravel", "Invalid")
            unlock = None
            if fast_name != "Invalid":
                destination = fast_travel.get(fast_name)
                if destination is None:
                    raise CatalogueError(
                        f"{source} record {record_index}: unknown fasttravel unlock {fast_name!r}"
                    )
                unlock = ExitUnlock(fast_name, destination)
            exits.append(SwampExit(
                module_index=module_index,
                module_path=module_path,
                record_index=record_index,
                object_name=obj.get("name", ""),
                target=ExitTarget(level_name, entrypoint, declaration),
                activation_condition=obj.get("activate_cond"),
                unlock=unlock,
            ))
    return tuple(exits)


def validate_swamp_exits(exits: tuple[SwampExit, ...]) -> None:
    expected = {
        (4, "data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp", 13):
            ("SWAMP_02", 0, "IsAfter_Gothicus2Survivors", None, "procedural_rules"),
        (6, "data/3d/modules/swamp/mgp/deadend_brdwalk_w_00.mgp", 1):
            ("SWAMP_CAVE_WITCH_A", 0, None, "a01_SWAMP_CAMP", "procedural_rules"),
        (7, "data/3d/modules/swamp/mgp/bossroom_ruins_ns_.mgp", 7):
            ("DARKWOOD", 0, "IsAfter_Swamp_Escape", "a01_SWAMP_CAMP", "static_mlx"),
    }
    actual = {
        (row.module_index, row.module_path, row.record_index): (
            row.target.level_name, row.target.entrypoint_id, row.activation_condition,
            row.unlock.fast_travel_name if row.unlock else None, row.target.declaration.file_kind,
        )
        for row in exits
    }
    if actual != expected:
        raise CatalogueError(f"SWAMP exit records differ from recovered authored data: {actual!r}")


def _report(catalogue: Catalogue, exits: tuple[SwampExit, ...]) -> dict[str, object]:
    return {
        "scope": "catalogue and authored exit validation only; no level loading performed",
        "fast_travel_count": len(catalogue.fast_travel),
        "level_count": len(catalogue.levels),
        "level_file_kinds": {
            "static_mlx": sum(row.file_kind == "static_mlx" for row in catalogue.levels),
            "procedural_rules": sum(row.file_kind == "procedural_rules" for row in catalogue.levels),
        },
        "swamp_exits": [
            {
                "module_index": row.module_index,
                "module_path": row.module_path,
                "record_index": row.record_index,
                "object_name": row.object_name,
                "target": {
                    "level_name": row.target.level_name,
                    "level_file": row.target.declaration.level_file,
                    "file_kind": row.target.declaration.file_kind,
                    "entrypoint_id": row.target.entrypoint_id,
                },
                "activation_condition": row.activation_condition,
                "unlock": None if row.unlock is None else {
                    "fast_travel_name": row.unlock.fast_travel_name,
                    "destination_level_name": row.unlock.destination.level_name,
                    "destination_entrypoint_id": row.unlock.destination.entrypoint_id,
                },
            }
            for row in exits
        ],
    }


def validate_cache(cache_root: Path) -> dict[str, object]:
    pydata = _cache_path(cache_root, "data/pydata")
    catalogue = decode_catalogue(
        _read_bounded_file(pydata / "levels_pyarray.bin", MAX_BINARY_BYTES,
                           "levels_pyarray.bin"),
        _read_bounded_file(pydata / "levels_pyarraynames.bin", MAX_BINARY_BYTES,
                           "levels_pyarraynames.bin"),
    )
    validate_cache_files(catalogue, cache_root)
    exits = parse_swamp_exits(cache_root, catalogue)
    validate_swamp_exits(exits)
    for row in exits:
        _cache_path(cache_root, f"data/scene/{row.target.declaration.level_file}")
    return _report(catalogue, exits)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache-root", type=Path, required=True,
                        help="unpacked cache root containing data/pydata and data/scene")
    args = parser.parse_args(argv)
    try:
        print(json.dumps(validate_cache(args.cache_root), indent=2))
    except CatalogueError as exc:
        print(f"catalogue validation failed: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
