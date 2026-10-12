"""Actual saved-slot/level, potion, cooldown and AI HUD getter instructions.
Script VCB and selected-spell calls are required borrowed services, not fabricated
script producers. Successful indexed domains only; native malformed guards are
covered separately. Timer arithmetic executes actual TMR_TimeLeft instructions.
"""
import argparse,hashlib,json,random,struct,sys,math
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../level-world/tests'))
from visual_timeline_differential import TimelineCpu
from navigation_search_differential import word,words
from aggro_differential import float_bits
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def signed(x):return x if x<0x80000000 else x-0x100000000
class Cpu(TimelineCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_ui2f':self.put(0,float_bits(float(self.reg(0))));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  super().external(uc,address,size,unused)
class Machine:
 def __init__(self,path,native,manifest):
  self.c=Cpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.owner=d+0x2000;self.rows=d+0x5000;self.out=d+0x6000;self.tree=d+0x7000;self.svc=d+0x8000;self.nodes=[d+0x9000+i*0x100 for i in range(7)]
  c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def call(self,op,index=0,level=0):
  self.events.append([op,index,signed(level&0xffffffff)])
  return (self.cfg[2] if self.events.count([1,0,0])==1 else self.cfg[3]) if op==1 else self.cfg[4] if op==2 else self.cfg[6] if op==3 else self.cfg[7]
 def callback(self,address):
  c=self.c;assert address==c.callback+32
  q=c.reg(2);op,index,level,_=struct.unpack('<4I',c.uc.mem_read(q,16));v=self.call(op,index,level)
  c.uc.mem_write(c.reg(3),words(v if op!=4 else 0,v if op==4 else 0));c.put(0,1)
 def hook(self,uc,address,size,unused):
  if self.native:return
  c=self.c
  if address in(0x3c02e8,0x3c0334):
   # Execute the actual pure predicates; caller controls genuine SM current.
   c.pointer(self.owner+0x51c,self.owner+0x1000);c.pointer(self.owner+0x1000,self.cfg[2] if address==0x3c02e8 else self.cfg[3]);self.events.append([1,0,0])
  elif address==0x3bb98c:self.ret(self.call(2,0xffffffff))
  elif address==0x3da9dc:self.ret(self.call(3,self.cfg[1],0))
  elif address==0x3daca8:
   v=self.call(4,self.cfg[1] if self.cfg[0]%2==0 else self.cfg[4],c.reg(1));c.uc.mem_write(c.reg(2),words(v));self.ret()
  elif address==0x30e004:self.ret()
 def query(self,cfg):
  self.cfg=cfg;self.events=[];c=self.c;op,index,a,b,selected,stage,usable,fraction,flags,present=cfg
  c.uc.mem_write(self.rows,struct.pack('<4Q',*(self.nodes[0] if present else 0 for _ in range(4))) if self.native else words(*(self.nodes[0] if present else 0 for _ in range(4))))
  c.uc.mem_write(self.out,words(0x778899aa,0x7fc12345))
  if self.native:
   c.uc.mem_write(self.owner,struct.pack('<QII',1,flags,0));c.uc.mem_write(self.s,struct.pack('<QiIQIIQII',self.owner,stage,0,self.rows,4,0,self.rows,4,0));c.uc.mem_write(self.svc,struct.pack('<QQ',0,c.callback+32))
   assert c.invoke('dh2_ui_hud_skill_query',[self.s,op,index,123,self.out,self.svc])==0
   result=word(c,self.out+(0 if op<2 else 4))
  else:
   c.pointer(self.s+4,self.owner);c.pointer(self.owner+0x520,flags);c.pointer(self.s+0x28,stage&0xffffffff);c.pointer(self.s+0xb4,self.rows);c.pointer(self.s+0xb8,self.rows+16);c.pointer(self.s+0xc0,self.rows);c.pointer(self.s+0xc4,self.rows+16)
   addr=(0x3d8358,0x3d80b4,0x3d7e88,0x3d7da8)[op]
   result=c.invoke(addr,([self.s,index],[self.s],[self.s,index,123,self.out],[self.s,self.out])[op]);result=result if op<2 else word(c,self.out)
  return result,self.events
 def potion(self,present,value):
  c=self.c;c.uc.mem_write(self.nodes[0]+0x50,struct.pack('<h',value))
  if self.native:
   c.uc.mem_write(self.rows,struct.pack('<h',value));assert c.invoke('dh2_ui_hud_num_potions',[self.rows if present else 0,self.out])==0;return word(c,self.out)
  c.pointer(self.s+0x24,self.nodes[0] if present else 0);return c.invoke(0x3fc690,[self.s])
 def level(self,index,present,value):
  c=self.c;raw=struct.pack('<IHH',0,value,0)*8;c.uc.mem_write(self.rows,raw)
  if self.native:
   c.uc.mem_write(self.s,struct.pack('<QII',self.rows if present else 0,8,0));assert c.invoke('dh2_ui_hud_skill_level',[self.s,index,self.out])==0;return word(c,self.out)
  c.pointer(self.s+0x80,self.rows if present else 0);c.pointer(self.s+0x84,8);return c.invoke(0x4668dc,[self.s,index])
 def slot(self,index,keys,values):
  c=self.c
  def build(lo,hi):
   if lo>=hi:return 0
   mid=(lo+hi)//2;p=self.nodes[mid];left=build(lo,mid);right=build(mid+1,hi)
   c.uc.mem_write(p,struct.pack('<QQii',left,right,keys[mid],values[mid]) if self.native else words(0,0,left,right,keys[mid]&0xffffffff,values[mid]&0xffffffff));return p
  root=build(0,len(keys))
  if self.native:
   c.uc.mem_write(self.tree,struct.pack('<QII',root,len(keys),0));assert c.invoke('dh2_ui_hud_skill_slot',[self.tree,index&0xffffffff,self.out])==0;return word(c,self.out)
  c.pointer(self.s+0x10,self.owner);c.pointer(self.s+0x88,self.tree);c.pointer(self.tree+4,root);return c.invoke(0x467488,[self.s,index&0xffffffff])
 def cooldown(self,index,elapsed,duration,active):
  c=self.c
  if self.native:
   raw=struct.pack('<IiIIBBHiQ',0,0,duration,elapsed,active,0,0,-1,0);c.uc.mem_write(self.rows,raw);c.uc.mem_write(self.tree,struct.pack('<QIIQII',self.rows,1,1,1,0,0));c.uc.mem_write(self.s,struct.pack('<QiI',self.tree,index,0));assert c.invoke('dh2_ui_hud_cooldown',[self.s,self.out])==0;return word(c,self.out)
  c.pointer(self.s+4,self.owner);c.pointer(self.s+0x18,index&0xffffffff);c.pointer(self.owner+0x3bc,self.rows);c.pointer(self.owner+0x3c0,self.rows+32);c.uc.mem_write(self.rows,bytes(32));c.pointer(self.rows+0xc,duration);c.pointer(self.rows+0x10,elapsed);c.uc.mem_write(self.rows+0x14,bytes((active,)));return c.invoke(0x3da3d0,[self.s])
def same(a,b):return a==b or (a&0x7f800000==0x7f800000 and a&0x7fffff and b&0x7f800000==0x7f800000 and b&0x7fffff)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();manifest=json.loads((ROOT/'reference/hud-manager/backends/original-functions.json').read_text());manifest['functions']+=json.loads((ROOT/'reference/hud-manager/backend-dependencies/original-functions.json').read_text())['functions'];assert sha(a.engine)==manifest['original_sha256'];old=Machine(a.engine,False,manifest);new=Machine(a.library,True,{'functions':[]});rng=random.Random(20261007);rows=[]
 def check(kind,cfg,x,y):
  assert x==y or kind=='cooldown' and same(x,y),(kind,cfg,x,y);rows.append({'kind':kind,'input':cfg,'after':x})
 for n in range(512):
  cfg=[n%4,n%4,rng.choice((3,6,7)),rng.choice((3,6,7)),n%4,rng.choice((-1,0,6,7,8)),rng.choice((0,1,255,0xffffffff)),float_bits(rng.choice((0.,.3,1.,-1.,math.nan))),rng.choice((0,0x8000)),n%3!=0]
  x,events=old.query(cfg);y,calls=new.query(cfg);assert events==calls,(cfg,events,calls);check('query',cfg,x,y);rows[-1]['calls']=events
 for n in range(128):
  v=rng.choice((-32768,-1,0,1,32767));check('potion',[n%2,v],old.potion(n%2,v),new.potion(n%2,v))
  v=rng.choice((0,1,32768,65535));check('level',[n%8,n%2,v],old.level(n%8,n%2,v),new.level(n%8,n%2,v))
  keys=[-2147483648,-3,-1,0,1,3,2147483647];values=[rng.randrange(-100,100) for _ in keys];i=rng.choice(keys+[-4,-2,2,4]);check('slot',[i,keys,values],old.slot(i,keys,values),new.slot(i,keys,values))
 for n in range(512):
  cfg=[rng.choice((-1,0,1,2147483647)),rng.choice((0,1,199,200,0x7fffffff,0x80000000,0xffffffff)),rng.choice((0,1,200,0xffffffff)),rng.choice((0,1,255))];check('cooldown',cfg,old.cooldown(*cfg),new.cooldown(*cfg))
 a.output.mkdir(parents=True,exist_ok=True);gold=a.output/'backends-gold.json';gold.write_text(json.dumps({'validation':'PASS','rows':rows},separators=(',',':'))+'\n');report=dict(validation='PASS',comparisons=len(rows),ordered_services=sum(len(x.get('calls',[])) for x in rows),original_sha256=sha(a.engine),library_sha256=sha(a.library),gold_sha256=sha(gold),scope=__doc__);(a.output/'backends-report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
