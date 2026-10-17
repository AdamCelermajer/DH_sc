"""Execute original row readers on the supplied cache with named stream primitives.

Top-level row-reader instructions run from the pinned ELF. Byte-stream reads,
allocation/free and readStringEx are modeled dependencies; their bodies and
the original object ABI/allocator are not reconstructed by the native table.
"""
import hashlib
import struct
import sys
from pathlib import Path
from unicorn import UC_HOOK_CODE

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT/'port/engine-resources/tests'))
from cpu import Cpu

EXPECTED_ELF = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
FUNCTIONS = [
    {'original_symbol': '_ZN7Structs16LevelDeclaration4readEP11IStreamBase', 'elf_address': '0x4fca84', 'size': 1388},
    {'original_symbol': '_ZN7Structs21FastTravelDestination4readEP11IStreamBase', 'elf_address': '0x4fcff0', 'size': 556},
]
PRIMITIVES = {0x4db89c: 'read_bool', 0x3df1a0: 'read_unsigned', 0x459090: 'read_signed',
              0x317454: 'read_string_ex', 0x31056c: 'allocate_array', 0x310440: 'free_array'}
OWNER, STREAM, HEAP = 0x2100000, 0x2200000, 0x2600000


class RowCpu(Cpu):
    def __init__(self, original, wire):
        super().__init__(original, False, {'functions': FUNCTIONS})
        for row in FUNCTIONS:
            assert self.symbols[row['original_symbol']] == int(row['elf_address'], 16)
        self.wire, self.cursor, self.heap, self.calls = wire, 0, HEAP, []
        for address in PRIMITIVES:
            self.uc.hook_add(UC_HOOK_CODE, self.primitive, begin=address, end=address)

    def take(self, size):
        assert 0 <= size <= len(self.wire)-self.cursor
        raw = self.wire[self.cursor:self.cursor+size]
        self.cursor += size
        return raw

    def primitive(self, uc, address, size, unused):
        operation = PRIMITIVES[address]
        self.calls.append(operation)
        if operation.startswith('read_'):
            assert self.reg(0) == STREAM
            if operation == 'read_bool':
                uc.mem_write(self.reg(1), self.take(1))
            elif operation in ('read_unsigned', 'read_signed'):
                uc.mem_write(self.reg(1), self.take(4))
            else:
                assert self.reg(3) == 0
                amount = self.reg(2)
                if amount: uc.mem_write(self.reg(1), self.take(amount))
        elif operation == 'allocate_array':
            amount = self.reg(0)
            assert 0 < amount <= 4097
            self.put(0, self.heap)
            uc.mem_write(self.heap, b'\0'*amount)
            self.heap += (amount+15)&~15
        elif operation == 'free_array':
            assert HEAP <= self.reg(0) < self.heap
        else:
            raise AssertionError(operation)
        uc.reg_write(self.pc, uc.reg_read(self.lr))

    def word(self, at):
        return struct.unpack('<i', bytes(self.uc.mem_read(OWNER+at, 4)))[0]

    def string(self, length_at, pointer_at):
        length = self.word(length_at)
        pointer = struct.unpack('<I', bytes(self.uc.mem_read(OWNER+pointer_at, 4)))[0]
        assert length >= 0 and bytes(self.uc.mem_read(pointer+length, 1)) == b'\0'
        return bytes(self.uc.mem_read(pointer, length)).decode('utf-8')

    def row(self, function, name):
        self.uc.mem_write(OWNER, b'\0'*72)
        self.heap = HEAP
        begin, before = self.cursor, len(self.calls)
        self.invoke(FUNCTIONS[function]['original_symbol'], [OWNER, STREAM])
        result = {'name': name}
        if function == 1:
            result.update(description_id=self.word(4), entrypoint_id=self.word(8),
                          level_name=self.string(0xc, 0x10), location_type=self.word(0x14), string_id=self.word(0x18))
        else:
            result.update(dbg_is_stable=bool(self.uc.mem_read(OWNER+4, 1)[0]), dynamic_bus_routing=self.string(8, 0xc),
                          hub=self.word(0x10), is_random=bool(self.uc.mem_read(OWNER+0x14, 1)[0]),
                          level_description=self.word(0x18), level_file=self.string(0x1c, 0x20))
            fields = ('level_name_id', 'level_state', 'map_name', 'monster_lvl_max', 'monster_lvl_max_hard',
                      'monster_lvl_max_nightmare', 'monster_lvl_min', 'monster_lvl_min_hard', 'monster_lvl_min_nightmare')
            result.update({field: self.word(0x24+n*4) for n, field in enumerate(fields)})
        return result, {'wire_begin': begin, 'wire_end': self.cursor, 'dependencies': self.calls[before:]}


def compare(original, wire, host):
    raw = original.read_bytes()
    assert hashlib.sha256(raw).hexdigest() == EXPECTED_ELF
    cpu = RowCpu(original, wire)
    traces = []
    for table, function in (('fast_travel', 1), ('levels', 0)):
        count = struct.unpack('<I', cpu.take(4))[0]
        assert count == len(host[table])
        for expected in host[table]:
            actual, trace = cpu.row(function, expected['name'])
            assert actual == expected, (expected['name'], actual, expected)
            traces.append({'name': expected['name'], **trace})
    assert cpu.cursor == len(wire)
    # ELF bytes are independently read from loaded original memory.
    manifest = [{**row, 'sha256': hashlib.sha256(bytes(cpu.uc.mem_read(int(row['elf_address'], 16), row['size']))).hexdigest()}
                for row in FUNCTIONS]
    return {'original_arm_record_comparisons': len(traces), 'mismatches': 0,
            'original_elf_sha256': EXPECTED_ELF, 'functions': manifest, 'record_traces': traces,
            'instruction_addresses_executed': len(cpu.seen), 'dependency_bodies_executed': 0, 'scope': __doc__}
