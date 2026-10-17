"""Whole original SafeGetCharPropsId/template helper and PROP reader replay.
IsPlayer, SG_Load's external body, strcmp, integer division and stream virtual
are declared fixtures. SG pointer wrappers, getters/setter, RNG inline, _GetType
and valid-id _GetProperty execute their original ARM instructions. No private
campaign bytes enter public fixtures.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
from capstone import Cs,CS_ARCH_ARM,CS_MODE_ARM
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3];REF=ROOT/'port/level-world/reference/character-saved-class-v1'
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from items_differential_v4_original import Original
W=lambda *v:struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
FUNCTIONS=['_ZN9Character18SafeGetCharPropsIdEv','_ZN9Character26SafeGetCharPropsTemplateIdEv',
 '_ZN9Character7SG_LoadEi','_ZNK9Character17SG_GetPlayerClassEv','_ZN9Character17SG_SetPlayerClassEi',
 '_ZN14PlayerSavegame16__LoadPropertiesEP11IStreamBasePv','_ZNK14CharProperties8_GetTypeEi',
 '_ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi',
 '_ZN11IStreamBase6readAsIiEEvRT_','_ZN11IStreamBase6readAsIbEEvRT_']
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def evidence(path):
 with path.open('rb') as f:
  elf=ELFFile(f);syms={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()};rows=[];asm=[]
  for name in FUNCTIONS:
   s=syms[name];a=s['st_value'];n=s['st_size']
   segment=next(q for q in elf.iter_segments() if q['p_type']=='PT_LOAD' and q['p_vaddr']<=a<a+n<=q['p_vaddr']+q['p_filesz'])
   f.seek(segment['p_offset']+a-segment['p_vaddr']);raw=f.read(n)
   rows.append({'original_symbol':name,'elf_address':hex(a),'size':n,'sha256':hashlib.sha256(raw).hexdigest()})
   asm.append(name+' '+hex(a)+'\n'+'\n'.join(f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in Cs(CS_ARCH_ARM,CS_MODE_ARM).disasm(raw,a)))
 return rows,'\n\n'.join(asm)+'\n'
class Truncated(Exception):pass
class Replay(Original):
 def __init__(self,path,manifest):
  super().__init__(path,manifest);self.character=self.data+0x5000;self.save=self.data+0x8000
  self.player=0;self.loads=0;self.queries=0;self.after_class=0;self.mode='class'
  self.pointer(self.character,self.vt+0x200);self.pointer(self.vt+0x228,self.callback+32)
  self.uc.hook_add(UC_HOOK_CODE,self.services)
  self.seed_address=self.word(0x994a98+self.word(0x3b3fe8));self.counter_address=self.word(0x994a98+self.word(0x3b3fec))
  self.template_members=self.word(0x994a98+self.word(0x3b3fe4))
  self.template_size=self.word(0x994a98+self.word(0x3b3794));self.template_names=self.word(0x994a98+self.word(0x3b3798))
  self.types_members=self.word(0x994a98+self.word(0x3def0c))
 def allocate(self,n):
  p=self.heap;self.heap+=(max(n,1)+15)&~15;assert self.heap<self.data+0x2000000
  self.uc.mem_write(p,bytes(max(n,1)));return p
 def string(self,v):
  p=self.allocate(len(v)+1);self.uc.mem_write(p,v+b'\0');return p
 def names(self,address,size,values):
  table=self.allocate(len(values)*4)
  for k,v in enumerate(values):self.pointer(table+4*k,self.string(v))
  self.pointer(address,table);self.pointer(size,len(values))
 def external(self,uc,address,size,unused):
  if address==self.callback+32:self.queries+=1;self.returned(self.player)
  elif self.imports.get(address)=='__aeabi_idivmod':
   a,b=self.reg(0),self.reg(1);a=a if a<0x80000000 else a-0x100000000;b=b if b<0x80000000 else b-0x100000000
   q=abs(a)//abs(b);q=-q if (a<0)!=(b<0) else q;self.put(1,a-q*b);self.returned(q)
  elif address==self.callback+16 and self.cursor+self.reg(2)>len(self.blob):raise Truncated()
  else:super().external(uc,address,size,unused)
 def services(self,uc,a,size,unused):
  if a==0x465430 and self.mode=='class':
   assert self.reg(0)==self.save and self.reg(1)==1;self.loads+=1;self.pointer(self.save+0x34,self.after_class&0xffffffff);self.returned()
 def class_case(self,args):
  cache,player,present,saved,after,template,explicit,variant,seed,counter,template_cache=args
  self.mode='class';self.player=player;self.loads=self.queries=0;self.after_class=after
  self.uc.mem_write(self.character+0x13c8,struct.pack('<HH',cache&65535,template_cache&65535));self.pointer(self.character+0x14e8,self.save if present else 0)
  self.pointer(self.save+0x34,saved&0xffffffff);self.pointer(self.seed_address,seed);self.pointer(self.counter_address,counter)
  names=[b'Other',b'KnightPlayerBase',b'MagePlayerBase',b'KnightPlayerBase'] if variant==0 else [b'Other',b'MagePlayerBase']
  self.names(0x9a6460,0x9a6458,names)
  templates=[(b'Mixed',[35,35,37,32768,65536]),(b'Empty',[]),(b'Mixed',[99])]
  self.names(self.template_names,self.template_size,[t[0] for t in templates]);records=self.allocate(12*len(templates));self.pointer(self.template_members,records)
  for k,(_,ids) in enumerate(templates):
   slots=self.allocate(8*len(ids))
   for i,value in enumerate(ids):self.uc.mem_write(slots+8*i+4,struct.pack('<H',value&65535))
   self.uc.mem_write(records+12*k,W(0,len(ids),slots))
  template_text=[b'',b'Mixed',b'Empty',b'Missing'][template]
  explicit_text=[b'',b'MagePlayerBase',b'Missing',b'KnightPlayerBase',b'MagePlayerBase\0suffix'][explicit]
  for start,end,v in [(0x13a8,0x13ac,template_text),(0x13c0,0x13c4,explicit_text)]:
   self.pointer(self.character+start,0);self.pointer(self.character+end,self.string(v) if v else 0)
  result=self.invoke(0x3b3d38,[self.character]);p,t=struct.unpack('<hh',self.uc.mem_read(self.character+0x13c8,4))
  return [result,p,t,self.word(self.save+0x34) if present else 0xffffffff,self.word(self.seed_address),self.word(self.counter_address),self.queries,self.loads]
 def prop_case(self,blob,types,before,flag):
  self.mode='prop';self.blob=blob;self.cursor=0;self.pointer(self.save+0x10,self.character)
  table=self.allocate(1800);self.pointer(self.types_members,table);self.uc.mem_write(table+900+4,W(*types))
  self.uc.mem_write(self.character+0x8ec+4,W(*before));self.uc.mem_write(self.save+0x194,bytes([flag]))
  complete=1
  try:self.invoke(0x46932c,[self.stream,self.save])
  except Truncated:complete=0
  return complete,self.cursor,bytes(self.uc.mem_read(self.character+0x8ec+4,896)),bytes(self.uc.mem_read(self.save+0x194,1))
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--original-elf',required=True,type=Path);a=p.parse_args();assert sha(a.original_elf)==SHA
 REF.mkdir(parents=True,exist_ok=True);rows,asm=evidence(a.original_elf);manifest={'original_sha256':SHA,'functions':rows,'scope':__doc__}
 (REF/'original-functions.json').write_text(json.dumps(manifest,indent=2)+'\n');(REF/'original-functions.asm').write_text(asm)
 cpu=Replay(a.original_elf,manifest);words={x for r in rows for x in range(int(r['elf_address'],16),int(r['elf_address'],16)+r['size'],4)};seen=set()
 cpu.uc.hook_add(UC_HOOK_CODE,lambda uc,a,n,u:seen.add(a) if a in words else None)
 cases=[];rng=random.Random(20261006)
 for k in range(512):
  args=[rng.choice([-1,0,7,-32768]),rng.randrange(2),rng.randrange(2),rng.choice([-1,1,263,32768,65535,65536,0x7fffffff]),rng.choice([-1,1,263,32768,65535,65536,0x7fffffff]),rng.randrange(4),rng.randrange(5),rng.randrange(2),rng.getrandbits(32),rng.getrandbits(32),rng.choice([-1,7])]
  cases.append(W(*args,*cpu.class_case(args)))
 # Guaranteed every template alternative with duplicate weighting intact.
 for seed in range(32):
  args=[-1,0,0,-1,-1,1,1,0,seed,0,-1];cases.append(W(*args,*cpu.class_case(args)))
 (REF/'class-fixtures.bin').write_bytes(b'CSC1'+W(len(cases))+b''.join(cases))
 properties=[]
 for k in range(32):
  types=[rng.choice([-1,0,1,16,32,33,36,0x80000020]) for _ in range(224)];before=[rng.getrandbits(32) for _ in range(224)];values=[rng.getrandbits(32) for _ in range(224)]
  blob=W(224,*values)+bytes([rng.choice([0,1,2,127,255])]);flag=rng.randrange(256)
  lengths=[len(blob)] if k else [0,1,3,4,5,8,9,43,44,899,900,901]
  for length in lengths:
   data=blob[:length];complete,used,after,final=cpu.prop_case(data,types,before,flag)
   properties.append(W(len(data))+data+W(*types,*before)+bytes([flag])+W(complete,used)+after+final)
 for count in [0,223,225,0xffffffff]:
  blob=W(count);types=[32]*224;before=[19]*224;complete,used,after,flag=cpu.prop_case(blob,types,before,17)
  properties.append(W(len(blob))+blob+W(*types,*before)+bytes([17])+W(complete,used)+after+flag)
 (REF/'prop-fixtures.bin').write_bytes(b'CSP1'+W(len(properties))+b''.join(properties))
 report={'validation':'PASS','original_sha256':SHA,'class_cases':len(cases),'prop_cases':len(properties),'functions':rows,'executed_words':len(seen),'executed_addresses':[hex(x) for x in sorted(seen)],'generator_sha256':sha(Path(__file__)),'fixture_sha256':{n:sha(REF/n) for n in ['class-fixtures.bin','prop-fixtures.bin']},'declared_leaves':['IsPlayer virtual+28','PlayerSavegame::SG_Load body','strcmp','__aeabi_idivmod','IStreamBase read virtual'],'scope':__doc__}
 (REF/'original-capture.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ['validation','class_cases','prop_cases','executed_words']}))
if __name__=='__main__':main()
