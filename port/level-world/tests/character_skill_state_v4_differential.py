"""Execute complete source CSSkill Focus/Blur/Event/Update and SM_SetSkillState.

Debug string ownership and body/animation/timer/class/table dependencies are
declared synchronous services. Source target→last target copy executes in ARM32.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
OLD=ROOT/'.local-inputs/libDungeonHunter2.so';LIB=ROOT/'.local-inputs/character-skill-state-v4/libskill_state_v4_oracle.so'
REF=ROOT/'port/level-world/reference/character-skill-state-v4'
ENTRIES=[0x3c4480,0x3c434c,0x3c0ac8,0x3c0018,0x3c6670]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Actor:
 def __init__(self,native):
  self.native=native;self.c=Cpu(LIB if native else OLD,native,{'functions':[]});c=self.c;d=c.data
  self.owner=d+0x1000;self.state=d+0x4000;self.ext=d+0x5000;self.svc=d+0x6000;self.row=d+0x7000;self.text=d+0x8000;self.debug=d+0x9000;self.body=d+0xa000
  if native:c.uc.mem_write(self.svc,struct.pack('<QQ',self.owner,c.stop+0x100))
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v=0):self.c.put(0,v);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def pointer(self,offset,v):self.c.pointer((self.state if self.native else self.owner)+offset,v)
 def mutate(self,op):
  # Mutate live fields after the original call, never the captured row animation.
  v=self.v;c=self.c
  if v['mutation']==op:
   self.pointer(4 if self.native else 0x520,0xfeed0000);self.pointer(8 if self.native else 0x528,v['mutated_mask'])
   c.uc.mem_write((self.ext+44 if self.native else self.owner+0x554),bytes([v['mutated_moving']]))
   c.pointer(self.ext+16 if self.native else self.owner+0x2dc,self.body if v['mutated_body']else 0)
   c.uc.mem_write(self.row+4,words(0xffffffff))
 def service(self,op,value=0,index=0,subject=0,payload=0):
  c=self.c;self.trace.append([op,value,index,subject,payload]);self.mutate(op)
  if op==2:return self.debug
  if op==13:return self.v['monster']
  if op==14:return self.v['mini']
  if op==15:return self.v['boss']
  if op==16:return self.row
  if op==17:return self.v['constant']
  if op==18:return self.v['stance']
  return 0
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native and at==c.stop+0x100:
   q,r=c.reg(2),c.reg(3);op,value,index,res,subject,payload=struct.unpack('<IIIIQQ',uc.mem_read(q,32));assert not res
   sid=1 if subject==self.debug else 2 if subject==self.body else 0
   if op==2:assert c.string(payload)==b'isTracingCharState';payload=0
   if op in(19,20):assert payload==self.text;payload=1
   answer=self.service(op,value,index,sid,payload);uc.mem_write(r,struct.pack('<IIQ',answer if op in(13,14,15,17,18)else 0,0,answer if op in(2,16)else 0));self.ret();return
  if self.native:return
  if at==0x3d49c4:return # Genuine last-target source store executes.
  mapping={0x337888:1,0x3140ec:2,0x337a88:3,0x318254:4,0x3938f8:5,0x3a4d5c:6,0x3c0b50:7,0x3c93fc:8,0x3bc6b8:9,0x46eb20:10,0x46eae0:11,0x3dbe24:12,0x3a3064:13,0x3a3144:14,0x3a3158:15,0x3bc784:16,0x4c4bdc:17,0x3a53e0:18,0x3c5684:19,0x3c1938:20}
  if at not in mapping:return
  op=mapping[at];value=index=subject=payload=0
  if op==2:assert c.string(c.reg(1))==b'isTracingCharState'
  if op in(3,4):subject=1
  if op in(10,11):assert c.reg(0)==self.body;subject=2
  if op in(6,7,8):value=c.reg(1)
  if op==12:
   value=c.reg(1);index=c.reg(3);assert c.reg(2)==0 and struct.unpack('<I',uc.mem_read(uc.reg_read(c.sp),4))[0]==0
  if op==16:index=c.reg(1)
  if op==17:assert c.string(c.reg(1))==b'AnimStancedAnim' and c.string(c.reg(2))==b'SL__LIST_IPHONE'
  if op in(19,20):value=c.reg(1)if op==19 else c.reg(2);index=0 if op==19 else c.reg(1);assert (c.reg(2)if op==19 else c.reg(3))==self.text;payload=1
  answer=self.service(op,value,index,subject,payload)
  if op==2:self.ret(c.reg(0))
  else:self.ret(answer)
 def run(self,op,v):
  c=self.c;self.v=v;self.trace=[];c.uc.mem_write(self.text,v['text'].encode()+b'\0');c.uc.mem_write(self.row,words(*([0]*19)));c.uc.mem_write(self.row+4,words(v['animation']))
  if self.native:
   c.uc.mem_write(self.state,words(-1,v['flags'],v['mask'],0,0,0,0,0,0,0,0,0,v['override'],0));c.uc.mem_write(self.ext,struct.pack('<QQQQQIBBBB',self.state,self.owner,self.body if v['body']else 0,v['target'],v['last'],v['old_index'],v['old_moving'],v['heading'],0,0))
   status=signed(c.invoke('dh2_character_skill_state_v4',[self.ext,op,v['index'],v['moving'],self.text,v['force'],self.svc]));assert status==0
   state=struct.unpack('<14I',c.uc.mem_read(self.state,56));ext=struct.unpack('<QQQQQIBBBB',c.uc.mem_read(self.ext,48));fields=[state[1],state[2],state[12],ext[4],ext[5],ext[6],ext[7]]
  else:
   for off,value in[(0x520,v['flags']),(0x528,v['mask']),(0x524,v['override']),(0x408,v['target']),(0x40c,v['last']),(0x550,v['old_index']),(0x2dc,self.body if v['body']else 0),(0x500,self.owner)]:c.pointer(self.owner+off,value)
   c.uc.mem_write(self.owner+0x554,bytes([v['old_moving']]));c.uc.mem_write(self.owner+0x412,bytes([v['heading']]))
   if op==4:c.invoke(ENTRIES[op],[self.owner+0x4fc,v['index'],v['moving'],self.text,v['force']])
   else:c.invoke(ENTRIES[op],[0,6,self.owner,self.owner+0x4fc,v['index'],self.text])
   fields=[struct.unpack('<I',c.uc.mem_read(self.owner+off,4))[0]for off in(0x520,0x528,0x524,0x40c,0x550)]+[c.uc.mem_read(self.owner+0x554,1)[0],c.uc.mem_read(self.owner+0x412,1)[0]]
  return fields,list(self.trace)
def main():
 o,n=Actor(False),Actor(True);rng=random.Random(202610057);cases=[]
 for i in range(4096):
  op=i%5;v=dict(flags=rng.getrandbits(32),mask=rng.getrandbits(32),override=rng.getrandbits(32),old_index=rng.getrandbits(32),old_moving=rng.randrange(256),heading=rng.randrange(256),body=i%2,target=rng.getrandbits(32),last=rng.getrandbits(32),index=0x28 if op==2 and i%3 else rng.getrandbits(32),moving=rng.randrange(256),force=i%2,monster=i%2,mini=(i//2)%2,boss=(i//4)%2,constant=rng.choice([0,0x200000,0xffffffff]),stance=rng.getrandbits(32),animation=rng.getrandbits(32),text=rng.choice(['is_stoppable','Is_stoppable','do_skill','is_stoppable_suffix','']),mutation=rng.choice([0,6,9,16,17]),mutated_mask=rng.getrandbits(32),mutated_moving=rng.randrange(256),mutated_body=i%2)
  expected=o.run(op,v);actual=n.run(op,v);assert expected==actual,(i,op,v,expected,actual);cases.append(dict(operation=op,input=v,fields=expected[0],trace=expected[1]))
 p=REF/'gold-v4.json';p.write_text(json.dumps(cases,indent=2)+'\n');report=dict(validation='PASS',cases=len(cases),ordered_services=sum(len(c['trace'])for c in cases),mismatches=0,original_sha256=sha(OLD),optimized_arm64_sha256=sha(LIB),gold_sha256=sha(p),scope=__doc__)
 report['source_sha256']={str(p.relative_to(ROOT)):sha(p)for p in [ROOT/'port/level-world/character_skill_state_v4.cpp',ROOT/'port/level-world/character_skill_state_v4.hpp',Path(__file__)]}
 p=ROOT/'port/level-world/reports/character-skill-state-v4-arm64-differential.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k]for k in('validation','cases','ordered_services','mismatches')}))
if __name__=='__main__':main()
