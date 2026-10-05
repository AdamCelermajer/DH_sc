"""Resolve loader-relevant ARM vtable slots from the supplied ELF image.

Reads the original initialized image through the existing Cpu loader. It does
not construct or activate gameplay objects. Relocated entries are reported
alongside every matching defined dynamic symbol, rather than guessed names.
"""
import argparse
import hashlib
import json
import pathlib
import struct
import sys

ap = argparse.ArgumentParser()
ap.add_argument('--engine', type=pathlib.Path, required=True)
ap.add_argument('--dependency-root', type=pathlib.Path, required=True)
ap.add_argument('--out', type=pathlib.Path, required=True)
a = ap.parse_args()
sys.path.insert(0, str(a.dependency_root))
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[2] / 'engine-math/tests'))
from differential import Cpu
from elftools.elf.elffile import ELFFile

engine_sha = hashlib.sha256(a.engine.read_bytes()).hexdigest()
assert engine_sha == '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class NoImports:
    def call(self, cpu, name):
        raise RuntimeError('Unexpected execution: ' + name)

cpu = Cpu(a.engine, False, NoImports(), {'functions': []})
names = {}
with a.engine.open('rb') as stream:
    elf = ELFFile(stream)
    for symbol in elf.get_section_by_name('.dynsym').iter_symbols():
        if symbol['st_shndx'] != 'SHN_UNDEF':
            names.setdefault(symbol['st_value'], []).append(symbol.name)

rows = []
for name, address_point_offset, slots in (
    ('_ZTV10ObjectBase', 8, range(0, 0x30, 4)),
    ('_ZTV10GameObject', 8, range(0, 0x30, 4)),
    ('_ZTV5Decor', 8, range(0, 0x30, 4)),
    ('_ZTV6Module', 8, range(0, 0x30, 4)),
    # RootSceneNode has a virtual base. Its constructor explicitly adds 28
    # to this vtable symbol at 0x35d890, rather than the ordinary +8.
    ('_ZTV13RootSceneNode', 28, (0x90, 0x94, 0x98, 0x9c, 0xa0, 0xa4, 0xb8)),
):
    if name == '_ZTV13RootSceneNode':
        assert bytes(cpu.uc.mem_read(0x35d890, 4)) == struct.pack('<I', 0xe283301c)
    base = cpu.symbols[name] + address_point_offset
    entries = []
    for slot in slots:
        value = struct.unpack('<I', cpu.uc.mem_read(base + slot, 4))[0]
        entries.append({'slot': hex(slot), 'target': hex(value), 'symbols': names.get(value, [])})
    rows.append({'symbol': name, 'address_point_offset': address_point_offset,
                 'address_point': hex(base), 'entries': entries})

for row in rows[:4]:
    gate = next(e for e in row['entries'] if e['slot'] == '0x20')
    init = next(e for e in row['entries'] if e['slot'] == '0x1c')
    assert any('IsGameObject' in n for n in gate['symbols'])
    assert any('InitPost' in n for n in init['symbols'])
assert bytes(cpu.uc.mem_read(0x8c0478, 12)) == b'LevelConfig\0'
report = {'validation': 'PASS', 'scope': __doc__, 'engine_sha256': engine_sha,
          'script_sha256': hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
          'cpu_helper_sha256': hashlib.sha256(pathlib.Path(sys.modules['differential'].__file__).read_bytes()).hexdigest(),
          'early_init_gametype_literal': {'address':'0x8c0478', 'value':'LevelConfig',
                                         'load_instruction':'0x34ba4c'},
          'vtables': rows}
a.out.write_text(json.dumps(report, indent=2) + '\n')
print(json.dumps({'validation':'PASS','vtables':len(rows),
                  'game_object_gate_slot':'0x20','init_post_slot':'0x1c',
                  'early_init_type':'LevelConfig','root_address_point_offset':28}))
