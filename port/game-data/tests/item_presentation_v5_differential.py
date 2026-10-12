"""Original Item text caller instructions versus O2 native caller bodies.
Actual metadata/class-name fields are used; controlled localization and a
declared integer/string varargs formatter isolate caller grammar/projection.
This is not a StringManager or real localized text/GPU parity claim.
"""
import sys,json,hashlib,struct,argparse,random
from pathlib import Path
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/game-data/tests'))
from item_presentation_v5_original import OriginalPresentation,W
from fresh_inventory_owned_v4_arm64 import InventoryCpu
class NativePresentation(InventoryCpu):
 def external(self,uc,a,z,u):
  if self.imports.get(a)=='wmemchr':
   p,value,n=self.reg(0),self.reg(1),self.reg(2);data=struct.unpack('<'+'I'*n,uc.mem_read(p,4*n));self.put(0,next((p+4*j for j,x in enumerate(data)if x==value),0));uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,a,z,u)
def block(b):return W(len(b))+b
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);a=ap.parse_args();old=OriginalPresentation();new=NativePresentation(a.library);gold=[];records=[bytes(old.uc.mem_read(old.table+164*i,164)) for i in range(1322)];raw=(R/'.local-inputs/actors/character_properties_pyarray.bin').read_bytes();rows=[263,264,265,325,327,326,290,292,291];oids=[struct.unpack_from('<I',raw,4+row*896+20)[0]for row in rows]
 chars=old.allocate(len(raw)+len(raw)//224);n=struct.unpack_from('<I',raw)[0]
 for row in range(n):old.uc.mem_write(chars+row*900,W(0)+raw[4+row*896:4+(row+1)*896])
 old.pointer(0x9a645c,chars);rng=random.Random(0x50525635);names=[b'Sword',b'Sword[f]',b'Sword[fs]',b'Sword[s]',b'^d gold[f]',b'[fs]root',b'^[s]'];materials=[b'',b'iron',b'iron[a]',b'[a]iron',b'iron#female#single#both[a]',b'#first#second#third',b'iron#female',b'#']
 def compare(id,op,record,name,material,value):
  old.uc.mem_write(old.table+164*id,record);old.keys={};old.localized={};out,trace=old.run(id,[0x3fb754,0x3fb290,0x3facdc][op],value,name,material)
  w=list(struct.unpack('<41I',record));w[0]=w[2]=w[20]=0;inp=W(op,value)+W(*w)+W(*oids)+block(name)+block(material);new.heap=new.data+0x200000;new.uc.mem_write(new.data+0x1000,inp);n=new.invoke('dh2_item_presentation_fixture_v5',[new.data+0x1000,new.data+0x10000],budget=10000000);actual=bytes(new.uc.mem_read(new.data+0x10000,n)) if n!=0xffffffff else None
  assert actual==out,(len(gold),id,op,actual,out,trace);gold.append(block(inp)+block(out))
 for i,record in enumerate(records):
  for op in range(3):compare(i,op,record,names[i%len(names)],materials[i%len(materials)],rng.randrange(-2147483648,2147483648))
 for name in names:
  for material in materials:
   w=list(struct.unpack('<41I',records[664]));w[17:19]=[40000,40001];compare(664,0,W(*w),name,material,123)
 for req in range(1,10):
  w=list(struct.unpack('<41I',records[664]));w[29:35]=[9,8,7,6,5,req];compare(664,2,W(*w),b'Sword',b'iron',123)
 blob=b'PRV5'+W(len(gold))+b''.join(gold);ref=R/'port/game-data/reference/player-item-effects-v5';p=ref/'presentation-fixtures.bin';p.write_bytes(blob);sources=['port/game-data/item_presentation_v5.hpp','port/game-data/item_presentation_v5.cpp','port/game-data/tests/item_presentation_v5_fixture.cpp','port/game-data/tests/item_presentation_v5_original.py',Path(__file__).relative_to(R).as_posix()]
 report=dict(validation='PASS',comparisons=len(gold),actual_metadata_rows=1322,actual_metadata_caller_cases=3966,name_grammar_cases=56,class_requirements_cases=9,mismatches=0,original_sha256=sha(R/'.local-inputs/libDungeonHunter2.so'),library_sha256=sha(a.library),gold_sha256=sha(p),source_sha256={x:sha(R/x)for x in sources},scope=__doc__);dest=R/'port/game-data/reports/item-presentation-v5-arm64-differential.json';dest.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
