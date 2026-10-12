"""Original getTextFormat body with real instruction arithmetic and explicit
constructor/virtual property writes/string pool services. Mutating setters
verify that subsequent field and font reads observe synchronous reentry.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
import text_layout_v1_original as base
from text_layout_v1_original import Original,TextCpu,words,fw,fl,txt
class FormatCpu(TextCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='__aeabi_f2d':
   lo,hi=struct.unpack('<II',struct.pack('<d',fl(self.reg(0))));self.put(0,lo);self.put(1,hi);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
base.TextCpu=FormatCpu
class Probe(Original):
 def __init__(self):
  super().__init__(ROOT/'.local-inputs/libDungeonHunter2.so',json.loads((ROOT/'port/engine-ui/reference/edit-text-connection-v1/method-map/original-functions.json').read_text()))
 def hook(self,uc,address,size,unused):
  if not self.active:return
  c=self.c;a,b,d=(c.reg(i) for i in range(3))
  if c.callback+32<=address<=c.callback+96 and uc.reg_read(c.pc)!=address:return
  if address==0x7a68b8:
   self.events+=words(1);result=self.word(a);uc.mem_write(result,bytes(12));uc.mem_write(result+1,b'\5');c.pointer(result+4,self.target);self.ret()
  elif address==0x75c2cc:
   value=self.string(b);self.events+=words(2)+txt(value);p=self.alloc(20);self.intern[p]=value;self.ret(p)
  elif address==0x7972d8:self.strings[a]=self.intern[b];self.ret(a)
  elif address==0x43e364:self.ret(self.root)
  elif address==c.callback+64 and a==self.target:
   key=self.string(b);kind=self.read(d+1,1)[0]
   if d in self.strings:
    value=words(3)+txt(self.strings.pop(d))
   elif kind==2:value=words(2)+self.read(d+4,8)
   elif kind==1:value=words(1,self.read(d+4,1)[0])
   else:raise AssertionError((kind,hex(d),key))
   self.events+=words(3)+txt(key)+value
   if self.mutate and key=='leftMargin':c.pointer(self.state+0x188,fw(-7.5))
   if self.mutate and key=='font':uc.mem_write(self.fontptr+0x4d,b'\1')
   if self.mutate and key=='bold':uc.mem_write(self.fontptr+0x4c,b'\1')
   self.ret(1)
  elif address==0x797124:self.strings.pop(a,None);self.ret()
  else:super().hook(uc,address,size,unused)
 def run(self,cfg,mutate):
  self.configure(cfg[:12],'');c=self.c;self.events=b'';self.intern={};self.strings={};self.mutate=mutate;c.pointer(self.vt+8,0x78a2f8)
  for off,w in zip((0x180,0x188,0x184,0x18c,0x190,0x174,0x17c,0x170),cfg[12:20]):c.pointer(self.state+off,w)
  self.store(self.fontptr+0x30,'Fontin');c.uc.mem_write(self.fontptr+0x4d,bytes((cfg[20],)));c.uc.mem_write(self.fontptr+0x4c,bytes((cfg[21],)))
  self.target=self.alloc(64);vt=self.alloc(48);c.pointer(self.target,vt);c.pointer(vt+0x1c,c.callback+64)
  fv=self.alloc(48);result=self.alloc(16);env=self.alloc(0x80);wh=self.alloc(8);uc=c.uc;uc.mem_write(wh+4,b'\1');c.pointer(env+0x64,wh);c.pointer(env+0x68,self.root)
  c.pointer(fv,result);c.pointer(fv+4,self.state);c.pointer(fv+0xc,env)
  c.invoke(0x791d48,[fv],budget=2000000);self.active=False;return self.events
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert not a.output.exists()
 m=Probe();r=random.Random(791);cases=[]
 for i in range(384):
  cfg=[fw(240),0,fw(4000),1,0,0,0,0,0,0,0,1]
  cfg +=[fw(r.uniform(-1000,1000)) for _ in range(6)]+[i%6,r.getrandbits(32),i%2,(i>>1)%2]
  mutate=(i%3==0);out=m.run(cfg,mutate);cases.append(words(*cfg,mutate,len(out))+out)
 blob=words(0x314d4654,len(cases))+b''.join(cases);a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(blob)
 print(json.dumps({'validation':'PASS','original_get_format_cases':len(cases),'gold_sha256':hashlib.sha256(blob).hexdigest(),'scope':__doc__}))
if __name__=='__main__':main()
