#!/usr/bin/env python3
"""Execute the original ARM32 TargetList point-query body on bounded fixtures."""
from __future__ import annotations

import argparse
import hashlib
import json
import math
from pathlib import Path
import shutil
import subprocess
import struct
import sys

from unicorn import UC_HOOK_CODE

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPO = HERE.parents[2]
sys.path.insert(0, str(HERE))
from navigation_differential import Cpu as BaseCpu  # noqa: E402
from elf_import_identity import verify_imports  # noqa: E402

MANIFEST = MODULE / "reference" / "character-aggro-point-query-original" / "original-functions.json"
ELF_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"


def bits(value: float) -> int:
    return struct.unpack("<I", struct.pack("<f", value))[0]


def floating(value: int) -> float:
    return struct.unpack("<f", struct.pack("<I", value & 0xFFFFFFFF))[0]


def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def check(value: bool, message: str) -> None:
    if not value:
        raise AssertionError(message)


class OriginalCpu(BaseCpu):
    """Original ELF execution with only imports and actor-owned calls supplied."""

    def __init__(self, path: Path, manifest: dict):
        super().__init__(path, False, manifest)
        self.alloc = self.data + 0x100000
        self.query = None
        self.trace: list[list[object]] = []
        self.list_fixture = None
        self.hook_addresses: dict[int, str] = {}
        self.executed_source_addresses: set[int] = set()

    def returned(self, value: int = 0) -> None:
        self.put(0, value)
        self.uc.reg_write(self.pc, self.uc.reg_read(self.lr))

    def allocate(self, size: int) -> int:
        if size < 0 or size > 0x100000:
            raise AssertionError(f"unsafe original allocation size {size}")
        ptr = (self.alloc + 15) & ~15
        self.alloc = ptr + max(size, 1)
        self.uc.mem_write(ptr, bytes(size))
        return ptr

    def binary32(self, op: str, left: int, right: int) -> int:
        a, b = floating(left), floating(right)
        if op == "__aeabi_fadd": value = a + b
        elif op == "__aeabi_fsub": value = a - b
        elif op == "__aeabi_fmul": value = a * b
        elif op == "__aeabi_fdiv":
            if b == 0.0:
                value = math.nan if a == 0.0 or math.isnan(a) else math.copysign(math.inf, a * math.copysign(1.0, b))
            else: value = a / b
        else: raise AssertionError(op)
        try:
            return bits(value)
        except OverflowError:
            return bits(math.copysign(math.inf, value))

    def external(self, uc, address, size, unused):
        name = self.imports.get(address)
        if name in ("__aeabi_fadd", "__aeabi_fsub", "__aeabi_fmul", "__aeabi_fdiv"):
            self.put(0, self.binary32(name, self.reg(0), self.reg(1)))
        elif name == "sqrtf":
            x = floating(self.reg(0))
            self.put(0, bits(math.sqrt(x) if x >= 0.0 else math.nan))
        elif name == "acosf":
            x = floating(self.reg(0))
            self.put(0, bits(math.acos(x) if -1.0 <= x <= 1.0 else math.nan))
        elif name in ("__aeabi_fcmplt", "__aeabi_fcmple", "__aeabi_fcmpgt", "__aeabi_fcmpge"):
            a, b = floating(self.reg(0)), floating(self.reg(1))
            result = {
                "__aeabi_fcmplt": a < b,
                "__aeabi_fcmple": a <= b,
                "__aeabi_fcmpgt": a > b,
                "__aeabi_fcmpge": a >= b,
            }[name]
            self.put(0, int(result))
        else:
            return super().external(uc, address, size, unused)
        self.import_calls[name] = self.import_calls.get(name, 0) + 1
        uc.reg_write(self.pc, uc.reg_read(self.lr))

    def hook(self, uc, address, size, unused):
        # TargetList's debug switches are diagnostics-only. The query body,
        # source gates, vector math and heap insertion remain original ARM.
        if address == 0x337888:  # DebugSwitches::Load
            self.trace.append(["debug_load_bypassed"])
            self.returned()
        elif address == 0x3140ec:  # debug key std::string construction
            self.returned(self.reg(0))
        elif address == 0x337a88:  # no active switch: make source lookup miss
            self.uc.mem_write(self.uc.reg_read(self.sp) + 0x60,
                              struct.pack("<I", self.reg(1)))
            self.trace.append(["debug_switch_miss"])
            self.returned()
        elif address in (0x708ec0, 0x310454):
            size = (struct.unpack("<I", self.uc.mem_read(self.reg(0), 4))[0]
                    if address == 0x708ec0 else self.reg(0))
            self.returned(self.allocate(size))
        elif address in (0x708f00, 0x310440):
            self.returned()
        elif address == 0x3d4c34:  # actor-owned CharAI::AI_GetMeleeRadius
            check(self.query is not None, "melee radius called outside query")
            owner = next(x for x in self.query["objects"] if x.get("owner"))
            self.trace.append(["ai_melee_radius", owner["index"]])
            self.returned(bits(self.query["melee_radius"]))
        elif address in self.hook_addresses:
            self._virtual(self.hook_addresses[address])

    def _virtual(self, operation: str) -> None:
        check(self.query is not None, "virtual called outside query")
        case = self.query
        object_pointer = self.reg(0)
        item = next((x for x in case["objects"] if x["address"] == object_pointer), None)
        check(item is not None, f"{operation} received unknown object {object_pointer:#x}; query objects=" +
              ",".join(hex(x.get("address", 0)) for x in case["objects"]) +
              f"; callback_pc={self.uc.reg_read(self.pc):#x} lr={self.uc.reg_read(self.lr):#x}")
        if operation == "is_character":
            self.trace.append([operation, item["index"]])
            self.returned(int(item.get("character", True)))
        elif operation == "is_zonable":
            self.trace.append([operation, item["index"]])
            self.returned(int(item.get("zonable", False)))
        elif operation == "is_interactive":
            check(self.reg(1) == case["owner"], "IsInteractive owner argument")
            self.trace.append([operation, item["index"]])
            if case.get("mutate_after") == item["index"]:
                node = case["nodes"][item["index"]]
                self.uc.mem_write(node, struct.pack("<I", case["sentinel"]))
                self.trace.append(["mutate_next_to_end", item["index"]])
            self.returned(int(item.get("interactive", True)))
        elif operation == "interaction_radius":
            self.trace.append([operation, item["index"]])
            self.returned(bits(item.get("interaction_radius", 0.0)))
        else:
            raise AssertionError(operation)


def build_fixture(cpu: OriginalCpu, case: dict, index: int) -> dict:
    """Write source-layout objects and an IObjectList vtable into fixture RAM."""
    cpu.alloc = cpu.data + 0x100000 + index * 0x20000
    owner = cpu.data + 0x10000
    objects_base = cpu.data + 0x20000
    nodes_base = cpu.data + 0x50000
    query_data = cpu.data + 0x80000
    list_object = cpu.data + 0x90000
    list_vtable = cpu.data + 0x91000
    callbacks = {name: cpu.data + 0x92000 + i * 0x20 for i, name in enumerate((
        "list_reset", "list_at_end", "list_next", "list_get", "list_get_char",
        "is_character", "is_interactive", "interaction_radius", "is_zonable"))}
    for address in callbacks.values():
        cpu.uc.mem_write(address, bytes.fromhex("1eff2fe1"))  # bx lr; calls are handled by hooks
    cpu.hook_addresses = {callbacks[name]: name for name in (
        "is_character", "is_interactive", "interaction_radius", "is_zonable")}
    cpu.uc.mem_write(list_vtable, bytes(0x20))
    for offset, name in ((0x08, "list_reset"), (0x0c, "list_at_end"),
                         (0x10, "list_next"), (0x18, "list_get"),
                         (0x1c, "list_get_char")):
        cpu.pointer(list_vtable + offset, callbacks[name])

    all_objects = []
    owner_vtable = cpu.data + 0x93000
    cpu.uc.mem_write(owner_vtable, bytes(0x100))
    cpu.pointer(owner_vtable + 0x24, callbacks["is_character"])
    cpu.uc.mem_write(owner, bytes(0x1800))
    cpu.pointer(owner, owner_vtable)
    cpu.uc.mem_write(owner + 0x1314, struct.pack("<I", case.get("owner_word1314", 100)))
    cpu.uc.mem_write(owner + 0x1310, struct.pack("<I", case.get("owner_word1310", 0)))

    listed_sources = case["objects"]
    has_owner = any(source.get("owner") for source in listed_sources)
    if not has_owner:
        implicit_owner = {"owner": True, "character": True, "index": -1,
                          "listed": False, "visible": 1, "word1310": 0,
                          "word1314": 0, "zonable": 0, "interactive": 1,
                          "interaction_radius": 0.0, "position": [0, 0, 0],
                          "has_target_position": 0, "target_position": [0, 0, 0]}
        case["all_objects"] = [implicit_owner]
    else:
        case["all_objects"] = []
    for i, source in enumerate(listed_sources):
        ptr = owner if source.get("owner") else objects_base + i * 0x2000
        if ptr != owner:
            cpu.uc.mem_write(ptr, bytes(0x1800))
            vtable = cpu.data + 0x94000 + i * 0x100
            cpu.uc.mem_write(vtable, bytes(0x100))
            cpu.pointer(vtable + 0x24, callbacks["is_character"])
            cpu.pointer(vtable + 0x88, callbacks["is_interactive"])
            cpu.pointer(vtable + 0x94, callbacks["interaction_radius"])
            cpu.pointer(vtable + 0xc4, callbacks["is_zonable"])
            cpu.pointer(ptr, vtable)
        cpu.uc.mem_write(ptr + 0x8a, bytes([int(source.get("visible", 1))]))
        cpu.uc.mem_write(ptr + 0x2ee, bytes([int(source.get("zoned", 0))]))
        cpu.uc.mem_write(ptr + 0x2f0, bytes([int(source.get("in_zone", 0))]))
        owner_word1310 = case.get("owner_word1310", 0) if source.get("owner") else 0
        owner_word1314 = case.get("owner_word1314", 0) if source.get("owner") else 0
        cpu.uc.mem_write(ptr + 0x1310, struct.pack("<I", source.get("word1310", owner_word1310)))
        cpu.uc.mem_write(ptr + 0x1314, struct.pack("<I", source.get("word1314", owner_word1314)))
        cpu.uc.mem_write(ptr + 0x160, struct.pack("<3f", *source.get("position", [0.0, 0.0, 0.0])))
        cpu.pointer(ptr + 0x180, ptr + 0x184)
        cpu.uc.mem_write(ptr + 0x80, bytes([int(source.get("has_target_position", 0))]))
        cpu.uc.mem_write(ptr + 0x184, struct.pack("<3f", *source.get("target_position", source.get("position", [0, 0, 0]))))
        source["index"] = i
        source["address"] = ptr
        source["listed"] = True
        all_objects.append(source)
        case["all_objects"].append(source)
    if not has_owner:
        cpu.uc.mem_write(owner, bytes(0x1800))
        cpu.pointer(owner, owner_vtable)
        cpu.uc.mem_write(owner + 0x1314, struct.pack("<I", implicit_owner["word1314"]))
        cpu.uc.mem_write(owner + 0x1310, struct.pack("<I", implicit_owner["word1310"]))
        implicit_owner["address"] = owner
        all_objects.insert(0, implicit_owner)

    # IObjectList is a borrowed circular intrusive view, not an entry copy.
    sentinel = nodes_base
    listed_objects = [source for source in all_objects if source.get("listed", True)]
    nodes = [nodes_base + 0x100 + i * 16 for i in range(len(listed_objects))]
    cpu.pointer(list_object, list_vtable)
    cpu.pointer(list_object + 4, sentinel)  # current cursor; reset uses this as before-begin
    cpu.pointer(sentinel, nodes[0] if nodes else sentinel)
    cpu.pointer(sentinel + 4, nodes[-1] if nodes else sentinel)
    cpu.pointer(sentinel + 8, 0)
    for i, (node, source) in enumerate(zip(nodes, listed_objects)):
        cpu.pointer(node, nodes[i + 1] if i + 1 < len(nodes) else sentinel)
        cpu.pointer(node + 4, source["address"] if source.get("character", 1) else 0)
        cpu.pointer(node + 8, source["address"])

    query = {
        "owner": owner,
        "objects": case["all_objects"],
        "nodes": nodes,
        "sentinel": sentinel,
        "list_object": list_object,
        "list_vtable": list_vtable,
        "callbacks": callbacks,
        "melee_radius": case.get("melee_radius", 0.0),
        "mutate_after": case.get("mutate_after"),
        "case": case,
        "allocation_base": cpu.alloc,
    }
    cpu.query = query
    cpu.list_fixture = query
    cpu.uc.mem_write(query_data, struct.pack("<3f", *case.get("center", [0.0, 0.0, 0.0])))
    cpu.uc.mem_write(query_data + 0x20, struct.pack("<3f", *case.get("forward", [0.0, 1.0, 0.0])))

    # Actual TargetList(owner, source flags, Character filter, closest sort).
    target_list = cpu.data + 0xA0000
    cpu.invoke(0x4a2730, [target_list, owner, 0x7fffffff, 2, 1], budget=1000000)
    check(struct.unpack("<I", cpu.uc.mem_read(target_list + 0x34, 4))[0] == 0x7fffffff,
          "source target flags")
    check(struct.unpack("<I", cpu.uc.mem_read(target_list + 0x38, 4))[0] == 2,
          "source Character object filter")
    check(struct.unpack("<I", cpu.uc.mem_read(target_list + 0x30, 4))[0] == owner,
          "source constructor recognizes Character owner")
    query.update({"target_list": target_list, "center": query_data,
                  "forward": query_data + 0x20,
                  "radius": bits(case.get("radius", 100.0)),
                  "cone": bits(case.get("cone", 2.0 * math.pi))})
    return query


def run_query(cpu: OriginalCpu, query: dict) -> dict:
    cpu.trace = []
    cpu.invoke(0x4a2f34, [query["target_list"], query["center"], query["radius"],
                          query["forward"], query["cone"], query["list_object"]],
               budget=2_000_000)
    # Pop in source priority order; `38fb18` is the actual priority_queue pop.
    rows = []
    target_list = query["target_list"]
    while struct.unpack("<I", cpu.uc.mem_read(target_list + 0x10, 4))[0] != \
          struct.unpack("<I", cpu.uc.mem_read(target_list, 4))[0]:
        first = struct.unpack("<I", cpu.uc.mem_read(target_list, 4))[0]
        words = struct.unpack("<5I", cpu.uc.mem_read(first, 20))
        rows.append({"object": words[0], "distance_bits": words[1],
                     "angle_bits": words[2], "flags": words[3], "reserved": words[4]})
        cpu.invoke(0x38fb18, [target_list], budget=200000)
    return {"results": rows, "trace": cpu.trace.copy(),
            "allocation_bytes": cpu.alloc - query["allocation_base"]}


def build_host_input(cases: list[dict], queries: list[dict], path: Path) -> dict:
    """Serialize exactly the same source fixtures for the typed C++ facade."""
    output = bytearray(b"CPQ1")
    output += struct.pack("<I", len(cases))
    expected_trace: dict[str, list[list[int]]] = {}
    for case, query in zip(cases, queries):
        name = case["name"].encode("utf-8")
        actor_specs = query["objects"]
        owner_index = next(i for i, actor in enumerate(actor_specs) if actor.get("owner"))
        listed = [i for i, actor in enumerate(actor_specs) if actor.get("listed", True)]
        mutate_actor = next((i for i, actor in enumerate(actor_specs)
                             if actor.get("index") == case.get("mutate_after")), None)
        mutate_index = 0xFFFFFFFF if mutate_actor is None else mutate_actor
        center = case.get("center", [0.0, 0.0, 0.0])
        forward = case.get("forward", [0.0, 1.0, 0.0])
        output += struct.pack(
            "<5I9f", len(name), len(actor_specs), len(listed), owner_index,
            mutate_index, *center, *forward, case.get("radius", 100.0),
            case.get("cone", 2.0 * math.pi), case.get("melee_radius", 0.0))
        output += name
        for actor in actor_specs:
            flags = 0
            if actor.get("character", True): flags |= 1 << 0
            if actor.get("visible", 1): flags |= 1 << 1
            if actor.get("zonable", 0): flags |= 1 << 2
            if actor.get("zoned", 0): flags |= 1 << 3
            if actor.get("in_zone", 0): flags |= 1 << 4
            if actor.get("interactive", 1): flags |= 1 << 5
            if actor.get("has_target_position", 0): flags |= 1 << 6
            is_owner = actor.get("owner", False)
            word1310 = actor.get("word1310", case.get("owner_word1310", 0)
                                 if is_owner else 0)
            word1314 = actor.get("word1314", case.get("owner_word1314", 0)
                                 if is_owner else 0)
            position = actor.get("position", [0.0, 0.0, 0.0])
            target_position = actor.get("target_position", position)
            output += struct.pack(
                "<iIIIf6f", actor.get("index", -1), flags, word1310 & 0xFFFFFFFF,
                word1314 & 0xFFFFFFFF, actor.get("interaction_radius", 0.0),
                *position, *target_position)
        for actor_index in listed:
            output += struct.pack("<I", actor_index)

        # Source vtable and melee calls, plus source-observed mutation order;
        # list cursor implementation callbacks are deliberately excluded.
        source_events: list[list[int]] = []
        for event in query.get("oracle_service_trace", []):
            name0 = event[0]
            operation = {"is_zonable": 2, "is_interactive": 3,
                         "interaction_radius": 4, "ai_melee_radius": 5}.get(name0)
            if operation is not None:
                source_events.append([operation, int(event[1]) & 0xFFFFFFFF])
            elif name0 == "mutate_next_to_end":
                source_events.append([0xFFFFFFFF, int(event[1]) & 0xFFFFFFFF])
        expected_trace[case["name"]] = source_events
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(output)
    return {"input_sha256": sha256(output), "cases": len(cases),
            "expected_service_traces": expected_trace}


def run_host_differential(cases: list[dict], queries: list[dict],
                          summaries: list[dict], output_dir: Path) -> dict:
    compiler = shutil.which("g++") or shutil.which("clang++")
    check(compiler is not None, "host differential requires g++ or clang++")
    output_dir.mkdir(parents=True, exist_ok=True)
    host_input = output_dir / "fixtures.bin"
    host_output = output_dir / "host-results.bin"
    executable = output_dir / ("character-aggro-point-query-host.exe"
                               if sys.platform == "win32" else
                               "character-aggro-point-query-host")
    source = HERE / "character_aggro_point_query_host.cpp"
    production_sources = [
        MODULE / "character_aggro_target_search.cpp",
        MODULE / "character_aggro_character_list.cpp",
    ]
    command = [compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic",
               "-fno-fast-math", "-ffp-contract=off", str(source),
               *(str(item) for item in production_sources), "-o", str(executable)]
    build = subprocess.run(command, check=False, capture_output=True, text=True)
    check(build.returncode == 0,
          f"host differential compile failed ({build.returncode}):\n{build.stdout}{build.stderr}")

    # The ARM trace was captured during each query before host serialization.
    for case, query in zip(cases, queries):
        query["oracle_service_trace"] = query["result"]["trace"]
    input_facts = build_host_input(cases, queries, host_input)
    run = subprocess.run([str(executable), str(host_input), str(host_output)],
                         check=False, capture_output=True, text=True)
    check(run.returncode == 0,
          f"host differential executable failed ({run.returncode}):\n{run.stdout}{run.stderr}")
    raw = host_output.read_bytes()
    check(len(raw) >= 8 and raw[:4] == b"CPO1", "host output header")
    count = struct.unpack_from("<I", raw, 4)[0]
    check(count == len(summaries), "host output case count")
    offset = 8
    comparisons = []
    mismatches = []
    trace_expected = input_facts["expected_service_traces"]
    for summary in summaries:
        result_count = struct.unpack_from("<I", raw, offset)[0]
        offset += 4
        actual_rows = []
        for _ in range(result_count):
            index, distance_bits, angle_bits, flags, reserved = struct.unpack_from(
                "<5I", raw, offset)
            offset += 20
            actual_rows.append({"object_index": index, "distance_bits": distance_bits,
                                "angle_bits": angle_bits, "flags": flags,
                                "reserved": reserved})
        trace_count = struct.unpack_from("<I", raw, offset)[0]
        offset += 4
        actual_trace = []
        for _ in range(trace_count):
            operation, actor_index = struct.unpack_from("<2I", raw, offset)
            offset += 8
            actual_trace.append([operation, actor_index])
        expected_rows = [{key: row[key] for key in (
            "object_index", "distance_bits", "angle_bits", "flags", "reserved")}
                         for row in summary["results"]]
        expected_events = trace_expected[summary["name"]]
        if actual_rows != expected_rows:
            mismatches.append({"case": summary["name"], "kind": "result_pop_order",
                               "host": actual_rows, "source": expected_rows})
        if actual_trace != expected_events:
            mismatches.append({"case": summary["name"], "kind": "service_trace",
                               "host": actual_trace, "source": expected_events,
                               "source_full_trace": summary["trace"]})
        comparisons.append({"name": summary["name"], "pop_order_count": len(actual_rows),
                            "pop_order_equal": actual_rows == expected_rows,
                            "service_events": len(actual_trace),
                            "service_trace_equal": actual_trace == expected_events})
    check(offset == len(raw), "host output has no trailing bytes")
    return {
        "status": "passed" if not mismatches else "failed",
        "compiler": compiler, "compile_command": command,
        "compile_stdout": build.stdout, "compile_stderr": build.stderr,
        "host_stdout": run.stdout, "host_stderr": run.stderr,
        "fixture_input": str(host_input), "fixture_input_sha256": input_facts["input_sha256"],
        "host_output": str(host_output), "host_output_sha256": sha256(raw),
        "host_executable": str(executable), "host_executable_sha256": sha256(executable.read_bytes()),
        "linked_sources": [{"path": str(item), "sha256": sha256(item.read_bytes())}
                           for item in [source, *production_sources]],
        "comparisons": comparisons,
        "mismatches": mismatches,
        "result_pop_order_compared": True,
        "target_info_distance_angle_flags_reserved_compared": True,
        "actor_service_trace_compared": True,
        "debug_override_path": "excluded: only the no-active-diagnostic-switch boundary has providers",
        "scope": "The real typed host ObjectListMethods/CharacterList query and heap kernel "
                 "were compiled from linked production sources and run over byte-identical "
                 "actor/list facts serialized from the five original ARM fixture cases. "
                 "Popped TargetInfo rows and actor-owned callback/mutation order match the "
                 "original-instruction oracle exactly.",
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original", type=Path,
                        default=REPO.parent / "test_strategy" / "libDungeonHunter2.so")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build" / "character-aggro-point-query-original" / "validation.json")
    parser.add_argument("--corpus", type=Path,
                        default=MODULE / "build" / "character-aggro-point-query-original" / "source-results.json")
    parser.add_argument("--host-dir", type=Path,
                        default=MODULE / "build" / "character-aggro-point-query-differential")
    args = parser.parse_args()
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    image = args.original.resolve().read_bytes()
    check(sha256(image) == ELF_SHA256 == manifest["original_sha256"], "pinned ELF identity")
    for function in manifest["functions"]:
        start, size = int(function["elf_address"], 0), function["size"]
        check(sha256(image[start:start + size]) == function["sha256"],
              f"original bytes changed: {function['original_symbol']}")
    verified_imports = verify_imports(args.original.resolve(), {
        0x30e3ac: "__aeabi_fsub", 0x30ed6c: "__aeabi_fmul",
        0x30eba4: "__aeabi_fadd", 0x30ec94: "__aeabi_fdiv",
        0x30e124: "sqrtf", 0x30e3dc: "acosf",
        0x30e2f8: "__aeabi_fcmpgt", 0x30e70c: "__aeabi_fcmplt",
        0x30e4b4: "__aeabi_fcmpge", 0x30e9ac: "__aeabi_fcmple",
    })

    cpu = OriginalCpu(args.original.resolve(), manifest)
    owner_vtable_callback = cpu.data + 0x92000 + 5 * 0x20
    cpu.hook_addresses = {
        owner_vtable_callback: "is_character",
        cpu.data + 0x92000 + 6 * 0x20: "is_interactive",
        cpu.data + 0x92000 + 7 * 0x20: "interaction_radius",
        cpu.data + 0x92000 + 8 * 0x20: "is_zonable",
    }
    # Hook list cursor methods, owner/candidate vtable methods and narrow global services.
    callback_names = {
        "list_reset": 0, "list_at_end": 1, "list_next": 2,
        "list_get": 3, "list_get_char": 4,
    }
    callbacks = {name: cpu.data + 0x92000 + i * 0x20 for name, i in callback_names.items()}
    # The fixture changes cursor state live, after the source calls Reset.
    cpu.fixture_callbacks = callbacks

    # Add stateful list-method handling ahead of generic virtual hooks.
    def fixture_hook(uc, address, size, unused):
        for start, length in ((0x4a2f34, 1172), (0x4a2730, 296),
                              (0x4a1ab8, 528), (0x4a1950, 116),
                              (0x4a2440, 164), (0x4a1f30, 156),
                              (0x4a22b4, 396), (0x38fb18, 160),
                              (0x38d570, 68),
                              (0x3935dc, 36), (0x313058, 16),
                              (0x312f40, 280)):
            if start <= address < start + length:
                cpu.executed_source_addresses.add(address)
        q = cpu.query
        if address in callbacks.values():
            check(q is not None, "list method called without fixture")
            name = next(k for k, v in callbacks.items() if v == address)
            sentinel = q["sentinel"]
            current = struct.unpack("<I", uc.mem_read(q["list_object"] + 4, 4))[0]
            if name == "list_reset":
                cpu.pointer(q["list_object"] + 4, 0)
                cpu.trace.append([name])
                cpu.returned()
            elif name == "list_at_end":
                if current == 0:
                    ended = struct.unpack("<I", uc.mem_read(sentinel, 4))[0] == sentinel
                else:
                    ended = current == sentinel
                cpu.trace.append([name, int(ended)])
                cpu.returned(int(ended))
            elif name == "list_next":
                nxt = struct.unpack("<I", uc.mem_read(sentinel, 4))[0] if current == 0 else \
                      struct.unpack("<I", uc.mem_read(current, 4))[0]
                cpu.pointer(q["list_object"] + 4, nxt)
                cpu.trace.append([name, next((i for i, node in enumerate(q["nodes"])
                                              if node == nxt), -1)])
                cpu.returned()
            elif name == "list_get":
                obj = struct.unpack("<I", uc.mem_read(current + 8, 4))[0]
                cpu.trace.append([name, next((i for i, actor in enumerate(q["objects"])
                                              if actor["address"] == obj), -1)])
                cpu.returned(obj)
            elif name == "list_get_char":
                char = struct.unpack("<I", uc.mem_read(current + 4, 4))[0]
                cpu.trace.append([name, next((i for i, actor in enumerate(q["objects"])
                                              if actor["address"] == char), -1)])
                cpu.returned(char)
            return
        cpu.hook(uc, address, size, unused)
    # List cursor methods are handled here; all other known boundaries delegate once.
    cpu.uc.hook_add(UC_HOOK_CODE, fixture_hook)

    # Vtable method names already have real callback identities in each fixture.
    # Source constructor and query invoke these address hooks directly.
    cases = [
        {"name": "filters-radius-closest-and-growth", "center": [0, 0, 0],
         "forward": [0, 1, 0], "radius": 20.0, "cone": 2.0 * math.pi,
         "melee_radius": 1.0, "owner_word1314": 10,
         "objects": [
             {"owner": True, "character": True},
             {"position": [0, 5, 0], "interaction_radius": 2.0},
             {"position": [0, 23, 0], "interaction_radius": 2.0},
             {"position": [0, 23.01, 0], "interaction_radius": 2.0},
             {"position": [0, 3, 0], "visible": 0},
             {"position": [0, 3, 0], "interactive": 0},
             {"position": [0, 3, 0], "zonable": 1, "zoned": 1, "in_zone": 0},
             {"position": [0, 4, 0], "zonable": 1, "zoned": 1, "in_zone": 1},
             {"position": [0, 4, 0], "word1310": 11},
             {"position": [0, 6, 0], "character": 0},
             *[{"position": [0, float(12 + i), 0]} for i in range(7)],
         ]},
        {"name": "authored-target-position-and-negative-radius", "center": [0, 0, 0],
         "forward": [1, 0, 0], "radius": 20.0, "cone": 2.0 * math.pi,
         "melee_radius": 0.0,
         "objects": [{"owner": True, "character": True},
                     {"position": [0, 2, 0], "target_position": [5, 0, 0],
                      "has_target_position": 1, "interaction_radius": -1.0}]},
        {"name": "live-link-mutation-during-interactive", "center": [0, 0, 0],
         "forward": [0, 1, 0], "radius": 50.0, "cone": 2.0 * math.pi,
         "objects": [{"owner": True, "character": True},
                     {"position": [0, 2, 0]}, {"position": [0, 4, 0]}],
         "mutate_after": 1},
        {"name": "source-cone-threshold-and-opposite-side", "center": [0, 0, 0],
         "forward": [0, 1, 0], "radius": 50.0, "cone": 0.5,
         "objects": [{"owner": True, "character": True},
                     {"position": [0, 5, 0]}, {"position": [5, 0, 0]},
                     {"position": [0, -5, 0]}]},
        {"name": "empty-list", "center": [0, 0, 0], "radius": 10.0,
         "objects": []},
    ]
    summaries = []
    host_cases = []
    host_queries = []
    for index, case in enumerate(cases):
        # Avoid mutation of the authored case when fixture stores source addresses.
        case = json.loads(json.dumps(case))
        q = build_fixture(cpu, case, index)
        result = run_query(cpu, q)
        q["result"] = result
        rows = []
        for row in result["results"]:
            obj = next((x for x in q["objects"] if x["address"] == row["object"]), None)
            check(obj is not None, "heap result object belongs to the live source list")
            rows.append({**row, "object_index": obj["index"],
                         "distance": floating(row["distance_bits"]),
                         "angle": floating(row["angle_bits"])})
        # Basic independent source expectations ensure the oracle is exercising
        # actual filters, adjusted distance, insertion and live list mutation.
        if case["name"] == "filters-radius-closest-and-growth":
            check([r["object_index"] for r in rows] == [1, 7, 10, 11, 12, 13, 14, 15, 16, 2],
                  f"unexpected source result ordering: {[r['object_index'] for r in rows]}")
            check(len(rows) > 6, "source heap block growth path reached")
            check(0x4a22b4 in cpu.executed_source_addresses,
                  "source deque allocates a new block past its initial capacity")
            check(result["allocation_bytes"] >= 2 * 120,
                  "source allocator fixture served initial and grown TargetInfo blocks")
            check(any(x[0] == "is_zonable" and x[1] == 7 for x in result["trace"]),
                  "zonable+in-zone candidate proceeds through source gate")
            check(not any(x[0] == "is_interactive" and x[1] == 4 for x in result["trace"]),
                  "invisible candidate rejected before IsInteractive")
        if case["name"] == "authored-target-position-and-negative-radius":
            check(len(rows) == 1 and rows[0]["object_index"] == 1,
                  "GetTargetPosition and signed adjusted radius affect query")
            check(abs(rows[0]["distance"] - 6.0) < 1e-5,
                  f"source target position and negative interaction radius reflected in distance: {rows}")
        if case["name"] == "live-link-mutation-during-interactive":
            check([r["object_index"] for r in rows] == [1],
                  "source reads mutated next link and skips removed successor")
        if case["name"] == "empty-list":
            check(rows == [], "empty borrowed source list returns no candidates")
        if case["name"] == "source-cone-threshold-and-opposite-side":
            check([r["object_index"] for r in rows] == [1],
                  "source cone threshold rejects perpendicular and opposite candidates")
            check(abs(rows[0]["angle"]) < 1e-6,
                  "source TargetInfo stores the computed angle")
        summaries.append({"name": case["name"], "result_count": len(rows),
                          "results": rows, "trace": result["trace"],
                          "source_allocated_bytes": result["allocation_bytes"]})
        host_cases.append(case)
        host_queries.append(q)
        cpu.query = None

    for address in (0x4a2f34, 0x4a3114, 0x4a3120, 0x4a317c,
                    0x4a3250, 0x4a32e8, 0x4a2440, 0x4a1f30,
                    0x4a22b4, 0x38fb18, 0x38d570,
                    0x3935dc, 0x313058, 0x312f40, 0x4a1ab8,
                    0x4a1950):
        check(address in cpu.executed_source_addresses,
              f"source instruction path not reached at {address:#x}")

    host_report = run_host_differential(host_cases, host_queries, summaries, args.host_dir)
    host_report_path = args.host_dir / "host-differential.json"
    host_report_path.write_text(json.dumps(host_report, indent=2) + "\n", encoding="utf-8")
    check(host_report["status"] == "passed",
          f"host differential has {len(host_report['mismatches'])} mismatch(es); "
          f"details: {host_report_path}")

    args.corpus.parent.mkdir(parents=True, exist_ok=True)
    args.corpus.write_text(json.dumps({"original_sha256": ELF_SHA256,
                                       "cases": summaries}, indent=2) + "\n",
                           encoding="utf-8")
    report = {
        "status": "passed",
        "original_sha256": ELF_SHA256,
        "source_query_function": "_ZN14ObjectSearcher10TargetList6SearchERK7Point3DIfEfS4_fRNS_11IObjectListE",
        "source_query_address": "0x4a2f34",
        "source_query_size": 1172,
        "cases": len(summaries),
        "source_arm_instructions_executed": True,
        "source_target_list_constructor_executed": True,
        "source_character_validity_and_target_heap_executed": True,
        "source_character_list_nodes_read_live": True,
        "active_debug_switch_branch_supported": False,
        "debug_switch_fixture": "original GetSwitch lookup misses; active debug override path is excluded",
        "original_import_calls": cpu.import_calls,
        "verified_original_imports": verified_imports,
        "executed_source_addresses": [f"0x{x:x}" for x in sorted(cpu.executed_source_addresses)],
        "explicit_fixture_services": [
            "DebugSwitches::Load/GetSwitch report no active diagnostic switch",
            "source allocator/free entry points use bounded fixture storage",
            "Character IsZonable/IsInteractive/GetInteractionRadius virtual return values",
            "owner CharAI::AI_GetMeleeRadius return value",
            "borrowed IObjectList virtual methods over live intrusive nodes",
        ],
        "scope": (
            "Original ARM32 TargetList constructor, complete 1,172-byte point-query body, "
            "Character validation, TargetInfo insertion, closest heap and pop execute. "
            "Actor-owned virtuals and diagnostics are explicit fixture services. A compiled "
            "portable CharacterList/ObjectListMethods host query now matches every ordered "
            "TargetInfo pop and actor-service/mutation event under byte-identical fixtures. "
            "Live Android actor-state acquisition remains outside this proof."
        ),
        "host_differential": host_report,
        "host_differential_report": str(host_report_path),
        "corpus_sha256": sha256(args.corpus.read_bytes()),
        "runner_sha256": sha256(Path(__file__).read_bytes()),
        "manifest_sha256": sha256(MANIFEST.read_bytes()),
    }
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, RuntimeError, AssertionError) as exc:
        raise SystemExit(f"ERROR: {exc}") from exc
