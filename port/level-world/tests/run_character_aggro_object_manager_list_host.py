#!/usr/bin/env python3
"""Host checks and original ARM32 differential for ObjectManager's Character list."""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPO = HERE.parents[2]
MANIFEST = MODULE / "reference" / "character-aggro-object-manager-list" / "original-functions.json"
EXPECTED_ELF = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"


def require(condition: bool, message: str) -> None:
    if not condition:
        raise RuntimeError(message)


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def verify_source(original: Path) -> tuple[dict, dict]:
    try:
        from capstone import CS_ARCH_ARM, CS_MODE_ARM, Cs
        from elftools.elf.elffile import ELFFile
    except ImportError as exc:
        raise RuntimeError(f"original ARM oracle dependencies unavailable: {exc}") from exc

    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    image = original.read_bytes()
    require(sha(image) == EXPECTED_ELF == manifest["original_sha256"],
            "the ARM oracle ELF does not match the pinned original")
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {symbol.name: symbol for symbol in elf.get_section_by_name(".symtab").iter_symbols()}
        for row in manifest["functions"]:
            symbol = symbols.get(row["original_symbol"])
            require(symbol is not None, "missing ELF symbol: " + row["original_symbol"])
            address, size = int(row["elf_address"], 0), int(row["size"])
            require((int(symbol["st_value"]), int(symbol["st_size"])) == (address, size),
                    "symbol range changed: " + row["original_symbol"])
            require(sha(image[address:address + size]) == row["sha256"],
                    "function bytes changed: " + row["original_symbol"])
        for row in manifest["vtable_ranges"]:
            symbol = symbols.get(row["symbol"])
            require(symbol is not None, "missing vtable: " + row["symbol"])
            address, size = int(row["elf_address"], 0), int(row["size"])
            require((int(symbol["st_value"]), int(symbol["st_size"])) == (address, size),
                    "vtable range changed: " + row["symbol"])
            require(sha(image[address:address + size]) == row["sha256"],
                    "vtable bytes changed: " + row["symbol"])

        text = elf.get_section_by_name(".text")
        text_bytes = text.data()
        text_address = int(text["sh_addr"])
        decoder = Cs(CS_ARCH_ARM, CS_MODE_ARM)
        decoder.detail = True
        call_edges = []
        for edge in manifest["callers"]:
            caller = symbols[edge["caller_symbol"]]
            start, end = int(caller["st_value"]), int(caller["st_value"] + caller["st_size"])
            instructions = decoder.disasm(text_bytes[start - text_address:end - text_address], start)
            found = {instruction.address: instruction for instruction in instructions}
            call = found.get(int(edge["call_address"], 0))
            require(call is not None and call.mnemonic == "bl" and
                    int(call.operands[0].imm) == int(edge["target"], 0),
                    f"caller edge changed at {edge['call_address']}")
            call_edges.append({"address": edge["call_address"],
                               "target": edge["target"], "meaning": edge["meaning"]})
    return manifest, {"call_edges": call_edges, "symbols": symbols}


def make_cpu(original: Path, manifest: dict):
    sys.path.insert(0, str(REPO / "port" / "engine-resources" / "tests"))
    from cpu import Cpu
    return Cpu(original, False, manifest)


def read_u32(cpu, address: int) -> int:
    return struct.unpack("<I", cpu.uc.mem_read(address, 4))[0]


def read_node_ring(cpu, sentinel: int, limit: int = 32) -> list[tuple[int, int, int, int]]:
    values: list[tuple[int, int, int, int]] = []
    current = read_u32(cpu, sentinel)
    while current != sentinel:
        require(current != 0 and len(values) < limit, "source node ring is malformed or unbounded")
        values.append((current, read_u32(cpu, current), read_u32(cpu, current + 4),
                       read_u32(cpu, current + 8)))
        current = read_u32(cpu, current)
    require(read_u32(cpu, sentinel + 4) == (values[-1][0] if values else sentinel),
            "source sentinel tail link mismatch")
    return values


class AddFixture:
    def __init__(self, original: Path, manifest: dict, lookup_ids: list[int],
                 new_handle_id: int = 11, resolver: dict[int, int] | None = None):
        from unicorn import UC_HOOK_CODE
        self.cpu = make_cpu(original, manifest)
        self.uc = self.cpu.uc
        self.base = self.cpu.data
        self.manager = self.base + 0x1000
        self.new_object = self.base + 0x3000
        self.existing_object = self.base + 0x3800
        self.out = self.base + 0x4000
        self.node_addresses = [self.base + 0x5000, self.base + 0x5020, self.base + 0x5040]
        self.map_slot = self.base + 0x6000
        self.type_name = self.base + 0x7000
        self.object_name = self.base + 0x7010
        self.noop = self.base + 0x7800
        self.fake_vtable = self.base + 0x7900
        self.lookup_ids = list(lookup_ids)
        self.new_handle_id = new_handle_id
        self.resolver = dict(resolver or {})
        self.trace: list[str] = []
        self.nodes_used = 0
        self.character_checks = 0
        self.base_character_checks = 0
        self.noop_calls = 0

        for offset in (0x60, 0x2c, 0x70, 0x88, 0x90):
            self.cpu.pointer(self.manager + offset, self.manager + offset)
            self.cpu.pointer(self.manager + offset + 4, self.manager + offset)
        self.cpu.pointer(self.manager + 0x50, 0)
        self.uc.mem_write(self.type_name, b"CharacterType\0")
        self.uc.mem_write(self.object_name, b"SourceActor\0")
        self.cpu.pointer(self.new_object + 0x2c, self.base + 0x6800)
        self.uc.mem_write(self.new_object + 0x2f4, bytes(8))
        self.uc.mem_write(self.new_object + 0xf4, bytes(4))
        self.uc.mem_write(self.existing_object + 0xf4, bytes(4))
        self.uc.mem_write(self.noop, struct.pack("<I", 0xE12FFF1E))  # bx lr
        self.cpu.pointer(self.fake_vtable + 4, self.noop)
        self.uc.hook_add(UC_HOOK_CODE, self._hook)

    def set_vtable(self, object_address: int, table: int) -> None:
        self.cpu.pointer(object_address, table)

    def _hook(self, uc, address, _size, _user) -> None:
        if address == 0x34aca0:  # GetObjectByName: output ObjectHandle supplied by owner lookup.
            out_handle = self.cpu.reg(0)
            ident = self.lookup_ids.pop(0) if self.lookup_ids else 0
            uc.mem_write(out_handle, struct.pack("<III", ident, 0, 0))
            self.trace.append(f"lookup:{ident}")
            self.cpu.put(0, out_handle)
        elif address == 0x33fdc0:  # ObjectHandle::GetObject(global-manager service boundary).
            handle = self.cpu.reg(0)
            ident = read_u32(self.cpu, handle)
            self.cpu.put(0, self.resolver.get(ident, 0))
            self.trace.append(f"resolve:{ident}")
        elif address == 0x33fc88:  # map operator[] owner-storage boundary.
            self.cpu.put(0, self.map_slot)
            self.trace.append("map-slot")
        elif address == 0x34ac18:  # name setter is outside list semantics.
            self.cpu.put(0, self.cpu.reg(0))
        elif address == 0x30de54:
            address_string = self.cpu.reg(0)
            self.cpu.put(0, bytes(uc.mem_read(address_string, 1024)).split(b"\0", 1)[0].__len__())
        elif address == 0x3109e0:  # string storage is not part of the list owner.
            self.cpu.put(0, self.cpu.reg(0))
        elif address == 0x33dd2c:  # ObjectBase::GetHandle reads the global manager.
            result, object_address = self.cpu.reg(0), self.cpu.reg(1)
            uc.mem_write(result, struct.pack("<III", self.new_handle_id, object_address, 0))
            self.resolver[self.new_handle_id] = object_address
            self.cpu.put(0, result)
        elif address == 0x342690:  # exact list-node allocation boundary.
            require(self.nodes_used < len(self.node_addresses), "Add requested too many nodes")
            node = self.node_addresses[self.nodes_used]
            self.nodes_used += 1
            self.cpu.put(0, node)
        elif address == 0x3a2e1c:
            self.character_checks += 1
            return  # count, then execute the original IsCharacter instruction body.
        elif address == 0x33dcd0:
            self.base_character_checks += 1
            return  # count, then execute the original false-returning base body.
        elif address == self.noop:
            self.noop_calls += 1
            return  # execute the one-instruction test virtual service (bx lr)
        else:
            return
        uc.reg_write(self.cpu.pc, uc.reg_read(self.cpu.lr))

    def add(self, object_address: int, vtable: int, numeric_id: int = 7) -> None:
        self.set_vtable(object_address, vtable)
        self.cpu.pointer(object_address + 0x2c, self.base + 0x6800)
        self.uc.mem_write(object_address + 0x2f4, bytes(8))
        self.uc.mem_write(object_address + 0xf4, bytes(4))
        self.cpu.invoke(0x34b270,
                        [self.out, self.manager, object_address, self.type_name],
                        stack=(self.object_name, numeric_id, 0), budget=300000)


def add_arm_checks(original: Path, manifest: dict) -> dict:
    # The first two source Add calls intentionally register the same Character*
    # under distinct unique names. The real ARM body appends both entries.
    fixture = AddFixture(original, manifest, [0, 0])
    fixture.resolver[11] = fixture.new_object
    fixture.add(fixture.new_object, 0x965F38, 7)
    fixture.add(fixture.new_object, 0x965F38, 8)
    ring = read_node_ring(fixture.cpu, fixture.manager + 0x60)
    require([row[3] for row in ring] == [fixture.new_object, fixture.new_object],
            "original Add did not append the same Character pointer in source order")
    require(read_u32(fixture.cpu, fixture.manager + 0x50) == 2,
            "source ObjectManager count did not advance for both unique adds")
    require(fixture.character_checks == 2 and fixture.nodes_used == 2,
            "source Character virtual/allocation branch did not execute twice")

    # A resolved existing name takes the duplicate branch and invokes the new
    # object's vtable byte offset +4; it must not touch the list or count.
    duplicate = AddFixture(original, manifest, [21], resolver={21: fixture.existing_object})
    duplicate.set_vtable(duplicate.new_object, duplicate.fake_vtable)
    duplicate.add(duplicate.new_object, duplicate.fake_vtable, 9)
    require(duplicate.noop_calls == 1,
            "source duplicate branch did not call the incoming object's vtable +4")
    require(read_node_ring(duplicate.cpu, duplicate.manager + 0x60) == [] and
            read_u32(duplicate.cpu, duplicate.manager + 0x50) == 0 and
            duplicate.nodes_used == 0,
            "resolved duplicate changed the Character list/count")

    # GameObject::IsCharacter is false at the actual vtable slot +0x24, so a
    # unique non-Character object is registered without a Character node.
    non_character = AddFixture(original, manifest, [0])
    non_character.resolver[11] = non_character.new_object
    non_character.add(non_character.new_object, 0x964750, 10)
    require(non_character.base_character_checks == 1 and
            read_node_ring(non_character.cpu, non_character.manager + 0x60) == [] and
            non_character.nodes_used == 0,
            "source IsCharacter gate appended a non-Character object")

    return {
        "unique_add_sequence": [hex(row[3]) for row in ring],
        "same_character_pointer_is_not_deduplicated": True,
        "duplicate_lookup": {"resolved_handle": 21, "vtable_slot4_calls": duplicate.noop_calls,
                             "list_count_after": 0},
        "non_character_gate": {"base_IsCharacter_calls": non_character.base_character_checks,
                               "list_count_after": 0},
        "explicit_services": ["GetObjectByName output", "ObjectHandle::GetObject resolver",
                               "ObjectBase::GetHandle", "ObjectManager map slot",
                               "list-node allocation", "ObjectBase name storage"],
    }


class RemoveFixture:
    def __init__(self, original: Path, manifest: dict, values: list[int],
                 requested_id: int, resolver: dict[int, int]):
        from unicorn import UC_HOOK_CODE
        self.cpu = make_cpu(original, manifest)
        self.uc = self.cpu.uc
        self.base = self.cpu.data
        self.manager = self.base + 0x1000
        self.target_list = self.manager + 0x60
        self.objects = [self.base + 0x3000 + i * 0x100 for i in range(8)]
        self.nodes = [self.base + 0x5000 + i * 0x20 for i in range(8)]
        self.resolver = dict(resolver)
        self.requested_id = requested_id
        self.frees: list[tuple[int, int]] = []
        self.stop_address = 0x348fec
        self.stopped = False

        for offset in (0x2c, 0x60, 0x70, 0x88, 0x90):
            self.cpu.pointer(self.manager + offset, self.manager + offset)
            self.cpu.pointer(self.manager + offset + 4, self.manager + offset)
        self.cpu.pointer(self.manager + 0x50, len(values))
        for obj in self.objects:
            self.cpu.pointer(obj, 0x965F38)
            self.uc.mem_write(obj + 0x2f4, bytes(8))
            self.cpu.pointer(obj + 0x3c8 + 0x34, 0)
        for i, object_index in enumerate(values):
            obj = self.objects[object_index]
            # CharAI::RemoveFromGroup sees a null group pointer at Character+0x3fc.
            node = self.nodes[i]
            previous = self.target_list if i == 0 else self.nodes[i - 1]
            next_node = self.target_list if i + 1 == len(values) else self.nodes[i + 1]
            self.cpu.pointer(node, next_node)
            self.cpu.pointer(node + 4, previous)
            self.cpu.pointer(node + 8, obj)
        if values:
            self.cpu.pointer(self.target_list, self.nodes[0])
            self.cpu.pointer(self.target_list + 4, self.nodes[len(values) - 1])
        self.uc.hook_add(UC_HOOK_CODE, self._hook)

    def _hook(self, uc, address, _size, _user) -> None:
        if address == 0x33fdc0:  # actual GameObject/Character conversion wrappers call this boundary.
            handle = self.cpu.reg(0)
            ident = read_u32(self.cpu, handle)
            self.cpu.put(0, self.resolver.get(ident, 0))
            uc.reg_write(self.cpu.pc, uc.reg_read(self.cpu.lr))
        elif address == 0x708f00:  # allocator release; list unlinking remains original ARM.
            self.frees.append((self.cpu.reg(0), self.cpu.reg(1)))
            uc.reg_write(self.cpu.pc, uc.reg_read(self.cpu.lr))
        elif address == self.stop_address:
            self.stopped = True
            uc.emu_stop()

    def remove_until_after_character_list(self) -> None:
        sp = self.cpu.stack + 0xe000
        self.cpu.uc.reg_write(self.cpu.sp, sp)
        self.cpu.uc.reg_write(self.cpu.lr, self.cpu.stop)
        for index, value in enumerate((self.manager, self.requested_id, 0, 0)):
            self.cpu.put(index, value)
        self.uc.emu_start(0x348ea4, self.cpu.stop, count=500000)
        require(self.stopped and self.uc.reg_read(self.cpu.pc) == self.stop_address,
                "original Remove did not reach the post-CharacterList cleanup boundary")


def remove_arm_checks(original: Path, manifest: dict) -> dict:
    # Four entries include two occurrences of the Character being removed.
    fixture = RemoveFixture(original, manifest, [0, 1, 0, 2], 1,
                            {1: 0x2003000, 2: 0x2003100, 3: 0x2003200})
    # Resolver addresses are fixture-specific and are filled from the owner map.
    fixture.resolver = {1: fixture.objects[0], 2: fixture.objects[1],
                        3: fixture.objects[2]}
    fixture.remove_until_after_character_list()
    ring = read_node_ring(fixture.cpu, fixture.target_list)
    require([row[3] for row in ring] == [fixture.objects[1], fixture.objects[2]],
            "source Remove did not unlink all matching Characters while preserving other order")
    expected_frees = [(fixture.nodes[0], 12), (fixture.nodes[2], 12)]
    require(fixture.frees == expected_frees,
            f"source Remove freed wrong nodes/order: {fixture.frees!r}")

    missing = RemoveFixture(original, manifest, [0, 1], 3, {})
    missing.resolver[3] = missing.objects[2]
    missing.remove_until_after_character_list()
    missing_ring = read_node_ring(missing.cpu, missing.target_list)
    require([row[3] for row in missing_ring] == [missing.objects[0], missing.objects[1]] and
            missing.frees == [], "source Remove changed a ring without the target Character")
    return {
        "remove_target_pointer": hex(fixture.objects[0]),
        "remaining_after_remove": [hex(row[3]) for row in ring],
        "freed_source_nodes": [{"address": hex(address), "bytes": size}
                               for address, size in fixture.frees],
        "missing_target_is_noop": True,
        "stop_boundary": "0x348fec, after list scan and CharAI::RemoveFromGroup, before other manager cleanup",
    }


def host_checks(cxx: str, output: Path) -> str:
    compiler = shutil.which(cxx)
    require(compiler is not None, f"C++ compiler not found: {cxx}")
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
               str(MODULE / "character_aggro_object_manager_list.cpp"),
               str(HERE / "character_aggro_object_manager_list.cpp"), "-o", str(output)]
    build = subprocess.run(command, cwd=REPO, text=True, capture_output=True)
    require(build.returncode == 0, "host compile failed:\n" + build.stdout + build.stderr)
    run = subprocess.run([str(output)], cwd=REPO, text=True, capture_output=True)
    require(run.returncode == 0, "host checks failed:\n" + run.stdout + run.stderr)
    return run.stdout.strip()


def run(original: Path, cxx: str, output: Path) -> dict:
    manifest, source = verify_source(original)
    host_output = host_checks(cxx, output)
    add = add_arm_checks(original, manifest)
    remove = remove_arm_checks(original, manifest)
    source_files = [MODULE / "character_aggro_object_manager_list.hpp",
                    MODULE / "character_aggro_object_manager_list.cpp",
                    HERE / "character_aggro_object_manager_list.cpp",
                    Path(__file__).resolve(), MANIFEST,
                    MANIFEST.with_name("NOTES.md")]
    return {
        "status": "passed",
        "original_sha256": EXPECTED_ELF,
        "host": host_output,
        "arm32_add": add,
        "arm32_remove": remove,
        "verified_caller_edges": source["call_edges"],
        "source_sha256": {path.relative_to(REPO).as_posix(): sha(path.read_bytes())
                          for path in source_files},
        "scope": "Original ObjectManager Add/Remove ARM instructions and dynamic type checks executed. Explicit fixture services supply name lookup, global handle resolution/construction, map storage, node allocation/free. The Remove run stops after the flat CharacterList unlink loop and CharAI group-removal call, before manager/map cleanup. Host owner/cursor checks verify non-overlay list semantics and live-link reads.",
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original-elf", type=Path,
                        default=REPO.parent / "test_strategy" / "libDungeonHunter2.so")
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build" / "character-aggro-object-manager-list" /
                        "character-aggro-object-manager-list-host.exe")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build" / "character-aggro-object-manager-list" /
                        "validation.json")
    args = parser.parse_args()
    result = run(args.original_elf.resolve(), args.compiler, args.output.resolve())
    report = args.report.resolve()
    report.parent.mkdir(parents=True, exist_ok=True)
    report.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print("ObjectManager flat Character-list host + original ARM checks passed")
    print("ARM Add: duplicate/Character gates and tail appends; Remove: remove-all and order retained")
    print(f"report: {report}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, RuntimeError, AssertionError) as exc:
        raise SystemExit(f"ERROR: {exc}") from exc
