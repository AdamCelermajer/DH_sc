"""Host checks and original ARM caller-flow comparison for SetSkillsAndSpells."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
MANIFEST = MODULE / "reference/character-ai-set-skills-and-spells/original-functions.json"
ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
AI = 0x02010000
OWNER = 0x02020000
AIS = 0x02030000
DEBUG = 0x02031000
CALLBACK = 0x02032000
CASES = ("baseline", "vectors_nonempty", "skill_only_empty", "faery_only_empty",
         "empty_lists", "all_gated_null", "load_fail")


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def source_symbols(original: Path, manifest: dict) -> None:
    from elftools.elf.elffile import ELFFile

    raw = original.read_bytes()
    assert digest(original) == ORIGINAL_SHA
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        sections = [elf.get_section_by_name(".symtab"), elf.get_section_by_name(".dynsym")]
        symbols = {symbol.name: symbol for section in sections if section
                   for symbol in section.iter_symbols()}
        segments = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]

        def read(address: int, size: int) -> bytes:
            segment = next(item for item in segments if int(item["p_vaddr"]) <= address and
                           address + size <= int(item["p_vaddr"]) + int(item["p_filesz"]))
            offset = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            return raw[offset:offset + size]

        for entry in manifest["functions"]:
            address, size = int(entry["elf_address"], 0), int(entry["size"])
            symbol = symbols[entry["original_symbol"]]
            assert int(symbol["st_value"]) == address
            assert int(symbol["st_size"]) == size
            assert hashlib.sha256(read(address, size)).hexdigest() == entry["sha256"]
    return None


def run_arm(original: Path, executable: Path) -> dict:
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    source_symbols(original, manifest)
    sys.path.insert(0, str(MODULE / "tests"))
    from elf_import_identity import verify_imports
    verified_imports = verify_imports(original, {0x30e964: "__aeabi_i2f"})

    # This caller's `slot`→Lua number conversion is a genuine ARM EABI import.
    # Execute the ABI conversion, not a guessed call-site value.
    original_external = Cpu.external

    def external(cpu, uc, address, size, user):
        name = cpu.imports.get(address)
        if name == "puts":
            raise AssertionError(f"unexpected diagnostic puts from return PC {uc.reg_read(cpu.lr):#x}; "
                                 f"this={cpu.reg(0):#x}, args={cpu.reg(1):#x}")
        if name == "__aeabi_i2f":
            integer = cpu.reg(0)
            bits = struct.unpack("<I", struct.pack("<f", float(integer)))[0]
            cpu.put(0, bits)
            uc.reg_write(cpu.pc, uc.reg_read(cpu.lr))
            cpu.import_calls[name] = cpu.import_calls.get(name, 0) + 1
            return
        return original_external(cpu, uc, address, size, user)

    Cpu.external = external
    cpu = Cpu(original, False, manifest)
    Cpu.external = original_external
    bump = cpu.data + 0x10000
    allocated: list[tuple[int, int]] = []

    def alloc(size: int, alignment: int = 16) -> int:
        nonlocal bump
        bump = (bump + alignment - 1) & ~(alignment - 1)
        result = bump
        bump += max(size, 1)
        allocated.append((result, size))
        cpu.uc.mem_write(result, bytes(max(size, 1)))
        return result

    def word(address: int) -> int:
        return struct.unpack("<I", cpu.uc.mem_read(address, 4))[0]

    def putword(address: int, value: int) -> None:
        cpu.pointer(address, value & 0xffffffff)

    def cstring(address: int) -> str:
        data = bytearray()
        for offset in range(4096):
            value = cpu.uc.mem_read(address + offset, 1)[0]
            if not value:
                return data.decode("ascii")
            data.append(value)
        raise AssertionError("unterminated source C string")

    def std_string(address: int) -> str:
        begin, end = word(address + 0x14), word(address + 0x10)
        if not begin or end < begin or end - begin > 0x1000:
            raise AssertionError(f"invalid source string range {address:#x}: {begin:#x}..{end:#x}")
        return bytes(cpu.uc.mem_read(begin, end - begin)).decode("ascii")

    def set_string(address: int, value: bytes) -> None:
        storage = alloc(len(value) + 1)
        cpu.uc.mem_write(storage, value + b"\0")
        putword(address, storage + len(value) + 1)       # _Rep capacity end
        putword(address + 0x10, storage + len(value))   # end
        putword(address + 0x14, storage)               # begin

    source_rows = {
        "baseline": {"skill_count": 3, "faery_count": 3, "skill_gates": (1, 0, 1),
                     "faery_gates": (1, 0, 1), "load_success": True},
        "vectors_nonempty": {"skill_count": 3, "faery_count": 3, "skill_gates": (1, 0, 1),
                              "faery_gates": (1, 0, 1), "load_success": True},
        "skill_only_empty": {"skill_count": 3, "faery_count": 3, "skill_gates": (1, 0, 1),
                             "faery_gates": (1, 0, 1), "load_success": True},
        "faery_only_empty": {"skill_count": 3, "faery_count": 3, "skill_gates": (1, 0, 1),
                             "faery_gates": (1, 0, 1), "load_success": True},
        "empty_lists": {"skill_count": 0, "faery_count": 0, "skill_gates": (),
                        "faery_gates": (), "load_success": True},
        "all_gated_null": {"skill_count": 3, "faery_count": 3, "skill_gates": (0, 0, 0),
                            "faery_gates": (0, 0, 0), "load_success": True},
        "load_fail": {"skill_count": 3, "faery_count": 3, "skill_gates": (1, 0, 1),
                      "faery_gates": (1, 0, 1), "load_success": False},
    }

    def one_case(case: str) -> dict:
        config = source_rows[case]
        # Reset only the fixture address arena and stack between original invocations.
        cpu.uc.mem_write(cpu.stack, bytes(0x10000))
        cpu.uc.mem_write(AI, bytes(0x1000))
        cpu.uc.mem_write(AIS, bytes(0x100))
        cpu.uc.mem_write(DEBUG, bytes(0x100))
        cpu.uc.mem_write(CALLBACK, bytes(0x100))
        start = cpu.data + 0x10000
        putword(AI + 4, OWNER)
        putword(AI + 0x1c, AIS)
        putword(AIS, cpu.data + 0x8000)
        putword(AIS + 0x68, AIS + 0x68)
        set_string(AIS + 0x68, b"data/scripts/ai/")
        # Fake active-AIS vtable dispatch at the source's +0xcc slot.
        vtable = alloc(0x100)
        putword(AIS, vtable)
        putword(vtable + 0xcc, CALLBACK)
        skill_rows = alloc(max(1, config["skill_count"]) * 0x40)
        faery_rows = alloc(max(1, config["faery_count"]) * 0x40)
        skill_names = [alloc(len(text) + 1) for text in (b"Skill_One", b"Skill_Two")]
        faery_names = [alloc(len(text) + 1) for text in (b"Faery_One", b"Faery_Two")]
        for address, text in zip(skill_names, (b"Skill_One", b"Skill_Two")):
            cpu.uc.mem_write(address, text + b"\0")
        for address, text in zip(faery_names, (b"Faery_One", b"Faery_Two")):
            cpu.uc.mem_write(address, text + b"\0")
        for slot, gate in enumerate(config["skill_gates"]):
            row = skill_rows + slot * 0x40
            putword(row + 0x24, gate)
            putword(row + 0x28, skill_names[0 if slot == 0 else 1] if gate else 0)
        for slot, gate in enumerate(config["faery_gates"]):
            row = faery_rows + slot * 0x40
            putword(row + 0x14, gate)
            putword(row + 0x18, faery_names[0 if slot == 0 else 1] if gate else 0)
        skill_list = alloc(8)
        faery_list = alloc(8)
        putword(skill_list + 4, config["skill_count"])
        putword(faery_list + 4, config["faery_count"])
        skill_vec, faery_vec = AI + 0xb4, AI + 0xc0
        skill_storage = alloc(0x100)
        faery_storage = alloc(0x100)
        if case in ("vectors_nonempty", "faery_only_empty"):
            putword(skill_vec, skill_storage)
            putword(skill_vec + 4, skill_storage + 4)
            putword(skill_vec + 8, skill_storage + 0x100)
            putword(skill_storage, 0x11110001)
        if case in ("vectors_nonempty", "skill_only_empty"):
            putword(faery_vec, faery_storage)
            putword(faery_vec + 4, faery_storage + 4)
            putword(faery_vec + 8, faery_storage + 0x100)
            putword(faery_storage, 0x22220001)

        events: list[dict] = []
        args_addresses: dict[int, int] = {}
        current_arg_base: dict[int, int] = {}
        current_arg_count: dict[int, int] = {}
        next_arg = 0
        next_script = 0
        next_value = 0
        function_addresses = {int(row["elf_address"], 0): row["original_symbol"]
                              for row in manifest["functions"]}
        hook_addresses = set(function_addresses)
        hook_addresses.update((0x708f00, 0x310440))
        append_pcs = {
            0x3ce390: (18, 2), 0x3ce3ec: (18, 2), 0x3ce434: (18, 2),
            0x3ce5dc: (18, 1), 0x3ce638: (18, 1), 0x3ce680: (18, 1),
        }

        def returned(value: int = 0) -> None:
            cpu.put(0, value)
            cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))

        def observe(_uc, address: int, _size: int, _user) -> None:
            nonlocal next_arg, next_script, next_value
            if address in append_pcs:
                operation, list_id = append_pcs[address]
                slot = cpu.reg(5)
                vector_end = cpu.reg(1)
                identity = cpu.reg(6) if address in (0x3ce390, 0x3ce5dc) else 0
                events.append({"op": "append", "id": operation, "list": list_id,
                               "slot": slot, "identity": identity, "vector_end": vector_end})
                return
            if address not in hook_addresses and address != CALLBACK:
                return
            name = function_addresses.get(address)
            op = {
                "_ZN13DebugSwitches4loadEv": "debug_load",
                "_ZN13DebugSwitches9GetSwitchERKSs": "debug_get_switch",
                "_ZNSsC1EPKcRKSaIcE": "string_construct",
                "_ZNSs9_M_assignEPKcS0_": "set_script_path",
                "_ZNSs19_M_range_initializeEPKcS0_": "capture_script_path",
                "_ZNK9Character16GetCharSkillListEv": "get_skill_list",
                "_ZNK9Character12GetCharSkillEi": "get_skill",
                "_ZNK9Character16GetCharFaeryListEv": "get_faery_list",
                "_ZNK9Character12GetCharFaeryEi": "get_faery",
                "_ZNSt6vectorIP17CharAISkillScriptSaIS1_EE7reserveEj": "reserve",
                "_ZN3sfc6script3lua9ArgumentsC1Ev": "arguments_construct",
                "_ZN3sfc6script3lua9Arguments10pushStringEPKc": "arguments_push_string",
                "_ZN3sfc6script3lua9Arguments11pushIntegerEi": "arguments_push_integer",
                "_ZN3sfc6script3lua9ArgumentsD1Ev": "arguments_destroy",
                "_ZN3sfc6script3lua5Value9setStringEPKc": "arguments_set_string",
                "_ZN3sfc6script3lua5Value9setNumberEf": "arguments_set_number",
                "_ZN9LuaScript4LoadEPKc": "load_script",
                "_ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsE": "call_script_args",
                "_ZN9LuaScript4CallEPKc": "call_script",
                "_Znwj15MemoryHintState": "allocate_skill_script",
                "_ZN17CharAISkillScriptC1EP9CharacterPKcj": "construct_skill_script",
                "_ZNSsD1Ev": "string_destroy",
            }.get(name)
            if address == CALLBACK:
                events.append({"op": "init_vcb", "id": 19, "ai": cpu.reg(0)})
                returned()
                return
            if address == 0x708f00 or address == 0x310440:
                # Free/deallocation boundary for a small temporary source string.
                # The final saved path lease is released after the InitVCB call.
                if address == 0x708f00 and any(event.get("op") == "init_vcb" for event in events):
                    events.append({"op": "release_script_path", "id": 20})
                returned()
                return
            if op is None:
                return
            r0, r1, r2, r3 = [cpu.reg(i) for i in range(4)]
            event: dict = {"op": op}
            ids = {
                "debug_load": 0, "debug_get_switch": 1, "capture_script_path": 2,
                "set_script_path": 3, "get_skill_list": 4, "get_skill": 5,
                "get_faery_list": 6, "get_faery": 7, "reserve": 8,
                "arguments_construct": 9, "arguments_push_string": 10,
                "arguments_push_integer": 11, "arguments_set_string": 12,
                "arguments_set_number": 13, "arguments_destroy": 14,
                "load_script": 15, "call_script_args": 16, "call_script": 16,
                "allocate_skill_script": 17,
            }
            if op in ids:
                event["id"] = ids[op]
            if op == "debug_load":
                event["receiver"] = r0
                returned()
            elif op == "debug_get_switch":
                key = std_string(r1)
                event.update(receiver=r0, key=key)
                returned(0)
            elif op == "string_construct":
                value = cstring(r1).encode("ascii")
                event["value"] = value.decode("ascii")
                set_string(r0, value)
                returned(r0)
            elif op == "set_script_path":
                value = bytes(cpu.uc.mem_read(r1, r2 - r1))
                event.update(receiver=r0, value=value.decode("ascii"))
                set_string(r0, value)
                returned(r0)
            elif op == "capture_script_path":
                event.update(receiver=r0, begin=r1, end=r2,
                             value=bytes(cpu.uc.mem_read(r1, r2-r1)).decode("ascii"))
                set_string(r0, event["value"].encode("ascii"))
                returned(r0)
            elif op == "get_skill_list":
                event.update(receiver=r0, list=1)
                returned(skill_list)
            elif op == "get_faery_list":
                event.update(receiver=r0, list=2)
                returned(faery_list)
            elif op == "get_skill":
                event.update(receiver=r0, list=1, slot=r1)
                returned(skill_rows + r1 * 0x40)
            elif op == "get_faery":
                event.update(receiver=r0, list=2, slot=r1)
                returned(faery_rows + r1 * 0x40)
            elif op == "reserve":
                event.update(vector=r0, count=r1)
                if r1 > 0:
                    storage = skill_storage if r0 == skill_vec else faery_storage
                    putword(r0, storage)
                    putword(r0 + 4, storage)
                    putword(r0 + 8, storage + 0x100)
                returned()
            elif op == "arguments_construct":
                arg = r0
                vector = alloc(0x20)
                values = alloc(0x100)
                value_base = alloc(0x200)
                putword(arg, cpu.data + 0x7000)  # source Arguments vtable tag
                putword(arg + 4, vector)
                putword(vector, values)
                putword(vector + 4, values)
                putword(vector + 8, values + 0x100)
                args_addresses[arg] = vector
                current_arg_base[arg] = value_base
                current_arg_count[arg] = 0
                event.update(arguments=arg, values=values, vector=vector)
                returned(arg)
            elif op == "arguments_push_string":
                arg = r0
                event.update(arguments=arg, text=cstring(r1), current_size=current_arg_count[arg])
                putword(args_addresses[arg], current_arg_base[arg])
                current_arg_count[arg] += 1
                putword(args_addresses[arg] + 4, word(args_addresses[arg]) + current_arg_count[arg] * 16)
                returned(arg)
            elif op == "arguments_push_integer":
                arg = r0
                event.update(arguments=arg, integer=struct.unpack("<i", struct.pack("<I", r1))[0],
                             current_size=current_arg_count[arg])
                putword(args_addresses[arg], current_arg_base[arg])
                current_arg_count[arg] += 1
                putword(args_addresses[arg] + 4, word(args_addresses[arg]) + current_arg_count[arg] * 16)
                returned(arg)
            elif op == "arguments_destroy":
                event.update(arguments=r0, size=current_arg_count.get(r0, 0))
                returned(r0)
            elif op == "arguments_set_string":
                event.update(value_address=r0, value=cstring(r1), name=cstring(r1))
                returned(r0)
            elif op == "arguments_set_number":
                event.update(value_address=r0, raw_number_bits=r1, number_bits=r1)
                returned(r0)
            elif op == "load_script":
                script = cstring(r1)
                event.update(ai=r0, script=script, text=script)
                returned(1 if script != "_commons" and config["load_success"] else 0)
            elif op in ("call_script_args", "call_script"):
                function = cstring(r1)
                event.update(ai=r0, function=function,
                             arguments=r2 if op == "call_script_args" else None,
                             text=function)
                returned()
            elif op == "allocate_skill_script":
                event.update(bytes=r0, hint=r1, allocation_bytes=r0, allocation_hint=r1)
                created = alloc(r0)
                next_script += 1
                event["allocation"] = created
                returned(created)
            elif op == "construct_skill_script":
                event.update(identity=r0, owner=r1, script=cstring(r2), index=r3,
                             slot=r3 if r3 != 0xffffffff else 0)
                returned(r0)
            elif op == "string_destroy":
                returned(r0)
            events.append(event)

        hook = cpu.uc.hook_add(UC_HOOK_CODE, observe)
        returned_value = cpu.invoke("_ZN6CharAI18SetSkillsAndSpellsEv", [AI], budget=2_000_000)
        cpu.uc.hook_del(hook)
        # The small bump allocator is deliberately reset per case; all pointer
        # values remain valid for each complete source invocation.
        assert returned_value == 0
        assert cpu.symbols["_ZN6CharAI18SetSkillsAndSpellsEv"] in cpu.seen
        assert events[-1]["op"] == "release_script_path", events[-5:]
        return {"case": case, "events": events,
                "skill_values": read_vector(cpu, AI + 0xb4),
                "faery_values": read_vector(cpu, AI + 0xc0),
                "init_vcb": sum(event["op"] == "init_vcb" for event in events),
                "source_instructions_seen": sum(0x3ce044 <= address < 0x3ce7a0 for address in cpu.seen)}

    def read_vector(cpu, address):
        begin, end = word(address), word(address + 4)
        if begin == end == 0:
            return []
        if not begin or end < begin or (end - begin) % 4:
            raise AssertionError(f"invalid result vector at {address:#x}: {begin:#x}..{end:#x}")
        return list(struct.unpack("<" + "I" * ((end - begin) // 4), cpu.uc.mem_read(begin, end - begin)))

    observations = []
    for case in CASES:
        i2f_before = cpu.import_calls.get("__aeabi_i2f", 0)
        arm = one_case(case)
        i2f_calls = cpu.import_calls.get("__aeabi_i2f", 0) - i2f_before
        expected_i2f_calls = (0 if case in ("vectors_nonempty", "faery_only_empty")
                              else sum(source_rows[case]["skill_gates"]))
        assert i2f_calls == expected_i2f_calls, (case, i2f_calls, expected_i2f_calls)
        arm["i2f_calls"] = i2f_calls
        host = json.loads(subprocess.check_output([str(executable), case], text=True))
        arm_ids = [event["id"] for event in arm["events"] if "id" in event]
        host_ids = [event[0] for event in host["events"]]
        assert arm_ids == host_ids, (case, arm_ids, host_ids, arm["events"])
        arm_skill_presence = [value != 0 for value in arm["skill_values"]]
        arm_faery_presence = [value != 0 for value in arm["faery_values"]]
        assert arm_skill_presence == [value != 0 for value in host["skill_values"]], (case, arm, host)
        assert arm_faery_presence == [value != 0 for value in host["faery_values"]], (case, arm, host)
        assert arm["init_vcb"] == host["init_vcb_called"] == 1, (case, arm, host)
        if case == "baseline":
            number_bits = [event["raw_number_bits"] for event in arm["events"]
                           if event["op"] == "arguments_set_number"]
            assert number_bits == [0, 0x40000000, 0xbf800000, 0xbf800000], (number_bits, arm["events"])
            child_indices = [event["index"] for event in arm["events"]
                             if event["op"] == "construct_skill_script"]
            assert child_indices == [0, 2, 0xffffffff, 0xffffffff], child_indices
        arm["host_operation_sequence_matches"] = True
        arm["skill_null_pattern_matches"] = True
        arm["faery_null_pattern_matches"] = True
        observations.append(arm)
    # Compare the original call vocabulary and source vector effects to the
    # maintained source-driven host fixture. Child selector internals are a
    # separately tested source unit and GetCharFaery is intercepted at its
    # exact callee boundary here.
    source_body_instructions = sum(0x3ce044 <= address < 0x3ce7a0 for address in cpu.seen)
    assert source_body_instructions > 180
    return {"validation": "PASS", "cases": len(observations), "mismatches": 0,
            "observations": observations, "original_caller_instructions_observed": source_body_instructions,
            "verified_imports": verified_imports,
            "original_i2f_calls": cpu.import_calls.get("__aeabi_i2f", 0),
            "scope": "The original SetSkillsAndSpells caller executes. Its named leaf calls are intercepted at the original boundaries. GetCharFaery's selector is covered by its separate original-ARM unit; Arguments/Value, vector reserve, debug persistence, Lua loading/calls and the child constructor are recorded provider boundaries."
           }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build/character-ai-set-skills-and-spells/host.exe")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build/character-ai-set-skills-and-spells/validation.json")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++")
    if not compiler:
        parser.error("pass --compiler")
    executable = args.output.resolve()
    executable.parent.mkdir(parents=True, exist_ok=True)
    sources = [MODULE / "character_ai_skill_script_constructor.cpp",
               MODULE / "character_faery_selection.cpp",
               MODULE / "character_ai_set_skills_and_spells.cpp",
               MODULE / "tests/character_ai_set_skills_and_spells.cpp"]
    command = [compiler, "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               *map(str, sources), "-o", str(executable)]
    result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
    host = json.loads(subprocess.check_output([str(executable)], text=True))
    assert host["validation"] == "PASS" and host["host_cases"] == 14
    arm = run_arm(args.original_elf.resolve(), executable)
    tracked = sources + [MODULE / "character_ai_set_skills_and_spells.hpp",
                         Path(__file__).resolve(), MANIFEST, MANIFEST.with_name("NOTES.md")]
    report = {"validation": "PASS", "host_report": host, "original_arm_comparison": arm,
              "original_sha256": ORIGINAL_SHA, "compiler_command": command,
              "compiler_diagnostics": result.stderr,
              "source_sha256": {path.relative_to(ROOT).as_posix(): digest(path) for path in tracked},
              "native_wired": False}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "host_cases": host["host_cases"],
                      "original_arm_cases": arm["cases"], "mismatches": 0,
                      "source_instructions": arm["original_caller_instructions_observed"]}))


if __name__ == "__main__":
    main()
