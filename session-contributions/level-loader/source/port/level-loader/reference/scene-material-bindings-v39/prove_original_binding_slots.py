from pathlib import Path
import sys,struct,json,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
shared=Path(r'C:\Users\adamc\Desktop\workspace\DH_sc');sys.path.insert(0,str(shared/'port/game-data/tests'))
from items_differential import Original as Base
W=lambda *v:struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
class Original(Base):
 def __init__(self):
  self.trace=[];super().__init__(shared/'.local-inputs/libDungeonHunter2.so',{'functions':[]})
  self.mesh=self.data+0x6000;self.meshvt=self.data+0x6100;self.factory=self.data+0x6200;self.factoryvt=self.data+0x6300
  self.db=self.data+0x6400;self.root=self.data+0x6500;self.driver=self.data+0x6600;self.result=self.data+0x6700
  self.uc.mem_write(self.mesh,bytes(256));self.pointer(self.mesh,self.meshvt);self.pointer(self.meshvt+0x20,self.callback+128)
  self.pointer(self.factory,self.factoryvt);self.pointer(self.factoryvt+0x24,self.callback+132);self.pointer(self.db+4,self.factory)
 def storage(self,uc,address,size,unused):
  if address==0x61aaa8:self.pointer(self.reg(0),self.mesh);self.returned(self.reg(0)) # Explicit mesh-construction endpoint.
  elif address==0x60e400:self.returned(self.data+0x8000+36*self.reg(1)) # Explicit material-catalog endpoint, IDs delivered as record addresses.
  elif address==0x65cafc:self.pointer(self.reg(0),self.reg(2));self.returned(self.reg(0)) # Explicit driver/root material creation endpoint.
  elif address in (0x31d584,0x310be8,0x57a26c):self.returned() # Endpoint-only fixture reference release.
  else:super().storage(uc,address,size,unused)
 def external(self,uc,address,size,unused):
  if address==self.callback+128:
   self.trace.append((self.reg(1),(self.word(self.reg(2))-self.data-0x8000)//36));self.returned()
  elif address==self.callback+132:self.pointer(self.reg(0),0);self.returned(self.reg(0)) # Actual factory attribute-map endpoint explicit fixture.
  else:super().external(uc,address,size,unused)
c=Original();geometry=c.data+0x9000;bindings=c.data+0xa000;name=c.data+0xb000;c.uc.mem_write(name,b'#FixtureGeometry\0')
cases=[]
for ids in [[],[0],[1,2],[2,0,2],[7,3,1,0,5,2,7]]:
 c.trace=[];c.uc.mem_write(geometry,W(0,name,0,len(ids),bindings,0));c.uc.mem_write(bindings,bytes(60*max(1,len(ids))))
 for i,index in enumerate(ids):c.pointer(bindings+60*i+8,index)
 c.invoke(0x61aeb8,[c.result,c.db,c.driver,geometry,c.root])
 assert c.trace==list(enumerate(ids)),(ids,c.trace)
 cases.append({'material_catalog_ids':ids,'original_setMaterial_calls':[{'primitive_slot':slot,'material_catalog_id':idx} for slot,idx in c.trace]})
out=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader\reference\scene-material-bindings-v39')
report={'validation':'PASS','original_sha256':hashlib.sha256((shared/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'original_constructGeometry_body':'0x61aeb8','cases':cases,'scope':'Original whole constructGeometry local-reference binding iteration and exact setMaterial slot assignment. Mesh/resource/GPU catalog/creation, vertex-map and reference-release endpoints are explicit fixtures. No native GPU readiness or original material creation claimed.'}
(out/'original-binding-loop-gold.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report,indent=2))
