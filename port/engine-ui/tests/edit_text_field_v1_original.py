"""Original field setters composed with the original whole layout/parser.

The optional variable parse/target/get/set and AS conversion are explicit graph
fixture services. The original setter, binding gates and to_string coordinator
execute, including double conversion and synchronous field mutation.
"""
import argparse,hashlib,json,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from text_layout_v1_original import Original,words,fw,txt

class Field(Original):
 def __init__(self):
  super().__init__(ROOT/'.local-inputs/libDungeonHunter2.so',json.loads((ROOT/'port/engine-ui/reference/edit-text-connection-v1/original-functions.json').read_text()))
 def hook(self,uc,address,size,unused):
  if not self.active:return
  c=self.c;a,b,d=(c.reg(i) for i in range(3))
  # TextCpu's external hook has already delivered this callback before the
  # all-code hook runs. Do not dispatch again using its changed result regs.
  if c.callback+32<=address<=c.callback+96 and uc.reg_read(c.pc)!=address:return
  if address==0x7cd130:
   self.bound_events+=words(10)+txt(self.string(a));self.ret(0)
  elif address==c.callback+48 and a==self.parent:
   self.bound_events+=words(11)+txt(self.string(b))
   uc.mem_write(d,bytes(12));uc.mem_write(d+1,bytes((5 if self.bound[0]&2 else 0,)))
   if self.bound[0]&2:c.pointer(d+4,self.state)
   self.ret(bool(self.bound[0]&1))
  elif address==0x420a84:
   n=self.converted;self.converted+=1;self.bound_events+=words(12,n)
   if self.bound[0]&4:self.store(self.state+0x138,self.bound[1])
   p=self.alloc(16);self.store(p,self.bound[2] if n else self.bound[1]);self.ret(p)
  elif address==0x797350:self.values[a]=self.cstr(b);self.ret(a)
  elif address==c.callback+64 and a==self.parent:
   self.bound_events+=words(13)+txt(self.string(b))+txt(self.values[d]);self.ret(1)
  elif address==0x797124:self.ret()
  elif address in (0x76146c,0x784a8c,0x761970,0x784cac):
   self.bound_events+=words({0x76146c:14,0x784a8c:15,0x761970:16,0x784cac:17}[address]);self.ret()
  else:super().hook(uc,address,size,unused)
 def setup(self,cfg,initial,maximum,variable,bound):
  super().configure(cfg,initial)
  c=self.c;self.bound=bound;self.converted=0;self.bound_events=b'';self.values={}
  c.pointer(self.definition+0x64,maximum&0xffffffff);self.store(self.definition+0x34,variable)
  parent=self.parent=self.alloc(0x80);pv=self.alloc(0x40);wh=self.alloc(8)
  c.pointer(parent,pv);c.pointer(pv+0x20,c.callback+48);c.pointer(pv+0x1c,c.callback+64)
  c.pointer(self.state+0x40,parent);c.pointer(self.state+0x3c,wh);c.uc.mem_write(wh+4,b'\1')
  c.pointer(self.vt+0xc,0x791ac4)
 def run(self,op,html,input):
  if op in (0,1):
   p=self.alloc(16);self.store(p,input)
   self.c.invoke(0x78f1ec if op==0 else 0x790ab0,[self.state,p,html],budget=5000000)
  elif op==2:self.c.invoke(0x791ac4,[self.state],budget=5000000)
  else:
   self.store(self.definition+0x7c,input)
   for off,value in ((0x58,self.fontptr if self.cfg[11]&1 else 0),(0x5c,self.cfg[0]),(0x60,0),(0x68,self.cfg[1]),(0x6c,self.cfg[5]),(0x70,self.cfg[6]),(0x74,self.cfg[7]),(0x78,self.cfg[8])):self.c.pointer(self.definition+off,value)
   self.c.uc.mem_write(self.definition+0x4f,b'\1')
   self.c.invoke(0x7914dc,[self.state],budget=5000000)
  self.active=False
  out=txt(self.string(self.state+0x138))+self.snapshot()+words(len(self.bound_events))+self.bound_events
  if op==3:out+=words(self.read(self.state+0x9d,1)[0],self.read(self.state+0x14c,1)[0],self.word(self.state+0x150),self.word(self.state+0x194))
  return out

def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);p.add_argument('--init',action='store_true');a=p.parse_args();assert not a.output.exists()
 m=Field();cases=[]
 texts=['','AV','one two three','A<font color="#aB12ef" size="18">BIG</font>Z','<u>A\nV</u>','A<img src="Icon" width="11" height="13">B','<font face="Other"><b>AV</b></font>','one\r\ntwo','日本語Ω']
 # Same-input gates retain unformatted/cached prefix; length and HTML policy
 # vary independently. Byte truncation is kept inside valid UTF-8 boundaries.
 for op in (() if a.init else (0,1)):
  for html in (0,1):
   for maximum in (-1,0,1,7,100):
    for i,input in enumerate(texts):
     if maximum in (1,7) and i==8:continue
     for same in (0,1):
      cfg=[fw(240 if i%3 else 360),i%4,fw(1000 if i%2 else 4000),i%2,i%2,fw(20),fw(40),fw(10),fw(5),fw(2),int(i%4!=0),int(i%7!=0)]
      initial=input if same else 'old';variable='bound' if op else '';bound=(0,'','')
      m.setup(cfg,initial,maximum,variable,bound);out=m.run(op,html,input)
      cases.append(words(op,html,maximum,*cfg)+txt(initial)+txt(input)+txt(variable)+words(bound[0])+txt(bound[1])+txt(bound[2])+words(len(out))+out)
 # Genuine original read gates: miss/self/match/mismatch; second conversion
 # differs, and first conversion mutates the field to suppress the setter.
 for flags in (() if a.init else (0,1,3,5)):
  for first in ('old','changed','<u>AV</u>'):
   for second in ('second','AV',''):
    for hasfont in (0,1):
     cfg=[fw(240),2,fw(1000),1,0,fw(20),fw(40),fw(10),fw(5),fw(2),1,hasfont]
     bound=(flags,first,second);m.setup(cfg,'old',0,'bound',bound);out=m.run(2,0,'')
     cases.append(words(2,0,0,*cfg)+txt('old')+txt('')+txt('bound')+words(flags)+txt(first)+txt(second)+words(len(out))+out)
 if a.init:
  for supplied in texts:
   for variable in ('','bound'):
    for maximum in (0,1,7):
     if maximum and supplied==texts[8]:continue
     for flags in (0,1,3,5):
      for hasfont in (0,1):
       cfg=[fw(240),2,fw(1000),1,0,fw(20),fw(40),fw(10),fw(5),fw(2),1,hasfont]
       bound=(flags,'changed','AV');m.setup(cfg,'old',maximum,variable,bound);out=m.run(3,0,supplied)
       cases.append(words(3,0,maximum,*cfg)+txt('old')+txt(supplied)+txt(variable)+words(flags)+txt(bound[1])+txt(bound[2])+words(len(out))+out)
 blob=words(0x31464554,len(cases))+b''.join(cases);a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(blob)
 print(json.dumps({'validation':'PASS','original_cases':len(cases),'gold_sha256':hashlib.sha256(blob).hexdigest(),'scope':__doc__}))
if __name__=='__main__':main()
