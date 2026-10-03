"""Original pin/unpin, SetMass and SetMassFromShapes versus native ARM64.

All original mass, center/sweep, fixed-rotation, type and pin/sleep instructions
execute. Shape ComputeMass/UpdateSweepRadius/RefilterProxy are supplied services,
compared in exact order with their current pin/type and argument fields. No
modern mass minimum or velocity correction is substituted. This verifies the
mass/lifecycle bridge, not shape mass geometry, collision or world stepping.
"""
import argparse,hashlib,json,math,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from navigation_differential import ROOT,Cpu as BaseCpu,equal
from navigation_search_differential import words,word
from aggro_differential import float_bits

class Cpu(BaseCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='lifecycle_callback':return self.lifecycle_callback(uc,address,size,unused)
  return super().external(uc,address,size,unused)

def main():
 p=argparse.ArgumentParser()
 for name in ('engine','library','report','reference-output'):p.add_argument('--'+name,type=Path,required=True)
 p.add_argument('--cases',type=int,default=4096);a=p.parse_args();started=time.monotonic();manifest=json.loads((ROOT/'reference/physical-lifecycle/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]});ob=old.data+0x1000;obj=old.data+0x1200;ow=old.data+0x2000;osh=old.data+0x22000;vt=old.data+0x25000;bp=old.data+0x26000;om=old.data+0x27000;zero=old.data+0x28000
 ns=new.data+0x1000;nb=new.data+0x1100;nv=new.data+0x1200;nw=new.data+0x1300;nc=new.data+0x1400;nm=new.data+0x1500;nsh=new.data+0x2000;context=0xabcdef0123456789;nbp=0xb23456789abcdef0;shape_masses=[];events=[[],[]];rng=random.Random(20261025);records=[];totals={'set_mass':0,'mass_from_shapes':0,'pin':0,'unpin':0,'computed':0,'sweep_radius':0,'refilter_proxy':0,'locked':0,'type_changes':0};snapshots=[]
 old.pointer(obj+0x14,ob);old.pointer(vt+0x10,old.callback+32);old.pointer(vt+0x1c,old.callback+48)
 # Supply the original global b2Vec2 zero constant, not an arithmetic answer.
 got=(0x7e1844+word(old,0x7e1b20))&0xffffffff;old.pointer(got+word(old,0x7e1b24),zero);old.uc.mem_write(zero,bytes(8))
 def state():return words(struct.unpack('<H',old.uc.mem_read(ob,2))[0])+bytes(old.uc.mem_read(ob+4,8))+bytes(old.uc.mem_read(ob+0x38,4))+bytes(old.uc.mem_read(ob+0x40,24))+bytes(old.uc.mem_read(ob+0x8c,4))+bytes(old.uc.mem_read(obj+12,4))
 def extra():return bytes(old.uc.mem_read(ob+12,24))+bytes(old.uc.mem_read(ob+0x2c,8))+bytes(old.uc.mem_read(ob+0x24,8))+bytes(old.uc.mem_read(ob+0x34,4))+bytes(4)
 def view():return bytes(old.uc.mem_read(ob+0x74,16))+words(struct.unpack('<H',old.uc.mem_read(ob+2,2))[0],old.uc.mem_read(obj+0x27,1)[0])
 def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def old_cb(uc,address,size,unused):
  ident=(old.reg(0)-osh)//64;assert old.reg(0)==osh+ident*64;kind=1 if address==old.callback+32 else 2
  payload=shape_masses[ident] if kind==1 else bytes(uc.mem_read(old.reg(1),8))
  if kind==1:uc.mem_write(old.reg(1),payload)
  else:assert old.reg(1)==ob+0x1c
  events[0].append(words(kind,ident,old.uc.mem_read(obj+0x27,1)[0],struct.unpack('<H',old.uc.mem_read(ob+2,2))[0])+payload+bytes(24-len(payload)));ret()
 old.lifecycle_callback=old_cb
 for address in (old.callback+32,old.callback+48):old.imports[address]='lifecycle_callback'
 def old_hook(uc,address,size,unused):
  if address!=0x7e61b0:return
  ident=(old.reg(0)-osh)//64;assert old.reg(0)==osh+ident*64 and old.reg(1)==bp and old.reg(2)==ob+4
  events[0].append(words(3,ident,old.uc.mem_read(obj+0x27,1)[0],struct.unpack('<H',old.uc.mem_read(ob+2,2))[0])+bytes(uc.mem_read(old.reg(2),24)));ret()
 old.uc.hook_add(UC_HOOK_CODE,old_hook,begin=0x7e61b0,end=0x7e61b0)
 def new_cb(uc,address,size,unused):
  assert new.reg(0)==context;ident=new.reg(1)-0x100000000;kind=(address-new.callback-32)//16+1;payload=b''
  if kind==1:payload=shape_masses[ident];uc.mem_write(new.reg(2),payload)
  elif kind==2:assert new.reg(2)==nb+24;payload=bytes(uc.mem_read(new.reg(2),8))
  else:assert new.reg(2)==nbp;payload=bytes(uc.mem_read(new.reg(3),24))
  typ,pinned=struct.unpack('<2I',uc.mem_read(nv+24,8));events[1].append(words(kind,ident,pinned,typ)+payload+bytes(24-len(payload)));uc.reg_write(new.pc,uc.reg_read(new.lr))
 for address in (new.callback+32,new.callback+48,new.callback+64):new.imports[address]='lifecycle_callback'
 new.lifecycle_callback=new_cb;new.uc.mem_write(nc,struct.pack('<3Q',new.callback+32,new.callback+48,new.callback+64))
 def compare(op,locked,initial,initial_extra,initial_view,mass,masses,label=None):
  nonlocal shape_masses
  shape_masses=masses;count=len(masses);events[0].clear();events[1].clear();old.uc.mem_write(ob,bytes(0x100));old.uc.mem_write(ob,struct.pack('<H',struct.unpack_from('<I',initial)[0]));old.uc.mem_write(ob+4,initial[4:12]);old.uc.mem_write(ob+0x38,initial[12:16]);old.uc.mem_write(ob+0x40,initial[16:40]);old.uc.mem_write(ob+0x8c,initial[40:44]);old.uc.mem_write(obj+12,initial[44:48]);old.uc.mem_write(ob+12,initial_extra[:24]);old.uc.mem_write(ob+0x2c,initial_extra[24:32]);old.uc.mem_write(ob+0x24,initial_extra[32:40]);old.uc.mem_write(ob+0x34,initial_extra[40:44]);old.uc.mem_write(ob+0x74,initial_view[:16]);typ,pinned=struct.unpack_from('<2I',initial_view,16);old.uc.mem_write(ob+2,struct.pack('<H',typ));old.uc.mem_write(obj+0x27,bytes((pinned,)));old.pointer(ob+0x58,ow);old.pointer(ob+0x64,osh if count else 0);old.uc.mem_write(ow,bytes(0x19200));old.pointer(ow+0x191d8,bp);old.uc.mem_write(ow+0x191d4,bytes((locked,)));old.uc.mem_write(om,mass)
  for i in range(count):old.pointer(osh+i*64,vt);old.pointer(osh+i*64+8,osh+(i+1)*64 if i+1<count else 0);new.uc.mem_write(nsh+i*16,struct.pack('<2Q',0x100000000+i,nsh+(i+1)*16 if i+1<count else 0))
  new.uc.mem_write(ns,initial);new.uc.mem_write(nb,struct.pack('<Q',ns)+initial_extra);new.uc.mem_write(nv,struct.pack('<Q',nb)+initial_view);new.uc.mem_write(nw,struct.pack('<3QB7x',context,nbp,nsh if count else 0,locked));new.uc.mem_write(nm,mass)
  old.invoke((0x7e1b28,0x7e1818,0x46eb20,0x46eae0)[op],[ob,om] if op==0 else [ob] if op==1 else [obj]);name=('dh2_physical_set_mass','dh2_physical_mass_from_shapes','dh2_physical_pin','dh2_physical_unpin')[op];assert new.invoke(name,[nv,nm,nw,nc] if op==0 else [nv,nw,nc])==0
  after=state();after_extra=extra();after_view=view()
  for field,left,right in [('state',after,bytes(new.uc.mem_read(ns,48))),('extra',after_extra,bytes(new.uc.mem_read(nb+8,48))),('mass/type/pin',after_view,bytes(new.uc.mem_read(nv+8,24)))]:assert equal(left,right),(len(records),label,op,field,left.hex(),right.hex())
  assert len(events[0])==len(events[1]) and all(equal(x,y) for x,y in zip(*events)),(len(records),'service order',label,[x.hex() for x in events[0]],[x.hex() for x in events[1]])
  # The old mass/lifecycle kernel never changes velocities, forces or torque.
  assert after[16:40]==initial[16:40],'mass changed velocity/force'
  totals[('set_mass','mass_from_shapes','pin','unpin')[op]]+=1;totals['locked']+=locked;totals['type_changes']+=after_view[16:20]!=initial_view[16:20]
  for event in events[0]:totals[('computed','sweep_radius','refilter_proxy')[struct.unpack_from('<I',event)[0]-1]]+=1
  records.append(words(op,locked,count,len(events[0]))+initial+initial_extra+initial_view+mass+b''.join(masses)+after+after_extra+after_view+b''.join(events[0]))
  if label:snapshots.append({'case':label,'op':op,'mass_view_words':list(struct.unpack('<6I',after_view)),'body_flags':struct.unpack_from('<I',after)[0],'events':[[*struct.unpack_from('<4I',e)] for e in events[0]]})
  return after,after_extra,after_view
 def initial(flags=0x58):return words(flags)+struct.pack('<11f',10.,-20.,.37,3.,-4.,5.,6.,-7.,8.,9.,10.)
 ex=struct.pack('<11fI',1.,0.,-0.,1.,2.,-3.,4.,-5.,6.,-7.,.37,0);mv=struct.pack('<4f2I',13.,1./13.,17.,1./17.,1,0);mass=struct.pack('<4f',19.,2.,-3.,23.);ordinary=[struct.pack('<4f',2.,1.,-2.,13.),struct.pack('<4f',3.,-1.,2.,19.),struct.pack('<4f',5.,2.,3.,41.)]
 for flags in (0,8,0x40,0x58,0x78,0xffff):
  for locked in (0,1):
   for typ in (0,1,0xffff):
    for pinned in (0,1):
     for op in range(4):compare(op,locked,initial(flags),ex,mv[:16]+words(typ,pinned),mass,ordinary,f'flags {flags} lock {locked} type {typ} pin {pinned} op {op}')
 for pinned in (0,1):
  current=(initial(),ex,mv[:20]+words(pinned))
  for op in (2,2,3,3,2,3):current=compare(op,0,*current,mass,ordinary,f'carried pin/unpin op {op}')
 boundary=(0,0x80000000,1,0x80000001,0x007fffff,0x00800000,0x3f800000,0xbf800000,0x7f7fffff,0xff7fffff,0x7f800000,0xff800000,0x7fc01234,0xffc01234,0x7f801234)
 for x in boundary:
  for y in boundary:
   for op in (0,1):compare(op,0,initial(8),ex,mv,words(x,y,0,x),[words(x,y,0,x),words(y,0,x,y)])
 for n in range(a.cases):
  op=n%4;count=rng.randrange(5);flags=rng.choice((0,8,0x40,0x58,0x78));s=words(flags,*[rng.getrandbits(32) for _ in range(11)]);e=words(*[rng.getrandbits(32) for _ in range(11)],0);v=words(*[rng.getrandbits(32) for _ in range(4)],rng.choice((0,1,0xffff)),rng.randrange(2));m=words(*[rng.choice(boundary) if n%2 else rng.getrandbits(32) for _ in range(4)]);ms=[words(*[rng.choice(boundary) if n%2 else rng.getrandbits(32) for _ in range(4)]) for _ in range(count)];compare(op,int(n%17==0),s,e,v,m,ms)
 # Atomic native contracts; no original arbitrary-pointer validation claim.
 new.uc.mem_write(nv+28,words(2));before=[bytes(new.uc.mem_read(at,size)) for at,size in ((ns,48),(nb,56),(nv,32))];event_count=len(events[1]);rejects=0
 for name,args in [('dh2_physical_set_mass',[nv,nm,nw,nc]),('dh2_physical_mass_from_shapes',[nv,nw,nc]),('dh2_physical_pin',[nv,nw,nc]),('dh2_physical_unpin',[nv,nw,nc])]:
  assert new.invoke(name,args)==1 and before==[bytes(new.uc.mem_read(at,size)) for at,size in ((ns,48),(nb,56),(nv,32))] and len(events[1])==event_count;rejects+=1
 reference=words(0x31434c50,len(records))+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(reference);report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(reference).hexdigest(),'comparisons':len(records),'synthetic_cases':a.cases,'mismatches':0,'totals':totals,'body_type_0_static_1_dynamic':True,'velocity_force_words_unchanged':True,'pin_type_callback_order_compared':True,'atomic_rejection_checks':rejects,'original_import_calls':old.import_calls,'behavior_snapshots':snapshots,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v for k,v in report.items() if k!='behavior_snapshots'}))
if __name__=='__main__':main()
