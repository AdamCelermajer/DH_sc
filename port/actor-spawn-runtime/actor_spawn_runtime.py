"""Small, fail-closed projection of cached character templates and Spawn state.

This does not simulate AI, timers, animation, rendering, or random selection.
Template alternatives are decoded exactly and callers must provide an explicit
choice when they need one. SpawnCharacter changes state only for an already
loaded actor with the exact requested name.
"""
from __future__ import annotations

from dataclasses import dataclass
import struct
from typing import Any, Mapping, Sequence


MAX_INPUT_BYTES = 16 * 1024 * 1024
MAX_RECORDS = 4096
MAX_NAME_BYTES = 255


class DecodeError(ValueError):
    """Input is malformed or outside this bounded reader's supported format."""


@dataclass(frozen=True)
class CharacterTemplate:
    name: str
    property_ids: tuple[int, ...]


def _u32(data: bytes, offset: int) -> tuple[int, int]:
    if offset < 0 or len(data) - offset < 4:
        raise DecodeError("truncated 32-bit word")
    return struct.unpack_from("<I", data, offset)[0], offset + 4


def _parse_name_table(data: bytes, offset: int) -> tuple[tuple[str, ...], int]:
    """Decode one count + length-prefixed UTF-8 name table."""
    count, offset = _u32(data, offset)
    if count > MAX_RECORDS or count > (len(data) - offset) // 4:
        raise DecodeError("invalid names count")
    result: list[str] = []
    for _ in range(count):
        size, offset = _u32(data, offset)
        if size > MAX_NAME_BYTES or size > len(data) - offset:
            raise DecodeError("invalid name length")
        value = data[offset:offset + size]
        offset += size
        if b"\0" in value:
            raise DecodeError("embedded NUL in name")
        try:
            result.append(value.decode("utf-8", errors="strict"))
        except UnicodeDecodeError as exc:
            raise DecodeError("invalid UTF-8 name") from exc
    return tuple(result), offset


def parse_name_file(data: bytes) -> tuple[str, ...]:
    """Decode a one-table name sidecar."""
    if not isinstance(data, bytes) or len(data) > MAX_INPUT_BYTES:
        raise DecodeError("invalid or oversized names file")
    result, offset = _parse_name_table(data, 0)
    if offset != len(data):
        raise DecodeError("trailing names bytes")
    return result


def parse_name_tables(data: bytes) -> tuple[tuple[str, ...], ...]:
    """Decode concatenated tables in files shared by several Arrays readers."""
    if not isinstance(data, bytes) or not data or len(data) > MAX_INPUT_BYTES:
        raise DecodeError("invalid or oversized names file")
    tables: list[tuple[str, ...]] = []
    offset = 0
    while offset < len(data):
        table, offset = _parse_name_table(data, offset)
        tables.append(table)
    return tuple(tables)


def parse_character_templates(
    array_data: bytes,
    names_data: bytes,
    property_names_data: bytes,
) -> tuple[CharacterTemplate, ...]:
    """Decode the cache's Charater_Templates array and check property IDs.

    The original reader serializes one uint count per template followed by
    that many signed 32-bit CharInfoName values. Native Character code uses a
    selected CharInfoName value as a CharacterTable ID. This function keeps
    alternatives intact and validates that link against the table name count.
    """
    if not isinstance(array_data, bytes) or len(array_data) > MAX_INPUT_BYTES:
        raise DecodeError("invalid or oversized template array")
    names = parse_name_file(names_data)
    property_tables = parse_name_tables(property_names_data)
    if not property_tables:
        raise DecodeError("empty CharacterTable name file")
    # This shared file starts with the original CharacterTable name table;
    # later concatenated tables are StatAutoAssignSchemeTable and StatListTable.
    properties = property_tables[0]
    if not properties:
        raise DecodeError("empty CharacterTable name file")
    count, offset = _u32(array_data, 0)
    if count != len(names):
        raise DecodeError("template array/name count mismatch")
    if count > MAX_RECORDS:
        raise DecodeError("too many templates")
    templates: list[CharacterTemplate] = []
    for name in names:
        alternatives, offset = _u32(array_data, offset)
        if alternatives > MAX_RECORDS or alternatives > (len(array_data) - offset) // 4:
            raise DecodeError(f"invalid CharInfo count for {name}")
        values: list[int] = []
        for _ in range(alternatives):
            raw, offset = _u32(array_data, offset)
            value = struct.unpack("<i", struct.pack("<I", raw))[0]
            # SafeGetCharPropsId stores this value in a signed short and uses
            # it as a CharacterTable index, so reject values not representable
            # as an actual in-range source table member.
            if value < 0 or value >= len(properties) or value > 0x7FFF:
                raise DecodeError(f"CharInfo ID {value} outside CharacterTable")
            values.append(value)
        templates.append(CharacterTemplate(name, tuple(values)))
    if offset != len(array_data):
        raise DecodeError("trailing template array bytes")
    return tuple(templates)


def parse_integer_constants(data: bytes) -> dict[str, dict[str, int]]:
    """Decode the game's nested length-prefixed integer-constant file."""
    if not isinstance(data, bytes) or len(data) > MAX_INPUT_BYTES:
        raise DecodeError("invalid or oversized constants file")
    groups, offset = _u32(data, 0)
    if groups > MAX_RECORDS:
        raise DecodeError("too many constant groups")
    result: dict[str, dict[str, int]] = {}

    def text_at(at: int) -> tuple[str, int]:
        size, at = _u32(data, at)
        if size > MAX_NAME_BYTES or size > len(data) - at:
            raise DecodeError("invalid constant name length")
        raw = data[at:at + size]
        if b"\0" in raw:
            raise DecodeError("embedded NUL in constant name")
        try:
            value = raw.decode("utf-8", errors="strict")
        except UnicodeDecodeError as exc:
            raise DecodeError("invalid UTF-8 constant name") from exc
        return value, at + size

    for _ in range(groups):
        group, offset = text_at(offset)
        entries, offset = _u32(data, offset)
        if entries > MAX_RECORDS or entries > (len(data) - offset) // 8:
            raise DecodeError(f"invalid constant count for {group}")
        target = result.setdefault(group, {})
        for _ in range(entries):
            key, offset = text_at(offset)
            raw, offset = _u32(data, offset)
            target[key] = struct.unpack("<i", struct.pack("<I", raw))[0]
    if offset != len(data):
        raise DecodeError("trailing constants bytes")
    return result


def choose_template_property(
    template: CharacterTemplate,
    alternative_index: int,
    property_names: Sequence[str],
) -> dict[str, Any]:
    """Resolve a caller-selected alternative without inventing RNG behavior."""
    if type(alternative_index) is not int or not 0 <= alternative_index < len(template.property_ids):
        return {"status": "alternative_out_of_range", "selected": None}
    property_id = template.property_ids[alternative_index]
    if not 0 <= property_id < len(property_names):
        return {"status": "property_id_unresolved", "selected": None}
    return {
        "status": "explicit_alternative",
        "alternative_index": alternative_index,
        "property_id": property_id,
        "property_name": property_names[property_id],
        "random_choice_made": False,
    }


def apply_spawn_character_request(
    command: Mapping[str, Any],
    loaded_character_names: Sequence[str],
    state_ids: Mapping[str, int],
    current_state_id: int,
    registered_state_ids: Sequence[int] | None = None,
) -> dict[str, Any]:
    """Model only the observed SpawnCharacter -> direct Spawn state change."""
    unchanged = {
        "state_changed": False,
        "from_state_id": current_state_id,
        "to_state_id": current_state_id,
        "actor_created": False,
        "animation_requested": False,
    }
    if command.get("name") != "SpawnCharacter":
        return {"status": "unsupported_command", **unchanged}
    payload = command.get("payload")
    if not isinstance(payload, (tuple, list)) or len(payload) != 1 or not isinstance(payload[0], str):
        return {"status": "malformed_command", **unchanged}
    actor_name = payload[0]
    if loaded_character_names.count(actor_name) != 1:
        return {"status": "actor_lookup_miss_or_ambiguous", "actor_name": actor_name, **unchanged}
    if type(state_ids.get("Spawn")) is not int:
        return {"status": "spawn_state_unresolved", "actor_name": actor_name, **unchanged}
    spawn_id = state_ids["Spawn"]
    if registered_state_ids is None:
        return {
            "status": "state_registration_unverified",
            "actor_name": actor_name,
            "requested_state_id": spawn_id,
            "state_changed": None,
            "from_state_id": current_state_id,
            "to_state_id": current_state_id,
            "actor_created": False,
            "animation_requested": False,
            "source_call": "SM_SetSpawnState(false, false)",
        }
    if spawn_id not in registered_state_ids:
        return {
            "status": "spawn_state_not_registered",
            "actor_name": actor_name,
            "requested_state_id": spawn_id,
            **unchanged,
        }
    return {
        "status": "direct_state_change",
        "actor_name": actor_name,
        "requested_state_id": spawn_id,
        "state_changed": current_state_id != spawn_id,
        "from_state_id": current_state_id,
        "to_state_id": spawn_id,
        "actor_created": False,
        "animation_requested": False,
        "source_call": "SM_SetSpawnState(false, false)",
    }
