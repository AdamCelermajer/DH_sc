"""Run the original ARM32 AI_SetTarget body against the host source kernel.

DebugSwitches, Character::GetCharAIId, target IsDead and AI_IsInSight are
synchronous deterministic fixtures. The AI_SetTarget ARM instructions execute
from the pinned original ELF; those fixture bodies are intercepted explicitly.
"""
from __future__ import annotations

import argparse
import ctypes
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys

from unicorn import UC_HOOK_CODE

HERE = Path(__file__).resolve().parent
MODULE = HERE.parent
REPO = HERE.parents[2]
sys.path.insert(0, str(HERE))
from navigation_differential import Cpu  # noqa: E402

MANIFEST = (REPO / "port" / "level-world" / "reference" /
            "character-ai-set-target" / "original-functions.json")
EXPECTED_ORIGINAL_SHA256 = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"

AI_ID = 0x101
OWNER_A_ID = 0x201
OWNER_B_ID = 0x202
TARGET_IDS = {0: 0, 1: 0x301, 2: 0x302, 3: 0x303}
ID_TO_TARGET = {value: key for key, value in TARGET_IDS.items() if value}
TRACE_UPPER = 1
TRACE_LOWER = 2
DEBUG_LOAD = 1
DEBUG_GET = 2
IS_DEAD = 3
IN_SIGHT = 4


class Owner(ctypes.Structure):
    _fields_ = [("identity", ctypes.c_size_t), ("character_ai_id", ctypes.c_int32),
                ("target_change_marker_14d0", ctypes.c_uint16), ("reserved", ctypes.c_uint16)]


class State(ctypes.Structure):
    _fields_ = [("identity", ctypes.c_size_t), ("owner", ctypes.POINTER(Owner)),
                ("requested_target", ctypes.c_size_t), ("target", ctypes.c_size_t),
                ("last_target", ctypes.c_size_t), ("alive_snapshot", ctypes.c_uint8),
                ("sight_snapshot", ctypes.c_uint8), ("sticky", ctypes.c_uint8),
                ("reserved", ctypes.c_uint8)]


class Request(ctypes.Structure):
    _fields_ = [("operation", ctypes.c_uint32), ("key", ctypes.c_uint32),
                ("ai_identity", ctypes.c_size_t), ("owner_identity", ctypes.c_size_t),
                ("target_identity", ctypes.c_size_t)]


class Response(ctypes.Structure):
    _fields_ = [("word", ctypes.c_uint32), ("reserved", ctypes.c_uint32)]


Invoke = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                          ctypes.POINTER(Request), ctypes.POINTER(Response))


class Services(ctypes.Structure):
    _fields_ = [("context", ctypes.c_void_p), ("ai_property_count", ctypes.c_int32),
                ("invoke", Invoke)]


class Scenario:
    def __init__(self, name: str, *, current: int, last: int, requested: int,
                 force: int = 0, owner_marker: int = 0x1234,
                 owner_ai_id: int = 35, trace: int = 0, detail: int = 0,
                 dead: dict[int, int] | None = None, sight: dict[int, int] | None = None,
                 mutate_current_on_trace: bool = False,
                 mutate_target_on_dead: int | None = None,
                 mutate_owner_on_dead: bool = False):
        self.name = name
        self.current = current
        self.last = last
        self.requested = requested
        self.force = force
        self.owner_marker = owner_marker
        self.owner_ai_id = owner_ai_id
        self.trace = trace
        self.detail = detail
        self.dead = dead or {}
        self.sight = sight or {}
        self.mutate_current_on_trace = mutate_current_on_trace
        self.mutate_target_on_dead = mutate_target_on_dead
        self.mutate_owner_on_dead = mutate_owner_on_dead


class ArmOracle:
    def __init__(self, engine: Path, manifest: dict):
        self.cpu = Cpu(engine, False, manifest)
        self.ai = self.cpu.data + 0x1000
        self.owner_a = self.cpu.data + 0x5000
        self.owner_b = self.cpu.data + 0x7000
        self.targets = {1: self.cpu.data + 0x9000,
                        2: self.cpu.data + 0xb000,
                        3: self.cpu.data + 0xd000}
        self.vtables = {1: self.cpu.data + 0x11000,
                        2: self.cpu.data + 0x12000,
                        3: self.cpu.data + 0x13000}
        self.cpu.events = []
        self.virtual = self.cpu.data + 0x1ff0000
        self.cpu.uc.mem_write(self.virtual, bytes.fromhex("1eff2fe1"))  # bx lr
        self.events: list[tuple[int, int, int, int, int]] = []
        self.getter_owners: list[int] = []
        self.keys: dict[int, str] = {}
        self.active: Scenario | None = None
        self.owner_map = {self.owner_a: OWNER_A_ID, self.owner_b: OWNER_B_ID}
        self.target_map = {address: tag for tag, address in self.targets.items()}
        self.current_ai_tag = AI_ID
        self.cpu.uc.hook_add(UC_HOOK_CODE, self.hook)

    def word(self, address: int) -> int:
        return int.from_bytes(self.cpu.uc.mem_read(address, 4), "little")

    def put_word(self, address: int, value: int) -> None:
        self.cpu.pointer(address, value)

    def read_target(self, value: int) -> int:
        if not value:
            return 0
        return self.target_map.get(value, -1)

    def returned(self, value: int = 0) -> None:
        self.cpu.put(0, value)
        self.cpu.uc.reg_write(self.cpu.pc, self.cpu.uc.reg_read(self.cpu.lr))

    def configure(self, scenario: Scenario) -> None:
        self.active = scenario
        self.events.clear()
        self.getter_owners.clear()
        self.keys.clear()
        c = self.cpu
        for address in (self.ai, self.owner_a, self.owner_b):
            c.uc.mem_write(address, bytes(0x2000))
        for tag, target in self.targets.items():
            vtable = self.vtables[tag]
            c.uc.mem_write(target, bytes(0x200))
            c.pointer(target, vtable)
            c.pointer(vtable + 0x34, self.virtual)
        owner = self.owner_a
        c.pointer(self.ai + 4, owner)
        self.put_word(self.ai + 0x3c, 0)
        self.put_word(self.ai + 0x40, self.targets.get(scenario.current, 0))
        self.put_word(self.ai + 0x44, self.targets.get(scenario.last, 0))
        c.uc.mem_write(self.ai + 0x48, bytes((3, 4, 1)))
        c.uc.mem_write(self.ai + 0x4c, b"\x01")
        c.uc.mem_write(owner + 0x14d0, scenario.owner_marker.to_bytes(2, "little"))
        c.uc.mem_write(owner + 0xffc, int(scenario.owner_ai_id & 0xffffffff).to_bytes(4, "little"))
        c.uc.mem_write(self.owner_b + 0x14d0, b"\x55\xaa")
        c.uc.mem_write(self.owner_b + 0xffc, (0xffffffff).to_bytes(4, "little"))

    def normalize_owner(self, address: int) -> int:
        return self.owner_map.get(address, -1)

    def hook(self, uc, address: int, size: int, _user_data) -> None:
        scenario = self.active
        if scenario is None:
            return
        if address == 0x337888:  # DebugSwitches::load
            self.events.append((DEBUG_LOAD, 0, AI_ID, 0, 0))
            self.returned()
        elif address == 0x3140ec:  # source string constructor
            destination = self.cpu.reg(0)
            string_pointer = self.cpu.reg(1)
            raw = bytearray()
            for offset in range(64):
                value = bytes(uc.mem_read(string_pointer + offset, 1))[0]
                if value == 0:
                    break
                raw.append(value)
            self.keys[destination] = raw.decode("ascii")
            self.returned(destination)
        elif address == 0x337a88:  # DebugSwitches::GetSwitch
            key = self.keys.get(self.cpu.reg(1), "")
            key_id = TRACE_UPPER if key == "IsTracingCharAITarget" else TRACE_LOWER
            self.events.append((DEBUG_GET, key_id, AI_ID, 0, 0))
            if key_id == TRACE_UPPER and scenario.mutate_current_on_trace:
                self.put_word(self.ai + 0x40,
                              self.targets.get(scenario.requested, 0))
            self.returned(scenario.trace if key_id == TRACE_UPPER else scenario.detail)
        elif address in (0x318254, 0x310440):  # temporary string teardown/allocator
            self.returned()
        elif address == 0x3a2fec:  # Character::GetCharAIId, pure getter
            owner = self.cpu.reg(0)
            self.getter_owners.append(owner)
            raw = self.word(owner + 0xffc)
            signed = raw if raw < 0x80000000 else raw - 0x100000000
            self.returned(signed if 0 <= signed < 76 else 8)
        elif address == self.virtual:  # target's virtual IsDead
            target = self.cpu.reg(0)
            target_tag = self.read_target(target)
            self.events.append((IS_DEAD, 0, AI_ID, 0, target_tag))
            self.returned(scenario.dead.get(target_tag, 0))
            if scenario.mutate_target_on_dead is not None:
                self.put_word(self.ai + 0x40,
                              self.targets.get(scenario.mutate_target_on_dead, 0))
                if scenario.mutate_owner_on_dead:
                    uc.mem_write(self.ai + 4, self.owner_b.to_bytes(4, "little"))
        elif address == 0x3d4ed8:  # complete AI_IsInSight service
            ai = self.cpu.reg(0)
            arg = self.cpu.reg(1)
            target_tag = self.read_target(arg)
            owner = self.word(ai + 4)
            self.events.append((IN_SIGHT, 0, AI_ID, self.normalize_owner(owner), target_tag))
            if not arg:
                live_target = self.word(ai + 0x40)
                target_tag = self.read_target(live_target)
            self.returned(scenario.sight.get(target_tag, 0))

    def run(self, scenario: Scenario) -> dict:
        self.configure(scenario)
        self.cpu.invoke(0x3d6890, [self.ai,
                                   self.targets.get(scenario.requested, 0),
                                   scenario.force])
        owner = self.word(self.ai + 4)
        return {
            "requested": self.read_target(self.word(self.ai + 0x3c)),
            "target": self.read_target(self.word(self.ai + 0x40)),
            "last": self.read_target(self.word(self.ai + 0x44)),
            "alive": bytes(self.cpu.uc.mem_read(self.ai + 0x48, 1))[0],
            "sight": bytes(self.cpu.uc.mem_read(self.ai + 0x49, 1))[0],
            "sticky": bytes(self.cpu.uc.mem_read(self.ai + 0x4c, 1))[0],
            "owner_id": self.normalize_owner(owner),
            "owner_a_marker": int.from_bytes(self.cpu.uc.mem_read(self.owner_a + 0x14d0, 2), "little"),
            "owner_b_marker": int.from_bytes(self.cpu.uc.mem_read(self.owner_b + 0x14d0, 2), "little"),
            "events": list(self.events),
            "getter_owner_ids": [self.normalize_owner(x) for x in self.getter_owners],
        }


def identity_for_target(tag: int) -> int:
    return TARGET_IDS.get(tag, 0)


def build_host_library(compiler: str, output: Path) -> None:
    compiler_path = shutil.which(compiler)
    if compiler_path is None:
        raise RuntimeError(f"C++ compiler not found: {compiler}")
    output.parent.mkdir(parents=True, exist_ok=True)
    command = [compiler_path, "-std=c++17", "-shared", "-static-libgcc",
               "-static-libstdc++", "-Wl,--export-all-symbols",
               str(MODULE / "character_ai_set_target.cpp"), "-o", str(output)]
    result = subprocess.run(command, cwd=REPO, capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(f"DLL build failed ({result.returncode}):\n{result.stdout}{result.stderr}")


def run_host_case(library, scenario: Scenario) -> tuple[dict, list[tuple[int, int, int, int, int]]]:
    owner_a = Owner(OWNER_A_ID, scenario.owner_ai_id, scenario.owner_marker, 0)
    owner_b = Owner(OWNER_B_ID, -1, 0xaa55, 0)
    owners = {OWNER_A_ID: owner_a, OWNER_B_ID: owner_b}
    state = State(AI_ID, ctypes.pointer(owner_a), 0,
                  identity_for_target(scenario.current),
                  identity_for_target(scenario.last), 3, 4, 1, 0)
    events: list[tuple[int, int, int, int, int]] = []

    @Invoke
    def invoke(_context, request_pointer, response_pointer):
        request = request_pointer.contents
        op, key = int(request.operation), int(request.key)
        target_tag = (0 if not request.target_identity else
                      ID_TO_TARGET.get(int(request.target_identity), -1))
        events.append((op, key, int(request.ai_identity),
                       int(request.owner_identity), target_tag))
        response = response_pointer.contents
        response.word = 0
        response.reserved = 0
        if op == DEBUG_GET:
            if key == TRACE_UPPER:
                response.word = scenario.trace
                if scenario.mutate_current_on_trace:
                    state.target = identity_for_target(scenario.requested)
            else:
                response.word = scenario.detail
        elif op == IS_DEAD:
            target_tag = ID_TO_TARGET.get(int(request.target_identity), -1)
            response.word = scenario.dead.get(target_tag, 0)
            if scenario.mutate_target_on_dead is not None:
                state.target = identity_for_target(scenario.mutate_target_on_dead)
                if scenario.mutate_owner_on_dead:
                    state.owner = ctypes.pointer(owner_b)
        elif op == IN_SIGHT:
            target_tag = ID_TO_TARGET.get(int(request.target_identity), -1)
            if not request.target_identity and state.target:
                target_tag = ID_TO_TARGET.get(int(state.target), -1)
            response.word = scenario.sight.get(target_tag, 0)
        return 0

    services = Services(None, 76, invoke)
    library.dh2_character_ai_set_target.argtypes = [ctypes.POINTER(State), ctypes.c_size_t,
                                                     ctypes.c_uint8, ctypes.POINTER(Services)]
    library.dh2_character_ai_set_target.restype = ctypes.c_int
    status = library.dh2_character_ai_set_target(
        ctypes.byref(state), identity_for_target(scenario.requested), scenario.force,
        ctypes.byref(services))
    if status != 0:
        raise AssertionError((scenario.name, "host status", status))
    owner_identity = int(state.owner.contents.identity) if state.owner else 0
    snapshot = {
        "requested": ID_TO_TARGET.get(int(state.requested_target), 0),
        "target": ID_TO_TARGET.get(int(state.target), 0),
        "last": ID_TO_TARGET.get(int(state.last_target), 0),
        "alive": int(state.alive_snapshot),
        "sight": int(state.sight_snapshot),
        "sticky": int(state.sticky),
        "owner_id": owner_identity,
        "owner_a_marker": int(owner_a.target_change_marker_14d0),
        "owner_b_marker": int(owner_b.target_change_marker_14d0),
        "events": events,
        "getter_owner_ids": ([owner_identity] if state.target else []),
    }
    return snapshot, events


def compare(scenario: Scenario, old: dict, new: dict) -> None:
    for key in ("requested", "target", "last", "alive", "sight", "sticky",
                "owner_id", "owner_a_marker", "owner_b_marker", "events"):
        if old[key] != new[key]:
            raise AssertionError((scenario.name, key, old[key], new[key]))
    # GetCharAIId is a pure source getter and the kernel evaluates its local
    # projection, so compare source call count/identity separately.
    expected_getters = ([OWNER_A_ID] if scenario.requested and not scenario.force else [])
    if old["getter_owner_ids"] != expected_getters:
        raise AssertionError((scenario.name, "original getter order", old["getter_owner_ids"], expected_getters))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--engine", type=Path,
                        default=REPO.parent / "test_strategy" / "libDungeonHunter2.so")
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--library", type=Path,
                        default=MODULE / "build" / "character-ai-set-target" /
                        "character_ai_set_target.dll")
    parser.add_argument("--report", type=Path,
                        default=MODULE / "build" / "character-ai-set-target" /
                        "arm-differential.json")
    args = parser.parse_args()
    manifest = json.loads(MANIFEST.read_text(encoding="utf-8"))
    original_sha = hashlib.sha256(args.engine.read_bytes()).hexdigest()
    if original_sha != EXPECTED_ORIGINAL_SHA256 or manifest["original_sha256"] != original_sha:
        raise RuntimeError(f"wrong source ELF SHA-256: {original_sha}")
    build_host_library(args.compiler, args.library.resolve())
    host = ctypes.CDLL(str(args.library.resolve()))

    scenarios = [
        Scenario("force-nonnull", current=1, last=3, requested=2, force=1,
                 owner_marker=0x4242),
        Scenario("normal-set-character-target", current=0, last=3, requested=1,
                 dead={1: 1}, sight={1: 1}),
        Scenario("normal-clear-null", current=1, last=3, requested=0,
                 owner_marker=0xbeef),
        Scenario("trace-clear-null-early-return", current=1, last=3, requested=0,
                 trace=1),
        Scenario("trace-old-null-to-target", current=0, last=0, requested=2,
                 trace=1, detail=0, dead={2: 1}, sight={2: 1}),
        Scenario("trace-target-switch", current=1, last=1, requested=2,
                 trace=1, detail=1, dead={2: 0}, sight={2: 1}),
        Scenario("duplicate-target-refresh", current=2, last=2, requested=2,
                 dead={2: 1}, sight={2: 0}),
        Scenario("fresh-current-after-debug", current=1, last=0, requested=2,
                 trace=1, mutate_current_on_trace=True, dead={2: 1}, sight={2: 1}),
        Scenario("fresh-target-and-owner-after-virtual", current=1, last=0, requested=2,
                 dead={2: 0}, sight={3: 1}, mutate_target_on_dead=3,
                 mutate_owner_on_dead=True),
    ]
    oracle = ArmOracle(args.engine.resolve(), manifest)
    rows = []
    for scenario in scenarios:
        old = oracle.run(scenario)
        new, _events = run_host_case(host, scenario)
        compare(scenario, old, new)
        rows.append({"name": scenario.name, "snapshot": old,
                     "service_events": len(old["events"])})

    report = {
        "status": "passed",
        "original_sha256": original_sha,
        "host_library_sha256": hashlib.sha256(args.library.read_bytes()).hexdigest(),
        "scenario_count": len(scenarios),
        "mismatches": 0,
        "original_function_executed": "CharAI::AI_SetTarget at 0x3d6890",
        "source_helper_services": [
            "DebugSwitches::load/GetSwitch",
            "Character::GetCharAIId pure leaf",
            "target GameObject vtable +0x34 IsDead",
            "CharAI::AI_IsInSight(GameObject const*)",
        ],
        "scope": (
            "The original AI_SetTarget ARM32 instructions execute in Unicorn and are compared "
            "with the host source kernel, including final fields and ordered debug/dead/sight "
            "services. Source helpers are deterministic fixtures. This is not Android actor "
            "integration or an independent DebugSwitches/IsDead/AI_IsInSight reconstruction."
        ),
        "scenarios": rows,
    }
    report_path = args.report.resolve()
    report_path.parent.mkdir(parents=True, exist_ok=True)
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"original ARM32 AI_SetTarget differential passed: {len(scenarios)} scenarios; 0 mismatches")
    print(f"report: {report_path}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (OSError, ValueError, KeyError, RuntimeError) as exc:
        raise SystemExit(f"ERROR: {exc}") from exc
