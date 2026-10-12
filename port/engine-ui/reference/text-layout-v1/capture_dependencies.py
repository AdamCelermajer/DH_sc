"""Read-only ARM call/literal evidence for the original dynamic text batch."""
import hashlib
import json
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parents[4]
HERE = Path(__file__).resolve().parent
engine = ROOT/'.local-inputs/libDungeonHunter2.so'
assert hashlib.sha256(engine.read_bytes()).hexdigest() == '36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
manifest = json.loads((HERE/'original-functions.json').read_text())
with engine.open('rb') as stream:
    elf = ELFFile(stream)
    symbols = {s['st_value']: s.name for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_value']}
    segments = [(s['p_vaddr'], s['p_vaddr']+s['p_filesz'], s['p_offset']) for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
    def read(address, size):
        for low, high, offset in segments:
            if low <= address and address+size <= high:
                stream.seek(offset+address-low)
                return stream.read(size)
        return b''
    result = {}
    for row in manifest['functions']:
        address = int(row['elf_address'],16)
        instructions = list(Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(read(address,row['size']),address))
        calls, literals, registers = {}, [], {}
        for i in instructions:
            if i.mnemonic in ('bl','b') and i.op_str.startswith('#0x'):
                target = int(i.op_str[1:],16)
                if target in symbols:
                    calls[hex(target)] = symbols[target]
            if i.mnemonic == 'ldr' and ', [pc, #' in i.op_str:
                register, tail = i.op_str.split(', [pc, #')
                offset = int(tail.split(']')[0],0)
                raw = read(i.address+8+offset,4)
                if len(raw)==4:
                    registers[register]=int.from_bytes(raw,'little')
            if i.mnemonic=='add' and ', pc, ' in i.op_str:
                dest, source=i.op_str.split(', pc, ')
                if dest==source and dest in registers:
                    pointer=(i.address+8+registers[dest])&0xffffffff
                    raw=read(pointer,96).split(b'\0',1)[0]
                    if raw and all(32<=c<127 or c in (9,10,13) for c in raw):
                        literals.append(dict(instruction=hex(i.address),address=hex(pointer),text=raw.decode()))
        result[row['original_symbol']]=dict(calls=calls,literals=literals)
out=HERE/'dependencies-and-literals.json'
assert not out.exists(), 'Preserve prior captured dependency evidence'
out.write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
