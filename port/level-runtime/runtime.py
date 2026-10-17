#!/usr/bin/env python3
"""Bounded static-MLX import and module-binding coordinator for DH2."""
from __future__ import annotations

import argparse
import ctypes as c
import hashlib
import importlib.util
import json
from pathlib import Path, PurePosixPath
import sys


MAX_BINARY_BYTES = 8 * 1024 * 1024
MAX_XML_BYTES = 8 * 1024 * 1024
MAX_BDAE_BYTES = 128 * 1024 * 1024
MAX_MODULES = 256
MAX_ENTITIES = 32768
MAX_SCENE_NODES = 65536
MAX_OUTPUT_BYTES = 16 * 1024 * 1024
WORLD_ROOT = Path(__file__).resolve().parents[1] / "world-data"
CATALOGUE_SOURCE = Path(__file__).resolve().parents[1] / "level-catalogue" / "catalogue.py"


class LevelRuntimeError(RuntimeError):
    """A bounded load failed; no partial level description is returned."""


U = c.c_uint32
I = c.c_int32
P = c.c_void_p


class Diagnostic(c.Structure):
    _fields_ = [(name, U) for name in ("error", "byte_offset", "line", "column")]
    _fields_ += [("message", c.c_char * 160)]


class Transform(c.Structure):
    _fields_ = [(name, c.c_float * 3) for name in ("position", "rotation_degrees", "scale")]


class Field(c.Structure):
    _fields_ = [("name", c.c_char_p), ("value", c.c_char_p)]


class Object(c.Structure):
    _fields_ = [("source_path", c.c_char_p)] + [(name, U) for name in
        ("source_record", "source_begin", "source_end", "module_index", "kind")]
    _fields_ += [("fields", c.POINTER(Field)), ("field_count", U),
        ("name", c.c_char_p), ("gametype", c.c_char_p), ("local", Transform),
        ("world_position", c.c_float * 3)]


class Module(c.Structure):
    _fields_ = [("record", Object)]
    _fields_ += [(name, c.c_char_p) for name in
        ("cache_dae", "cache_mgp", "cache_mvp", "catalogue_node_id")]
    _fields_ += [("mgp_loaded", c.c_bool), ("mvp_loaded", c.c_bool)]


class Level(c.Structure):
    _fields_ = [("name", c.c_char_p), ("source_path", c.c_char_p), ("config", Object),
        ("modules", c.POINTER(Module)), ("module_count", U),
        ("entities", c.POINTER(Object)), ("entity_count", U)]


class Bres(c.Structure):
    _fields_ = [("bytes", P), ("size", c.c_size_t)] + [(name, U) for name in
        ("fixup_count", "fixup_offset", "root_offset", "tail_offset",
         "bulk_size", "block_count", "tail_size")]


class Scene(c.Structure):
    _fields_ = [("image", Bres)] + [(name, U) for name in
        ("references", "reference_offset", "visuals", "visual_offset")]


class Binding(c.Structure):
    _fields_ = [("visual_index", U), ("node_record", U),
        ("catalogue_origin", c.c_float * 3), ("placement_delta", c.c_float * 3)]


def bind_library(path: Path):
    """Bind the existing world, BRES and scene reader APIs in a host library."""
    dll = c.CDLL(str(path.resolve()))
    specs = {
        "dh2_world_import_level": (U, [c.POINTER(Level), c.c_char_p, c.c_char_p,
                                        P, c.c_size_t, c.POINTER(Diagnostic)]),
        "dh2_world_import_module_objects": (U, [c.POINTER(Level), U, U, c.c_char_p,
                                                P, c.c_size_t, c.POINTER(Diagnostic)]),
        "dh2_world_free": (None, [c.POINTER(Level)]),
        "dh2_world_field": (c.c_char_p, [c.POINTER(Object), c.c_char_p]),
        "dh2_world_bind_module": (U, [c.POINTER(Binding), c.POINTER(Module),
                                       c.POINTER(Scene), c.POINTER(Diagnostic)]),
        "dh2_world_module_records": (U, [c.POINTER(U), U, c.POINTER(U),
            c.POINTER(Binding), c.POINTER(Scene), c.POINTER(Diagnostic)]),
        "dh2_bres_open": (U, [c.POINTER(Bres), P, c.c_size_t]),
        "dh2_scene_open": (U, [c.POINTER(Scene), c.POINTER(Bres)]),
    }
    for name, (result, args) in specs.items():
        function = getattr(dll, name)
        function.restype = result
        function.argtypes = args
    return dll


def _catalogue_module():
    name = "dh2_level_runtime_catalogue"
    existing = sys.modules.get(name)
    if existing is not None:
        return existing
    spec = importlib.util.spec_from_file_location(name, CATALOGUE_SOURCE)
    if spec is None or spec.loader is None:
        raise LevelRuntimeError(f"cannot load level catalogue source: {CATALOGUE_SOURCE}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def _normalize_cache_path(value: str) -> str:
    value = value.replace("\\", "/")
    try:
        value.encode("ascii")
    except UnicodeEncodeError as exc:
        raise LevelRuntimeError(f"cache path is not ASCII: {value!r}") from exc
    value = value.lower()
    if value.startswith("data/iphone/"):
        value = "data/" + value[len("data/iphone/"):]
    path = PurePosixPath(value)
    if (not value.startswith("data/") or path.is_absolute() or "#" in value
            or ":" in value or any(part in ("", ".", "..") for part in value.split("/"))):
        raise LevelRuntimeError(f"unsafe cache asset path: {value!r}")
    return value


def _cache_file(root: Path, relative: str, limit: int) -> tuple[Path, bytes]:
    canonical = _normalize_cache_path(relative)
    resolved_root = root.resolve()
    path = resolved_root.joinpath(*PurePosixPath(canonical).parts).resolve()
    if path != resolved_root and resolved_root not in path.parents:
        raise LevelRuntimeError(f"cache path escapes supplied root: {relative!r}")
    try:
        size = path.stat().st_size
    except OSError as exc:
        raise LevelRuntimeError(f"missing/unreadable cache file {canonical}: {exc}") from exc
    if size > limit:
        raise LevelRuntimeError(f"cache file {canonical} is {size} bytes; limit is {limit}")
    try:
        with path.open("rb") as stream:
            data = stream.read(limit + 1)
    except OSError as exc:
        raise LevelRuntimeError(f"cannot read cache file {canonical}: {exc}") from exc
    if len(data) > limit:
        raise LevelRuntimeError(f"cache file {canonical} grew beyond {limit} bytes")
    return path, data


def _decode(value: bytes | None) -> str:
    return "" if value is None else value.decode("utf-8", errors="strict")


def _vector(values) -> list[float]:
    return [float(value) for value in values]


def _object_fields(obj: Object) -> dict[str, str]:
    return {_decode(obj.fields[index].name): _decode(obj.fields[index].value)
            for index in range(obj.field_count)}


def _object_report(obj: Object) -> dict[str, object]:
    return {
        "source_path": _decode(obj.source_path),
        "source_record": int(obj.source_record),
        "module_index": None if obj.module_index == 0xffffffff else int(obj.module_index),
        "kind": ("level", "mgp", "mvp")[int(obj.kind)],
        "name": _decode(obj.name),
        "gametype": _decode(obj.gametype),
        "local_position": _vector(obj.local.position),
        "world_position": _vector(obj.world_position),
        "fields": _object_fields(obj),
    }


def _fail_native(result: int, diagnostic: Diagnostic, context: str) -> None:
    if result:
        detail = diagnostic.message.decode("utf-8", errors="replace") or f"reader error {result}"
        raise LevelRuntimeError(f"{context}: {detail} (code {result})")


def load_static_level(cache_root: Path, level_name: str, library: Path, *,
                      max_modules: int = MAX_MODULES,
                      max_records: int = MAX_ENTITIES + MAX_MODULES + 1,
                      max_scene_nodes: int = MAX_SCENE_NODES,
                      max_output_bytes: int = MAX_OUTPUT_BYTES) -> dict[str, object]:
    """Import one catalogue-declared static MLX and bind all module sources.

    The returned document is assembled privately and returned only after every
    module MGP/MVP and BDAE root has passed its bounded readers. Imported
    gameplay records are descriptions only; no runtime factories are invoked.
    """
    if not isinstance(level_name, str) or not level_name:
        raise LevelRuntimeError("level name is required")
    if not (1 <= max_modules <= MAX_MODULES):
        raise LevelRuntimeError(f"max_modules must be in 1..{MAX_MODULES}")
    if not (1 <= max_records <= MAX_ENTITIES + MAX_MODULES + 1):
        raise LevelRuntimeError("max_records is outside supported output bounds")
    if not (1 <= max_scene_nodes <= MAX_SCENE_NODES):
        raise LevelRuntimeError(f"max_scene_nodes must be in 1..{MAX_SCENE_NODES}")
    if not (1 <= max_output_bytes <= MAX_OUTPUT_BYTES):
        raise LevelRuntimeError(f"max_output_bytes must be in 1..{MAX_OUTPUT_BYTES}")

    catalogue_reader = _catalogue_module()
    root = Path(cache_root)
    _, levels_bytes = _cache_file(root, "data/pydata/levels_pyarray.bin", MAX_BINARY_BYTES)
    _, names_bytes = _cache_file(root, "data/pydata/levels_pyarraynames.bin", MAX_BINARY_BYTES)
    try:
        catalogue = catalogue_reader.decode_catalogue(levels_bytes, names_bytes)
    except catalogue_reader.CatalogueError as exc:
        raise LevelRuntimeError(f"invalid LevelList catalogue: {exc}") from exc
    declaration = catalogue.levels_by_name.get(level_name)
    if declaration is None:
        raise LevelRuntimeError(f"LevelList has no level named {level_name!r}")
    if declaration.file_kind != "static_mlx":
        raise LevelRuntimeError(
            f"{level_name} uses {declaration.file_kind}; this slice accepts static MLX only")

    level_path = _normalize_cache_path(f"data/scene/{declaration.level_file}")
    _, level_bytes = _cache_file(root, level_path, MAX_XML_BYTES)
    dll = bind_library(library)
    level = Level()
    diagnostic = Diagnostic()
    level_buffer = c.create_string_buffer(level_bytes)
    code = dll.dh2_world_import_level(c.byref(level), level_name.encode("utf-8"),
        level_path.encode("ascii"), level_buffer, len(level_bytes), c.byref(diagnostic))
    _fail_native(code, diagnostic, f"importing {level_path}")
    hashes = {
        "data/pydata/levels_pyarray.bin": hashlib.sha256(levels_bytes).hexdigest(),
        "data/pydata/levels_pyarraynames.bin": hashlib.sha256(names_bytes).hexdigest(),
        level_path: hashlib.sha256(level_bytes).hexdigest(),
    }
    try:
        if level.module_count > max_modules:
            raise LevelRuntimeError(
                f"level has {level.module_count} modules; output bound is {max_modules}")
        if level.module_count == 0:
            raise LevelRuntimeError("static MLX contains no modules")

        source_records = [_object_report(level.config)]
        dae_cache: dict[str, tuple[object, Bres, Scene]] = {}
        module_reports: list[dict[str, object]] = []

        for module_index in range(level.module_count):
            module = level.modules[module_index]
            for kind, relative in ((1, _decode(module.cache_mgp)),
                                   (2, _decode(module.cache_mvp))):
                canonical = _normalize_cache_path(relative)
                _, data = _cache_file(root, canonical, MAX_XML_BYTES)
                buffer = c.create_string_buffer(data)
                code = dll.dh2_world_import_module_objects(c.byref(level), module_index,
                    kind, canonical.encode("ascii"), buffer, len(data), c.byref(diagnostic))
                _fail_native(code, diagnostic,
                    f"importing module {module_index} {('MGP' if kind == 1 else 'MVP')} {canonical}")
                hashes[canonical] = hashlib.sha256(data).hexdigest()
                total_records = len(source_records) + int(level.entity_count) + int(level.module_count)
                if total_records > max_records:
                    raise LevelRuntimeError(
                        f"loaded source records exceed output bound {max_records}")

            dae_path = _normalize_cache_path(_decode(module.cache_dae))
            if dae_path not in dae_cache:
                _, dae_bytes = _cache_file(root, dae_path, MAX_BDAE_BYTES)
                dae_buffer = c.create_string_buffer(dae_bytes)
                bres = Bres()
                result = dll.dh2_bres_open(c.byref(bres), dae_buffer, len(dae_bytes))
                if result:
                    raise LevelRuntimeError(f"opening BDAE {dae_path}: BRES error {result}")
                scene = Scene()
                result = dll.dh2_scene_open(c.byref(scene), c.byref(bres))
                if result:
                    raise LevelRuntimeError(f"opening BDAE scene {dae_path}: scene error {result}")
                dae_cache[dae_path] = (dae_buffer, bres, scene)
                hashes[dae_path] = hashlib.sha256(dae_bytes).hexdigest()

            _dae_buffer, _bres, scene = dae_cache[dae_path]
            binding = Binding()
            result = dll.dh2_world_bind_module(c.byref(binding), c.byref(module),
                                              c.byref(scene), c.byref(diagnostic))
            _fail_native(result, diagnostic,
                         f"binding module {module_index} root {_decode(module.catalogue_node_id)}")
            records = (U * max_scene_nodes)()
            count = U()
            result = dll.dh2_world_module_records(records, max_scene_nodes, c.byref(count),
                c.byref(binding), c.byref(scene), c.byref(diagnostic))
            _fail_native(result, diagnostic, f"collecting BDAE subtree for module {module_index}")
            module_reports.append({
                "module_index": module_index,
                "source_record": int(module.record.source_record),
                "name": _decode(module.record.name),
                "catalogue_node_id": _decode(module.catalogue_node_id),
                "dae": dae_path,
                "mgp": _decode(module.cache_mgp),
                "mvp": _decode(module.cache_mvp),
                "mgp_loaded": bool(module.mgp_loaded),
                "mvp_loaded": bool(module.mvp_loaded),
                "level_origin": _vector(module.record.local.position),
                "catalogue_origin": _vector(binding.catalogue_origin),
                "placement_delta": _vector(binding.placement_delta),
                "visual_index": int(binding.visual_index),
                "root_node_record": int(binding.node_record),
                "subtree_node_records": [int(records[i]) for i in range(count.value)],
            })

        source_records.extend(_object_report(level.entities[i])
                              for i in range(level.entity_count))
        if len(source_records) > max_records:
            raise LevelRuntimeError(
                f"loaded source records exceed output bound {max_records}")
        # Module instance records are separate in the native owned Level, but
        # are inserted into the unified source-order view between config and
        # imported module entities, matching MLX order.
        source_records = [source_records[0]] + [
            _object_report(level.modules[i].record) for i in range(level.module_count)
        ] + source_records[1:]

        spawn_points = []
        for item in source_records:
            if item["gametype"] != "SpawnPoint":
                continue
            entry_text = item["fields"].get("entrypointID")
            entrypoint = None
            if entry_text is not None:
                try:
                    entrypoint = int(entry_text, 10)
                except ValueError:
                    pass
            spawn_points.append({
                "name": item["name"], "gametype": item["gametype"],
                "source_path": item["source_path"],
                "source_record": item["source_record"],
                "module_index": item["module_index"], "entrypoint_id": entrypoint,
                "local_position": item["local_position"],
                "world_position": item["world_position"],
            })

        result: dict[str, object] = {
            "scope": "static MLX plus authored module records and BDAE root bindings; no object activation",
            "level_name": level_name,
            "catalogue_level_file": declaration.level_file,
            "source_path": level_path,
            "source_record_count": len(source_records),
            "module_count": len(module_reports),
            "modules": module_reports,
            "spawn_points": spawn_points,
            "entrypoint_id_0": [point for point in spawn_points if point["entrypoint_id"] == 0],
            "source_records": source_records,
            "source_sha256": dict(sorted(hashes.items())),
            "objects_activated": False,
            "scripts_executed": False,
            "enemies_spawned": False,
            "camera_started": False,
            "gameplay_ready": False,
        }
        encoded = (json.dumps(result, indent=2, ensure_ascii=False) + "\n").encode("utf-8")
        if len(encoded) > max_output_bytes:
            raise LevelRuntimeError(
                f"serialized level output is {len(encoded)} bytes; bound is {max_output_bytes}")
        return result
    finally:
        dll.dh2_world_free(c.byref(level))


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache-root", type=Path, required=True,
                        help="caller-supplied unpacked cache root")
    parser.add_argument("--level", default="INFECTED_VILLAGE_01",
                        help="LevelList key; static MLX entries only")
    parser.add_argument("--library", type=Path, default=Path(__file__).parent / "build" /
                        ("libdh2_level_runtime_host.dll" if sys.platform == "win32"
                         else "libdh2_level_runtime_host.so"))
    parser.add_argument("--report", type=Path)
    args = parser.parse_args(argv)
    try:
        report = load_static_level(args.cache_root, args.level, args.library)
    except (LevelRuntimeError, OSError) as exc:
        print(f"static level load failed: {exc}", file=sys.stderr)
        return 1
    serialized = json.dumps(report, indent=2, ensure_ascii=False) + "\n"
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(serialized, encoding="utf-8")
    print(serialized, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
