"""Full original Lua CreateBuff/RemoveBuff wrappers versus native callbacks.
Actual original argument guards/getters and IEEE conversion execute. Buff core
Add/Delete and ReturnValues pointer append are observed services; native owned
Buff application/removal is exercised separately on the same retained session.
"""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(REPO/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu as Base,words,bits,signed,floating
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Cpu(Base):
 def external(self,uc,address,size,user):
  name=self.imports.get(address)
  if self.arm64 and name in ('dh2_character_buff_add','dh2_character_buff_delete'):self.capture(name);return
  return super().external(uc,address,size,user)
class Buff:
 def __init__(self,path,native):
  self.native=native;self.c=Cpu(path,native,{'functions':[]});c=self.c;d=c.data
  self.owner=d+0x1000;self.args=d+0x3000;self.vector=d+0x3100;self.values=d+0x4000;self.out=d+0x7000;self.result=d+0x7100;self.bind=d+0x7200;self.text=d+0x8000
  c.capture=self.capture;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if not native:
   c.pointer(0x99f698,0)
   base=0x3b86d0+8+self.word(0x3b8bc0)
   for literal,value in [(0x3b8bc4,1000),(0x3b8bd4,8)]:
    slot=base+self.word(literal);cell=d+0xb000+(literal&255)*16;c.pointer(slot,cell);c.pointer(cell,value)
   base=0x3b8444+8+self.word(0x3b8564);slot=base+self.word(0x3b8568);cell=d+0xc000;c.pointer(slot,cell);c.pointer(cell,1000)
 def word(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def ret(self,n=0):self.c.put(0,n);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def capture(self,name):
  c=self.c
  if name=='dh2_character_buff_add':
   self.trace.append(['add',signed(c.reg(2)),c.reg(3),signed(c.reg(4)),c.reg(5),signed(c.reg(6)),c.string(c.reg(7)).decode()]);c.uc.mem_write(c.reg(0),struct.pack('<QIIiI',self.return_identity,0,0,1,0));self.ret(1)
  else:self.trace.append(['delete',signed(c.reg(2)),1 if c.reg(3) else 0]);self.ret(1)
 def hook(self,uc,address,size,user):
  c=self.c
  if self.native:return
  if address==0x37baf8:self.ret(self.values+112*c.reg(1));return
  if address==0x3e232c:
   stack=struct.unpack('<III',uc.mem_read(uc.reg_read(c.sp),12));self.trace.append(['add',signed(c.reg(1)),c.reg(2),signed(c.reg(3)),stack[0],signed(stack[1]),c.string(stack[2]).decode()]);self.ret(self.return_identity);return
  if address==0x3e101c:self.trace.append(['delete',signed(c.reg(1)),1 if c.reg(2) else 0]);self.ret();return
  if address==0x38eb00:self.result_identity=c.reg(1);self.ret();return
 def run(self,remove,values,identity):
  c=self.c;self.trace=[];self.result_identity=0;self.return_identity=0xdead1111 if identity else 0;c.uc.mem_write(self.text,b'buff\0');c.uc.mem_write(self.result,words(0))
  for i,(tag,number)in enumerate(values):
   if self.native:c.uc.mem_write(self.values+i*40,struct.pack('<IIfIQQQ',tag,0,float(number),int(number!=0)if tag==1 else 0,self.text if tag==4 else 0,4 if tag==4 else 0,0xdead2222 if tag==2 and number else 0))
   else:
    c.uc.mem_write(self.values+i*112,bytes(112));c.pointer(self.values+i*112+4,tag);c.pointer(self.values+i*112+8,bits(float(number)));c.pointer(self.values+i*112+0x6c,0xdead2222 if tag==2 and number else 0)
    if tag==4:c.uc.mem_write(self.values+i*112+0x20,struct.pack('<IIIIII',4,self.text,self.text,self.text,self.text+4,self.text))
  if self.native:
   c.uc.mem_write(self.bind,struct.pack('<QIIQQ',self.owner,1000,8,0,0));r=c.invoke('skill_remove_buff_v3' if False else '_ZN3dh29character6skills20skill_remove_buff_v3EPvPK16dh2_script_valuejPS3_jPjPcm' if remove else '_ZN3dh29character6skills20skill_create_buff_v3EPvPK16dh2_script_valuejPS3_jPjPcm',[self.bind,self.values,len(values),self.out,1,self.result,self.out+0x100,256]);assert signed(r)==0,(r,values)
   if self.word(self.result):self.result_identity=struct.unpack('<Q',c.uc.mem_read(self.out+32,8))[0]
  else:
   c.pointer(self.args+4,self.vector);c.uc.mem_write(self.vector,words(self.values,self.values+112*len(values),self.values+112*len(values)));c.invoke(0x3b842c if remove else 0x3b86a8,[self.args,self.out,self.owner])
  return self.trace,self.result_identity
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();old=REPO/'.local-inputs/libDungeonHunter2.so';o,n=Buff(old,False),Buff(a.library,True);rng=random.Random(20261005);rows=[]
 for i in range(512):
  values=[(3,rng.choice([-2.,-1.,-.125,0.,.999,1.,999.,1000.,math.nan,math.inf]))]
  choices=[[(0,0),(3,0),(3,1000.5),(1,1)],[(0,0),(1,0),(1,1),(3,-2),(3,2.9)],[(0,0),(3,3.9),(3,-1),(1,1)],[(0,0),(3,-1),(3,3),(3,999)],[(0,0)]]
  for choice in choices:values.append(rng.choice(choice))
  values=values[:i%7];expected=o.run(False,values,i%2);actual=n.run(False,values,i%2);assert actual==expected,(i,values,expected,actual);rows.append(dict(remove=False,values=[[t,bits(float(v))]for t,v in values],identity=i%2,trace=expected[0],result=expected[1]))
 for i in range(192):
  values=[(3,rng.choice([-2.,0.,.9,999.,1000.,math.nan])),rng.choice([(0,0),(2,0),(2,1),(3,1)])][:i%3];expected=o.run(True,values,False);actual=n.run(True,values,False);assert actual==expected,(i,values,expected,actual);rows.append(dict(remove=True,values=[[t,bits(float(v))]for t,v in values],identity=0,trace=expected[0],result=expected[1]))
 ref=ROOT/'reference/character-skill-gameplay-v3';gold=ref/'buff-wrapper-gold.json';gold.write_text(json.dumps(dict(validation='PASS',cases=rows),indent=2)+'\n');report=dict(validation='PASS',cases=len(rows),core_requests=sum(len(x['trace'])for x in rows),mismatches=0,original_sha256=sha(old),optimized_arm64_sha256=sha(a.library),gold_sha256=sha(gold),original_instructions_executed=True,optimized_arm64_instructions_executed=True,scope=__doc__);(ROOT/'reports/character-skill-gameplay-v3-buff-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
