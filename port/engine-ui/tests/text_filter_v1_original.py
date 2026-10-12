"""Original filter-list instructions; stream/storage are explicit services.

Compare defined fields only. Unwritten stack bytes are not invented source data.
The source consumes u16 strength then signed low byte, and an extra byte after
shadow/glow flags. Unknown kinds consume only their kind byte.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from text_layout_v1_original import Original,words,fw

OPS={0x783b28:1,0x783be0:2,0x7839a4:3,0x783a50:4,0x783ea8:5,0x796888:6}
def mask(kind):
 b=bytearray(44)
 def region(a,n):b[a:a+n]=bytes([1])*n
 region(0,4);region(32,8)
 if kind==0:region(4,16);region(20,3);region(24,8)
 elif kind==1:region(40,4)
 elif kind==2:region(4,8);region(12,3);region(16,4);region(40,4)
 else:raise AssertionError(kind)
 return bytes(b)

class Probe(Original):
 def __init__(self):
  super().__init__(ROOT/'.local-inputs/libDungeonHunter2.so',json.loads((ROOT/'port/engine-ui/reference/edit-text-connection-v1/filter-producer/original-functions.json').read_text()))
 def hook(self,uc,address,size,unused):
  if not self.active:return
  c=self.c;a,b=(c.reg(i) for i in range(2))
  if address in OPS:
   op,arg,value=self.input[self.cursor];self.cursor+=1;assert op==OPS[address],(hex(address),op)
   if op==3:assert arg==b
   self.events+=words(op,arg,value)
   if op==6:uc.mem_write(a,words(value));self.ret(a)
   else:self.ret(value)
  elif address==0x752ec8:
   p,n=self.array(a);oldcap=self.word(a+8)
   if b>oldcap:
    q=self.alloc(max(b,1)*44)
    if n:self.c.uc.mem_write(q,self.read(p,n*44))
    c.pointer(a,q);c.pointer(a+8,b)
   self.ret()
  else:super().hook(uc,address,size,unused)
 def run(self,inputs):
  self.next=self.c.data+0x10000;self.input=inputs;self.cursor=0;self.events=b'';self.active=True
  self.c.uc.mem_write(self.definition,bytes(24));self.c.invoke(0x758ca8,[self.state,self.definition],budget=1000000)
  p,n=self.array(self.definition+4);output=words(n)
  for i in range(n):
   raw=self.read(p+i*44,44);defined=mask(struct.unpack_from('<I',raw)[0]);output+=defined+bytes(a if b else 0 for a,b in zip(raw,defined))
  assert self.cursor==len(inputs);return output+words(len(self.events))+self.events

def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert not a.output.exists()
 r=random.Random(7984);cases=[];m=Probe()
 for case in range(640):
  inputs=[]
  def q(op,value=0,arg=0):inputs.append((op,arg,value&0xffffffff))
  kinds=[] if case==0 else [r.randrange(8) for _ in range(1+case%7)]
  q(1,len(kinds))
  for k in kinds:
   q(1,k)
   if k>3:continue
   if k==3:
    q(6,r.getrandbits(32));q(6,r.getrandbits(32))
    for _ in range(4):q(5,fw(r.uniform(-30,30)))
    q(2,r.getrandbits(16))
    for _ in range(4):q(4,r.randrange(2))
    q(3,r.randrange(16),4);q(1,r.getrandbits(8));continue
   if k!=1:q(6,r.getrandbits(32))
   q(5,fw(r.uniform(-30,30)));q(5,fw(r.uniform(-30,30)))
   if k==0:q(5,fw(r.uniform(-30,30)));q(5,fw(r.uniform(-30,30)))
   if k==1:q(3,r.randrange(32),5);q(3,r.randrange(8),3)
   else:
    q(2,r.getrandbits(16))
    for _ in range(3):q(4,r.randrange(2))
    q(3,r.randrange(32),5);q(1,r.getrandbits(8))
  out=m.run(inputs);cases.append(words(len(inputs))+b''.join(words(*row) for row in inputs)+words(len(out))+out)
 blob=words(0x31465445,len(cases))+b''.join(cases);a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(blob)
 print(json.dumps({'validation':'PASS','original_filter_cases':len(cases),'gold_sha256':hashlib.sha256(blob).hexdigest(),'scope':__doc__}))
if __name__=='__main__':main()
