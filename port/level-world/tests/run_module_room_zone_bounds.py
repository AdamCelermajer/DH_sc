#!/usr/bin/env python3
"""Host/ARM differential for Module::InitPost's RoomZone box producer."""
from __future__ import annotations

import argparse
import hashlib
import json
import math
from pathlib import Path
import random
import struct
import subprocess
import sys

from unicorn import UC_HOOK_CODE

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / "port/level-world"
EXPECTED_ELF_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
SYMBOLS = {
    "init_with_bounding_box": (
        "_ZN4Zone19InitWithBoundingBoxERKN6glitch4core8aabbox3dIfEE",
        0x397594, 392),
    "room_zone_ctor": ("_ZN8RoomZoneC1EN10ObjectBase6GO_IDSE", 0x396558, 108),
    "zone_ctor": ("_ZN4ZoneC2EN10ObjectBase6GO_IDSEbb", 0x397CA0, 116),
    "module_init_post": ("_ZN6Module8InitPostEv", 0x388B20, 312),
    "root_bounds_getter": (
        "_ZNK6glitch5scene15CEmptySceneNode14getBoundingBoxEv", 0x5839B0, 8),
    "root_transformed_bounds_getter": (
        "_ZNK6glitch5scene10ISceneNode25getTransformedBoundingBoxEv", 0x59770C, 124),
    "root_refresh_bounds": (
        "_ZN13RootSceneNode18RefreshBoundingBoxEv", 0x35C854, 156),
    "root_refresh_adjustment_getter": (
        "_ZNK6glitch5scene10ISceneNode11getPositionEv", 0x597124, 8),
    "root_constructor": (
        "_ZN13RootSceneNodeC1ERKN6glitch7collada16CColladaDatabaseE", 0x35D824, 236),
    "set_position": ("_ZN10GameObject11SetPositionERK7Point3DIfEb", 0x393DB4, 220),
}
SET_POSITION = 0x393DB4
ZONE = 0x02100000
BOX = 0x02200000


def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def source_bytes(elf, raw: bytes, address: int, size: int) -> bytes:
    for segment in elf.iter_segments():
        if segment["p_type"] != "PT_LOAD":
            continue
        start, filesz, memsz = (int(segment[k]) for k in ("p_vaddr", "p_filesz", "p_memsz"))
        if start <= address and address + size <= start + memsz:
            available = max(0, min(size, start + filesz - address))
            if available:
                offset = int(segment["p_offset"]) + address - start
                return raw[offset:offset + available] + b"\0" * (size - available)
            return b"\0" * size
    raise ValueError(f"original ELF bytes unavailable at {address:#x}/{size:#x}")


def f32(bits: int) -> float:
    return struct.unpack("<f", struct.pack("<I", bits))[0]


def fbits(value: float) -> int:
    try:
        return struct.unpack("<I", struct.pack("<f", value))[0]
    except OverflowError:
        return 0xFF800000 if value < 0 else 0x7F800000


class ZoneCpu:
    """Execute the exact ELF routine, intercepting only SetPosition's call."""
    def __init__(self, elf_path: Path):
        from cpu import Cpu

        class FloatCpu(Cpu):
            def __init__(self, *args, **kwargs):
                self.float_calls: list[str] = []
                super().__init__(*args, **kwargs)

            def external(self, uc, address, size, unused):
                name = self.imports.get(address)
                if name in ("__aeabi_fsub", "__aeabi_fadd", "__aeabi_fmul"):
                    self.import_calls[name] = self.import_calls.get(name, 0) + 1
                    self.float_calls.append(name)
                    a, b = f32(self.reg(0)), f32(self.reg(1))
                    value = a - b if name.endswith("fsub") else a + b if name.endswith("fadd") else a * b
                    self.put(0, fbits(value))
                    uc.reg_write(self.pc, uc.reg_read(self.lr))
                    return
                return super().external(uc, address, size, unused)

        self.cpu = FloatCpu(elf_path, False, {"functions": [
            {"original_symbol": SYMBOLS["init_with_bounding_box"][0],
             "elf_address": hex(SYMBOLS["init_with_bounding_box"][1]),
             "size": SYMBOLS["init_with_bounding_box"][2]}]})
        self.cpu.uc.hook_add(UC_HOOK_CODE, self.hook, begin=SET_POSITION, end=SET_POSITION)
        self.position_calls: list[dict] = []
        self.required_imports = {"__aeabi_fsub", "__aeabi_fadd", "__aeabi_fmul"}
        if not self.required_imports.issubset(set(self.cpu.imports.values())):
            raise AssertionError("original arithmetic import identity is incomplete")

    def hook(self, uc, address, size, unused):
        c = self.cpu
        if address != SET_POSITION:
            return
        obj, vector, update = c.reg(0), c.reg(1), c.reg(2)
        raw = bytes(uc.mem_read(vector, 12))
        self.position_calls.append({"address": address, "object": obj,
                                    "update": update,
                                    "center_bits": list(struct.unpack("<3I", raw))})
        uc.reg_write(c.pc, uc.reg_read(c.lr))

    @staticmethod
    def read_words(uc, address, count):
        return list(struct.unpack("<" + "I" * count, bytes(uc.mem_read(address, count * 4))))

    def run(self, box_words: list[int]) -> dict:
        c = self.cpu
        uc = c.uc
        uc.mem_write(ZONE, b"\0" * 0x500)
        uc.mem_write(BOX, struct.pack("<6I", *box_words))
        # Nonzero sentinels show that the original writes every expected field.
        for axis in range(3):
            c.pointer(ZONE + 0x374 + axis * 4, 0x4F123456 + axis)
            c.pointer(ZONE + 0x144 + axis * 4, 0x4F223456 + axis)
            c.pointer(ZONE + 0x150 + axis * 4, 0x4F323456 + axis)
        c.uc.mem_write(ZONE + 0x380, b"\0")
        self.position_calls.clear()
        c.float_calls.clear()
        c.invoke(SYMBOLS["init_with_bounding_box"][0], [ZONE, BOX])
        if len(self.position_calls) != 1:
            raise AssertionError(f"SetPosition calls: {self.position_calls}")
        return {
            "dimensions": self.read_words(uc, ZONE + 0x374, 3),
            "relative_minimum": self.read_words(uc, ZONE + 0x144, 3),
            "relative_maximum": self.read_words(uc, ZONE + 0x150, 3),
            "center": self.position_calls[0]["center_bits"],
            "set_position": self.position_calls[0],
            "float_calls": list(c.float_calls),
        }


def input_cases() -> list[tuple[str, list[int]]]:
    cases = [
        ("zero_box", [0, 0, 0, 0, 0, 0]),
        ("asymmetric_positive", [0x3F800000, 0x40000000, 0x40400000,
                                  0x40A00000, 0x40C00000, 0x40E00000]),
        ("mixed_negative", [0xC1200000, 0xC0A00000, 0xBF800000,
                             0x3F400000, 0x40000000, 0x40800000]),
        ("reversed_input_is_not_normalized", [0x40800000, 0x40000000, 0x3F800000,
                                               0x3F800000, 0x3F000000, 0x00000000]),
        ("negative_zero_edges", [0x80000000, 0x80000000, 0x80000000,
                                  0x00000000, 0x00000000, 0x00000000]),
        ("large_cancellation", [0x7EFFFFFF, 0x7EFFFFFE, 0xFEFFFFFF,
                                0xFEFFFFFF, 0x7EFFFFFF, 0x7EFFFFFE]),
        ("infinite_endpoints", [0xFF800000, 0xFF800000, 0xFF800000,
                                0x7F800000, 0x7F800000, 0x7F800000]),
        ("nan_endpoints", [0x7FC00001, 0xFFC12345, 0x3F800000,
                            0x3F800000, 0x7FC00002, 0xBF800000]),
        ("subnormal_edges", [0x00000001, 0x80000001, 0x007FFFFF,
                              0x00800000, 0x00000001, 0x807FFFFF]),
    ]
    rng = random.Random(0x397594)
    for i in range(96):
        cases.append((f"seeded_bits_{i:03d}", [rng.getrandbits(32) for _ in range(6)]))
    return cases


def equal_words(left: list[int], right: list[int]) -> bool:
    for a, b in zip(left, right):
        af, bf = f32(a), f32(b)
        if math.isnan(af) or math.isnan(bf):
            if not (math.isnan(af) and math.isnan(bf)):
                return False
        elif a != b:
            return False
    return len(left) == len(right)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", required=True)
    parser.add_argument("--original-elf", type=Path,
                        default=ROOT.parent / "test_strategy/libDungeonHunter2.so")
    parser.add_argument("--output", type=Path,
                        default=MODULE / "build/module-room-zone-bounds-host")
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    src = [MODULE / "module_room_zone_bounds.hpp",
           MODULE / "module_room_zone_bounds.cpp",
           MODULE / "tests/module_room_zone_bounds.cpp",
           Path(__file__).resolve()]
    exe = output / "module-room-zone-bounds-host.exe"
    subprocess.run([args.compiler, "-std=c++17", "-O1", "-fno-fast-math",
                    "-ffp-contract=off", "-Wall", "-Wextra", "-Werror", "-pedantic",
                    str(src[1]), str(src[2]), "-o", str(exe)], check=True)

    raw = args.original_elf.read_bytes()
    digest = sha(raw)
    if digest != EXPECTED_ELF_SHA:
        raise ValueError(f"original ELF SHA mismatch: {digest}")
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from elftools.elf.elffile import ELFFile
    with args.original_elf.open("rb") as stream:
        elf = ELFFile(stream)
        symtab = elf.get_section_by_name(".symtab")
        symbols = {s.name: s for s in symtab.iter_symbols()}
        function_evidence = {}
        for label, (name, address, size) in SYMBOLS.items():
            symbol = symbols.get(name)
            if symbol is None or (int(symbol["st_value"]), int(symbol["st_size"])) != (address, size):
                raise ValueError(f"symbol identity mismatch for {label}: {name}")
            function_evidence[label] = {
                "original_symbol": name, "address": hex(address), "size": size,
                "sha256": sha(source_bytes(elf, raw, address, size)),
            }
        root_vtable = symbols.get("_ZTV13RootSceneNode")
        if root_vtable is None or int(root_vtable["st_value"]) != 0x95FDD8:
            raise ValueError("RootSceneNode vtable identity changed")
        table_base = int(root_vtable["st_value"])
        # RootSceneNode C1 derives its AP from a relocation-backed GOT entry,
        # then emits `add r3,r3,#0x1c; str r3,[r4]` into the object vptr.
        # Recompute that exact path from its ARM PC-relative literal/GOT slot.
        got_base_literal_address = 0x35D900
        got_base_literal = struct.unpack("<I", source_bytes(elf, raw, got_base_literal_address, 4))[0]
        got_base = 0x35D83C + got_base_literal  # PC value for ADD at 0x35d834.
        vtable_slot_literal_address = 0x35D90C
        vtable_slot_offset = struct.unpack("<I", source_bytes(elf, raw, vtable_slot_literal_address, 4))[0]
        vtable_got_slot = got_base + vtable_slot_offset
        relocation_records = []
        for section in elf.iter_sections():
            if section["sh_type"] not in ("SHT_REL", "SHT_RELA"):
                continue
            symsec = elf.get_section(section["sh_link"])
            for relocation in section.iter_relocations():
                if int(relocation["r_offset"]) == vtable_got_slot:
                    relocation_records.append({
                        "section": section.name,
                        "type_id": int(relocation["r_info_type"]),
                        "symbol": symsec.get_symbol(relocation["r_info_sym"]).name,
                    })
        if relocation_records != [{"section": ".rel.dyn", "type_id": 23, "symbol": ""}]:
            raise ValueError(f"RootSceneNode constructor's vtable GOT relocation changed: {relocation_records}")
        vtable_got_target = struct.unpack("<I", source_bytes(elf, raw, vtable_got_slot, 4))[0]
        if vtable_got_target != table_base:
            raise ValueError(f"RootSceneNode constructor's vtable base changed: {vtable_got_target:#x}")
        ctor_tail = source_bytes(elf, raw, 0x35D890, 8)
        if ctor_tail != bytes.fromhex("1c3083e2003084e5"):
            raise ValueError(f"RootSceneNode constructor vptr store changed: {ctor_tail.hex()}")
        vptr = table_base + 0x1C

        def vtable_target(offset: int) -> tuple[int, int]:
            address = vptr + offset
            target = struct.unpack("<I", source_bytes(elf, raw, address, 4))[0]
            rels = []
            for section in elf.iter_sections():
                if section["sh_type"] not in ("SHT_REL", "SHT_RELA"):
                    continue
                for relocation in section.iter_relocations():
                    if int(relocation["r_offset"]) == address:
                        rels.append((section.name, int(relocation["r_info_type"])))
            if rels != [(".rel.dyn", 23)]:
                raise ValueError(f"RootSceneNode vtable relocation changed at {address:#x}: {rels}")
            return address, target

        local_box_slot_address, local_box_slot_target = vtable_target(0x30)
        transformed_slot_address, transformed_slot_target = vtable_target(0x34)
        if local_box_slot_target != SYMBOLS["root_bounds_getter"][1]:
            raise ValueError(f"RootSceneNode vptr+0x30 target changed: {local_box_slot_target:#x}")
        if transformed_slot_target != SYMBOLS["root_transformed_bounds_getter"][1]:
            raise ValueError(f"RootSceneNode vptr+0x34 target changed: {transformed_slot_target:#x}")
        adjustment_slot_address, adjustment_slot_target = vtable_target(0xA0)
        adjustment_slot_target = struct.unpack(
            "<I", source_bytes(elf, raw, adjustment_slot_address, 4))[0]
        if adjustment_slot_target != SYMBOLS["root_refresh_adjustment_getter"][1]:
            raise ValueError(
                f"RootSceneNode vptr+0xa0 target changed: {adjustment_slot_target:#x}")
        root_header = source_bytes(elf, raw, vptr - 8, 8)
        if root_header != b"\0" * 8:
            raise ValueError(f"RootSceneNode address-point header changed: {root_header.hex()}")
        module_get_bounds_call = source_bytes(elf, raw, 0x388B6C, 4)
        module_init_zone_call = source_bytes(elf, raw, 0x388C30, 4)
        room_zone_ctor_flags = source_bytes(elf, raw, 0x39655C, 8)
        zone_ctor_visual_store = source_bytes(elf, raw, 0x397CCC, 4)
        if module_get_bounds_call != bytes.fromhex("34f093e5"):
            raise ValueError(f"Module::InitPost virtual bounds call changed: {module_get_bounds_call.hex()}")
        if module_init_zone_call != bytes.fromhex("573a00eb"):
            raise ValueError(f"Module::InitPost Zone call changed: {module_init_zone_call.hex()}")
        if room_zone_ctor_flags != bytes.fromhex("0020a0e30130a0e3"):
            raise ValueError(f"RoomZone constructor flag arguments changed: {room_zone_ctor_flags.hex()}")
        if zone_ctor_visual_store != bytes.fromhex("8053c6e5"):
            raise ValueError(f"Zone +0x380 flag store changed: {zone_ctor_visual_store.hex()}")
        function_evidence["root_vtable_bounds_slot"] = {
            "vtable_symbol": "_ZTV13RootSceneNode", "vtable_address": "0x95fdd8",
            "constructor_symbol": SYMBOLS["root_constructor"][0],
            "constructor_address": "0x35d824",
            "constructor_vptr_instructions": "add r3,r3,#0x1c; str r3,[r4]",
            "vptr_got_slot": hex(vtable_got_slot),
            "vptr_got_relocation": relocation_records[0],
            "itanium_address_point": hex(vptr),
            "address_point_header_words": [0, 0],
            "local_bounds_slot_offset": "0x30",
            "local_bounds_slot_address": hex(local_box_slot_address),
            "local_bounds_target": hex(local_box_slot_target),
            "transformed_bounds_slot_offset": "0x34",
            "transformed_bounds_slot_address": hex(transformed_slot_address),
            "transformed_bounds_target": hex(transformed_slot_target),
        }
        function_evidence["root_vtable_refresh_adjustment_slot"] = {
            "vtable_symbol": "_ZTV13RootSceneNode", "vtable_address": "0x95fdd8",
            "itanium_address_point": hex(vptr), "slot_offset": "0xa0",
            "slot_address": hex(adjustment_slot_address),
            "target": hex(adjustment_slot_target),
            "target_symbol": SYMBOLS["root_refresh_adjustment_getter"][0],
            "semantic_boundary": "ISceneNode::getPosition returns the RootSceneNode position at +0xac; RefreshBoundingBox subtracts it from all six root-local bounds",
        }
        function_evidence["module_room_zone_callsite"] = {
            "module_init_post_symbol": SYMBOLS["module_init_post"][0],
            "virtual_call_address": "0x388b6c",
            "virtual_call_bytes": module_get_bounds_call.hex(),
            "root_vptr_slot": "0x34",
            "resolved_target": hex(transformed_slot_target),
            "zone_init_address": "0x388c30",
            "zone_init_call_bytes": module_init_zone_call.hex(),
            "room_zone_constructor_bool_args": [0, 1],
            "zone_visual_flag_store_address": "0x397ccc",
            "zone_visual_flag_store_bytes": zone_ctor_visual_store.hex(),
            "zone_visual_flag_result": 0,
        }

    original = ZoneCpu(args.original_elf)
    rows = []
    ordered_calls = ["__aeabi_fsub"] * 3 + ["__aeabi_fadd", "__aeabi_fmul"] * 3
    for name, words in input_cases():
        host_args = [f"{word:08x}" for word in words] + ["0", "0"]
        host = json.loads(subprocess.run([str(exe), *host_args], capture_output=True,
                                         text=True, check=True).stdout)
        arm = original.run(words)
        expected = {key: arm[key] for key in ("dimensions", "relative_minimum",
                                               "relative_maximum", "center")}
        observed = {key: host[key] for key in expected}
        mismatches = [key for key in expected if not equal_words(expected[key], observed[key])]
        if mismatches:
            raise AssertionError({"case": name, "mismatches": mismatches,
                                  "host": observed, "original_arm": expected})
        if host["status"] != 0 or host["calls"] != 1 or host["request_update"] != 1:
            raise AssertionError({"case": name, "host_request": host})
        if arm["set_position"]["address"] != SET_POSITION or arm["set_position"]["update"] != 1:
            raise AssertionError({"case": name, "source_set_position": arm["set_position"]})
        if arm["float_calls"] != ordered_calls:
            raise AssertionError({"case": name, "arithmetic_call_order": arm["float_calls"]})
        rows.append({"name": name, "input_bits": [f"0x{x:08x}" for x in words],
                     "matches": True, "set_position_center_bits": [f"0x{x:08x}" for x in arm["center"]]})

    # Exercise the diagnostic guard and callback fail-stop path independently;
    # the ELF caller's ordinary RoomZone always has +0x380 == 0.
    sample = input_cases()[1][1]
    guard = json.loads(subprocess.run([str(exe), *(f"{x:08x}" for x in sample), "1", "0"],
                                      capture_output=True, text=True, check=True).stdout)
    if guard["status"] != 4 or guard["calls"] != 0:
        raise AssertionError({"optional_visual_guard": guard})
    failure = json.loads(subprocess.run([str(exe), *(f"{x:08x}" for x in sample), "0", "1"],
                                        capture_output=True, text=True, check=True).stdout)
    if failure["status"] != 3 or failure["calls"] != 1 or failure["request_update"] != 1:
        raise AssertionError({"set_position_failure": failure})

    report = {
        "validation": "PASS", "original_arm_cases": len(rows), "mismatches": 0,
        "original_sha256": digest, "functions": function_evidence,
        "arithmetic_import_identity_and_order": ordered_calls,
        "source_sha256": {str(p.relative_to(ROOT)).replace("\\", "/"): sha(p.read_bytes())
                          for p in src},
        "scope": "Module::InitPost's RootSceneNode bounding-box argument through the RoomZone-specific Zone::InitWithBoundingBox path: exact dimensions/min/max writes and the exact SetPosition(center,true) call. SetPosition is intercepted by address and ABI, not executed; physical/visual adapters, RootSceneNode bounds production/world transform, and Module-to-runtime-room identity remain external providers.",
        "room_zone_constructor_gate": "RoomZone ctor passes Zone(flag_380=false, flag_381=true); optional visual branch excluded.",
        "optional_visual_guard": guard,
        "set_position_failure_case": failure,
        "cases": rows,
    }
    (output / "validation.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({k: report[k] for k in ("validation", "original_arm_cases", "mismatches")}))


if __name__ == "__main__":
    main()
