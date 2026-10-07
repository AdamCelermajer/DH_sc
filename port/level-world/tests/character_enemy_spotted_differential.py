"""Complete original EnemySpotted prefix vs O2 ARM64, with real design input.

Actual DesignSettings table/row readers decode EnemySpottedAggro from cache.
Group, state/combat/player, aggro map and active AIS bodies remain explicit
synchronous services, not accepted defaults or a live world/script claim.
"""
import argparse,hashlib,itertools,json,struct,sys,zipfile
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-enemy-spotted';SCRATCH=REPO/'.local-inputs/character-enemy-spotted';CACHE=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original,words,strings
from navigation_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Oracle:
 def __init__(self,library):
  self.old=Original(REPO/'.local-inputs/libDungeonHunter2.so',json.loads((REF/'original-functions.json').read_text()));self.new=Cpu(library,True,{'functions':[]});c=self.old;self.ai=c.data+0x10000;self.owners=[c.data+0x20000,c.data+0x24000];self.enemy=c.data+0x28000;self.active=[c.data+0x30000,c.data+0x31000];self.vtables=[c.data+0x32000,c.data+0x33000];self.callbacks=[c.data+0x40000,c.data+0x40020];self.group=c.data+0x45000
  assert sha(CACHE)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679';SCRATCH.mkdir(exist_ok=True)
  with zipfile.ZipFile(CACHE) as z:
   for name in ('design_pyarray.bin','design_pystructnames.bin'):raw=z.read('com.gameloft.android.GAND.GloftD2SS/files/data/pydata/'+name);(SCRATCH/name).write_bytes(raw)
  c.blob=(SCRATCH/'design_pyarray.bin').read_bytes();c.cursor=0;c.invoke(0x4b3cd0,[c.stream]);self.design_consumed=c.cursor;assert c.cursor==176
  got=0x3d14cc+self.word(c,0x3d167c);self.design_global=self.word(c,got+self.word(c,0x3d168c));self.design_table=self.word(c,self.design_global);self.threat=self.word(c,self.design_table+48);assert self.threat==0x41200000
  self.other_design=c.data+0x46000;c.uc.mem_write(self.other_design,bytes(c.uc.mem_read(self.design_table,176)));c.uc.mem_write(self.other_design+48,words(self.threat^0x80000000))
  self.field_names=strings((SCRATCH/'design_pystructnames.bin').read_bytes())[0];assert self.field_names[11]==b'EnemySpottedAggro';self.enabled=True;c.uc.hook_add(UC_HOOK_CODE,self.old_hook)
  n=self.new;self.state=n.data+0x1000;self.ai_view=n.data+0x1100;self.target=n.data+0x1200;self.nowners=[n.data+0x1300,n.data+0x1400];self.services=n.data+0x1500;self.callback=n.data+0x1600;self.threats=[n.data+0x1700,n.data+0x1800];n.uc.mem_write(self.threats[0],words(self.threat));n.uc.mem_write(self.threats[1],words(self.threat^0x80000000));n.uc.mem_write(self.callback,bytes.fromhex('c0035fd6'));n.uc.hook_add(UC_HOOK_CODE,self.new_hook)
 def word(self,c,p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
 def text(self,c,p):
  b=b''
  while c.uc.mem_read(p+len(b),1)!=b'\0':b+=bytes(c.uc.mem_read(p+len(b),1))
  return b.decode()
 def snapshot(self,native):
  c=self.new if native else self.old
  if native:
   owner=struct.unpack('<Q',c.uc.mem_read(self.target+8,8))[0];active=struct.unpack('<Q',c.uc.mem_read(self.ai_view+8,8))[0];group=struct.unpack('<Q',c.uc.mem_read(self.state+8,8))[0];threat=struct.unpack('<Q',c.uc.mem_read(self.services+16,8))[0];return [self.nowners.index(owner)+1,active,group,c.uc.mem_read(self.ai_view+16,1)[0],c.uc.mem_read(self.target+42,1)[0],self.threats.index(threat)]
  active=self.word(c,self.ai+28);return [self.owners.index(self.word(c,self.ai+4))+1,0 if not active else self.active.index(active)+31,51 if self.word(c,self.ai+52) else 0,c.uc.mem_read(self.ai+120,1)[0],c.uc.mem_read(self.ai+76,1)[0],int(self.word(c,self.design_global)!=self.design_table)]
 def mutate(self,native,op):
  if self.triggered or self.row[10]!=op:return
  self.triggered=True;c=self.new if native else self.old;mode=self.row[11]
  if mode in (1,2):
   if native:c.uc.mem_write(self.ai_view+8,struct.pack('<Q',0 if mode==1 else 32))
   else:c.pointer(self.ai+28,0 if mode==1 else self.active[1])
  elif mode==3:
   if native:c.uc.mem_write(self.target+8,struct.pack('<Q',self.nowners[1]))
   else:c.pointer(self.ai+4,self.owners[1])
  elif mode==4:
   self.trace.append([12,*([0]*11)]);saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
   try:
    if native:assert c.invoke('dh2_character_enemy_spotted',[self.state,3,self.services])==0
    else:c.invoke(0x3d14b4,[self.ai,self.enemy])
   finally:c.stack=stack;c.uc.context_restore(saved)
   self.trace.append([13,*([0]*11)])
  elif mode==5:
   if native:c.uc.mem_write(self.state+8,struct.pack('<Q',0))
   else:c.pointer(self.ai+52,0)
  elif mode==6:
   if native:c.uc.mem_write(self.services+16,struct.pack('<Q',self.threats[1]))
   else:c.pointer(self.design_global,self.other_design)
 def observe(self,native,entry):self.trace.append([*entry,*self.snapshot(native)]);self.mutate(native,entry[0])
 def old_hook(self,uc,a,size,_):
  c=self.old;op=None;subject=other=enemy=word=text=0
  if a==0x337888:op=0
  elif a==0x3140ec:
   op=1;name=self.text(c,c.reg(1));text={'isTracingCharAIEvents':1,'isTracingThreatChange':2}[name];c.uc.mem_write(c.reg(0),bytes(24));c.pointer(c.reg(0)+20,c.reg(1))
  elif a==0x337a88:op=2;text={'isTracingCharAIEvents':1,'isTracingThreatChange':2}[self.text(c,self.word(c,c.reg(1)+20))]
  elif a==0x3139ac:op=3;text={'isTracingCharAIEvents':1,'isTracingThreatChange':2}[self.text(c,self.word(c,c.reg(0)+20))]
  elif a==0x3d27cc:op=4;subject=51;other=self.owners.index(c.reg(1))+1;assert c.reg(0)==self.group and c.reg(2)==self.enemy;enemy=3
  elif a in (0x3c0230,0x3c01c0):op=5 if a==0x3c0230 else 6;obj=c.reg(0)-0x4fc;subject=3 if obj==self.enemy else self.owners.index(obj)+1
  elif a==0x3d4bc4:op=7;subject=11;assert c.reg(0)==self.ai
  elif a==self.callbacks[0]:op=8;subject=3;assert c.reg(0)==self.enemy
  elif a==0x3d4ac8:op=9;subject=11;enemy=3;assert c.reg(0)==self.ai and c.reg(1)==self.enemy
  elif a==0x3d7c68:op=10;subject=self.owners.index(c.reg(0)-0x3c8)+1;enemy=3;word=c.reg(2);assert c.reg(1)==self.enemy
  elif a==self.callbacks[1]:op=11;subject=self.active.index(c.reg(0))+31;enemy=3;assert c.reg(1)==self.enemy
  if op is None:return
  self.observe(False,[op,subject,other,enemy,word,text]);value={5:self.row[1 if subject==3 else 2],6:self.row[3 if subject==3 else 4],7:self.row[5],8:self.row[6],9:self.row[7],10:self.row[8]}.get(op,0);c.returned(value)
 def new_hook(self,uc,a,size,_):
  if a!=self.callback:return
  c=self.new;op,reserved,subject,other,enemy,text,word,res2=struct.unpack('<II4QII',uc.mem_read(c.reg(2),48));assert reserved==res2==0;name={'isTracingCharAIEvents':1,'isTracingThreatChange':2}[self.text(c,text)] if text else 0;self.observe(True,[op,subject,other,enemy,word,name]);value={5:self.row[1 if subject==3 else 2],6:self.row[3 if subject==3 else 4],7:self.row[5],8:self.row[6],9:self.row[7],10:self.row[8]}.get(op,0);uc.mem_write(c.reg(3),words(value));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 def execute(self,row,native):
  self.row=row;self.trace=[];self.triggered=False;c=self.new if native else self.old
  if native:
   for i,p in enumerate(self.nowners):c.uc.mem_write(p,struct.pack('<QHHI',i+1,0,0,0))
   c.uc.mem_write(self.target,struct.pack('<5Q4BI',11,self.nowners[0],0,0,0,1,1,77,0,0));c.uc.mem_write(self.ai_view,struct.pack('<4Q',self.target,row[9],1,0));c.uc.mem_write(self.state,struct.pack('<QQ',self.ai_view,51 if row[0] else 0));c.uc.mem_write(self.services,struct.pack('<3Q',0,self.callback,self.threats[0]));assert c.invoke('dh2_character_enemy_spotted',[self.state,3,self.services])==0
  else:
   c.uc.mem_write(self.ai,bytes(0x100));c.pointer(self.ai+4,self.owners[0]);c.pointer(self.ai+28,self.active[row[9]-31] if row[9] else 0);c.pointer(self.ai+52,self.group if row[0] else 0);c.uc.mem_write(self.ai+120,b'\x01');c.uc.mem_write(self.ai+76,b'M');c.pointer(self.design_global,self.design_table);c.pointer(self.enemy,self.vtables[0]);c.pointer(self.vtables[0]+40,self.callbacks[0]);
   for p in self.active:c.pointer(p,self.vtables[1])
   c.pointer(self.vtables[1]+52,self.callbacks[1]);c.invoke(0x3d14b4,[self.ai,self.enemy])
  return self.snapshot(native),self.trace.copy()
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();o=Oracle(a.library);base=[1,0,0,0,0,1,1,0,0x3f800000,31,0xffffffff,0];cases=[]
 for group,es,os,el,ol,combat,player,active in itertools.product((0,1),repeat=8):v=base.copy();v[:7]=[group,es,os,el,ol,combat,player];v[9]=31 if active else 0;cases.append(v)
 ieee=(0,0x80000000,0x3f800000,0xbf800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345,0xffffffff)
 for get,add in itertools.product(ieee,repeat=2):v=base.copy();v[7:9]=[get,add];cases.append(v)
 for op,mode in itertools.product(range(12),range(1,7)):v=base.copy();v[10:12]=[op,mode];cases.append(v)
 records=[];calls=0
 for row in cases:
  expected=o.execute(row,False);actual=o.execute(row,True);assert expected==actual,(row,expected,actual);records.append([row,*expected]);calls+=len(expected[1])
 gold=b'CES1'+words(len(records),o.threat)
 for row,after,trace in records:gold+=words(*row,*after,len(trace),*(x for entry in trace for x in entry))
 (REF/'enemy-spotted-fixtures.bin').write_bytes(gold)
 report={'validation':'PASS','comparisons':len(records),'ordered_services':calls,'mismatches':0,'original_sha256':sha(REPO/'.local-inputs/libDungeonHunter2.so'),'manifest_sha256':sha(REF/'original-functions.json'),'arm64_sha256':sha(a.library),'corpus_sha256':hashlib.sha256(gold).hexdigest(),'source_sha256':{str(x.relative_to(REPO)):sha(x) for x in (ROOT/'character_enemy_spotted.hpp',ROOT/'character_enemy_spotted.cpp',Path(__file__))},'cache_sha256':sha(CACHE),'design_pyarray_sha256':sha(SCRATCH/'design_pyarray.bin'),'design_names_sha256':sha(SCRATCH/'design_pystructnames.bin'),'genuine_design_member':'EnemySpottedAggro','design_member_index':11,'original_runtime_offset':48,'design_table_consumed':o.design_consumed,'authored_threat_bits':o.threat,'authored_threat':10.0,'scope':__doc__};(ROOT/'reports/character-enemy-spotted-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
