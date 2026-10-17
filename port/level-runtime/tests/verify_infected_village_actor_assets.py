#!/usr/bin/env python3
"""Verify the cache-only Infected Village actor asset closure."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import sys
import xml.etree.ElementTree as ET

sys.dont_write_bytecode = True

HERE = Path(__file__).resolve().parent
DEFAULT_MANIFEST = HERE.parent / "infected-village-actor-assets.json"
MAX_MANIFEST_BYTES = 1024 * 1024
MAX_CACHE_ASSET_BYTES = 128 * 1024 * 1024
MAX_MGP_BYTES = 8 * 1024 * 1024
HEX_SHA256 = re.compile(r"^[0-9a-f]{64}$")


class VerificationError(ValueError):
    """The manifest or supplied cache failed a closed validation check."""


def _object(value: object, where: str) -> dict:
    if not isinstance(value, dict):
        raise VerificationError(f"{where} must be an object")
    return value


def _array(value: object, where: str) -> list:
    if not isinstance(value, list):
        raise VerificationError(f"{where} must be an array")
    return value


def _cache_relative_path(value: object, where: str) -> str:
    if not isinstance(value, str) or not value:
        raise VerificationError(f"{where} must be a non-empty path")
    if "\\" in value or value != value.lower() or not value.startswith("data/"):
        raise VerificationError(f"{where} is not a canonical lower-case data path: {value!r}")
    parts = value.split("/")
    if any(part in ("", ".", "..") for part in parts):
        raise VerificationError(f"{where} contains an unsafe path component: {value!r}")
    try:
        value.encode("ascii")
    except UnicodeEncodeError as exc:
        raise VerificationError(f"{where} must be ASCII: {value!r}") from exc
    return value


def _read_beneath(root: Path, relative: str, limit: int, where: str) -> bytes:
    candidate = (root / Path(*PurePosixPath(relative).parts)).resolve(strict=True)
    if candidate != root and root not in candidate.parents:
        raise VerificationError(f"{where} escapes supplied cache root: {relative}")
    try:
        size = candidate.stat().st_size
    except OSError as exc:
        raise VerificationError(f"cannot stat {where} {relative}: {exc}") from exc
    if size > limit:
        raise VerificationError(f"{where} exceeds {limit} bytes: {relative} ({size})")
    try:
        with candidate.open("rb") as stream:
            raw = stream.read(limit + 1)
    except OSError as exc:
        raise VerificationError(f"cannot read {where} {relative}: {exc}") from exc
    if len(raw) > limit:
        raise VerificationError(f"{where} grew beyond {limit} bytes while reading: {relative}")
    return raw


def _check_manifest_shape(data: dict) -> tuple[list[dict], dict[str, dict]]:
    if data.get("format") != "dh2.infected-village-actor-assets.v1":
        raise VerificationError("unsupported or missing manifest format")

    source = _object(data.get("source_records"), "source_records")
    records = _array(source.get("records"), "source_records.records")
    if source.get("module_index") != 0 or len(records) != 4:
        raise VerificationError("expected four Ambush Character records from module 0")
    shared_template = _object(source.get("shared_template"), "source_records.shared_template")
    if (shared_template.get("template_id") != 68
            or shared_template.get("template_name") != "InfectedVillage_CommonType1"
            or shared_template.get("template_name_field") != "MonsterCommonType1"
            or shared_template.get("char_template_pydata") != "Charater_Templates"):
        raise VerificationError("shared template id/name is missing")

    record_names: set[str] = set()
    record_indices: set[int] = set()
    for index, row_value in enumerate(records):
        row = _object(row_value, f"source_records.records[{index}]")
        name = row.get("name")
        source_record = row.get("source_record")
        attrs = _object(row.get("attributes"), f"source record {index}.attributes")
        if not isinstance(name, str) or not name or name in record_names:
            raise VerificationError(f"source record {index} has an empty or duplicate name")
        if not isinstance(source_record, int) or source_record < 0 or source_record in record_indices:
            raise VerificationError(f"source record {index} has an invalid or duplicate index")
        if attrs.get("name") != name or attrs.get("gametype") != "Character":
            raise VerificationError(f"source record {name} identity fields disagree")
        for field, expected in (
            ("char_template_pydata", shared_template.get("char_template_pydata")),
            ("_templateName", shared_template.get("template_name_field")),
            ("char_template", shared_template.get("template_name")),
            ("ai_state", "Limbus"),
            ("auto_spawn", "0"),
        ):
            if attrs.get(field) != expected:
                raise VerificationError(f"source record {name} has unexpected {field}={attrs.get(field)!r}")
        record_names.add(name)
        record_indices.add(source_record)
    if {row["source_record"]: row["name"] for row in records} != {
        6: "_prim_tmp_infected05",
        7: "_prim_tmp_infected06",
        8: "_prim_tmp_infected07",
        19: "_prim_tmp_infected16",
    }:
        raise VerificationError("the four Ambush actor record identities changed")

    choices = _object(data.get("template_choices"), "template_choices")
    ids = _array(choices.get("character_table_ids_in_template_order"),
                 "template_choices.character_table_ids_in_template_order")
    models = _array(choices.get("models"), "template_choices.models")
    if (ids != [258, 252, 251, 255, 256, 257]
            or len(models) != 6 or len(set(ids)) != 6):
        raise VerificationError("the shared template must have six distinct CharacterTable choices")
    model_keys: set[str] = set()
    model_paths: set[str] = set()
    model_rows: dict[str, dict] = {}
    for index, model_value in enumerate(models):
        model = _object(model_value, f"template_choices.models[{index}]")
        model_id = model.get("character_table_id")
        key = model.get("model_key")
        path = _cache_relative_path(model.get("bdae_path"), f"model {index}.bdae_path")
        if model_id != ids[index]:
            raise VerificationError("template CharacterTable ids and model-choice order disagree")
        if not isinstance(key, str) or not key or key in model_keys or path in model_paths:
            raise VerificationError(f"model choice {index} has a duplicate key or path")
        if not model.get("character_name") or not model.get("skin_controller_id"):
            raise VerificationError(f"model choice {index} lacks its Character/skin identity")
        model_keys.add(key)
        model_paths.add(path)
        model_rows[key] = model

    animation = _object(data.get("animation"), "animation")
    if (animation.get("table") != "CharAnimTable" or animation.get("table_id") != 29
            or animation.get("name") != "Infected"):
        raise VerificationError("shared CharacterTable animation reference is not CharAnimTable[29] Infected")
    anim_tpls = _array(animation.get("anim_tpls"), "animation.anim_tpls")
    states = _array(animation.get("states"), "animation.states")
    unmapped = _array(animation.get("unmapped_state_slots"), "animation.unmapped_state_slots")
    if not states or animation.get("positive_state_mapping_count") != len(states):
        raise VerificationError("positive CharAnim state mapping count does not match its entries")
    tpl_by_id: dict[int, dict] = {}
    for index, tpl_value in enumerate(anim_tpls):
        tpl = _object(tpl_value, f"animation.anim_tpls[{index}]")
        tpl_id = tpl.get("id")
        if not isinstance(tpl_id, int) or tpl_id <= 0 or tpl_id in tpl_by_id:
            raise VerificationError(f"AnimTpl {index} has an invalid or duplicate positive id")
        if not isinstance(tpl.get("name"), str) or not tpl["name"]:
            raise VerificationError(f"AnimTpl {tpl_id} has no name")
        paths = _array(tpl.get("anim_dict_paths"), f"AnimTpl {tpl_id}.anim_dict_paths")
        if not paths:
            raise VerificationError(f"AnimTpl {tpl_id} has no AnimDict paths")
        tpl["anim_dict_paths"] = [
            _cache_relative_path(path, f"AnimTpl {tpl_id} AnimDict path") for path in paths
        ]
        tpl_by_id[tpl_id] = tpl

    state_names: set[str] = set()
    for index, state_value in enumerate(states):
        state = _object(state_value, f"animation.states[{index}]")
        name = state.get("state")
        tpl_id = state.get("anim_tpl_id")
        if not isinstance(name, str) or not name or name in state_names:
            raise VerificationError(f"CharAnim state {index} has an empty or duplicate name")
        if not isinstance(tpl_id, int) or tpl_id <= 0 or tpl_id not in tpl_by_id:
            raise VerificationError(f"CharAnim state {name} references an absent/non-positive AnimTpl")
        state_names.add(name)
    if not any(row.get("state") == "Limbus" and row.get("anim_tpl_id") == -1 for row in unmapped):
        raise VerificationError("the unresolved Limbus=-1 CharAnim slot must remain explicit")
    if "Limbus" in state_names:
        raise VerificationError("Limbus is marked as a positive CharAnim state despite its -1 slot")

    clips = {path for tpl in tpl_by_id.values() for path in tpl["anim_dict_paths"]}
    if len(clips) != 20:
        raise VerificationError(f"expected 20 unique positive AnimDict paths; found {len(clips)}")

    texture_section = _object(data.get("model_textures"), "model_textures")
    applies = _array(texture_section.get("applies_to_model_keys"),
                     "model_textures.applies_to_model_keys")
    textures = _array(texture_section.get("paths"), "model_textures.paths")
    normalized_textures = {
        _cache_relative_path(path, "model texture path") for path in textures
    }
    if len(normalized_textures) != 4 or set(applies) != model_keys:
        raise VerificationError("the four shared model textures must apply to all six model choices")

    assets = _array(data.get("asset_files"), "asset_files")
    by_path: dict[str, dict] = {}
    role_counts = {"model": 0, "animation": 0, "texture": 0}
    for index, asset_value in enumerate(assets):
        asset = _object(asset_value, f"asset_files[{index}]")
        path = _cache_relative_path(asset.get("path"), f"asset_files[{index}].path")
        role = asset.get("role")
        if path in by_path or role not in role_counts:
            raise VerificationError(f"asset_files[{index}] has a duplicate path or unknown role")
        size = asset.get("size_bytes")
        digest = asset.get("sha256")
        if not isinstance(size, int) or size < 0 or size > MAX_CACHE_ASSET_BYTES:
            raise VerificationError(f"asset {path} has an invalid size_bytes")
        if not isinstance(digest, str) or not HEX_SHA256.fullmatch(digest):
            raise VerificationError(f"asset {path} has an invalid lower-case SHA-256")
        role_counts[role] += 1
        by_path[path] = asset
    if role_counts != {"model": 6, "animation": 20, "texture": 4}:
        raise VerificationError(f"unexpected closure role counts: {role_counts}")
    if set(by_path) != model_paths | clips | normalized_textures:
        raise VerificationError("closure file paths do not equal the model, animation and texture links")
    for path in model_paths:
        if by_path[path]["role"] != "model":
            raise VerificationError(f"model choice points to a non-model file: {path}")
    for path in clips:
        if by_path[path]["role"] != "animation":
            raise VerificationError(f"AnimDict points to a non-animation file: {path}")
    for path in normalized_textures:
        if by_path[path]["role"] != "texture":
            raise VerificationError(f"model texture points to a non-texture file: {path}")

    closure_summary = _object(data.get("closure_summary"), "closure_summary")
    if closure_summary.get("unique_asset_paths") != len(by_path):
        raise VerificationError("closure_summary.unique_asset_paths does not match asset_files")
    declared_total = closure_summary.get("total_size_bytes")
    if not isinstance(declared_total, int) or declared_total != sum(
        asset["size_bytes"] for asset in by_path.values()
    ):
        raise VerificationError("closure_summary.total_size_bytes does not match the per-file sizes")
    return assets, by_path


def verify(cache_root: Path, manifest_path: Path) -> dict:
    root = cache_root.resolve(strict=True)
    if not root.is_dir():
        raise VerificationError(f"cache root is not a directory: {root}")
    try:
        manifest_size = manifest_path.stat().st_size
        if manifest_size > MAX_MANIFEST_BYTES:
            raise VerificationError(f"manifest exceeds {MAX_MANIFEST_BYTES} bytes")
        manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as exc:
        raise VerificationError(f"cannot read manifest {manifest_path}: {exc}") from exc
    data = _object(manifest, "manifest")
    assets, by_path = _check_manifest_shape(data)

    checked_bytes = 0
    for asset in assets:
        path = asset["path"]
        try:
            raw = _read_beneath(root, path, MAX_CACHE_ASSET_BYTES, "asset")
        except (OSError, FileNotFoundError) as exc:
            raise VerificationError(f"missing/unreadable asset {path}: {exc}") from exc
        actual_size = len(raw)
        actual_sha = hashlib.sha256(raw).hexdigest()
        if actual_size != asset["size_bytes"]:
            raise VerificationError(
                f"size mismatch for {path}: expected {asset['size_bytes']}, found {actual_size}"
            )
        if actual_sha != asset["sha256"]:
            raise VerificationError(f"SHA-256 mismatch for {path}: expected {asset['sha256']}, found {actual_sha}")
        checked_bytes += actual_size

    source = data["source_records"]
    source_path = _cache_relative_path(source.get("source_path"), "source_records.source_path")
    try:
        mgp = _read_beneath(root, source_path, MAX_MGP_BYTES, "MGP source")
    except (OSError, FileNotFoundError) as exc:
        raise VerificationError(f"missing/unreadable MGP source {source_path}: {exc}") from exc
    try:
        xml_root = ET.fromstring(mgp)
    except ET.ParseError as exc:
        raise VerificationError(f"cannot parse MGP source {source_path}: {exc}") from exc
    if xml_root.tag != "Module":
        raise VerificationError(f"MGP source has root {xml_root.tag!r}, expected 'Module'")
    objects = xml_root.findall("GameObject")
    if len(objects) > 4096:
        raise VerificationError("MGP GameObject count exceeds the verifier bound")
    for row in source["records"]:
        index = row["source_record"]
        if index >= len(objects):
            raise VerificationError(f"MGP record index {index} is outside the source file")
        actual = objects[index].attrib
        if actual != row["attributes"]:
            raise VerificationError(
                f"MGP source record {index} ({row['name']}) no longer matches the recorded attributes"
            )

    animation = data["animation"]
    template_anim = data["template_choices"]["shared_character_animation_table"]
    if (animation.get("table"), animation.get("table_id"), animation.get("name")) != (
        template_anim.get("table"), template_anim.get("table_id"), template_anim.get("name")
    ):
        raise VerificationError("template choices and CharAnim table link disagree")

    summary = data["closure_summary"]
    if checked_bytes != summary["total_size_bytes"]:
        raise VerificationError("verified byte total disagrees with closure summary")
    return {
        "status": "verified",
        "manifest": str(manifest_path.resolve()),
        "cache_root": str(root),
        "asset_paths_checked": len(by_path),
        "asset_bytes_checked": checked_bytes,
        "source_mgp_records_checked": len(source["records"]),
        "data_link_checks": {
            "mgp_records_match_cache": True,
            "four_records_share_template": True,
            "six_character_model_choices_link_to_model_files": True,
            "positive_charanim_states_link_to_animtpls": True,
            "animtpls_link_to_animation_files": True,
            "four_textures_link_to_closure": True,
            "pydata_mapping_scope": "cross-reference consistency in manifest; no PyData bytes are embedded or decoded by this verifier"
        }
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache-root", type=Path, required=True,
                        help="extracted original cache root (contains data/)")
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST,
                        help=f"metadata manifest (default: {DEFAULT_MANIFEST})")
    args = parser.parse_args()
    try:
        result = verify(args.cache_root, args.manifest)
    except (OSError, VerificationError) as exc:
        print(f"verification failed: {exc}", file=sys.stderr)
        return 1
    print(json.dumps(result, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
