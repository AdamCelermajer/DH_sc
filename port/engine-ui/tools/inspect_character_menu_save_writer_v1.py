from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
import json

root = Path(__file__).resolve().parents[3]
with (root / '.local-inputs/libDungeonHunter2.so').open('rb') as stream:
    elf = ELFFile(stream)
    symbols = {s['st_value']: (s.name, s['st_size']) for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type'] == 'STT_FUNC'}
    def read(address, size):
        for segment in elf.iter_segments():
            if segment['p_type'] == 'PT_LOAD' and segment['p_vaddr'] <= address < address + size <= segment['p_vaddr'] + segment['p_filesz']:
                stream.seek(segment['p_offset'] + address - segment['p_vaddr'])
                return stream.read(size)
        raise ValueError(hex(address))
    callbacks = json.loads((root / 'port/game-data/reference/player-save-load-v1/section-callbacks-v1.json').read_text())
    writer_names = {entry['second'] for entries in callbacks.values() for entry in entries}
    pending = [0x3bc4a8] + [address for address, (name, _) in symbols.items() if name in writer_names or name.startswith('_ZN14PlayerSavegameC')]
    for address, (name, size) in symbols.items():
        if name.startswith('_ZN14PlayerSavegame') and size:
            instructions = list(Cs(CS_ARCH_ARM, CS_MODE_ARM).disasm(read(address, size), address))
            if any(ins.mnemonic.startswith('str') and ('#0xc]' in ins.op_str or '#0x178]' in ins.op_str) for ins in instructions):
                pending.append(address)
    seen = set()
    rows = []
    while pending:
        address = pending.pop(0)
        if address in seen:
            continue
        seen.add(address)
        name, size = symbols[address]
        lines, calls = [], {}
        for ins in Cs(CS_ARCH_ARM, CS_MODE_ARM).disasm(read(address, size), address):
            lines.append(f'{ins.address:08x}: {ins.mnemonic} {ins.op_str}')
            if ins.mnemonic in ('bl', 'b') and ins.op_str.startswith('#'):
                target = int(ins.op_str[1:], 0)
                if target in symbols:
                    target_name = symbols[target][0]
                    calls[hex(target)] = target_name
                    if ('Save' in target_name or 'Write' in target_name or 'write' in target_name) and target not in seen:
                        pending.append(target)
        rows.append({'address': hex(address), 'name': name, 'size': size, 'calls': calls, 'asm': lines})
out = root / 'port/engine-ui/reference/character-menu-native-v1/save-writer-functions-v1.json'
out.write_text(json.dumps(rows, indent=2) + '\n')
out.with_suffix('.asm').write_text('\n\n'.join(row['address']+' '+row['name']+'\n'+'\n'.join(row['asm']) for row in rows)+'\n')
for row in rows:
    print(row['address'], row['name'], row['size'], row['calls'])
