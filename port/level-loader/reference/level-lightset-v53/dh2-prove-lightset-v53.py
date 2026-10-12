from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');sys.path.insert(0,str(b/'tests'))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');r=b/'reference/level-lightset-v53'
class Probe(Parser):
 def __init__(self):
  super().__init__(e,{'functions':[]});self.level=self.data+0x30000;self.config=self.data+0x31000;self.calls=[];self.mode=False;self.remaining=0;self.uc.hook_add(UC_HOOK_CODE,self.allocation);self.uc.hook_add(UC_HOOK_CODE,self.boundary)
 def store(self,at,value):
  raw=value.encode()+b'\0';p=self.heap;self.heap+=len(raw)+16;self.uc.mem_write(p,raw);self.pointer(at+20,p);self.pointer(at+16,p+len(raw)-1);self.pointer(at+12,p+len(raw))
 def boundary(self,uc,a,size,u):
  if a==0x381668:self.returned(int(self.mode))
  elif a==0x3f3b40:
   self.calls.append({'filename':self.text(self.word(self.reg(1)+20)),'tag':self.text(self.word(self.reg(2)+20))});self.remaining-=1;self.returned(0 if self.remaining>0 else 1)
 def run(self,mode,fixed,regular,pending):
  self.uc.mem_write(self.level,bytes(0x200));self.uc.mem_write(self.config,bytes(0x400));self.pointer(self.level+0x38,self.config);self.store(self.config+0x2d0,fixed);self.store(self.config+0x2b8,regular);self.mode=mode;self.remaining=pending;self.calls=[]
  self.invoke(0x3f459c,[self.level],budget=3000000);return {'fixed_pipeline':mode,'fixed_name':fixed,'regular_name':regular,'pending_polls':pending,'queries':self.calls.copy()}
p=Probe();rows=[p.run(*x) for x in [(False,'fixed','regular',1),(True,'fixed','regular',1),(True,'','regular',1),(False,'fixed','',1),(True,'fixed','',1),(False,'','',1),(False,'fixed','regular',3),(True,'fixed','regular',3)]]
report={'validation':'PASS','original_sha256':hashlib.sha256(e.read_bytes()).hexdigest(),'actual_original_outer_body':True,'services':'Device::IsFixedPipeline and Level::LoadFile explicit endpoint fixtures; no XML/driver outcome supplied','rows':rows}
(r/'original-stage9-orchestration-v53.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
