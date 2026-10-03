"""Original UpdatePath -> Stop -> actual setters -> SetXForm -> sleep reset.

Paths are empty, boundary/avoidance policies disabled, and scene/body/world
ownership supplied. Original empty-path DropPath and Character policy getters
execute. Only shape Synchronize/DestroyProxy and broadphase Commit are fixtures.
This audits physical Stop orchestration, not complete paths, shape/proxy services,
contacts, stepping, root motion or UpdateSubObjects. sin/cos/atan IEEE services
are consistently modeled; generated NaNs compare by class.
"""
import argparse,hashlib,json,math,random,struct,time
from unittest.mock import patch
from unicorn.arm64_const import UC_ARM64_REG_S0
from navigation_controller_differential import OriginalController,ControllerCpu
from navigation_avoidance_differential import OriginalAvoidance,actor
import navigation_controller_differential as controller
from navigation_differential import ROOT,equal
from navigation_search_differential import words,word
from aggro_differential import float_bits
from combat_result_differential import floating

class Cpu(ControllerCpu):
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name=='physical_callback':return self.physical_callback(uc,address,size,unused)
  if name in ('sinf','cosf'):
   raw=uc.reg_read(UC_ARM64_REG_S0) if self.arm64 else self.reg(0);f=floating(raw)
   result=float_bits((math.sin if name=='sinf' else math.cos)(f)) if math.isfinite(f) else 0x7fc00000
   if self.arm64:uc.reg_write(UC_ARM64_REG_S0,result)
   else:self.put(0,result)
   self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)

class Original(OriginalController):
 def hook(self,uc,address,size,unused):
  if getattr(self,'running',False):
   c=self.c
   if address in (0x46e918,0x46e978,0x46ea80):self.physical_calls.append(address);return OriginalAvoidance.hook(self,uc,address,size,unused)
   if address==0x52aae4:self.drop_count+=1
   if address==0x7e164c:self.transform_calls+=1;self.transform_request=bytes(uc.mem_read(c.reg(1),8))+words(c.reg(2),1);self.transform_mid=self.body_state()
   if address==0x3939b0:self.transform_result=c.reg(0)
   if address in (0x7e63d0,0x7e6180,0x7e2ac8):
    if address==0x7e2ac8:assert c.reg(0)==self.bp;self.events.append((3,0,b''));self.ret();return
    ident=(c.reg(0)-self.shapes_base)//32;assert c.reg(0)==self.shapes_base+ident*32 and c.reg(1)==self.bp
    if address==0x7e63d0:assert c.reg(2)==c.reg(3)==self.body+4;self.events.append((1,ident,bytes(uc.mem_read(self.body+4,24))));self.ret(int(ident!=self.fail))
    else:self.events.append((2,ident,b''));self.ret()
    return
  super().hook(uc,address,size,unused)
 def body_state(self):
  c=self.c;b=self.body;return words(struct.unpack('<H',c.uc.mem_read(b,2))[0])+bytes(c.uc.mem_read(b+4,8))+bytes(c.uc.mem_read(b+0x38,4))+bytes(c.uc.mem_read(b+0x40,24))+bytes(c.uc.mem_read(b+0x8c,4))+bytes(c.uc.mem_read(self.physics[0]+12,4))
 def extra(self):
  c=self.c;b=self.body;return bytes(c.uc.mem_read(b+12,24))+bytes(c.uc.mem_read(b+0x2c,8))+bytes(c.uc.mem_read(b+0x24,8))+bytes(c.uc.mem_read(b+0x34,4))+bytes(4)

def main():
 p=argparse.ArgumentParser()
 for name in ('engine','library','report','reference-output'):p.add_argument('--'+name,required=True,type=__import__('pathlib').Path)
 p.add_argument('--cases',type=int,default=1024);a=p.parse_args();started=time.monotonic()
 manifests=[json.loads((ROOT/'reference'/name/'original-functions.json').read_text()) for name in ('navigation-controller','physical-controls','body-transform')];manifest={'original_sha256':manifests[0]['original_sha256'],'functions':list({r['elf_address']:r for m in manifests for r in m['functions']}.values())};assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 geometry={'triangles':[[[[0.,0.,0.],[1.,0.,0.],[0.,1.,0.]]] for _ in range(8)],'bounds':[[0.,0.,0.,1.,1.,0.]]*8,'world_bounds':[0.,0.,0.,1.,1.,0.]}
 with patch.object(controller,'ControllerCpu',Cpu):old=Original(a.engine,manifest,geometry)
 c=old.c;old.heap=c.data+0x100000;old.edge_ptrs={};c.edge_ids={};old.drop_count=0
 for obj in old.objects:c.invoke(0x524644,[obj])
 c.uc.mem_write(old.world,bytes(160));old.body=c.data+0x1e00000;old.physics_world=c.data+0x1d10000;old.shapes_base=c.data+0x1d40000;old.bp=c.data+0x1d50000
 new=Cpu(a.library,True,{'functions':[]});d=new.data;state=d+0x1000;path=d+0x1100;object=d+0x1200;policy=d+0x1300;scene=d+0x1400;actors=d+0x1500;keyptr=d+0x1900;workspace=d+0x2000;request=d+0x2100;output=d+0x2200;bs=d+0x2300;bt=d+0x2400;world=d+0x2500;cb=d+0x2600;binding=d+0x2700;sh=d+0x2800
 context=0xabcdef0123456789;nbp=0xb23456789abcdef0;events=[];fail=-1
 def new_hook(uc,address,size,unused):
  assert new.reg(0)==context
  if address==new.callback+64:assert new.reg(1)==nbp;events.append((3,0,b''))
  else:
   ident=new.reg(1)-0x100000000;assert new.reg(2)==nbp
   if address==new.callback+32:assert new.reg(3)==new.reg(4);events.append((1,ident,bytes(uc.mem_read(new.reg(3),24))));new.put(0,int(ident!=fail))
   else:events.append((2,ident,b''))
  uc.reg_write(new.pc,uc.reg_read(new.lr))
 for addr in (new.callback+32,new.callback+48,new.callback+64):new.imports[addr]='physical_callback'
 new.physical_callback=new_hook;new.uc.mem_write(cb,struct.pack('<3Q',new.callback+32,new.callback+48,new.callback+64));new.uc.mem_write(keyptr,struct.pack('<Q',0x100000001));new.uc.mem_write(workspace,bytes(48));records=[];rng=random.Random(20261023);totals={'stopped':0,'applied':0,'locked':0,'frozen':0,'failure':0,'commit':0,'sync':0,'destroy':0,'transform_calls':0};samples=[]
 def compare(before,policy_words,present,locked,count,failed,before_body,before_extra,label=None):
  nonlocal fail
  fail=failed;old.fail=failed;old.events=[];events.clear();old.transform_result=0;old.transform_calls=0;old.transform_request=bytes(16);old.transform_mid=before_body
  pos=struct.unpack_from('<3f',before);dest=struct.unpack_from('<3f',before,12);actor_raw=actor(position=pos,target=dest,floor=0,flags=14,user=0x100000001,physical=(present,0,0,1,0,1,65535,1,0,0,0,0));raw=actor_raw+b''.join(actor(user=0x100000001+i) for i in range(1,8));old.load_scene(raw,words(0,0))
  path_raw=words(2)+struct.pack('<16f',36.,*pos,*dest,*pos,*dest,13.,-17.,0.)+words(0,0);policy_raw=words(*policy_words);old.configure(0,before,policy_raw,path_raw)
  b=old.body;c.uc.mem_write(b,bytes(0x100));c.uc.mem_write(b,struct.pack('<H',struct.unpack_from('<I',before_body)[0]));c.uc.mem_write(b+4,before_body[4:12]);c.uc.mem_write(b+0x38,before_body[12:16]);c.uc.mem_write(b+0x40,before_body[16:40]);c.uc.mem_write(b+0x8c,before_body[40:44]);c.uc.mem_write(old.physics[0]+12,before_body[44:48]);c.uc.mem_write(b+12,before_extra[:24]);c.uc.mem_write(b+0x2c,before_extra[24:32]);c.uc.mem_write(b+0x24,before_extra[32:40]);c.uc.mem_write(b+0x34,before_extra[40:44]);c.pointer(b+0x58,old.physics_world);c.pointer(b+0x64,old.shapes_base if count else 0);c.uc.mem_write(old.physics_world,bytes(0x19200));c.pointer(old.physics_world+0x191d8,old.bp);c.uc.mem_write(old.physics_world+0x191d4,bytes((locked,)))
  for i in range(count):c.pointer(old.shapes_base+i*32+8,old.shapes_base+(i+1)*32 if i+1<count else 0);new.uc.mem_write(sh+i*16,struct.pack('<2Q',0x100000000+i,sh+(i+1)*16 if i+1<count else 0))
  new.uc.mem_write(state,before);new.uc.mem_write(path,path_raw[:68]+bytes(28));new.uc.mem_write(object,actor_raw[:64]);new.uc.mem_write(actors,actor_raw);new.uc.mem_write(scene,struct.pack('<3Q2I',0,actors,keyptr,1,0));new.uc.mem_write(policy,policy_raw);new.uc.mem_write(request,struct.pack('<9Q',state,path,object,0,0,scene,policy,workspace,0x100000001));new.uc.mem_write(output,bytes(88));new.uc.mem_write(bs,before_body);new.uc.mem_write(bt,struct.pack('<Q',bs)+before_extra);new.uc.mem_write(world,struct.pack('<3QB7x',context,nbp,sh if count else 0,locked));new.uc.mem_write(binding,struct.pack('<3Q',bt,world,cb))
  expected=old.execute(1,0);assert new.invoke('dh2_nav_update_path_physical',[output,request,binding if present else 0])==0
  after=bytes(new.uc.mem_read(state,56));after_object=bytes(new.uc.mem_read(object,64));after_path=bytes(new.uc.mem_read(path,68))+words(0,0);out=bytes(new.uc.mem_read(output,88));after_body=bytes(new.uc.mem_read(bs,48));after_extra=bytes(new.uc.mem_read(bt+8,48));expected_result=expected[4]+words(old.transform_result,bool(old.transform_calls))
  for label2,left,right in [('controller',expected[0],after),('object',expected[1],after_object),('path',expected[2],after_path),('result',expected_result,out),('body',old.body_state(),after_body),('extra',old.extra(),after_extra)]:assert equal(left,right),(len(records),label,label2,left.hex(),right.hex())
  assert len(old.events)==len(events) and all(e[:2]==n[:2] and equal(e[2],n[2]) for e,n in zip(old.events,events)),('services',label,old.events,events)
  assert old.physical_calls==([0x46e918,0x46e978,0x46ea80] if old.transform_calls else []),'actual setter order'
  stopped=struct.unpack_from('<I',out,60)[0];applied=struct.unpack_from('<I',out,84)[0];totals['stopped']+=stopped;totals['applied']+=applied;totals['locked']+=bool(applied and locked);totals['frozen']+=bool(applied and not locked and struct.unpack_from('<I',before_body)[0]&2);totals['failure']+=bool(applied and any(k==2 for k,i,p in events));totals['commit']+=sum(k==3 for k,i,p in events);totals['sync']+=sum(k==1 for k,i,p in events);totals['destroy']+=sum(k==2 for k,i,p in events);totals['transform_calls']+=old.transform_calls
  records.append(words(present,locked,count,len(old.events))+struct.pack('<i',failed)+policy_raw+before+actor_raw[:64]+path_raw+before_body+before_extra+expected[0]+expected[1]+expected[2]+expected_result+old.body_state()+old.extra()+b''.join(words(k,i)+payload+bytes(24-len(payload)) for k,i,payload in old.events))
  if label:samples.append({'case':label,'stopped':stopped,'applied':applied,'transform_result':old.transform_result,'events':[[k,i] for k,i,p in events]})
 def controller_state(pos=(100.,-200.,300.),arrived=True,requested=1,active=1):return struct.pack('<10f4I',*pos,*(pos if arrived else (pos[0]+1000.,pos[1],pos[2])),.2,-.3,17.,.37,active,0,requested,0)
 def body(flags=8,angle=.37):return words(flags)+struct.pack('<11f',3.,-4.,angle,5.,-6.,7.,8.,-9.,10.,11.,12.)
 extra=struct.pack('<11fI',1.,0.,-0.,1.,13.,-17.,19.,-23.,29.,-31.,.5,0)
 for up in (0,1):
  for physics in (0,1):
   for present in (0,1):
    for requested in (0,1):
     for arrived in (False,True):
      for active in (0,1):compare(controller_state(arrived=arrived,requested=requested,active=active),(up,0,0,physics),present,0,0,-1,body(),extra,f'policy {up} physics {physics} body {present} requested {requested} arrived {arrived} heading {active}')
 for flags in (0,2,8,0xfffd,0xffff):
  for locked in (0,1):
   for count in (0,1,4):
    for failed in range(-1,count):compare(controller_state(),(1,0,0,1),1,locked,count,failed,body(flags),extra,f'transform flags {flags} lock {locked} shapes {count} fail {failed}')
 for pos in ((0.,-0.,0.),(-0.,0.,0.),(3e38,-3e38,0.)):
  for angle_word in (0,0x80000000,0x7f800000,0xff800000,0x7fc01234,0x7f801234):
   raw=bytearray(body());struct.pack_into('<I',raw,12,angle_word);compare(controller_state(pos),(1,0,0,1),1,0,3,-1,bytes(raw),extra,f'position {pos} angle word {angle_word}')
 for n in range(a.cases):
  pos=tuple(rng.uniform(-1e5,1e5) for _ in range(3));arrived=bool(n%3);up=int(n%11!=0);physics=int(n%5!=0);present=int(n%7!=0);count=rng.randrange(5);failed=rng.randrange(-1,count) if count else -1;flags=rng.choice((0,8,2,0xfffd));before_body=words(flags)+words(*[rng.getrandbits(32) for _ in range(11)]);before_extra=words(*[rng.getrandbits(32) for _ in range(11)],0)
  compare(controller_state(pos,arrived,rng.randrange(2),rng.randrange(2)),(up,0,0,physics),present,int(n%13==0),count,failed,before_body,before_extra)
 # Body-presence mismatch is rejected before the core or physical state writes.
 before=[bytes(new.uc.mem_read(at,size)) for at,size in ((state,56),(path,96),(object,64),(bs,48),(bt,56),(output,88))];new.uc.mem_write(policy,words(1,0,0,1));new.uc.mem_write(actors+96,words(1));assert new.invoke('dh2_nav_update_path_physical',[output,request,0])==1
 assert before==[bytes(new.uc.mem_read(at,size)) for at,size in ((state,56),(path,96),(object,64),(bs,48),(bt,56),(output,88))]
 reference=words(0x31465043,len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(reference);report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'mismatches':0,'totals':totals,'original_empty_drop_path_calls':old.drop_count,'actual_original_physical_setters_execute':True,'actual_original_setxform_executes':True,'shape_broadphase_services_are_fixtures':True,'atomic_rejection_checks':1,'original_import_calls':c.import_calls,'behavior_snapshots':samples,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}))
if __name__=='__main__':main()
