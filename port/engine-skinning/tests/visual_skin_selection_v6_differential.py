"""Actual ordered modular/weapon source bodies versus optimized native kernels.
Controller construction, GPU packing, scene search/attachment and intrusive
storage are explicit services here. The separate cache host audit constructs
genuine decoded resources; this instruction proof does not emulate a GPU.
"""
import sys,struct,json,hashlib,argparse
from pathlib import Path
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/engine-resources/tests'));from cpu import Cpu,i32
sys.path.insert(0,str(R/'port/game-data/tests'));from fresh_inventory_owned_v4_arm64 import InventoryCpu
from unicorn import UC_HOOK_CODE
W=lambda *v:struct.pack('<'+'I'*len(v),*(v&0xffffffff for v in v))
Q=lambda *v:struct.pack('<'+'Q'*len(v),*v)
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest()
class Source(Cpu):
 def __init__(self):
  super().__init__(R/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});self.heap=self.data+0x10000;self.active=False;self.imports[self.callback+16]='source_detach';self.imports[self.callback+32]='source_attach';self.uc.hook_add(UC_HOOK_CODE,self.service)
 def word(self,p):return struct.unpack('<I',self.uc.mem_read(p,4))[0]
 def alloc(self,n):p=self.heap;self.heap+=(n+15)&~15;self.uc.mem_write(p,bytes(n));return p
 def text(self,p):
  b=bytearray()
  while p and self.uc.mem_read(p+len(b),1)!=b'\0':b.extend(self.uc.mem_read(p+len(b),1))
  return bytes(b)
 def string(self,b):p=self.alloc(len(b)+1);self.uc.mem_write(p,b+b'\0');return p
 def ret(self,v=0):self.put(0,v);self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))
 def external(self,uc,a,z,u):
  n=self.imports.get(a)
  if n=='strcmp':x,y=self.text(self.reg(0)),self.text(self.reg(1));self.ret((x>y)-(x<y))
  elif n=='strlen':self.ret(len(self.text(self.reg(0))))
  elif n=='source_detach':self.trace.append(['detach',self.identity(self.reg(0))]);self.ret()
  elif n=='source_attach':self.trace.append(['attach',self.identity(self.reg(1))]);self.ret()
  else:super().external(uc,a,z,u)
 def identity(self,p):return self.identities.get(p,0) if p else 0
 def service(self,uc,a,z,u):
  if not self.active:return
  if a==0x3140ec:
   obj,p=self.reg(0),self.reg(1);self.strings[obj]=self.text(p);self.pointer(obj+20,self.string(self.strings[obj]));self.ret(obj)
  elif a==0x310804:
   obj,p,end=[self.reg(i) for i in range(3)];self.strings[obj]+=bytes(uc.mem_read(p,end-p));self.pointer(obj+20,self.string(self.strings[obj]));self.ret(obj)
  elif a in (0x708f00,0x310440):self.ret()
  elif a==0x61ace8:
   uri=self.text(self.word(self.reg(3)+4));self.trace.append(['construct_module',uri.decode()]);p=self.new_resource if self.success else 0
   if getattr(self,'mutation',False):self.uc.mem_write(self.cells,W(17,self.intervening_resource))
   if getattr(self,'dynamic',False) and p:p=self.alloc(0x100);self.pointer(p+4,1);self.identities[p]=len(self.identities)+100;self.uris[p]=uri.decode()
   self.pointer(self.reg(0),p);self.ret(self.reg(0))
  elif a==0x31d584:
   p=self.reg(0);self.trace.append(['release',self.identity(p)]);self.pointer(p+4,self.word(p+4)-1);self.ret()
  elif a==0x6483c8:self.trace.append(['update_buffers',self.reg(1)]);self.ret()
  elif a==0x5890a8:self.trace.append(['visibility']);self.ret()
  elif a==0x61bbd4:
   uri=self.text(self.reg(1)).decode();self.trace.append(['construct_weapon',uri,self.reg(2)]);p=self.new_resource if self.success else 0
   if getattr(self,'dynamic',False) and p:
    path=R/'.local-inputs/visual-skin-owner-v6/weapons'/uri.rsplit('/',1)[-1].lower();p=self.alloc(0x100)if path.exists()else 0
    if p:self.pointer(p+4,1);self.pointer(p,self.word(self.old_resource));self.identities[p]=len(self.identities)+100;self.uris[p]=uri
   self.ret(p)
  elif a==0x35a0e4:self.trace.append(['search',self.text(self.reg(2)).decode(),self.reg(3)]);self.ret(self.parent if self.found else 0)
 def setup(self,catalog,old_id,old_present,flags,success,found=True,mutation=False):
  self.heap=self.data+0x10000;self.trace=[];self.strings={};self.success=success;self.found=found;self.mutation=mutation;self.identities={};self.uris={};self.visual=self.alloc(0x100);self.mesh=self.alloc(0x100);self.cells=self.alloc(8*len(catalog));self.instance=self.alloc(16);cats=self.alloc(16*len(catalog));self.pointer(self.instance,len(catalog));self.pointer(self.instance+4,cats)
  for i,c in enumerate(catalog):
   mods=self.alloc(8*len(c['modules']));self.uc.mem_write(cats+16*i,W(self.string(c['name'].encode()),self.string(c['default'].encode()),len(c['modules']),mods))
   for j,m in enumerate(c['modules']):desc=self.alloc(24);self.pointer(desc+4,self.string(m['uri'].encode()));self.uc.mem_write(mods+8*j,W(2,desc))
   self.uc.mem_write(self.cells+8*i,W(old_id,0))
  self.pointer(self.mesh+0x1c,self.instance);self.pointer(self.mesh+0x24,self.cells);self.pointer(self.mesh+0x14,flags)
  node=self.alloc(0x150);self.pointer(node+0x134,self.mesh);self.pointer(self.visual+0x2c,node)
  self.old_resource=self.alloc(0x100);self.new_resource=self.alloc(0x100);self.parent=self.alloc(0x180);self.identities[self.old_resource]=101;self.identities[self.new_resource]=202;self.pointer(self.old_resource+4,1);self.pointer(self.new_resource+4,1)
  self.intervening_resource=self.alloc(0x100);self.pointer(self.intervening_resource+4,1);self.identities[self.intervening_resource]=303
  # Caller-projected source virtual receivers for removal/add-child.
  vt=self.alloc(0x100);self.pointer(vt+0x68,self.callback+16);self.pointer(self.old_resource,vt);pvt=self.alloc(0x100);self.pointer(pvt+0x5c,self.callback+32);self.pointer(self.parent,pvt)
  self.pointer(self.cells+4,self.old_resource if old_present else 0);self.pointer(self.visual+0x30,self.old_resource if old_present else 0);self.pointer(self.visual+0x34,self.old_resource if old_present else 0)
  # Exact GOT producer projections required before the explicit factory call.
  app=self.alloc(0x100);video=self.alloc(0x100);self.pointer(app+0x20,video);self.pointer(video+0x10,123)
  base=(0x648ff0+self.word(0x6490dc))&0xffffffff;self.pointer(self.word(base+self.word(0x6490e0)),app)
  base=(0x473cf0+self.word(0x473e84))&0xffffffff;holder=self.word(base+self.word(0x473e94));application=self.alloc(0x100);scene=self.alloc(0x100);self.pointer(holder,application);self.pointer(application+0x10,scene);self.pointer(application+0x20,video);self.pointer(scene+0x10,123);self.pointer(scene+0x1c,456)
  guard=self.word(base+self.word(0x473e88));self.pointer(guard,0x23456789)
  self.active=True
class Native(InventoryCpu):
 def __init__(self,p):super().__init__(p);self.imports[self.callback]='v6_service'
 def identity(self,p):return self.identities.get(p,0) if p else 0
 def external(self,uc,a,z,u):
  if self.imports.get(a)!='v6_service':return super().external(uc,a,z,u)
  req=self.reg(2);op,c,m,slot,mode=struct.unpack('<I4i',uc.mem_read(req,20));resource,desc,text=struct.unpack('<3Q',uc.mem_read(req+24,24));result=0
  if op==0x61ace8:
   self.trace.append(['construct_module',self.text(text).decode()]);result=self.new_resource if self.success else 0
   if self.mutation:self.uc.mem_write(self.cells,W(17,0)+Q(self.intervening_resource))
  elif op==0x649080:self.refs[resource]+=1
  elif op==0x31d584:self.trace.append(['release',self.identity(resource)]);self.refs[resource]-=1
  elif op==0x6483c8:self.trace.append(['update_buffers',mode])
  elif op==0x5890a8:self.trace.append(['visibility'])
  elif op==0x61bbd4:self.trace.append(['construct_weapon',self.text(text).decode(),1]);result=self.new_resource if self.success else 0
  elif op==0x35a0e4:self.trace.append(['search',self.text(text).decode(),0]);result=self.parent if self.found else 0
  elif op==0x473dc0:self.trace.append(['detach',self.identity(resource)])
  elif op==0x473e14:self.trace.append(['attach',self.identity(resource)])
  else:raise AssertionError(hex(op))
  self.pointer(self.reg(3),result);self.put(0,1);uc.reg_write(self.pc,uc.reg_read(self.lr))
 def alloc(self,n):p=self.heap;self.heap+=(n+15)&~15;self.uc.mem_write(p,bytes(n));return p
 def string(self,s):b=s.encode();p=self.alloc(len(b)+1);self.uc.mem_write(p,b+b'\0');return p
 def setup(self,catalog,old_id,old_present,flags,success,found=True,mutation=False):
  self.heap=self.data+0x200000;self.trace=[];self.success=success;self.found=found;self.mutation=mutation;self.identities={};self.refs={};cats=self.alloc(32*len(catalog));self.cells=self.alloc(16*len(catalog));self.s=self.alloc(48);self.v=self.alloc(16)
  for i,c in enumerate(catalog):
   mods=self.alloc(16*len(c['modules']));self.uc.mem_write(cats+32*i,Q(self.string(c['name']),self.string(c['default']),mods)+W(len(c['modules']),0))
   for j,m in enumerate(c['modules']):self.uc.mem_write(mods+16*j,Q(self.string(m['uri']),0x100000000+i*1000+j))
   self.uc.mem_write(self.cells+16*i,W(old_id,0)+Q(0))
  self.old_resource=self.alloc(8);self.new_resource=self.alloc(8);self.parent=self.alloc(8);self.identities[self.old_resource]=101;self.identities[self.new_resource]=202;self.refs={self.old_resource:1,self.new_resource:1};self.pointer(self.cells+8,self.old_resource if old_present else 0)
  self.intervening_resource=self.alloc(8);self.identities[self.intervening_resource]=303;self.refs[self.intervening_resource]=1
  self.uc.mem_write(self.s,Q(cats,self.cells)+W(len(catalog),flags)+Q(self.old_resource if old_present else 0,self.old_resource if old_present else 0,0));self.uc.mem_write(self.v,Q(self.s,self.callback))
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);a=ap.parse_args();old=Source();new=Native(a.library);catalog=json.loads((R/'.local-inputs/visual-skin-owner-v6/catalog.json').read_text())['instances'][0]['categories'];cases=[];requests=0
 def compare(kind,old_id,old_present,flags,success,arg,update=1,found=True,slot=1,mutation=False):
  nonlocal requests
  old.setup(catalog,old_id,old_present,flags,success,found,mutation);new.setup(catalog,old_id,old_present,flags,success,found,mutation)
  if kind=='category':r=old.invoke(0x647538,[old.mesh,old.string(arg.encode())]);nr=new.invoke('dh2_visual_category_v6',[new.s,new.string(arg)])
  elif kind=='module':r=old.invoke(0x6474b8,[old.mesh,old.string(arg.encode())]);nr=new.invoke('dh2_visual_module_uri_v6',[new.s,new.string(arg)])
  elif kind=='set':
   old.invoke(0x648fd0,[old.mesh,0,arg,update]);nr=new.invoke('dh2_visual_set_category_v6',[new.s,0,arg,update,new.v]);r=0;assert nr==0
   assert i32(old.word(old.cells))==i32(struct.unpack('<I',new.uc.mem_read(new.cells,4))[0]);assert old.identity(old.word(old.cells+4))==new.identity(struct.unpack('<Q',new.uc.mem_read(new.cells+8,8))[0]);assert old.word(old.new_resource+4)==new.refs[new.new_resource];assert old.word(old.old_resource+4)==new.refs[new.old_resource]
   assert old.word(old.intervening_resource+4)==new.refs[new.intervening_resource]
  else:
   old.invoke(0x473cd8,[old.visual,old.string(arg.encode())if arg is not None else 0,slot,update]);nr=new.invoke('dh2_visual_set_weapon_v6',[new.s,new.string(arg)if arg is not None else 0,slot,update,new.v]);r=0;assert nr==0;assert old.identity(old.word(old.visual+(0x30 if slot==1 else 0x34)))==new.identity(struct.unpack('<Q',new.uc.mem_read(new.s+(24 if slot==1 else 32),8))[0])
  assert i32(r)==i32(nr),(kind,arg,r,nr);assert old.trace==new.trace,(kind,arg,old.trace,new.trace);requests+=len(old.trace);cases.append({'kind':kind,'old_id':old_id,'old_present':old_present,'flags':flags,'success':success,'argument':arg,'update_or_mode':update,'found':found,'slot':slot,'mutation':mutation,'return':i32(r),'trace':old.trace})
 for c in catalog:compare('category',-1,0,0,1,c['name'])
 for n in ('','MC_missing','mc_feet'):compare('category',-1,0,0,1,n)
 for c in catalog:
  for m in c['modules']:compare('module',-1,0,0,1,m['uri'])
 for n in ('','#missing-mesh-skin','#MC_Feet__naked'):compare('module',-1,0,0,1,n)
 for old_id in (-1,0,1):
  for present in (0,1):
   for flags in (0,1,0xffffffff):
    for success in (0,1):
     for update in (0,1):
      for arg in (-1,0,1,42):compare('set',old_id,present,flags,success,arg,update)
 for present in (0,1):
  for success in (0,1):
   for update in (0,1):compare('set',-1,present,0,success,42,update,mutation=True)
 for present in (0,1):
  for success in (0,1):
   for found in (0,1):
    for slot in (0,1,2,99):
     for mode in (0,1,2):
      for name in (None,'MC_RWeapon_LongSword_01','MC_LWeapon_Claw_01'):compare('weapon',-1,present,0,success,name,mode,found,slot)
 ref=R/'port/engine-skinning/reference/visual-skin-owner-v6';ref.mkdir(parents=True,exist_ok=True);gold=ref/'selection-fixtures.json';gold.write_text(json.dumps({'catalog':catalog,'cases':cases},indent=2)+'\n');sources=['port/engine-skinning/visual_skin_selection_v6.hpp','port/engine-skinning/visual_skin_selection_v6.cpp',Path(__file__).relative_to(R).as_posix()];report=dict(validation='PASS',comparisons=len(cases),ordered_requests=requests,actual_modular_records=172,mismatches=0,original_sha256=sha(R/'.local-inputs/libDungeonHunter2.so'),library_sha256=sha(a.library),gold_sha256=sha(gold),source_sha256={p:sha(R/p)for p in sources},scope=__doc__);p=R/'port/engine-skinning/reports/visual-skin-selection-v6-arm64-differential.json';p.parent.mkdir(exist_ok=True);p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
