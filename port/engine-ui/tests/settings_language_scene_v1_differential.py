"""Original setLanguage live graph ordering versus O2 native graph prefix.
Original IsPlayer/IsMerchant/IsGameObject wrappers execute; character type and
inventory/item localization bodies are explicit caller services. No backend
inventory reconstruction or fake refresh_scene success is claimed.
"""
import argparse,pathlib,sys,json,struct,hashlib,itertools
R=pathlib.Path(__file__).resolve().parents[3];D=R/'.local-inputs/hud-owned-settings-v1'
sys.path.insert(0,str(D));from original_owner import Owner,W
sys.path.insert(0,str(R/'port/engine-resources/tests'));from cpu import Cpu
from unicorn import UC_HOOK_CODE
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=pathlib.Path,required=True);ap.add_argument('--report',type=pathlib.Path,required=True);a=ap.parse_args()
 old=Owner();calls=[];ids={};types={};mutation=False
 chars=[old.data+0x9000+0x1000*i for i in range(3)];objects=[old.data+0xc000+0x1000*i for i in range(3)];cn=[old.data+0x8000+16*i for i in range(3)];on=[old.data+0x8100+64*i for i in range(3)];vt=old.data+0x8200
 ids.update({x:i+1 for i,x in enumerate(chars)});ids.update({x:i+11 for i,x in enumerate(objects)})
 def hook(uc,address,size,u):
  ident=ids.get(old.reg(0),0)
  if address==0x3a3054:old.returned(types[old.reg(0)])
  elif address==0x3a49f0:calls.append([1,ident])
  elif address==0x3a30c4:calls.append([2,ident])
  elif address==0x3fdfa0:calls.append([3,ids[old.reg(0)-0x37c]]);old.returned()
  elif address==0x340054:calls.append([4,ident])
  elif address==0x3ebca8:
   calls.append([5,ident]);p=old.reg(0)
   if mutation:old.uc.mem_write(p+0xf4,W(14))
   old.returned()
 old.uc.hook_add(UC_HOOK_CODE,hook)
 class Native(Cpu):
  def external(self,uc,address,size,u):
   if address==self.callback+48:
    op,res,ident=struct.unpack('<IIQ',uc.mem_read(self.reg(1),16));assert not res and self.reg(0)==0xf123456789abcdef
    self.calls.append([op,ident]);result=0
    if op==1:result=self.types[ident]==1
    elif op==2:result=self.types[ident]==7
    elif op==4:result=1
    elif op==5 and self.mutation:uc.mem_write(self.typeptr[ident],W(14))
    uc.mem_write(self.reg(2),W(result));self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr))
   else:super().external(uc,address,size,u)
 new=Native(a.library,True,{'functions':[]});nbase=new.data+0x1000;nc=[nbase+0x100+i*16 for i in range(3)];no=[nbase+0x200+i*32 for i in range(3)];nv=[nbase+0x300+i*24 for i in range(3)];ns=nbase+0x400;ne=nbase+0x500;sv=nbase+0x600;ty=nbase+0x700;valid=nbase+0x800
 cases=[]
 for ct,ot,mut in itertools.product(itertools.product((1,7,2),repeat=3),((3,14,1),(14,3,3),(1,1,1)),(0,1)):
  old.fresh();mutation=bool(mut);types=dict(zip(chars,ct));calls.clear();old.calls.clear();old.pointer(old.obj+0x60,cn[0]);old.pointer(old.obj+0x14,on[0]);old.pointer(old.obj+0xc+4,on[0]);old.pointer(old.obj+0xc+12,on[-1])
  old.pointer(vt+0x28,0x3a49f0);old.pointer(vt+0x20,0x340054)
  for i,p in enumerate(chars):old.pointer(p,vt);old.pointer(cn[i],cn[i+1] if i<2 else old.obj+0x60);old.pointer(cn[i]+8,p)
  for i,p in enumerate(objects):old.pointer(p,vt);old.uc.mem_write(p+0xf4,W(ot[i]));old.uc.mem_write(p+0x819,b'\1');old.uc.mem_write(on[i],W(0,on[i-1] if i else old.obj+0xc,0,on[i+1] if i<2 else 0)+bytes(28)+W(p))
  old.invoke(0x46d104,[old.manager,2]);expected=[old.uc.mem_read(p+0x819,1)[0] for p in objects];want=calls.copy()
  new.calls=[];new.types={i+1:t for i,t in enumerate(ct)};new.typeptr={i+11:ty+4*i for i in range(3)};new.mutation=mut
  new.uc.mem_write(nbase,struct.pack('<QQQ',ns,ne,no[0]));new.uc.mem_write(ns,struct.pack('<QQ',nc[0],0));new.uc.mem_write(ne,struct.pack('<QQQQ',no[0],no[0],no[-1],0));new.uc.mem_write(sv,struct.pack('<QQ',0xf123456789abcdef,new.callback+48));new.uc.mem_write(ty,W(*ot));new.uc.mem_write(valid,b'\1'*3)
  for i,p in enumerate(nc):new.uc.mem_write(p,struct.pack('<QQ',nc[i+1] if i<2 else ns,i+1))
  for i,p in enumerate(no):new.uc.mem_write(p,struct.pack('<QQQQ',no[i-1] if i else ne,0,no[i+1] if i<2 else 0,nv[i]));new.uc.mem_write(nv[i],struct.pack('<QQQ',i+11,ty+4*i,valid+i))
  assert new.invoke('dh2_settings_v1_refresh_language_scene',[nbase,sv])==0
  assert new.calls==want and list(new.uc.mem_read(valid,3))==expected,(ct,ot,mut,want,new.calls)
  cases.append(W(*ct,*ot,mut,*expected,len(want))+b''.join(W(*x) for x in want))
 gold=b'LSV1'+W(len(cases))+b''.join(cases);path=R/'port/engine-ui/reference/owned-hud-settings-v1/language-scene-fixtures.bin';path.write_bytes(gold)
 report={'validation':'PASS','original_sha256':hashlib.sha256((R/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'comparisons':len(cases),'mismatches':0,'corpus_sha256':hashlib.sha256(gold).hexdigest(),'source_sha256':{f'port/engine-ui/settings_language_scene_v1.{e}':hashlib.sha256((R/f'port/engine-ui/settings_language_scene_v1.{e}').read_bytes()).hexdigest() for e in ('hpp','cpp')},'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),'scope':__doc__}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
