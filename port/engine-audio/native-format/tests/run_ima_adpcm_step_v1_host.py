#!/usr/bin/env python3
"""Compile and run the isolated, source-backed IMA nibble primitive tests."""
from __future__ import annotations

import argparse
import hashlib
from pathlib import Path
import struct
import subprocess

AREA = Path(__file__).resolve().parents[1]


def verify_elf(path: Path) -> None:
    data = path.read_bytes()
    if hashlib.sha256(data).hexdigest() != "36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80":
        raise ValueError("original ELF SHA-256 mismatch")
    if data[:7] != b"\x7fELF\x01\x01\x01":
        raise ValueError("expected ELF32 little-endian source")
    phoff = struct.unpack_from("<I", data, 28)[0]
    phentsize, phnum = struct.unpack_from("<HH", data, 42)
    loads = []
    for i in range(phnum):
        row = struct.unpack_from("<8I", data, phoff + i * phentsize)
        if row[0] == 1:
            loads.append((row[2], row[1], row[4]))

    def read_va(address: int, length: int) -> bytes:
        for va, file_offset, size in loads:
            if va <= address and address + length <= va + size:
                start = file_offset + address - va
                return data[start:start + length]
        raise ValueError(f"VA outside file-backed PT_LOAD: {address:#x}")

    decoder = read_va(0x887068, 1020)
    literal = struct.unpack("<I", read_va(0x887458, 4))[0]
    got = 0x8870DC + 8 + literal  # ARM PC for `add r1, pc, r1` in DecodeBlock.
    index_va = struct.unpack("<I", read_va(got + 0x2160, 4))[0]
    step_va = struct.unpack("<I", read_va(got + 0x3894, 4))[0]
    index_table = read_va(index_va, 16)
    step_table = read_va(step_va, 178)
    expected = {
        "decoder": "35db31f76e36891a2d3e3055848f65ce83e0d8d40948410798b096cd5ebdaebe",
        "index table": "54eeb55fd7262bd30a093d8219411929ddd8367f2e447f4e882f4276d6fa11ff",
        "step table": "98d29d10eef87b44f37e4e6a0410e9f3db98eaf56579d1189c7e3317e63bc3f3",
    }
    for name, body in (("decoder", decoder), ("index table", index_table), ("step table", step_table)):
        if hashlib.sha256(body).hexdigest() != expected[name]:
            raise ValueError(f"original {name} SHA-256 mismatch")
    if (index_va, step_va) != (0x911A14, 0x911A24):
        raise ValueError(f"unexpected resolved lookup table VAs: {index_va:#x}, {step_va:#x}")
    print("original ELF verified: DecodeBlock + IMA tables match pinned hashes")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--compiler", default="g++")
    parser.add_argument("--build-dir", type=Path, required=True)
    parser.add_argument("--original-elf", type=Path)
    args = parser.parse_args()
    if args.original_elf:
        verify_elf(args.original_elf)
    args.build_dir.mkdir(parents=True, exist_ok=True)
    exe = args.build_dir / "ima_adpcm_step_v1_host.exe"
    source = AREA / "tests/ima_adpcm_step_v1_host.cpp"
    subprocess.run([args.compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror",
                    str(source), "-o", str(exe)], check=True)
    subprocess.run([str(exe)], check=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
