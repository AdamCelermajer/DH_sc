#!/usr/bin/env python3
"""Compile a cache-authored static level into DWLD/SPWN v1.

Generalized form of compile_swamp_static_level.py. It preserves the selected
original MLX/MGP/MVP/BDAE bytes and the caller-named render dependencies, then
emits authored module placements plus unconditional, unique entrypoints. It
does not activate gameplay objects or interpret conditional spawn scripts.

Level selection is data, not code: --row names the LevelList row, --level-name
and --mlx are validated against that row, and --out-prefix names the outputs.
--swamp-compat fills those three from the historical SWAMP preset so the output
is byte-identical to compile_swamp_static_level.py.

Example (DARKWOOD):
  python compile_static_level.py --cache <cache.zip> --output <new-dir> \
      --row 4 --level-name DARKWOOD --mlx 003_darkwood.mlx \
      --render-asset data/3d/textures/env_darkwoods_solide.tga \
      --render-asset data/3d/textures/env_darkwoods_alpha.tga \
      --render-asset data/3d/textures/pvr2_env_darkwoods_alpha_alpha.tga \
      --render-asset data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import math
from pathlib import Path, PurePosixPath
import re
import shutil
import struct
import sys
import tempfile
import xml.etree.ElementTree as ET
import zipfile


EXPECTED_CACHE_SHA256 = "3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679"
MAX_XML_BYTES = 8 * 1024 * 1024
MAX_BDAE_BYTES = 128 * 1024 * 1024
MAX_BUNDLE_BYTES = 512 * 1024 * 1024
MAX_MODULES = 256

# Historical SWAMP inputs. Only --swamp-compat selects these; the generic path
# has no level-specific constants.
SWAMP_PRESET = {
    "row": 41,
    "level_name": "SWAMP",
    "mlx": "001_swamp.mlx",
    "render_assets": (
        "data/3d/textures/env_swamp.tga",
        "data/3d/textures/env_swamp_spec.tga",
        "data/3d/textures/pvr2_env_swamp_alpha.tga",
        "data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae",
    ),
}

HERE = Path(__file__).resolve().parent
CATALOGUE_SOURCE = HERE.parents[1] / "level-catalogue" / "catalogue.py"
TEXTURE_HINT = re.compile(rb"[A-Za-z0-9_\-]+\.(?:tga|png|pvr|dds)", re.IGNORECASE)


class CompileError(ValueError):
    """An input did not satisfy the supported static-level source subset."""


def sha256(raw: bytes) -> str:
    return hashlib.sha256(raw).hexdigest()


def file_sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def canonical_cache_path(value: str) -> str:
    value = value.replace("\\", "/")
    try:
        value.encode("ascii", errors="strict")
    except UnicodeEncodeError as exc:
        raise CompileError(f"non-ASCII cache path {value!r}") from exc
    value = value.lower()
    if value.startswith("data/iphone/"):
        value = "data/" + value[len("data/iphone/"):]
    path = PurePosixPath(value)
    if (not value.startswith("data/") or path.is_absolute() or "\x00" in value
            or any(part in ("", ".", "..") for part in value.split("/"))):
        raise CompileError(f"unsafe cache path {value!r}")
    return value


def archive_cache_path(member: str) -> str | None:
    value = member.replace("\\", "/").lower()
    marker = "/files/"
    if marker in value:
        value = value.split(marker, 1)[1]
    elif value.startswith("files/"):
        value = value[len("files/"):]
    elif not value.startswith("data/"):
        return None
    if not value.startswith("data/"):
        return None
    try:
        return canonical_cache_path(value)
    except CompileError:
        return None


def load_catalogue_module():
    name = "dh2_static_compiler_catalogue"
    existing = sys.modules.get(name)
    if existing is not None:
        return existing
    spec = importlib.util.spec_from_file_location(name, CATALOGUE_SOURCE)
    if spec is None or spec.loader is None:
        raise CompileError(f"cannot load LevelList decoder: {CATALOGUE_SOURCE}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def parse_xml(raw: bytes, expected_root: str, label: str) -> ET.Element:
    if len(raw) > MAX_XML_BYTES:
        raise CompileError(f"{label} exceeds {MAX_XML_BYTES} bytes")
    upper = raw.upper()
    if b"<!DOCTYPE" in upper or b"<!ENTITY" in upper:
        raise CompileError(f"{label} contains a DTD or entity declaration")
    try:
        root = ET.fromstring(raw)
    except ET.ParseError as exc:
        raise CompileError(f"cannot parse {label}: {exc}") from exc
    if root.tag != expected_root:
        raise CompileError(f"{label} root is {root.tag!r}; expected {expected_root!r}")
    if any(child.tag != "GameObject" for child in root):
        raise CompileError(f"{label} has a non-GameObject direct child")
    return root


def vector(value: str | None, label: str) -> tuple[float, float, float]:
    if value is None:
        raise CompileError(f"missing {label}")
    try:
        fields = tuple(float(field.strip()) for field in value.split(","))
    except ValueError as exc:
        raise CompileError(f"invalid {label}: {value!r}") from exc
    if len(fields) != 3 or not all(math.isfinite(field) for field in fields):
        raise CompileError(f"invalid finite 3-vector {label}: {value!r}")
    return fields  # type: ignore[return-value]


def _numeric_float32(value: float, label: str) -> float:
    try:
        packed = struct.pack("<f", value)
    except (OverflowError, struct.error) as exc:
        raise CompileError(f"{label} is outside float32 range: {value}") from exc
    result = struct.unpack("<f", packed)[0]
    if not math.isfinite(result):
        raise CompileError(f"{label} is not a finite float32")
    return result


def _condition_value(value: str | None) -> str | None:
    if value is None or value.strip() == "" or value.strip().lower() == "invalid":
        return None
    return value.strip()


def _load_source_index(archive: zipfile.ZipFile) -> dict[str, list[zipfile.ZipInfo]]:
    index: dict[str, list[zipfile.ZipInfo]] = {}
    for member in archive.infolist():
        if member.is_dir():
            continue
        canonical = archive_cache_path(member.filename)
        if canonical is not None:
            index.setdefault(canonical, []).append(member)
    return index


def _read_source(archive: zipfile.ZipFile,
                 index: dict[str, list[zipfile.ZipInfo]],
                 relative: str, *, limit: int) -> tuple[bytes, zipfile.ZipInfo]:
    canonical = canonical_cache_path(relative)
    matches = index.get(canonical, [])
    if len(matches) != 1:
        raise CompileError(f"{canonical}: expected one cache member, found {len(matches)}")
    member = matches[0]
    if member.file_size > limit:
        raise CompileError(f"{canonical}: {member.file_size} bytes exceeds {limit}")
    try:
        with archive.open(member, "r") as stream:
            raw = stream.read(limit + 1)
    except (OSError, RuntimeError, zipfile.BadZipFile) as exc:
        raise CompileError(f"cannot read {canonical}: {exc}") from exc
    if len(raw) > limit or len(raw) != member.file_size:
        raise CompileError(f"{canonical}: bounded archive read size mismatch")
    return raw, member


def _write_file(root: Path, relative: str, raw: bytes) -> None:
    path = root.joinpath(*PurePosixPath(relative).parts)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(raw)


def _texture_hints(raw: bytes, index: dict[str, list[zipfile.ZipInfo]],
                   closure: dict[str, object]) -> dict[str, list[str]]:
    """Audit-only: texture basenames named by a BDAE that are not in the closure."""
    resolved: set[str] = set()
    unresolved: set[str] = set()
    for match in TEXTURE_HINT.finditer(raw):
        name = match.group(0).decode("ascii").lower()
        name = re.sub(r"^map__\d+__", "", name)
        candidate = "data/3d/textures/" + name
        if candidate in closure:
            continue
        (resolved if candidate in index else unresolved).add(candidate)
    return {"found_in_cache_not_copied": sorted(resolved),
            "missing_from_cache": sorted(unresolved)}


def compile_level(cache_path: Path, output: Path, *, row: int, level_name: str,
                  mlx_name: str, out_prefix: str, render_assets: tuple[str, ...],
                  allow_conditional_entry_zero: bool = False,
                  expected_cache_sha256: str = EXPECTED_CACHE_SHA256) -> dict[str, object]:
    cache_hash = file_sha256(cache_path)
    if cache_hash != expected_cache_sha256:
        raise CompileError(
            f"cache SHA-256 mismatch: expected {expected_cache_sha256}, got {cache_hash}")
    if not re.fullmatch(r"[0-9a-z_]+", out_prefix):
        raise CompileError(f"invalid output prefix {out_prefix!r}")

    output = output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists() and (not output.is_dir() or any(output.iterdir())):
        raise CompileError(f"output must be a new or empty directory: {output}")

    stage = Path(tempfile.mkdtemp(prefix=".static-level-build-", dir=output.parent))
    try:
        with zipfile.ZipFile(cache_path, "r") as archive:
            source_index = _load_source_index(archive)

            catalogue_reader = load_catalogue_module()
            levels_raw, levels_member = _read_source(
                archive, source_index, "data/pydata/levels_pyarray.bin", limit=8 * 1024 * 1024)
            names_raw, names_member = _read_source(
                archive, source_index, "data/pydata/levels_pyarraynames.bin", limit=8 * 1024 * 1024)
            catalogue = catalogue_reader.decode_catalogue(levels_raw, names_raw)
            if len(catalogue.levels) <= row:
                raise CompileError(f"LevelList has no row {row}")
            declaration = catalogue.levels[row]
            if declaration.name != level_name or declaration.level_file.lower() != mlx_name.lower():
                raise CompileError(
                    f"LevelList row {row} is {declaration.name!r}/{declaration.level_file!r}; "
                    f"expected {level_name!r}/{mlx_name!r}")

            level_path = canonical_cache_path(f"data/scene/{declaration.level_file}")
            level_raw, level_member = _read_source(
                archive, source_index, level_path, limit=MAX_XML_BYTES)
            level_root = parse_xml(level_raw, "Level", level_path)
            level_records = list(level_root.findall("GameObject"))
            configs = [r for r in level_records if r.get("gametype") == "LevelConfig"]
            source_modules = [r for r in level_records if r.get("gametype") == "Module"]
            if len(configs) != 1 or not source_modules:
                raise CompileError(
                    f"{level_path}: expected one LevelConfig and at least one Module")
            if len(source_modules) > MAX_MODULES:
                raise CompileError(f"module count exceeds {MAX_MODULES}")

            source_assets: dict[str, tuple[bytes, zipfile.ZipInfo]] = {}
            module_rows: list[dict[str, object]] = []
            spawn_rows: list[dict[str, object]] = []

            def add_source(relative: str, limit: int) -> str:
                canonical = canonical_cache_path(relative)
                if canonical not in source_assets:
                    source_assets[canonical] = _read_source(
                        archive, source_index, canonical, limit=limit)
                return canonical

            add_source(level_path, MAX_XML_BYTES)
            skybox = configs[0].get("skybox")
            if skybox:
                add_source(skybox, MAX_BDAE_BYTES)
            # Caller-named render dependencies (for SWAMP: the env textures and
            # gl_diffuse effect). Preserved in full with the runtime bundle.
            for render_asset in render_assets:
                add_source(render_asset, MAX_BDAE_BYTES)
            for module_index, module in enumerate(source_modules):
                xref = module.get("xrefobject")
                name = module.get("name")
                if not xref or not name:
                    raise CompileError(f"module row {module_index} lacks name/xrefobject")
                module_id = xref + "-node"
                placement = vector(module.get("position"), f"module {module_index} position")
                rotation = vector(module.get("rotation"), f"module {module_index} rotation")
                scale = vector(module.get("scale"), f"module {module_index} scale")
                if len(module_id.encode("utf-8")) >= 112:
                    raise CompileError(f"module node id is too long: {module_id!r}")

                # MGP and MVP are optional as a pair: a visual-only module (no children,
                # e.g. MACHINE_ROOMS_01) has neither. A single one is refused.
                child_fields = [field for field in ("mgp", "mvp") if module.get(field)]
                if child_fields and len(child_fields) != 2:
                    raise CompileError(f"module row {module_index} has only one of mgp/mvp")
                has_children = len(child_fields) == 2
                paths: dict[str, str] = {}
                if has_children:
                    for field in ("mgp", "mvp"):
                        paths[field] = add_source(module.get(field), MAX_XML_BYTES)
                dae_value = module.get("dae")
                if not dae_value:
                    raise CompileError(f"module row {module_index} lacks dae")
                paths["dae"] = add_source(dae_value, MAX_BDAE_BYTES)

                children = []
                if has_children:
                    mgp_raw, _ = source_assets[paths["mgp"]]
                    mvp_raw, _ = source_assets[paths["mvp"]]
                    mgp = parse_xml(mgp_raw, "Module", paths["mgp"])
                    mvp = parse_xml(mvp_raw, "Module", paths["mvp"])
                    children = [("mgp", mgp, paths["mgp"]), ("mvp", mvp, paths["mvp"])]

                # A repeated node id is legal only for visual-only placements (each
                # placement is a separate room instance, e.g. MACHINE_ROOMS_01).
                if any(existing["node_id"] == module_id and
                       (existing["has_children"] or has_children)
                       for existing in module_rows):
                    raise CompileError(f"duplicate module node id {module_id!r}")

                record_counts: dict[str, int] = {"mgp": 0, "mvp": 0}
                for kind, module_xml, source_path in children:
                    for record_index, obj in enumerate(module_xml.findall("GameObject")):
                        record_counts[kind] += 1
                        dae = obj.get("dae")
                        if dae:
                            add_source(dae, MAX_BDAE_BYTES)
                        if kind == "mgp" and obj.get("gametype") == "SpawnPoint":
                            id_text = obj.get("entrypointID")
                            entrypoint_id = None
                            if id_text is not None:
                                try:
                                    entrypoint_id = int(id_text, 10)
                                except ValueError:
                                    pass
                            local_position = vector(obj.get("position"),
                                f"{source_path} record {record_index} spawn position")
                            local_rotation = vector(obj.get("rotation"),
                                f"{source_path} record {record_index} spawn rotation")
                            local_scale = vector(obj.get("scale"),
                                f"{source_path} record {record_index} spawn scale")
                            world_position = tuple(
                                placement[axis] + local_position[axis] for axis in range(3))
                            world_rotation = local_rotation
                            world_scale = local_scale
                            for field_name, values in (
                                ("local position", local_position), ("local rotation", local_rotation),
                                ("local scale", local_scale), ("world position", world_position),
                                ("world rotation", world_rotation), ("world scale", world_scale)):
                                for value in values:
                                    _numeric_float32(value,
                                        f"{source_path} record {record_index} {field_name}")
                            name_value = obj.get("name")
                            if not name_value or len(name_value.encode("utf-8")) >= 64:
                                raise CompileError(
                                    f"{source_path} record {record_index} has invalid spawn name")
                            spawn_rows.append({
                                "entrypoint_id": entrypoint_id,
                                "name": name_value,
                                "module_index": module_index,
                                "source_path": source_path,
                                "source_record": record_index,
                                "activate_condition": _condition_value(obj.get("activate_cond")),
                                "deactivate_condition": _condition_value(obj.get("deactivate_cond")),
                                "local_position": local_position,
                                "local_rotation": local_rotation,
                                "local_scale": local_scale,
                                "world_position": world_position,
                                "world_rotation": world_rotation,
                                "world_scale": world_scale,
                            })

                module_rows.append({
                    "module_index": module_index,
                    "source_record": level_records.index(module),
                    "name": name,
                    "node_id": module_id,
                    "position": placement,
                    "rotation": rotation,
                    "scale": scale,
                    "mgp": paths.get("mgp"),
                    "mgp_records": record_counts["mgp"],
                    "mvp": paths.get("mvp"),
                    "mvp_records": record_counts["mvp"],
                    "dae": paths["dae"],
                    "has_children": has_children,
                })

            # Entry zero is the only hard launch requirement. Other spawn IDs are
            # selectable only when their ID is unique and unconditional; the v1
            # SPWN record has no field for source condition semantics.
            zero_rows = [row_ for row_ in spawn_rows if row_["entrypoint_id"] == 0]
            if not zero_rows:
                raise CompileError("no authored entrypoint ID 0")
            if len(zero_rows) != 1 and not allow_conditional_entry_zero:
                raise CompileError(
                    f"expected exactly one authored entrypoint ID 0, found {len(zero_rows)}; "
                    "pass --allow-conditional-entry-zero to pick the first source-order row")
            id_counts: dict[int, int] = {}
            for row_ in spawn_rows:
                entrypoint_id = row_["entrypoint_id"]
                if isinstance(entrypoint_id, int):
                    id_counts[entrypoint_id] = id_counts.get(entrypoint_id, 0) + 1
            selectable: list[dict[str, object]] = []
            unsupported: list[dict[str, object]] = []
            for row_ in spawn_rows:
                reasons: list[str] = []
                entrypoint_id = row_["entrypoint_id"]
                if not isinstance(entrypoint_id, int):
                    reasons.append("missing_or_invalid_entrypoint_id")
                elif id_counts.get(entrypoint_id) != 1:
                    reasons.append("duplicate_entrypoint_id")
                if row_["activate_condition"] is not None:
                    reasons.append("conditional_activation")
                if row_["deactivate_condition"] is not None:
                    reasons.append("conditional_deactivation")
                if reasons:
                    unsupported.append({**row_, "unsupported_reasons": reasons})
                else:
                    selectable.append(row_)
            entry_zero = zero_rows[0]
            entry_zero_policy = "unconditional_unique"
            if entry_zero not in selectable:
                if not allow_conditional_entry_zero:
                    raise CompileError("authored entrypoint ID 0 is conditional or duplicated")
                # Development policy: the DWLD start position is the first source-order
                # row. Source conditions are not evaluated; the policy is recorded.
                entry_zero_policy = "first_source_order_conditional"
            zero_spread = max(
                math.dist(zero_rows[0]["world_position"], row_["world_position"])
                for row_ in zero_rows)

            start_position = entry_zero["world_position"]
            # DWLD v1 (128-byte records) stays the output whenever every module is an
            # untransformed, child-bearing placement, so existing levels are unchanged.
            # v2 (152-byte records) adds rotation (degrees, MLX order rx,ry,rz), scale and
            # flags: bit0 = has MGP/MVP children, bit1 = visual-only (no children).
            needs_v2 = any(row_["rotation"] != (0.0, 0.0, 0.0) or
                           row_["scale"] != (1.0, 1.0, 1.0) or not row_["has_children"]
                           for row_ in module_rows)
            dwld_version = 2 if needs_v2 else 1
            dwld = bytearray(struct.pack(
                "<4sII3f", b"DWLD", dwld_version, len(module_rows),
                *(_numeric_float32(v, "entrypoint-zero world position") for v in start_position)))
            for row_ in module_rows:
                encoded_id = str(row_["node_id"]).encode("utf-8")
                xyz = tuple(_numeric_float32(v, f"module {row_['module_index']} position")
                            for v in row_["position"])
                dwld.extend(encoded_id.ljust(112, b"\0"))
                if dwld_version == 1:
                    dwld.extend(struct.pack("<3fI", *xyz, 0))
                else:
                    rot = tuple(_numeric_float32(v, f"module {row_['module_index']} rotation")
                                for v in row_["rotation"])
                    scl = tuple(_numeric_float32(v, f"module {row_['module_index']} scale")
                                for v in row_["scale"])
                    flags = 1 if row_["has_children"] else 2
                    dwld.extend(struct.pack("<9fI", *xyz, *rot, *scl, flags))

            spwn = bytearray(struct.pack("<4sIII", b"SPWN", 1, len(selectable), 0))
            for row_ in selectable:
                encoded_name = str(row_["name"]).encode("utf-8")
                values = (*row_["local_position"], *row_["local_rotation"], *row_["local_scale"],
                          *row_["world_position"], *row_["world_rotation"], *row_["world_scale"])
                packed = tuple(_numeric_float32(value, f"spawn {row_['name']} transform")
                               for value in values)
                spwn.extend(struct.pack("<iI64s18f", int(row_["entrypoint_id"]),
                    int(row_["module_index"]), encoded_name.ljust(64, b"\0"), *packed))

            total_bytes = sum(len(raw) for raw, _ in source_assets.values())
            if total_bytes > MAX_BUNDLE_BYTES:
                raise CompileError(f"source closure exceeds {MAX_BUNDLE_BYTES} bytes")
            for relative, (raw, _member) in sorted(source_assets.items()):
                _write_file(stage, "assets/" + relative, raw)
            (stage / f"{out_prefix}.dwld").write_bytes(dwld)
            (stage / f"{out_prefix}.spwn").write_bytes(spwn)

            asset_manifest = []
            for relative, (raw, member) in sorted(source_assets.items()):
                asset_manifest.append({
                    "path": relative,
                    "archive_member": member.filename,
                    "bytes": len(raw),
                    "sha256": sha256(raw),
                })
            texture_hints: dict[str, object] = {}
            for module_dae in sorted({str(row_["dae"]) for row_ in module_rows}):
                texture_hints[module_dae] = _texture_hints(
                    source_assets[module_dae][0], source_index, source_assets)
            generated = {
                f"{out_prefix}.dwld": {"bytes": len(dwld), "sha256": sha256(dwld)},
                f"{out_prefix}.spwn": {"bytes": len(spwn), "sha256": sha256(spwn)},
            }
            manifest: dict[str, object] = {
                "format": "dh2-static-level-bundle-v1",
                "compiler": "compile_static_level.py",
                "cache_sha256": cache_hash,
                "level_list": {"row": row, "name": declaration.name,
                    "level_file": declaration.level_file},
                "level_list_inputs": {
                    "data/pydata/levels_pyarray.bin": {
                        "bytes": len(levels_raw), "sha256": sha256(levels_raw)},
                    "data/pydata/levels_pyarraynames.bin": {
                        "bytes": len(names_raw), "sha256": sha256(names_raw)},
                },
                "source_layout": level_path,
                "module_count": len(module_rows),
                "modules": module_rows,
                "entrypoint_zero": entry_zero,
                "entrypoint_zero_policy": entry_zero_policy,
                "entrypoint_zero_candidates": len(zero_rows),
                "entrypoint_zero_spread": zero_spread,
                "spawn_point_count": len(spawn_rows),
                "selectable_entrypoints": [row_["entrypoint_id"] for row_ in selectable],
                "unsupported_entrypoints": unsupported,
                "unsupported_spawn_id_groups": [
                    {"entrypoint_id": entrypoint_id,
                     "source_rows": [{"name": row_["name"], "module_index": row_["module_index"],
                         "source_path": row_["source_path"], "source_record": row_["source_record"],
                         "activate_condition": row_["activate_condition"],
                         "deactivate_condition": row_["deactivate_condition"]}
                         for row_ in spawn_rows if row_["entrypoint_id"] == entrypoint_id],
                     "reason": "duplicate IDs and/or conditions cannot be represented by SPWN v1"}
                    for entrypoint_id in sorted(id_counts)
                    if id_counts[entrypoint_id] > 1 or any(
                        row_["entrypoint_id"] == entrypoint_id and
                        (row_["activate_condition"] is not None or
                         row_["deactivate_condition"] is not None)
                        for row_ in spawn_rows)
                ],
                "source_assets": asset_manifest,
                "texture_hints": texture_hints,
                "generated": generated,
                "scope": "authored static module transforms and unique unconditional spawn selectors; no gameplay activation",
            }
            manifest_raw = (json.dumps(manifest, indent=2, ensure_ascii=False,
                sort_keys=True) + "\n").encode("utf-8")
            (stage / "manifest.json").write_bytes(manifest_raw)

            # LevelList bytes are hashed in the manifest; unrelated tables are not copied.
            _ = levels_member, names_member, level_member

        if output.exists():
            output.rmdir()  # allowed only because the caller-provided directory was empty
        stage.rename(output)
        return manifest
    except BaseException:
        if stage.exists():
            shutil.rmtree(stage, ignore_errors=True)
        raise


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--cache", type=Path, required=True,
        help="pinned original cache archive")
    parser.add_argument("--output", type=Path, required=True,
        help="new or empty directory for descriptors and original source assets")
    parser.add_argument("--swamp-compat", action="store_true",
        help="use the historical SWAMP row/name/MLX/render-asset preset")
    parser.add_argument("--row", type=int, help="LevelList row index")
    parser.add_argument("--level-name", help="expected LevelList name for the row")
    parser.add_argument("--mlx", help="expected LevelFile basename under data/scene/ (e.g. 003_darkwood.mlx)")
    parser.add_argument("--out-prefix", help="output stem; default is the MLX basename without extension")
    parser.add_argument("--render-asset", action="append", default=[], metavar="CACHE_PATH",
        help="extra cache path to preserve in full (repeatable)")
    parser.add_argument("--allow-conditional-entry-zero", action="store_true",
        help="if every ID-0 spawn is conditional, use the first source-order row as the "
             "DWLD start position (recorded as entrypoint_zero_policy in the manifest)")
    args = parser.parse_args()

    if args.swamp_compat:
        row = args.row if args.row is not None else SWAMP_PRESET["row"]
        level_name = args.level_name or SWAMP_PRESET["level_name"]
        mlx_name = args.mlx or SWAMP_PRESET["mlx"]
        render_assets = tuple(SWAMP_PRESET["render_assets"]) + tuple(args.render_asset)
    else:
        if args.row is None or not args.level_name or not args.mlx:
            parser.error("--row, --level-name and --mlx are required without --swamp-compat")
        row, level_name, mlx_name = args.row, args.level_name, args.mlx
        render_assets = tuple(args.render_asset)
    out_prefix = args.out_prefix or Path(mlx_name).stem.lower()

    try:
        manifest = compile_level(args.cache, args.output, row=row, level_name=level_name,
            mlx_name=mlx_name, out_prefix=out_prefix, render_assets=render_assets,
            allow_conditional_entry_zero=args.allow_conditional_entry_zero)
    except (CompileError, OSError, zipfile.BadZipFile, ValueError) as exc:
        parser.error(str(exc))
    print(json.dumps({"validation": "PASS", "output": str(args.output.resolve()),
        "level_list_row": row, "level_name": level_name,
        "modules": manifest["module_count"],
        "spawn_points": manifest["spawn_point_count"],
        "selectable_entrypoints": manifest["selectable_entrypoints"],
        "unsupported_entrypoint_rows": len(manifest["unsupported_entrypoints"]),
        "entrypoint_zero_policy": manifest["entrypoint_zero_policy"],
        "assets": len(manifest["source_assets"]),
        f"{out_prefix}.dwld_sha256": manifest["generated"][f"{out_prefix}.dwld"]["sha256"],
        f"{out_prefix}.spwn_sha256": manifest["generated"][f"{out_prefix}.spwn"]["sha256"]},
        sort_keys=True))


if __name__ == "__main__":
    main()
