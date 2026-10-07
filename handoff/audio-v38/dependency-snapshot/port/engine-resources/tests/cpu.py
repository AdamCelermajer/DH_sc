"""Minimal ELF/Unicorn loader shared by the resource validation scripts.

Original engine routines and compiled ARM64 routines execute real instructions.
Only imported libc memcpy/memset and the caller's ARM32 completion callback are
modeled. Unknown imports fail immediately.
"""
import struct
from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_ARM, UC_ARCH_ARM64, UC_MODE_ARM, UC_HOOK_CODE, UC_HOOK_BLOCK
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_LR, UC_ARM_REG_SP, UC_ARM_REG_PC
from unicorn.arm64_const import UC_ARM64_REG_X0, UC_ARM64_REG_LR, UC_ARM64_REG_SP, UC_ARM64_REG_PC

def u32(value):
    return value & 0xffffffff

def i32(value):
    v = u32(value)
    return v if v < 0x80000000 else v - 0x100000000

class Cpu:
    def __init__(self, path, arm64, provenance):
        self.arm64 = arm64
        self.uc = Uc(UC_ARCH_ARM64 if arm64 else UC_ARCH_ARM, UC_MODE_ARM)
        self.base = 0x100000000 if arm64 else 0
        self.data = 0x200000000 if arm64 else 0x2000000
        self.stack = 0x400000000 if arm64 else 0x4000000
        self.stop = 0x500000000 if arm64 else 0x5000000
        self.extern = self.stop + 0x10000
        self.word_size = 8 if arm64 else 4
        self.pointer_format = '<Q' if arm64 else '<I'
        self.reg0 = UC_ARM64_REG_X0 if arm64 else UC_ARM_REG_R0
        self.lr = UC_ARM64_REG_LR if arm64 else UC_ARM_REG_LR
        self.sp = UC_ARM64_REG_SP if arm64 else UC_ARM_REG_SP
        self.pc = UC_ARM64_REG_PC if arm64 else UC_ARM_REG_PC
        self.symbols, self.imports, self.import_calls, self.seen = {}, {}, {}, set()
        self.address_owner = {}
        for row in provenance['functions']:
            start = int(row['elf_address'], 16)
            for address in range(start, start + row['size'], 4):
                self.address_owner[address] = row['original_symbol']
        with path.open('rb') as stream:
            elf = ELFFile(stream)
            loads = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
            low = min(s['p_vaddr'] for s in loads) & ~4095
            high = (max(s['p_vaddr'] + s['p_memsz'] for s in loads) + 4095) & ~4095
            self.uc.mem_map(self.base + low, high - low)
            for s in loads:
                self.uc.mem_write(self.base + s['p_vaddr'], s.data())
            for name in ('.dynsym', '.symtab'):
                sec = elf.get_section_by_name(name)
                if sec:
                    for symbol in sec.iter_symbols():
                        if symbol['st_shndx'] != 'SHN_UNDEF':
                            self.symbols[symbol.name] = self.base + symbol['st_value']
            for sec in elf.iter_sections():
                if sec['sh_type'] not in ('SHT_REL', 'SHT_RELA'):
                    continue
                syms = elf.get_section(sec['sh_link'])
                for r in sec.iter_relocations():
                    kind = r['r_info_type']
                    if arm64 and kind == 1027:
                        self.pointer(self.base + r['r_offset'], self.base + r['r_addend'])
                    elif kind == (1026 if arm64 else 22):
                        s = syms.get_symbol(r['r_info_sym'])
                        if s['st_shndx'] != 'SHN_UNDEF':
                            target = self.symbols[s.name]
                        else:
                            target = self.extern + len(self.imports) * 16
                            self.imports[target] = s.name
                        self.pointer(self.base + r['r_offset'], target)
        self.uc.mem_map(self.data, 0x2000000)
        self.uc.mem_map(self.stack, 0x10000)
        self.uc.mem_map(self.stop, 0x1000)
        self.uc.mem_map(self.extern, 0x10000)
        self.callback = self.extern + 0xff00
        self.imports[self.callback] = 'caller_completion'
        self.uc.hook_add(UC_HOOK_CODE, self.external, begin=self.extern, end=self.extern + 0xffff)
        self.coverage_hook = None
        if not arm64:
            self.coverage_hook = self.uc.hook_add(UC_HOOK_BLOCK, self.executed)

    def pointer(self, address, value):
        self.uc.mem_write(address, struct.pack(self.pointer_format, value))

    def reg(self, i):
        return self.uc.reg_read(self.reg0 + i)

    def put(self, i, value):
        self.uc.reg_write(self.reg0 + i, value & (0xffffffffffffffff if self.arm64 else 0xffffffff))

    def executed(self, uc, address, size, unused):
        self.seen.update(a for a in range(address, address + size, 4) if a in self.address_owner)

    def external(self, uc, address, size, unused):
        name = self.imports.get(address)
        self.import_calls[name] = self.import_calls.get(name, 0) + 1
        if name in ('memcpy', 'memmove', '__aeabi_memcpy', '__aeabi_memcpy4'):
            dst, src, n = [self.reg(i) for i in range(3)]
            if n > 0x2000000:
                raise AssertionError('Unsafe original memcpy count')
            if n:
                self.uc.mem_write(dst, bytes(self.uc.mem_read(src, n)))
            self.put(0, dst)
        elif name == 'memset':
            dst, value, n = [self.reg(i) for i in range(3)]
            self.uc.mem_write(dst, bytes([value & 255]) * n)
            self.put(0, dst)
        elif name == 'caller_completion':
            n, error, file, user = [self.reg(i) for i in range(4)]
            # Caller fixture, not engine behavior. File is the original memory
            # reader; the ARM64 callback executes the actual compiled fixture.
            cursor = struct.unpack('<i', self.uc.mem_read(file + 0x1c, 4))[0]
            calls = struct.unpack('<I', self.uc.mem_read(user + 16, 4))[0]
            self.uc.mem_write(user, struct.pack('<IIIIIi', u32(n), u32(error), file, user, calls + 1, cursor))
        else:
            raise AssertionError('Unmodeled dependency executed: ' + str(name))
        self.uc.reg_write(self.pc, self.uc.reg_read(self.lr))

    def invoke(self, symbol, args, stack=(), budget=1000000):
        entry_sp = self.stack + 0xe000
        self.uc.reg_write(self.sp, entry_sp)
        self.uc.reg_write(self.lr, self.stop)
        register_count = 8 if self.arm64 else 4
        for i, v in enumerate(args[:register_count]):
            self.put(i, v)
        spilled = tuple(args[register_count:]) + tuple(stack)
        if spilled:
            self.uc.mem_write(entry_sp, struct.pack('<' + ('Q' if self.arm64 else 'I') * len(spilled),
                                                  *(v & ((1 << (self.word_size * 8)) - 1) for v in spilled)))
        address = self.symbols[symbol] if isinstance(symbol, str) else symbol
        self.uc.emu_start(address, self.stop, count=budget)
        if self.uc.reg_read(self.pc) != self.stop or self.uc.reg_read(self.sp) != entry_sp:
            raise AssertionError('Routine failed to return/restore stack: ' + str(symbol))
        return self.reg(0)
