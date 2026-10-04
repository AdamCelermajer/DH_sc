"""Execute source auto/equip/unequip, metadata and set/stack-byte algorithms.
Split, force-add, matching/merge, removal and Character effects are explicit
controlled services. Real Item.GetItem/GetCurrentEquipSet execute in ARM32.
No live visual/item-effect/campaign parity is claimed.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from item_inventory_v1_original import Inventory,W
from navigation_differential import Cpu
from unicorn import UC_HOOK_CODE
OPS=(0x3fc3e0,0x3ff5d4,0x3fe1cc,0x3fdaf0,0x3fa17c,0x3fe7d8,0x3e08a8,0x3a999c,0x3bd140)
ADDR=(0x400c84,0x3a9fa8,0x400634,0x4003a4,0x4001a0,0x4002d8)
EXPORT=('dh2_equipment_auto_v3','dh2_equipment_character_auto_v3','dh2_equipment_to_slot_v3','dh2_equipment_from_slot_v3','dh2_equipment_has_two_hander_v3','dh2_equipment_slot_taken_v3')
class Pair:
 def __init__(self,library):
  self.old=Inventory();self.old.loading=False;self.old.fresh_inventory(-1);self.old.loading=False
  self.new=Cpu(library,True,{'functions':[]});self.oldspace=self.old.allocate(4096);self.trace=[];self.kind=0;self.values=[];self.mutation=False
  self.newbase=self.new.data+0x1000;self.svc=self.new.data+0x2000;self.out=self.new.data+0x2100;self.callback=self.new.data+0x30000
  self.new.uc.mem_write(self.svc,struct.pack('<QQ',0x123400007777,self.callback));self.old.uc.hook_add(UC_HOOK_CODE,self.oldhook);self.new.uc.hook_add(UC_HOOK_CODE,self.newhook)
 def setup(self,native,values):
  c=self.new if native else self.old;self.trace=[];self.values=values;self.kind=values[0];self.mutation=False;v=values
  self.order=list(range(4));self.items=[];self.slots=[];self.sets=[]
  if native:
   self.state=self.newbase;self.list=self.newbase+0x100;table=self.newbase+0x800
   for i in range(5):
    item=self.newbase+0x200+i*32;slot=self.newbase+0x400+i*32;self.items.append(item);self.slots.append(slot)
    t,st,stack,qty,s0,s1=v[8+i*6:14+i*6];c.uc.mem_write(item,struct.pack('<ihHQ',i,qty,0,0x123400000000+i));c.uc.mem_write(slot,struct.pack('<Qbb6x',item,s0,s1));c.uc.mem_write(table+12*i,W(t,st,stack))
   self.sets=[self.newbase+0x600,self.newbase+0x700]
   c.uc.mem_write(self.state,struct.pack('<QQIi2Q4IQ2I',0x123400005678,self.list,4,v[4],*self.sets,9,v[5],v[6],0,table,5,0))
  else:
   self.state=c.inv;self.list=self.oldspace;eq=self.oldspace+0x100;c.pointer(c.inv+4,c.character);c.pointer(c.inv+8,self.list);c.pointer(c.inv+0xc,self.list+16);c.pointer(c.inv+0x14,eq);c.uc.mem_write(c.inv+0x2e,bytes([v[4]]));c.uc.mem_write(c.character+0x1320,W(v[5],v[6]));
   for i in range(5):
    item=self.oldspace+0x200+i*128;slot=self.oldspace+0x600+i*16;self.items.append(item);self.slots.append(slot);t,st,stack,qty,s0,s1=v[8+i*6:14+i*6];c.uc.mem_write(item,bytes(0x6c));c.pointer(item+4,i);c.uc.mem_write(item+0x50,struct.pack('<h',qty));c.uc.mem_write(slot,struct.pack('<Ibb2x',item,s0,s1));c.pointer(c.table+164*i+0x58,t);c.pointer(c.table+164*i+0x68,st&0xffffffff);c.uc.mem_write(c.table+164*i+0x1c,bytes([stack]));
   self.sets=[self.oldspace+0x800,self.oldspace+0x900]
   for j,p in enumerate(self.sets):c.uc.mem_write(eq+12*j,W(p,p+36,p+36))
  for i in range(4):self.putptr(c,self.list+i*(8 if native else 4),self.slots[i])
  for j in range(2):
   for i,label in enumerate(v[38+j*9:47+j*9]):self.putptr(c,self.sets[j]+i*(8 if native else 4),self.slots[label] if label>=0 else 0)
 def putptr(self,c,p,v):c.uc.mem_write(p,struct.pack('<Q' if c is self.new else '<I',v))
 def ptr(self,c,p):return struct.unpack('<Q' if c is self.new else '<I',c.uc.mem_read(p,8 if c is self.new else 4))[0]
 def qty(self,c,p):return struct.unpack('<h',c.uc.mem_read(p+(4 if c is self.new else 0x50),2))[0]
 def selected(self,c,v):c.uc.mem_write(self.state+20,W(v)) if c is self.new else c.uc.mem_write(c.inv+0x2e,bytes([v]))
 def event(self,c,op,caller,item,args):
  label=self.items.index(item) if item else -1;self.trace.append(W(op,caller,label,*args));response=0;index=0;result=0
  if op==0x3fc3e0:
   result=self.items[4];quantity=args[0];c.uc.mem_write(item+(4 if c is self.new else 0x50),struct.pack('<h',1));c.uc.mem_write(result+(4 if c is self.new else 0x50),struct.pack('<h',quantity));sourceid=label
   c.uc.mem_write(result+(0 if c is self.new else 4),W(sourceid))
  elif op==0x3ff5d4:self.order.append(4)
  elif op==0x3fe1cc:response=self.kind==3 and self.values[7]%3==1;index=0 if label!=0 else 1
  elif op==0x3fdaf0:response=self.values[7]%4==2
  elif op==0x3fa17c:c.uc.mem_write(item+(4 if c is self.new else 0x50),struct.pack('<h',self.qty(c,item)+args[0]))
  elif op==0x3fe7d8:self.order.remove(label)
  if self.values[7]%5==3 and not self.mutation:self.mutation=True;self.selected(c,1-self.values[4])
  for i,label2 in enumerate(self.order):self.putptr(c,self.list+i*(8 if c is self.new else 4),self.slots[label2])
  if c is self.new:c.uc.mem_write(self.state+16,W(len(self.order)))
  else:c.pointer(c.inv+0xc,self.list+4*len(self.order))
  return result,response,index
 def oldhook(self,uc,a,z,u):
  if a not in OPS:return
  c=self.old;item=0;args=[0]*4;caller=0x400854 if a==0x3ff5d4 else uc.reg_read(c.lr)-4 # genuine tail branch, LR belongs to incoming caller
  if a in (0x3fc3e0,0x3fa17c):item=c.reg(0);args[0]=c.reg(1)
  elif a==0x3ff5d4:item=c.reg(1);args[:2]=[c.reg(2),c.reg(3)]
  elif a in (0x3fe1cc,0x3fe7d8):item=c.reg(1)
  elif a==0x3fdaf0:args[0]=c.reg(1)
  args=[x-(1<<32) if x>=1<<31 else x for x in args];result,value,index=self.event(c,a,caller,item,args)
  if a==0x3fe1cc and value:c.pointer(c.reg(2),index)
  c.returned(result if a==0x3fc3e0 else value)
 def newhook(self,uc,a,z,u):
  if a!=self.callback:return
  c=self.new;assert c.reg(0)==0x123400007777 and c.reg(1)==self.state;owner,item,op,caller,*args=struct.unpack('<QQII4i',uc.mem_read(c.reg(2),40));assert owner==0x123400005678;result,value,index=self.event(c,op,caller,item,args);uc.mem_write(c.reg(3),struct.pack('<QiI',result,value,index));c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 def snapshot(self,c):
  native=c is self.new;out=W(len(self.order),*self.order)
  for i in range(5):
   p=self.items[i];out+=W(struct.unpack('<i',c.uc.mem_read(p+(0 if native else 4),4))[0],self.qty(c,p));out+=bytes(c.uc.mem_read(self.slots[i]+(8 if native else 4),2))+bytes(2)
  out+=W(struct.unpack('<i',c.uc.mem_read(self.state+20,4))[0] if native else c.uc.mem_read(c.inv+0x2e,1)[0])
  for p in self.sets:
   for i in range(9):q=self.ptr(c,p+i*(8 if native else 4));out+=W(self.slots.index(q) if q else -1)
  return out
 def run(self,v):
  k,i,slot,force=v[:4];self.setup(False,v);c=self.old
  args=([c.inv,i] if k==0 else [c.character,i] if k==1 else [c.inv,slot,i,force] if k==2 else [c.inv,slot,-1] if k==3 else [c.inv,force] if k==4 else [c.inv,slot]);expected=c.invoke(ADDR[k],args);expected=expected if k in (0,1,4,5) else 0;oldstate=self.snapshot(c);oldtrace=self.trace.copy()
  self.setup(True,v);c=self.new;c.uc.mem_write(self.out,W(0xdeadbeef));args=([self.out,self.state,i,self.svc] if k in (0,1) else [self.state,slot,i,force,self.svc] if k==2 else [self.state,slot,-1,self.svc] if k==3 else [self.out,self.state,force] if k==4 else [self.out,self.state,slot]);status=c.invoke(EXPORT[k],args);got=struct.unpack('<I',c.uc.mem_read(self.out,4))[0] if k in (0,1,4,5) else 0
  assert status==0 and expected==got and oldstate==self.snapshot(c) and oldtrace==self.trace,(v,status,expected,got,oldstate.hex(),self.snapshot(c).hex(),[x.hex() for x in oldtrace],[x.hex() for x in self.trace]);return W(*v,expected,len(oldstate))+oldstate+W(len(oldtrace))+b''.join(oldtrace),len(oldtrace)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);a=ap.parse_args();p=Pair(a.library);rng=random.Random(0xE003);cases=[];total=0
 for k in range(1200):
  method=k%6;v=[method,k%4,rng.randrange(9),rng.randrange(2),rng.randrange(2),rng.choice((0,1,2,0xffffffff)),rng.choice((0,1,2,0xffffffff)),k%7]
  for i in range(5):
   qty=rng.choice((1,1,2,5));v += [rng.choice((0,4,5,6,13)),rng.choice((-4,-3,-2,-1,0,1,2,3,5,8,9)),int(qty>1 or rng.randrange(2)),qty,-1,-1]
  eq=[-1]*18
  for i in range(4):
   for s in range(2):
    if rng.randrange(3)==0:
     j=rng.randrange(9)
     if eq[s*9+j]<0:eq[s*9+j]=i;v[8+i*6+4+s]=j
  v+=eq;case,n=p.run(v);cases.append(case);total+=n
 ref=ROOT/'port/game-data/reference/player-equipment-v3';ref.mkdir(parents=True,exist_ok=True);gold=b'EQU3'+W(len(cases))+b''.join(cases);(ref/'fixtures.bin').write_bytes(gold);sha=lambda x:hashlib.sha256(Path(x).read_bytes()).hexdigest();r={'validation':'PASS','comparisons':len(cases),'ordered_requests':total,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'library_sha256':sha(a.library),'gold_sha256':hashlib.sha256(gold).hexdigest(),'source_sha256':{n:sha(ROOT/n) for n in ('port/game-data/player_equipment_v3.hpp','port/game-data/player_equipment_v3.cpp')},'script_sha256':sha(__file__),'mismatches':0,'scope':__doc__};(ROOT/'port/game-data/reports/player-equipment-v3-arm64-differential.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r))
if __name__=='__main__':main()
