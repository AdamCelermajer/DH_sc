#!/usr/bin/env python3
"""Compile the cache-authored static SWAMP level into DWLD/SPWN v1.

This bounded compiler preserves the selected original MLX/MGP/MVP/BDAE bytes
and its audited texture/effect/skybox dependencies, then emits authored module
placements plus unconditional, unique entrypoints. It does not activate
gameplay objects or interpret conditional spawn scripts.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import math
from pathlib import Path, PurePosixPath
import struct
import sys
import tempfile
import xml.etree.ElementTree as ET
import zipfile


EXPECTED_CACHE_SHA256 = "3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679"
LEVEL_ROW = 41
LEVEL_NAME = "SWAMP"
LEVEL_FILE = "001_swamp.mlx"
MAX_XML_BYTES = 8 * 1024 * 1024
MAX_BDAE_BYTES = 128 * 1024 * 1024
MAX_BUNDLE_BYTES = 512 * 1024 * 1024

HERE = Path(__file__).resolve().parent
CATALOGUE_SOURCE = HERE.parents[1] / "level-catalogue" / "catalogue.py"


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
    name = "dh2_swamp_static_compiler_catalogue"
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


def compile_swamp(cache_path: Path, output: Path) -> dict[str, object]:
    cache_hash = file_sha256(cache_path)
    if cache_hash != EXPECTED_CACHE_SHA256:
        raise CompileError(
            f"cache SHA-256 mismatch: expected {EXPECTED_CACHE_SHA256}, got {cache_hash}")

    output = output.resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    if output.exists() and (not output.is_dir() or any(output.iterdir())):
        raise CompileError(f"output must be a new or empty directory: {output}")

    stage = Path(tempfile.mkdtemp(prefix=".swamp-level-build-", dir=output.parent))
    try:
        with zipfile.ZipFile(cache_path, "r") as archive:
            source_index = _load_source_index(archive)

            catalogue_reader = load_catalogue_module()
            levels_raw, levels_member = _read_source(
                archive, source_index, "data/pydata/levels_pyarray.bin", limit=8 * 1024 * 1024)
            names_raw, names_member = _read_source(
                archive, source_index, "data/pydata/levels_pyarraynames.bin", limit=8 * 1024 * 1024)
            catalogue = catalogue_reader.decode_catalogue(levels_raw, names_raw)
            if len(catalogue.levels) <= LEVEL_ROW:
                raise CompileError(f"LevelList has no row {LEVEL_ROW}")
            declaration = catalogue.levels[LEVEL_ROW]
            if declaration.name != LEVEL_NAME or declaration.level_file.lower() != LEVEL_FILE:
                raise CompileError(
                    f"LevelList row {LEVEL_ROW} is {declaration.name!r}/{declaration.level_file!r}; "
                    f"expected {LEVEL_NAME}/{LEVEL_FILE}")

            level_path = canonical_cache_path(f"data/scene/{declaration.level_file}")
            level_raw, level_member = _read_source(
                archive, source_index, level_path, limit=MAX_XML_BYTES)
            level_root = parse_xml(level_raw, "Level", level_path)
            level_records = list(level_root.findall("GameObject"))
            configs = [row for row in level_records if row.get("gametype") == "LevelConfig"]
            source_modules = [row for row in level_records if row.get("gametype") == "Module"]
            if len(configs) != 1 or not source_modules:
                raise CompileError(
                    f"{level_path}: expected one LevelConfig and at least one Module")
            if len(source_modules) > 256:
                raise CompileError("module count exceeds 256")

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
            # The selected swamp modules use this source material/effect set.
            # Preserve the full referenced source resources with the runtime
            # bundle; the current Android renderer consumes the three images.
            for render_asset in (
                "data/3d/textures/env_swamp.tga",
                "data/3d/textures/env_swamp_spec.tga",
                "data/3d/textures/pvr2_env_swamp_alpha.tga",
                "data/gfx/effects/gl_diffuse_l1_vc_iphone.bdae",
            ):
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
                if rotation != (0.0, 0.0, 0.0) or scale != (1.0, 1.0, 1.0):
                    raise CompileError(
                        f"module row {module_index} is rotated or scaled; only translations are supported")
                if len(module_id.encode("utf-8")) >= 112:
                    raise CompileError(f"module node id is too long: {module_id!r}")
                if any(existing["node_id"] == module_id for existing in module_rows):
                    raise CompileError(f"duplicate module node id {module_id!r}")

                paths: dict[str, str] = {}
                for field in ("mgp", "mvp", "dae"):
                    value = module.get(field)
                    if not value:
                        raise CompileError(f"module row {module_index} lacks {field}")
                    paths[field] = add_source(value, MAX_BDAE_BYTES if field == "dae"
                                              else MAX_XML_BYTES)

                mgp_raw, _ = source_assets[paths["mgp"]]
                mvp_raw, _ = source_assets[paths["mvp"]]
                mgp = parse_xml(mgp_raw, "Module", paths["mgp"])
                mvp = parse_xml(mvp_raw, "Module", paths["mvp"])

                # Validate runtime visual references authored by module records,
                # and retain the referenced BDAE bytes in the source closure.
                record_counts: dict[str, int] = {"mgp": 0, "mvp": 0}
                for kind, module_xml, source_path in (
                    ("mgp", mgp, paths["mgp"]), ("mvp", mvp, paths["mvp"])):
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
                    "mgp": paths["mgp"],
                    "mgp_records": record_counts["mgp"],
                    "mvp": paths["mvp"],
                    "mvp_records": record_counts["mvp"],
                    "dae": paths["dae"],
                })

            # Entry zero is the only hard launch requirement. Other spawn IDs are
            # selectable only when their ID is unique and unconditional; the v1
            # SPWN record has no field for source condition semantics.
            zero_rows = [row for row in spawn_rows if row["entrypoint_id"] == 0]
            if len(zero_rows) != 1:
                raise CompileError(
                    f"expected exactly one authored entrypoint ID 0, found {len(zero_rows)}")
            id_counts: dict[int, int] = {}
            for row in spawn_rows:
                entrypoint_id = row["entrypoint_id"]
                if isinstance(entrypoint_id, int):
                    id_counts[entrypoint_id] = id_counts.get(entrypoint_id, 0) + 1
            selectable: list[dict[str, object]] = []
            unsupported: list[dict[str, object]] = []
            for row in spawn_rows:
                reasons: list[str] = []
                entrypoint_id = row["entrypoint_id"]
                if not isinstance(entrypoint_id, int):
                    reasons.append("missing_or_invalid_entrypoint_id")
                elif id_counts.get(entrypoint_id) != 1:
                    reasons.append("duplicate_entrypoint_id")
                if row["activate_condition"] is not None:
                    reasons.append("conditional_activation")
                if row["deactivate_condition"] is not None:
                    reasons.append("conditional_deactivation")
                if reasons:
                    unsupported.append({**row, "unsupported_reasons": reasons})
                else:
                    selectable.append(row)
            if zero_rows[0] not in selectable:
                raise CompileError("authored entrypoint ID 0 is conditional or duplicated")

            start_position = zero_rows[0]["world_position"]
            dwld = bytearray(struct.pack(
                "<4sII3f", b"DWLD", 1, len(module_rows),
                *(_numeric_float32(v, "entrypoint-zero world position") for v in start_position)))
            for row in module_rows:
                encoded_id = str(row["node_id"]).encode("utf-8")
                xyz = tuple(_numeric_float32(v, f"module {row['module_index']} position")
                            for v in row["position"])
                dwld.extend(encoded_id.ljust(112, b"\0"))
                dwld.extend(struct.pack("<3fI", *xyz, 0))

            spwn = bytearray(struct.pack("<4sIII", b"SPWN", 1, len(selectable), 0))
            for row in selectable:
                encoded_name = str(row["name"]).encode("utf-8")
                values = (*row["local_position"], *row["local_rotation"], *row["local_scale"],
                          *row["world_position"], *row["world_rotation"], *row["world_scale"])
                packed = tuple(_numeric_float32(value, f"spawn {row['name']} transform")
                               for value in values)
                spwn.extend(struct.pack("<iI64s18f", int(row["entrypoint_id"]),
                    int(row["module_index"]), encoded_name.ljust(64, b"\0"), *packed))

            total_bytes = sum(len(raw) for raw, _ in source_assets.values())
            if total_bytes > MAX_BUNDLE_BYTES:
                raise CompileError(f"source closure exceeds {MAX_BUNDLE_BYTES} bytes")
            for relative, (raw, _member) in sorted(source_assets.items()):
                _write_file(stage, "assets/" + relative, raw)
            (stage / "001_swamp.dwld").write_bytes(dwld)
            (stage / "001_swamp.spwn").write_bytes(spwn)

            asset_manifest = []
            for relative, (raw, member) in sorted(source_assets.items()):
                asset_manifest.append({
                    "path": relative,
                    "archive_member": member.filename,
                    "bytes": len(raw),
                    "sha256": sha256(raw),
                })
            generated = {
                "001_swamp.dwld": {"bytes": len(dwld), "sha256": sha256(dwld)},
                "001_swamp.spwn": {"bytes": len(spwn), "sha256": sha256(spwn)},
            }
            manifest: dict[str, object] = {
                "format": "dh2-static-level-bundle-v1",
                "cache_sha256": cache_hash,
                "level_list": {"row": LEVEL_ROW, "name": declaration.name,
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
                "entrypoint_zero": zero_rows[0],
                "spawn_point_count": len(spawn_rows),
                "selectable_entrypoints": [row["entrypoint_id"] for row in selectable],
                "unsupported_entrypoints": unsupported,
                "unsupported_spawn_id_groups": [
                    {"entrypoint_id": entrypoint_id,
                     "source_rows": [{"name": row["name"], "module_index": row["module_index"],
                         "source_path": row["source_path"], "source_record": row["source_record"],
                         "activate_condition": row["activate_condition"],
                         "deactivate_condition": row["deactivate_condition"]}
                         for row in spawn_rows if row["entrypoint_id"] == entrypoint_id],
                     "reason": "duplicate IDs and/or conditions cannot be represented by SPWN v1"}
                    for entrypoint_id in sorted(id_counts)
                    if id_counts[entrypoint_id] > 1 or any(
                        row["entrypoint_id"] == entrypoint_id and
                        (row["activate_condition"] is not None or
                         row["deactivate_condition"] is not None)
                        for row in spawn_rows)
                ],
                "source_assets": asset_manifest,
                "generated": generated,
                "scope": "authored static module transforms and unique unconditional spawn selectors; no gameplay activation",
            }
            manifest_raw = (json.dumps(manifest, indent=2, ensure_ascii=False,
                sort_keys=True) + "\n").encode("utf-8")
            (stage / "manifest.json").write_bytes(manifest_raw)

            # Keep the otherwise-unused LevelList bytes in the deterministic
            # provenance manifest as hashes without copying unrelated tables.
            # Row 41 is independently decoded from both original binary tables.
            _ = levels_member, names_member

        if output.exists():
            output.rmdir()  # allowed only because the caller-provided directory was empty
        stage.rename(output)
        return manifest
    except BaseException:
        if stage.exists():
            import shutil
            shutil.rmtree(stage, ignore_errors=True)
        raise


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, required=True,
        help="pinned original cache archive")
    parser.add_argument("--output", type=Path, required=True,
        help="new or empty directory for descriptors and original source assets")
    args = parser.parse_args()
    try:
        manifest = compile_swamp(args.cache, args.output)
    except (CompileError, OSError, zipfile.BadZipFile, ValueError) as exc:
        parser.error(str(exc))
    print(json.dumps({"validation": "PASS", "output": str(args.output.resolve()),
        "level_list_row": LEVEL_ROW, "modules": manifest["module_count"],
        "spawn_points": manifest["spawn_point_count"],
        "selectable_entrypoints": manifest["selectable_entrypoints"],
        "unsupported_entrypoint_rows": len(manifest["unsupported_entrypoints"]),
        "assets": len(manifest["source_assets"]),
        "dwld_sha256": manifest["generated"]["001_swamp.dwld"]["sha256"],
        "spwn_sha256": manifest["generated"]["001_swamp.spwn"]["sha256"]}, sort_keys=True))


if __name__ == "__main__":
    main()
