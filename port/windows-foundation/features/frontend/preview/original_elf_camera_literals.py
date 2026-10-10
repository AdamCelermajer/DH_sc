"""Verify camera ctor MOVW/MOVT constants in original ELF32 ARM bytes."""
import hashlib
import struct
import sys
from pathlib import Path

source = Path(sys.argv[1])
data = source.read_bytes()
if data[:6] != b"\x7fELF\x01\x01":
    raise RuntimeError("Expected original ELF32 little-endian")
phoff = struct.unpack_from("<I", data, 28)[0]
phentsize, phnum = struct.unpack_from("<HH", data, 42)
segments = []
for i in range(phnum):
    kind, offset, address, _, size, _, _, _ = struct.unpack_from("<8I", data, phoff + i * phentsize)
    if kind == 1:
        segments.append((address, offset, size))

def read(address):
    for base, offset, size in segments:
        if base <= address and address + 4 <= base + size:
            return struct.unpack_from("<I", data, offset + address - base)[0]
    raise RuntimeError("Address outside original ELF load segment")

def constant(address):
    low, high = read(address), read(address + 4)
    if low & 0x0FF0F000 != 0x03003000 or high & 0x0FF0F000 != 0x03403000:
        raise RuntimeError("Expected original MOVW/MOVT r3 pair")
    def immediate(instruction):
        return ((instruction >> 4) & 0xF000) | (instruction & 0xFFF)
    return immediate(low) | (immediate(high) << 16)

for address, store, offset, expected in ((0x583830, 0x583838, 0x154, 1067506044),
                                         (0x58383C, 0x583848, 0x158, 1068149419)):
    word = constant(address)
    if word != expected or read(store) != 0xE5843000 | offset:
        raise RuntimeError("Original camera constant/store differs")
    print(f"ARM {address:#x}: MOVW/MOVT r3 bits {word:#x} "
          f"float {struct.unpack('<f', struct.pack('<I', word))[0]} "
          f"-> STR at {store:#x} object+{offset:#x}")
print("PASS original ctor constants and stores; ELF SHA256", hashlib.sha256(data).hexdigest())
