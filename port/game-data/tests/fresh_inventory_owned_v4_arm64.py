"""O2 ARM64 executes the authoritative owned graph against original snapshots.
libc/STL allocation boundaries are modeled; metadata and logical storage bodies
execute natively. Required item and Character effects remain explicit fixtures.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from fresh_inventory_v2_arm64 import Owned,W

class InventoryCpu(Owned):
 def external(self,uc,a,z,u):
  if self.imports.get(a)=="posix_memalign":
   dst,alignment,n=[self.reg(i) for i in range(3)];assert alignment>=8 and alignment&(alignment-1)==0 and n<0x1000000
   self.heap=(self.heap+alignment-1)&~(alignment-1);p=self.heap;self.heap+=(max(n,1)+15)&~15;assert self.heap<self.data+0x2000000;uc.mem_write(p,bytes(max(n,1)));self.pointer(dst,p);self.allocations+=1;self.import_calls["posix_memalign"]=self.import_calls.get("posix_memalign",0)+1;self.put(0,0);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,a,z,u)

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);ap.add_argument('--output',type=Path,default=ROOT/'port/game-data/reports/player-inventory-owned-v4-arm64-differential.json');a=ap.parse_args();cpu=InventoryCpu(a.library);goldpath=ROOT/'port/game-data/reference/player-inventory-owned-v4/fixtures.bin';gold=goldpath.read_bytes();assert gold[:4]==b'IOV4';count=struct.unpack_from('<I',gold,4)[0];at=8;steps=0;services=0
 blobs=[(ROOT/'.local-inputs/items-discovery'/n).read_bytes() for n in ('loot_table_pyarray.bin','loot_table_pyarraynames.bin','loot_table_pystructnames.bin')];pointers=[];where=cpu.data+0x10000
 for blob in blobs:cpu.uc.mem_write(where,blob);pointers.append(where);where+=(len(blob)+255)&~255
 f=cpu.data+0x1000;commands=cpu.data+0x80000;output=cpu.data+0x100000;written=cpu.data+0x5000
 for k in range(count):
  cap,flags,selected,synthetic,n=struct.unpack_from('<5I',gold,at);at+=20;operations=bytearray();expected=bytearray()
  for j in range(n):
   op=gold[at:at+16];at+=16;operations+=op;result,size=struct.unpack_from('<2I',gold,at);at+=8;state=gold[at:at+size];at+=size;nr=struct.unpack_from('<I',gold,at)[0];at+=4;req=gold[at:at+nr*28];at+=nr*28;expected+=W(result,size)+state+W(nr)+req;services+=nr
  cpu.uc.mem_write(f,struct.pack('<3Q8IQ2I',*pointers,*map(len,blobs),cap,flags,selected,synthetic,0,commands,n,0));cpu.uc.mem_write(commands,bytes(operations));cpu.uc.mem_write(written,W(0));cpu.heap=cpu.data+0x200000;status=cpu.invoke('dh2_inventory_owned_fixture_v4',[f,output,0x100000,written],budget=300000000);assert status==0,(k,status,hex(cpu.uc.reg_read(cpu.pc)));size=struct.unpack('<I',cpu.uc.mem_read(written,4))[0];got=bytes(cpu.uc.mem_read(output,size));assert got==expected,(k,next((i for i in range(min(len(got),len(expected))) if got[i]!=expected[i]),None),len(got),len(expected));steps+=n
 assert at==len(gold);sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();sources=['fresh_inventory_owned_v4.hpp','fresh_inventory_owned_v4.cpp','loot_tables_v2.hpp','loot_tables_v2.cpp','item_inventory_v1.hpp','item_inventory_v1.cpp','player_savegame_v1.hpp','player_savegame_v1.cpp','skill_tables.hpp','skill_tables.cpp','items.hpp','items.cpp','tests/fresh_inventory_owned_v4.cpp','tests/fresh_inventory_owned_v4_arm64_fixture.cpp'];report={'validation':'PASS','cases':count,'comparisons':steps,'source_effect_requests':services,'actual_metadata_cases':56,'synthetic_stackable_equipment_cases':28,'synchronous_mutation_cases':4,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'gold_sha256':sha(goldpath),'library_sha256':sha(a.library),'source_sha256':{'port/game-data/'+v:sha(ROOT/'port/game-data'/v) for v in sources},'script_sha256':sha(__file__),'storage_allocations':cpu.allocations,'storage_frees':cpu.frees,'import_calls':cpu.import_calls,'mismatches':0,'scope':__doc__};a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
