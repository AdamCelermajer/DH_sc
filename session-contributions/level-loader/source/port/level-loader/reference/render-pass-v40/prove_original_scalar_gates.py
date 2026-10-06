from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
shared=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(shared/'port/game-data/tests'));from items_differential import Original as Base
class Original(Base):
 def __init__(self):
  self.calls=0;self.culled=0;super().__init__(shared/'.local-inputs/libDungeonHunter2.so',{'functions':[]});self.node=self.data+0x8000;self.parent=self.data+0x9000;self.manager=self.data+0xa000;self.vtable=self.data+0xb000
  self.uc.mem_write(self.node,bytes(0x200));self.pointer(self.node,self.vtable);self.pointer(self.vtable+0x10,self.callback+128);self.pointer(self.node+0xec,self.parent);self.pointer(self.node+4,self.parent+0xf4);self.pointer(self.node+0xf4,self.node+0xf4)
 def storage(self,uc,address,size,unused):
  if address==0x58ab28:self.returned(self.culled) # Explicit real-camera culling endpoint fixture.
  elif address==0x646730:self.calls+=1;self.returned() # Explicit downstream mesh-render endpoint fixture.
  else:super().storage(uc,address,size,unused)
 def external(self,uc,address,size,unused):
  if address==self.callback+128:self.calls+=1;self.returned(1) # Explicit onRegister body endpoint, sole node/empty children.
  else:super().external(uc,address,size,unused)
c=Original();cases=[]
for flags in (0,1,2,3,0x60f,0xfffffffe,0xffffffff):
 for culled in (0,1):
  c.pointer(c.node+0x11c,flags);c.culled=culled;c.calls=0;c.invoke(0x58b88c,[c.manager,c.node]);assert c.calls==int(bool(flags&1) and not culled),(flags,culled,c.calls);cases.append({'flags':flags,'culled':culled,'registered_calls':c.calls})
render=[]
for enabled in (0,1,2,127,255):
 c.uc.mem_write(c.node+0x138,bytes([enabled]));c.calls=0;c.invoke(0x35a9a8,[c.node,0]);assert c.calls==int(enabled!=0);render.append({'enabled138':enabled,'render_calls':c.calls})
out=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reference\render-pass-v40')
result={'validation':'PASS','original_sha256':hashlib.sha256((shared/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'registration_original':'0x58b88c','mesh_render_gate_original':'0x35a9a8','registration_cases':cases,'render_cases':render,'scope':'Original single-leaf visibility/culling registration gate and whole custom rigid-mesh byte138 render gate. Camera isCulled, node.onRegister and downstream mesh rendering are explicit fixture services. No genuine render-pass registration/material/GL execution claim.'}
(out/'original-scalar-gold.json').write_text(json.dumps(result,indent=2)+'\n');print('PASS registration_cases',len(cases),'render_cases',len(render))
