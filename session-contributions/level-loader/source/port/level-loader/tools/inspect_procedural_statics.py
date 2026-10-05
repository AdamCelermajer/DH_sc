import json,pathlib,struct,sys
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[4]/'dependencies'))
from elftools.elf.elffile import ELFFile
path=pathlib.Path(r'C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs\libDungeonHunter2.so')
raw=path.read_bytes();elf=ELFFile(path.open('rb'))
def read(at,n):
    for segment in elf.iter_segments():
        if segment['p_type']=='PT_LOAD' and segment['p_vaddr']<=at and at+n<=segment['p_vaddr']+segment['p_filesz']:
            offset=segment['p_offset']+at-segment['p_vaddr'];return raw[offset:offset+n]
    raise ValueError(hex(at))
def text(at):return read(at,256).split(b'\0')[0].decode(errors='replace')
syms={s.name:s for s in elf.get_section_by_name('.dynsym').iter_symbols()}
for name,s in syms.items():
    if 'rnd' in name and ('Direction' in name or s['st_info']['type']=='STT_OBJECT'):
        print(json.dumps({'symbol':name,'address':hex(s['st_value']),'size':s['st_size']}))
root=pathlib.Path(__file__).resolve().parents[1]
rows=json.loads((root/'reference/procedural-functions/symbols-and-strings.json').read_text())['functions']
for row in rows:
    if any(n in row['symbol'] for n in ('TryIsZero','GetBlockUnitPosition','FromFilename','4find','Compare','Direction')):
        print(json.dumps(row))
for j in range(5):
    words=struct.unpack('<4I',read(0x956ae4+j*16,16))
    print(json.dumps({'direction_row':j,'words':list(words),'name':text(words[1]),'delta':list(struct.unpack('<2i',struct.pack('<2I',*words[2:])))}))
print(json.dumps({'block_default_unit_bits':hex(1157627904+3899392),'value':struct.unpack('<f',struct.pack('<I',1157627904+3899392))[0]}))
