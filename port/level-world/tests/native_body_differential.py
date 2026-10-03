"""Actual original PhysicalObject/Stop/pin/unpin and Box2D world instructions
versus the native adapter linked to the genuine Box2D 2.0.1 release. No shape,
proxy, mass, transform, DropPath, policy getter, solver or Step is intercepted.
Allocator/libc/imported IEEE arithmetic and trig are the desktop CPU services.
The public snapshot contains no inferred native private force/sleep-time fields.
"""
import argparse,hashlib,importlib.util,json,random,struct,time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
spec=importlib.util.spec_from_file_location('genuine_backend_cpu',REPO/'port/physics-backend/tests/differential.py');backend=importlib.util.module_from_spec(spec);spec.loader.exec_module(backend)
Cpu=backend.Cpu;float_bits=backend.float_bits
def words(*v):return struct.pack('<'+'I'*len(v),*v)

def main():
 p=argparse.ArgumentParser()
 for n in ('engine','library','report','reference-output'):p.add_argument('--'+n,type=Path,required=True)
 p.add_argument('--cases',type=int,default=48);a=p.parse_args();start=time.monotonic()
 manifests=[json.loads((ROOT/'reference'/n/'original-functions.json').read_text()) for n in ('physical-controls','physical-lifecycle','navigation-path','navigation-controller')]+[json.loads((REPO/'port/physics-backend/reference/original-functions.json').read_text())]
 manifest={'original_sha256':manifests[0]['original_sha256'],'functions':list({r['elf_address']:r for m in manifests for r in m['functions']}.values())};assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});world=old.data+0x1000;bounds=old.data+0x20000;gravity=bounds+16;bodydef=bounds+32;shape=bounds+128;obj=bounds+256;game=old.data+0x30000;vt=game+0x1000;zero=vt+0x1000;result=zero+64
 ni=new.data+0x1000;na=new.data+0x2000;no=new.data+0x3000;nq=no+128
 def word(at):return struct.unpack('<I',old.uc.mem_read(at,4))[0]
 old.pointer(vt+0x64,0x3a2e44);got=(0x393910+word(0x3939e4))&0xffffffff;old.pointer(got+word(0x3939ec),zero);old.uc.mem_write(zero,bytes(12))
 records=[];counts=[0]*8;rng=random.Random(20261023);steps=0
 for scene in range(a.cases):
  flags=(0x40 if scene%2 else 0)|(8 if scene%3==0 else 0)|(0x20 if scene%4==0 else 0)|(0x100 if scene%5==0 else 0)
  config=struct.pack('<8f',(.2,.36,.7)[scene%3],(.0,.12,-.15)[scene%3],(.0,-.08,.25)[scene%3],(.5,1.,2.)[scene%3],rng.uniform(-3,3),rng.uniform(-3,3),(0.,.37,-.7)[scene%3],float(flags))
  radius,cx,cy,density,x,y,angle,unused=struct.unpack('<8f',config);old.heap=old.data+0x400000;new.heap=new.data+0x400000
  old.uc.mem_write(world,bytes(0x19278));old.uc.mem_write(bounds,struct.pack('<4f',-100.,-100.,100.,100.));old.uc.mem_write(gravity,bytes(8));old.invoke(0x7e80b8,[world,bounds,gravity,1],budget=2000000)
  old.uc.mem_write(bodydef,bytes(16)+struct.pack('<I5f4B',0,x,y,angle,0.,0.,1,bool(flags&8),bool(flags&0x40),bool(flags&0x20)));body=old.invoke(0x7e7ef8,[world,bodydef]);assert body
  old.uc.mem_write(shape,bytes(100));old.uc.mem_write(shape+4,struct.pack('<II3fB1xHHH',0,0,.2,0.,density,0,1,0xffff,0));old.uc.mem_write(shape+32,struct.pack('<3f',cx,cy,radius));old.invoke(0x7e1dc8,[body,shape]);old.invoke(0x7e1818,[body])
  old.uc.mem_write(obj,bytes(48));old.pointer(obj+0x14,body);old.uc.mem_write(obj+12,config[:4])
  if flags&0x100:old.invoke(0x46eb20,[obj])
  old.uc.mem_write(game,bytes(0x600));old.pointer(game,vt);old.pointer(game+0x2dc,obj);old.uc.mem_write(game+0x520,words(2));header=game+0x1c8+0x38;old.pointer(header,header);old.pointer(header+4,header)
  new.uc.mem_write(ni,config);native=new.invoke('dh2_native_body_audit_create',[ni],budget=2000000);assert native
  def snapshot():
   f=struct.unpack('<H',old.uc.mem_read(body,2))[0];typ=struct.unpack('<H',old.uc.mem_read(body+2,2))[0]
   return bytes(old.uc.mem_read(body+4,8))+bytes(old.uc.mem_read(body+0x38,4))+config[:4]+bytes(old.uc.mem_read(body+0x40,12))+bytes(old.uc.mem_read(body+0x74,4))+bytes(old.uc.mem_read(body+0x7c,4))+bytes(old.uc.mem_read(body+0x1c,8))+bytes(old.uc.mem_read(body+0x2c,8))+words(bool(f&8),bool(f&2),typ==1,bool(f&0x20),old.uc.mem_read(obj+0x27,1)[0],0)
  def compare(op,v):
   nonlocal steps
   values=struct.pack('<4f',*v);new.uc.mem_write(na,values);raw=struct.unpack('<4I',values);counts[op]+=1
   if op==0:old.invoke(0x46e918,[obj,*raw[:2]]);assert new.invoke('dh2_native_body_set_linear',[native,na])==0
   elif op==1:old.invoke(0x46e864,[obj,*raw]);assert new.invoke('dh2_native_body_add_linear',[native,na])==0
   elif op==2:old.invoke(0x46e978,[obj,raw[0]]);assert new.invoke('dh2_native_body_set_angular',[native,na])==0
   elif op==3:old.invoke(0x46ea80,[obj,*raw[:2]],budget=2000000);assert new.invoke('dh2_native_body_set_position',[native,na],budget=2000000)>=0
   elif op==4:old.uc.mem_write(game+0x160,values[:8]+bytes(4));old.invoke(0x3938f8,[game],budget=2000000);assert new.invoke('dh2_native_body_stop',[native,na],budget=2000000)==0
   elif op==5:old.invoke(0x46eb20,[obj],budget=2000000);assert new.invoke('dh2_native_body_pin',[native],budget=2000000)==0
   elif op==6:old.invoke(0x46eae0,[obj],budget=2000000);assert new.invoke('dh2_native_body_unpin',[native],budget=2000000)==0
   else:old.invoke(0x7e8b1c,[world,raw[0],10],budget=10000000);new.invoke('dh2_native_body_audit_step',[native,na],budget=10000000);steps+=1
   expected=snapshot();assert new.invoke('dh2_native_body_observe',[no,native])==0;actual=bytes(new.uc.mem_read(no,76))
   if expected!=actual:raise AssertionError((scene,op,'body public snapshot',[(i,hex(e),hex(n)) for i,(e,n) in enumerate(zip(struct.unpack('<19I',expected),struct.unpack('<19I',actual))) if e!=n]))
   old.invoke(0x46e818,[result,obj]);old.uc.mem_write(result+8,words(old.invoke(0x46e858,[obj]),old.invoke(0x46e750,[obj])));assert new.invoke('dh2_native_body_query',[nq,native])==0;query=bytes(old.uc.mem_read(result,16));assert query==bytes(new.uc.mem_read(nq,16)),(scene,op,'game query')
   records.append(config+words(op)+values+expected+query)
  sequence=[(0,(0.,-0.,0.,0.)),(2,(0.,0.,0.,0.)),(5,(0.,0.,0.,0.)),(5,(0.,0.,0.,0.)),(0,(1.,-.5,0.,0.)),(7,(1/60,0.,0.,0.)),(6,(0.,0.,0.,0.)),(6,(0.,0.,0.,0.)),(7,(1/60,0.,0.,0.)),(1,(2.,-4.,2.,-1.)),(7,(.01,0.,0.,0.)),(2,(.5,0.,0.,0.)),(7,(.01,0.,0.,0.)),(3,(200.,-100.,0.,0.)),(7,(0.,0.,0.,0.)),(4,(200.,-100.,0.,0.)),(7,(1/60,0.,0.,0.))]
  for j in range(6):sequence.extend([(0,(rng.uniform(-5,5),rng.uniform(-5,5),0.,0.)),(7,(.01,0.,0.,0.)),(1,(rng.uniform(-2,2),rng.uniform(-2,2),rng.uniform(-1,2),rng.uniform(-1,2)))])
  sequence.extend([(3,(20000.,0.,0.,0.)),(4,(20000.,0.,0.,0.))])
  for op,v in sequence:compare(op,v)
  old.invoke(0x7e7f7c,[world],budget=2000000);new.invoke('dh2_native_body_audit_destroy',[native],budget=2000000)
  print(f'Native body original scene {scene+1}/{a.cases}',flush=True)
 gold=words(0x31424e44,len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(gold)
 coverage=[]
 for r in manifest['functions']:
  at=int(r['elf_address'],16);seen=sum(at<=s<at+r['size'] for s in old.seen)
  if seen:coverage.append({'symbol':r['original_symbol'],'address':r['elf_address'],'instruction_addresses_seen':seen})
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(gold).hexdigest(),'scenes':a.cases,'comparisons':len(records),'world_steps':steps,'operations':dict(zip(('set_linear','add_linear','set_angular','set_position','stop','pin','unpin','step'),counts)),'mismatches':0,'public_state_words_compared':19,'game_query_words_compared':4,'finite_words_exact':True,'genuine_shape_broadphase_and_mass':True,'genuine_original_and_native_step':True,'original_empty_drop_path_and_policy_getter_execute':True,'original_import_calls':old.import_calls,'native_import_calls':new.import_calls,'coverage':coverage,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-start,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='coverage'}))
if __name__=='__main__':main()
