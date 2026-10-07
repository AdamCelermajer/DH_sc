"""Real ARM32 CharAI::_OnAnimEvent state6 do_skill/ignored payload differential.
Named prefixes and states5/7/13 retain explicit required provider boundaries.
"""
import json,struct,hashlib,random
from character_skill_state_v4_differential import Actor,ROOT,LIB,OLD,words,signed

class AnimActor(Actor):
 def hook(self,uc,at,size,user):
  c=self.c
  if self.native:
   if at!=c.stop+0x100:return
   q,r=c.reg(2),c.reg(3)
   op,value,index,res,subject,payload=struct.unpack('<IIIIQQ',uc.mem_read(q,32));assert not res
   self.trace.append(op)
   result=self.step if op==21 else self.count if op==22 else self.current if op==23 else 0
   identity=self.debug if op==2 else 0
   if op==2:assert c.string(payload)==b'isTracingCharAI_AnimEvent'
   uc.mem_write(r,struct.pack('<IIQ',result&0xffffffff,0,identity));self.ret();return
  mapping={0x3c932c:21,0x3c934c:22,0x3c01ac:23,0x337888:1,
   0x3d43ec:2,0x337a88:3,0x3139ac:4,0x3d8bf8:24}
  if at not in mapping:return
  op=mapping[at];self.trace.append(op)
  result=self.step if op==21 else self.count if op==22 else self.current if op==23 else c.reg(0) if op==2 else 0
  self.ret(result)
 def run_anim(self,text,current,step,count):
  c=self.c;self.trace=[];self.step=step;self.count=count;self.current=current
  c.uc.mem_write(self.text,text.encode()+b'\0')
  if self.native:
   c.uc.mem_write(self.state,words(current,*([0]*13)))
   c.uc.mem_write(self.ext,struct.pack('<QQQQQIBBBB',self.state,self.owner,0,0,0,0,0,0,0,0))
   result=signed(c.invoke('dh2_character_skill_state_v4',[self.ext,5,0,0,self.text,0,self.svc]));assert result==0
  else:
   ai=self.ext;c.pointer(ai+4,self.owner)
   c.invoke(0x3d4434,[ai,self.text])
  return self.trace

def main():
 o,n=AnimActor(False),AnimActor(True);rng=random.Random(2026100571);cases=[]
 for i in range(512):
  text=['do_skill','Do_skill','do_skill_suffix','','is_stoppable'][i%5]
  current=6 if i%2 else 3;step=rng.getrandbits(32);count=rng.getrandbits(32)
  expected=o.run_anim(text,current,step,count);actual=n.run_anim(text,current,step,count)
  assert expected==actual,(i,text,current,expected,actual)
  cases.append(dict(text=text,current=current,step=step,count=count,trace=expected))
 gold=ROOT/'port/level-world/reference/character-skill-state-v4/anim-event-gold-v4.json'
 gold.write_text(json.dumps(cases,indent=2)+'\n')
 report=dict(validation='PASS',cases=len(cases),ordered_services=sum(len(x['trace'])for x in cases),mismatches=0,scope=__doc__,original_sha256=hashlib.sha256(OLD.read_bytes()).hexdigest(),optimized_arm64_sha256=hashlib.sha256(LIB.read_bytes()).hexdigest())
 (ROOT/'port/level-world/reports/character-skill-anim-event-v4-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps(report))
if __name__=='__main__':main()
