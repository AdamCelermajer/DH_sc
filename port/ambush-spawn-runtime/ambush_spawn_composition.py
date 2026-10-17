"""Join cache-backed Ambush projection to the bounded Spawn-state model."""
from __future__ import annotations

from typing import Any, Mapping, Sequence

from actor_spawn_runtime import apply_spawn_character_request, choose_template_property


EXPECTED_REQUESTS = (
    ("_prim_tmp_infected05", "requested", "character_spawn_state_requested", 6),
    ("_prim_tmp_infected07", "requested", "character_spawn_state_requested", 8),
    ("_prim_tmp_infected17", "lookup_miss", "object_lookup_miss", None),
    ("_prim_tmp_infected06", "requested", "character_spawn_state_requested", 7),
    ("_prim_tmp_infected16", "requested", "character_spawn_state_requested", 19),
)
EXPECTED_TEMPLATE = "InfectedVillage_CommonType1"
EXPECTED_MODULE = "_module_infectedvillage_01_001"


class CompositionError(ValueError):
    """The connected source projection does not match the bounded contract."""


def _require(condition: bool, message: str) -> None:
    if not condition:
        raise CompositionError(message)


def compose_projection(
    projection: Mapping[str, Any],
    templates: Sequence[Any],
    property_names: Sequence[str],
    ai_states: Mapping[str, int],
    registered_state_ids: Sequence[int] | None = None,
) -> dict[str, Any]:
    """Apply actor-spawn-runtime to actual trigger/registry projection records.

    The source script/registry owns request order and loaded Character names.
    This layer validates each present source template, resolves every authored
    property alternative without choosing RNG, then maps each script request
    through the existing SpawnCharacter state-boundary function.
    """
    _require(projection.get("activation_status") == "activated",
             "source Ambush did not activate")
    _require(projection.get("source_actor_count") == 26 and
             projection.get("runtime_object_count") == 26,
             "source registry/runtime object counts changed")
    _require(projection.get("actors_created") == 0 and
             projection.get("render_objects_created") == 0,
             "this projection must not claim actor or render allocation")
    loaded_names = projection.get("loaded_character_names")
    requests = projection.get("requests")
    _require(projection.get("request_count") == len(EXPECTED_REQUESTS),
             "source registry request count changed")
    _require(isinstance(loaded_names, list) and len(loaded_names) == 26 and
             all(isinstance(name, str) for name in loaded_names),
             "expected the exact 26 imported Character names")
    _require(len(set(loaded_names)) == len(loaded_names),
             "source registry must not contain ambiguous Character names")
    _require(isinstance(requests, list) and len(requests) == len(EXPECTED_REQUESTS),
             "expected the five ordered Ambush SpawnCharacter requests")

    matching_templates = [item for item in templates if item.name == EXPECTED_TEMPLATE]
    template = matching_templates[0] if len(matching_templates) == 1 else None
    _require(template is not None and len(template.property_ids) > 0,
             "source template or its property alternatives are missing")
    resolved_alternatives = [
        choose_template_property(template, index, property_names)
        for index in range(len(template.property_ids))
    ]
    _require(all(row.get("status") == "explicit_alternative" and
                 row.get("random_choice_made") is False
                 for row in resolved_alternatives),
             "one or more cached template alternatives failed to resolve")
    _require(ai_states.get("Limbus") == 0 and ai_states.get("Spawn") == 1,
             "cached AIStates Limbus/Spawn source IDs changed")
    if registered_state_ids is not None:
        _require(ai_states["Limbus"] in registered_state_ids and
                 ai_states["Spawn"] in registered_state_ids,
                 "per-Character state map lacks the source Limbus or Spawn state")

    results: list[dict[str, Any]] = []
    for index, (request, expected) in enumerate(zip(requests, EXPECTED_REQUESTS)):
        name, expected_registry_result, expected_event, expected_source_record = expected
        _require(request.get("name") == name,
                 f"request {index} name/order differs from the decoded Ambush")
        _require(request.get("registry_result") == expected_registry_result and
                 request.get("event_type") == expected_event and
                 request.get("event_command_id") == 30,
                 f"request {name} does not match its source SpawnCharacter event")
        actor = request.get("source_actor")
        if expected_source_record is None:
            _require(actor is None and name not in loaded_names,
                     "unresolved infected17 acquired a source Character")
        else:
            _require(name in loaded_names and isinstance(actor, dict),
                     f"source Character row is missing for {name}")
            _require(actor.get("module") == EXPECTED_MODULE and
                     actor.get("source_record") == expected_source_record and
                     actor.get("template") == EXPECTED_TEMPLATE and
                     actor.get("template_data_class") == "Charater_Templates" and
                     actor.get("editor_template_name") == "MonsterCommonType1" and
                     actor.get("ai_state") == "Limbus" and
                     actor.get("auto_spawn") == 0 and
                     actor.get("lifecycle") == "spawn_requested",
                     f"source-backed Character fields differ for {name}")

        command = {"name": "SpawnCharacter", "payload": [name]}
        initial_state = ai_states["Limbus"]
        spawn = apply_spawn_character_request(
            command, loaded_names, ai_states, initial_state,
            registered_state_ids=registered_state_ids,
        )
        if expected_source_record is None:
            _require(spawn.get("status") == "actor_lookup_miss_or_ambiguous" and
                     spawn.get("state_changed") is False and
                     spawn.get("to_state_id") == initial_state,
                     "missing infected17 must remain a no-transition lookup miss")
        else:
            expected_status = (
                "direct_state_change" if registered_state_ids is not None
                else "state_registration_unverified"
            )
            expected_state_changed = (
                initial_state != ai_states["Spawn"]
                if registered_state_ids is not None else None
            )
            _require(spawn.get("status") == expected_status and
                     spawn.get("requested_state_id") == ai_states["Spawn"] and
                     spawn.get("state_changed") == expected_state_changed and
                     spawn.get("actor_created") is False and
                     spawn.get("animation_requested") is False,
                     f"Spawn state result differs from native registration for {name}")
        results.append({
            "ambush_order": index + 1,
            "actor_name": name,
            "template": actor.get("template") if actor else None,
            "template_alternative_count": len(template.property_ids) if actor else 0,
            "template_alternatives_resolved": len(resolved_alternatives) if actor else 0,
            "template_choice_performed": False,
            "script_event_type": expected_event,
            "registry_result": expected_registry_result,
            "spawn_runtime": spawn,
        })

    _require(projection.get("registry_lookup_misses") == 1 and
             projection.get("last_lookup_miss") == "_prim_tmp_infected17" and
             projection.get("missing_name_still_unregistered") is True,
             "actor registry no longer retains the unresolved infected17 miss")
    return {
        "complete_game": False,
        "scope": (
            "Cache-backed Infected Village Ambush trigger and actor registry "
            "composed with cached Character template decoding and the "
            "SpawnCharacter-to-Spawn request boundary."
        ),
        "source_actor_count": projection["source_actor_count"],
        "ordered_requests": results,
        "ai_states": {
            "Limbus": ai_states["Limbus"],
            "Spawn": ai_states["Spawn"],
            "registered_per_character": list(registered_state_ids)
            if registered_state_ids is not None else None,
        },
        "template": {
            "name": template.name,
            "property_ids": list(template.property_ids),
            "property_names": [row["property_name"] for row in resolved_alternatives],
            "alternative_choice_performed": False,
            "random_choice_made": False,
        },
        "unresolved_lookup": {
            "name": "_prim_tmp_infected17",
            "registry_result": "lookup_miss",
            "spawn_runtime_status": results[2]["spawn_runtime"]["status"],
            "still_registered": False,
            "state_changed": False,
        },
        "checks": {
            "five_script_requests_preserve_order": True,
            "four_source_character_spawn_state_requests": sum(
                item["registry_result"] == "requested" for item in results
            ) == 4,
            "all_four_source_templates_match": all(
                item["template"] == EXPECTED_TEMPLATE
                for item in results if item["registry_result"] == "requested"
            ),
            "native_spawn_state_registration_verified": (
                registered_state_ids is not None and
                all(item["spawn_runtime"]["status"] == "direct_state_change"
                    for item in results if item["registry_result"] == "requested")
            ),
            "missing_infected17_remains_unresolved": True,
            "actor_or_render_allocation_claimed": False,
        },
        "caveats": [
            "The trigger contact bounds and activation are an approximate host proxy; native Zone::IsInside and final bounds behavior remain unresolved.",
            "Charater_Templates alternatives are decoded and all resolve, but the native random choice is not reproduced and no variant is selected here.",
            "The constructor-backed state map is modeled, but native state callbacks, transition events, CSSpawn animation playback, timers, AI decisions, and rendering are not executed.",
            "No Character object is created, AI or animation is run, and no render object is allocated.",
        ],
    }
