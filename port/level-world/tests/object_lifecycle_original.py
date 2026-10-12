"""Execute complete original Remove/FakeRemove bodies and map erase instructions.

Fixtures supply caller registry/list ownership and room/AI/destructor services.
This is additional original-only choreography evidence, not a native full manager.
"""
import hashlib,itertools,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original,words
REF=ROOT/'reference/object-identity-lifecycle'
class Probe(Original):
 def __init__(self):
  super().__init__(REPO/'.local-inputs/libDungeonHunter2.so',json.loads((REF/'original-functions.json').read_text()));self.manager=self.data+0x8000;self.context=self.data+0x9000;self.objects=[self.data+0x10000+i*0x4000 for i in range(3)];self.vt=self.data+0x20000;self.app=self.data+0x21000;self.destructor=self.data+0x22000;self.keybuf=self.data+0x23000
  for base,literal,offset in ((0x33dd40,0x33dd68,0x33dd6c),(0x33fde4,0x33fe90,0x33fe94)):self.pointer(base+self.word(literal)+self.word(offset),self.context)
  self.pointer(self.context+0x38,self.manager);self.trace=[];self.uc.hook_add(UC_HOOK_CODE,self.hook)
 def hook(self,uc,a,size,unused):
  if a==0x708ec0:
   n=self.word(self.reg(0));p=self.heap;self.heap+=(n+15)&~15;uc.mem_write(p,bytes(n));self.returned(p)
  elif a==0x708f00:self.returned()
  elif a==0x7fd794:self.returned(self.app)
  elif a in (0x3968cc,0x3462b8,0x3d2ff8,0x3a66f8,self.destructor):
   self.trace.append([hex(a),self.reg(0),self.reg(1)]);self.returned()
 def execute(self,fake,character,persist,room,network,duplicates):
  self.heap=self.data+0x100000;self.trace=[];m=self.manager;self.uc.mem_write(m,bytes(0x200));self.uc.mem_write(self.app,bytes(8));self.uc.mem_write(self.app+5,bytes([network]));tree=m+12;self.uc.mem_write(tree,words(0,0,tree,tree,0));self.pointer(m+0x78,0xffffffff);self.pointer(m+0x50,3)
  for i,obj in enumerate(self.objects):
   self.uc.mem_write(obj,bytes(0x3000));v=self.vt+i*0x100;self.pointer(obj,v);self.pointer(v+4,self.destructor);self.pointer(v+0x20,0x340054);self.pointer(v+0x24,0x3a2e1c if character else 0x33dcd0);self.pointer(v+0x28,0x33dcd8);self.pointer(obj+0xf4,4);self.uc.mem_write(obj+0x2fc,bytes([persist]));self.pointer(obj+0x2f4,self.data+0x30000 if room else 0);shared=obj+0x200;self.pointer(obj+0x2c,shared);self.uc.mem_write(shared,words(i+1,obj,0xffffffff));self.uc.mem_write(self.keybuf,words(i+1));record=self.invoke(0x33fc88,[tree,self.keybuf]);self.pointer(record+0x18,obj)
  obj=self.objects[1];lists={}
  for offset in (4,0x2c,0x34,0x3c,0x44,0x60,0x68,0x70,0x90,0x100,0x120):
   sent=m+offset;values=[] if offset in (4,0x34,0x3c,0x44,0x68,0x120) else [self.objects[0],obj,obj] if duplicates else [obj];nodes=[]
   for value in values:p=self.heap;self.heap+=16;nodes.append(p)
   self.uc.mem_write(sent,words(nodes[0] if nodes else sent,nodes[-1] if nodes else sent))
   for i,p in enumerate(nodes):self.uc.mem_write(p,words(nodes[i+1] if i+1<len(nodes) else sent,nodes[i-1] if i else sent,values[i]))
   lists[offset]=sent
  self.invoke(0x349240 if fake else 0x348ea4,[m,2,obj,0xffffffff],budget=2000000)
  after={}
  for offset,sent in lists.items():
   values=[];p=self.word(sent);previous=sent
   while p!=sent:
    assert self.word(p+4)==previous;value=self.word(p+8);values.append(self.objects.index(value)+1 if value in self.objects else value);previous=p;p=self.word(p);assert len(values)<10
   assert self.word(sent+4)==previous;after[hex(offset)]=values
  keys=[];bindings=[]
  def walk(p):
   if not p:return
   walk(self.word(p+8));keys.append(self.word(p+16));value=self.word(p+44);bindings.append([self.word(p+16),self.objects.index(value)+1 if value in self.objects else value]);walk(self.word(p+12))
  walk(self.word(tree+4));return {'frame':self.word(m+0x78),'count':self.word(m+0x50),'disabled':self.uc.mem_read(obj+0x81,1)[0],'keys':keys,'bindings':bindings,'lists':after,'calls':self.trace.copy()}
def main():
 p=Probe();cases=[]
 for row in itertools.product((0,1),repeat=6):
  result=p.execute(*row);assert result['bindings']==([[1,1],[2,0],[3,3]] if row[0] else [[1,1],[3,3]]);assert result['frame']==(0xffffffff if row[0] else 0);assert result['count']==(3 if row[0] else 2);assert bool(result['lists']['0x4'])==bool(not row[0] and row[2]);destruct=sum(c[0]==hex(p.destructor) for c in result['calls']);assert destruct==int(not row[0] and not row[2]);cases.append({'input':list(row),'output':result})
 report={'validation':'PASS','original_only_cases':len(cases),'original_sha256':hashlib.sha256((REPO/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'manifest_sha256':hashlib.sha256((REF/'original-functions.json').read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,'cases':cases}
 (REF/'removal-original-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='cases'}))
if __name__=='__main__':main()
