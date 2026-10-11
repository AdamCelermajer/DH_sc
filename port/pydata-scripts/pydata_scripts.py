"""Bounded, data-only reader for the recovered PyData script tables.

This module decodes command records; it never dispatches or executes them.
The command layouts are limited to types observed in common scripts and the
SWAMP level corpus. Unknown commands fail closed instead of guessing a size.
"""
from dataclasses import dataclass
import struct
from typing import Any, Sequence


MAX_SCRIPTS = 4096
MAX_COMMANDS_PER_SCRIPT = 4096
MAX_TOTAL_COMMANDS = 100_000
MAX_STRING_BYTES = 1 << 20
MAX_TOTAL_STRING_BYTES = 16 << 20
MAX_ARRAY_ITEMS = 1 << 16
MAX_TABLE_BYTES = 64 << 20


class DecodeError(ValueError):
    """Malformed or unsupported table data, with the failing byte offset."""

    def __init__(self, message: str, offset: int):
        super().__init__(f"{message} at byte {offset}")
        self.message = message
        self.offset = offset


@dataclass(frozen=True)
class Command:
    command_id: int
    name: str
    # Fields preserve reader order. i32/u32 are Python ints, bools are bool,
    # strings are decoded UTF-8, and the ExecScript vector is a tuple of i32.
    payload: tuple[Any, ...]
    byte_offset: int
    byte_size: int


@dataclass(frozen=True)
class Script:
    name: str
    commands: tuple[Command, ...]
    table_index: int
    byte_offset: int
    byte_end: int


@dataclass(frozen=True)
class ScriptTable:
    names: tuple[str, ...]
    scripts: tuple[Script, ...]


# Field codes are taken from Structs::<command>::read in the recovered native
# decompilation. S means u32 byte length followed by that many UTF-8 bytes;
# A means u32 element count followed by that many i32 values.
_COMMAND_SCHEMAS: dict[int, tuple[str, tuple[str, ...]]] = {
    0: ("ExecScript", ("B", "I", "A", "B")),
    1: ("EnterCutSceneMode", ()),
    2: ("ExitCutSceneMode", ()),
    3: ("CONSOLE", ("S",)),
    5: ("PlayCamera", ("I", "B")),
    6: ("SetCameraClip", ("I", "I")),
    8: ("SetCameraTarget", ("I", "S", "B")),
    10: ("StartDialog", ("I", "I", "I")),
    11: ("StartDialogID", ("I",)),
    12: ("WaitDialog", ()),
    13: ("PlaySound", ("I", "B", "B", "I")),
    14: ("StopSound", ("I", "B", "I")),
    15: ("PlayLevelMusic", ("I",)),
    16: ("EnterSafeZone", ()),
    17: ("LeaveSafeZone", ()),
    19: ("PlayAnimByName", ("S", "B", "S", "B")),
    # PlayEffect::read contains one i32 then Pos3D::read (three i32s),
    # then a length-prefixed string and one byte bool.
    20: ("PlayEffect", ("I", "I", "I", "I", "S", "B")),
    21: ("StopEffect", ("I",)),
    22: ("ShowFlash", ("I", "S", "B")),
    23: ("HideFlash", ("I", "S", "B")),
    24: ("LockCharacter", ("S",)),
    25: ("UnlockCharacter", ("S",)),
    26: ("Wait", ("I",)),
    27: ("SetFaeryState", ("I", "I")),
    29: ("PutCharacterInLimbus", ("B", "S")),
    30: ("SpawnCharacter", ("S",)),
    31: ("PutCharacterInIdle", ("S",)),
    32: ("MarkCharacterAsScripted", ("B", "S")),
    39: ("StopActor", ("S",)),
    40: ("MoveActor", ("S", "B", "S", "B")),
    41: ("LookActor", ("S", "S")),
    42: ("ShowActor", ("S",)),
    43: ("HideActor", ("S",)),
    44: ("KillActor", ("S",)),
    45: ("PlayActorAnim", ("I", "I", "I", "S", "B")),
    46: ("SetActorPosition", ("S", "S")),
    51: ("UnEquipHands", ("S",)),
    52: ("ReEquipHands", ("S",)),
    54: ("OpenDoor", ("S", "B")),
    55: ("CloseDoor", ("S", "B")),
    63: ("RestartLevel", ()),
    68: ("ShowTrophies", ()),
    69: ("SaveGame", ()),
    70: ("BlockSaveGame", ()),
    74: ("EnqueueCharMenuTutorialMessage", ("S", "S", "I")),
    75: ("SkipCharMenuTutorialMessage", ()),
    76: ("SkipAllCharMenuTutorialMessages", ()),
    77: ("LockTutorial", ("I",)),
    78: ("DoTutorial", ("S", "I")),
    79: ("FlushMessages", ()),
}


class _Reader:
    def __init__(self, data: bytes | bytearray | memoryview):
        try:
            self.data = memoryview(data).cast("B")
        except (TypeError, ValueError) as exc:
            raise TypeError("input must be a contiguous bytes-like object") from exc
        if len(self.data) > MAX_TABLE_BYTES:
            raise DecodeError(f"table exceeds {MAX_TABLE_BYTES}-byte limit", 0)
        self.offset = 0
        self.string_bytes = 0

    def take(self, size: int, label: str) -> memoryview:
        start = self.offset
        if size < 0 or size > len(self.data) - start:
            raise DecodeError(f"truncated {label}", start)
        self.offset += size
        return self.data[start:self.offset]

    def u32(self, label: str) -> int:
        start = self.offset
        raw = self.take(4, label)
        return struct.unpack_from("<I", raw)[0]

    def i32(self, label: str) -> int:
        raw = self.take(4, label)
        return struct.unpack_from("<i", raw)[0]

    def boolean(self, label: str) -> bool:
        start = self.offset
        value = self.take(1, label)[0]
        if value not in (0, 1):
            raise DecodeError(f"invalid bool byte {value}", start)
        return bool(value)

    def string(self, label: str = "string") -> str:
        length_offset = self.offset
        length = self.u32(f"{label} length")
        if length > MAX_STRING_BYTES:
            raise DecodeError(f"{label} exceeds {MAX_STRING_BYTES}-byte limit", length_offset)
        if length > MAX_TOTAL_STRING_BYTES - self.string_bytes:
            raise DecodeError(f"table strings exceed {MAX_TOTAL_STRING_BYTES}-byte limit",
                              length_offset)
        self.string_bytes += length
        string_offset = self.offset
        raw = self.take(length, label)
        try:
            return bytes(raw).decode("utf-8", errors="strict")
        except UnicodeDecodeError as exc:
            raise DecodeError(f"invalid UTF-8 in {label}", string_offset + exc.start) from exc

    def finish(self, label: str) -> None:
        if self.offset != len(self.data):
            raise DecodeError(f"trailing bytes in {label}", self.offset)


def parse_script_names(data: bytes | bytearray | memoryview) -> tuple[str, ...]:
    """Decode u32 count + repeated u32 byte length + UTF-8 bytes."""
    reader = _Reader(data)
    count_offset = reader.offset
    count = reader.u32("script-name count")
    if count > MAX_SCRIPTS:
        raise DecodeError(f"script-name count exceeds {MAX_SCRIPTS}", count_offset)
    names = tuple(reader.string(f"script name {index}") for index in range(count))
    reader.finish("script-name table")
    return names


def parse_programs(data: bytes | bytearray | memoryview,
                   expected_script_count: int | None = None) -> tuple[tuple[Script, ...], int]:
    """Decode sequential command streams; return scripts and consumed bytes."""
    reader = _Reader(data)
    count_offset = reader.offset
    script_count = reader.u32("script count")
    if script_count > MAX_SCRIPTS:
        raise DecodeError(f"script count exceeds {MAX_SCRIPTS}", count_offset)
    if expected_script_count is not None and script_count != expected_script_count:
        raise DecodeError(
            f"script count {script_count} does not match {expected_script_count} names",
            count_offset,
        )

    scripts: list[Script] = []
    total_commands = 0
    for script_index in range(script_count):
        script_offset = reader.offset
        command_count_offset = reader.offset
        command_count = reader.u32(f"script {script_index} command count")
        if command_count > MAX_COMMANDS_PER_SCRIPT:
            raise DecodeError(
                f"script {script_index} command count exceeds {MAX_COMMANDS_PER_SCRIPT}",
                command_count_offset,
            )
        total_commands += command_count
        if total_commands > MAX_TOTAL_COMMANDS:
            raise DecodeError(f"total commands exceed {MAX_TOTAL_COMMANDS}", command_count_offset)

        commands: list[Command] = []
        for command_index in range(command_count):
            command_offset = reader.offset
            command_id = reader.u32(f"script {script_index} command {command_index} ID")
            spec = _COMMAND_SCHEMAS.get(command_id)
            if spec is None:
                raise DecodeError(f"unsupported command ID {command_id}", command_offset)
            command_name, fields = spec
            payload: list[Any] = []
            for field in fields:
                if field == "I":
                    payload.append(reader.i32(f"{command_name} i32"))
                elif field == "B":
                    payload.append(reader.boolean(f"{command_name} bool"))
                elif field == "S":
                    payload.append(reader.string(f"{command_name} string"))
                elif field == "A":
                    array_offset = reader.offset
                    length = reader.u32(f"{command_name} array count")
                    if length > MAX_ARRAY_ITEMS:
                        raise DecodeError(
                            f"{command_name} array count exceeds {MAX_ARRAY_ITEMS}", array_offset
                        )
                    if length > (len(reader.data) - reader.offset) // 4:
                        raise DecodeError(f"truncated {command_name} i32 array", reader.offset)
                    payload.append(tuple(reader.i32(f"{command_name} array item")
                                         for _ in range(length)))
                else:
                    raise AssertionError(f"bad internal field schema: {field}")
            commands.append(Command(command_id, command_name, tuple(payload), command_offset,
                                    reader.offset - command_offset))
        scripts.append(Script("", tuple(commands), script_index, script_offset, reader.offset))

    reader.finish("program table")
    return tuple(scripts), reader.offset


def decode_script_table(names_data: bytes | bytearray | memoryview,
                        programs_data: bytes | bytearray | memoryview) -> ScriptTable:
    names = parse_script_names(names_data)
    programs, _ = parse_programs(programs_data, expected_script_count=len(names))
    scripts = tuple(Script(names[index], script.commands, index,
                           script.byte_offset, script.byte_end)
                    for index, script in enumerate(programs))
    return ScriptTable(names, scripts)


def _ascii_fold(value: str) -> str:
    return "".join(chr(ord(char) + 32) if "A" <= char <= "Z" else char for char in value)


def resolve_script_id(names: Sequence[str], name: str, start_index: int = 0) -> int:
    """Return first ASCII case-insensitive match at/after start_index, else -1.

    ScriptManager::GetIDFromName uses this first-match scan. For a trigger
    property, pass the number of common names so level-only names resolve in
    the appended level table. For generic/tutorial lookup, pass zero.
    """
    if start_index < 0:
        raise ValueError("start_index must be nonnegative")
    needle = _ascii_fold(name)
    for index in range(start_index, len(names)):
        if _ascii_fold(names[index]) == needle:
            return index
    return -1
