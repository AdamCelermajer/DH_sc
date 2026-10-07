"""Original requirement/weapon accessors versus optimized native projections.
Online-manager and remote virtual return values are explicit caller services;
original ItemInventory/current-set/item-table and all predicate bodies execute.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(R/'port/engine-resources/tests'));from cpu import Cpu,i32
W=lambda *v:struct.pack('<'+'I'*len(v),*(v&0xffffffff for v in v))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
class Source(Cpu):
 def external(self,uc,a,z,u):
  if a==0x7fd794:self.put(0,self.manager);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if a==self.callback+16:self.put(0,self.remote);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  super().external(uc,a,z,u)
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,required=True);a=ap.parse_args();old=Source(R/'.local-inputs/libDungeonHunter2.so',False,{'functions':[]});new=Cpu(a.library,True,{'functions':[]});rng=random.Random(20261005)
 def word(p):return struct.unpack('<I',old.uc.mem_read(p,4))[0]
 got=(0x3f9e1c+word(0x3f9e2c))&0xffffffff;table=old.data+0x20000;old.pointer(word(got+word(0x3f9e30)),table)
 char=old.data+0x1000;inv=char+0x37c;old.pointer(inv+4,char);old.manager=old.data+0x10000;vt=old.manager+0x100;old.pointer(char,vt);old.pointer(vt+0x54,old.callback+16);old.uc.hook_add(UC_HOOK_CODE,old.external,begin=0x7fd794,end=0x7fd794)
 item=old.data+0x12000;old.uc.mem_write(item,W(0,0));native_row=new.data+0x10000;f=new.data+0x11000;out=new.data+0x12000;req=[];queries=[];comparisons=0
 edges=[0,1,255,256,257,0x7fffffff,0x80000000,0xffffffff,0xffffff00,0xfffffeff]
 for k in range(1000):
  online=rng.choice((0,1,255));old.remote=rng.choice((0,1,0xffffffff));present=rng.randrange(2);cached=[i32(rng.choice(edges))if k<500 else i32(rng.getrandbits(32))for _ in range(5)];row=[0]*41
  for j in range(5):row[29+j]=rng.choice((-1,0,1,8388607,-8388608,cached[j]>>8,(cached[j]>>8)+1))
  old.uc.mem_write(old.manager+5,bytes([online]));old.uc.mem_write(table,W(*row));
  for j,index in enumerate((19,149,150,151,152)):old.uc.mem_write(char+0xff8+4*index,W(cached[j]))
  expected=i32(old.invoke(0x3a4930,[char,item if present else 0]));new.uc.mem_write(native_row,W(*row));new.uc.mem_write(f,W(online,old.remote,present,*cached));assert new.invoke('dh2_equipment_requirements_v1',[out,f,native_row if present else 0])==0;actual=i32(struct.unpack('<I',new.uc.mem_read(out,4))[0]);assert actual==expected,(k,expected,actual);req.append(W(online,old.remote,present,*cached,*row,expected));comparisons+=1
 headers=old.data+0x21000;slots=[headers+0x100,headers+0x200];old.pointer(inv+0x14,headers)
 for j in range(2):old.uc.mem_write(headers+j*12,W(slots[j],slots[j]+36,slots[j]+36))
 for k in range(1000):
  selected=rng.randrange(2);old.uc.mem_write(inv+0x2e,bytes([selected]));flag=rng.choice((0,1,2,-1));old.uc.mem_write(char+0x1324,W(flag));rows=[];present=[]
  for j in range(2):
   row=[0]*41;row[22]=rng.choice((0,3,4,5,6,7,-1));row[26]=rng.choice((-4,-3,-1,1,2));row[37]=rng.choice((-1,0,3,4,5,6));rows.append(row);present.append(rng.randrange(2));old.uc.mem_write(table+j*164,W(*row));instance=item+j*0x100;cell=item+0x400+j*4;old.uc.mem_write(instance,W(0,j));old.pointer(cell,instance)
   for ss in range(2):old.pointer(slots[ss]+4*(j+1),cell if present[j] and ss==selected else 0)
  flags=0
  for address,bit,args in ((0x3ffe8c,1,[]),(0x400080,2,[]),(0x4000c8,4,[]),(0x40019c,8,[]),(0x400110,16,[]),(0x4001a0,32,[1]),(0x4001a0,64,[0])):
   if old.invoke(address,[inv,*args]):flags|=bit
   comparisons+=1
  categories=[]
  for j in range(2):
   p=old.invoke(0x3ffe3c,[inv,j+1]);base=old.invoke(0x3f9e08,[p])if p else 0;categories.append(i32(word(base+0x94))if base else -1)
  new.uc.mem_write(native_row,W(*rows[0],*rows[1]));assert new.invoke('dh2_equipment_queries_v1',[out,native_row if present[0]else 0,native_row+164 if present[1]else 0,flag])==0;expected=W(*categories,flags);assert bytes(new.uc.mem_read(out,12))==expected,(k,categories,flags);queries.append(W(selected,flag,*present,*rows[0],*rows[1],*categories,flags))
 ref=R/'port/level-world/reference/player-equipment-render-owner-v1';ref.mkdir(parents=True,exist_ok=True);gold=ref/'query-fixtures.bin';gold.write_bytes(b'ERQ1'+W(len(req),len(queries))+b''.join(req)+b''.join(queries))
 src=['port/level-world/player_equipment_queries_v1.hpp','port/level-world/player_equipment_queries_v1.cpp',Path(__file__).relative_to(R).as_posix()];report=dict(validation='PASS',requirements_cases=len(req),weapon_cases=len(queries),original_predicate_comparisons=comparisons,mismatches=0,original_sha256=sha(R/'.local-inputs/libDungeonHunter2.so'),library_sha256=sha(a.library),gold_sha256=sha(gold),source_sha256={p:sha(R/p)for p in src},scope=__doc__);p=R/'port/level-world/reports/player-equipment-queries-v1-arm64-differential.json';p.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
