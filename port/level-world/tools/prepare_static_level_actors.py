#!/usr/bin/env python3
"""Stage directly-authored Monster actors of a static level for the native DACT loader.

Generalized form of prepare_swamp_actors.py. It selects unconditional,
auto-spawn Monster records whose authored initial state is absent or Idle from
the level's pinned Module MGPs; template, quest, Limbus, NPC, and trigger-driven
factories remain source-side work. It copies the referenced actor BDAE models,
their texture dependencies and the animation clips reachable from the selected
character states into the Android asset tree, and writes
worlds/<out-prefix>.dact plus worlds/<out-prefix>-actors.json.

Level selection is data, not code: --row/--level-name/--mlx name the LevelList
row (validated against the cache), --out-prefix names the outputs, and
--provenance points at the static-level manifest produced by
compile_static_level.py (default: worlds/<out-prefix>-provenance.json under the
project assets). --swamp-compat fills the historical SWAMP preset, whose output
is byte-identical to prepare_swamp_actors.py.

Examples:
  # SWAMP regression (write to a scratch project, never the tracked tree):
  python prepare_static_level_actors.py <cache-dir> --swamp-compat --project <scratch>

  # DARKWOOD (manifest copied from compile_static_level.py output):
  python prepare_static_level_actors.py <cache-dir> --row 4 --level-name DARKWOOD \
      --mlx 003_darkwood.mlx --provenance <manifest.json>
"""
from __future__ import annotations

import argparse
import collections
import hashlib
import json
import math
from pathlib import Path, PurePosixPath
import struct
import sys
import xml.etree.ElementTree as ET

from prepare_actors import images, strings
from inspect_animation_data import parse as parse_animation_data
from inspect_animation_tables import sections as animation_sections


MAX_XML_BYTES = 8 * 1024 * 1024
MAX_SOURCE_BYTES = 128 * 1024 * 1024
MAX_OUTPUT_BYTES = 512 * 1024 * 1024
SWAMP_PRESET = {
    "row": 41,
    "level_name": "SWAMP",
    "mlx": "001_swamp.mlx",
    "expected_modules": 9,
    "expected_actors": 5,
}
SELECTION_TEXT = ("direct Monster; no activation/deactivation condition; "
                  "auto_spawn != 0; initial ai_state absent or Idle")
RUNTIME_GAPS_TEXT = ("Module rotation/scale, conditional/template/NPC/container factories, "
                     "source AI activation and LevelSavegame are not implied.")
CATALOGUE_SOURCE = Path(__file__).resolve().parents[2] / "level-catalogue" / "catalogue.py"


class CompileError(ValueError):
    pass


def digest(raw: bytes) -> str:
    return hashlib.sha256(raw).hexdigest()


def canonical(value: str) -> str:
    value = value.replace("\\", "/").lower()
    if value.startswith("data/iphone/"):
        value = "data/" + value[len("data/iphone/"):]
    path = PurePosixPath(value)
    if (not value.startswith("data/") or path.is_absolute() or "\x00" in value
            or any(part in ("", ".", "..") for part in value.split("/"))):
        raise CompileError(f"unsafe cache path: {value!r}")
    return value


def parse_xml(raw: bytes, label: str, root_name: str) -> ET.Element:
    if len(raw) > MAX_XML_BYTES:
        raise CompileError(f"{label} exceeds the XML limit")
    upper = raw.upper()
    if b"<!DOCTYPE" in upper or b"<!ENTITY" in upper:
        raise CompileError(f"{label} contains a DTD/entity declaration")
    try:
        root = ET.fromstring(raw)
    except ET.ParseError as exc:
        raise CompileError(f"cannot parse {label}: {exc}") from exc
    if root.tag != root_name:
        raise CompileError(f"{label} root is {root.tag!r}; expected {root_name!r}")
    return root


def vector(value: str | None, label: str) -> tuple[float, float, float]:
    try:
        out = tuple(float(part.strip()) for part in (value or "").split(","))
    except ValueError as exc:
        raise CompileError(f"invalid {label}") from exc
    if len(out) != 3 or not all(math.isfinite(item) for item in out):
        raise CompileError(f"invalid finite 3-vector for {label}")
    return out  # type: ignore[return-value]


def source_index(root: Path) -> dict[str, list[Path]]:
    index: dict[str, list[Path]] = {}
    for path in root.rglob("*"):
        if path.is_file():
            index.setdefault(path.name.casefold(), []).append(path)
    return index


def read_source(root: Path, index: dict[str, list[Path]], relative: str,
                limit: int = MAX_SOURCE_BYTES) -> tuple[bytes, Path]:
    path = root.joinpath(*canonical(relative).split("/"))
    if path.is_file():
        selected = path
    else:
        matches = index.get(PurePosixPath(relative.replace("\\", "/")).name.casefold(), [])
        if len(matches) != 1:
            raise CompileError(f"source asset is absent or ambiguous: {relative}")
        selected = matches[0]
    size = selected.stat().st_size
    if size <= 0 or size > limit:
        raise CompileError(f"source asset size rejected: {relative}")
    return selected.read_bytes(), selected


def copy_asset(assets: Path, folder: str, name: str, raw: bytes) -> dict[str, object]:
    safe_name = Path(name).name
    if safe_name != name or not safe_name or ".." in safe_name:
        raise CompileError(f"unsafe asset basename: {name!r}")
    output_name = safe_name.lower() if folder == "textures" else safe_name
    destination = assets / folder / output_name
    destination.parent.mkdir(parents=True, exist_ok=True)
    if destination.exists():
        if digest(destination.read_bytes()) != digest(raw):
            raise CompileError(f"staged asset conflicts with existing file: {folder}/{output_name}")
    else:
        destination.write_bytes(raw)
    return {"asset": f"{folder}/{output_name}", "bytes": len(raw), "sha256": digest(raw)}


def condition(value: str | None) -> str | None:
    if value is None or not value.strip() or value.strip().casefold() == "invalid":
        return None
    return value.strip()


def fixed_ascii(value: str, width: int, label: str) -> bytes:
    try:
        raw = value.encode("ascii")
    except UnicodeEncodeError as exc:
        raise CompileError(f"{label} is not ASCII") from exc
    if not raw or len(raw) >= width:
        raise CompileError(f"{label} exceeds the DACT field")
    return raw.ljust(width, b"\0")


def verify_level_row(cache: Path, index: dict[str, list[Path]], row: int,
                     level_name: str, mlx: str) -> None:
    """Check the LevelList row against the requested name and MLX (compile_static_level rules)."""
    import importlib.util
    spec = importlib.util.spec_from_file_location("dh2_static_actor_catalogue", CATALOGUE_SOURCE)
    if spec is None or spec.loader is None:
        raise CompileError(f"cannot load LevelList decoder: {CATALOGUE_SOURCE}")
    catalogue_module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = catalogue_module  # dataclasses resolves its module by name
    spec.loader.exec_module(catalogue_module)
    levels_raw, _ = read_source(cache, index, "data/pydata/levels_pyarray.bin", 8 * 1024 * 1024)
    names_raw, _ = read_source(cache, index, "data/pydata/levels_pyarraynames.bin", 8 * 1024 * 1024)
    catalogue = catalogue_module.decode_catalogue(levels_raw, names_raw)
    if len(catalogue.levels) <= row:
        raise CompileError(f"LevelList has no row {row}")
    declaration = catalogue.levels[row]
    if declaration.name != level_name or declaration.level_file.lower() != mlx.lower():
        raise CompileError(
            f"LevelList row {row} is {declaration.name!r}/{declaration.level_file!r}; "
            f"expected {level_name!r}/{mlx!r}")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("cache_root", type=Path,
        help="extracted cache root or its enclosing directory (files/data or data)")
    parser.add_argument("--project", type=Path,
        default=Path(__file__).resolve().parents[2] / "android-native",
        help="Android project whose app/src/main/assets receives the outputs")
    parser.add_argument("--swamp-compat", action="store_true",
        help="use the historical SWAMP row/name/MLX/module/actor preset")
    parser.add_argument("--row", type=int, help="LevelList row index (validated against the cache)")
    parser.add_argument("--level-name", help="expected LevelList name for the row")
    parser.add_argument("--mlx", help="LevelFile basename under data/scene/ (e.g. 003_darkwood.mlx)")
    parser.add_argument("--out-prefix", help="output stem; default is the MLX basename without extension")
    parser.add_argument("--provenance", type=Path,
        help="static-level manifest (compile_static_level.py output); default "
             "<assets>/worlds/<out-prefix>-provenance.json")
    parser.add_argument("--expected-modules", type=int,
        help="require exactly this many Module records (preset: 9 for SWAMP)")
    parser.add_argument("--expected-actors", type=int,
        help="require exactly this many supported actors (preset: 5 for SWAMP)")
    args = parser.parse_args()

    if args.swamp_compat:
        row = args.row if args.row is not None else SWAMP_PRESET["row"]
        level_name = args.level_name or SWAMP_PRESET["level_name"]
        mlx = args.mlx or SWAMP_PRESET["mlx"]
        expected_modules = args.expected_modules if args.expected_modules is not None else SWAMP_PRESET["expected_modules"]
        expected_actors = args.expected_actors if args.expected_actors is not None else SWAMP_PRESET["expected_actors"]
    else:
        if args.row is None or not args.level_name or not args.mlx:
            parser.error("--row, --level-name and --mlx are required without --swamp-compat")
        row, level_name, mlx = args.row, args.level_name, args.mlx
        expected_modules, expected_actors = args.expected_modules, args.expected_actors
    out_prefix = args.out_prefix or Path(mlx).stem.lower()
    level_path = f"data/scene/{mlx}"

    cache = args.cache_root
    if not (cache / "data").is_dir() and (cache / "files" / "data").is_dir():
        cache = cache / "files"
    if not (cache / "data").is_dir():
        parser.error("cache_root must contain files/data or data")
    assets = args.project / "app/src/main/assets"
    provenance_path = args.provenance or (assets / "worlds" / f"{out_prefix}-provenance.json")
    index = source_index(cache)

    try:
        verify_level_row(cache, index, row, level_name, mlx)
        level_raw, _ = read_source(cache, index, level_path)
        level = parse_xml(level_raw, level_path, "Level")
        source_modules = [item for item in level.findall("GameObject")
                          if item.get("gametype") == "Module"]
        provenance = json.loads(provenance_path.read_text(encoding="utf-8"))
        if (provenance.get("source_layout") != level_path
                or provenance.get("module_count") != len(source_modules)
                or (expected_modules is not None and len(source_modules) != expected_modules)):
            raise CompileError(f"{level_path} does not match the pinned module checkpoint "
                               f"({len(source_modules)} Module records)")

        source_hashes = {entry["path"]: entry["sha256"]
                         for entry in provenance.get("source_assets", [])}
        if source_hashes.get(level_path) and digest(level_raw) != source_hashes[level_path]:
            raise CompileError(f"{level_path} differs from the pinned world checkpoint")

        character_names, _ = strings((assets / "data/character_properties_pyarraynames.bin").read_bytes())
        fields, _ = strings((assets / "data/character_properties_pystructnames.bin").read_bytes())
        model_paths, _ = strings((assets / "data/character_models_dictionary_pyarray.bin").read_bytes())
        character_rows = (assets / "data/character_properties_pyarray.bin").read_bytes()
        if (len(fields) != 224 or len(character_rows) < 4 + len(character_names) * 896
                or struct.unpack_from("<I", character_rows)[0] != len(character_names)):
            raise CompileError("original CharacterTable assets are inconsistent")
        field_index = {name: position for position, name in enumerate(fields)}
        required_fields = ("ModelFile", "AnimTable", "Scale_X", "Scale_Y", "Scale_Z")
        if any(name not in field_index for name in required_fields):
            raise CompileError("required CharacterTable field is absent")

        animation_names = ("animations_pyarraynames.bin", "animations_pystructnames.bin")
        schemas = {name: animation_sections((assets / "data" / name).read_bytes())
                   for name in animation_names}
        animation_table = parse_animation_data(
            (assets / "data/animations_pyarray.bin").read_bytes(), schemas)
        clip_paths, _ = strings((assets / "data/animations_dictionary_pyarray.bin").read_bytes())

        actors: list[dict[str, object]] = []
        skipped: collections.Counter[str] = collections.Counter()
        character_records = 0
        for room, module in enumerate(source_modules):
            name = module.get("name")
            mgp_path = canonical(module.get("mgp", ""))
            if not name or mgp_path not in source_hashes:
                raise CompileError(f"Module {room} is not in pinned provenance")
            module_transform = {
                "position": vector(module.get("position"), f"Module {room} position"),
                "rotation": vector(module.get("rotation"), f"Module {room} rotation"),
                "scale": vector(module.get("scale"), f"Module {room} scale"),
            }
            if (module_transform["rotation"] != (0.0, 0.0, 0.0)
                    or module_transform["scale"] != (1.0, 1.0, 1.0)):
                raise CompileError("non-identity Module rotation/scale is unsupported")
            expected = provenance["modules"][room]
            if (expected.get("name") != name or expected.get("mgp") != mgp_path
                    or tuple(expected.get("position", ())) != module_transform["position"]):
                raise CompileError(f"Module {room} differs from pinned provenance")
            mgp_raw, _ = read_source(cache, index, mgp_path)
            if digest(mgp_raw) != source_hashes[mgp_path]:
                raise CompileError(f"MGP differs from pinned provenance: {mgp_path}")
            mgp = parse_xml(mgp_raw, mgp_path, "Module")
            for record, obj in enumerate(mgp.findall("GameObject")):
                if obj.get("gametype") != "Character":
                    continue
                character_records += 1
                template = obj.get("charpropsname")
                authored_name = obj.get("name")
                if obj.get("_templateName") != "Monster":
                    skipped[f"non-Monster template {obj.get('_templateName') or 'none'}"] += 1
                    continue
                if not template or not authored_name:
                    skipped["Monster without template or name"] += 1
                    continue
                if condition(obj.get("activate_cond")) or condition(obj.get("deactivate_cond")):
                    skipped["Monster with activation/deactivation condition"] += 1
                    continue
                if obj.get("auto_spawn", "1") == "0":
                    skipped["Monster with auto_spawn 0"] += 1
                    continue
                if obj.get("ai_state") not in (None, "", "Idle"):
                    skipped["Monster with non-Idle initial ai_state"] += 1
                    continue
                if template not in character_names:
                    raise CompileError(f"CharacterTable row is missing: {template}")
                row_index = character_names.index(template)
                values = struct.unpack_from("<224i", character_rows, 4 + row_index * 896)
                model_id = values[field_index["ModelFile"]]
                animation_id = values[field_index["AnimTable"]]
                if not 0 <= model_id < len(model_paths):
                    raise CompileError(f"invalid ModelFile for {template}")
                if not 0 <= animation_id < len(animation_table["characters"]):
                    raise CompileError(f"invalid AnimTable for {template}")
                model_path = canonical(model_paths[model_id])
                model_name = PurePosixPath(model_path).name
                local_position = vector(obj.get("position"), f"{authored_name} position")
                local_rotation = vector(obj.get("rotation"), f"{authored_name} rotation")
                local_scale = vector(obj.get("scale"), f"{authored_name} scale")
                position = tuple(local_position[i] + module_transform["position"][i] for i in range(3))
                rotation = tuple(local_rotation[i] + module_transform["rotation"][i] for i in range(3))
                scale = tuple(local_scale[i] * values[field_index[f"Scale_{axis}"]] / 100.0
                              for i, axis in enumerate("XYZ"))
                packed = (*position, *rotation, *scale)
                if (any(not math.isfinite(value) for value in packed)
                        or any(not 0.0 < value <= 100.0 for value in scale)):
                    raise CompileError(f"invalid placement for {authored_name}")
                actors.append({
                    "kind": 1, "room": room, "name": authored_name,
                    "character": template, "model": model_name,
                    "model_path": model_path, "animation_table": animation_id,
                    "position": position, "rotation_degrees": rotation, "scale": scale,
                    "source_mgp": mgp_path, "source_record": record,
                })

        if expected_actors is not None and len(actors) != expected_actors:
            raise CompileError(f"expected {expected_actors} supported direct monsters; found {len(actors)}")
        if not actors:
            print(f"prepare_static_level_actors: warning: no supported Monster actors in {level_path}",
                  file=sys.stderr)

        data = bytearray(struct.pack("<4sIII", b"DACT", 1, len(actors), 0))
        actor_keys: set[tuple[int, str]] = set()
        asset_reports: dict[str, dict[str, object]] = {}
        total_output = 0

        def stage_source(relative: str, folder: str, name: str | None = None) -> None:
            nonlocal total_output
            raw, source = read_source(cache, index, relative)
            output_name = name or PurePosixPath(relative).name
            report = copy_asset(assets, folder, output_name, raw)
            key = str(report["asset"])
            old = asset_reports.get(key)
            if old and old["sha256"] != report["sha256"]:
                raise CompileError(f"asset basename is ambiguous: {key}")
            asset_reports[key] = {**report, "source": relative, "source_file": str(source.name)}
            total_output += len(raw)
            if total_output > MAX_OUTPUT_BYTES:
                raise CompileError("actor asset bundle exceeds the size limit")

        for actor in actors:
            key = (int(actor["room"]), str(actor["name"]))
            if key in actor_keys:
                raise CompileError(f"duplicate actor name in room {key[0]}: {key[1]}")
            actor_keys.add(key)
            data.extend(struct.pack("<II", 1, int(actor["room"])))
            data.extend(fixed_ascii(str(actor["name"]), 64, "actor name"))
            data.extend(fixed_ascii(str(actor["character"]), 64, "CharacterTable name"))
            data.extend(fixed_ascii(str(actor["model"]), 64, "model basename"))
            data.extend(struct.pack("<9f", *actor["position"], *actor["rotation_degrees"], *actor["scale"]))
            data.extend(bytes(20))
            stage_source(str(actor["model_path"]), "actors")
            model_raw, _ = read_source(cache, index, str(actor["model_path"]))
            for texture in sorted(images(model_raw)):
                texture_name = texture.lower()
                candidates = index.get(texture_name, [])
                if not candidates:
                    candidates = index.get(("pvr2_" + texture).casefold(), [])
                if not candidates:
                    raise CompileError(f"texture dependency is missing: {texture}")
                hashes = {digest(path.read_bytes()) for path in candidates}
                if len(hashes) != 1:
                    raise CompileError(f"texture basename is ambiguous: {texture}")
                relative = candidates[0].relative_to(cache).as_posix()
                stage_source(relative, "textures", texture_name)

        selected_tables = sorted({int(actor["animation_table"]) for actor in actors})
        selected_clip_ids: set[int] = set()

        def collect_sequence(sequence_id: int, stack: tuple[int, ...]) -> None:
            if (sequence_id < 0 or sequence_id >= len(animation_table["animations"])
                    or sequence_id in stack or len(stack) >= 3):
                raise CompileError("animation sequence redirect is invalid or cyclic")
            sequence = animation_table["animations"][sequence_id]
            if not sequence["Steps"]:
                raise CompileError(f"animation sequence {sequence_id} is empty")
            for step in sequence["Steps"]:
                if step["Redir"] == 1:
                    collect_sequence(step["Anim"], stack + (sequence_id,))
                elif step["Redir"] == 0 and 0 <= step["Anim"] < len(clip_paths):
                    selected_clip_ids.add(step["Anim"])
                else:
                    raise CompileError("animation sequence contains an invalid clip")

        state_report = []
        for animation_id in selected_tables:
            character = animation_table["characters"][animation_id]
            for state in ("Idle", "Walk", "Attack", "Died"):
                sequence_id = int(character[state])
                collect_sequence(sequence_id, ())
                state_report.append({"table": animation_id, "character": character["name"],
                                     "state": state, "sequence": sequence_id})
        for clip_id in sorted(selected_clip_ids):
            stage_source(canonical(clip_paths[clip_id]), "actors")

        if len(data) != 16 + len(actors) * 256:
            raise CompileError("internal DACT size mismatch")
        world_dir = assets / "worlds"
        world_dir.mkdir(parents=True, exist_ok=True)
        (world_dir / f"{out_prefix}.dact").write_bytes(data)
        report = {
            "source_layout": level_path,
            "selected_characters": len(actors),
            "source_character_records": character_records,
            "selection": SELECTION_TEXT,
            "actors": actors,
            "animation_states": state_report,
            "clip_ids": sorted(selected_clip_ids),
            "dact": {"bytes": len(data), "sha256": digest(data)},
            "assets": sorted(asset_reports.values(), key=lambda item: str(item["asset"])),
            "runtime_gaps": RUNTIME_GAPS_TEXT,
        }
        (world_dir / f"{out_prefix}-actors.json").write_text(
            json.dumps(report, indent=2) + "\n", encoding="utf-8")
    except (CompileError, OSError, KeyError, IndexError, ValueError, struct.error) as exc:
        print(f"prepare_static_level_actors: {exc}", file=sys.stderr)
        raise SystemExit(1)

    print(json.dumps({"level": level_name, "out_prefix": out_prefix,
                      "actors": len(actors), "source_characters": character_records,
                      "models": len({str(actor["model"]) for actor in actors}),
                      "staged_assets": len(asset_reports), "dact_bytes": len(data),
                      "dact_sha256": digest(data),
                      "skipped": dict(sorted(skipped.items()))}, sort_keys=True))


if __name__ == "__main__":
    main()
