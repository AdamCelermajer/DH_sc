"""Recovered post-selection BlendedPlayClip replay versus native producer.
Existing animation selection/blend weight resolution are explicit fixtures.
PlayClip/NewAnim branches and actual timeline/getLoop/jump/scale instructions
execute. Root EnableDisplacement/reset backends are observed services.
"""
import argparse,hashlib,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_differential import ROOT,equal
from navigation_search_differential import words,word
from visual_timeline_differential import Machine,TimelineCpu,state

class Original(Machine):
 def __init__(self,path,manifest):
  super().__init__(path,False,manifest);c=self.c;d=c.data;self.controller=d+0x6000;self.blender=d+0x7000;self.sub=d+0x8000;self.bv=d+0x9000;self.sv=d+0xa000;self.root=d+0xb000;self.rv=d+0xc000;c.pointer(self.bv+0x24,c.callback+96);c.pointer(self.sv+0x44,c.callback+112);c.pointer(self.rv+0x14,c.callback+128);c.uc.hook_add(UC_HOOK_CODE,self.hook)
  c.pointer(self.vt+0x44,0x666c30);c.pointer(self.vt+0x40,0x666c28);c.pointer(self.vt+0x48,0x666c20);c.pointer(self.sv+0x30,c.callback+144)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def plan(self,restart):return self.snapshot()[:4]+words(self.facts[6],restart,self.facts[4])
 def hook(self,uc,address,size,unused):
  c=self.c
  if address==0x4748b8:self.ret(self.blender)
  elif address==0x36679c:self.ret()
  elif address==0x3674ac:c.pointer(self.sub+0x50,self.facts[1]&0xffffffff);self.ret(self.facts[1])
  elif address==0x35d624:self.new_event=len(self.events);self.events.append(words(1)+self.snapshot()+self.plan(0))
  elif address==0x35d4cc:c.uc.mem_write(self.root+0x1ec,bytes((self.facts[4],)));self.ret()
  elif address==0x35ce6c:
   assert c.reg(1)==(self.facts[6]+1)&0xffffffff;self.restart=1;self.events[self.new_event]=self.events[self.new_event][:-16]+self.plan(1);self.ret()
  elif address==0x366740:self.events.append(words(2)+self.snapshot()+self.plan(self.restart)) # actual empty BlendPost executes
 def callback(self,address):
  c=self.c
  if address==c.callback+96:c.put(0,12)
  elif address==c.callback+112:c.put(0,self.s)
  elif address==c.callback+128:
   if self.update_inside:c.put(0,self.s);c.put(1,self.facts[6]);c.uc.reg_write(c.pc,0x667104)
  elif address==c.callback+144:c.put(0,0) # animator binding/start selection is a fixture
  else:super().callback(address)
 def run(self,s,facts,inside):
  f=struct.unpack('<3i5I',facts);self.facts=f;self.update_inside=inside;self.restart=0;self.new_event=0;self.configure(s,(0,0,0,0),words(0,0),0);c=self.c;c.uc.mem_write(self.controller,bytes(0x40));c.pointer(self.controller+4,self.root);c.uc.mem_write(self.controller+0x10,bytes((f[4],)));c.pointer(self.blender,self.bv);c.pointer(self.blender+0x28,self.blender+0x200);c.pointer(self.blender+0x200,self.sub);c.pointer(self.blender+0x70,0);c.pointer(self.blender+0x98,f[2]&0xffffffff);c.pointer(self.sub,self.sv);c.pointer(self.sub+0x50,f[0]&0xffffffff);c.pointer(self.root,self.rv);c.pointer(self.root+0x1f0,self.root+0x300 if f[5] else 0);c.pointer(self.root+0x1fc,f[6]);accepted=c.invoke(0x47680c,[self.controller,7,f[3],0,0]);plan=self.plan(self.restart) if accepted else bytes(16);return words(accepted)+self.snapshot()+plan,tuple(self.events)

class Native(Machine):
 def __init__(self,path):super().__init__(path,True,{'functions':[]});c=self.c;self.f=c.data+0x6000;self.out=c.data+0x7000
 def callback(self,address):
  c=self.c;e=c.reg(1);self.events.append(words(e)+self.snapshot()+bytes(c.uc.mem_read(c.reg(3),16)))
  if e==1 and self.update_inside and word(c,c.reg(3)+8):c.put(0,self.s);c.put(1,self.timestamp);c.put(2,0);c.uc.reg_write(c.pc,c.symbols['dh2_timeline_update'])
 def run(self,s,facts,inside):
  self.configure(s,(0,0,0,0),words(0,0),0);c=self.c;self.update_inside=inside;self.timestamp=struct.unpack_from('<I',facts,24)[0];c.uc.mem_write(self.f,facts);c.uc.mem_write(self.out,bytes(16));c.uc.mem_write(self.services,struct.pack('<QQ',0,c.callback+32));accepted=c.invoke('dh2_timeline_replay',[self.out,self.s,self.f,self.services]);return words(accepted)+self.snapshot()+bytes(c.uc.mem_read(self.out,16)),tuple(self.events)

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args();start=time.monotonic();manifest=json.loads((ROOT/'reference/visual-timeline/original-functions.json').read_text());old=Original(a.engine,manifest);new=Native(a.library);assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256'];rng=random.Random(20261017);records=[];callbacks=0;restarts=0
 for i in range(640):
  begin=rng.randrange(-1000,2000);end=begin+rng.randrange(1,3000);current=begin+rng.randrange(0,end-begin);s=state(begin,end,current,loop=i%3==0,initialized=i%2,scale=rng.choice((.5,1.,2.)),last=.1,library=1);oldclip=i%7;mapped=-1 if i%19==0 else oldclip if i%2 else oldclip+1;timestamp=0 if i%5==0 else rng.randrange(1,900000);facts=struct.pack('<3i5I',oldclip,mapped,rng.randrange(-100,1000),i%3!=0,i%4!=0,i%7!=0,timestamp,0);inside=i%2;expected,events=old.run(s,facts,inside);actual,calls=new.run(s,facts,inside);assert equal(expected,actual),(i,expected.hex(),actual.hex());assert len(events)==len(calls) and all(equal(x,y) for x,y in zip(events,calls)),(i,events,calls);callbacks+=len(events);restarts+=struct.unpack_from('<I',expected,68)[0];inp=s+facts+words(inside);records.append(words(len(inp),len(expected),len(events))+inp+expected+b''.join(events))
 blob=b'VTR1'+words(len(records))+b''.join(records);a.reference_output.write_bytes(blob);report={'original_sha256':manifest['original_sha256'],'native_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'corpus_sha256':hashlib.sha256(blob).hexdigest(),'comparisons':len(records),'ordered_callbacks':callbacks,'root_restart_requests':restarts,'mismatches':0,'seconds':round(time.monotonic()-start,3),'scope':'Actual full BlendedPlayClip control flow after fixture animator resolution/selection, actual GetApplicator/GetLoop/Jump/Scale/NewAnim control flow. Root Enable/reset backend observed; root onAnimate optionally executes actual timeline update. Existing authored selection/refcounts/blend weights remain external.'};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
if __name__=='__main__':main()
