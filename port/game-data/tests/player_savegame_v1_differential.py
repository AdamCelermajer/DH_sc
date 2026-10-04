"""Optimized ARM64 instruction replay of original-derived saved-skill reader
and low16 fields, plus actual source potion/equipment scalar functions.
Owned STL initialization/assignment/native lifetime are proved separately by
the whole-owner sanitizer replay; caller storage/name services are explicit.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(Path(__file__).resolve().parent))
from player_savegame_v1_original import Save,W
from items_differential import strings
from navigation_differential import Cpu
def snapshot(blob):
 at=0
 def word():
  nonlocal at
  x=struct.unpack_from('<I',blob,at)[0];at+=4;return x
 count=word();rows=blob[at:at+8*count];at+=8*count;maps=[]
 for _ in range(2):
  m={}
  for _ in range(word()):k=word();v=word();m[k]=v
  maps.append(m)
 return rows,maps
class Native(Cpu):
 def __init__(self,path):
  super().__init__(path,True,{'functions':[]});self.maps=[{},{}];self.addresses={};self.next=self.data+0x90000
  self.names=strings((ROOT/'.local-inputs/skill-tables/skills_pyarraynames.bin').read_bytes())[1]
 def returned(self,n=0):self.put(0,n);self.uc.reg_write(self.pc,self.uc.reg_read(self.lr))
 def external(self,uc,a,z,u):
  if a==self.callback+32:
   key=bytes(uc.mem_read(self.reg(1),self.reg(2))).split(b'\0',1)[0];self.returned(self.names.index(key) if key in self.names else 0xffffffff)
  elif a==self.callback+48:
   s=self.reg(1);k=self.reg(2)&0xffffffff;t=(s,k)
   if t not in self.addresses:self.addresses[t]=self.next;self.next+=8;uc.mem_write(self.addresses[t],W(0))
   self.returned(self.addresses[t])
  else:super().external(uc,a,z,u)
 def set_maps(self,maps):
  self.addresses={};self.next=self.data+0x90000
  for s,m in enumerate(maps):
   for k,v in m.items():self.addresses[s,k]=self.next;self.next+=8;self.uc.mem_write(self.addresses[s,k],W(v))
 def get_maps(self):
  m=[{},{}]
  for (s,k),p in self.addresses.items():m[s][k]=struct.unpack('<I',self.uc.mem_read(p,4))[0]
  return m
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args();new=Native(a.library);old=Save();gold=ROOT/'port/game-data/reference/player-savegame-v1/fixtures.bin';b=gold.read_bytes();at=8
 def word():
  nonlocal at
  x=struct.unpack_from('<I',b,at)[0];at+=4;return x
 def block():
  nonlocal at
  n=word();x=b[at:at+n];at+=n;return x
 reader_cases=0;level_cases=0;rowsaddr=new.data+0x1000;view=new.data+0x2000;span=new.data+0x2100;services=new.data+0x2200;input=new.data+0x3000
 new.uc.mem_write(services,struct.pack('<4Q',new.data,new.callback+32,new.callback+48,0))
 for _ in range(struct.unpack_from('<I',b,4)[0]):
  n=word();ids=[word() for _ in range(n)];count=word();rows=b'';maps=[{},{}]
  for _ in range(count):
   op=block();expected=block();kind=struct.unpack_from('<I',op)[0];wantrows,wantmaps=snapshot(expected)
   if kind in (1,3):
    new.uc.mem_write(rowsaddr,rows);new.uc.mem_write(view,struct.pack('<QII',rowsaddr,len(rows)//8,0));new.set_maps(maps)
    if kind==1:
     i,value=struct.unpack_from('<II',op,4);assert new.invoke('dh2_saved_skill_v1_set_level',[view,i,value])==0;level_cases+=1
    else:
     raw=op[8:];assert len(raw)==struct.unpack_from('<I',op,4)[0];new.uc.mem_write(input,raw);new.uc.mem_write(span,struct.pack('<Q4I',input,len(raw),0,0,0));assert new.invoke('dh2_player_skills_v1_load',[view,span,services])==0
     assert struct.unpack('<Q4I',new.uc.mem_read(span,24))[2]==len(raw);assert new.get_maps()==wantmaps;reader_cases+=1
    assert bytes(new.uc.mem_read(rowsaddr,len(wantrows)))==wantrows
   rows,maps=wantrows,wantmaps
 assert at==len(b)
 scalar=0;inv=old.data+0xa000;item=old.data+0xb000
 for raw in [0,1,32767,32768,65535,65536,0x7fffffff,0xffffffff,*range(0,65536,251)]:
  old.pointer(inv+0x24,item);old.uc.mem_write(item+0x50,struct.pack('<H',raw&65535));want=old.invoke(0x3fc690,[inv]);assert new.invoke('dh2_inventory_v1_quantity',[raw])&0xffffffff==want;scalar+=1
 for raw in range(256):
  for requested in [-2,-1,0,1,2,3,4,0x7fffffff]:
   old.uc.mem_write(inv+0x2e,bytes([raw]));want=old.invoke(0x3fc6a8,[inv,requested&0xffffffff]);assert new.invoke('dh2_inventory_v1_current_equipment',[raw,requested&0xffffffff])&0xffffffff==want;scalar+=1
  old.uc.mem_write(inv+0x2e,bytes([raw]));old.invoke(0x3fc6c8,[inv]);assert new.invoke('dh2_inventory_v1_swap_equipment',[raw])==old.uc.mem_read(inv+0x2e,1)[0];scalar+=1
 report={'validation':'PASS','original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'library_sha256':hashlib.sha256(a.library.read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(gold.read_bytes()).hexdigest(),'source_sha256':{str(x.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(x.read_bytes()).hexdigest() for x in [ROOT/'port/game-data/player_savegame_v1.hpp',ROOT/'port/game-data/player_savegame_v1.cpp']},'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'reader_cases':reader_cases,'level_write_cases':level_cases,'actual_original_scalar_cases':scalar,'comparisons':reader_cases+level_cases+scalar,'mismatches':0,'scope':__doc__};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
