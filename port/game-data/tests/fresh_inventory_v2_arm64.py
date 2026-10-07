"""Execute the genuine C++ owned starting inventory against original gold.
Production constructors, table decoding, RNG, vectors, stable slots and item
mutation paths execute O2 ARM64. malloc/free and libc bytes are storage services;
native game effects retain the same explicit controlled providers as the gold.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from elftools.elf.elffile import ELFFile
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from navigation_differential import Cpu
W=lambda *v:struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
class Owned(Cpu):
 def __init__(self,path):
  super().__init__(path,True,{'functions':[]});self.heap=self.data+0x200000;self.allocations=0;self.frees=0
  with path.open('rb') as stream:
   elf=ELFFile(stream)
   for section in elf.iter_sections():
    if section['sh_type']!='SHT_RELA':continue
    syms=elf.get_section(section['sh_link'])
    for r in section.iter_relocations():
     if r['r_info_type'] not in (1025,257):continue
     symbol=syms.get_symbol(r['r_info_sym'])
     if symbol.name=='__stack_chk_guard':self.pointer(self.base+r['r_offset'],self.data+0x1f0000);self.pointer(self.data+0x1f0000,0xBEAF12347890)
     elif symbol['st_shndx']!='SHN_UNDEF':self.pointer(self.base+r['r_offset'],self.base+symbol['st_value']+r['r_addend'])
 def text(self,p):
  b=bytearray()
  while self.uc.mem_read(p+len(b),1)!=b'\0':b.extend(self.uc.mem_read(p+len(b),1))
  return bytes(b)
 def external(self,uc,address,size,unused):
  name=self.imports.get(address)
  if name in ('malloc','calloc'):
   n=self.reg(0)*(self.reg(1) if name=='calloc' else 1);assert n<0x1000000;p=self.heap;self.heap+=(max(n,1)+15)&~15;assert self.heap<self.data+0x2000000;uc.mem_write(p,bytes(max(n,1)));self.allocations+=1;self.put(0,p)
  elif name=='free':self.frees+=1;self.put(0,0)
  elif name=='strlen':
   try:self.put(0,len(self.text(self.reg(0))))
   except Exception:raise AssertionError(('Invalid source strlen span',hex(self.reg(0)),hex(uc.reg_read(self.lr))))
  elif name in ('strcmp','strncmp','memcmp'):
   if name=='memcmp':a,b=[bytes(uc.mem_read(self.reg(i),self.reg(2))) for i in (0,1)]
   else:a,b=self.text(self.reg(0)),self.text(self.reg(1));a,b=(a[:self.reg(2)],b[:self.reg(2)]) if name=='strncmp' else (a,b)
   self.put(0,(a>b)-(a<b))
  elif name=='memchr':
   p,n=self.reg(0),self.reg(2);b=bytes(uc.mem_read(p,n));at=b.find(bytes([self.reg(1)&255]));self.put(0,p+at if at>=0 else 0)
  else:return super().external(uc,address,size,unused)
  self.import_calls[name]=self.import_calls.get(name,0)+1;uc.reg_write(self.pc,uc.reg_read(self.lr))
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();c=Owned(a.library);goldpath=ROOT/'port/game-data/reference/player-creation-v2/fresh-fixtures.bin';gold=goldpath.read_bytes();count=struct.unpack_from('<I',gold,4)[0];at=8
 blobs=[(ROOT/'.local-inputs/items-discovery'/name).read_bytes() for name in ('loot_table_pyarray.bin','loot_table_pyarraynames.bin','loot_table_pystructnames.bin')];pointers=[];where=c.data+0x10000
 for b in blobs:c.uc.mem_write(where,b);pointers.append(where);where+=(len(b)+255)&~255
 f=c.data+0x1000;out=c.data+0x2000;used=c.data+0x4000;services=0
 for i in range(count):
  seed,calls,cap,minimal,mutation,loot,expected_seed,expected_calls,statebytes=struct.unpack_from('<9I',gold,at);at+=36;state=gold[at:at+statebytes];at+=statebytes;n=struct.unpack_from('<I',gold,at)[0];at+=4;requests=gold[at:at+n*28];at+=n*28;services+=n
  c.uc.mem_write(f,struct.pack('<3Q9I',*pointers,*map(len,blobs),seed,calls,cap,minimal,mutation,loot)+bytes(4));c.uc.mem_write(used,W(0));c.heap=c.data+0x200000;assert c.invoke('dh2_fresh_fixture_v2',[f,out,8192,used],budget=100000000)==0,(i,hex(c.uc.reg_read(c.pc)));written=struct.unpack('<I',c.uc.mem_read(used,4))[0];got=bytes(c.uc.mem_read(out,written));expected=W(expected_seed,expected_calls,statebytes)+state+W(n)+requests;assert got==expected,(i,got.hex(),expected.hex())
 assert at==len(gold);sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest();source=['port/game-data/fresh_inventory_v2.hpp','port/game-data/fresh_inventory_v2.cpp','port/game-data/loot_tables_v2.hpp','port/game-data/loot_tables_v2.cpp','port/game-data/items.hpp','port/game-data/items.cpp','port/game-data/tests/fresh_inventory_v2.cpp','port/game-data/tests/fresh_inventory_v2_arm64_fixture.cpp'];report={'validation':'PASS','comparisons':count,'ordered_requests':services,'library_sha256':sha(a.library),'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'gold_sha256':sha(goldpath),'source_sha256':{v:sha(ROOT/v) for v in source},'script_sha256':sha(__file__),'storage_allocations':c.allocations,'storage_frees':c.frees,'import_calls':c.import_calls,'mismatches':0,'scope':__doc__};(ROOT/'port/game-data/reports/player-creation-v2-fresh-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
