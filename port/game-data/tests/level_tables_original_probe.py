"""Capture real FastTravelList/LevelList and individual record reader outputs.

Original array, row and scalar/string stream instructions execute. The virtual
byte stream and allocator are explicit services. Vtables/string pointers and
bool padding are normalized, with every serialized scalar/string byte retained.
"""
import argparse
import hashlib
import json
import random
import struct
import sys
import zipfile
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE

DATA = Path(__file__).resolve().parents[1]
ROOT = DATA.parents[1]
sys.path.insert(0, str(DATA / 'tests'))
from items_differential import Original, strings, words


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def field(raw):
    return words(len(raw)) + raw


def projection(cpu, at, kind):
    values = list(struct.unpack('<' + ('7I' if kind == 0 else '18I'),
                              cpu.uc.mem_read(at, 28 if kind == 0 else 72)))
    pairs = [(3, 4)] if kind == 0 else [(2, 3), (7, 8)]
    texts = [bytes(cpu.uc.mem_read(values[p], values[n])) if values[n] else b''
             for n, p in pairs]
    values[0] = 0
    for _, pointer in pairs:
        values[pointer] = 0
    if kind == 1:
        values[1] &= 255
        values[5] &= 255
    return words(*values), texts


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    if args.output.exists():
        raise RuntimeError('Preserve the existing original level-table capture')
    args.output.mkdir(parents=True)
    engine = ROOT / '.local-inputs/libDungeonHunter2.so'
    cache = Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
    assert sha(engine) == '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
    assert sha(cache) == '3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
    assets = {}
    with zipfile.ZipFile(cache) as archive:
        for name in ('levels_pyarray.bin', 'levels_pyarraynames.bin', 'levels_pystructnames.bin'):
            entry = next(name_in_zip for name_in_zip in archive.namelist()
                         if name_in_zip.endswith('/' + name))
            payload = archive.read(entry)
            (args.output / name).write_bytes(payload)
            assets[name] = hashlib.sha256(payload).hexdigest()
    cpu = Original(engine, {'functions': []})
    cpu.blob = (args.output / 'levels_pyarray.bin').read_bytes()
    cpu.cursor = 0
    rows = []
    # Record entries also let us retain each exact serialized span without
    # implementing a Python row parser to supply any expected scalar values.
    pending = None

    def observe(uc, address, size, unused):
        nonlocal pending
        if address in (0x4fcff0, 0x4fca84):
            if pending:
                finish()
            pending = (0 if address == 0x4fcff0 else 1, cpu.reg(0), cpu.cursor)

    def finish():
        nonlocal pending
        kind, at, begin = pending
        scalar, texts = projection(cpu, at, kind)
        rows.append(dict(kind=kind, blob=cpu.blob[begin:cpu.cursor],
                         scalar=scalar, texts=texts, start=begin, end=cpu.cursor))
        pending = None

    cpu.uc.hook_add(UC_HOOK_CODE, observe)
    loads = []
    for address in (0x4ba8e4, 0x4ba794):
        begin = cpu.cursor
        cpu.invoke(address, [cpu.stream], budget=20000000)
        finish()
        loads.append(dict(reader=hex(address), start=begin, end=cpu.cursor))
    assert cpu.cursor == len(cpu.blob)
    counts = [sum(row['kind'] == kind for row in rows) for kind in (0, 1)]
    names = strings((args.output / 'levels_pyarraynames.bin').read_bytes())
    schema = strings((args.output / 'levels_pystructnames.bin').read_bytes())
    assert counts == [33, 51] and [len(block) for block in names] == counts
    cache_rows = len(rows)
    # Complete source readers, including noncanonical bool bytes, raw signed
    # values, empty strings, first-NUL contents and high bytes.
    rng = random.Random(20261004)
    for kind in (0, 1):
        for case in range(64):
            texts = [b'', b'\0tail', b'bytes\xff\x80', b'crypt01'][case % 4]
            if kind == 0:
                blob = words(rng.getrandbits(32), rng.getrandbits(32)) + field(texts)
                blob += words(rng.getrandbits(32), rng.getrandbits(32))
            else:
                blob = bytes([(0, 1, 127, 255)[case % 4]]) + field(texts)
                blob += words(rng.getrandbits(32)) + bytes([(255, 127, 1, 0)[case % 4]])
                blob += words(rng.getrandbits(32)) + field(texts[::-1])
                blob += words(*(rng.getrandbits(32) for _ in range(9)))
            cpu.blob, cpu.cursor = blob, 0
            target = cpu.data + 0x80000
            cpu.uc.mem_write(target, bytes(72))
            cpu.invoke(0x4fcff0 if kind == 0 else 0x4fca84,
                       [target, cpu.stream], budget=1000000)
            finish()
            assert cpu.cursor == len(blob)
    gold = b'LTD1' + words(len(rows))
    for row in rows:
        gold += words(row['kind']) + field(row['blob']) + row['scalar']
        gold += b''.join(field(raw) for raw in row['texts'])
    (args.output / 'level-table-fixtures.bin').write_bytes(gold)
    functions, assembly = [], []
    with engine.open('rb') as stream:
        elf = ELFFile(stream)
        symbols = list(elf.get_section_by_name('.symtab').iter_symbols())
        for address in (0x4ba8e4, 0x4ba794, 0x4fcff0, 0x4fca84, 0x4db89c,
                        0x313a90, 0x3df1a0, 0x459090, 0x317454, 0x4a6874, 0x4a66f4):
            symbol = next(s for s in symbols if s['st_value'] == address and s['st_size'])
            segment = next(s for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD'
                           and s['p_vaddr'] <= address < s['p_vaddr'] + s['p_filesz'])
            offset = address - segment['p_vaddr']
            raw = segment.data()[offset:offset + symbol['st_size']]
            functions.append(dict(address=hex(address), symbol=symbol.name,
                                  bytes=len(raw), sha256=hashlib.sha256(raw).hexdigest()))
            assembly += ['\n# ' + symbol.name] + [f'{i.address:08x}: {i.mnemonic} {i.op_str}'
                for i in Cs(CS_ARCH_ARM, CS_MODE_ARM).disasm(raw, address)]
    (args.output / 'original-functions.asm').write_text('\n'.join(assembly) + '\n')
    (args.output / 'original-functions.json').write_text(json.dumps(dict(
        original_sha256=sha(engine), functions=functions), indent=2) + '\n')
    crypt = []
    level_rows = [row for row in rows[:cache_rows] if row['kind'] == 1]
    for name, row in zip(names[1], level_rows):
        if b'CRYPT' in name:
            scalar = struct.unpack('<18i', row['scalar'])
            crypt.append(dict(name=name.decode(), texts=[raw.decode() for raw in row['texts']],
                              maximum=list(scalar[12:15]), minimum=list(scalar[15:18])))
    report = dict(validation='SOURCE_CAPTURED', scope=__doc__, original_sha256=sha(engine),
        cache_sha256=sha(cache), probe_sha256=sha(Path(__file__)), assets_sha256=assets,
        gold_sha256=sha(args.output / 'level-table-fixtures.bin'), actual_array_loads=loads,
        cache_fast_travel_rows=33, cache_level_rows=51, synthetic_rows=128,
        record_cases=len(rows), stream_reads=cpu.reads, allocator_calls=cpu.allocations,
        schema=[[raw.decode() for raw in block] for block in schema], crypt=crypt,
        source_Application_level_selection_verified=False)
    (args.output / 'original-loader-capture.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
