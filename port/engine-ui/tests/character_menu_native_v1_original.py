"""Recover authored AS member writes by executing original menu ARM bodies.
Character queries are explicit deterministic boundary fixtures; member names,
write order, AS value kind, arithmetic and branches execute original code.
"""
import json,struct,sys,math
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from text_layout_v1_original import TextCpu,words,Original as TextOriginal
from elftools.elf.elffile import ELFFile
class MenuCpu(TextCpu):
 def external(self,uc,a,size,u):
  if self.imports.get(a)=='strcmp':
   left=self.machine.cstr(self.reg(0)).encode();right=self.machine.cstr(self.reg(1)).encode()
   self.put(0,((left>right)-(left<right))&0xffffffff);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if self.imports.get(a)=='menu_member':self.machine.hook(uc,a,size,u);return
  if self.imports.get(a)=='__aeabi_dcmpun':
   x,y=struct.unpack('<dd',words(*(self.reg(i) for i in range(4))));self.put(0,math.isnan(x) or math.isnan(y));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if self.imports.get(a)=='sprintf':
   out=self.reg(0);fmt=self.machine.cstr(self.reg(1));arg=self.reg(2)
   assert fmt.count('%')==1 and '%d' in fmt,('unmodeled format',fmt)
   b=(fmt%arg).encode();uc.mem_write(out,b+b'\0');self.put(0,len(b));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,a,size,u)
class Original(TextOriginal):
 def __init__(self):
  manifest=json.loads((ROOT/'port/engine-ui/reference/character-menu-native-v1/original-functions.json').read_text())
  self.c=MenuCpu(ROOT/'.local-inputs/libDungeonHunter2.so',False,manifest);self.c.machine=self
  self.c.uc.hook_add(UC_HOOK_CODE,self.hook)
  self.next=self.c.data+0x100000;self.active=False;self.trace=[];self.services=[]
  self.named={}
  with (ROOT/'.local-inputs/libDungeonHunter2.so').open('rb') as f:
   elf=ELFFile(f)
   for s in elf.get_section_by_name('.symtab').iter_symbols():
    if s['st_info']['type']=='STT_FUNC' and s['st_size']:self.named[s['st_value']]=s.name
  self.entries={f['name']:int(f['elf_address'],16) for f in manifest['functions']}
  self.members=self.c.callback+128
  self.c.imports[self.members]='menu_member'
  self.character=self.alloc(0x2000);self.object=self.alloc(0x100);self.vtable=self.alloc(0x100)
  self.c.pointer(self.character,self.c.symbols['_ZTV9Character']+8)
  self.c.pointer(self.object,self.vtable);self.c.pointer(self.vtable+0x1c,self.members)
  self.output=self.alloc(12);self.environment=self.alloc(16);self.args=self.alloc(120);self.fn=self.alloc(32)
  self.c.pointer(self.environment,self.args);self.c.pointer(self.fn,self.output);self.c.pointer(self.fn+12,self.environment)
  self.c.pointer(self.fn+20,9)
  self.strings={};self.value_strings={};self.class_id=0x122
  self.skill_row=self.alloc(0x200)
  for off in (0x34,0x38,0x40):self.c.pointer(self.skill_row+off,0xffffffff)
  self.text=self.alloc(32);self.c.uc.mem_write(self.text,b'FIXTURE\0')
  self.item=self.alloc(0x100);self.c.pointer(self.item+0x54,25)
  for off in (0x34,0x4c):self.c.pointer(self.item+off,self.text)
  self.c.uc.mem_write(self.item+0x68,b'\1\0\0\0')
 def number(self,p,n):
  self.c.uc.mem_write(p,bytes((0,2,0,0))+struct.pack('<d',n))
 def hook(self,uc,a,size,u):
  if not self.active:return
  if a==self.members and uc.reg_read(self.c.pc)!=a:return
  c=self.c;x,y,z,w=(c.reg(i) for i in range(4));name=self.named.get(a,'')
  if name:self.last_entry=(hex(a),name,x,y,z)
  if a==self.members:
   typ=uc.mem_read(z+1,1)[0];key=self.string(y)
   value=self.value_strings.get(z) if typ==4 else struct.unpack('<d',uc.mem_read(z+4,8))[0] if typ==2 else self.word(z+4)
   self.trace.append(dict(key=key,type=typ,value=value,service_prefix=len(self.services)));self.ret(1)
  elif a==0x439cb4:self.ret(self.object)
  elif a==0x43c388:self.services.append([name,x,y]);self.ret(self.character)
  elif a==0x797a54:
   lo,hi=struct.unpack('<II',uc.mem_read(x+4,8));c.put(0,lo);c.put(1,hi);uc.reg_write(c.pc,uc.reg_read(c.lr))
  elif a in (0x439d8c,):self.ret(uc.mem_read(x+1,1)[0]==2)
  elif a in (0x43a1b8,):self.ret(int(struct.unpack('<d',uc.mem_read(x+4,8))[0]))
  elif a==0x797350:
   self.value_strings[x]=self.cstr(y);uc.mem_write(x,bytes((0,4,0,0))+bytes(8));self.ret()
  elif a==0x797250:uc.mem_write(x,bytes((0,5,0,0))+words(y,0));self.ret()
  elif a==0x797488:uc.mem_write(x,bytes((0,2,0,0))+words(z,w));self.ret()
  elif '__node_alloc11_M_allocateERj' in name:self.ret(self.alloc(self.word(x)))
  elif '__node_alloc13_M_deallocate' in name:self.ret()
  elif a==0x797124:self.value_strings.pop(x,None);self.ret()
  elif a==0x3bb7fc:self.ret(self.class_id)
  elif a in (0x3bc784,0x3aeac0):self.ret(self.skill_row)
  elif a==0x508edc:self.ret(self.text)
  elif a==0x4c4bdc:self.services.append([name,self.cstr(y),self.cstr(z)]);self.ret(0)
  elif a in (0x509aec,0x508ef4):
   self.services.append([name,self.cstr(z)])
   p=self.alloc(32);uc.mem_write(p,b'FIXTURE\0');c.pointer(y,p+32);c.pointer(y+16,p+7);c.pointer(y+20,p);self.ret()
  elif a==0x3d7e88:uc.mem_write(w,struct.pack('<f',0.5));self.ret()
  elif a==0x3fc61c:self.ret(self.item)
  elif a in (0x3f9e94,0x3f9f88):self.ret(self.text)
  elif a==0x3f9e80:self.ret(getattr(self,'power_count',0))
  elif a==0x3fa330:self.ret(1)
  elif a==0x3f9e58:self.ret(0)
  elif a==0x337888:self.services.append([name]);self.ret()
  elif a==0x337a88:self.services.append([name,self.cstr(self.word(y+20))]);self.ret(0)
  elif a==0x3df6e0 and getattr(self,'stat_entry',0) and not getattr(self,'full_stats',False):self.services.append([name,y,z]);self.ret(self.stat_points if y==148 else 10)
  elif a==0x3e0798 and getattr(self,'stat_entry',0) and not getattr(self,'full_stats',False):
   self.services.append([name,y,z if z<0x80000000 else z-0x100000000]);self.ret()
  elif a==0x3e087c and getattr(self,'stat_entry',0) and not getattr(self,'full_stats',False):self.services.append([name,y]);self.ret()
  elif a==0x7abe0c:self.services.append([name,x,y,self.cstr(z),w]);self.ret()
  elif a==0x3bb7e8:
   p=self.alloc(16);uc.mem_write(p,b'PLAYER\0');self.ret(p)
  elif getattr(self,'full_stats',False) and name.startswith(('_ZNK14CharProperties','_ZN14CharProperties')):return
  elif a!=getattr(self,'stat_entry',0) and name.startswith(('_ZNK9Character','_ZN9Character','_ZNK14CharProperties','_ZN14CharProperties','_ZNK13ItemInventory','_ZN13ItemInventory','_ZNK14PlayerSavegame')):
   self.services.append([name,x,y,z,w]);self.ret(10)
  elif name.startswith('_ZNK7gameswf8as_value') or name.startswith('_ZN7gameswf8as_value'):
   raise RuntimeError(('unrecovered AS service',hex(a),name))
  else:super().hook(uc,a,size,u)
 def run(self,name,class_id=0x122):
  self.trace=[];self.services=[];self.class_id=class_id
  counts={'NativeInvEquipItem':3,'NativeInvUnequipItem':2,'NativeInvAutoEquipSlot':2,'NativeSkillsGetSkillPointsLeft':1,'NativeGetPlayerStats':2,'NativeGetSkillDetails':3,'NativeInvGetItemDetails':3,'NativeEquipSkill':3,'NativeSkillsTrainSkill':2,'NativeSwapEquipment':1}
  self.c.pointer(self.fn+16,counts.get(name,3))
  for i in range(10):self.number(self.args+i*12,0)
  if name in ('NativeGetSkillDetails','NativeInvGetItemDetails','NativeGetPlayerStats'):
   object_index=9 if name=='NativeGetPlayerStats' else 8
   self.c.uc.mem_write(self.args+object_index*12,bytes((0,5,0,0))+words(self.object,0))
  else:
   for j in range(5):self.number(self.args+(9-j)*12,100+j)
  self.active=True
  try:self.c.invoke(self.entries[name],[self.fn],budget=2000000)
  except Exception:
   print('last source service',getattr(self,'last_entry',None),file=sys.stderr);raise
  finally:self.active=False
  return dict(callback=name,class_id=class_id,writes=self.trace,services=self.services)
if __name__=='__main__':
 if '--schema-variants' in sys.argv:
  m=Original();out=[]
  for actor in (263,290,325):
   for callback in ('NativeGetPlayerStats','NativeGetSkillDetails','NativeInvGetItemDetails'):
    for powers in ((0,2) if callback=='NativeInvGetItemDetails' else (0,)):
     m.power_count=powers;out.append(m.run(callback,actor))
  (ROOT/'port/engine-ui/reference/character-menu-native-v1/schema-variants-v1.json').write_text(json.dumps(out,indent=2))
  print(json.dumps(dict(validation='PASS',original_query_cases=len(out),write_counts=[len(v['writes']) for v in out])));sys.exit(0)
 if '--stat-actions' in sys.argv:
  m=Original();out=[]
  for actor in (263,290,325):
   m.c.uc.mem_write(m.character+0x13c8,struct.pack('<h',actor))
   for stat,label in enumerate(('Str','Dex','End','Nrg')):
    for points in (-1,0,1,2,2147483647):
     m.stat_points=points;m.stat_entry=m.entries['_ZN9Character10IncStat'+label+'Ev'];m.trace=[];m.services=[];m.active=True
     try:m.c.invoke(m.stat_entry,[m.character])
     finally:m.active=False
     out.append(dict(actor=actor,stat=stat,points=points,services=m.services))
  (ROOT/'port/engine-ui/reference/character-menu-native-v1/stat-actions-fixture-v1.json').write_text(json.dumps(out,indent=2))
  print(json.dumps(dict(validation='PASS',original_stat_action_cases=len(out))));sys.exit(0)
 if '--actions' in sys.argv:
  m=Original();names=['NativeEquipSkill','NativeInvEquipItem','NativeInvUnequipItem','NativeInvAutoEquipSlot','NativeSwapEquipment','NativeSkillsTrainSkill','NativeSkillsGetSkillPointsLeft']
  out=[m.run(n) for n in names]
  (ROOT/'port/engine-ui/reference/character-menu-native-v1/actions-fixture-v1.json').write_text(json.dumps(out,indent=2))
  print(json.dumps(out));sys.exit(0)
 m=Original();out=m.run(sys.argv[1] if len(sys.argv)>1 else 'NativeGetPlayerStats')
 print(json.dumps(out,indent=2))
