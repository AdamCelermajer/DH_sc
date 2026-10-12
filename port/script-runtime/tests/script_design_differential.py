"""Execute original design wrappers against optimized ARM64 callbacks.

Application design lookup and ReturnValues allocation are explicit services.
This proves wrapper guards, shared Struct/OID dispatch and signed integer
projection; it does not claim the full design-table manager is reconstructed.
"""
import argparse
import hashlib
import itertools
import json
import re
import struct
import sys
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'port/level-world/tests'))
from character_script_selection_differential import Cpu, string

ADDRESSES = [0x37f354, 0x37f4a8, 0x37f5fc]
NAMES = ['constant', 'struct', 'oid']
EXPECTED = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def bits(value):
    return struct.unpack('<I', struct.pack('<f', value))[0]


def signed(value):
    return value if value < 0x80000000 else value - 0x100000000


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--library', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    if (args.output / 'differential.json').exists():
        raise RuntimeError('Refusing to replace design binding proof')
    engine = ROOT / '.local-inputs/libDungeonHunter2.so'
    assert sha(engine) == EXPECTED
    captured, assembly, raw_functions = [], [], {}
    with engine.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = list(elf.get_section_by_name('.symtab').iter_symbols())
        segments = [s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
        addresses = ADDRESSES + [0x4bd640, 0x4c4bdc, 0x4bd4d0, 0x4c4998,
                                 0x414484, 0x31c49c, 0x37baf8, 0x37cb24,
                                 0x4af110, 0x4bdccc, 0x4be550]
        for address in addresses:
            symbol = next(s for s in symbols if s['st_value'] == address and s['st_size'])
            segment = next(s for s in segments
                           if s['p_vaddr'] <= address < s['p_vaddr'] + s['p_filesz'])
            stream.seek(segment['p_offset'] + address - segment['p_vaddr'])
            raw = stream.read(symbol['st_size'])
            raw_functions[address] = raw
            captured.append(dict(original_symbol=symbol.name, elf_address=hex(address),
                                 size=len(raw), sha256=hashlib.sha256(raw).hexdigest()))
            assembly.append('\n# ' + symbol.name)
            assembly.extend(f'{i.address:08x}: {i.mnemonic:8} {i.op_str}'
                            for i in Cs(CS_ARCH_ARM, CS_MODE_ARM).disasm(raw, address))
    manifest = dict(original_sha256=EXPECTED, functions=captured)
    (args.output / 'original-functions.json').write_text(json.dumps(manifest, indent=2)+'\n')
    (args.output / 'original-functions.asm').write_text('\n'.join(assembly)+'\n')
    old = Cpu(engine, False, manifest)
    native = Cpu(args.library, True, {'functions': []})
    oa, ov, oargs, oresults, app, manager = [old.data + x for x in
        (0x1000, 0x2000, 0x10000, 0x20000, 0x30000, 0x31000)]
    na, nout, ncount, ns, callback = [native.data+x for x in
        (0x1000, 0x10000, 0x11000, 0x12000, 0x13000)]
    text_old = [old.data+0x40000, old.data+0x41000]
    text_new = [native.data+0x40000, native.data+0x41000]
    for pointers, cpu in ((text_old, old), (text_new, native)):
        for pointer, value in zip(pointers, (b'CharacterProperties', b'SkillTree')):
            cpu.uc.mem_write(pointer, value+b'\0')
    old.pointer(oa+4, ov)
    native.uc.mem_write(ns, struct.pack('<3Q', 0xabcdef0123456789, callback, 0))
    native.uc.mem_write(callback, bytes.fromhex('c0035fd6'))
    cs = Cs(CS_ARCH_ARM, CS_MODE_ARM)
    for address in ADDRESSES:
        instructions = list(cs.disasm(raw_functions[address], address))
        load_base = next(i for i in instructions if i.mnemonic == 'ldr' and i.op_str.startswith('r5, [pc,'))
        add_base = next(i for i in instructions if i.mnemonic == 'add' and i.op_str == 'r5, pc, r5')
        load_offset = next(i for i in instructions if i.mnemonic == 'ldr' and i.op_str.startswith('r2, [pc,'))
        def literal(instruction):
            offset = int(re.search(r'#(0x[0-9a-f]+|\d+)', instruction.op_str).group(1), 0)
            return struct.unpack('<I', old.uc.mem_read(instruction.address+8+offset, 4))[0]
        got = (add_base.address+8+literal(load_base)) & 0xffffffff
        old.pointer(got+literal(load_offset), app)
        manager_load = next(i for i in instructions if i.mnemonic == 'ldr' and i.op_str.startswith('r8, [r2,'))
        offset = int(re.search(r'#(0x[0-9a-f]+|\d+)', manager_load.op_str).group(1), 0)
        old.pointer(app+offset, manager)
    services, returns = [[], []], [[], []]
    value = 0
    def ret(cpu, result=0):
        cpu.put(0, result & 0xffffffff)
        cpu.uc.reg_write(cpu.pc, cpu.uc.reg_read(cpu.lr))
    def old_hook(uc, address, size, unused):
        if address in (0x4bd640, 0x4c4bdc):
            assert old.reg(0) == manager
            services[0].append([0 if address == 0x4c4bdc else 1,
                                string(old, old.reg(1)).decode(), string(old, old.reg(2)).decode()])
            ret(old, value)
        elif address == 0x37cb24:
            assert old.reg(0) == oresults
            returns[0].append(bits(float(signed(old.reg(1)))))
            ret(old)
    def native_hook(uc, address, size, unused):
        if address == callback:
            assert native.reg(0) == 0xabcdef0123456789
            services[1].append([native.reg(1), string(native, native.reg(2)).decode(),
                                string(native, native.reg(3)).decode()])
            uc.mem_write(native.reg(4), struct.pack('<I', value))
            ret(native)
    old.uc.hook_add(UC_HOOK_CODE, old_hook)
    native.uc.hook_add(UC_HOOK_CODE, native_hook)
    rows = []
    corpus = bytearray(struct.pack('<2I', 0x314e4744, 0))
    for op, count, first, second in itertools.product(range(3), (0, 1, 2, 3, 16, 33), range(8), range(8)):
        for value in ((0x80000000, 0xffffffff, 0, 1, 0x7fffffff)
                      if count >= 2 and first == second == 4 else (0x13579bdf,)):
            old.uc.mem_write(oargs, bytes(112*max(count, 1)))
            old.uc.mem_write(ov, struct.pack('<3I', oargs, oargs+112*count, oargs+112*count))
            native.uc.mem_write(na, bytes(40*max(count, 1)))
            for i in range(count):
                kind = first if i == 0 else second if i == 1 else 5
                old.pointer(oargs+112*i+4, kind)
                old.pointer(oargs+112*i+0x20, text_old[min(i, 1)])
                native.uc.mem_write(na+40*i, struct.pack('<4I3Q', kind, 0, 0, 0,
                                    text_new[min(i, 1)], (19, 9)[min(i, 1)], 0))
            for items in services + returns:
                items.clear()
            old.invoke(ADDRESSES[op], [oa, oresults, 0])
            native.uc.mem_write(ncount, struct.pack('<I', 0xdeadbeef))
            assert native.invoke('dh2_script_design_get_'+NAMES[op],
                [ns, na, count, nout, 1, ncount, 0, 0]) == 0
            n = struct.unpack('<I', native.uc.mem_read(ncount, 4))[0]
            assert n <= 1
            if n:
                assert struct.unpack('<I', native.uc.mem_read(nout, 4))[0] == 3
                returns[1].append(struct.unpack('<I', native.uc.mem_read(nout+8, 4))[0])
            assert services[0] == services[1] and returns[0] == returns[1], (op, count, first, second)
            rows.append(dict(operation=op, count=count, first=first, second=second,
                             lookup_word=value, returns=returns[0].copy(), services=services[0].copy()))
            corpus += struct.pack('<7I', op, count, first, second, value, n,
                                  returns[0][0] if n else 0)
    struct.pack_into('<I', corpus, 4, len(rows))
    (args.output/'design-original-gold.bin').write_bytes(corpus)
    (args.output/'design-original-gold.json').write_text(json.dumps(rows, indent=2)+'\n')
    source_files = ['script_design_bindings.h', 'script_design_bindings.c', 'tests/script_design_differential.py']
    report = dict(validation='PASS', scope=__doc__, original_sha256=EXPECTED,
                  library_sha256=sha(args.library), comparisons=len(rows), mismatches=0,
                  ordered_lookup_calls=sum(len(r['services']) for r in rows),
                  source_sha256={p: sha(ROOT/'port/script-runtime'/p) for p in source_files},
                  original_manifest_sha256=sha(args.output/'original-functions.json'),
                  gold_sha256=hashlib.sha256(corpus).hexdigest(),
                  original_wrapper_instructions_executed=True, compiled_arm64_callbacks_executed=True,
                  manager_lookup_proved=False, actual_vm_executed=False)
    (args.output/'differential.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
