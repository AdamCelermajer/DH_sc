"""Differentially run Character::CanUpdate against the original ARM body."""
from __future__ import annotations
import argparse, hashlib, json, struct, subprocess, sys
from pathlib import Path

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
BASE = 0x02000000
ONLINE, PLAYER, MANAGER = BASE + 0x1000, BASE + 0x3000, BASE + 0x5000
VTABLE, STUB_REMOTE, STUB_DEAD = BASE + 0x7000, BASE + 0x8000, BASE + 0x8010
CHARACTER, VISUAL, NODE = BASE + 0x9000, BASE + 0xf000, BASE + 0x10000
ALT_VISUAL, ALT_NODE = BASE + 0x11000, BASE + 0x12000

def write_byte(cpu, address, value):
    cpu.uc.mem_write(address, bytes([value & 255]))

def write_word(cpu, address, value):
    cpu.pointer(address, value & 0xffffffff)

def fixtures():
    rows = []
    def add(name, **values):
        x = dict(online=0, remote=0, local=0x7002, cull=1, dead=0,
                 respawn=0, visual=1, node_cull=1, visibility_80=0,
                 field_2fc=0, gate_1480=0, node_flag=9, mutations=0)
        x.update(values)
        rows.append((name, x))
    add("offline_different_unculled", cull=1)
    add("offline_same_player", local=0x7001)
    add("offline_culling_rejects", cull=0)
    add("offline_culling_flag_bypasses", cull=0, gate_1480=1)
    add("offline_zero_culling_fields", cull=0, node_cull=0)
    add("offline_dead_no_respawn", dead=0x80000000, respawn=0)
    add("offline_dead_respawn", dead=1, respawn=0x80000000, visibility_80=0, cull=1)
    add("offline_visibility_skips_respawn", dead=1, visibility_80=1)
    add("online_remote_updated", online=0x80, remote=1, dead=1, respawn=0)
    add("online_not_remote_dead", online=1, remote=0, dead=1, respawn=1, cull=1)
    add("online_remote_cull_rejects", online=1, remote=0, cull=0)
    add("online_no_visual_dead", online=1, remote=0, visual=0, dead=1, respawn=1)
    add("online_remote_marks_scene", online=1, remote=1, visibility_80=1)
    add("local_field_mutated_by_online", mutations=1, local=0x7002, cull=1)
    add("visual_rebound_by_remote", online=1, remote=0, mutations=2,
        local=0x7002, cull=1)
    add("field_mutated_after_local_read", mutations=4, local=0x7002, cull=0)
    add("culling_gate_mutated_by_callback", mutations=8, cull=0)
    add("update_enabled_mutated_by_dead", mutations=16, dead=1, respawn=0, cull=1,
        visibility_80=0)
    add("culling_byte_2fc", field_2fc=1, cull=1)
    add("negative_raw_dead_and_remote", online=1, remote=1, dead=0xffffffff)
    add("online_byte_high_bit", online=0x80, remote=0xff, visibility_80=0x80)
    add("source_write_255_raw_bytes", online=0xff, remote=0xff, visibility_80=0xff)
    add("dead_callback_rebinds_captured_visual_root", dead=0, visibility_80=1,
        mutations=128, cull=1)
    add("respawn_callback_rebinds_captured_visual_root", dead=1, respawn=1,
        visibility_80=0, mutations=256, cull=1)
    add("port_protocol_same_character_reentry", mutations=32, cull=1)
    add("logical_identity_is_captured", mutations=1024, cull=1)
    return rows

def oracle(original, exe):
    from elftools.elf.elffile import ELFFile
    from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_WRITE
    sys.path.insert(0, str(ROOT / "port/engine-resources/tests"))
    from cpu import Cpu
    manifest = {"functions": [{"original_symbol": "_ZN9Character9CanUpdateEv",
                                "elf_address": "0x3a52a4", "size": 316}]}
    raw = original.read_bytes()
    with original.open("rb") as stream:
        elf = ELFFile(stream)
        symbols = {s.name: s for s in elf.get_section_by_name(".symtab").iter_symbols()}
        symbol = symbols["_ZN9Character9CanUpdateEv"]
        assert (int(symbol["st_value"]), int(symbol["st_size"])) == (0x3a52a4, 316)
        loads = [s for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]
        def image_bytes(address, count):
            segment = next(s for s in loads if s["p_vaddr"] <= address and
                           address + count <= s["p_vaddr"] + s["p_filesz"])
            start = int(segment["p_offset"]) + address - int(segment["p_vaddr"])
            return raw[start:start + count]
        function_bytes = image_bytes(0x3a52a4, 316)
        assert hashlib.sha256(function_bytes).hexdigest() == "8ab1ce0602bf7ecb8f54e165b150844dfe3931539ac1f2e708dd8369af879d35"
        # The caller performs LDR PC,[r3,#0x148], and the vtable entry points
        # to this exact predicate body.
        assert struct.unpack("<I", image_bytes(0x3abf60, 4))[0] == 0xe593f148
        assert struct.unpack("<I", image_bytes(0x966080, 4))[0] == 0x3a52a4
    old = Cpu(original, False, manifest)
    assert hashlib.sha256(raw).hexdigest() == SHA
    application = old.symbols["_ZN9SingletonI11ApplicationE6s_instE"]
    old.pointer(application + 0x40, MANAGER)
    records = []
    for name, x in fixtures():
        old.uc.mem_write(BASE, bytes(0x18000))
        old.uc.mem_write(ONLINE, bytes(16))
        write_byte(old, ONLINE + 5, x["online"])
        write_word(old, PLAYER + 0x660, x["local"])
        write_word(old, VTABLE + 0x34, STUB_DEAD)
        write_word(old, VTABLE + 0x54, STUB_REMOTE)
        write_word(old, CHARACTER, VTABLE)
        write_word(old, CHARACTER + 0x2d8, VISUAL if x["visual"] else 0)
        write_word(old, CHARACTER + 0x418, 0x7001)
        write_byte(old, CHARACTER + 0x80, x["visibility_80"])
        write_byte(old, CHARACTER + 0x2fc, x["field_2fc"])
        write_byte(old, CHARACTER + 0x1480, x["gate_1480"])
        write_word(old, VISUAL + 8, NODE)
        write_word(old, ALT_VISUAL + 8, ALT_NODE)
        write_word(old, NODE + 0x118, x["node_cull"])
        write_word(old, ALT_NODE + 0x118, x["node_cull"] + 1)
        write_byte(old, NODE + 0x200, x["node_flag"])
        write_byte(old, ALT_NODE + 0x200, x["node_flag"])
        calls, scene_writes = [], []
        def ret(value):
            old.put(0, value)
            old.uc.reg_write(old.pc, old.uc.reg_read(old.lr))
        def hook(uc, at, size, unused):
            if at == 0x7fd794:
                calls.append([0, x["online"]])
                if x["mutations"] & 1: write_word(old, CHARACTER + 0x418, 0x7002)
                ret(ONLINE)
            elif at == 0x36e478:
                calls.append([2, x["local"]])
                if x["mutations"] & 4: write_word(old, CHARACTER + 0x418, 0x7003)
                ret(PLAYER)
            elif at == 0x33de90:
                assert old.reg(0) == CHARACTER and old.reg(1) == CHARACTER + 0x12c
                calls.append([3, 0x12c, x["cull"]])
                if x["mutations"] & 8: write_byte(old, CHARACTER + 0x1480, 1)
                ret(x["cull"])
            elif at == 0x3a5248:
                calls.append([5, x["respawn"]])
                if x["mutations"] & 256:
                    write_byte(old, CHARACTER + 0x80, 1)
                    write_word(old, VISUAL + 8, ALT_NODE)
                ret(x["respawn"])
            elif at == STUB_REMOTE:
                calls.append([1, x["remote"]])
                if x["mutations"] & 2: write_word(old, CHARACTER + 0x2d8, ALT_VISUAL)
                ret(x["remote"])
            elif at == STUB_DEAD:
                calls.append([4, x["dead"]])
                if x["mutations"] & 16: write_byte(old, CHARACTER + 0x80, 1)
                if x["mutations"] & 128: write_word(old, VISUAL + 8, ALT_NODE)
                ret(x["dead"])
        def memory_write(uc, access, address, size, value, unused):
            if address in (NODE + 0x200, ALT_NODE + 0x200):
                scene_writes.append([address, size, value & 255])
        handle = old.uc.hook_add(UC_HOOK_CODE, hook)
        write_handle = old.uc.hook_add(UC_HOOK_MEM_WRITE, memory_write)
        actual = old.invoke("_ZN9Character9CanUpdateEv", [CHARACTER])
        old.uc.hook_del(handle)
        old.uc.hook_del(write_handle)

        host_args = [x[k] for k in ("online", "remote", "local", "cull", "dead", "respawn",
                                      "visual", "node_cull", "visibility_80", "field_2fc",
                                      "gate_1480", "node_flag", "mutations")]
        host = json.loads(subprocess.run([str(exe), *(str(v) for v in host_args)],
                                         check=True, capture_output=True, text=True).stdout)
        expected_ops = [q[0] for q in calls]
        got_ops = [q[0] for q in host["trace"]]
        expected_trace = []
        for q in calls:
            op = q[0]
            subject, argument, first, second = 0, 0, 0, 0
            if op:
                subject = 0x1001
            if op == 2:
                second = 1
            elif op == 3:
                argument = 0x12c
            expected_trace.append([op, 0x1001, subject, argument, first, second])
        assert host["status"] == 0 and host["value"] == actual, (name, actual, host)
        assert got_ops == expected_ops, (name, expected_ops, got_ops, host)
        assert host["trace"] == expected_trace, (name, expected_trace, host["trace"])
        assert host["writes"] == len(scene_writes), (name, scene_writes, host)
        if x["mutations"] & 32:
            assert host["nested_status"] == 5 and host["nested_calls"] == 0
        assert host["node_flag"] == old.uc.mem_read(NODE + 0x200, 1)[0], (name, host)
        assert host["alt_flag"] == old.uc.mem_read(ALT_NODE + 0x200, 1)[0], (name, host)
        records.append({"name": name, "input": x, "original_result": actual,
                        "original_calls": calls, "original_node_flag": old.uc.mem_read(NODE + 0x200, 1)[0],
                        "host": host})
    return records

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--output", type=Path, default=MODULE / "build/character-update-eligibility")
    args = parser.parse_args()
    out = args.output.resolve(); out.mkdir(parents=True, exist_ok=True)
    exe = out / "host.exe"
    src = MODULE / "character_update_eligibility.cpp"
    test = MODULE / "tests/character_update_eligibility.cpp"
    subprocess.run([args.compiler, "-std=c++17", "-O1", "-fno-fast-math", "-ffp-contract=off",
                    "-Wall", "-Wextra", "-Werror", "-pedantic", str(src), str(test), "-o", str(exe)], check=True)
    records = oracle(args.original_elf.resolve(), exe)
    guard_report = json.loads(subprocess.run([str(exe), "--guards"],
                                             check=True, capture_output=True, text=True).stdout)
    failure_report = json.loads(subprocess.run([str(exe), "--failures"],
                                               check=True, capture_output=True, text=True).stdout)
    report = {"validation": "PASS", "original_arm_cases": len(records), "mismatches": 0,
              "original_elf_sha256": SHA, "functions": [{"symbol": "_ZN9Character9CanUpdateEv",
              "address": "0x3a52a4", "symbol_size": 316, "active_instruction_bytes": 308}],
              "cases": records, "source_sha256": {str(p.relative_to(ROOT)).replace("\\", "/"):
              hashlib.sha256(p.read_bytes()).hexdigest() for p in (src, MODULE / "character_update_eligibility.hpp", test, Path(__file__).resolve())},
              "host_protocol_guards": guard_report, "provider_failure_cases": failure_report,
              "scope": "Character::CanUpdate only. Visual/scene state is a typed projection; online/local-player, culling, dead and respawn calls are fixture providers. Character::Update scheduler, world ownership, and native wiring are outside this unit."}
    (out / "validation.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({k: report[k] for k in ("validation", "original_arm_cases", "mismatches")}))

if __name__ == "__main__": main()
