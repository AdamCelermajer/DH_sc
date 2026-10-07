"""Complete original skill AI commands/focus vs O2 ARM64.

The actual nested AI_IsSkillUsable/Active/Begin/End bodies execute. FSM queries,
row getter, script callbacks, source SetSkillState, owner IsPlayer/property,
network and trophy effects are explicit synchronous service projections here.
Trophy pointer capture, name-loop ordering and AI fields execute natively.
"""
import hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from hud_formatting_v1_original import Cpu,words,signed
OLD=ROOT/'.local-inputs/libDungeonHunter2.so';LIB=ROOT/'.local-inputs/character-skill-gameplay-v3/libskill_gameplay_v3_oracle.so'
REF=ROOT/'port/level-world/reference/character-skill-gameplay-v3'
ENTRIES=[0x3d8358,0x3d85d4,0x3d86bc,0x3d8474,0x3d8868,0x3d84e0,0x3d808c,0x3d8bf8,0x3d8b7c]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Actor:
 def __init__(self,native):
  self.native=native;self.c=Cpu(LIB if native else OLD,native,{'functions':[]});c=self.c;d=c.data
  self.ai=d+0x1000;self.owner=d+0x3000;self.fields=d+0x5000;self.slots=d+0x5100;self.vec=d+0x5200;self.instance=d+0x5400;self.row=d+0x6000;self.svc=d+0x7000;self.out=d+0x7100;self.vtable=d+0x7200
  self.global_trophy=d+0x8000;self.global_count=d+0x8010;self.global_names=d+0x8020;self.names=d+0x8100;self.strings=d+0x8200;self.app=d+0x9000
  if native:c.uc.mem_write(self.svc,struct.pack('<QQ',self.owner,c.stop+0x100))
  else:
   got=0x994a98
   for literal,global_pointer in [(0x3d8854,self.global_trophy),(0x3d8858,self.app),(0x3d885c,self.global_count),(0x3d8860,self.global_names)]:c.pointer(got+struct.unpack('<I',c.uc.mem_read(literal,4))[0],global_pointer)
   c.pointer(self.owner,self.vtable);c.pointer(self.vtable+0x28,c.stop+0x108)
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,value=0):self.c.put(0,value);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def trace_call(self,op,index=0,value=0,subject=0):self.trace.append([op,index,value,subject])
 def service(self,op,index=0,value=0,subject=0):
  self.trace_call(op,index,value,subject);v=self.values;c=self.c
  if op==1:return self.using
  if op==2:return v['casting']
  if op==3:return self.row
  if op==4:
   if v.get('mutation',0)==1 and value==3:self.write_fields(9,1,1);c.uc.mem_write(self.row+8,words(77))
   if v.get('mutation',0)==3 and value==4:self.write_fields(13,1,1)
   return v['callbacks'][value]
  if op==5:
   if v.get('mutation',0)==2:self.write_fields(0,1,1)
   self.using=v['after'];return 0
  if op==6:return v['player']
  if op==7:
   if not value:c.pointer(self.global_trophy,self.owner+0x222)
   return v['cached'] if value else v['property']
  if op==8:return self.owner+0x111
  if op==9:return v['network']
  if op==10:return 0
  if op in(11,12):return 0
  raise AssertionError(op)
 def write_fields(self,current,continued,last):
  if self.native:self.c.uc.mem_write(self.fields,struct.pack('<iBBH',current,continued,last,0))
  else:self.c.pointer(self.ai+0xcc,current&0xffffffff);self.c.uc.mem_write(self.ai+0xd0,bytes([continued,last]))
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native and at==c.stop+0x100:
   q,r=c.reg(2),c.reg(3);op,index,value,res,character,subject=struct.unpack('<IIIIQQ',uc.mem_read(q,32));assert character==self.owner and not res
   sid=1 if subject==self.instance else 2 if subject==self.owner+0x111 else 3 if subject==self.owner+0x222 else 0
   answer=self.service(op,index,value,sid);raw=bytearray(32)
   if op==3:struct.pack_into('<Q',raw,16,answer)
   elif op==8:struct.pack_into('<Q',raw,8,answer)
   elif op==10:struct.pack_into('<I',raw,4,len(self.values['names']));struct.pack_into('<Q',raw,24,self.names)
   else:struct.pack_into('<I',raw,0,answer&0xffffffff)
   uc.mem_write(r,bytes(raw));self.ret();return
  if self.native:return
  mapping={0x3c02e8:1,0x3c0334:2,0x3bc784:3,0x3c6670:5,0x36effc:9,0x3c948c:12}
  if at in mapping:
   op=mapping[at];index=c.reg(1)if op in(3,5)else 0;value=c.reg(2)if op==5 else c.reg(1)if op==12 else 0
   if op in(1,2):assert c.reg(0)==self.owner+0x4fc
   if op==3:assert c.reg(0)==self.owner
   if op==5:assert c.reg(0)==self.owner+0x4fc and c.reg(3)==0 and struct.unpack('<I',uc.mem_read(uc.reg_read(c.sp),4))[0]==0
   self.ret(self.service(op,index,value,0));return
  if at==0x3cb458:return # Actual signed load-step body executes.
  if at==c.stop+0x108:self.ret(self.service(6));return
  if at in(0x3da8b8,0x3da794,0x3da6c0,0x3da9dc,0x3db16c):
   assert c.reg(0)==self.instance;op={0x3da8b8:0,0x3da794:1,0x3da6c0:2,0x3da9dc:3,0x3db16c:4}[at];self.ret(self.service(4,0,op,1));return
  if at in(0x3e0798,0x3df6e0):assert c.reg(1)==216;self.ret(self.service(7,216,c.reg(2)));return
  if at==0x3d87c0:self.service(8);return # Actual following LDR captures old manager.
  if at==0x3d87ec:self.trace_call(10);return # Actual inline count/name loop executes.
  if at==0x3813b8:self.ret(self.service(11,c.reg(1),0,2 if c.reg(0)==self.owner+0x111 else 3));return
 def run(self,op,v):
  c=self.c;self.values=v;self.using=v['using'];self.trace=[];c.uc.mem_write(self.row,words(*([0]*19)));c.uc.mem_write(self.row+8,words(v['raw']));c.uc.mem_write(self.row+0x48,words(v['type']));c.pointer(self.global_trophy,self.owner+0x111)
  for i,name in enumerate(v['names']):p=self.strings+i*128;c.uc.mem_write(p,name.encode()+b'\0');c.pointer(self.names+i*(8 if self.native else 4),p)
  c.pointer(self.global_names,self.names);c.pointer(self.global_count,len(v['names']));c.uc.mem_write(self.out,words(0x12345678))
  if self.native:
   c.uc.mem_write(self.ai,struct.pack('<QQQiI',self.owner,self.slots,self.fields,v['step'],0));c.uc.mem_write(self.owner,struct.pack('<QII',self.owner,v['flags'],0));c.uc.mem_write(self.fields,struct.pack('<iBBH',v['current'],v['continued'],v['last'],0));c.uc.mem_write(self.slots,struct.pack('<QQIIQQII',self.owner,self.vec,1,0,0,0,0,0));c.pointer(self.vec,self.instance if v['present']else 0)
   code=c.invoke('dh2_character_skill_ai_v3',[self.out,self.ai,op,0,self.svc]);assert signed(code)==0,(op,v,signed(code),self.trace);answer=struct.unpack('<I',c.uc.mem_read(self.out,4))[0];fields=struct.unpack('<iBBH',c.uc.mem_read(self.fields,8))[:3]
  else:
   c.pointer(self.ai+4,self.owner);c.pointer(self.ai+0xb4,self.vec);c.pointer(self.ai+0xb8,self.vec+4);c.pointer(self.ai+0x28,v['step']&0xffffffff);c.pointer(self.ai+0xcc,v['current']&0xffffffff);c.uc.mem_write(self.ai+0xd0,bytes([v['continued'],v['last']]));c.pointer(self.owner+0x520,v['flags']);c.pointer(self.owner,self.vtable);c.pointer(self.vec,self.instance if v['present']else 0)
   answer=c.invoke(ENTRIES[op],[self.ai,0]);answer=answer if op in(0,1,2,4)else 0;fields=(signed(struct.unpack('<I',c.uc.mem_read(self.ai+0xcc,4))[0]),*bytes(c.uc.mem_read(self.ai+0xd0,2)))
  return answer,list(self.trace),list(fields)
def main():
 o,n=Actor(False),Actor(True);rng=random.Random(202610046);cases=[]
 for i in range(2304):
  op=i%9;v=dict(using=rng.randrange(2),casting=rng.randrange(2),after=rng.randrange(2),step=rng.choice([-1,0,6,7,99]),flags=rng.choice([0,0x8000,0xffffffff]),current=rng.choice([-1,0,1]),continued=rng.randrange(2),last=rng.randrange(2),present=1,type=rng.randrange(3),raw=rng.randrange(256),callbacks=[rng.randrange(2)for _ in range(5)],player=rng.randrange(2),cached=rng.randrange(400),property=rng.choice([-1,0,199,200,999]),network=rng.randrange(2),names=rng.choice([[],['different'],['epic_withskills'],['different','epic_withskills','epic_withskills']]),mutation=i%4)
  if op in(0,1,3,5,6,7,8):v['present']=rng.randrange(2)
  expected=o.run(op,v);actual=n.run(op,v);assert expected==actual,(i,op,v,expected,actual);cases.append(dict(operation=op,input=v,answer=expected[0],trace=expected[1],fields=expected[2]))
 gold=REF/'ai-gold-v3.json';gold.write_text(json.dumps(cases,indent=2)+'\n');files=['port/level-world/character_skill_ai_v3.hpp','port/level-world/character_skill_ai_v3.cpp','port/level-world/tests/character_skill_ai_v3_differential.py']
 raw=words(0x33494153,len(cases));keys='using casting after step flags current continued last present type raw player cached property network mutation'.split()
 for case in cases:
  v=case['input'];record=words(case['operation'],*[v[k]for k in keys],*v['callbacks'],len(v['names']))
  for name in v['names']:encoded=name.encode();record+=words(len(encoded))+encoded
  record+=words(case['answer'],*case['fields'],len(case['trace']))+b''.join(words(*t)for t in case['trace']);raw+=words(len(record))+record
 (REF/'ai-gold-v3.bin').write_bytes(raw)
 report=dict(validation='PASS',cases=len(cases),ordered_services=sum(len(c['trace'])for c in cases),mismatches=0,original_sha256=sha(OLD),optimized_arm64_sha256=sha(LIB),gold_sha256=sha(gold),source_sha256={p:sha(ROOT/p)for p in files},scope=__doc__)
 p=ROOT/'port/level-world/reports/character-skill-gameplay-v3-ai-arm64-differential.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k]for k in('validation','cases','ordered_services','mismatches')}))
if __name__=='__main__':main()



