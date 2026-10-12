from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
b=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');sys.path.insert(0,str(b/'tests'))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
e=Path('C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');r=b/'reference/light-lifetime-v55'
class Dtor(Parser):
 def __init__(self):
  super().__init__(e,{'functions':[]});self.actor=self.data+0x60000;self.node=self.data+0x61000;self.vtable=self.data+0x62000;self.manager=self.data+0x63000;self.auto=self.data+0x64000;self.events=[];self.uc.hook_add(UC_HOOK_CODE,self.allocation);self.uc.hook_add(UC_HOOK_CODE,self.trace)
 def trace(self,uc,a,size,u):
  if a==0x3139ac:self.events.append({'op':'actual CString deallocation body','receiver_offset':hex(self.reg(0)-self.actor)})
  elif a==0x31d584:self.events.append({'op':'actual node IReferenceCounted.drop','node':hex(self.reg(0)),'refcount_before':self.word(self.node+4)})
  elif a==0x33e998:self.events.append({'op':'actual ObjectBase destructor body'})
 def run(self,node):
  self.uc.mem_write(self.actor,bytes(0x200));self.uc.mem_write(self.manager,bytes(0x500));self.invoke(0x40bdd8,[self.actor],budget=4000000);self.uc.mem_write(self.node,bytes(0x200));self.pointer(self.vtable-12,0);self.pointer(self.node,self.vtable);self.pointer(self.node+4,2);self.pointer(self.actor+0x120,self.node if node else 0)
  self.pointer(self.auto,self.actor);self.pointer(self.manager+0x424,self.auto);self.pointer(self.manager+0x428,self.auto+4);self.pointer(self.manager+0x42c,self.auto+4)
  before=bytes(self.uc.mem_read(self.manager+0x424,12));self.events=[];self.invoke(0x40bd1c,[self.actor],budget=6000000)
  return {'node_present':node,'events':self.events.copy(),'actor120_after':self.word(self.actor+0x120),'node_reference_count_after':self.word(self.node+4),'automatic_list_bytes_unchanged':before==bytes(self.uc.mem_read(self.manager+0x424,12)),'automatic_list_still_borrows_old_actor':self.word(self.auto)==self.actor}
p=Dtor();rows=[p.run(False),p.run(True)];assert rows[1]['actor120_after']==0 and rows[1]['node_reference_count_after']==1 and rows[1]['automatic_list_bytes_unchanged']
report={'validation':'PASS','actual_original_C1_and_full_Dtor_bodies':True,'actual_original_IReferenceCounted_drop':True,'node_fixture':'source-layout node refcount2; independent root owner retains1; no final renderer deletion asserted','automatic_list_fixture':'borrowed raw actor pointer in original embedded manager vector; no source destructor method touches it','original_sha256':hashlib.sha256(e.read_bytes()).hexdigest(),'rows':rows};(r/'original-lightpoint-dtor-v55.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
