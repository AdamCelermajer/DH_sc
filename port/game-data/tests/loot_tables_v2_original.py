"""Original typed Loot readers and Random clone versus optimized native ARM64.
Storage/stream services are explicit; original pointers/vtable headers alone
are normalized. The genuine first six cache readers select every actual row.
"""
import argparse,hashlib,json,struct,sys,random
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from items_differential import Original
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from navigation_differential import Cpu
W=lambda *x:struct.pack('<'+'I'*len(x),*(v&0xffffffff for v in x))
def digest(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def projection(c,p):
 w=struct.unpack('<9I',c.uc.mem_read(p,36));parts=[]
 for count,ptr,stride in ((w[3],w[4],32),(w[5],w[6],32)):
  parts.append(W(count)+b''.join(bytes(c.uc.mem_read(ptr+36*j+4,stride)) for j in range(count)))
 parts.append(W(w[7])+bytes(c.uc.mem_read(w[8],w[7]*4)) if w[7] else W(0))
 return W(w[1],w[2])+b''.join(parts)
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);a=p.parse_args();scratch=ROOT/'.local-inputs/player-creation-v2';ref=ROOT/'port/game-data/reference/player-creation-v2';ref.mkdir(parents=True,exist_ok=True)
 old=Original(ROOT/'.local-inputs/libDungeonHunter2.so',{'functions':[]});new=Cpu(a.library,True,{'functions':[]});cache=ROOT/'.local-inputs/items-discovery/loot_table_pyarray.bin';old.blob=cache.read_bytes();steps=[]
 for address in (0x4ba4fc,0x4ba3c8,0x4ba27c,0x4ba12c,0x4b9fe0,0x4b9e8c):
  start=old.cursor;old.invoke(address,[old.stream],budget=50000000);steps.append({'reader':hex(address),'start':start,'end':old.cursor})
 got=0x4b9eb8+old.word(0x4b9fd0);count=old.word(old.word(got+old.word(0x4b9fd4)));table=old.word(old.word(got+old.word(0x4b9fdc)))
 inputs=[];cursor=steps[-1]['start']+4
 for i in range(count):
  expected=projection(old,table+36*i);assert old.blob[cursor:cursor+len(expected)]==expected;inputs.append(expected);cursor+=len(expected)
 assert cursor==steps[-1]['end']
 rng=random.Random(20261005)
 for i in range(128):
  blob=W(rng.getrandbits(32),rng.getrandbits(32))
  for stride in (8,8,1):
   n=rng.randrange(5);blob+=W(n)+W(*(rng.getrandbits(32) for _ in range(n*stride)))
  old.blob=blob;old.cursor=0;row=old.data+0x2500;old.uc.mem_write(row,bytes(36));old.invoke(0x4fe794,[row,old.stream]);expected=projection(old,row);assert expected==blob and old.cursor==len(blob);inputs.append(blob)
 out=new.data+0x1000;b=new.data+0x2000
 for i,blob in enumerate(inputs):
  new.uc.mem_write(b,blob);new.uc.mem_write(out,bytes([0xa5])*64);assert new.invoke('dh2_loot_v2_decode',[out,b,len(blob)])==0
  raw=bytes(new.uc.mem_read(out,64));roll,probs=struct.unpack_from('<II',raw);actual=W(roll,probs)
  for off,stride in ((8,32),(24,32),(40,4)):
   ptr,n,res=struct.unpack_from('<QII',raw,off);assert res==0 and b<=ptr<=b+len(blob) and ptr+n*stride<=b+len(blob);actual+=W(n)+bytes(new.uc.mem_read(ptr,n*stride))
  assert actual==blob and struct.unpack_from('<II',raw,56)==(len(blob),0),(i,actual.hex(),blob.hex())
 base=0x401b10+old.word(0x401b84);seed=old.word(base+old.word(0x401b88));calls=old.word(base+old.word(0x401b8c));randoms=[]
 for i in range(512):
  initial=rng.getrandbits(32);counter=rng.getrandbits(32);bound=rng.choice((0,1,2,31,0x7fffffff,-1,-2,-2147483648));old.uc.mem_write(seed,W(initial));old.uc.mem_write(calls,W(counter));value=old.invoke(0x401afc,[bound]);want=W(old.word(seed),old.word(calls),value)
  new.uc.mem_write(out,W(initial,counter));assert not new.invoke('dh2_loot_v2_random',[out,bound,out+8]);got=bytes(new.uc.mem_read(out,12));assert got==want,(i,initial,bound,got.hex(),want.hex());randoms.append(W(initial,counter,bound)+want)
 gold=b'LTV2'+W(len(inputs))+b''.join(W(len(v))+v for v in inputs)+W(len(randoms))+b''.join(randoms);(ref/'fixtures.bin').write_bytes(gold)
 report={'validation':'PASS','original_sha256':digest(ROOT/'.local-inputs/libDungeonHunter2.so'),'library_sha256':digest(a.library),'source_sha256':{str(p.relative_to(ROOT)).replace('\\','/'):digest(p) for p in (ROOT/'port/game-data/loot_tables_v2.hpp',ROOT/'port/game-data/loot_tables_v2.cpp')},'script_sha256':digest(__file__),'gold_sha256':hashlib.sha256(gold).hexdigest(),'comparisons':len(inputs)+len(randoms),'actual_original_Loot_records':count,'synthetic_original_records':128,'original_random_operations':len(randoms),'reader_offsets':steps,'mismatches':0,'scope':__doc__}
 (ROOT/'port/game-data/reports/player-creation-v2-loot-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
