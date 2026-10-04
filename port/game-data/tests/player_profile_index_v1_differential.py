"""Actual original campaign-index fixtures versus optimized ARM64 index kernel.
Section-storage callback is a borrowed 64-bit caller service; original STL
indexing was executed to produce these fixtures. Invalid seek is a native
safety guard rather than original backup/file-open parity.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(Path(__file__).resolve().parent))
from player_savegame_v1_differential import Native
class Index(Native):
 def external(self,uc,a,z,u):
  if a==self.callback+64:
   tag,offset,size=struct.unpack('<4sII',uc.mem_read(self.reg(1),12));self.sections[tag.split(b'\0',1)[0]]=(offset,size);self.returned(1)
  else:super().external(uc,a,z,u)
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();c=Index(a.library)
 gold=ROOT/'port/game-data/reference/player-profile-index-v1/fixtures.bin';b=gold.read_bytes();at=4
 def word():
  nonlocal at
  n=struct.unpack_from('<I',b,at)[0];at+=4;return n
 def block():
  nonlocal at
  n=word();s=b[at:at+n];at+=n;return s
 cases=word();sections=0;data=c.data+0x1000;span=c.data+0x3000;service=c.data+0x3100;c.uc.mem_write(service,struct.pack('<QQ',0x123456789abcdef0,c.callback+64))
 for _ in range(cases):
  blob=block();expected=block();c.sections={};c.uc.mem_write(data,blob);c.uc.mem_write(span,struct.pack('<Q4I',data,len(blob),0,0,0));assert c.invoke('dh2_player_profile_v1_index',[span,service])==0
  count=struct.unpack_from('<I',expected)[0];ep=4;want={}
  for _ in range(count):
   n=struct.unpack_from('<I',expected,ep)[0];ep+=4;name=expected[ep:ep+n];ep+=n;want[name]=struct.unpack_from('<II',expected,ep);ep+=8
  assert ep==len(expected) and want==c.sections;sections+=count
 assert at==len(b)
 source=[ROOT/'port/game-data/player_profile_index_v1.hpp',ROOT/'port/game-data/player_profile_index_v1.cpp']
 report={'validation':'PASS','original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(gold.read_bytes()).hexdigest(),'source_sha256':{x.relative_to(ROOT).as_posix():hashlib.sha256(x.read_bytes()).hexdigest() for x in source},'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'comparisons':cases,'section_checks':sections,'mismatches':0,'scope':__doc__};a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
