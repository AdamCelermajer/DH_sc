"""Validate owned Skill/Faery cache rows and execute original row readers."""
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
MANIFEST = MODULE / "reference/skill-faery-tables/original-functions.json"
ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
ROW_FUNCTIONS = (
    "_ZN7Structs9SkillList4readEP11IStreamBase",
    "_ZN7Structs9FaeryList4readEP11IStreamBase",
    "_ZN7Structs5Skill4readEP11IStreamBase",
    "_ZN7Structs5Faery4readEP11IStreamBase",
)
PRIMITIVES = {
    0x4DB89C: "read_bool",
    0x3DF1A0: "read_unsigned",
    0x459090: "read_signed",
    0x317454: "read_string_ex",
    0x31056C: "allocate_array",
    0x310440: "free_array",
}
OWNER, STREAM, HEAP = 0x2100000, 0x2200000, 0x2600000


class Reader:
    def __init__(self, raw):
        self.raw, self.offset = raw, 0

    def take(self, amount):
        if amount < 0 or self.offset + amount > len(self.raw):
            raise AssertionError("truncated cache fixture")
        value = self.raw[self.offset:self.offset + amount]
        self.offset += amount
        return value

    def word(self):
        return struct.unpack("<I", self.take(4))[0]

    def integer(self):
        return struct.unpack("<i", self.take(4))[0]

    def string(self):
        return self.take(self.word()).decode("utf-8")

    def sections(self):
        values = []
        while self.offset != len(self.raw):
            count = self.word()
            values.append([self.string() for _ in range(count)])
        return values

    def finish(self):
        assert self.offset == len(self.raw)


def parse_skills(records, names):
    name_sections = Reader(names).sections()
    reader = Reader(records)
    lists = []
    count = reader.word()
    assert count == len(name_sections[0])
    list_wires = []
    for name in name_sections[0]:
        start = reader.offset
        size = reader.word()
        members = [reader.integer() for _ in range(size)]
        lists.append({"name": name, "members": members})
        list_wires.append(records[start:reader.offset])
    rows = []
    row_wires = []
    assert reader.word() == len(name_sections[1])
    for name in name_sections[1]:
        start = reader.offset
        row = {
            "table_name": name,
            "anim": reader.integer(),
            "anim_is_moving": bool(reader.take(1)[0]),
        }
        prop_count = reader.word()
        row["display_props"] = [reader.integer() for _ in range(prop_count)]
        row["elemental_type"] = reader.integer()
        row["fairie_dependant_text"] = bool(reader.take(1)[0])
        row["flags"] = reader.integer()
        row["level"] = reader.integer()
        row["script_length"] = reader.integer()
        row["script"] = reader.take(row["script_length"]).decode("utf-8")
        row["skill_assignable"] = bool(reader.take(1)[0])
        row["skill_curr_level"] = reader.integer()
        row["skill_description"] = reader.integer()
        row["skill_icon_length"] = reader.integer()
        row["skill_icon"] = reader.take(row["skill_icon_length"]).decode("utf-8")
        row["skill_name"] = reader.integer()
        row["skill_next_level"] = reader.integer()
        row["type"] = reader.integer()
        assert len(row["script"].encode()) == row["script_length"]
        assert len(row["skill_icon"].encode()) == row["skill_icon_length"]
        rows.append(row)
        row_wires.append(records[start:reader.offset])
    reader.finish()
    return name_sections, lists, rows, list_wires, row_wires


def parse_faeries(records, names):
    name_sections = Reader(names).sections()
    reader = Reader(records)
    lists = []
    list_wires = []
    assert reader.word() == len(name_sections[0])
    for name in name_sections[0]:
        start = reader.offset
        size = reader.word()
        members = [reader.integer() for _ in range(size)]
        lists.append({"name": name, "members": members})
        list_wires.append(records[start:reader.offset])
    rows = []
    row_wires = []
    assert reader.word() == len(name_sections[1])
    for name in name_sections[1]:
        start = reader.offset
        row = {"table_name": name}
        row["description"] = reader.integer()
        row["elemental"] = reader.integer()
        row["model_file"] = reader.integer()
        row["name_id"] = reader.integer()
        row["spell_script_length"] = reader.integer()
        row["spell_script"] = reader.take(row["spell_script_length"]).decode("utf-8")
        row["spell_type"] = reader.integer()
        row["type"] = reader.integer()
        assert len(row["spell_script"].encode()) == row["spell_script_length"]
        rows.append(row)
        row_wires.append(records[start:reader.offset])
    reader.finish()
    return name_sections, lists, rows, list_wires, row_wires


def sha256(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def arm_rows(original, skill_lists, skills, faery_lists, faeries,
             skill_list_wires, skill_wires, faery_list_wires, faery_wires):
    from unicorn import UC_HOOK_CODE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    cpu = Cpu(original, False, manifest)
    for name in ROW_FUNCTIONS:
        row = next(entry for entry in manifest["functions"] if entry["original_symbol"] == name)
        assert cpu.symbols[name] == int(row["elf_address"], 16)

    wires = {"skill_list": skill_list_wires, "faery_list": faery_list_wires,
             "skill": skill_wires, "faery": faery_wires}
    functions = {"skill_list": ROW_FUNCTIONS[0], "faery_list": ROW_FUNCTIONS[1],
                 "skill": ROW_FUNCTIONS[2], "faery": ROW_FUNCTIONS[3]}
    results = {"skill_list": [], "faery_list": [], "skill": [], "faery": []}
    def check_i32(address, offset):
        return struct.unpack("<i", bytes(cpu.uc.mem_read(address + offset, 4)))[0]

    def read_string(length_at, pointer_at):
        length = check_i32(OWNER, length_at)
        assert 0 <= length <= 1 << 20
        pointer = struct.unpack("<I", bytes(cpu.uc.mem_read(OWNER + pointer_at, 4)))[0]
        assert pointer != 0
        assert bytes(cpu.uc.mem_read(pointer + length, 1)) == b"\0"
        return bytes(cpu.uc.mem_read(pointer, length)).decode("utf-8")

    class State:
        def __init__(self, wire):
            self.wire = wire
            self.cursor = 0
            self.heap = HEAP
            self.allocations = []
            self.calls = []

        def take(self, amount):
            if amount < 0 or self.cursor + amount > len(self.wire):
                raise AssertionError("source reader consumed beyond cache row")
            value = self.wire[self.cursor:self.cursor + amount]
            self.cursor += amount
            return value

    state = None

    def primitive(uc, address, _size, _user):
        name = PRIMITIVES[address]
        state.calls.append(name)
        if name.startswith("read_"):
            assert cpu.reg(0) == STREAM
            if name == "read_bool": amount = 1
            elif name in ("read_unsigned", "read_signed"): amount = 4
            else:
                assert cpu.reg(3) == 0
                amount = cpu.reg(2)
            raw = state.take(amount)
            if amount: uc.mem_write(cpu.reg(1), raw)
        elif name == "allocate_array":
            amount = cpu.reg(0)
            if amount > (1 << 20) + 1:
                raise AssertionError(f"unexpected allocation size {amount:#x} while reading {current_group}[{current_index}]")
            pointer = state.heap
            storage_size = max(1, amount)  # source new[](0) still returns owned storage
            state.heap = (pointer + storage_size + 15) & ~15
            assert state.heap < 0x4000000
            uc.mem_write(pointer, b"\0" * storage_size)
            state.allocations.append((pointer, storage_size))
            cpu.put(0, pointer)
        elif name == "free_array":
            pointer = cpu.reg(0)
            assert pointer == 0 or any(base <= pointer < base + size for base, size in state.allocations)
        else:
            raise AssertionError(name)
        uc.reg_write(cpu.pc, uc.reg_read(cpu.lr))

    for address in PRIMITIVES:
        cpu.uc.hook_add(UC_HOOK_CODE, primitive, begin=address, end=address)

    for group in ("skill_list", "faery_list", "skill", "faery"):
        for index, wire in enumerate(wires[group]):
            current_group, current_index = group, index
            state = State(wire)
            cpu.uc.mem_write(OWNER, b"\0" * 0x100)
            cpu.uc.mem_write(STREAM, b"\0" * 0x100)
            cpu.invoke(functions[group], [OWNER, STREAM])
            assert state.cursor == len(wire), (group, index, state.cursor, len(wire))
            if group in ("skill_list", "faery_list"):
                row = {"name": (skill_lists if group == "skill_list" else faery_lists)[index]["name"]}
                row["members"] = []
                count = check_i32(OWNER, 4)
                assert 0 <= count <= 4096
                if count:
                    pointer = struct.unpack("<I", bytes(cpu.uc.mem_read(OWNER + 8, 4)))[0]
                    row["members"] = [check_i32(pointer, n * 4) for n in range(count)]
            elif group == "skill":
                expected = skills[index]
                row = {"table_name": expected["table_name"], "anim": check_i32(OWNER, 4),
                       "anim_is_moving": bool(cpu.uc.mem_read(OWNER + 8, 1)[0])}
                prop_count = check_i32(OWNER, 0xc)
                pointer = struct.unpack("<I", bytes(cpu.uc.mem_read(OWNER + 0x10, 4)))[0]
                row["display_props"] = [check_i32(pointer, n * 4) for n in range(prop_count)] if prop_count else []
                row.update(elemental_type=check_i32(OWNER, 0x14),
                           fairie_dependant_text=bool(cpu.uc.mem_read(OWNER + 0x18, 1)[0]),
                           flags=check_i32(OWNER, 0x1c), level=check_i32(OWNER, 0x20),
                           script_length=check_i32(OWNER, 0x24), script=read_string(0x24, 0x28),
                           skill_assignable=bool(cpu.uc.mem_read(OWNER + 0x2c, 1)[0]),
                           skill_curr_level=check_i32(OWNER, 0x30), skill_description=check_i32(OWNER, 0x34),
                           skill_icon_length=check_i32(OWNER, 0x38), skill_icon=read_string(0x38, 0x3c),
                           skill_name=check_i32(OWNER, 0x40), skill_next_level=check_i32(OWNER, 0x44),
                           type=check_i32(OWNER, 0x48))
            else:
                expected = faeries[index]
                row = {"table_name": expected["table_name"], "description": check_i32(OWNER, 4),
                       "elemental": check_i32(OWNER, 8), "model_file": check_i32(OWNER, 0xc),
                       "name_id": check_i32(OWNER, 0x10), "spell_script_length": check_i32(OWNER, 0x14),
                       "spell_script": read_string(0x14, 0x18), "spell_type": check_i32(OWNER, 0x1c),
                       "type": check_i32(OWNER, 0x20)}
            expected = {"skill_list": skill_lists, "faery_list": faery_lists,
                        "skill": skills, "faery": faeries}[group][index]
            assert row == expected, (group, index, row, expected)
            results[group].append({"name": expected.get("table_name", expected.get("name")),
                                   "reader_calls": state.calls, "source_bytes": len(wire)})
    return results


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", type=Path)
    parser.add_argument("--cache", type=Path,
                        default=ROOT.parent / "cache/files")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build/skill-faery-tables")
    args = parser.parse_args()
    compiler = args.compiler or os.environ.get("CXX") or shutil.which("g++")
    if not compiler:
        parser.error("pass --compiler or set CXX")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    source_paths = [MODULE / "skill_tables.cpp", MODULE / "tests/skill_tables.cpp"]
    executable = output / "host.exe"
    command = [str(compiler), "-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic",
               *map(str, source_paths), "-o", str(executable)]
    built = subprocess.run(command, cwd=ROOT, capture_output=True, text=True)
    if built.returncode:
        raise RuntimeError(built.stdout + built.stderr)

    pydata = args.cache / "data/pydata"
    cache_names = ("skills_pyarray.bin", "skills_pyarraynames.bin", "skills_pystructnames.bin",
                   "faeries_pyarray.bin", "faeries_pyarraynames.bin", "faeries_pystructnames.bin")
    paths = [pydata / name for name in cache_names]
    assert all(path.is_file() for path in paths), "missing local cache-only skill/faery input"
    skill_records, skill_names, skill_schema, faery_records, faery_names, faery_schema = [p.read_bytes() for p in paths]
    skill_name_sections, skill_lists, skills, skill_list_wires, skill_wires = parse_skills(skill_records, skill_names)
    faery_name_sections, faery_lists, faeries, faery_list_wires, faery_wires = parse_faeries(faery_records, faery_names)
    host_raw = subprocess.check_output([str(executable), *map(str, paths)], cwd=ROOT, text=True)
    host = json.loads(host_raw)
    assert host["validation"] == "PASS"
    assert host["skill_lists"] == skill_lists and host["skills"] == skills
    assert host["faery_lists"] == faery_lists and host["faeries"] == faeries
    assert skill_name_sections[0][3] == "DEFAULT" and skill_lists[3]["members"] == []
    assert faery_name_sections[0][0] == "DEFAULT" and faery_lists[0]["members"] == [2, 4, 5, 6, 3]

    original = args.original_elf.resolve()
    assert hashlib.sha256(original.read_bytes()).hexdigest() == ORIGINAL_SHA
    arm = arm_rows(original, skill_lists, skills, faery_lists, faeries,
                   skill_list_wires, skill_wires, faery_list_wires, faery_wires)
    assert len(arm["skill_list"]) == 36 and len(arm["skill"]) == 127
    assert len(arm["faery_list"]) == 4 and len(arm["faery"]) == 16

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    from elftools.elf.elffile import ELFFile
    raw_elf = original.read_bytes()
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in elf.get_section_by_name(".symtab").iter_symbols()}
        segments = [segment for segment in elf.iter_segments() if segment["p_type"] == "PT_LOAD"]
        for item in manifest["functions"]:
            symbol = symbols[item["original_symbol"]]
            address, size = int(item["elf_address"], 16), item["size"]
            assert (int(symbol["st_value"]), int(symbol["st_size"])) == (address, size)
            segment = next(s for s in segments if int(s["p_vaddr"]) <= address and
                           address + size <= int(s["p_vaddr"]) + int(s["p_filesz"]))
            offset = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            assert hashlib.sha256(raw_elf[offset:offset + size]).hexdigest() == item["sha256"]

    report_inputs = source_paths + [MODULE / "skill_tables.hpp", MODULE / "data.hpp",
                                    Path(__file__).resolve(), MANIFEST]
    report_inputs += [MANIFEST.with_name("NOTES.md")] if MANIFEST.with_name("NOTES.md").exists() else []
    report = {
        "validation": "PASS",
        "native_wired": False,
        "cache_row_comparisons": 183,
        "original_arm_row_comparisons": 183,
        "original_arm_by_table": {key: len(value) for key, value in arm.items()},
        "rejection_cases": host["rejection_cases"],
        "mismatches": 0,
        "crypt_ghost_projection": {
            "skill_list_fallback_index": 3,
            "skill_list_fallback_name": skill_lists[3]["name"],
            "skill_members": skill_lists[3]["members"],
            "faery_list_fallback_index": 0,
            "faery_members": faery_lists[0]["members"],
            "faery_names": [faeries[index]["table_name"] for index in faery_lists[0]["members"]],
            "spell_script_lengths": [faeries[index]["spell_script_length"] for index in faery_lists[0]["members"]],
        },
        "source_sha256": {p.relative_to(ROOT).as_posix(): sha256(p) for p in report_inputs},
        "cache_sha256": {p.name: sha256(p) for p in paths},
        "original_elf_sha256": ORIGINAL_SHA,
        "original_functions": manifest["functions"],
        "modeled_dependencies": manifest["modeled_dependencies"],
        "compiler_command": command,
        "executable_sha256": sha256(executable),
        "scope": ("Owned data/schema decoder plus actual-cache comparison. Original Structs::SkillList, "
                  "FaeryList, Skill and Faery row-reader instructions execute on every actual record; "
                  "stream primitive, bounded allocation/free and readStringEx dependencies are modeled. "
                  "The four Arrays table-reader wrappers are pinned/disassembly-reviewed but not executed. "
                  "This does not wire native SetSkillsAndSpells or publish source Lua runtimes."),
    }
    report_path = output / "validation.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"validation": "PASS", "cache_rows": 183,
                      "original_arm_rows": 183, "rejection_cases": host["rejection_cases"],
                      "mismatches": 0, "report": str(report_path)}))


if __name__ == "__main__":
    main()
