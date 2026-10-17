#!/usr/bin/env python3
"""Build a deterministic, bounded bundle of assets exposed by the static loader."""
from __future__ import annotations

import argparse
import ctypes as c
import hashlib
import importlib.util
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import sys
import tempfile
import xml.etree.ElementTree as ET


HERE = Path(__file__).resolve().parent
RUNTIME_SOURCE = HERE / "runtime.py"
PYDATA_SCRIPTS_SOURCE = HERE.parent / "pydata-scripts" / "pydata_scripts.py"
SCENE_DRAW_LIBRARY = HERE.parent / "scene-draw" / "build" / (
    "libdh2_scene_draw_host.dll" if sys.platform == "win32"
    else "libdh2_scene_draw_host.so")
MAX_FILES = 512
MAX_FILE_BYTES = 128 * 1024 * 1024
MAX_TOTAL_BYTES = 512 * 1024 * 1024
CHUNK_BYTES = 1024 * 1024
MAX_DRAW_NODES = 200_000
MAX_DRAW_COMMANDS = 200_000
MAX_BDAE_MATERIALS = 32_768
MAX_MATERIAL_SAMPLERS = 32_768
MAX_LIGHTSET_XML_BYTES = 16 * 1024 * 1024

# These fields are explicit runtime asset paths in the static loader's records.
# Authoring-only xrefmax/editormax fields are deliberately not included.
DIRECT_ASSET_FIELDS = {
    "dae": "visual-scene",
    "mgp": "module-gameplay-xml",
    "mvp": "module-visual-xml",
    "camera_file": "camera-config",
    "light_set": "level-lightset",
    "fixed_light_set": "fixed-lightset",
}


class AssetBundleError(RuntimeError):
    """A source closure or staging operation failed its bounds or safety checks."""


U = c.c_uint32
I = c.c_int32
P = c.c_void_p


class BresView(c.Structure):
    _fields_ = [("bytes", P), ("size", c.c_size_t)] + [
        (name, U) for name in ("fixup_count", "fixup_offset", "root_offset",
                               "tail_offset", "bulk_size", "block_count", "tail_size")]


class DrawMatrix(c.Structure):
    _fields_ = [("m", c.c_float * 16), ("identity_hint", c.c_uint8)]


class DrawCommand(c.Structure):
    _fields_ = [("world", DrawMatrix), ("node_id", P), ("geometry_id", P),
                ("material_id", P)] + [(name, U) for name in
                ("visual_index", "node_record", "visible", "vertex_count",
                 "index_count", "index_width")] + [
                (name, I) for name in ("geometry_index", "primitive_index", "material_index")]


class DrawStats(c.Structure):
    _fields_ = [(name, U) for name in (
        "visual_references", "visual_scenes", "nodes", "instances",
        "geometry_instances", "resolved_geometry", "skipped_nonvisual_references",
        "skipped_unresolved_visuals", "skipped_unresolved_geometry",
        "skipped_unsupported_geometry", "skipped_nontriangle_primitives",
        "unresolved_materials", "draw_commands")] + [("triangles", c.c_uint64)]


class Material(c.Structure):
    _fields_ = [("image", BresView), ("id", P), ("name", P),
                ("external_effect_file", P), ("effect_url", P), ("record", P),
                ("parameter_count", U), ("parameter_records", P)]


class MaterialParameter(c.Structure):
    _fields_ = [("image", BresView), ("id", P), ("semantic", P),
                ("type_code", U), ("value_count", U), ("raw_value", P)]


class ImageReference(c.Structure):
    _fields_ = [("index", I), ("id", P), ("name", P), ("source_path", P)]


DRAW_VISIT = c.CFUNCTYPE(c.c_bool, c.POINTER(DrawCommand), P)


def _load_runtime():
    name = "dh2_level_runtime_for_asset_bundle"
    existing = sys.modules.get(name)
    if existing is not None:
        return existing
    spec = importlib.util.spec_from_file_location(name, RUNTIME_SOURCE)
    if spec is None or spec.loader is None:
        raise AssetBundleError(f"cannot load static level source: {RUNTIME_SOURCE}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


runtime = _load_runtime()


def _canonical(value: str) -> str:
    try:
        return runtime._normalize_cache_path(value)
    except (TypeError, runtime.LevelRuntimeError) as exc:
        raise AssetBundleError(f"invalid source asset path {value!r}: {exc}") from exc


def collect_closure(level_report: dict[str, object], *,
                    extra_assets: list[dict[str, object]] | None = None) -> list[dict[str, object]]:
    """Collect loader paths and checked external references with their provenance."""
    entries: dict[str, dict[str, object]] = {}

    def add(path_text: str, role: str, reference: dict[str, object],
            expected_sha256: str | None = None) -> None:
        path = _canonical(path_text)
        entry = entries.setdefault(path, {
            "path": path,
            "roles": set(),
            "references": set(),
            "loader_sha256": set(),
        })
        entry["roles"].add(role)
        entry["references"].add(json.dumps(reference, ensure_ascii=False,
                                            sort_keys=True, separators=(",", ":")))
        if expected_sha256:
            entry["loader_sha256"].add(expected_sha256)

    source_hashes = level_report.get("source_sha256")
    if not isinstance(source_hashes, dict) or not source_hashes:
        raise AssetBundleError("static loader returned no source closure hashes")
    for source_path, source_hash in source_hashes.items():
        add(str(source_path), "static-loader-input", {
            "source_path": str(source_path), "source_record": -1,
            "object_name": "", "field": "static-loader-input",
        }, str(source_hash))

    records = level_report.get("source_records")
    if not isinstance(records, list):
        raise AssetBundleError("static loader returned no source records")
    for record in records:
        if not isinstance(record, dict):
            raise AssetBundleError("static loader returned a malformed source record")
        fields = record.get("fields")
        if not isinstance(fields, dict):
            continue
        source_path = str(record.get("source_path", ""))
        source_record = int(record.get("source_record", -1))
        object_name = str(record.get("name", ""))
        script_file = fields.get("scriptFile")
        if script_file is not None and str(script_file).strip():
            script_ref = {
                "source_path": source_path,
                "source_record": source_record,
                "object_name": object_name,
                "field": "scriptFile",
                "resolution": "native Level::_LoadScripts packed-table convention",
            }
            level_names, level_programs = _script_table_paths(str(script_file))
            add(level_names, "level-script-name-table", script_ref)
            add(level_programs, "level-script-program-table", script_ref)
            # The native loader appends common script tables before the level tables.
            add("data/pydata/scripts_pyscriptnames.bin", "common-script-name-table", {
                "source_path": source_path, "source_record": source_record,
                "object_name": object_name, "field": "scriptFile",
                "resolution": "native Level::_LoadScripts common table",
            })
            add("data/pydata/scripts_pyscripts.bin", "common-script-program-table", {
                "source_path": source_path, "source_record": source_record,
                "object_name": object_name, "field": "scriptFile",
                "resolution": "native Level::_LoadScripts common table",
            })
        for field, role in DIRECT_ASSET_FIELDS.items():
            value = fields.get(field)
            if value is None or str(value).strip() == "":
                continue
            add(str(value), role, {
                "source_path": source_path,
                "source_record": source_record,
                "object_name": object_name,
                "field": field,
            })

    for external in extra_assets or []:
        path = _canonical(str(external["path"]))
        entry = entries.setdefault(path, {
            "path": path, "roles": set(), "references": set(), "loader_sha256": set(),
        })
        entry["roles"].add(str(external["role"]))
        for reference in external["references"]:
            entry["references"].add(json.dumps(reference, ensure_ascii=False,
                                                sort_keys=True, separators=(",", ":")))
    # The preceding loader entries are normalized here too, after all references are merged.
    result = []
    for path in sorted(entries):
        entry = entries[path]
        hashes = sorted(entry["loader_sha256"])
        if len(hashes) > 1:
            raise AssetBundleError(f"loader reported conflicting hashes for {path}")
        result.append({
            "path": path,
            "roles": sorted(entry["roles"]),
            "references": [json.loads(item) for item in sorted(entry["references"])],
            "loader_sha256": hashes[0] if hashes else None,
        })
    if len(result) > MAX_FILES:
        raise AssetBundleError(f"closure has {len(result)} files; limit is {MAX_FILES}")
    return result


def _scene_library_api(path: Path):
    """Load the checked BRES, static draw, and material readers."""
    try:
        library = c.CDLL(str(path))
    except OSError as exc:
        raise AssetBundleError(f"cannot load checked scene/material library {path}: {exc}") from exc
    library.dh2_bres_open.argtypes = [c.POINTER(BresView), P, c.c_size_t]
    library.dh2_bres_open.restype = U
    library.dh2_bres_library_count.argtypes = [c.POINTER(BresView), U]
    library.dh2_bres_library_count.restype = U
    library.dh2_material_record.argtypes = [P, P, I]
    library.dh2_material_record.restype = U
    library.dh2_material_parameter.argtypes = [P, P, I]
    library.dh2_material_parameter.restype = U
    library.dh2_material_sampler_image.argtypes = [P, P, I]
    library.dh2_material_sampler_image.restype = U
    library.dh2_static_scene_draws.argtypes = [P, P, DRAW_VISIT, P, U, U]
    library.dh2_static_scene_draws.restype = U
    return library


def _c_text(pointer: int | None) -> str | None:
    if not pointer:
        return None
    try:
        return c.string_at(pointer).decode("utf-8", errors="strict")
    except (UnicodeDecodeError, OSError, ValueError) as exc:
        raise AssetBundleError(f"checked BRES reader returned invalid UTF-8 string: {exc}") from exc


def _bres_texture_path(source_path: str) -> str:
    """Map only an observed game-drive data root into the supplied cache."""
    source = source_path.replace("\\", "/")
    if not re.match(r"^[A-Za-z]:/data(?:/iphone)?/", source):
        raise AssetBundleError(f"unrecognized BRES image source root: {source_path!r}")
    return _canonical(source[3:])


def collect_material_assets(cache: Path, level_report: dict[str, object],
                            library_path: Path) -> tuple[list[dict[str, object]], dict[str, object]]:
    """Resolve BDAE sampler image paths via checked scene→material→BRES APIs."""
    api = _scene_library_api(library_path)
    entries = collect_closure(level_report)
    bdae_paths = sorted({str(entry["path"]) for entry in entries
                         if "visual-scene" in entry["roles"] and str(entry["path"]).endswith(".bdae")})
    if not bdae_paths:
        return [], {"scenes": [], "source_sampler_count": 0, "unique_texture_count": 0}

    modules = level_report.get("modules")
    module_node_records: dict[str, set[int]] = {}
    module_node_contexts: dict[str, dict[int, list[dict[str, object]]]] = {}
    if isinstance(modules, list):
        for module in modules:
            if not isinstance(module, dict) or not isinstance(module.get("dae"), str):
                continue
            path = _canonical(module["dae"])
            context = {
                "module_index": module.get("module_index"),
                "module_source_record": module.get("source_record"),
                "module_name": module.get("name"),
                "visual_index": module.get("visual_index"),
                "root_node_record": module.get("root_node_record"),
            }
            node_records = {int(item) for item in module.get("subtree_node_records", [])}
            module_node_records.setdefault(path, set()).update(node_records)
            node_contexts = module_node_contexts.setdefault(path, {})
            for node_record in node_records:
                node_contexts.setdefault(node_record, []).append(context)

    source_refs: dict[str, list[dict[str, object]]] = {}
    source_records = level_report.get("source_records", [])
    if isinstance(source_records, list):
        for record in source_records:
            if not isinstance(record, dict) or not isinstance(record.get("fields"), dict):
                continue
            raw_dae = record["fields"].get("dae")
            if not isinstance(raw_dae, str) or not raw_dae.strip():
                continue
            path = _canonical(raw_dae)
            source_refs.setdefault(path, []).append({
                "source_path": record.get("source_path"),
                "source_record": record.get("source_record"),
                "object_name": record.get("name"),
                "field": "dae",
            })

    unresolved: list[dict[str, object]] = []
    asset_refs: dict[str, set[str]] = {}
    scene_reports: list[dict[str, object]] = []
    total_samplers = 0
    for bdae_path in bdae_paths:
        source = _source_file(cache, bdae_path)
        size = source.stat().st_size
        if size > MAX_FILE_BYTES:
            raise AssetBundleError(f"BDAE exceeds checked-reader input bound: {bdae_path}")
        raw = source.read_bytes()
        raw_buffer = c.create_string_buffer(raw)
        view = BresView()
        opened = api.dh2_bres_open(c.byref(view), c.cast(raw_buffer, P), len(raw))
        if opened != 0:
            raise AssetBundleError(f"checked BRES reader rejected {bdae_path}: error {opened}")

        subtree = module_node_records.get(bdae_path)
        captured_draws: list[dict[str, object]] = []
        callback_error: list[str] = []

        @DRAW_VISIT
        def visit(command_pointer, _user):
            command = command_pointer.contents
            if len(captured_draws) >= MAX_DRAW_COMMANDS:
                callback_error.append("draw command limit exceeded")
                return False
            if subtree is not None and int(command.node_record) not in subtree:
                return True
            captured_draws.append({
                "node_id": _c_text(command.node_id),
                "node_record": int(command.node_record),
                "geometry_id": _c_text(command.geometry_id),
                "geometry_index": int(command.geometry_index),
                "primitive_index": int(command.primitive_index),
                "primitive_material_id": _c_text(command.material_id),
                "material_index": int(command.material_index),
                "visual_index": int(command.visual_index),
            })
            return True

        stats = DrawStats()
        draw_error = api.dh2_static_scene_draws(c.byref(stats), c.byref(view), visit,
                                                None, MAX_DRAW_NODES, MAX_DRAW_COMMANDS)
        if draw_error != 0 or callback_error:
            detail = callback_error[0] if callback_error else f"reader error {draw_error}"
            raise AssetBundleError(f"static draw traversal failed for {bdae_path}: {detail}")
        draws_by_material: dict[int, list[dict[str, object]]] = {}
        for draw in captured_draws:
            if draw["material_index"] >= 0:
                draws_by_material.setdefault(int(draw["material_index"]), []).append(draw)
        material_count = int(api.dh2_bres_library_count(c.byref(view), U(6)))
        if material_count > MAX_BDAE_MATERIALS:
            raise AssetBundleError(f"BDAE material count exceeds bound: {bdae_path}")
        scene_sampler_count = 0
        scene_material_only_count = 0
        scene_paths: set[str] = set()
        material_indices = (sorted(draws_by_material) if subtree is not None
                            else list(range(material_count)))
        for material_index in material_indices:
            material = Material()
            result = api.dh2_material_record(c.byref(material), c.byref(view), I(material_index))
            if result != 0:
                raise AssetBundleError(f"checked material reader failed for {bdae_path} material {material_index}: {result}")
            material_id, material_name = _c_text(material.id), _c_text(material.name)
            material_draws = draws_by_material.get(material_index, [])
            for parameter_index in range(int(material.parameter_count)):
                if (scene_sampler_count >= MAX_MATERIAL_SAMPLERS or
                        total_samplers >= MAX_MATERIAL_SAMPLERS):
                    raise AssetBundleError(f"BDAE sampler count exceeds bound: {bdae_path}")
                parameter = MaterialParameter()
                result = api.dh2_material_parameter(c.byref(parameter), c.byref(material), I(parameter_index))
                if result != 0:
                    raise AssetBundleError(f"checked material parameter failed for {bdae_path}: {result}")
                if int(parameter.type_code) != 11:
                    continue
                image = ImageReference()
                result = api.dh2_material_sampler_image(c.byref(image), c.byref(material), I(parameter_index))
                if result != 0:
                    raise AssetBundleError(f"checked sampler image failed for {bdae_path}: {result}")
                scene_sampler_count += 1
                total_samplers += 1
                if int(image.index) < 0:
                    unresolved.append({"category": "BDAE sampler image", "status": "unbound",
                                       "source_bdae": bdae_path, "material_index": material_index,
                                       "material_id": material_id, "parameter_index": parameter_index})
                    continue
                raw_path = _c_text(image.source_path)
                if not raw_path:
                    unresolved.append({"category": "BDAE sampler image", "status": "no source path",
                                       "source_bdae": bdae_path, "material_index": material_index,
                                       "material_id": material_id, "parameter_index": parameter_index,
                                       "image_index": int(image.index)})
                    continue
                try:
                    canonical = _bres_texture_path(raw_path)
                    _source_file(cache, canonical)
                except (AssetBundleError, OSError) as exc:
                    unresolved.append({"category": "BDAE sampler image", "status": "unresolved source path",
                                       "source_bdae": bdae_path, "material_index": material_index,
                                       "material_id": material_id, "parameter_index": parameter_index,
                                       "image_index": int(image.index), "source_path": raw_path,
                                       "reason": str(exc)})
                    continue
                scene_paths.add(canonical)
                parameter_id = _c_text(parameter.id)
                image_ref = {
                    "source_bdae": bdae_path,
                    "bdae_source_records": source_refs.get(bdae_path, []),
                    "module_context": [],
                    "node_id": None,
                    "node_record": None,
                    "geometry_id": None,
                    "primitive_index": None,
                    "material_id": material_id,
                    "material_name": material_name,
                    "material_index": material_index,
                    "parameter_id": parameter_id,
                    "parameter_index": parameter_index,
                    "image_id": _c_text(image.id),
                    "image_name": _c_text(image.name),
                    "image_index": int(image.index),
                    "source_path": raw_path,
                    "resolution": "checked BDAE draw/material/sampler image path to exact cache file",
                }
                if material_draws:
                    for draw in material_draws:
                        reference = {**image_ref,
                            "node_id": draw["node_id"], "node_record": draw["node_record"],
                            "geometry_id": draw["geometry_id"],
                            "geometry_index": draw["geometry_index"],
                            "primitive_index": draw["primitive_index"],
                            "primitive_material_id": draw["primitive_material_id"],
                            "visual_index": draw["visual_index"],
                            "module_context": module_node_contexts.get(bdae_path, {}).get(
                                int(draw["node_record"]), []),
                        }
                        asset_refs.setdefault(canonical, set()).add(json.dumps(
                            reference, ensure_ascii=False, sort_keys=True, separators=(",", ":")))
                else:
                    scene_material_only_count += 1
                    asset_refs.setdefault(canonical, set()).add(json.dumps(
                        image_ref, ensure_ascii=False, sort_keys=True, separators=(",", ":")))
        scene_reports.append({
            "source_bdae": bdae_path,
            "source_records": source_refs.get(bdae_path, []),
            "module_subtree_node_count": len(subtree) if subtree is not None else None,
            "draw_commands_total": int(stats.draw_commands),
            "draw_commands_in_scope": len(captured_draws),
            "materials_in_scope": len(material_indices),
            "sampler_count": scene_sampler_count,
            "unique_resolved_textures": sorted(scene_paths),
            "material_only_sampler_count": scene_material_only_count,
        })

    assets = [{"path": path, "role": "bdae-material-sampler-texture",
               "references": [json.loads(reference) for reference in sorted(references)]}
              for path, references in sorted(asset_refs.items())]
    return assets, {
        "reader": "port/scene-draw and port/material-bindings checked host APIs",
        "scenes": scene_reports,
        "source_sampler_count": total_samplers,
        "unique_texture_count": len(assets),
        "unresolved_sampler_references": unresolved,
    }


def audit_lightsets(cache: Path, entries: list[dict[str, object]]) -> dict[str, object]:
    """Report paths actually present in bundled lightset XML without schema guesses."""
    results = []
    for entry in entries:
        roles = set(entry["roles"])
        if not ({"level-lightset", "fixed-lightset"} & roles):
            continue
        relative = str(entry["path"])
        source = _source_file(cache, relative)
        if source.stat().st_size > MAX_LIGHTSET_XML_BYTES:
            raise AssetBundleError(f"lightset exceeds XML audit bound: {relative}")
        try:
            root = ET.fromstring(source.read_bytes())
        except (ET.ParseError, OSError) as exc:
            raise AssetBundleError(f"cannot audit lightset XML {relative}: {exc}") from exc
        path_values = []
        object_types = {}
        for element in root.iter():
            object_type = element.attrib.get("type")
            if object_type:
                object_types[object_type] = object_types.get(object_type, 0) + 1
            values = list(element.attrib.values())
            if element.text and element.text.strip():
                values.append(element.text.strip())
            for value in values:
                normalized = value.replace("\\", "/")
                if re.search(r"(?:[A-Za-z]:/|(?:^|\s)/|\\\\)", normalized):
                    path_values.append(value)
        results.append({"path": relative, "object_types": object_types,
                        "path_like_value_count": len(path_values),
                        "path_like_values": sorted(set(path_values))})
    return {"method": "bounded XML field audit; nested dependencies not inferred",
            "files": results,
            "observed_nested_path_count": sum(item["path_like_value_count"] for item in results)}


def _script_table_paths(script_file: str) -> tuple[str, str]:
    """Resolve the recovered .pyscript table naming convention; no filesystem search."""
    canonical = _canonical(script_file)
    if not canonical.endswith(".pyscript"):
        raise AssetBundleError(
            f"cannot resolve scriptFile without the recovered .pyscript suffix: {script_file!r}")
    base = canonical[:-len(".pyscript")]
    return base + "_pyscriptnames.bin", base + "_pyscripts.bin"


def _load_pydata_script_reader():
    name = "dh2_pydata_scripts_for_asset_bundle"
    existing = sys.modules.get(name)
    if existing is not None:
        return existing
    spec = importlib.util.spec_from_file_location(name, PYDATA_SCRIPTS_SOURCE)
    if spec is None or spec.loader is None:
        raise AssetBundleError(f"cannot load bounded PyData script reader: {PYDATA_SCRIPTS_SOURCE}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def _source_file(cache_root: Path, relative: str) -> Path:
    path = cache_root.joinpath(*PurePosixPath(relative).parts)
    current = cache_root
    for part in PurePosixPath(relative).parts:
        current = current / part
        if current.is_symlink():
            raise AssetBundleError(f"cache asset path contains a symbolic link: {relative}")
    try:
        resolved = path.resolve(strict=True)
    except OSError as exc:
        raise AssetBundleError(f"missing/unreadable cache asset {relative}: {exc}") from exc
    if resolved != cache_root and cache_root not in resolved.parents:
        raise AssetBundleError(f"cache asset escapes supplied root: {relative}")
    if not resolved.is_file():
        raise AssetBundleError(f"cache asset is not a regular file: {relative}")
    return resolved


def _copy_hashed(source: Path, destination: Path, relative: str,
                 max_file_bytes: int, remaining_total: int) -> tuple[int, str]:
    try:
        initial_size = source.stat().st_size
    except OSError as exc:
        raise AssetBundleError(f"cannot stat cache asset {relative}: {exc}") from exc
    if initial_size > max_file_bytes:
        raise AssetBundleError(
            f"cache asset {relative} is {initial_size} bytes; per-file bound is {max_file_bytes}")
    if initial_size > remaining_total:
        raise AssetBundleError(
            f"cache asset {relative} would exceed remaining bundle byte bound {remaining_total}")
    digest = hashlib.sha256()
    size = 0
    try:
        with source.open("rb") as src, destination.open("xb") as dst:
            while True:
                chunk = src.read(CHUNK_BYTES)
                if not chunk:
                    break
                size += len(chunk)
                if size > max_file_bytes:
                    raise AssetBundleError(f"cache asset {relative} grew beyond per-file bound")
                if size > remaining_total:
                    raise AssetBundleError(f"cache asset {relative} exceeded total bundle bound")
                dst.write(chunk)
                digest.update(chunk)
    except AssetBundleError:
        raise
    except OSError as exc:
        raise AssetBundleError(f"copying cache asset {relative} failed: {exc}") from exc
    if size != initial_size:
        raise AssetBundleError(f"cache asset {relative} changed size while being copied")
    return size, digest.hexdigest()


def _atomic_manifest(directory: Path, manifest: dict[str, object]) -> None:
    encoded = (json.dumps(manifest, ensure_ascii=False, sort_keys=True, indent=2) + "\n").encode("utf-8")
    temporary = directory / ".asset-manifest.json.tmp"
    try:
        with temporary.open("xb") as stream:
            stream.write(encoded)
        os.replace(temporary, directory / "asset-manifest.json")
    except OSError as exc:
        raise AssetBundleError(f"writing bundle manifest failed: {exc}") from exc


def _validate_script_tables(cache: Path, entries: list[dict[str, object]]) -> list[dict[str, object]]:
    """Validate each packed name/program pair with the existing bounded reader."""
    reader = _load_pydata_script_reader()
    by_role = {role: str(entry["path"]) for entry in entries for role in entry["roles"]}
    pairs = [
        ("common", "common-script-name-table", "common-script-program-table"),
        ("level", "level-script-name-table", "level-script-program-table"),
    ]
    report = []
    for scope, names_role, programs_role in pairs:
        names_path = by_role.get(names_role)
        programs_path = by_role.get(programs_role)
        if names_path is None and programs_path is None:
            continue
        if names_path is None or programs_path is None:
            raise AssetBundleError(f"incomplete {scope} PyData script-table pair")
        names_file = _source_file(cache, names_path)
        programs_file = _source_file(cache, programs_path)
        try:
            names_size = names_file.stat().st_size
            programs_size = programs_file.stat().st_size
            table_limit = min(MAX_FILE_BYTES, int(reader.MAX_TABLE_BYTES))
            if names_size > table_limit or programs_size > table_limit:
                raise AssetBundleError(f"{scope} PyData script table exceeds {table_limit} bytes")
            table = reader.decode_script_table(names_file.read_bytes(), programs_file.read_bytes())
        except AssetBundleError:
            raise
        except (OSError, reader.DecodeError) as exc:
            raise AssetBundleError(f"validating {scope} PyData script tables failed: {exc}") from exc
        report.append({
            "scope": scope,
            "names_path": names_path,
            "programs_path": programs_path,
            "script_count": len(table.scripts),
            "command_count": sum(len(script.commands) for script in table.scripts),
            "validated_by": "port/pydata-scripts/pydata_scripts.py",
        })
    return report


def audit_script_resources(cache: Path, entries: list[dict[str, object]],
                           level_report: dict[str, object]) -> dict[str, object]:
    """Resolve only script references covered by the existing bounded readers."""
    reader = _load_pydata_script_reader()
    by_role = {role: str(entry["path"]) for entry in entries for role in entry["roles"]}
    common_names_path = by_role.get("common-script-name-table")
    common_programs_path = by_role.get("common-script-program-table")
    level_names_path = by_role.get("level-script-name-table")
    level_programs_path = by_role.get("level-script-program-table")
    if not all((common_names_path, common_programs_path, level_names_path, level_programs_path)):
        return {"status": "script tables unavailable", "resolved_exec_script_calls": [],
                "actor_references": [], "unresolved_numeric_resources": []}
    try:
        common = reader.decode_script_table(
            _source_file(cache, common_names_path).read_bytes(),
            _source_file(cache, common_programs_path).read_bytes())
        level = reader.decode_script_table(
            _source_file(cache, level_names_path).read_bytes(),
            _source_file(cache, level_programs_path).read_bytes())
    except (reader.DecodeError, OSError, AssetBundleError) as exc:
        raise AssetBundleError(f"auditing PyData script references failed: {exc}") from exc
    common_count = len(common.names)
    all_names = (*common.names, *level.names)
    resolved_exec = []
    actor_refs = []
    numeric_refs = []
    source_records = level_report.get("source_records", [])
    records_by_name: dict[str, list[dict[str, object]]] = {}
    if isinstance(source_records, list):
        for record in source_records:
            if isinstance(record, dict) and isinstance(record.get("name"), str):
                records_by_name.setdefault(record["name"], []).append(record)
    for script_index, script in enumerate(level.scripts):
        for command_index, command in enumerate(script.commands):
            context = {"script_name": script.name, "script_index": script_index,
                       "command_index": command_index, "command": command.name}
            if command.name == "ExecScript":
                global_flag, target, _arguments, _tail_flag = command.payload
                resolved_index = int(target) if global_flag else common_count + int(target)
                if 0 <= resolved_index < len(all_names):
                    resolved_exec.append({**context, "global_flag": bool(global_flag),
                        "script_id": int(target), "resolved_name": all_names[resolved_index],
                        "resolved_table": "common" if resolved_index < common_count else "level",
                        "resolved_index": resolved_index})
                else:
                    numeric_refs.append({**context, "status": "unresolved script index",
                                         "script_id": int(target), "global_flag": bool(global_flag)})
            elif command.name == "SpawnCharacter":
                actor_name = str(command.payload[0])
                matches = records_by_name.get(actor_name, [])
                actor_refs.append({**context, "actor_name": actor_name,
                    "resolution": "source record found" if matches else "source record absent",
                    "source_records": [{"source_path": item.get("source_path"),
                        "source_record": item.get("source_record"),
                        "gametype": item.get("gametype")} for item in matches]})
            elif command.name in ("StartDialog", "PlayLevelMusic", "PlaySound", "StopSound"):
                numeric_refs.append({**context, "status": "numeric runtime resource not mapped to file",
                                     "payload": list(command.payload)})
    return {
        "reader": "port/pydata-scripts bounded script-table decoder",
        "common_script_count": common_count,
        "level_script_count": len(level.names),
        "resolved_exec_script_calls": resolved_exec,
        "actor_references": actor_refs,
        "unresolved_numeric_resources": numeric_refs,
        "notes": [
            "ExecScript targets resolve by the recovered common-then-level name-table order.",
            "SpawnCharacter strings are checked against imported source-record names only; absence does not prove runtime absence.",
            "Dialog and audio/music numeric values are not mapped to data files by an existing checked reader.",
        ],
    }


def build_bundle(cache_root: Path, level_name: str, library: Path, stage_dir: Path, *,
                 scene_library: Path = SCENE_DRAW_LIBRARY,
                 max_files: int = MAX_FILES,
                 max_file_bytes: int = MAX_FILE_BYTES,
                 max_total_bytes: int = MAX_TOTAL_BYTES) -> dict[str, object]:
    """Copy a deterministic loader-visible asset closure into a new staging directory."""
    if not (1 <= max_files <= MAX_FILES):
        raise AssetBundleError(f"max_files must be in 1..{MAX_FILES}")
    if not (1 <= max_file_bytes <= MAX_FILE_BYTES):
        raise AssetBundleError(f"max_file_bytes must be in 1..{MAX_FILE_BYTES}")
    if not (1 <= max_total_bytes <= MAX_TOTAL_BYTES):
        raise AssetBundleError(f"max_total_bytes must be in 1..{MAX_TOTAL_BYTES}")
    cache_arg = Path(cache_root).absolute()
    if cache_arg.is_symlink():
        raise AssetBundleError("cache root must not itself be a symbolic link")
    cache = cache_arg.resolve(strict=True)
    if not cache.is_dir():
        raise AssetBundleError(f"cache root is not a directory: {cache}")

    requested_stage = Path(stage_dir).absolute()
    if requested_stage.name in ("", ".", ".."):
        raise AssetBundleError("staging directory must have a concrete final path component")
    parent = requested_stage.parent.resolve()
    destination = parent / requested_stage.name
    if destination.exists() or destination.is_symlink():
        raise AssetBundleError(f"staging directory already exists: {destination}")
    if destination == cache or destination in cache.parents or cache in destination.parents:
        raise AssetBundleError("staging directory must not contain or be inside the cache root")

    try:
        level_report = runtime.load_static_level(cache, level_name, library)
    except (runtime.LevelRuntimeError, OSError) as exc:
        raise AssetBundleError(f"static source load failed: {exc}") from exc
    material_assets, material_resolution = collect_material_assets(
        cache, level_report, Path(scene_library))
    entries = collect_closure(level_report, extra_assets=material_assets)
    if len(entries) > max_files:
        raise AssetBundleError(f"closure has {len(entries)} files; limit is {max_files}")
    script_table_validation = _validate_script_tables(cache, entries)
    script_resource_audit = audit_script_resources(cache, entries, level_report)
    lightset_audit = audit_lightsets(cache, entries)

    parent.mkdir(parents=True, exist_ok=True)
    temporary = Path(tempfile.mkdtemp(prefix=f".{destination.name}.bundle-", dir=parent))
    total = 0
    final_files = []
    try:
        for entry in entries:
            relative = str(entry["path"])
            source = _source_file(cache, relative)
            output = temporary.joinpath(*PurePosixPath(relative).parts)
            output.parent.mkdir(parents=True, exist_ok=True)
            size, digest = _copy_hashed(source, output, relative, max_file_bytes,
                                        max_total_bytes - total)
            expected = entry["loader_sha256"]
            if expected is not None and digest != expected:
                raise AssetBundleError(f"cache asset changed since source load: {relative}")
            total += size
            final_files.append({
                "path": relative,
                "size_bytes": size,
                "sha256": digest,
                "roles": entry["roles"],
                "references": entry["references"],
            })

        manifest: dict[str, object] = {
            "format": "dh2-source-asset-bundle-v1",
            "level_name": level_report["level_name"],
            "level_source": level_report["source_path"],
            "scope": (
                "Direct cache inputs and explicit asset-path fields exposed by the bounded "
                "static level loader; no runtime activation or recursive opaque-format parsing."
            ),
            "module_count": level_report["module_count"],
            "source_record_count": level_report["source_record_count"],
            "script_table_validation": script_table_validation,
            "script_resource_audit": script_resource_audit,
            "material_texture_resolution": material_resolution,
            "lightset_audit": lightset_audit,
            "file_count": len(final_files),
            "total_bytes": total,
            "files": final_files,
            "unresolved_dependencies": [
                {
                    "category": "external effects and runtime shader passes",
                    "status": "partially_resolved",
                    "reason": (
                        "BDAE material samplers with exact source paths are included, with node/" 
                        "material provenance where static draw commands bind them. External effect "
                        "files, runtime effect-group selection, animated passes, and unbound image "
                        "samplers are not resolved. See material_texture_resolution."
                    ),
                },
                {
                    "category": "script resource IDs and dynamically selected actors",
                    "status": "not_resolved",
                    "reason": (
                        "The native script loader's common and per-level packed tables are included "
                        "and format-validated. Numeric dialog/text resource IDs, music/audio file "
                        "dependencies, and dynamically selected/spawned actors outside imported "
                        "source records are not mapped to files by the bounded current readers."
                    ),
                },
                {
                    "category": "nested lightset dependencies",
                    "status": ("none observed in included files" if
                               lightset_audit["observed_nested_path_count"] == 0 else
                               "path-like fields observed; not followed"),
                    "reason": (
                        "The included lightset XML files are audited for path-like fields; this is "
                        "not a schema-wide proof that other lightset formats have no dependencies."
                    ),
                },
            ],
            "excluded_source_fields": [
                {
                    "fields": ["xrefmax", "editormax"],
                    "reason": "Editor/authoring .max references are present in source records but are not runtime assets asserted by this loader.",
                },
            ],
        }
        _atomic_manifest(temporary, manifest)
        try:
            os.rename(temporary, destination)
        except OSError as exc:
            raise AssetBundleError(f"cannot publish staging directory {destination}: {exc}") from exc
        return manifest
    except Exception:
        if temporary.exists():
            shutil.rmtree(temporary, ignore_errors=True)
        raise


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache-root", type=Path, required=True,
                        help="caller-supplied unpacked cache root")
    parser.add_argument("--level", default="INFECTED_VILLAGE_01",
                        help="LevelList key; static MLX entries only")
    parser.add_argument("--library", type=Path, default=HERE / "build" /
                        ("libdh2_level_runtime_host.dll" if sys.platform == "win32"
                         else "libdh2_level_runtime_host.so"))
    parser.add_argument("--scene-library", type=Path, default=SCENE_DRAW_LIBRARY,
                        help="checked scene/material host library used for BDAE sampler paths")
    parser.add_argument("--stage-dir", type=Path, required=True,
                        help="new directory to create; existing staging paths are rejected")
    parser.add_argument("--max-files", type=int, default=MAX_FILES)
    parser.add_argument("--max-file-bytes", type=int, default=MAX_FILE_BYTES)
    parser.add_argument("--max-total-bytes", type=int, default=MAX_TOTAL_BYTES)
    args = parser.parse_args(argv)
    try:
        manifest = build_bundle(args.cache_root, args.level, args.library, args.stage_dir,
            scene_library=args.scene_library,
            max_files=args.max_files, max_file_bytes=args.max_file_bytes,
            max_total_bytes=args.max_total_bytes)
    except (AssetBundleError, OSError) as exc:
        print(f"asset bundle failed: {exc}", file=sys.stderr)
        return 1
    print(json.dumps(manifest, ensure_ascii=False, sort_keys=True, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
