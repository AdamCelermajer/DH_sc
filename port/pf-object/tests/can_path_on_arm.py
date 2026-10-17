#!/usr/bin/env python3
"""Exercise the original ARM32 PFObject path-mask leaf routines in Unicorn.

The harness loads the original Android ELF and calls the native instructions
with minimal synthetic PFObject/PFFloor storage. It does not instantiate a game
world or test navigation integration.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM
from unicorn.arm_const import (
    UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R0, UC_ARM_REG_R1,
    UC_ARM_REG_R2, UC_ARM_REG_R12, UC_ARM_REG_SP,
)


CAN_PATH_ON = 0x00524230
SET_FLYING = 0x005241F4
SET_SWIMMING = 0x00524218
CTOR_C2 = 0x00524538
CTOR_MASK_INIT = 0x00524560  # Constructor basic-block start, after GOT setup.
CTOR_MASK_END = 0x00524580   # Immediately after the STR at object + 0x14.
OBJECT_OFFSET_PATH_MASK = 0x14
FLOOR_OFFSET_TYPE_MASK = 0x24


class OriginalArm:
    def __init__(self, path):
        self.path = path.resolve()
        with self.path.open('rb') as stream:
            elf = ELFFile(stream)
            if elf.elfclass != 32 or elf.header['e_machine'] != 'EM_ARM':
                raise AssertionError('Expected an ARM32 original library')
            segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
            low = min(s['p_vaddr'] for s in segments) & ~0xFFF
            high = (max(s['p_vaddr'] + s['p_memsz'] for s in segments) + 0xFFF) & ~0xFFF
            self.uc = Uc(UC_ARCH_ARM, UC_MODE_ARM)
            self.uc.mem_map(low, high - low)
            for segment in segments:
                self.uc.mem_write(segment['p_vaddr'], segment.data())
            self.function_sizes = {}
            for section_name in ('.dynsym', '.symtab'):
                section = elf.get_section_by_name(section_name)
                if section is None:
                    continue
                for symbol in section.iter_symbols():
                    if symbol['st_shndx'] != 'SHN_UNDEF':
                        self.function_sizes[symbol.name] = (
                            int(symbol['st_value']), int(symbol['st_size']))
        self.sha256 = hashlib.sha256(self.path.read_bytes()).hexdigest()
        self.data = (high + 0x10000 + 0xFFFF) & ~0xFFFF
        self.stack = self.data + 0x10000
        self.return_to = self.data + 0x20000
        self.uc.mem_map(self.data, 0x10000)
        self.uc.mem_map(self.stack, 0x10000)
        self.uc.mem_map(self.return_to, 0x1000)
        self.object = self.data + 0x1000
        self.floor = self.data + 0x2000

    def write_word(self, address, value):
        self.uc.mem_write(address, struct.pack('<I', value & 0xFFFFFFFF))

    def read_word(self, address):
        return struct.unpack('<I', self.uc.mem_read(address, 4))[0]

    def call(self, address, registers):
        entry_sp = self.stack + 0xF000
        self.uc.reg_write(UC_ARM_REG_SP, entry_sp)
        self.uc.reg_write(UC_ARM_REG_LR, self.return_to)
        for reg, value in registers.items():
            self.uc.reg_write(reg, value)
        self.uc.emu_start(address, self.return_to, count=1000)
        if self.uc.reg_read(UC_ARM_REG_PC) != self.return_to:
            raise AssertionError(f'ARM routine {address:#x} did not return')
        if self.uc.reg_read(UC_ARM_REG_SP) != entry_sp:
            raise AssertionError(f'ARM routine {address:#x} did not restore SP')
        return self.uc.reg_read(UC_ARM_REG_R0) & 0xFFFFFFFF

    def ctor_mask_block(self):
        """Run actual constructor instructions through the path-mask store."""
        entry_sp = self.stack + 0xF000
        self.uc.reg_write(UC_ARM_REG_SP, entry_sp)
        self.uc.reg_write(UC_ARM_REG_R0, self.object)
        # The preceding constructor instructions set r2 and ip/r12 to zero.
        self.uc.reg_write(UC_ARM_REG_R2, 0)
        self.uc.reg_write(UC_ARM_REG_R12, 0)
        self.uc.emu_start(CTOR_MASK_INIT, CTOR_MASK_END, count=100)
        if self.uc.reg_read(UC_ARM_REG_PC) != CTOR_MASK_END:
            raise AssertionError('Constructor mask initialization block did not stop at its boundary')
        if self.uc.reg_read(UC_ARM_REG_SP) != entry_sp:
            raise AssertionError('Constructor mask initialization block changed SP')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--original', required=True, type=Path,
                        help='Path to the original armeabi-v7a libDungeonHunter2.so')
    args = parser.parse_args()
    arm = OriginalArm(args.original)
    can_path_calls = 0
    setter_calls = 0
    checks = 0

    # This executes original constructor instructions from MOV r1,#2 through
    # STR r1,[r0,#0x14]. It deliberately stops before later GOT/list setup and
    # the constructor call at 0x524628, which are irrelevant to this field.
    arm.uc.mem_write(arm.object, b'\xa5' * 0xA8)
    arm.ctor_mask_block()
    assert arm.read_word(arm.object + OBJECT_OFFSET_PATH_MASK) == 2
    checks += 1

    def can_path(object_mask, floor_mask):
        nonlocal can_path_calls
        arm.write_word(arm.object + OBJECT_OFFSET_PATH_MASK, object_mask)
        if floor_mask is None:
            floor_ptr = 0
        else:
            arm.write_word(arm.floor + FLOOR_OFFSET_TYPE_MASK, floor_mask)
            floor_ptr = arm.floor
        result = arm.call(CAN_PATH_ON, {
            UC_ARM_REG_R0: arm.object,
            UC_ARM_REG_R1: floor_ptr,
        })
        can_path_calls += 1
        expected = 0 if floor_mask is None else int(
            floor_mask == 0 or (floor_mask & object_mask) == floor_mask)
        assert result == expected, (object_mask, floor_mask, result, expected)
        return result

    # Constructor default: object supports swimming (bit 1), not flying (bit 0).
    for required in (None, 0, 1, 2, 3, 4, 6, 0x80000000):
        can_path(2, required)
    checks += 8

    # Native setters toggle bits 0 (flying) and 1 (swimming), preserving others.
    arm.write_word(arm.object + OBJECT_OFFSET_PATH_MASK, 2)
    for address, value, expected_mask in (
        (SET_FLYING, 1, 3), (SET_FLYING, 1, 3),
        (SET_SWIMMING, 0, 1), (SET_SWIMMING, 0, 1),
        (SET_FLYING, 0, 0), (SET_SWIMMING, 1, 2),
        (SET_FLYING, 1, 3), (SET_SWIMMING, 0, 1),
    ):
        arm.call(address, {UC_ARM_REG_R0: arm.object, UC_ARM_REG_R1: value})
        setter_calls += 1
        actual = arm.read_word(arm.object + OBJECT_OFFSET_PATH_MASK)
        assert actual == expected_mask, (hex(address), value, actual, expected_mask)
        checks += 1
        # Both floor bits are checked across each native state transition.
        can_path(actual, 1)
        can_path(actual, 2)
        checks += 2

    # Exhaust all small flag combinations against required-mask subset semantics.
    for object_mask in range(16):
        for floor_mask in range(16):
            can_path(object_mask, floor_mask)
            checks += 1
    # Zero required mask is unconditional even for an all-zero object mask.
    for object_mask in (0, 1, 2, 3, 0xFFFFFFFF):
        can_path(object_mask, 0)
        checks += 1

    print(json.dumps({
        'original_sha256': arm.sha256,
        'can_path_on_address': hex(CAN_PATH_ON),
        'set_flying_address': hex(SET_FLYING),
        'set_swimming_address': hex(SET_SWIMMING),
        'constructor_c2_address': hex(CTOR_C2),
        'constructor_mask_block': [hex(CTOR_MASK_INIT), hex(CTOR_MASK_END)],
        'constructor_path_mask': 2,
        'can_path_on_calls': can_path_calls,
        'setter_calls': setter_calls,
        'assertions': checks,
        'mismatches': 0,
        'result': 'PASS',
        'scope': 'original ARM32 leaf code with synthetic object/floor storage; no game/world integration',
        'constructor_limit': 'executed the actual constructor basic block through STR [object+0x14], not the full constructor',
    }, indent=2))


if __name__ == '__main__':
    main()
