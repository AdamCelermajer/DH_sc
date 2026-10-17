"""Compare the original ARM FAES callback with the native Save projection.

The original __LoadFaeries body and readAs<T> wrappers execute from the pinned
ELF. The virtual byte reader is an explicit provider. Short reads are compared
only through complete native fields; rejecting partial fields is a native
safety policy because the source callback has no recoverable read result.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import struct
import subprocess
import sys
from pathlib import Path

from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_HOOK_CODE
from unicorn.arm_const import (
    UC_ARM_REG_LR,
    UC_ARM_REG_PC,
    UC_ARM_REG_R0,
    UC_ARM_REG_R1,
    UC_ARM_REG_R2,
    UC_ARM_REG_SP,
)

ROOT = Path(__file__).resolve().parents[3]
MODULE = ROOT / "port/game-data"
TEST = MODULE / "tests/player_faeries_reader_host.cpp"
ELF_SHA = "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80"
ENTRY = 0x4691D0
SIZE = 348
SYMBOL = "_ZN14PlayerSavegame13__LoadFaeriesEP11IStreamBasePv"
ASSERT = 0x99F914
SAVE = 0x10001000
STREAM = 0x10003000
VTABLE = 0x10004000
ARRAYS = [0x10008000 + d * 0x1000 for d in range(3)]
STREAM_READ = 0x20001000
STOP = 0x30000000
READERS = [
    (0x38B758, 176),  # IStreamBase::readAs<int32_t>
    (0x313B48, 176),  # IStreamBase::readAs<uint32_t>
    (0x469070, 176),  # IStreamBase::readAs<uint16_t>
    (0x469120, 176),  # IStreamBase::readAs<uint8_t>
]

sys.path.insert(0, str(ROOT / "port/player-info-level/tests"))
from player_locality_v1_original import get, image, put  # noqa: E402


def sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def word(value: int) -> bytes:
    return struct.pack("<I", value & 0xFFFFFFFF)


def block(identifier: int, rows: list[tuple[int, int]]) -> bytes:
    return word(identifier) + word(len(rows)) + b"".join(
        struct.pack("<HB", level & 0xFFFF, state & 0xFF)
        for level, state in rows
    )


ROWS = [
    [(0x1234, 0x11), (0xFFFF, 0xFE), (0, 0), (0x8001, 0x73), (0x4321, 0xA5)],
    [(0xABCD, 0x22), (0x0102, 0x80), (0x7FFF, 0x03), (0x4567, 0xFA), (0x9999, 0x5C)],
    [(0xFEDC, 0x33), (0x2222, 0x40), (0x0001, 0xFF), (0xBEEF, 0x21), (0xC001, 0x7E)],
]
IDS = [-7, 0x123456, -2147483648]
VALID = b"".join(block(IDS[d], ROWS[d]) for d in range(3))
assert len(VALID) == 69


def make_cases() -> list[dict]:
    cases = [dict(name="valid_three_difficulties", initialized=True,
                  payload=VALID, kind="valid")]
    for difficulty in range(3):
        payload = b"".join(block(IDS[d], ROWS[d]) for d in range(difficulty))
        payload += word(0x4000 + difficulty) + word(4)
        payload += b"".join(block(IDS[d], ROWS[d]) for d in range(difficulty + 1, 3))
        cases.append(dict(name=f"count_mismatch_d{difficulty}", initialized=True,
                          payload=payload, kind="mismatch", difficulty=difficulty))
    for prefix in range(len(VALID)):
        cases.append(dict(name=f"truncated_prefix_{prefix}", initialized=True,
                          payload=VALID[:prefix], kind="truncated", prefix=prefix))
    cases.append(dict(name="all_faery_rows_uninitialized", initialized=False,
                      payload=VALID, kind="uninitialized"))
    return cases


class Original:
    def __init__(self, elf: Path):
        data, names, _ = image(elf)
        assert sha(elf) == ELF_SHA
        symbol = names[SYMBOL]
        assert (symbol["st_value"], symbol["st_size"]) == (ENTRY, SIZE)
        self.entry_sha256 = hashlib.sha256(data[ENTRY:ENTRY + SIZE]).hexdigest()
        self.u = Uc(UC_ARCH_ARM, UC_MODE_ARM)
        self.u.mem_map(0, len(data))
        self.u.mem_write(0, data)
        for base, size in [(0x10000000, 0x30000), (0x20000000, 0x10000),
                           (STOP, 0x1000)]:
            self.u.mem_map(base, size)
        self.u.hook_add(UC_HOOK_CODE, self.code)
        self.words: set[int] = set()

    def ret(self, value: int) -> None:
        self.u.reg_write(UC_ARM_REG_R0, value & 0xFFFFFFFF)
        self.u.reg_write(UC_ARM_REG_PC, self.u.reg_read(UC_ARM_REG_LR))

    def code(self, uc, address: int, size: int, context) -> None:
        if address == STOP:
            uc.emu_stop()
            return
        if address == STREAM_READ:
            stream = uc.reg_read(UC_ARM_REG_R0)
            destination = uc.reg_read(UC_ARM_REG_R1)
            request = uc.reg_read(UC_ARM_REG_R2)
            assert stream == STREAM and request in (1, 2, 4)
            available = max(0, len(self.payload) - self.cursor)
            count = min(request, available)
            if count:
                uc.mem_write(destination,
                             self.payload[self.cursor:self.cursor + count])
            self.cursor += count
            self.read_sizes.append(request)
            self.ret(count)
            return
        allowed = [(ENTRY, ENTRY + SIZE),
                   *((start, start + extent) for start, extent in READERS)]
        assert any(lo <= address < hi for lo, hi in allowed), \
            f"unexpected original execution at {address:#x}"
        self.words.add(address)

    def execute(self, case: dict) -> dict:
        self.payload = case["payload"]
        self.cursor = 0
        self.read_sizes: list[int] = []
        uc = self.u
        uc.mem_write(SAVE, bytes(0x198))
        uc.mem_write(0x20000000, bytes(0x10000))
        uc.mem_write(STREAM, word(VTABLE))
        uc.mem_write(VTABLE + 0x18, word(STREAM_READ))
        put(uc, ASSERT, 0)  # Avoid deliberate assertion-log/crash modes.
        for d, base in enumerate(ARRAYS):
            uc.mem_write(base, bytes(20))
            if case["initialized"]:
                put(uc, SAVE + 0x94 + d * 4, base)
                put(uc, SAVE + 0xA0 + d * 4, 5)
        uc.reg_write(UC_ARM_REG_R0, STREAM)
        uc.reg_write(UC_ARM_REG_R1, SAVE)
        uc.reg_write(UC_ARM_REG_SP, 0x20008000)
        uc.reg_write(UC_ARM_REG_LR, STOP)
        uc.emu_start(ENTRY, STOP + 4, count=100000)
        assert uc.reg_read(UC_ARM_REG_PC) == STOP
        current = list(struct.unpack("<3i", bytes(uc.mem_read(SAVE + 0xAC, 12))))
        faeries = []
        for base in ARRAYS:
            for row in range(5):
                raw = bytes(uc.mem_read(base + row * 4, 4))
                faeries.append((raw[0], struct.unpack_from("<H", raw, 2)[0]))
        return dict(cursor=self.cursor, read_sizes=list(self.read_sizes),
                    current=current, faeries=faeries)


def read_native(path: Path, expected_count: int) -> list[dict]:
    raw = path.read_bytes()
    record_size = 69
    assert len(raw) == expected_count * record_size, (len(raw), expected_count)
    result = []
    at = 0
    for _ in range(expected_count):
        status, consumed, mismatch = struct.unpack_from("<III", raw, at)
        at += 12
        current = list(struct.unpack_from("<3i", raw, at))
        at += 12
        faeries = []
        for _ in range(15):
            state, level = struct.unpack_from("<BH", raw, at)
            at += 3
            faeries.append((state, level))
        result.append(dict(failed=bool(status), consumed=consumed,
                           mismatch=bool(mismatch), current=current,
                           faeries=faeries))
    return result


def projection(original: dict, native: dict) -> None:
    assert original["cursor"] == native["consumed"], (original, native)
    assert original["current"] == native["current"], (original, native)
    assert original["faeries"] == native["faeries"], (original, native)


def compare_completed_prefix_fields(original: dict, native: dict) -> int:
    # Native bounds checks reject an incomplete scalar before advancing its
    # cursor. Only fields fully reached by the native parser are compared.
    end = native["consumed"]
    cursor = 0
    compared = 0
    for d in range(3):
        cursor += 4
        if cursor <= end:
            assert original["current"][d] == native["current"][d]
            compared += 1
        cursor += 4  # count word
        for row in range(5):
            cursor += 2
            if cursor <= end:
                index = d * 5 + row
                assert original["faeries"][index][1] == native["faeries"][index][1]
                compared += 1
            cursor += 1
            if cursor <= end:
                index = d * 5 + row
                assert original["faeries"][index][0] == native["faeries"][index][0]
                compared += 1
    return compared


def run(command: list[str], env=None) -> str:
    result = subprocess.run(command, capture_output=True, text=True, env=env)
    if result.returncode:
        raise RuntimeError("command failed: " + " ".join(command) + "\n" +
                           result.stdout + result.stderr)
    return result.stdout.strip()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--original-elf", type=Path,
                        default=ROOT / ".local-inputs/libDungeonHunter2.so")
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--library", type=Path,
                        help="selected game-data shared library; direct source build otherwise")
    parser.add_argument("--output", type=Path,
                        default=Path("C:/tmp/dh2-faes-reader-differential"))
    args = parser.parse_args()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    cases = make_cases()
    original = Original(args.original_elf.resolve())
    gold = [original.execute(case) for case in cases]

    input_path = output / "fixtures.bin"
    native_path = output / "native.bin"
    executable = output / "player_faeries_reader_host.exe"
    with input_path.open("wb") as stream:
        stream.write(word(len(cases)))
        for case in cases:
            stream.write(word(int(case["initialized"])))
            stream.write(word(len(case["payload"])))
            stream.write(case["payload"])

    compiler = str(Path(args.compiler).resolve()) if Path(args.compiler).exists() else args.compiler
    command = [compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror", str(TEST)]
    if args.library:
        command.append(str(args.library.resolve()))
    else:
        sources = [
            "player_savegame_v1.cpp",
            "player_save_level_states_v1.cpp",
            "player_saved_level_states_v1.cpp",
            "player_saved_fast_travel_v1.cpp",
            "quest_savegame_v1.cpp",
            "player_saved_quests_v1.cpp",
            "quest_runtime_fields_v1.cpp",
        ]
        command.extend(str(MODULE / source) for source in sources)
    command.extend(["-o", str(executable)])
    run(command)
    env = os.environ.copy()
    selected_dlls: list[Path] = []
    if args.library:
        selected_dlls = sorted(args.library.resolve().parent.parent.rglob("*.dll"))
        env["PATH"] = os.pathsep.join(
            [*(str(path.parent) for path in selected_dlls), env.get("PATH", "")])
    policy_checks = int(run([str(executable), str(input_path), str(native_path)], env))
    native = read_native(native_path, len(cases))

    compared_prefix_fields = 0
    raw_truncation_differences = 0
    for index, (case, old, new) in enumerate(zip(cases, gold, native)):
        if case["kind"] == "valid":
            assert not new["failed"] and not new["mismatch"]
            projection(old, new)
            assert old["cursor"] == 69 and old["read_sizes"] == [4, 4, *([2, 1] * 5)] * 3
        elif case["kind"] == "mismatch":
            d = case["difficulty"]
            expected_cursor = d * 23 + 8
            expected_reads = d * 12 + 2
            assert not new["failed"] and new["mismatch"], (index, case, new)
            projection(old, new)
            assert old["cursor"] == expected_cursor, (d, old["cursor"])
            assert len(old["read_sizes"]) == expected_reads
        elif case["kind"] == "truncated":
            assert old["cursor"] == case["prefix"], (case, old["cursor"])
            assert new["failed"] and new["consumed"] <= case["prefix"], (case, new)
            compared_prefix_fields += compare_completed_prefix_fields(old, new)
            if old["current"] != new["current"] or old["faeries"] != new["faeries"]:
                raw_truncation_differences += 1
        else:
            assert old["cursor"] == 0 and not old["read_sizes"]
            assert new["failed"] and new["consumed"] == 0
            assert not any(old["current"]) and not any(new["current"])
            assert all(state == 0 and level == 0
                       for state, level in old["faeries"] + new["faeries"])

    report = {
        "validation": "PASS",
        "cases": len(cases),
        "valid_payload_cases": 1,
        "count_mismatch_cases": 3,
        "truncated_prefixes": len(VALID),
        "uninitialized_policy_cases": 1,
        "native_policy_checks": policy_checks,
        "original_elf_sha256": sha(args.original_elf.resolve()),
        "original_symbol": SYMBOL,
        "original_entry": hex(ENTRY),
        "original_size": SIZE,
        "original_entry_sha256": original.entry_sha256,
        "original_instruction_addresses": len(original.words),
        "selected_library": str(args.library.resolve()) if args.library else None,
        "native_implementation": "selected game-data library" if args.library else "direct production translation units",
        "valid_and_mismatch_projection_parity": True,
        "mismatch_behavior": "reader exits at mismatch after ID+count; later difficulty remains unread",
        "truncated_native_policy": "reject incomplete field; compare all fields fully consumed by native; source readAs<T> continues after short reads under gAssertLevel=0",
        "truncated_complete_projection_fields_compared": compared_prefix_fields,
        "truncated_prefixes_with_raw_projection_differences": raw_truncation_differences,
        "uninitialized_policy": "original skips null row pointers with assertion mode 0 and consumes no bytes; native rejects before consuming",
        "relevant_source_sha256": {
            "native_method": sha(MODULE / "player_savegame_v1.cpp"),
            "native_header": sha(MODULE / "player_savegame_v1.hpp"),
            "host_fixture": sha(TEST),
            "runner": sha(Path(__file__).resolve()),
        },
        "scope": "Original ARM __LoadFaeries and four IStreamBase::readAs<T> wrappers execute. The stream vtable read provider supplies bounded bytes and source gAssertLevel is zero. InitFaeries allocation and live SG_Load/Transport route are outside this test.",
    }
    report_path = output / "report.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({key: value for key, value in report.items()
                      if key not in ("relevant_source_sha256",)}))


if __name__ == "__main__":
    main()
