"""Whole original NativeReloadSkills caller; AS/EABI/player/reload boundaries
are explicit fixtures. Original instructions select arity, order and null guard.
"""
from pathlib import Path
import sys,struct,json,hashlib
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from character_menu_native_v1_original import Original,words
class ActionOriginal(Original):
 def __init__(self):
  super().__init__();self.action_active=False;self.records=[]
 def hook(self,uc,address,size,user):
  if not self.action_active:return super().hook(uc,address,size,user)
  if address==0x797a54:
   assert self.c.reg(0)==self.args+9*12
   self.records.append((1,0));lo,hi=struct.unpack('<II',struct.pack('<d',self.value))
   self.c.put(0,lo);self.c.put(1,hi);uc.reg_write(self.c.pc,uc.reg_read(self.c.lr));return
  if address==0x30ea24:
   # Observe the actual PLT entry before Cpu's registered external callback.
   # The ordinary external EABI fixture still delivers the conversion.
   value=struct.unpack('<d',words(self.c.reg(0),self.c.reg(1)))[0]
   self.records.append((2,int(value)));return
  if address==0x43c388:
   assert self.c.reg(1)==0
   index=self.c.reg(0);index=index if index<0x80000000 else index-0x100000000
   self.records.append((3,index));self.ret(self.character if self.present else 0);return
  if address==0x3a9db4:
   assert self.c.reg(0)==self.character
   self.records.append((4,0));self.ret();return
 def run_case(self,count,kind,value,present):
  self.value=value;self.present=present;self.records=[]
  self.c.pointer(self.fn+16,count)
  for i in range(10):self.number(self.args+i*12,value)
  self.c.uc.mem_write(self.args+9*12+1,bytes([kind]))
  self.number(self.output,99.5);before=self.read(self.output,12)
  self.action_active=True
  try:self.c.invoke(0x43dbb8,[self.fn])
  finally:self.action_active=False
  assert self.read(self.output,12)==before
  return self.records
machine=ActionOriginal();cases=[]
for count in (0,1,2,3,7):
 for kind in (0,1,2,4,5,6):
  for value in (-2147483648.,-9.75,-1.,0.,0.75,1.,9.9,2147483647.):
   for present in (0,1):cases.append((count,kind,value,present))
blob=words(0x31415243,len(cases));boundaries=0
for count,kind,value,present in cases:
 records=machine.run_case(count,kind,value,present);boundaries+=len(records)
 blob+=struct.pack('<IIdII',count,kind,value,present,len(records))
 blob+=b''.join(words(op,index) for op,index in records)
ref=ROOT/'port/engine-ui/reference/character-menu-flow-v1'
(ref/'reload-action-gold-v1.bin').write_bytes(blob)
report={'validation':'PASS','original_complete_cases':len(cases),'ordered_boundaries':boundaries,
 'gold_sha256':hashlib.sha256(blob).hexdigest(),'AS_EABI_player_reload_services_are_fixtures':True,
 'nonfinite_or_out_of_range_EABI_semantics_proven':False,'live_character_menu':False}
(ref/'reload-action-original-v1.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
