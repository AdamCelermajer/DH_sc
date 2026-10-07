"""Capture the actual original dynamic-text display/font producer bodies."""
import hashlib
import json
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[5]
HERE = Path(__file__).resolve().parent
SOURCE = ROOT / '.local-inputs/libDungeonHunter2.so'
EXPECTED = '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
addresses = ()
assert hashlib.sha256(SOURCE.read_bytes()).hexdigest() == EXPECTED
assert not (HERE / 'original-functions.json').exists(), 'Preserve captured evidence'
with SOURCE.open('rb') as stream:
    elf = ELFFile(stream)
    symbols = {symbol['st_value']: symbol for symbol in elf.get_section_by_name('.symtab').iter_symbols()
               if symbol['st_info']['type'] == 'STT_FUNC' and symbol['st_size']}
    segments = [(s['p_vaddr'], s['p_vaddr'] + s['p_filesz'], s['p_offset'])
                for s in elf.iter_segments() if s['p_type'] == 'PT_LOAD']
    def read(address, size):
        for low, high, offset in segments:
            if low <= address and address + size <= high:
                stream.seek(offset + address - low)
                return stream.read(size)
        raise RuntimeError(('Source range outside file', address, size))
    addresses = (0x758ca8,0x75cb6c)
    functions, assembly = [], []
    for address in addresses:
        symbol = symbols[address]
        raw = read(address, symbol['st_size'])
        calls = {}
        assembly.append('# ' + symbol.name)
        for instruction in Cs(CS_ARCH_ARM, CS_MODE_ARM).disasm(raw, address):
            assembly.append(f'{instruction.address:08x}: {instruction.mnemonic:8} {instruction.op_str}')
            if instruction.mnemonic in ('bl', 'b') and instruction.op_str.startswith('#0x'):
                target = int(instruction.op_str[1:], 16)
                if target in symbols:
                    calls[hex(target)] = symbols[target].name
        assembly.append('')
        functions.append(dict(original_symbol=symbol.name, elf_address=hex(address), size=len(raw),
                              sha256=hashlib.sha256(raw).hexdigest(), calls=calls))
(HERE / 'reference').mkdir(exist_ok=True)
(HERE / 'reference/original-functions.asm').write_text('\n'.join(assembly) + '\n', encoding='utf-8')
(HERE / 'original-functions.json').write_text(json.dumps(dict(original_sha256=EXPECTED, functions=functions), indent=2) + '\n')
print(json.dumps(dict(captured_functions=len(functions), bytes=sum(row['size'] for row in functions))))




