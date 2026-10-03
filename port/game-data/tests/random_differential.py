"""Execute original Random::GetRandom and compiled ARM64 reconstruction.

Only imported unsigned division/remainder and caller-owned global state storage
are modeled. Original ARM32 code and Unicorn remain validation inputs.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../engine-resources/tests'))
from cpu import Cpu as Parent
class Cpu(Parent):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_uidivmod':
   a,b=self.reg(0),self.reg(1);assert b>0
   self.put(0,a//b);self.put(1,a%b)
   self.import_calls['__aeabi_uidivmod']=self.import_calls.get('__aeabi_uidivmod',0)+1
   uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'reference/selection/original-functions.json').read_text());assert hashlib.sha256(a.engine.read_bytes()).hexdigest()==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);new=Cpu(a.library,True,{'functions':[]})
 def word(address):return struct.unpack('<I',old.uc.mem_read(address,4))[0]
 # PIC instruction at 0x3ca714 adds its literal to the architectural PC.
 got_base=0x3ca714+8+word(0x3ca790)
 seed_slot=got_base+word(0x3ca794);calls_slot=got_base+word(0x3ca798)
 old_seed,old_calls=old.data+0x1000,old.data+0x2000
 old.pointer(seed_slot,old_seed);old.pointer(calls_slot,old_calls)
 new_seed,new_calls=new.data+0x1000,new.data+0x2000
 rng=random.Random(20261002);cases=0
 for i in range(2000):
  seed=(0,1,14348906,14348907,0x7fffffff,0x80000000,0xffffffff)[i%7] if i<700 else rng.getrandbits(32)
  calls=(0,1,0xfffffffe,0xffffffff)[i%4]
  count=(0,1,2,3,6,10000)[i%6]
  for cpu,sp,cp in ((old,old_seed,old_calls),(new,new_seed,new_calls)):
   cpu.uc.mem_write(sp,struct.pack('<I',seed));cpu.uc.mem_write(cp,struct.pack('<I',calls))
  for _ in range(5):
   expected=old.invoke(0x3ca708,[count]);actual=new.invoke('dh2_animation_random',[new_seed,new_calls,count])
   assert actual==expected and bytes(old.uc.mem_read(old_seed,4))==bytes(new.uc.mem_read(new_seed,4)) and bytes(old.uc.mem_read(old_calls,4))==bytes(new.uc.mem_read(new_calls,4)),(i,count,seed,expected,actual)
   cases+=1
 report={'original_sha256':manifest['original_sha256'],'arm64_library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'original_address':'0x3ca708','bit_exact_random_index_and_state_cases':cases,'zero_count_cases':1670,'scope':'original animation random helper only; imported unsigned division/remainder modeled; runtime RNG seed and full animation callbacks not reconstructed'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
