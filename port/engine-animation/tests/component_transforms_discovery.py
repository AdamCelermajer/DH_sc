"""Capture original float component interpreter/default/compatibility evidence."""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from elftools.elf.elffile import ELFFile
from compiled_transforms_differential import Cpu, database, nodes, relocate, words

ROOT = Path(__file__).resolve().parents[1]
REPO = ROOT.parents[1]
KEYS = ('PositionXExIfE', 'PositionYExIfE', 'PositionZExIfE',
        'ScaleXExIfE', 'ScaleYExIfE', 'ScaleZExIfE')


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--engine', type=Path, default=REPO / '.local-inputs/libDungeonHunter2.so')
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    args.output.mkdir(parents=True, exist_ok=True)
    if (args.output / 'discovery.json').exists():
        raise RuntimeError('Refusing to overwrite original component discovery')
    assert sha(args.engine) == '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
    base_manifest = json.loads((ROOT / 'reference/dynamic-compiled-transforms/original-functions.json').read_bytes())
    functions = {int(row['elf_address'], 16): row for row in base_manifest['functions']}
    assembly = []
    vtables = []
    with args.engine.open('rb') as stream:
        elf = ELFFile(stream)
        segments = [segment for segment in elf.iter_segments() if segment['p_type'] == 'PT_LOAD']
        for symbol in elf.get_section_by_name('.symtab').iter_symbols():
            name = symbol.name
            is_component = any(key in name for key in KEYS)
            wanted = is_component and any(method in name for method in
                        ('getKeyBasedValueEx', 'retrieveValue', 'getValueSize', 'applyValue', 'getInstance'))
            if symbol['st_value'] in (0x611ae0, 0x61c0c8, 0x670a60):
                wanted = True
            if symbol['st_shndx'] == 'SHN_UNDEF' or not symbol['st_value'] or not symbol['st_size'] or not (wanted or (is_component and name.startswith('_ZTV'))):
                continue
            address = symbol['st_value']
            segment = next((segment for segment in segments if
                           segment['p_vaddr'] <= address < segment['p_vaddr'] + segment['p_filesz']), None)
            if segment is None:
                continue
            stream.seek(segment['p_offset'] + address - segment['p_vaddr'])
            raw = stream.read(symbol['st_size'])
            row = dict(original_symbol=name, elf_address=hex(address), size=len(raw),
                       sha256=hashlib.sha256(raw).hexdigest())
            if name.startswith('_ZTV'):
                row['words'] = list(struct.unpack('<' + 'I' * (len(raw)//4), raw))
                vtables.append(row)
            elif symbol['st_info']['type'] == 'STT_FUNC':
                functions[address] = row
                assembly.append('\n# ' + name)
                assembly.extend(f'{ins.address:08x}: {ins.mnemonic:8} {ins.op_str}'
                                for ins in Cs(CS_ARCH_ARM, CS_MODE_ARM).disasm(raw, address))
    manifest = dict(original_sha256=sha(args.engine), functions=list(functions.values()))
    (args.output / 'original-functions.json').write_text(json.dumps(manifest, indent=2)+'\n')
    (args.output / 'component-functions.asm').write_text('\n'.join(assembly)+'\n')
    (args.output / 'component-vtables.json').write_text(json.dumps(vtables, indent=2)+'\n')
    cpu = Cpu(args.engine, False, manifest)
    model = (REPO / 'port/android-native/app/src/main/assets/models/prince_modular.bdae').read_bytes()
    authored = nodes(model)
    image = cpu.data + 0x10000
    db = database(cpu, relocate(cpu, model, image), cpu.data + 0x1000)
    cpu.uc.mem_write(0x9f7110, words([1]))
    table = cpu.invoke(0x670a60, [])
    cpu.pointer(0x9f7570, table)
    compatibility = []
    for kind in range(11):
        mask = struct.unpack('<3I', cpu.uc.mem_read(table + kind * 12, 12))
        compatibility.append(dict(type=kind, words=mask,
                                 compatible=[other for other in range(11) if mask[other//32] & (1 << (other%32))]))
    defaults = []
    channel, name, output = cpu.data+0x2000, cpu.data+0x3000, cpu.data+0x4000
    for uri, offset in authored[:3]:
        cpu.uc.mem_write(name, uri.encode() + b'\0')
        for kind in (1, 2, 3, 4, 5, 7, 8, 9, 10):
            cpu.uc.mem_write(channel, words([0, name, kind, 0]))
            cpu.pointer(output, 0)
            result = cpu.invoke(0x61c6bc, [db, channel, output])
            pointer = struct.unpack('<I', cpu.uc.mem_read(output, 4))[0]
            defaults.append(dict(uri=uri, type=kind, result=result,
                                 node_relative=pointer-image-offset if pointer else None,
                                 value_words=list(struct.unpack('<4I', cpu.uc.mem_read(pointer, 16))) if pointer else None))
    report = dict(validation='PASS', original_sha256=sha(args.engine),
                  manifest_sha256=sha(args.output/'original-functions.json'),
                  script_sha256=sha(Path(__file__)), model_sha256=hashlib.sha256(model).hexdigest(),
                  functions=len(functions), vtables=len(vtables), compatibility=compatibility,
                  default_queries=defaults, original_import_calls=cpu.import_calls,
                  scope='Actual original type compatibility initializer and default queries; '
                        'float component interpreter/vtable capture. No native sampler or GPU parity claim.')
    (args.output / 'discovery.json').write_text(json.dumps(report, indent=2)+'\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
