"""Compare the bounded AI delay producer to original ARM instructions."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess

MODULE = Path(__file__).resolve().parents[1]
ROOT = MODULE.parents[1]
ORIGINAL_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"


def fixtures():
    # countdown, elapsed, turn, disabled, dt1, dt2, ordinary seed/counter,
    # mutation provider. They are fixture values, not original startup globals.
    return [
        ("not_turn_fresh_dt", [100, 0, 0, 0, 16, 19, 123, 4, 0]),
        ("cross_500_waits_this_frame", [10, 499, 0, 0, 16, 20, 123, 4, 0]),
        ("elapsed_500_forces_turn", [100, 500, 0, 0, 16, 19, 123, 4, 0]),
        ("elapsed_negative_is_signed", [0, 0xfffffff0, 0, 0, 16, 20, 123, 4, 0]),
        ("elapsed_large_signed_forces", [0, 0x7fffffff, 0, 0, 16, 20, 123, 4, 0]),
        ("turn_delay_still_positive", [17, 41, 1, 0, 16, 19, 123, 4, 0]),
        ("turn_exact_delay_expiry", [16, 41, 1, 0, 16, 19, 123, 4, 0]),
        ("turn_delay_overshoot", [15, 41, 1, 0, 16, 19, 123, 4, 0]),
        ("zero_delay_no_dt", [0, 41, 1, 0, 16, 19, 123, 4, 0]),
        ("negative_delay_no_dt", [0xffffffff, 41, 1, 0, 16, 19, 123, 4, 0]),
        ("debug_bypasses_delay_rng", [100, 41, 1, 1, 16, 19, 123, 4, 0]),
        ("debug_noncanonical_true", [100, 41, 7, 0xffffffff, 16, 19, 123, 4, 0]),
        ("debug_not_queried_without_turn", [100, 41, 0, 1, 16, 19, 123, 4, 0]),
        ("zero_seed", [0, 0, 1, 0, 16, 19, 0, 0, 0]),
        ("seed_32bit_multiply_wrap", [0, 0, 1, 0, 16, 19, 0xffffffff, 4, 0]),
        ("counter_wrap", [0, 0, 1, 0, 16, 19, 0x80000000, 0xffffffff, 0]),
        ("negative_dt_wrap", [5, 41, 1, 0, 0xffffffff, 19, 123, 4, 0]),
        ("signed_subtract_wrap", [0x7fffffff, 41, 1, 0, 0xffffffff, 19, 123, 4, 0]),
        ("fresh_snapshot_after_first_dt", [100, 41, 0, 0, 16, 19, 123, 4, 1]),
        ("turn_captured_countdown", [15, 41, 1, 0, 16, 19, 123, 4, 1]),
    ]


def arm_oracle(original_path: Path, cases):
    from elftools.elf.elffile import ELFFile
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
    from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC

    raw = original_path.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == ORIGINAL_SHA
    with original_path.open("rb") as stream:
        elf = ELFFile(stream)
        segments = [(int(s["p_vaddr"]), int(s["p_filesz"]), int(s["p_memsz"]), int(s["p_offset"]))
                    for s in elf.iter_segments() if s["p_type"] == "PT_LOAD"]

    def image(address, size):
        for base, length, _, offset in segments:
            if base <= address and address + size <= base + length:
                return raw[offset + address - base:offset + address - base + size]
        raise ValueError(f"ELF range missing {address:#x}/{size}")

    def source_word(address):
        return struct.unpack("<I", image(address, 4))[0]

    got_base = (0x3cf408 + source_word(0x3cfbb0)) & 0xffffffff
    records = []
    for name, values in cases:
        countdown, elapsed, turn, disabled, dt1, dt2, seed, counter, mutation = values
        uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        maximum = max(base + memory for base, _, memory, _ in segments)
        uc.mem_map(0, (maximum + 4095) & ~4095)
        for base, length, _, offset in segments:
            uc.mem_write(base, raw[offset:offset + length])
        uc.mem_map(0x10000000, 0x40000)
        actor, app, seed_address, counter_address = 0x10010000, 0x10018000, 0x10019000, 0x10019004

        def put(address, value):
            uc.mem_write(address, struct.pack("<I", value & 0xffffffff))

        def get(address):
            return struct.unpack("<I", uc.mem_read(address, 4))[0]

        # Only the source branch's four global identities are provided. This
        # is a component oracle, not a loaded game or Android page-size test.
        put(got_base + source_word(0x3cfbbc), 0x1001a000)
        put(got_base + source_word(0x3cfbc8), app)
        put(got_base + source_word(0x3cfbe8), seed_address)
        put(got_base + source_word(0x3cfbec), counter_address)
        put(actor + 8, countdown); put(actor + 12, elapsed)
        put(seed_address, seed); put(counter_address, counter)
        observed = {"turn_queries": 0, "debug_queries": 0, "delta_reads": 0}
        ended = []

        def observer(machine, address, _, __):
            if address in (0x3cf430, 0x3cf468):
                ended.append(address); machine.emu_stop()
            elif address == 0x3cc484:
                observed["turn_queries"] += 1
                assert machine.reg_read(UC_ARM_REG_R0) == actor
                machine.reg_write(UC_ARM_REG_R0, turn)
                machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))
            elif address in (0x337888, 0x3140ec):
                # Borrowed debug-loader/std::string dependencies; their
                # allocator/storage bodies are outside this branch projection.
                machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))
            elif address == 0x337a88:
                observed["debug_queries"] += 1
                assert get(actor + 12) == 0
                machine.reg_write(UC_ARM_REG_R0, disabled)
                machine.reg_write(UC_ARM_REG_PC, machine.reg_read(UC_ARM_REG_LR))
            elif address == 0x31f66c:
                # Execute the actual ldr [Application+0x8c]; bx lr leaf.
                index = observed["delta_reads"]; observed["delta_reads"] += 1
                assert machine.reg_read(UC_ARM_REG_R0) == app
                put(app + 0x8c, dt1 if index == 0 else dt2)
                if mutation:
                    if index == 0: put(actor + 8, 999); put(actor + 12, 41)
                    else: put(actor + 12, 999)

        uc.hook_add(UC_HOOK_CODE, observer)
        uc.reg_write(UC_ARM_REG_R4, got_base)
        uc.reg_write(UC_ARM_REG_R5, actor)
        uc.reg_write(UC_ARM_REG_SP, 0x10030000)
        uc.emu_start(0x3cf75c, 0xffffffff, count=2000)
        assert len(ended) == 1, (name, ended)
        decision = 3 if ended[0] == 0x3cf468 else (2 if observed["debug_queries"] else 1)
        record = {"countdown": get(actor + 8), "elapsed": get(actor + 12),
                  "seed": get(seed_address), "counter": get(counter_address),
                  "sync_seed": 0x76543210, "sync_counter": 0x01234567,
                  "decision": decision, **observed,
                  "random_draws": int(get(counter_address) != counter)}
        records.append((name, values, record))
    return records, {
        "original_sha256": ORIGINAL_SHA,
        "executed_scope": "ARM _UpdateAggro timed branch0x3cf75c..0x3cf8c0, exact RNG arithmetic, real GetDt leaf; IsMyTurn/debug query are supplied services; prior owner/player/Faerie branches and later NPC/acquisition bodies excluded",
        "ranges": {f"{address:#x}/{size}": hashlib.sha256(image(address, size)).hexdigest()
                   for address, size in [(0x3cf3f0, 2052), (0x3cc484, 288), (0x31f66c, 8), (0x33ff90, 196)]},
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--compiler")
    parser.add_argument("--c-compiler")
    parser.add_argument("--original-elf", type=Path, required=True)
    parser.add_argument("--output", type=Path, default=MODULE / "build/character-aggro-delay/test.exe")
    parser.add_argument("--report", type=Path, required=True)
    args = parser.parse_args()
    cxx = args.compiler or os.environ.get("CXX") or shutil.which("g++")
    if not cxx: parser.error("pass --compiler")
    cc = args.c_compiler or str(Path(cxx).with_name(Path(cxx).name.replace("g++", "gcc").replace("clang++", "clang")))
    output = args.output.resolve(); output.parent.mkdir(parents=True, exist_ok=True)
    random_obj = output.with_suffix('.random.o')
    c_sources = [ROOT / "port/random/random.c"]
    cpp_sources = [MODULE / "character_aggro_delay.cpp", MODULE / "tests/character_aggro_delay.cpp"]
    commands = [[cc, '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-c', str(c_sources[0]), '-o', str(random_obj)],
                [cxx, '-std=c++17', '-O2', '-Wall', '-Wextra', '-Werror', *(str(p) for p in cpp_sources), str(random_obj), '-o', str(output)]]
    for command in commands: subprocess.run(command, cwd=ROOT, check=True, text=True, capture_output=True)
    guards = json.loads(subprocess.check_output([str(output)], cwd=ROOT, text=True))
    records, reference = arm_oracle(args.original_elf.resolve(), fixtures())
    results = []
    for name, values, expected in records:
        actual = json.loads(subprocess.check_output([str(output), *(str(v) for v in values)], cwd=ROOT, text=True))
        assert actual == expected, (name, actual, expected)
        results.append({"case": name, "input_words": values, "matched": True, "actual": actual})
    dependencies = cpp_sources + c_sources + [MODULE / "character_aggro_delay.hpp", ROOT / "port/random/random.h", Path(__file__).resolve()]
    report = {"validation": "PASS", "original_arm_cases": len(results), "guard_cases": guards['guard_cases'],
              "mismatches": 0, "native_ai_wired": False, "original_oracle": reference,
              "fixture_globals_are_original_startup_values": False, "results": results,
              "compiler_commands": commands,
              "source_sha256": {str(p.relative_to(ROOT)).replace('\\', '/'): hashlib.sha256(p.read_bytes()).hexdigest() for p in dependencies}}
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({k: report[k] for k in ['validation','original_arm_cases','guard_cases','mismatches','native_ai_wired']}))


if __name__ == '__main__':
    main()
