#!/usr/bin/env python3
"""Exercise the actor state-request translator against checked-in host readers."""
from __future__ import annotations

import argparse
import ctypes
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

HERE = Path(__file__).resolve().parents[1]
PORT = HERE.parent
REPO = PORT.parent
DEFAULT_CACHE = REPO.parent / "cache" / "files"


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def import_file(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot import checked-in reader {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


class PyCstView(ctypes.Structure):
    _fields_ = [("bytes", ctypes.c_void_p), ("size", ctypes.c_uint32),
                ("groups", ctypes.c_uint32), ("entries", ctypes.c_uint32)]


class PyCstResult(ctypes.Structure):
    _fields_ = [("found", ctypes.c_uint32), ("value", ctypes.c_int32)]


def compile_pycst() -> Path:
    build = HERE / "build"
    build.mkdir(parents=True, exist_ok=True)
    suffix = ".dll" if os.name == "nt" else ".so"
    output = build / ("pycst-host" + suffix)
    compiler = shutil.which(os.environ.get("CC", "gcc"))
    if compiler is None:
        raise RuntimeError("gcc not found; set CC to a host C compiler")
    command = [compiler, "-std=c99", "-O2", "-shared", "-Wall", "-Wextra", "-Werror"]
    if os.name != "nt":
        command.append("-fPIC")
    command += [str(PORT / "pydata-constants" / "constants.c"), "-o", str(output)]
    subprocess.run(command, check=True)
    return output


def read_ai_state_constants(library_path: Path, raw: bytes) -> dict[str, int]:
    lib = ctypes.CDLL(str(library_path.resolve()))
    lib.dh2_pycst_open.argtypes = [ctypes.POINTER(PyCstView), ctypes.c_void_p,
                                  ctypes.c_uint32]
    lib.dh2_pycst_open.restype = ctypes.c_uint32
    lib.dh2_pycst_get.argtypes = [ctypes.POINTER(PyCstView), ctypes.c_void_p,
                                 ctypes.c_uint32, ctypes.c_void_p, ctypes.c_uint32,
                                 ctypes.POINTER(PyCstResult)]
    lib.dh2_pycst_get.restype = ctypes.c_uint32
    owner = ctypes.create_string_buffer(raw)
    view = PyCstView()
    if lib.dh2_pycst_open(ctypes.byref(view), owner, len(raw)) != 0:
        raise RuntimeError("checked-in PyData constants reader rejected ai_pycst.bin")
    result: dict[str, int] = {}
    for name in ("Limbus", "Spawn", "Idle", "Move", "Attack", "Stunned", "COUNT"):
        group = ctypes.create_string_buffer(b"AIStates")
        key = ctypes.create_string_buffer(name.encode("ascii"))
        value = PyCstResult()
        status = lib.dh2_pycst_get(ctypes.byref(view), group, 8, key, len(name),
                                   ctypes.byref(value))
        if status != 0 or value.found != 1:
            raise RuntimeError(f"AIStates.{name} missing from source constants")
        result[name] = value.value
    return result


def function_body(index_path: Path, symbol: str) -> tuple[dict[str, Any], str]:
    for line in index_path.read_text(encoding="utf-8").splitlines():
        row = json.loads(line)
        if row.get("name") != symbol:
            continue
        source = index_path.parent / row["file"]
        raw = source.read_bytes()
        start, size = int(row["byte_offset"]), int(row["byte_length"])
        return row, raw[start:start + size].decode("utf-8")
    raise RuntimeError(f"native function not found in checked-in index: {symbol}")


def verify_native_request_path() -> dict[str, Any]:
    native = REPO / "recovered/native/decompiled/libDungeonHunter2.so"
    index = native / "function-index.jsonl"
    script_row, script_body = function_body(index,
        "_ZN21Script_SpawnCharacter7ExecuteEbi")
    state_row, state_body = function_body(index,
        "_ZN16CharStateMachine16SM_SetSpawnStateEbb")
    if "SM_SetSpawnStateEbb(iVar1 + 0x4fc,0,0)" not in script_body:
        raise RuntimeError("SpawnCharacter no longer requests SM_SetSpawnState(false,false)")
    if "_SetStateEiiPv(param_1,1,0xffffffff)" not in state_body:
        raise RuntimeError("SM_SetSpawnState(false,false) no longer queues state id 1")
    limbus_row, limbus_body = function_body(index,
        "_ZN8CSLimbus6OnInitEiP9CharacterP16CharStateMachine")
    if "param_2,0x2f,1" not in limbus_body or "Character9CSM_Spawn" not in limbus_body:
        raise RuntimeError("CSLimbus event 0x2f -> CSM_Spawn registration changed")
    spawn_focus_row, spawn_focus_body = function_body(index,
        "_ZN7CSSpawn7OnFocusEiP9CharacterP16CharStateMachineiiPv")
    if "uVar3 + 0x80" not in spawn_focus_body:
        raise RuntimeError("CSSpawn animation table slot access changed")

    limbus_asm = REPO / "recovered/native/assembly/libDungeonHunter2.so/CSLimbus-f65fc268875e-001.asm"
    asm_text = limbus_asm.read_text(encoding="utf-8")
    for address in ("0x003bffe4", "0x003bffe8"):
        marker = f"; FUNCTION {address}, declared_size=4"
        begin = asm_text.find(marker)
        if begin < 0:
            raise RuntimeError(f"CSLimbus handler missing: {address}")
        block = asm_text[begin:asm_text.find("; FUNCTION", begin + len(marker))]
        if "bx lr" not in block:
            raise RuntimeError(f"CSLimbus handler is no longer a no-op: {address}")
    return {
        "spawn_script_function": {"symbol": script_row["name"],
                                  "elf_address": script_row["elf_address"]},
        "spawn_state_function": {"symbol": state_row["name"],
                                 "elf_address": state_row["elf_address"]},
        "limbus_init_function": {"symbol": limbus_row["name"],
                                 "elf_address": limbus_row["elf_address"],
                                 "registered_event": 47,
                                 "callback": "Character::CSM_Spawn"},
        "spawn_focus_function": {"symbol": spawn_focus_row["name"],
                                 "elf_address": spawn_focus_row["elf_address"],
                                 "char_anim_table_record_offset": 128},
        "limbus_on_update_and_event_noop": True,
        "limbus_assembly_sha256": sha256(limbus_asm),
        "function_index_sha256": sha256(index),
    }


def build_translation_report(cache_root: Path, library: Path) -> dict[str, Any]:
    sys.path.insert(0, str(PORT / "level-runtime" / "tests"))
    sys.path.insert(0, str(PORT / "pydata-scripts"))
    gameplay_reader = import_file("infected_gameplay_reader",
        PORT / "level-runtime" / "tests" / "audit_infected_village_gameplay.py")
    runtime = import_file("actor_ai_runtime", HERE / "actor_ai_runtime.py")

    source = gameplay_reader.audit(cache_root)
    manifest_path = PORT / "level-runtime" / "infected-village-actor-assets.json"
    compatibility_path = PORT / "infected-actor-compatibility" / "validation.json"
    manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
    compatibility = json.loads(compatibility_path.read_text(encoding="utf-8"))
    asset_verifier = import_file("infected_actor_asset_verifier",
        PORT / "level-runtime" / "tests" / "verify_infected_village_actor_assets.py")
    asset_closure = asset_verifier.verify(cache_root, manifest_path)
    if compatibility.get("source_manifest_sha256") != sha256(manifest_path):
        raise RuntimeError("six-model skeleton audit does not pin the current source manifest")

    models = manifest["template_choices"]["models"]
    model_ids = {model["character_table_id"] for model in models}
    if len(models) != 6 or model_ids != {row["character_table_id"] for row in compatibility["models"]}:
        raise RuntimeError("six model choices do not match the checked skeleton audit")
    if any(row["status"] != "ok" for row in compatibility["models"]):
        raise RuntimeError("one or more infected skeleton readers failed")
    idle_walk_rows = compatibility["model_clip_results"]
    variant_anim_checks = {}
    for model in models:
        rows = [row for row in idle_walk_rows
                if row["model_key"] == model["model_key"]
                and row["state"] in ("Idle", "Walk")]
        expected_counts = {"Idle": 2, "Walk": 1}
        counts = {state: sum(1 for row in rows if row["state"] == state)
                  for state in expected_counts}
        if counts != expected_counts or any(row["status"] != "compatible" for row in rows):
            raise RuntimeError(f"Idle/Walk skeleton compatibility changed for {model['model_key']}")
        variant_anim_checks[model["model_key"]] = {
            "idle_clip_segments_compatible": sum(
                row["segment_fully_compatible_count"] for row in rows if row["state"] == "Idle"),
            "idle_clip_segments_tested": sum(
                row["segment_count"] for row in rows if row["state"] == "Idle"),
            "walk_clip_segments_compatible": sum(
                row["segment_fully_compatible_count"] for row in rows if row["state"] == "Walk"),
            "walk_clip_segments_tested": sum(
                row["segment_count"] for row in rows if row["state"] == "Walk"),
        }

    cache = cache_root.resolve()
    ai_constants_path = cache / "data/pydata/ai_pycst.bin"
    ai_constants_raw = ai_constants_path.read_bytes()
    state_values = read_ai_state_constants(library, ai_constants_raw)
    if state_values["Limbus"] != 0 or state_values["Spawn"] != 1:
        raise RuntimeError(f"AIStates source constants changed: {state_values}")

    expected = runtime.StateIds(limbus=state_values["Limbus"], spawn=state_values["Spawn"])
    records = {row["name"]: row for row in manifest["source_records"]["records"]}
    requests = source["ambush_spawn_character_requests"]
    translations = []
    for request in requests:
        name = request["actor_name"]
        raw_match = request["static_character_record_matches"]
        source_record = records.get(name)
        if source_record is None:
            matches: list[dict[str, Any]] = []
        else:
            attrs = source_record["attributes"]
            matches = [{
                "name": name,
                "source_record": source_record["source_record"],
                "ai_state": attrs.get("ai_state"),
                "auto_spawn": attrs.get("auto_spawn"),
                "template_name": attrs.get("char_template"),
            }]
            if len(raw_match) != 1:
                raise RuntimeError(f"source parser match count changed for {name}")
        translated = runtime.translate_spawn_command(
            {"name": "SpawnCharacter", "payload": [name]}, matches, models, expected)
        translations.append({
            "command_index": request["command_index"],
            "byte_offset": request["byte_offset"],
            **translated,
        })

    accepted = [row for row in translations if row["status"] == "translated_explicit_request"]
    unresolved = [row for row in translations if row["status"] == "unresolved_static_character"]
    if (len(requests), len(accepted), len(unresolved)) != (5, 4, 1):
        raise RuntimeError("Ambush SpawnCharacter translation totals changed")
    if any(row["animation_request"] is not None for row in accepted):
        raise RuntimeError("fail-closed translator unexpectedly emitted an animation")
    variant_pairs = sum(len(row["model_choices"]) for row in accepted)

    anim_states = manifest["animation"]["states"]
    tested_clip_summaries = []
    for state in compatibility["table_evidence"]["tested_states"]:
        state_name = state["state"].replace("-talk", "")
        clip_rows = [row for row in compatibility["clip_file_summary"]
                     if row["anim_tpl_id"] == state["anim_tpl_id"]]
        tested_clip_summaries.append({
            "state": state["state"], "anim_tpl_id": state["anim_tpl_id"],
            "manifest_mapping_count": sum(1 for row in anim_states
                                           if row["state"] == state_name),
            "clip_compatibility": [{
                "path": row["path"],
                "model_pairs_tested": row["model_pairs_tested"],
                "compatible_model_pairs": row["compatible_model_pairs"],
                "segments_tested": row["segments_tested"],
                "fully_compatible_segments": row["fully_compatible_segments"],
                "unresolved_target_occurrences": row["unresolved_target_occurrences"],
                "unresolved_target_names": row["unresolved_target_names"],
            } for row in clip_rows],
            "autonomous_transition_or_request_emitted": False,
        })

    typed_payloads = (
        "data/pydata/ai_pyarray.bin",
        "data/pydata/ai_pyarraynames.bin",
        "data/pydata/ai_pystructnames.bin",
        "data/pydata/character_templates_pyarray.bin",
        "data/pydata/character_templates_pyarraynames.bin",
        "data/pydata/character_templates_pystructnames.bin",
    )
    input_hashes = dict(source["source_sha256"])
    for relative in typed_payloads:
        path = cache / Path(relative)
        input_hashes[relative] = sha256(path)

    native_evidence = verify_native_request_path()
    struct_trace_path = REPO / "reports/pydata-struct-name-trace.json"
    array_trace_path = REPO / "reports/pydata-array-name-trace.json"
    struct_trace = json.loads(struct_trace_path.read_text(encoding="utf-8"))
    array_trace = json.loads(array_trace_path.read_text(encoding="utf-8"))
    struct_names = {row["class"]: row["names"] for row in struct_trace["tables"]}
    array_name_counts = {
        table["class"]: table["count"]
        for file_row in array_trace["files"] for table in file_row["tables"]
        if table["class"] in ("AITable", "Charater_Templates", "CharAnimTable")
    }
    native_files = [
        REPO / "recovered/native/decompiled/libDungeonHunter2.so/functions-006.pseudo.c",
        REPO / "recovered/native/decompiled/libDungeonHunter2.so/functions-003.pseudo.c",
        REPO / "recovered/native/assembly/libDungeonHunter2.so/CSLimbus-f65fc268875e-001.asm",
        REPO / "recovered/native/assembly/libDungeonHunter2.so/CSSpawn-6f640697797f-001.asm",
    ]

    return {
        "format": "dh2.actor-ai-runtime.v1",
        "scope": "Explicit Infected Village Ambush SpawnCharacter state requests only; no AI simulation or animation playback.",
        "complete_game": False,
        "scripts_executed": False,
        "trigger_conditions_evaluated": False,
        "cache_payloads_embedded": False,
        "state_constant_reader": "port/pydata-constants/constants.c",
        "script_and_level_reader": "port/level-runtime/tests/audit_infected_village_gameplay.py + port/pydata-scripts/pydata_scripts.py",
        "state_constants": state_values,
        "native_evidence": native_evidence,
        "source_manifest": "port/level-runtime/infected-village-actor-assets.json",
        "source_manifest_sha256": sha256(manifest_path),
        "source_manifest_cache_verification": {
            "status": asset_closure["status"],
            "source_mgp_records_checked": asset_closure["source_mgp_records_checked"],
            "asset_paths_checked": asset_closure["asset_paths_checked"],
            "data_link_checks": asset_closure["data_link_checks"],
        },
        "skeleton_compatibility_report": "port/infected-actor-compatibility/validation.json",
        "skeleton_compatibility_report_sha256": sha256(compatibility_path),
        "initial_records": {
            "source_character_record_count": len(records),
            "records": [{"source_record": row["source_record"],
                         "name": row["name"],
                         "template_name": row["attributes"]["char_template"],
                         "initial_state": row["attributes"]["ai_state"],
                         "initial_state_id": state_values[row["attributes"]["ai_state"]],
                         "auto_spawn": row["attributes"]["auto_spawn"]}
                        for row in records.values()],
        },
        "model_variants": [{
            "character_table_id": model["character_table_id"],
            "character_name": model["character_name"], "model_key": model["model_key"],
            "model_status": next(row["status"] for row in compatibility["models"]
                                 if row["character_table_id"] == model["character_table_id"]),
            "idle_walk_skeleton_check": variant_anim_checks[model["model_key"]],
        } for model in models],
        "animation_candidates": tested_clip_summaries,
        "translation_summary": {
            "ambush_script_requests": len(requests),
            "static_character_matches": len(accepted),
            "unresolved_requests": len(unresolved),
            "explicit_spawn_state_requests": len(accepted),
            "actor_model_variant_pairs": variant_pairs,
            "animation_requests_emitted": 0,
            "request_rows": translations,
        },
        "unresolved_template_ai_data": {
            "typed_template_and_ai_records_decoded": False,
            "reason": (
                "The checked-in PyData readers cover array/struct names and integer constants, "
                "but not typed AIProps, CharTemplate, or CharAnimTable records. The original "
                "native record readers are documented in the recovered assembly/decompilation, "
                "but no checked-in host decoder exposes these record values."
            ),
            "checked_in_struct_field_names_only": {
                "AIProps": struct_names.get("AIProps", []),
                "CharTemplate": struct_names.get("CharTemplate", []),
            },
            "checked_in_array_name_counts_only": array_name_counts,
            "struct_name_trace_sha256": sha256(struct_trace_path),
            "array_name_trace_sha256": sha256(array_trace_path),
            "cache_files_hashed": list(typed_payloads) + ["data/pydata/ai_pycst.bin"],
            "transition_condition_recovered": False,
            "movement_inferred": False,
            "attack_inferred": False,
            "idle_inferred": False,
        },
        "cache_source_sha256": input_hashes,
        "checked_in_native_source_sha256": {path.relative_to(REPO).as_posix(): sha256(path)
                                             for path in native_files},
        "translation_policy": {
            "SpawnCharacter_to_Spawn": "supported only as a conditional script state request for a unique static Character match",
            "model_selection": "all six alternatives retained; selection policy unresolved",
            "animation_request": "withheld",
            "ai_decisions": "none",
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, default=DEFAULT_CACHE)
    parser.add_argument("--report", type=Path, default=HERE / "validation.json")
    args = parser.parse_args()
    cache = args.cache.resolve()
    if not cache.is_dir():
        raise SystemExit(f"cache/files not found: {cache}")
    library = compile_pycst()
    report = build_translation_report(cache, library)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2, ensure_ascii=False) + "\n",
                           encoding="utf-8")
    print(json.dumps({"format": report["format"],
                      "explicit_spawn_state_requests": report["translation_summary"]["explicit_spawn_state_requests"],
                      "unresolved_requests": report["translation_summary"]["unresolved_requests"],
                      "variant_pairs": report["translation_summary"]["actor_model_variant_pairs"],
                      "animation_requests_emitted": report["translation_summary"]["animation_requests_emitted"],
                      "report": str(args.report)}))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
