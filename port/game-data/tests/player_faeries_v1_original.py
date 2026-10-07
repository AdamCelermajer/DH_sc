"""Actual saved-faery constructor/init, setters, named readers and level
queries. Three source difficulties each allocate literal five rows; selected
difficulty is an actual source global, not private per-owner inferred state.
"""
import struct,json,hashlib,random,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from player_savegame_v1_original import Save,ROOT,W
def snapshot(c):
 b=b''
 for s in range(3):
  p=c.word(c.obj+0x94+4*s);assert c.word(c.obj+0xa0+4*s)==5
  for i in range(5):x=bytes(c.uc.mem_read(p+4*i,4));b+=x[:1]+b'\0'+x[2:]
 return b+bytes(c.uc.mem_read(c.obj+0xac,12))+W(c.word(c.obj+0x3c))
def main():
 c=Save();r=random.Random(20261004);cases=[];comparisons=0;queries=0
 for k in range(48):
  c.fresh([1]);c.invoke(0x4694c8,[c.obj]);ops=[]
  for j in range(18):
   if j%6==0:op=W(0);c.invoke(0x4694c8,[c.obj]);cursor=0
   elif j%6 in (1,2):
    id=r.randrange(5);difficulty=r.randrange(3);value=r.choice([0,1,255,256,-1,65535,65536,0x7fffffff]);op=W(j%6,id,value,difficulty);c.invoke(0x46663c if j%6==1 else 0x466588,[c.obj,id,value&0xffffffff,difficulty]);cursor=0
   elif j%6==3:
    blob=b''
    for s in range(3):
     count=4 if k%8==0 and s==k%3 else 5;blob+=W(r.choice([0,1,-1,99]),count)+b''.join(struct.pack('<HB',r.randrange(65536),r.randrange(256)) for _ in range(count))
    op=W(3,len(blob))+blob;c.blob=blob;c.cursor=0;c.invoke(0x4691d0,[c.stream,c.obj]);cursor=c.cursor
   elif j%6==4:
    blob=W(*(r.choice([-1,0,1,4,5,500]) for _ in range(3)));op=W(4,len(blob))+blob;c.blob=blob;c.cursor=0;c.invoke(0x468cd4,[c.stream,c.obj]);cursor=c.cursor
   else:
    blob=W(r.randrange(3),r.choice([-1,0,1,2,0x7fffffff]));op=W(5,len(blob))+blob;c.blob=blob;c.cursor=0;c.invoke(0x468968,[c.stream,c.obj]);cursor=c.cursor
   gold=snapshot(c)
   for s in range(3):
    for i in range(5):assert c.invoke(0x466700,[c.obj,i,s])==struct.unpack_from('<H',gold,s*20+i*4+2)[0];queries+=1
   ops.append(W(len(op))+op+W(cursor)+gold);comparisons+=1
  cases.append(W(len(ops))+b''.join(ops))
 gold=b'FSG1'+W(len(cases))+b''.join(cases);ref=ROOT/'port/game-data/reference/player-savegame-v1';(ref/'faery-fixtures.bin').write_bytes(gold)
 report={'validation':'PASS','comparisons':comparisons,'level_queries':queries,'original_sha256':hashlib.sha256((ROOT/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'gold_sha256':hashlib.sha256(gold).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':__doc__,'native_comparison':False};(ref/'original-faery-gold.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
