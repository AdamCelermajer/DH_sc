"""Capture complete original stack/coordinator bodies for the in-game UI."""
from pathlib import Path
import json,hashlib,sys,re,struct
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
ROOT=Path(__file__).resolve().parents[3]
SOURCE=ROOT/'.local-inputs/libDungeonHunter2.so'
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert hashlib.sha256(SOURCE.read_bytes()).hexdigest()==EXPECTED
HERE=ROOT/'port/engine-ui/reference/character-menu-flow-v1'
selected=[0x43b1b4,0x43b158,0x43ac28,0x439dd8,0x444990,
 0x4317e8,0x42e208,0x431924,0x42e2b0,0x42d234,0x42d1f0,
 0x438278,0x438c14,0x439270,0x452b1c,0x4528a0,0x453014,0x4528b8,
 0x4528d0,0x452810,0x452468,0x3a9db4,0x3d8cfc,0x3bc4d0,0x3e0af8,0x3a9d10,
 0x3cc238,0x3cc448,0x3bbe2c,0x467324,0x465430,0x464f4c,0x468574]
with SOURCE.open('rb') as stream:
 elf=ELFFile(stream);symbols={s['st_value']:s for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC' and s['st_size']}
 selected.extend(a for a,s in symbols.items() if 'NativeReloadSkills' in s.name)
 segments=[(s['p_vaddr'],s['p_vaddr']+s['p_filesz'],s['p_offset']) for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
 def read(address,size):
  for lo,hi,offset in segments:
   if lo<=address and address+size<=hi:stream.seek(offset+address-lo);return stream.read(size)
  raise ValueError(('unmapped',address,size))
 rows=[];assembly=[]
 for address in selected:
  symbol=symbols[address];raw=read(address,symbol['st_size']);calls={};literals={};strings=[]
  assembly.append('\n# '+symbol.name+' '+hex(address))
  for ins in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,address):
   assembly.append(f'{ins.address:08x}: {ins.mnemonic:8} {ins.op_str}')
   match=re.fullmatch(r'(\w+), \[pc, #(-?0x[0-9a-f]+)\]',ins.op_str)
   if ins.mnemonic=='ldr' and match:
    reg,offset=match.groups();literals[reg]=struct.unpack('<I',read(ins.address+8+int(offset,0),4))[0]
   match=re.fullmatch(r'(\w+), pc, (\w+)',ins.op_str)
   if ins.mnemonic=='add' and match and match[2] in literals:
    pointer=(ins.address+8+literals[match[2]])&0xffffffff
    try:
     value=read(pointer,128).split(b'\0',1)[0]
     if value and all(32<=c<127 for c in value):strings.append({'pc':hex(ins.address),'address':hex(pointer),'text':value.decode()})
    except ValueError:pass
   if ins.mnemonic in ('bl','b','blx') and ins.op_str.startswith('#'):
    target=int(ins.op_str[1:],0)
    if target in symbols:calls[hex(target)]=symbols[target].name
  rows.append({'symbol':symbol.name,'address':hex(address),'size':len(raw),'sha256':hashlib.sha256(raw).hexdigest(),'direct_calls':calls,'literal_strings':strings})
 (HERE/'native-stack-functions.json').write_text(json.dumps({'original_sha256':EXPECTED,'functions':rows},indent=2)+'\n')
 (HERE/'native-stack-functions.asm').write_text('\n'.join(assembly)+'\n')
print(json.dumps({'validation':'PASS','complete_bodies':len(rows)}))
