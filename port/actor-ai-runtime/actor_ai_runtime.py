"""Fail-closed translator for explicit Infected Village spawn script requests.

This is a host-side audit/translation boundary, not an AI simulation. It maps
only a decoded SpawnCharacter command for one uniquely matched, cache-authored
Infected Village Character to the native Spawn state request. It never chooses
a model variant, invents a trigger condition, or requests an animation.
"""
from __future__ import annotations

from dataclasses import dataclass
from typing import Any, Mapping, Sequence


@dataclass(frozen=True)
class StateIds:
    limbus: int
    spawn: int


def translate_spawn_command(
    command: Mapping[str, Any],
    static_character_matches: Sequence[Mapping[str, Any]],
    model_choices: Sequence[Mapping[str, Any]],
    state_ids: StateIds,
) -> dict[str, Any]:
    """Translate only the native state request carried by SpawnCharacter.

    A command is considered eligible only when cache decoding found exactly
    one matching static Character record. The caller supplies model choices as
    alternatives; this function preserves all of them and never samples one.
    """
    if command.get("name") != "SpawnCharacter":
        return {"status": "unsupported_command", "state_request": None,
                "animation_request": None}

    payload = command.get("payload")
    if not isinstance(payload, (tuple, list)) or len(payload) != 1 or not isinstance(payload[0], str):
        return {"status": "malformed_command", "state_request": None,
                "animation_request": None}

    actor_name = payload[0]
    if len(static_character_matches) != 1:
        return {"status": "unresolved_static_character", "actor_name": actor_name,
                "match_count": len(static_character_matches), "state_request": None,
                "animation_request": None}

    source = static_character_matches[0]
    if not isinstance(source, Mapping):
        return {"status": "source_record_not_supported", "actor_name": actor_name,
                "state_request": None, "animation_request": None}
    if (source.get("name") != actor_name
            or source.get("ai_state") != "Limbus"
            or source.get("auto_spawn") != "0"
            or source.get("template_name") != "InfectedVillage_CommonType1"):
        return {"status": "source_record_not_supported", "actor_name": actor_name,
                "state_request": None, "animation_request": None}

    if not model_choices or any(
            not isinstance(model, Mapping)
            or type(model.get("character_table_id")) is not int
            or model["character_table_id"] < 0
            for model in model_choices):
        return {"status": "model_choices_unresolved", "actor_name": actor_name,
                "state_request": None, "animation_request": None}

    return {
        "status": "translated_explicit_request",
        "actor_name": actor_name,
        "source_record": source.get("source_record"),
        "authored_initial_state": {"name": "Limbus", "id": state_ids.limbus},
        "state_request": {
            "name": "Spawn",
            "id": state_ids.spawn,
            "origin": "SpawnCharacter -> SM_SetSpawnState(false, false)",
        },
        "animation_request": None,
        "animation_status": "withheld_unresolved",
        "animation_reason": (
            "No checked-in typed CharTemplate/CharAnimTable record reader proves the "
            "native CSSpawn animation slot for these data. The cache's named Spawn "
            "mapping is not emitted as a runtime animation request."
        ),
        "model_choices": [
            {"character_table_id": model["character_table_id"],
             "character_name": model["character_name"],
             "model_key": model["model_key"]}
            for model in model_choices
        ],
        "model_choice_selected": False,
        "script_or_trigger_executed": False,
    }
