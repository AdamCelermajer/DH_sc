"""Original Character policy/property getters, actual ANIM_SetSpeed product,
and exact CSMove OnFocus flag/type write prefix versus native ARM64 producers.
Visual animator SetSpeed is an argument observer; full FSM/animation selection,
focus logging/UpdateType/unpin and property-sheet producers are outside this API.
"""
import argparse,hashlib,importlib.util,json,random,struct,time
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R5
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
spec=importlib.util.spec_from_file_location('genuine_backend_cpu',REPO/'port/physics-backend/tests/differential.py');b=importlib.util.module_from_spec(spec);spec.loader.exec_module(b);Base=b.Cpu
class Cpu(Base):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='speed_observer':self.speed_argument=self.reg(1);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
def words(*v):return struct.pack('<'+'I'*len(v),*v)
def main():
 p=argparse.ArgumentParser()
 for n in ('engine','library','report','reference-output'):p.add_argument('--'+n,type=Path,required=True)
 a=p.parse_args();started=time.monotonic();m=json.loads((ROOT/'reference/move-state/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==m['original_sha256'];old=Cpu(a.engine,False,m);new=Cpu(a.library,True,{'functions':[]})
 char=old.data+0x1000;anim=old.data+0x4000;visual=old.data+0x5000;ctl=old.data+0x6000;vt=old.data+0x7000;sheet=new.data+0x1000;args=new.data+0x2000;policy=args+128;speed=policy+64;rotation=speed+64;flags=args+16;typ=flags+4
 old.uc.mem_write(char,bytes(0x2400));old.pointer(anim+4,char);old.pointer(char+0x2d8,visual);old.pointer(visual+0x38,ctl);old.pointer(ctl,vt);old.pointer(vt+0x28,old.callback+128);old.imports[old.callback+128]='speed_observer';new.uc.mem_write(sheet,bytes(896));rows=[];rng=random.Random(20261024)
 def prefix_hook(uc,at,size,unused):
  if at==0x3c3c7c:uc.reg_write(old.pc,old.stop)
 old.uc.hook_add(UC_HOOK_CODE,prefix_hook,begin=0x3c3c7c,end=0x3c3c7c)
 edge=(-0x80000000,-25601,-25600,-25599,-1,0,1,256,25600,0x7fffffff)
 for i in range(4352):
  f=i if i<256 else rng.getrandbits(32);w=edge[i%len(edge)] if i<256 else rng.randrange(-0x80000000,0x80000000);r=edge[(i+3)%len(edge)] if i<256 else rng.randrange(-0x80000000,0x80000000);factor=(.125,.5,1.,1.3,2.,100.)[i%6];bits=b.float_bits(factor)
  old.uc.mem_write(char+0x520,words(f));old.uc.mem_write(char+0x560+0xb50,struct.pack('<2i',w,r));new.uc.mem_write(flags,words(f));new.uc.mem_write(sheet+46*4,struct.pack('<2i',w,r));new.uc.mem_write(args,words(bits))
  expected_policy=words(*[old.invoke(at,[char]) for at in (0x3a2e38,0x3a2e44,0x3a2e50,0x3a2e5c,0x3a2e68,0x3a2e2c,0x3a2e78)]);assert new.invoke('dh2_move_policy',[policy,flags])==0;assert expected_policy==bytes(new.uc.mem_read(policy,28)),(i,'policy')
  walk=old.invoke(0x3de6c4,[char+0x560]);rot=old.invoke(0x3de708,[char+0x560]);old.uc.mem_write(anim+0x34,words(bits));old.invoke(0x3c93fc,[anim,walk]);assert old.uc.mem_read(anim+0x40,4)==words(walk);expected_speed=words(walk,rot,old.speed_argument);assert new.invoke('dh2_move_speed',[speed,sheet,args])==0;assert expected_speed==bytes(new.uc.mem_read(speed,12)),(i,'speed')
  expected_rotation=words(old.invoke(0x3a372c,[char]));assert new.invoke('dh2_move_rotation_speed',[rotation,flags,sheet])==0;assert expected_rotation==bytes(new.uc.mem_read(rotation,4)),(i,'rotation')
  old.uc.mem_write(char+0x53c,words(0xabcdef12));old.uc.reg_write(UC_ARM_REG_R5,char);old.invoke(0x3c3c60,[]);expected_focus=bytes(old.uc.mem_read(char+0x520,4))+bytes(old.uc.mem_read(char+0x53c,4));new.uc.mem_write(typ,words(0xabcdef12));assert new.invoke('dh2_move_focus_begin',[flags,typ])==0;assert expected_focus==bytes(new.uc.mem_read(flags,8)),(i,'focus prefix')
  rows.append(words(f)+struct.pack('<2i',w,r)+words(bits)+expected_policy+expected_speed+expected_rotation+expected_focus)
 rejects=0;new.uc.mem_write(policy,b'\x5a'*28);new.uc.mem_write(speed,b'\x5a'*12);new.uc.mem_write(rotation,b'\x5a'*4)
 for name,argv in [('dh2_move_policy',[policy,0]),('dh2_move_policy',[flags,flags]),('dh2_move_speed',[speed,0,args]),('dh2_move_speed',[sheet,sheet,args]),('dh2_move_rotation_speed',[rotation,0,sheet]),('dh2_move_focus_begin',[flags,flags])]:
  before=[bytes(new.uc.mem_read(at,n)) for at,n in ((sheet,896),(args,32),(policy,28),(speed,12),(rotation,4))];assert new.invoke(name,argv)==1;assert before==[bytes(new.uc.mem_read(at,n)) for at,n in ((sheet,896),(args,32),(policy,28),(speed,12),(rotation,4))];rejects+=1
 for value in (0.,-1.,101.,float('inf'),float('nan')):
  new.uc.mem_write(args,words(b.float_bits(value)));before=bytes(new.uc.mem_read(speed,12));assert new.invoke('dh2_move_speed',[speed,sheet,args])==1 and before==bytes(new.uc.mem_read(speed,12));rejects+=1
 ref=words(0x31564d44,len(rows))+b''.join(rows);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(ref);report={'original_sha256':m['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'reference_sha256':hashlib.sha256(ref).hexdigest(),'comparisons':len(rows),'policy_getter_calls':len(rows)*7,'walk_property_index':46,'rotation_property_index':47,'exact_original_animation_speed_argument':True,'original_focus_prefix_executes':True,'atomic_rejection_checks':rejects,'mismatches':0,'imports':old.import_calls,'scope':__doc__,'elapsed_seconds':round(time.monotonic()-started,2)};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
