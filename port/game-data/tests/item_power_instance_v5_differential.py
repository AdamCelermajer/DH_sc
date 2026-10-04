"""Full actual Power-instance append, value projection and stable insertion sort.
Actual cached definition/name data execute on original and O2 native owners;
localized raw strings/parseEx are explicit fixture services, not full text proof.
The native full Power state is tied to its real item ID vector, not inventory
mirror storage. Powered V4 creation valuation remains a separate boundary.
"""
import sys,struct,json,hashlib,argparse,random
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/game-data/tests'))
from item_presentation_v5_original import OriginalPresentation,W
from item_presentation_v5_differential import NativePresentation,block,sha
class OriginalPower(OriginalPresentation):
 def __init__(self):
  super().__init__();self.blob=(R/'.local-inputs/player-item-effects-v5/power-cache/item_powers_pyarray.bin').read_bytes();self.cursor=0
  for address in (0x4bacc8,0x4bab7c):self.invoke(address,[self.stream],budget=30000000)
  self.blob=(R/'.local-inputs/player-item-effects-v5/power-cache/item_powers_pyarraynames.bin').read_bytes();self.cursor=4;count=struct.unpack_from('<I',self.blob)[0]
  for _ in range(count):n=struct.unpack_from('<I',self.blob,self.cursor)[0];self.cursor+=4+n
  self.invoke(0x4b6630,[self.stream]);assert self.cursor==len(self.blob)
 def external(self,uc,a,z,u):
  if self.imports.get(a)=='strcpy':p=self.reg(0);uc.mem_write(p,self.cstring(self.reg(1))+b'\0');self.returned(p)
  else:super().external(uc,a,z,u)
 def present(self,uc,a,z,u):
  if self.loadingsource and a==0x508edc:
   i=self.reg(1);signed=struct.unpack('<i',W(i))[0];self.returned(self.txt(b'OID'+str(signed).encode()));return
  if self.loadingsource and a==0x509aec:
   obj=self.reg(1);text=self.cstring(self.reg(2));args=self.reg(3);p,n=self.word(args+4),self.word(args+8);out=text
   for at in range(p,n,12):bits,integer,_=struct.unpack('<3I',uc.mem_read(at,12));out+=b':'+str(bits).encode()+b'/'+str(struct.unpack('<i',W(integer))[0]).encode()
   p=self.txt(out);saved=uc.context_save();prior=self.stack;self.stack-=0x8000;self.loadingsource=False;self.invoke(0x3109e0,[obj,p,p+len(out)]);self.stack=prior;uc.context_restore(saved);self.loadingsource=True;self.returned();return
  super().present(uc,a,z,u)
 def run_power(self,commands):
  self.textat=self.textheap;self.localized={};self.uc.mem_write(self.item+0x5c,W(0,0,0));self.loadingsource=True;result=b''
  try:
   for id,mode in commands:
    self.invoke(0x3fbc60,[self.item,id,mode],budget=20000000);begin,end=self.word(self.item+0x5c),self.word(self.item+0x60);result+=W((end-begin)//32)
    for at in range(begin,end,32):result+=bytes(self.uc.mem_read(at,8))+block(self.stringvalue(at+8))
  finally:self.loadingsource=False
  return result
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();old=OriginalPower();new=NativePresentation(a.library);blobs=[(R/'.local-inputs/player-item-effects-v5/power-cache'/x).read_bytes()for x in ('item_powers_pyarray.bin','item_powers_pyarraynames.bin','item_powers_pystructnames.bin')];cache=b''.join(block(x)for x in blobs);rng=random.Random(0x50575635);commands=[[(i,0)]for i in range(937)]+[[(i,mode)]for i in range(0,935,11)for mode in (1,2)]+[[(rng.randrange(935),rng.randrange(3))for _ in range(24)]for k in range(12)];gold=[];calls=0
 for i,ops in enumerate(commands):
  expected=old.run_power(ops);inp=W(len(ops))+b''.join(W(*op)for op in ops);new.heap=new.data+0x200000;new.uc.mem_write(new.data+0x10000,cache+inp);n=new.invoke('dh2_item_power_instance_fixture_v5',[new.data+0x10000,new.data+0x100000],budget=60000000);actual=bytes(new.uc.mem_read(new.data+0x100000,n))if n!=0xffffffff else None;assert actual==expected,(i,ops,actual,expected);gold.append(block(inp)+block(expected));calls+=len(ops)
 ref=R/'port/game-data/reference/player-item-effects-v5/power-instance-fixtures.bin';ref.write_bytes(b'PIV5'+W(len(gold))+b''.join(gold));sources=['port/game-data/item_presentation_v5.hpp','port/game-data/item_presentation_v5.cpp','port/game-data/item_power_tables_v5.hpp','port/game-data/item_power_tables_v5.cpp','port/game-data/tests/item_power_instance_v5_fixture.cpp',Path(__file__).relative_to(R).as_posix(),'port/game-data/tests/item_presentation_v5_original.py'];report=dict(validation='PASS',comparisons=len(gold),power_append_calls=calls,actual_power_definitions=937,sorted_chain_cases=12,mismatches=0,original_sha256=sha(R/'.local-inputs/libDungeonHunter2.so'),library_sha256=sha(a.library),gold_sha256=sha(ref),source_sha256={x:sha(R/x)for x in sources},scope=__doc__);dest=R/'port/game-data/reports/item-power-instance-v5-arm64-differential.json';dest.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()


