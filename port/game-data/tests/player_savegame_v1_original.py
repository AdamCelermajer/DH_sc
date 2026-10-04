"""Actual constructors/skill ownership/STL/profile readers; explicit storage,
Character SkillList and CharAI.UpdateSkills services. Original map algorithms
execute; name backing is the actual cache dictionary, not a lookup-result stub.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]; SCR=ROOT/'.local-inputs/player-inventory-v1'
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from items_differential import Original,strings
from unicorn import UC_HOOK_CODE
W=lambda *x:struct.pack('<'+'I'*len(x),*(v&0xffffffff for v in x))
class Save(Original):
 def __init__(self):
  super().__init__(ROOT/'.local-inputs/libDungeonHunter2.so',{'functions':[]})
  self.obj=self.data+0x4000;self.character=self.data+0x5000;self.list=self.data+0x7000;self.calls=0
  self.uc.hook_add(UC_HOOK_CODE,self.service)
  self.names=strings((ROOT/'.local-inputs/skill-tables/skills_pyarraynames.bin').read_bytes())[1]
  got=0x46ac8c+self.word(0x46afd4);names=self.allocate(len(self.names)*4)
  for i,n in enumerate(self.names):p=self.allocate(len(n)+1);self.uc.mem_write(p,n+b'\0');self.pointer(names+4*i,p)
  self.pointer(self.word(got+self.word(0x46afdc)),len(self.names));self.pointer(self.word(got+self.word(0x46afe0)),names)
 def allocate(self,n):
  p=self.heap;self.heap+=(max(n,1)+15)&~15;assert self.heap<self.data+0x2000000;self.uc.mem_write(p,bytes(max(n,1)));return p
 def service(self,uc,a,z,u):
  if a==0x310570:self.returned(self.allocate(self.reg(0)))
  elif a==0x708ec0:self.returned(self.allocate(self.word(self.reg(0))))
  elif a==0x708f00:self.returned()
  elif a==0x3bc5fc:self.calls+=1;assert self.reg(0)==self.character;self.returned(self.list)
  elif a==0x3d8a04:self.calls+=1;assert self.reg(0)==self.character+0x3c8;self.returned()
 def external(self,uc,a,z,u):
  if self.imports.get(a)=='strlen':
   p=self.reg(0);n=0
   while uc.mem_read(p+n,1)!=b'\0':n+=1
   self.returned(n)
  elif self.imports.get(a)=='memcmp':
   n=self.reg(2);x=bytes(uc.mem_read(self.reg(0),n));y=bytes(uc.mem_read(self.reg(1),n));self.returned((x>y)-(x<y))
  else:super().external(uc,a,z,u)
 def fresh(self,ids):
  self.uc.mem_write(self.obj,bytes(0x198));self.invoke(0x465ae0,[self.obj]);self.pointer(self.obj+0x10,self.character)
  p=self.allocate(4*len(ids));self.uc.mem_write(p,W(*ids));self.uc.mem_write(self.list,W(0,len(ids),p));self.calls=0
 def snapshot(self):
  n=self.word(self.obj+0x84);p=self.word(self.obj+0x80);rows=[]
  for i in range(n):rows.append(bytes(self.uc.mem_read(p+8*i,7))+b'\0')
  out=W(n)+b''.join(rows);maps=self.word(self.obj+0x88)
  def walk(node):
   if not node:return []
   return walk(self.word(node+8))+[(self.word(node+16),self.word(node+20))]+walk(self.word(node+12))
  for s in range(2):a=walk(self.word(maps+24*s+4));out+=W(len(a))+b''.join(W(*v) for v in a)
  return out+W(self.calls)
def encoded(s):return W(len(s)+1)+s+b'\0'
def main():
 c=Save();r=random.Random(20261004);cases=[];comparisons=0
 for k in range(64):
  ids=[r.randrange(len(c.names)) for _ in range(r.randrange(1,25))];c.fresh(ids);ops=[]
  for j in range(24):
   if j in (0,23):op=W(0);c.invoke(0x469764,[c.obj])
   elif j%5==1:
    i=r.randrange(len(ids));v=r.choice([0,1,-1,32767,32768,65535,65536,0x7fffffff]);op=W(1,i,v);c.invoke(0x4667c4,[c.obj,i,v&0xffffffff])
   elif j%5 in (2,3):
    key=r.randrange(8);value=r.choice([0xffffffff,*range(len(ids))]);op=W(2,key,value);c.invoke(0x4680a8,[c.obj,key,value])
   else:
    levels=[(r.choice(c.names+[b'absent',c.names[ids[0]]+b'\0ignored']),r.randrange(65536)) for _ in range(r.randrange(7))]
    blob=W(len(levels))+b''.join(encoded(n)+struct.pack('<H',v) for n,v in levels)
    for _ in range(2):
     a=[(r.choice([-3,-1,0,1,2,7,0x7fffffff]),r.choice([0xffffffff,0,1,500])) for _ in range(r.randrange(7))];blob+=W(len(a))+b''.join(W(*p) for p in a)
    c.blob=blob;c.cursor=0;c.invoke(0x46ac74,[c.stream,c.obj]);assert c.cursor==len(blob);op=W(3,len(blob))+blob
   snap=c.snapshot();ops.append(W(len(op))+op+W(len(snap))+snap);comparisons+=1
  cases.append(W(len(ids))+W(*ids)+W(len(ops))+b''.join(ops))
 blob=b'PGS1'+W(len(cases))+b''.join(cases);ref=ROOT/'port/game-data/reference/player-savegame-v1';ref.mkdir(parents=True,exist_ok=True);(ref/'fixtures.bin').write_bytes(blob)
 report={'validation':'PASS','comparisons':comparisons,'cases':len(cases),'original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(blob).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'names_sha256':hashlib.sha256((ROOT/'.local-inputs/skill-tables/skills_pyarraynames.bin').read_bytes()).hexdigest(),'scope':__doc__,'native_comparison':False}
 (ref/'original-gold.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
