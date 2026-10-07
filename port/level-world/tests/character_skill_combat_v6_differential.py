"""Original SkillCombatRoll wrapper vs O2 native ordering.
Calculate/apply/handle/controller are explicit recorded synchronous services;
this proof does not substitute them for completed native combat gameplay.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
OLD=ROOT/'.local-inputs/libDungeonHunter2.so';LIB=ROOT/'.local-inputs/libskill_combat_v6_oracle.so'
class Actor:
 def __init__(self,native):
  self.native=native;self.c=Cpu(LIB if native else OLD,native,{'functions':[]});c=self.c;d=c.data
  self.owner=d+0x1000;self.obj=d+0x3000;self.target=d+0x4000;self.handle=d+0x5000;self.row=d+0x6000;self.lst=d+0x7000;self.args=d+0x8000;self.vec=d+0x8100;self.values=d+0x9000;self.out=d+0xb000;self.svc=d+0xc000;self.table=d+0xd000
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if native:c.uc.mem_write(self.svc,struct.pack('<QQ',self.owner,c.stop+0x100))
  else:
   c.pointer(self.obj,self.table);c.pointer(self.table+0x90,c.stop+0x200);c.pointer(self.table+0x98,c.stop+0x204)
 def ret(self,v=0):self.c.put(0,v);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def service(self,op,index=0,mask=0,element=0,result=0):
  self.trace.append([op,index,mask,element]);v=self.v;c=self.c
  if op==1:return v['list']
  if op==2:return self.handle
  if op==3:return self.target if v['character']else 0
  if op==4:return self.row
  if op==5:return v['main']
  if op==6:return self.off
  if op==7:
   self.amount=v['off_amount']if mask&0x4000000 else v['amount'];c.uc.mem_write(result,words(self.amount,*([0]*9)));return 0
  if op==8:
   self.off=v['off_after'];self.element=v['element_after'];c.uc.mem_write(self.row+(0 if self.native else 0x14),words(self.element));return 0
  if op==9:return v['kind']
  if op==10:return 0
  raise AssertionError(op)
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native:
   if at!=c.stop+0x100:return
   q,r,result=c.reg(1),c.reg(2),c.reg(3)
   op,index,mask,res,attacker,target,element,pad=struct.unpack('<IIIIQQiI',uc.mem_read(q,40));assert attacker==self.owner and not(res or pad)
   answer=self.service(op,index,mask,element,result);raw=bytearray(24)
   if op in(2,3):struct.pack_into('<Q',raw,0,answer)
   elif op==4:struct.pack_into('<Q',raw,8,answer)
   else:struct.pack_into('<I',raw,16,answer&0xffffffff)
   uc.mem_write(r,bytes(raw));self.ret();return
  if at==0x37baf8:self.ret(self.values+112*c.reg(1));return
  if at==0x38d798:self.ret(self.v['index']);return
  if at==0x31b5a0:self.ret(self.obj if self.v['nonnull']else 0);return
  if at==0x33dd2c:c.pointer(c.reg(0),self.handle);self.service(2);self.ret(c.reg(0));return
  if at==0x33ff54:self.ret(self.service(3));return
  if at==0x3bc5fc:self.service(1);self.ret(self.lst);return
  if at==0x3bc784:self.ret(self.service(4,c.reg(1)));return
  if at in(0x3ffe8c,0x400158):self.ret(self.service(5 if at==0x3ffe8c else 6));return
  if at==0x3b31a4:
   element=signed(struct.unpack('<I',uc.mem_read(uc.reg_read(c.sp),4))[0]);self.service(7,0,c.reg(3),element,c.reg(0));self.ret();return
  if at==0x3b10b4:self.service(8);self.ret();return
  if at==0x37cb24:self.results.append(signed(c.reg(1)));self.ret();return
  if at==0x37c7e4:self.boolean_count+=1;assert c.reg(1)==0;self.ret();return
  if at==c.stop+0x200:self.ret(self.service(9));return
  if at==c.stop+0x204:self.service(10);self.ret();return
 def run(self,v):
  c=self.c;self.v=v;self.trace=[];self.results=[];self.boolean_count=0;self.off=v['off'];self.element=v['element']
  if self.native:
   args=[]
   for i in range(v['count']):args.append(struct.pack('<IIfIQQQ',v['type0']if i==0 else v['type1'],0,float(v['index'])if i==0 else 0,0,0,0,self.obj if v['nonnull']else 0))
   c.uc.mem_write(self.values,b''.join(args)if args else bytes(40));c.uc.mem_write(self.row,words(v['element'],v['mask']))
   status=signed(c.invoke('dh2_character_skill_combat_roll_v6',[self.out,self.owner,self.values,v['count'],self.svc]));assert status==0
   a,b,n,bo,calls,phase=struct.unpack('<iiIIII',c.uc.mem_read(self.out,24));return [a,b][:n],bo,self.trace
  c.pointer(self.args+4,self.vec);c.pointer(self.vec,self.values);c.pointer(self.vec+4,self.values+112*v['count']);c.pointer(self.lst+4,v['list']);c.uc.mem_write(self.row+0x14,words(v['element']));c.uc.mem_write(self.row+0x1c,words(v['mask']))
  c.uc.mem_write(self.values+4,words(v['type0']));c.uc.mem_write(self.values+116,words(v['type1']));c.invoke(0x3b9fbc,[self.args,self.out,self.owner]);return self.results,self.boolean_count,self.trace
def main():
 o,n=Actor(False),Actor(True);rng=random.Random(202610056);cases=[]
 for i in range(8192):
  v=dict(count=rng.choice([0,1,2,3]),type0=rng.choice([0,3,3,3]),type1=rng.choice([0,2,7,2,7]),index=rng.randrange(4),list=rng.randrange(5),nonnull=rng.randrange(2),character=rng.randrange(2),kind=rng.choice([0,8,8]),main=rng.randrange(2),off=rng.randrange(2),off_after=rng.randrange(2),mask=rng.choice([0,0x800000,0x18001001,0xffffffff]),element=rng.choice([-1,0,4]),element_after=rng.choice([-1,2,4]),amount=rng.randrange(10000),off_amount=rng.randrange(10000))
  if i>=4096:v.update(count=2,type0=3,type1=2 if i%2 else 7,index=0,list=3,nonnull=1)
  a=o.run(v);b=n.run(v);assert a==b,(i,v,a,b);cases.append({'input':v,'values':a[0],'booleans':a[1],'services':a[2]})
 ref=ROOT/'port/level-world/reference/character-skill-combat-v6';(ref/'wrapper-gold-v6.json').write_text(json.dumps(cases,indent=2)+'\n')
 report=dict(validation='PASS',cases=len(cases),ordered_services=sum(len(x['services'])for x in cases),mismatches=0,original_sha256=hashlib.sha256(OLD.read_bytes()).hexdigest(),optimized_sha256=hashlib.sha256(LIB.read_bytes()).hexdigest(),scope=__doc__)
 (ROOT/'port/level-world/reports/character-skill-combat-v6-wrapper-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
