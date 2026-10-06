from pathlib import Path
import sys,struct,json
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
shared=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(shared/'port/game-data/tests'));from items_differential import Original as Base
class Original(Base):
 def __init__(self):
  self.mode=0;self.passes=[];self.prepares=0;super().__init__(shared/'.local-inputs/libDungeonHunter2.so',{'functions':[]})
  self.node=self.data+0x10000;self.mesh=self.data+0x11000;self.manager=self.data+0x12000;self.driver=self.data+0x13000;self.material=self.data+0x14000;self.renderer=self.data+0x15000;self.technique=self.data+0x16000;self.flags=self.data+0x17000;self.buffer=self.data+0x18000;self.meshvt=self.data+0x19000;self.managervt=self.data+0x1a000
  self.pointer(self.node+0x134,self.mesh);self.pointer(self.node+0x110,self.manager);self.pointer(self.manager,self.managervt);self.pointer(self.manager+0x14,self.driver);self.pointer(self.managervt+0x24,self.callback+180)
  self.pointer(self.mesh,self.meshvt)
  for slot,cb in [(0x10,160),(0x14,164),(0x18,168),(0x38,172),(0x24,176)]:self.pointer(self.meshvt+slot,self.callback+cb)
  self.pointer(self.material+4,self.renderer);self.pointer(self.renderer+0x18,self.technique);self.pointer(self.technique+8,self.flags)
 def storage(self,uc,address,size,unused):
  if address in (0x31d584,0x310be8):self.returned() # Endpoint lifetime fixtures.
  elif address==0x5c5d34:self.returned(0) # Actual technique-selection endpoint supplied0, actual flags memory read by original body.
  else:super().storage(uc,address,size,unused)
 def external(self,uc,address,size,unused):
  if address==self.callback+160:self.returned(1) # One actual source-buffer endpoint fixture.
  elif address==self.callback+164:self.pointer(self.reg(0),self.buffer);self.returned(self.reg(0))
  elif address==self.callback+168:self.pointer(self.reg(0),self.material);self.returned(self.reg(0))
  elif address==self.callback+172:self.returned(self.mode) # Opaque prepare-render mode endpoint.
  elif address==self.callback+176:self.prepares+=1;self.returned()
  elif address==self.callback+180:
   sp=self.uc.reg_read(self.sp);self.passes.append({'source_slot_plus_one':self.reg(3),'pass':self.word(sp)});self.returned(1)
  else:super().external(uc,address,size,unused)
c=Original();cases=[]
for mode in (0,4,5,16,17):
 for flags in (0,0x10000,0xffffffff):
  for nodeflags in (1,0x801):
   c.mode=mode;c.pointer(c.flags+4,flags);c.pointer(c.node+0x11c,nodeflags);c.passes=[];c.prepares=0;c.invoke(0x6463c8,[c.node]);
   expected=[{'source_slot_plus_one':1,'pass':8 if flags&0x10000 else 4}]+([{'source_slot_plus_one':1,'pass':7}] if nodeflags&0x800 else []) if mode in (4,16) else []
   assert c.passes==expected and c.prepares==int(mode==5),(mode,flags,nodeflags,c.passes,c.prepares)
   cases.append({'opaque_mesh_mode':mode,'actual_technique_flags':flags,'node_flags':nodeflags,'register_calls':c.passes,'prepare_only_calls':c.prepares})
out=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reference\render-pass-v40');(out/'original-pass-classifier-gold.json').write_text(json.dumps({'validation':'PASS','original_method':'0x6463c8','cases':cases,'scope':'Whole original one-buffer collada mesh onRegister control-flow/pass-number projection. Mesh modes/buffer/catalog, technique selection and actual backend registration endpoints explicit fixtures. Real original technique flag memory is read by original code; no GPU readiness or opacity guessed from color.'},indent=2)+'\n');print('PASS original mesh pass cases',len(cases))
