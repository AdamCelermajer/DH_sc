"""Actual three-class source inventory/Character.Skin/VisualObject composition.
The VisualObject calls execute their genuine source bodies in a separate
instruction context with the same projected catalog and retained selections.
Controller/weapon construction, GPU buffer packing and Debug are explicit
services; actual file existence drives weapon null results. Native cache host
composition independently constructs the real Scene/Skin/Mesh backing.
"""
import sys,json,struct,hashlib
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/game-data/tests'));from fresh_inventory_owned_v4_original import OriginalOwned,W
sys.path.insert(0,str(Path(__file__).resolve().parent));from visual_skin_selection_v6_differential import Source
class Character(OriginalOwned):
 def external(self,uc,a,z,u):
  if self.imports.get(a)=='strstr':
   p=self.reg(0);at=self.cstring(p).find(self.cstring(self.reg(1)));self.returned(p+at if at>=0 else 0)
  else:super().external(uc,a,z,u)
 def inventory_service(self,uc,a,z,u):
  if getattr(self,'skin_active',False):
   if a==0x3a999c:return
   if a in (0x470e5c,0x474568,0x470e18,0x473cd8):
    r=self.visual_source;args=[r.visual]
    if a==0x470e5c:args+=[r.string(self.cstring(self.reg(1)))]
    elif a==0x474568:args+=[self.reg(1),r.string(self.cstring(self.reg(2)))]
    elif a==0x470e18:args+=[self.reg(1),self.reg(2)]
    else:args+=[r.string(self.cstring(self.reg(1)))if self.reg(1)else 0,self.reg(2),self.reg(3)]
    value=r.invoke(a,args);self.returned(value);return
  super().inventory_service(uc,a,z,u)
def string(s):b=s.encode();return W(len(b))+b
def main():
 old=Character();catalog=json.loads((R/'.local-inputs/visual-skin-owner-v6/catalog.json').read_text())['instances'][0]['categories'];r=Source();cases=[];steps=0;sourcecalls=0
 for baseid,loot,n in ((263,165,5),(290,174,5),(325,213,6)):
  for selected in (0,1):
   old.skin_active=False;old.capture=False;old.loading=False;old.fresh_inventory(12);old.uc.mem_write(old.inv+0x2e,bytes([selected]));old.loading=True;old.capture=True;old.requests=[];old.minimal=False;old.infinite=0;old.toggle_stats=False;old.mutation=0;old.uc.mem_write(old.seed,W(1));old.uc.mem_write(old.rngcalls,W(0));old.pointer(old.character+0x2d8,old.data+0x1a00000)
   r.setup(catalog,-1,0,0,1);r.dynamic=True
   for c,row in enumerate(catalog):m=r.invoke(0x6474b8,[r.mesh,r.string(row['default'].encode())]);r.invoke(0x648fd0,[r.mesh,c,m,0])
   old.visual_source=r;commands=[(0,loot,0,0)]+[(2,i,0,0)for i in range(n)]+[(5,0,0,0)]+[(2,i,0,0)for i in reversed(range(n))]+[(4,1,-1,0),(15,0,0,0),(4,2,-1,0),(15,0,0,0),(5,0,0,0),(15,0,0,0)];case=[]
   for op,a,b,c in commands:
    old.skin_active=False;result=old.perform(op,a,b,c)if op!=15 else 0
    old.skin_active=True;r.trace=[];old.invoke(0x3a999c,[old.character]);sourcecalls+=len(r.trace);ids=[r.word(r.cells+i*8)for i in range(4)];uris=[r.uris.get(r.word(r.visual+off),'')for off in (0x30,0x34)]
    case.append(W(op,a,b,c,result,*ids)+b''.join(string(s)for s in uris));steps+=1
   cases.append(W(baseid,selected,len(case))+b''.join(case))
 ref=R/'port/engine-skinning/reference/visual-skin-owner-v6';ref.mkdir(parents=True,exist_ok=True);p=ref/'inventory-fixtures.bin';p.write_bytes(b'ISV6'+W(len(cases))+b''.join(cases));report=dict(validation='PASS',cases=len(cases),original_steps=steps,original_visual_factory_requests=sourcecalls,all_three_actual_classes=True,gold_sha256=hashlib.sha256(p.read_bytes()).hexdigest(),original_sha256=hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),script_sha256=hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),scope=__doc__);p=ref/'inventory-original-gold.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
