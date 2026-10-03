"""Resolve the original PIC registration names/callbacks without native guesses."""
import hashlib,json,re,struct
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
engine=ROOT/'.local-inputs/libDungeonHunter2.so'
assert hashlib.sha256(engine.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
with engine.open('rb') as f:
    elf=ELFFile(f);segments=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
    def raw(a,n):
        s=next(s for s in segments if s['p_vaddr']<=a<s['p_vaddr']+s['p_filesz'])
        f.seek(s['p_offset']+a-s['p_vaddr']);return f.read(n)
    def word(a):return struct.unpack('<I',raw(a,4))[0]
    rows=[]
    for start,size in ((0x3b56bc,5332),(0x37b5a0,1252)):
        registers={}
        for ins in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw(start,size),start):
            m=re.fullmatch(r'(r\d+|ip|sl|sb|fp), \[pc(?:, #(-?0x[0-9a-f]+))?\]',ins.op_str)
            if ins.mnemonic=='ldr' and m:
                registers[m[1]]=word(ins.address+8+int(m[2] or '0',0))
            m=re.fullmatch(r'(r\d+|ip|sl|sb|fp), pc, (r\d+|ip|sl|sb|fp)',ins.op_str)
            if ins.mnemonic=='add' and m and m[2] in registers:
                registers[m[1]]=(ins.address+8+registers[m[2]])&0xffffffff
            m=re.fullmatch(r'(r\d+|ip|sl|sb|fp), \[(r\d+|ip|sl|sb|fp), (r\d+|ip|sl|sb|fp)\]',ins.op_str)
            if ins.mnemonic=='ldr' and m and m[2] in registers and m[3] in registers:
                registers[m[1]]=word((registers[m[2]]+registers[m[3]])&0xffffffff)
            m=re.fullmatch(r'(r\d+|ip|sl|sb|fp), (r\d+|ip|sl|sb|fp)',ins.op_str)
            if ins.mnemonic=='mov' and m and m[2] in registers:
                registers[m[1]]=registers[m[2]]
            if ins.mnemonic in ('bl','b') and ins.op_str=='#0x31a4d4':
                name=raw(registers['r1'],256).split(b'\0')[0].decode('ascii')
                rows.append({'caller':hex(start),'instruction':hex(ins.address),
                    'name':name,'callback':hex(registers.get('r2',0))})
    selected=[row for row in rows if row['name'] in ('StartTimer','StopTimer','Trace')]
    assert {row['name']:row['callback'] for row in selected}=={
        'StartTimer':'0x3b7590','StopTimer':'0x3b7064','Trace':'0x37ee80'}
    result={'original_sha256':hashlib.sha256(engine.read_bytes()).hexdigest(),
        'capture_script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'bindings':rows,'selected':selected}
    (HERE/'registration-names.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(selected))
