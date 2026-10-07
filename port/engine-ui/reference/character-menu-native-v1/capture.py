"""Read-only source capture for the authored in-game inventory/stat/skill menu."""
import hashlib,json,struct,re
from pathlib import Path
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
SOURCE=ROOT/'.local-inputs/libDungeonHunter2.so'
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert hashlib.sha256(SOURCE.read_bytes()).hexdigest()==EXPECTED
menu=json.loads((ROOT/'port/engine-ui/reference/hud-player-infos/native-action-names.json').read_text())
names=next(row['native_strings'] for row in menu if row['file']=='dqcharmenu.swf')
selected=[n for n in names if n.startswith(('NativeInv','NativeSkill','NativeStats','NativeHUD')) and n!='NativeInvitePlayer']
selected += ['NativeGetPlayerStats','NativeGetSkillDetails','NativeEquipSkill','NativeSwapEquipment','NativeSaveGame']
with SOURCE.open('rb') as stream:
 elf=ELFFile(stream)
 symbols={s['st_value']:s for s in elf.get_section_by_name('.symtab').iter_symbols() if s['st_info']['type']=='STT_FUNC' and s['st_size']}
 segments=[(s['p_vaddr'],s['p_vaddr']+s['p_filesz'],s['p_offset']) for s in elf.iter_segments() if s['p_type']=='PT_LOAD']
 def read(a,n):
  for lo,hi,off in segments:
   if lo<=a and a+n<=hi:stream.seek(off+a-lo);return stream.read(n)
  raise RuntimeError(('unmapped source',a,n))
 functions=[];assembly=[]
 class_keys=[]
 for requirement,pc,literal in [(1,0x3fa4bc,0x3fa59c),(2,0x3fa548,0x3fa5b8),(3,0x3fa534,0x3fa5b4),(4,0x3fa520,0x3fa5b0),(5,0x3fa3b0,0x3fa594),(6,0x3fa50c,0x3fa5ac),(7,0x3fa4f8,0x3fa5a8),(8,0x3fa4e4,0x3fa5a4),(9,0x3fa4d0,0x3fa5a0)]:
  ptr=(pc+8+struct.unpack('<I',read(literal,4))[0])&0xffffffff
  text=read(ptr,64).split(b'\0',1)[0].decode()
  class_keys.append(dict(requirement=requirement,source_pc=hex(pc),source_string=hex(ptr),character_key=text))
 (HERE/'item-class-requirements-v1.json').write_text(json.dumps(class_keys,indent=2)+'\n')
 owners=['_ZN9Character15EquipItemToSlotEjj','_ZN9Character19UnEquipItemFromSlotEj','_ZN9Character10IncStatStrEv','_ZN9Character10IncStatDexEv','_ZN9Character10IncStatEndEv','_ZN9Character10IncStatNrgEv']
 owners += [symbols[a].name for a in (0x3e087c,0x3defbc,0x3df2a4,0x3e0810,0x3def34,0x3df250,0x3e2e20,0x3bca84,0x3bca50,0x3bc784,0x3bc5c0)]
 owners += [symbols[a].name for a in (0x3df8ac,0x3df81c,0x3df7c4,0x3a56a4,0x3bb7e8,0x3bb7fc,0x3bc9ec,0x3fa330,0x3f9f88,0x3f9e94,0x3fa710,0x3fed48,0x3fee50,0x43bff4)]
 owners += [symbols[a].name for a in (0x3a4c14,0x3a9ee8,0x401940,0x4016e4,0x401634,0x3fd1e8,0x3fd1dc,0x3fa1fc,0x3fa6dc,0x3fa5cc,0x3fdcbc,0x3fd5d0)]
 owners += [symbols[a].name for a in (0x3bbb94,0x467ce4,0x3ae99c,0x3f9e68,0x3a4bec,0x3a4a3c,0x3ff858,0x3ec974,0x3fe448)]
 for name in sorted(set(selected))+owners:
  hits=[(a,s) for a,s in symbols.items() if s.name==name or (name in s.name and s.name.startswith('_Z'+str(len(name))+name))]
  assert len(hits)==1,(name,hits)
  a,s=hits[0];raw=read(a,s['st_size']);calls={};literals={};literal_strings=[]
  assembly.append('# '+name+' '+s.name)
  for ins in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,a):
   assembly.append(f'{ins.address:08x}: {ins.mnemonic:8} {ins.op_str}')
   match=re.fullmatch(r'(\w+), \[pc, #(-?0x[0-9a-f]+)\]',ins.op_str)
   if ins.mnemonic=='ldr' and match:
    reg,offset=match.groups();literals[reg]=struct.unpack('<I',read(ins.address+8+int(offset,0),4))[0]
   match=re.fullmatch(r'(\w+), pc, (\w+)',ins.op_str)
   if ins.mnemonic=='add' and match and match[2] in literals:
    ptr=(ins.address+8+literals[match[2]])&0xffffffff
    try:
     txt=read(ptr,192).split(b'\0',1)[0]
     if txt and all(32<=v<127 for v in txt):literal_strings.append(dict(source_pc=hex(ins.address),address=hex(ptr),text=txt.decode()))
    except RuntimeError:pass
   if ins.mnemonic in ('bl','blx','b') and ins.op_str.startswith('#'):
    target=int(ins.op_str[1:],0)
    if target in symbols:calls[hex(target)]=symbols[target].name
  functions.append(dict(name=name,original_symbol=s.name,elf_address=hex(a),size=s['st_size'],sha256=hashlib.sha256(raw).hexdigest(),direct_calls=calls,literal_strings=literal_strings))
 (HERE/'original-functions.json').write_text(json.dumps(dict(original_sha256=EXPECTED,functions=functions),indent=2)+'\n')
 (HERE/'original-functions.asm').write_text('\n'.join(assembly)+'\n')
 print(json.dumps(dict(validation='PASS',captured_callbacks=len(functions),scope='original in-game menu callback bodies; schemas remain to recover')))
