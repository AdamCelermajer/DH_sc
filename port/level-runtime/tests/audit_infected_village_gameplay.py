#!/usr/bin/env python3
"""Audit authored Infected Village records and packed scripts; execute nothing."""
from __future__ import annotations

import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path, PurePosixPath
import sys
import xml.etree.ElementTree as ET

sys.dont_write_bytecode = True
HERE = Path(__file__).resolve().parents[1]
REPO = HERE.parents[1]
sys.path.insert(0, str(REPO / "port/level-catalogue"))
sys.path.insert(0, str(REPO / "port/pydata-scripts"))
import catalogue  # noqa: E402
import pydata_scripts  # noqa: E402

LEVEL_NAME = "INFECTED_VILLAGE_01"
MAX_XML_BYTES = 8 * 1024 * 1024
MAX_TABLE_BYTES = pydata_scripts.MAX_TABLE_BYTES


class AuditError(ValueError):
    """Input or authored-data mismatch found by this audit."""


def canonical_cache_path(value: str) -> str:
    normalized = value.replace("\\", "/")
    try:
        normalized.encode("ascii")
    except UnicodeEncodeError as exc:
        raise AuditError(f"cache path is not ASCII: {value!r}") from exc
    normalized = normalized.lower()
    if normalized.startswith("data/iphone/"):
        normalized = "data/" + normalized[len("data/iphone/"):]
    path = PurePosixPath(normalized)
    if (not normalized.startswith("data/") or path.is_absolute()
            or ":" in normalized or "#" in normalized
            or any(part in ("", ".", "..") for part in normalized.split("/"))):
        raise AuditError(f"unsafe cache path {value!r}")
    return normalized


class CacheInputs:
    def __init__(self, root: Path):
        self.root = root.resolve()
        self.data: dict[str, bytes] = {}
        self.hashes: dict[str, str] = {}

    def read(self, relative: str, limit: int) -> bytes:
        canonical = canonical_cache_path(relative)
        if canonical in self.data:
            return self.data[canonical]
        path = self.root.joinpath(*PurePosixPath(canonical).parts).resolve()
        if path != self.root and self.root not in path.parents:
            raise AuditError(f"cache file escaped supplied root: {canonical}")
        try:
            size = path.stat().st_size
        except OSError as exc:
            raise AuditError(f"missing/unreadable cache file {canonical}: {exc}") from exc
        if size > limit:
            raise AuditError(f"cache file {canonical} is {size} bytes; limit is {limit}")
        try:
            with path.open("rb") as stream:
                data = stream.read(limit + 1)
        except OSError as exc:
            raise AuditError(f"cannot read cache file {canonical}: {exc}") from exc
        if len(data) > limit:
            raise AuditError(f"cache file {canonical} grew beyond {limit} bytes")
        self.data[canonical] = data
        self.hashes[canonical] = hashlib.sha256(data).hexdigest()
        return data


def _xml_objects(raw: bytes, path: str, expected_root: str) -> list[ET.Element]:
    try:
        root = ET.fromstring(raw)
    except ET.ParseError as exc:
        raise AuditError(f"cannot parse {path}: {exc}") from exc
    if root.tag != expected_root:
        raise AuditError(f"{path}: expected <{expected_root}>, got <{root.tag}>")
    objects = root.findall("GameObject")
    if len(objects) > 4096:
        raise AuditError(f"{path}: GameObject count exceeds 4096")
    return objects


def _script_paths(script_file: str) -> tuple[str, str]:
    canonical = canonical_cache_path(script_file)
    suffix = ".pyscript"
    if not canonical.endswith(suffix):
        raise AuditError(f"LevelConfig scriptFile does not end in {suffix}: {script_file!r}")
    stem = canonical[:-len(suffix)]
    return stem + "_pyscriptnames.bin", stem + "_pyscripts.bin"


def _convert(value):
    if isinstance(value, tuple):
        return [_convert(item) for item in value]
    return value


def _record(path: str, source_index: int, element: ET.Element,
            *, module_index: int | None, asset_kind: str) -> dict[str, object]:
    return {
        "source_path": path,
        "source_record": source_index,
        "module_index": module_index,
        "asset_kind": asset_kind,
        "gametype": element.get("gametype", ""),
        "name": element.get("name", ""),
    }


def audit(cache_root: Path) -> dict[str, object]:
    inputs = CacheInputs(cache_root)
    names_path = "data/pydata/levels_pyarraynames.bin"
    levels_path = "data/pydata/levels_pyarray.bin"
    try:
        levels = catalogue.decode_catalogue(
            inputs.read(levels_path, 8 * 1024 * 1024),
            inputs.read(names_path, 8 * 1024 * 1024))
    except catalogue.CatalogueError as exc:
        raise AuditError(f"cannot decode LevelList catalogue: {exc}") from exc
    declaration = levels.levels_by_name.get(LEVEL_NAME)
    if declaration is None or declaration.file_kind != "static_mlx":
        raise AuditError(f"{LEVEL_NAME} is not a catalogue-declared static MLX")

    mlx_path = canonical_cache_path(f"data/scene/{declaration.level_file}")
    level_objects = _xml_objects(inputs.read(mlx_path, MAX_XML_BYTES), mlx_path, "Level")
    if (not level_objects or level_objects[0].get("gametype") != "LevelConfig"
            or any(item.get("gametype") not in ("LevelConfig", "Module")
                   for item in level_objects)):
        raise AuditError("Infected Village MLX has an unsupported level record layout")
    module_elements = [item for item in level_objects if item.get("gametype") == "Module"]
    if len(module_elements) != 2:
        raise AuditError(f"expected two authored module placements, found {len(module_elements)}")

    level_records = []
    for index, element in enumerate(level_objects):
        module_index = index - 1 if element.get("gametype") == "Module" else None
        level_records.append(_record(mlx_path, index, element,
            module_index=module_index, asset_kind="mlx"))

    module_records: list[dict[str, object]] = []
    module_type_counts: dict[str, dict[str, dict[str, int]]] = {}
    module_source_rows = []
    for module_index, module in enumerate(module_elements):
        module_name = module.get("name", "")
        files_for_module = {}
        module_type_counts[str(module_index)] = {}
        for field, asset_kind, root_tag in (("mgp", "mgp", "Module"),
                                            ("mvp", "mvp", "Module")):
            source = module.get(field)
            if not source:
                raise AuditError(f"module {module_index} has no {field} source path")
            source_path = canonical_cache_path(source)
            elements = _xml_objects(inputs.read(source_path, MAX_XML_BYTES), source_path, root_tag)
            counts = Counter(element.get("gametype", "") for element in elements)
            module_type_counts[str(module_index)][asset_kind] = dict(sorted(counts.items()))
            files_for_module[asset_kind] = source_path
            for source_index, element in enumerate(elements):
                row = _record(source_path, source_index, element,
                    module_index=module_index, asset_kind=asset_kind)
                row["module_name"] = module_name
                if row["gametype"] == "Character":
                    row["auto_spawn"] = element.get("auto_spawn")
                    row["ai_state"] = element.get("ai_state")
                module_records.append(row)
        module_source_rows.append({
            "module_index": module_index,
            "module_source_record": module_index + 1,
            "module_name": module_name,
            "mgp": files_for_module["mgp"],
            "mvp": files_for_module["mvp"],
        })

    # The level-owned scriptFile path determines this packed pair; names and
    # programs are decoded by the existing bounded PyData table reader.
    script_file = level_objects[0].get("scriptFile")
    if not script_file:
        raise AuditError("LevelConfig has no scriptFile")
    level_names_path, level_programs_path = _script_paths(script_file)
    try:
        level_scripts = pydata_scripts.decode_script_table(
            inputs.read(level_names_path, MAX_TABLE_BYTES),
            inputs.read(level_programs_path, MAX_TABLE_BYTES))
        common_scripts = pydata_scripts.decode_script_table(
            inputs.read("data/pydata/scripts_pyscriptnames.bin", MAX_TABLE_BYTES),
            inputs.read("data/pydata/scripts_pyscripts.bin", MAX_TABLE_BYTES))
    except pydata_scripts.DecodeError as exc:
        raise AuditError(f"cannot decode packed script tables: {exc}") from exc

    expected_script_names = (
        "Ambush", "ExitParty", "Inspect_Cliff", "Inspect_Well",
        "ResumeLevelMusic", "enterLocation_Infected",
    )
    if level_scripts.names != expected_script_names:
        raise AuditError(f"level script name/order mismatch: {level_scripts.names!r}")
    total_commands = sum(len(script.commands) for script in level_scripts.scripts)
    command_counts = tuple(len(script.commands) for script in level_scripts.scripts)
    if total_commands != 36 or command_counts != (5, 5, 12, 12, 1, 1):
        raise AuditError(f"level script command counts changed: {command_counts!r}")

    scripts_by_name = {script.name: script for script in level_scripts.scripts}
    ambush = scripts_by_name["Ambush"]
    ambush_requests = [
        {"command_index": command_index, "byte_offset": command.byte_offset,
         "actor_name": command.payload[0]}
        for command_index, command in enumerate(ambush.commands)
        if command.name == "SpawnCharacter"
    ]
    if len(ambush_requests) != 5 or len(ambush_requests) != len(ambush.commands):
        raise AuditError("Ambush must decode to its five authored SpawnCharacter requests")
    expected_ambush_actors = (
        "_prim_tmp_infected05", "_prim_tmp_infected07", "_prim_tmp_infected17",
        "_prim_tmp_infected06", "_prim_tmp_infected16",
    )
    if tuple(item["actor_name"] for item in ambush_requests) != expected_ambush_actors:
        raise AuditError("Ambush SpawnCharacter request order changed")

    named_records: dict[str, list[dict[str, object]]] = {}
    character_records: dict[str, list[dict[str, object]]] = {}
    for record in module_records:
        named_records.setdefault(str(record["name"]), []).append(record)
        if record["gametype"] == "Character":
            character_records.setdefault(str(record["name"]), []).append(record)
    for request in ambush_requests:
        request["static_record_name_matches"] = [
            {"module_index": row["module_index"], "source_path": row["source_path"],
             "source_record": row["source_record"], "gametype": row["gametype"]}
            for row in named_records.get(str(request["actor_name"]), [])
        ]
        request["static_character_record_matches"] = [
            {"module_index": row["module_index"], "source_path": row["source_path"],
             "source_record": row["source_record"],
             "auto_spawn": row.get("auto_spawn"), "ai_state": row.get("ai_state")}
            for row in character_records.get(str(request["actor_name"]), [])
        ]
    if [len(item["static_character_record_matches"]) for item in ambush_requests] != [1, 1, 0, 1, 1]:
        raise AuditError("Ambush static Character record cross-references changed")
    if [len(item["static_record_name_matches"]) for item in ambush_requests] != [1, 1, 0, 1, 1]:
        raise AuditError("Ambush name lookup across all MGP/MVP records changed")
    for item in (ambush_requests[0], ambush_requests[1],
                 ambush_requests[3], ambush_requests[4]):
        match = item["static_character_record_matches"][0]
        if match["auto_spawn"] != "0" or match["ai_state"] != "Limbus":
            raise AuditError(f"ambush actor authored state changed: {item['actor_name']}")

    ambush_triggers = [
        record for record in module_records
        if record["gametype"] == "TriggerZone"
        and record["name"] == "_prim_TriggerZone_ambush"
        and record["source_path"].endswith(".mgp")
        and record.get("module_index") in (0, 1)
    ]
    trigger_elements = []
    for record in ambush_triggers:
        source_path = str(record["source_path"])
        source_elements = _xml_objects(inputs.data[source_path], source_path, "Module")
        trigger = source_elements[int(record["source_record"])]
        if trigger.get("script") != "Ambush":
            raise AuditError("an authored ambush trigger does not refer to the Ambush script")
        trigger_elements.append({
            "module_index": record["module_index"],
            "source_path": source_path,
            "source_record": record["source_record"],
            "name": record["name"],
            "gametype": record["gametype"],
            "script_name": trigger.get("script"),
            "triggercount": trigger.get("triggercount"),
            "position": trigger.get("position"),
            "spawn_character_requests": [item["actor_name"] for item in ambush_requests],
        })
    if len(trigger_elements) != 2 or [item["module_index"] for item in trigger_elements] != [0, 1]:
        raise AuditError("expected one TriggerZone_ambush in each authored module")
    if [item["source_record"] for item in trigger_elements] != [20, 0]:
        raise AuditError("TriggerZone_ambush source record ordering changed")

    spawn_references = []
    for script in level_scripts.scripts:
        for command_index, command in enumerate(script.commands):
            if command.name == "SpawnCharacter":
                actor_name = command.payload[0]
                spawn_references.append({
                    "script_name": script.name,
                    "command_index": command_index,
                    "byte_offset": command.byte_offset,
                    "actor_name": actor_name,
                    "static_record_name_matches": [
                        {"module_index": row["module_index"],
                         "source_path": row["source_path"],
                         "source_record": row["source_record"],
                         "gametype": row["gametype"]}
                        for row in named_records.get(actor_name, [])
                    ],
                    "static_character_record_matches": [
                        {"module_index": row["module_index"],
                         "source_path": row["source_path"],
                         "source_record": row["source_record"],
                         "auto_spawn": row.get("auto_spawn"),
                         "ai_state": row.get("ai_state")}
                        for row in character_records.get(actor_name, [])
                    ],
                })
    exit_party_refs = [item for item in spawn_references if item["script_name"] == "ExitParty"]
    if ([item["actor_name"] for item in exit_party_refs] != [
            "_prim_tmp_infected_exit01", "_prim_tmp_infected_exit02",
            "_prim_tmp_infected_exit03"]
            or any(item["static_record_name_matches"] for item in exit_party_refs)):
        raise AuditError("ExitParty static MGP/MVP name references changed")

    exits = []
    for record in module_records:
        if record["gametype"] != "TriggerZoneExitLevel":
            continue
        source_path = str(record["source_path"])
        source_elements = _xml_objects(inputs.data[source_path], source_path, "Module")
        obj = source_elements[int(record["source_record"])]
        target_name = obj.get("levelName")
        try:
            entrypoint_id = int(obj.attrib["entrypointID"], 10)
        except (KeyError, ValueError) as exc:
            raise AuditError(f"invalid exit entrypoint in {source_path} record {record['source_record']}") from exc
        target = levels.levels_by_name.get(target_name or "")
        if target is None or entrypoint_id < 0:
            raise AuditError(f"exit has unknown target or negative entrypoint: {target_name!r}/{entrypoint_id}")
        fasttravel_name = obj.get("fasttravel")
        fasttravel_destination = levels.fast_travel_by_name.get(fasttravel_name or "")
        exits.append({
            "module_index": record["module_index"],
            "source_path": source_path,
            "source_record": record["source_record"],
            "name": record["name"],
            "target_level_name": target.name,
            "target_entrypoint_id": entrypoint_id,
            "target_level_file": target.level_file,
            "target_level_file_kind": target.file_kind,
            "fasttravel_field_raw": fasttravel_name,
            "fasttravel_catalogue_row": None if fasttravel_destination is None else {
                "name": fasttravel_destination.name,
                "level_name": fasttravel_destination.level_name,
                "entrypoint_id": fasttravel_destination.entrypoint_id,
            },
        })
    if len(exits) != 2:
        raise AuditError(f"expected exactly two authored exit records, found {len(exits)}")
    expected_exits = {
        ("_prim_ExitLevelZone_toDW", 0, "DARKWOOD", 9, "003_darkwood.mlx",
         "a08_DARKWOOD_INFECTED_VILLAGE_01_ENTRANCE"),
        ("_prim_ExitLevelZone_toBasement", 0, "SECRET_BASEMENT_01", 0,
         "006_basement.rule.xml", "SWAMP_BEFORE_SWAMPKING"),
    }
    actual_exits = {
        (item["name"], item["module_index"], item["target_level_name"],
         item["target_entrypoint_id"], item["target_level_file"],
         item["fasttravel_field_raw"])
        for item in exits
    }
    if actual_exits != expected_exits:
        raise AuditError(f"authored exit targets changed: {actual_exits!r}")

    quest_zones = [record for record in module_records
                   if record["gametype"] == "QuestMoveInZone"]
    if len(quest_zones) != 1:
        raise AuditError(f"expected one QuestMoveInZone, found {len(quest_zones)}")
    quest_record = quest_zones[0]
    quest_path = str(quest_record["source_path"])
    quest_object = _xml_objects(inputs.data[quest_path], quest_path, "Module")[
        int(quest_record["source_record"])]
    quest_move_in_zone = {
        **quest_record,
        "position": quest_object.get("position"),
        "activate_cond": quest_object.get("activate_cond"),
        "collision_effect_executed": False,
    }
    if (quest_record["module_index"], quest_record["source_record"],
            quest_record["name"], quest_object.get("activate_cond")) != (
            1, 1, "_prim_QuestMoveInzone_wellstart", ""):
        raise AuditError("QuestMoveInZone source record or authored fields changed")

    common_references = []
    for script in level_scripts.scripts:
        for command_index, command in enumerate(script.commands):
            if command.name != "ExecScript":
                continue
            is_absolute, script_id, _args, _last_flag = command.payload
            target = None
            if is_absolute and 0 <= script_id < len(common_scripts.scripts):
                target = common_scripts.scripts[script_id].name
            common_references.append({
                "source_script": script.name,
                "command_index": command_index,
                "byte_offset": command.byte_offset,
                "absolute_common_id": script_id if is_absolute else None,
                "resolved_common_name": target,
            })
    if (len(common_scripts.scripts) != 15
            or sum(len(script.commands) for script in common_scripts.scripts) != 81
            or [item["resolved_common_name"] for item in common_references]
                != ["BeginScriptedCutScene", "EndScriptedCutScene",
                    "BeginScriptedCutScene", "EndScriptedCutScene"]):
        raise AuditError("common ExecScript references did not resolve to the expected cutscene pair")

    all_source_records = level_records + module_records
    if len(level_records) != 3 or len(module_records) != 50 or len(all_source_records) != 53:
        raise AuditError(
            f"expected 3 MLX records + 50 module records (53 total), got "
            f"{len(level_records)} + {len(module_records)}")
    expected_counts = {
        "0": {
            "mgp": {"Character": 26, "DestructibleContainer": 3, "Door": 3,
                    "OpenableContainer": 2, "SoundEmitter": 1, "SpawnPoint": 1,
                    "TriggerZone": 1, "TriggerZoneExitLevel": 2},
            "mvp": {"AnimatedDecor": 4},
        },
        "1": {"mgp": {"QuestMoveInZone": 1, "TriggerZone": 1},
              "mvp": {"AnimatedDecor": 5}},
    }
    if module_type_counts != expected_counts:
        raise AuditError(f"module/type record counts changed: {module_type_counts!r}")
    expected_files = {
        "data/pydata/levels_pyarray.bin", "data/pydata/levels_pyarraynames.bin",
        "data/scene/005_infectedvillage.mlx",
        "data/3d/modules/infectedvillage/mgp/infected01.mgp",
        "data/3d/modules/infectedvillage/mvp/infected01.mvp",
        "data/3d/modules/infectedvillage/mgp/infected02.mgp",
        "data/3d/modules/infectedvillage/mvp/infected02.mvp",
        "data/pydata/scripts/005_infectedvillage_pyscriptnames.bin",
        "data/pydata/scripts/005_infectedvillage_pyscripts.bin",
        "data/pydata/scripts_pyscriptnames.bin", "data/pydata/scripts_pyscripts.bin",
    }
    if set(inputs.hashes) != expected_files:
        raise AuditError(f"source cache-file closure changed: {set(inputs.hashes)!r}")

    script_rows = []
    for script in level_scripts.scripts:
        script_rows.append({
            "table_index": script.table_index,
            "name": script.name,
            "command_count": len(script.commands),
            "byte_offset": script.byte_offset,
            "byte_end": script.byte_end,
            "commands": [{
                "byte_offset": command.byte_offset,
                "name": command.name,
                "payload": _convert(command.payload),
            } for command in script.commands],
        })

    return {
        "scope": "authored cache records and packed command decoding only; no gameplay dispatch",
        "level_name": LEVEL_NAME,
        "level_file": declaration.level_file,
        "level_record_count": len(level_records),
        "module_entity_record_count": len(module_records),
        "total_source_record_count": len(all_source_records),
        "module_order": module_source_rows,
        "module_type_counts": module_type_counts,
        "records": all_source_records,
        "script_tables": {
            "level_names_path": level_names_path,
            "level_programs_path": level_programs_path,
            "level_script_count": len(level_scripts.scripts),
            "level_command_count": total_commands,
            "scripts": script_rows,
            "common_names_path": "data/pydata/scripts_pyscriptnames.bin",
            "common_programs_path": "data/pydata/scripts_pyscripts.bin",
            "common_script_count": len(common_scripts.scripts),
            "common_command_count": sum(len(script.commands) for script in common_scripts.scripts),
            "exec_script_references": common_references,
        },
        "ambush_spawn_character_requests": ambush_requests,
        "ambush_triggers": trigger_elements,
        "all_level_spawn_character_references": spawn_references,
        "exits": exits,
        "quest_move_in_zone": quest_move_in_zone,
        "source_sha256": dict(sorted(inputs.hashes.items())),
        "executed": False,
        "characters_activated": False,
        "collision_dispatched": False,
        "dialogues_started": False,
        "camera_started": False,
        "exits_dispatched": False,
        "quest_events_dispatched": False,
    }


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache-root", type=Path, required=True,
                        help="caller-supplied extracted cache root")
    parser.add_argument("--report", type=Path,
                        help="optional JSON report path")
    args = parser.parse_args(argv)
    try:
        result = audit(args.cache_root)
    except (AuditError, OSError, catalogue.CatalogueError,
            pydata_scripts.DecodeError) as exc:
        print(f"Infected Village gameplay audit failed: {exc}", file=sys.stderr)
        return 1
    output = json.dumps(result, indent=2, ensure_ascii=False) + "\n"
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(output, encoding="utf-8")
    print(output, end="")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
