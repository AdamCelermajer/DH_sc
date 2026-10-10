"""Audit source PlayActorAnim execution against the original ELF and campaign data."""
from pathlib import Path
import hashlib
import json
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[4]
IDA = ROOT / ".local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so"
ELF = ROOT / ".local-inputs/ida-ghidra-review/libDungeonHunter2.so"
CAMPAIGN = ROOT / ".local-inputs/windows-source-clock-v19-preview-3/assets/original-campaign.xml"


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def function(symbol: str) -> dict:
    for line in (IDA / "functions.jsonl").read_text(encoding="utf-8").splitlines():
        value = json.loads(line)
        if (value.get("demangled") or "").startswith(symbol):
            return value
    raise AssertionError(f"missing original function {symbol}")


assert digest(ELF) == "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
assert digest(CAMPAIGN) == "3ab2055920a8b648b16d77aa07b0435cbd899c624f5be3bcc145e2133c5cdd14"

read = function("Structs::PlayActorAnim::read")
execute = function("Script_PlayActorAnim::Execute")
initialize = function("Script_PlayActorAnim::Init")
blocking = function("Script_PlayActorAnim::IsBlocking")
assert (read["address"], read["pseudocode_sha256"]) == (
    "0x500ff4", "ecb2395d5b0df56da26babe2f11d904ed9dfbb52a466294bcd8b9bc35b1fc384")
assert (execute["address"], execute["pseudocode_sha256"]) == (
    "0x45e890", "e58b0e4dda733f985e5e7e2ebef44bb8ececcd18d0816232a9f8cfcd6abe4069")
assert (initialize["address"], initialize["pseudocode_sha256"]) == (
    "0x45a2ec", "bba107e7767d79eb39a5385a21c473bc4f06cbf51645aa17f08152bbd936ab1d")
assert (blocking["address"], blocking["pseudocode_sha256"]) == (
    "0x459628", "5cdae1c2c28fec4cd992a442c0b5eff49a8d66bc2c1673929a065381fbf3e32f")

def pseudo(record: dict) -> str:
    path = IDA / record["pseudocode"]
    assert path.is_file() and record["pseudocode_sha256"]
    return path.read_text(encoding="utf-8")


read_body = pseudo(read)
init_body = pseudo(initialize)
execute_body = pseudo(execute)
blocking_body = pseudo(blocking)
assert "readAs<int>((int)a2, (int)(this + 2))" in read_body
assert "readAs<int>((int)a2, (int)(this + 3))" in read_body
assert "readAs<int>((int)a2, (int)(this + 4))" in read_body
assert "readAs<unsigned int>((int)a2, (int)(this + 5))" in read_body
assert "readStringEx(a2" in read_body and "readAs<bool>((int)a2, (int)(this + 7))" in read_body
assert "ANIM_AddAnimDictToSet" in init_body and "*(_DWORD *)(v1 + 8)" in init_body and "*(_DWORD *)(v1 + 12)" in init_body
assert "if ( !a2 )" in execute_body
assert "GetObjectByName" in execute_body and "ObjectHandle::operator Character" in execute_body
assert "v8 + 2" in execute_body and "v8 + 8" in execute_body
assert "SM_SetAnimState" in execute_body
assert "v1 + 28" in blocking_body and "v1 + 16" in blocking_body and "+ 2" in blocking_body

root = ET.parse(CAMPAIGN).getroot()
swamp = next(script for script in root.iter("script") if script.attrib.get("name") == "Swamp_Intro")
commands = {int(command.attrib["index"]): command for command in swamp.findall("command")}
for index, actor, anim, slot in (
    (45, "_prim_NPC_PriestGood", 396, 2),
    (46, "_prim_Faery", 377, 3),
):
    command = commands[index]
    scalars = {int(x.attrib["offset"]): int(x.attrib["bits"]) for x in command.findall("scalar")}
    strings = {int(x.attrib["offset"]): x.attrib["value"] for x in command.findall("string")}
    assert command.attrib["className"] == "Script_PlayActorAnim"
    assert (scalars[8], scalars[12], scalars[16], scalars[20], scalars[28], strings[24]) == (
        anim, 0xFFFFFFFF, slot, len(actor), 0, actor)

print("PlayActorAnim source semantics PASS: offset20 is receiver string byte-count; execution is an actor/FSM AnimDict update, with no authored loop/rate policy for Swamp IDs 396/377")
