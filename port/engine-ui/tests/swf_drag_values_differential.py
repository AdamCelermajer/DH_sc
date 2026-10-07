"""Source full 2D do_mouse_drag instruction arithmetic against optimized native.

Root drag/mouse/world/set-state/set-matrix virtuals use explicit fixtures;
source weak parent and both source inverse routines execute. This proves field
output, bounds/IEEE selection and delayed initialization, not a 3D scene mouse.
"""
import argparse,json,math,random,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from swf_cursor_input_differential import InputCpu,words,sha
from body_transform_differential import equal
ROOT=Path(__file__).resolve().parents[1]
class Machine:
 def __init__(self,p):
  self.c=c=InputCpu(p,False,{'functions':[]});d=c.data;self.ch=d+0x1000;self.root=d+0x2000;self.parent=d+0x3000;self.vt=d+0x4000;self.local=d+0x5000;self.world=d+0x6000;self.parent_world=d+0x7000;self.proxy=d+0x8000
  c.pointer(self.ch,self.vt);c.pointer(self.root,self.vt);c.pointer(self.parent+0x54,0);c.pointer(self.ch+0x54,0);c.pointer(self.ch+0x4c,self.local)
  for off,cb in((0xdc,48),(0xe0,64),(0x134,80),(0x70,96)):c.pointer(self.vt+off,c.callback+cb)
  c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def callback(self,a):
  c=self.c;cb=a-c.callback
  if cb==48:c.uc.mem_write(c.reg(1),self.drag)
  elif cb==64:self.drag=bytes(c.uc.mem_read(c.reg(1),32));self.sets+=1
  elif cb==80:c.put(0,self.root)
  elif cb==96:
   for i,v in enumerate(self.mouse):c.uc.mem_write(c.reg(i+1),words(v))
   c.uc.mem_write(c.reg(3),words(0))
  else:raise AssertionError(cb)
 def hook(self,uc,a,size,unused):
  c=self.c
  if a==0x753f74:c.put(0,self.world if c.reg(0)==self.ch else self.parent_world);uc.reg_write(c.pc,uc.reg_read(c.lr))
  elif a==0x4121f8:self.result=bytes(c.uc.mem_read(c.reg(1),24));uc.reg_write(c.pc,uc.reg_read(c.lr))
  elif a==0x75e52c:self.local_mouse=bytes(c.uc.mem_read(uc.reg_read(c.sp)+0x74,8))
 def run(self,row):
  c=self.c;self.mouse=row['mouse'];self.sets=0;self.result=None;self.local_mouse=None;init,lock,bounded=row['flags'];bounds=row['bounds'];self.drag=words(self.ch)+bytes([init,lock,bounded,0])+struct.pack('<4f',bounds[0],bounds[2],bounds[1],bounds[3])+struct.pack('<2f',*row['offset'])
  for p,m in((self.local,row['local']),(self.world,row['world']),(self.parent_world,row['parent'])):c.uc.mem_write(p,struct.pack('<6f',*m))
  c.pointer(self.ch+0x40,self.parent if row['has_parent'] else 0);c.pointer(self.ch+0x3c,self.proxy if row['has_parent'] else 0);c.uc.mem_write(self.proxy,words(20)+b'\1');c.pointer(self.parent+0x40,0);c.uc.mem_write(self.ch+0x9d,b'\0');c.invoke(0x75e3a8,[self.ch]);assert self.result is not None
  return self.result+self.drag[24:32]+self.local_mouse+words(self.drag[4]),self.sets
def main():
 p=argparse.ArgumentParser();[p.add_argument('--'+k,type=Path,required=True)for k in('engine','library','gold','report')];a=p.parse_args();old=Machine(a.engine);new=InputCpu(a.library,True,{'functions':[]});src=new.data+0x1000;out=new.data+0x2000;rng=random.Random(20261005);records=[];sets=0
 for k in range(2400):
  parent=[1.,0.,rng.uniform(-100,100),0.,1.,rng.uniform(-100,100)]if k%5 else[2.,.3,12.,-.2,.7,70.]
  row={'mouse':[rng.randrange(-2147483648,2147483648)if k%13==0 else rng.randrange(-400,600)for _ in range(2)],'world':[rng.choice([0.,1.,2.]),rng.choice([0.,.3]),rng.uniform(-200,200),rng.choice([0.,-.4]),rng.choice([0.,1.,.7]),rng.uniform(-200,200)],'parent':parent,'local':[1.,.3,rng.uniform(-200,200),-.2,1.,rng.uniform(-200,200)],'offset':[rng.uniform(-200,200),rng.uniform(-200,200)],'bounds':[rng.uniform(-30,10),rng.uniform(-10,30),rng.uniform(-30,10),rng.uniform(-10,30)],'flags':[k%2,(k//2)%2,(k//4)%2],'has_parent':k%7!=0}
  if not row['has_parent']:row['parent']=[1.,0.,0.,0.,1.,0.]
  if k%17==0:
   vals=[math.inf,-math.inf,math.nan,-0.,0.];row['bounds'][k%4]=vals[(k//17)%5];row['offset'][k%2]=vals[(k//17)%5]
  expected,count=old.run(row);payload=struct.pack('<2i24f4B',*row['mouse'],*row['world'],*row['parent'],*row['local'],*row['bounds'],*row['offset'],*row['flags'],0);new.uc.mem_write(src,payload);assert new.invoke('dh2_ui_swf_drag_values',[out,src])==0;actual=bytes(new.uc.mem_read(out,44));assert equal(expected[:40],actual[:40])and expected[40:]==actual[40:],(k,row,expected.hex(),actual.hex());records.append({'input':payload.hex(),'state':expected.hex(),'source_set_drag_callbacks':count});sets+=count
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_text(json.dumps(records,separators=(',',':'))+'\n');report={'validation':'PASS','comparisons':len(records),'source_delayed_set_drag_callbacks':sets,'mismatches':0,'original_sha256':sha(a.engine),'arm64_library_sha256':sha(a.library),'gold_sha256':sha(a.gold),'scope':__doc__,'source_sha256':{str(x.relative_to(ROOT.parents[1])).replace('\\','/'):sha(x)for x in[ROOT/'swf_drag_values.hpp',ROOT/'swf_drag_values.cpp',Path(__file__)]}};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
