#!/usr/bin/env python3
"""Run the source-backed character-template and Spawn transition checks."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPO = HERE.parents[2]
sys.path.insert(0, str(MODULE))

from actor_spawn_runtime import (  # noqa: E402
    DecodeError,
    apply_spawn_character_request,
    choose_template_property,
    parse_character_templates,
    parse_integer_constants,
    parse_name_file,
    parse_name_tables,
)


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def require(condition: bool, message: str) -> None:
    if not condition:
        raise RuntimeError(message)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cache", type=Path, default=REPO.parent / "cache" / "files")
    parser.add_argument(
        "--report", type=Path,
        default=MODULE / "build" / "actor-spawn-runtime-validation.json",
    )
    args = parser.parse_args()
    cache = args.cache.resolve()
    pydata = cache / "data" / "pydata"
    paths = {
        "templates": pydata / "character_templates_pyarray.bin",
        "template_names": pydata / "character_templates_pyarraynames.bin",
        "property_names": pydata / "character_properties_pyarraynames.bin",
        "ai_constants": pydata / "ai_pycst.bin",
    }
    for path in paths.values():
        if not path.is_file():
            raise SystemExit(f"missing cache input: {path}")
    raw = {key: path.read_bytes() for key, path in paths.items()}

    property_name_tables = parse_name_tables(raw["property_names"])
    require(len(property_name_tables) == 3,
            "character_properties name file no longer contains its three source tables")
    property_names = property_name_tables[0]
    templates = parse_character_templates(
        raw["templates"], raw["template_names"], raw["property_names"]
    )
    constants = parse_integer_constants(raw["ai_constants"])
    template = next(
        (item for item in templates if item.name == "InfectedVillage_CommonType1"),
        None,
    )
    require(template is not None, "InfectedVillage_CommonType1 not present in cache")
    assert template is not None
    require(len(template.property_ids) > 1, "expected authored property alternatives")
    resolved = [
        choose_template_property(template, i, property_names)
        for i in range(len(template.property_ids))
    ]
    require(
        all(row["status"] == "explicit_alternative" for row in resolved),
        "template property alternative did not resolve against CharacterTable names",
    )
    require(
        all(row["random_choice_made"] is False for row in resolved),
        "reader must preserve alternatives without inventing RNG",
    )
    require(constants.get("AIStates", {}).get("Limbus") == 0,
            "AIStates.Limbus source constant changed")
    require(constants.get("AIStates", {}).get("Spawn") == 1,
            "AIStates.Spawn source constant changed")

    actor = "_prim_tmp_infected05"
    command = {"name": "SpawnCharacter", "payload": [actor]}
    accepted = apply_spawn_character_request(
        command, [actor], constants["AIStates"], constants["AIStates"]["Limbus"],
        registered_state_ids=(constants["AIStates"]["Limbus"], constants["AIStates"]["Spawn"]),
    )
    require(accepted["status"] == "direct_state_change", "Spawn request was not applied")
    require(accepted["from_state_id"] == 0 and accepted["to_state_id"] == 1,
            "Limbus -> Spawn transition IDs changed")
    require(accepted["actor_created"] is False, "script boundary created an actor")
    require(accepted["animation_requested"] is False,
            "script boundary must not invent an animation request")
    require(accepted["state_changed"] is True, "expected state change from Limbus")
    registration_unverified = apply_spawn_character_request(
        command, [actor], constants["AIStates"], constants["AIStates"]["Limbus"]
    )
    require(registration_unverified["status"] == "state_registration_unverified" and
            registration_unverified["state_changed"] is None and
            registration_unverified["requested_state_id"] == constants["AIStates"]["Spawn"],
            "unverified native state registration must remain a request")
    unregistered = apply_spawn_character_request(
        command, [actor], constants["AIStates"], constants["AIStates"]["Limbus"],
        registered_state_ids=(constants["AIStates"]["Limbus"],),
    )
    require(unregistered["status"] == "spawn_state_not_registered" and
            unregistered["to_state_id"] == unregistered["from_state_id"],
            "unregistered Spawn state changed state")

    missing = apply_spawn_character_request(
        {"name": "SpawnCharacter", "payload": ["_prim_tmp_infected17"]},
        [actor], constants["AIStates"], constants["AIStates"]["Limbus"],
    )
    require(missing["status"] == "actor_lookup_miss_or_ambiguous",
            "unresolved static actor was not rejected")
    require(missing["to_state_id"] == missing["from_state_id"],
            "lookup miss changed state")
    malformed = apply_spawn_character_request(
        {"name": "SpawnCharacter", "payload": []},
        [actor], constants["AIStates"], constants["AIStates"]["Limbus"],
    )
    require(malformed["status"] == "malformed_command", "malformed command was accepted")
    repeated = apply_spawn_character_request(
        command, [actor], constants["AIStates"], constants["AIStates"]["Spawn"],
        registered_state_ids=(constants["AIStates"]["Spawn"],),
    )
    require(repeated["status"] == "direct_state_change" and
            repeated["state_changed"] is False and repeated["to_state_id"] == 1,
            "repeated request should remain at Spawn without a second state change")
    ambiguous = apply_spawn_character_request(
        command, [actor, actor], constants["AIStates"], constants["AIStates"]["Limbus"]
    )
    require(ambiguous["status"] == "actor_lookup_miss_or_ambiguous",
            "ambiguous exact-name lookup was accepted")

    malformed_checks = 0
    for broken in (raw["templates"][:-1], raw["templates"] + b"\0"):
        try:
            parse_character_templates(broken, raw["template_names"], raw["property_names"])
        except DecodeError:
            malformed_checks += 1
    require(malformed_checks == 2, "malformed template input was not rejected")
    try:
        parse_name_file(raw["template_names"] + b"\0")
    except DecodeError:
        malformed_checks += 1
    require(malformed_checks == 3, "malformed sidecar input was not rejected")

    # Pin the native evidence that gives this port its deliberately narrow
    # semantics. Decompiled addresses are +0x10000 from ELF addresses.
    native = REPO / "recovered" / "native" / "decompiled" / "libDungeonHunter2.so"
    script_src = (native / "functions-006.pseudo.c").read_text(encoding="utf-8")
    chars_src = (native / "functions-003.pseudo.c").read_text(encoding="utf-8")
    state_src = (native / "functions-003.pseudo.c").read_text(encoding="utf-8")
    template_reader_src = (native / "functions-008.pseudo.c").read_text(encoding="utf-8")
    assembly = REPO / "recovered" / "native" / "assembly" / "libDungeonHunter2.so"
    character_asm = (assembly / "Character-1405a63e8a78-001.asm").read_text(
        encoding="utf-8"
    )
    state_machine_asm = (
        assembly / "CharStateMachine-9e67f9b0cab6-001.asm"
    ).read_text(encoding="utf-8")
    require("SM_SetSpawnStateEbb(iVar1 + 0x4fc,0,0)" in script_src,
            "SpawnCharacter native call evidence changed")
    require("_ZN16CharStateMachine9_SetStateEiiPv(param_1,1,0xffffffff)" in state_src,
            "false Spawn-state setter path no longer requests state 1")
    require("_ZNK16CharStateMachine9_HasStateEi(param_1,param_2)" in state_src and
            "if (iVar1 == 0)" in state_src,
            "native state registration gate evidence changed")
    require("FUNCTION 0x003a9340" in character_asm and
            "bl #0x3c7318" in character_asm and
            "cmp r6, #0x14" in character_asm,
            "Character constructor no longer registers state IDs 0..19 per instance")
    require("FUNCTION 0x003c7318" in state_machine_asm and
            "cmp r3, #0x14" in state_machine_asm,
            "per-machine RegisterState factory lookup evidence changed")
    require("random >= 0 && random < (int)charTemplate.CharInfoSize" in chars_src,
            "native template selection bound evidence changed")
    require("*(short *)(*(int *)(iVar4 + 8) + iVar5 * 8 + 4)" in chars_src,
            "native CharInfoName-to-property-ID read changed")
    require("_ZN6Arrays18Charater_Templates4readEP11IStreamBase" in template_reader_src and
            "iVar4 * 0xc + _ZN6Arrays18Charater_Templates7membersE" in chars_src,
            "native template reader/record stride evidence changed")

    report = {
        "complete_game": False,
        "scope": "Cache Charater_Templates parsing, explicit character-property alternative resolution, and the observed SpawnCharacter-to-Spawn state path only.",
        "template_count": len(templates),
        "character_property_name_count": len(property_names),
        "character_property_name_table_count": len(property_name_tables),
        "template": {
            "name": template.name,
            "property_ids": list(template.property_ids),
            "property_names": [row["property_name"] for row in resolved],
            "selection_performed": False,
        },
        "ai_states": {"Limbus": constants["AIStates"]["Limbus"],
                      "Spawn": constants["AIStates"]["Spawn"]},
        "native_state_registration": {
            "character_constructor_elf": "0x3a9340",
            "register_state_elf": "0x3c7318",
            "per_character_state_ids": list(range(20)),
            "state_implementations_shared_singletons": True,
        },
        "spawn_request": accepted,
        "negative_cases": {
            "missing_static_actor": missing["status"],
            "malformed_command": malformed["status"],
            "state_registration_unverified": registration_unverified["status"],
            "state_not_registered": unregistered["status"],
            "duplicate_actor_name": ambiguous["status"],
            "repeat_request_state_change": repeated["state_changed"],
        },
        "checks": {
            "template_alternatives_all_resolve": True,
            "no_random_choice_invented": True,
            "direct_Limbus_to_Spawn": True,
            "native_state_registration_gate_modelled": True,
            "actor_not_created": True,
            "malformed_inputs_rejected": malformed_checks == 3,
            "native_source_evidence_pinned": True,
        },
        "input_files": {
            key: {"bytes": len(data), "sha256": sha256(data)}
            for key, data in raw.items()
        },
        "caveats": [
            "The native random generator is not ported. The selected property alternative is only resolved when the caller supplies an index.",
            "Native state focus/blur callbacks, event delivery, CSSpawn animation lookup, fade-in, timers, AI decisions, and rendering are not run.",
            "The constructor-backed state map and direct state-setter path are modeled, but native focus/blur callbacks, CSSpawn animation playback, timers, AI decisions, and rendering are not executed.",
            "This reports the observed direct state-setter effect, not a complete actor creation or game loop.",
        ],
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    print(f"wrote {args.report}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
