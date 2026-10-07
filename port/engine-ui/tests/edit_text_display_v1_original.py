"""Execute complete original 7928cc/78b128; renderer and glyph service requests remain explicit."""
import argparse,json,sys,struct,math,random,hashlib
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from text_layout_v1_original import TextCpu,words,fw,fl
class DisplayCpu(TextCpu):
 def external(self,uc,a,size,u):
  n=self.imports.get(a)
  if n in ('__aeabi_f2iz','__aeabi_f2uiz'):
   f=fl(self.reg(0));v=0 if math.isnan(f) else min(max(math.trunc(f) if math.isfinite(f) else (2**40 if f>0 else -2**40),-2**31 if n.endswith('iz') and not n.endswith('uiz') else 0),2**31-1 if n=='__aeabi_f2iz' else 2**32-1);self.put(0,v)
  elif n in ('cosf','sinf'):
   f=fl(self.reg(0));out=fw((math.cos if n=='cosf' else math.sin)(f));self.machine.trig+=words(0 if n=='cosf' else 1,fw(f),out);self.put(0,out)
  elif n in ('__aeabi_fcmplt','__aeabi_fcmpge'):
   x,y=fl(self.reg(0)),fl(self.reg(1));self.put(0,x<y if n.endswith('lt') else x>=y)
  else:return super().external(uc,a,size,u)
  uc.reg_write(self.pc,uc.reg_read(self.lr))
class Original:
 def __init__(self):
  self.c=DisplayCpu(ROOT/'.local-inputs/libDungeonHunter2.so',False,json.loads((ROOT/'port/engine-ui/reference/edit-text-connection-v1/original-functions.json').read_text()));self.c.machine=self;self.c.uc.hook_add(UC_HOOK_CODE,self.hook)
  d=self.c.data;self.state=d+0x1000;self.definition=d+0x2000;self.player=d+0x3000;self.root=d+0x4000;self.renderer=d+0x5000;self.vt=d+0x6000;self.matrix=d+0x7000;self.context=d+0x8000;self.provider=d+0x9000;self.effect=d+0xa000;self.parent=d+0xc000;self.parent_effect=d+0xd000;self.weak=d+0xf000
  self.renderglobal=d+0x10000;self.filterglobal=d+0x10010
  base=0x7928ec+self.word(0x793520)
  self.ptr(base+self.word(0x793524),self.renderglobal);self.ptr(base+self.word(0x793528),self.filterglobal)
  self.callbacks={};self.ptr(self.renderer,self.vt)
  for i,(off,kind) in enumerate(((0x50,0),(0x6c,1),(0x58,2),(0x78,3),(0x7c,4),(0x60,5),(0x4c,6),(0x64,7))):
   addr=self.c.callback+32+i*4;self.ptr(self.vt+off,addr);self.callbacks[addr]=kind
 def word(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def ptr(self,p,v):self.c.pointer(p,v)
 def ret(self,v=0):self.c.put(0,v);self.c.uc.reg_write(self.c.pc,self.c.uc.reg_read(self.c.lr))
 def hook(self,uc,a,size,u):
  if not self.active:return
  if a in self.callbacks and uc.reg_read(self.c.pc)!=a:return
  x,y,z=(self.c.reg(i) for i in range(3))
  if a in self.callbacks:
   k=self.callbacks[a];self.events+=words(k)
   if k==0:self.events+=uc.mem_read(y,24)
   elif k in (1,3):self.events+=words(self.word(z) if k==1 else y)
   elif k==4:self.events+=words(y)
   elif k in (2,5):self.events+=words(z)+uc.mem_read(y,z*8)
   elif k==6:self.events+=words(bool(y))
   self.ret()
  elif a==0x76d5b4:self.ret(self.root)
  elif a==0x753f74:self.ret(self.matrix)
  elif a==0x7738a4:self.events+=words(20);self.ret(bool(self.flags&8))
  elif a==0x78adc0:self.events+=words(21);self.ret()
  elif a==0x753f9c:self.events+=words(22);self.ret()
  elif a==0x78faa0:
   sp=self.c.uc.reg_read(self.c.sp);override=self.word(sp)
   self.events+=words(23,bool(x))+ (uc.mem_read(x,24) if x else words(fw(1),0,0,0,fw(1),0))+words(bool(override),self.word(override) if override else 0,self.word(sp+4),self.word(sp+8),self.word(sp+12));self.ret()
  elif a==0x8be2a0:
   f=fl(x);self.ret(0 if math.isnan(f) or f<=0 else min(int(f) if math.isfinite(f) else 2**40,2**32-1))
 def run(self,cfg,own,parent):
  self.flags=cfg[0];self.events=b'';self.trig=b'';c=self.c;uc=c.uc
  for p,n in ((self.state,0x300),(self.definition,0x100),(self.player,0x100),(self.root,0x100),(self.parent,0x100)):uc.mem_write(p,bytes(n))
  self.ptr(self.state+0x30,self.player);self.ptr(self.state+0x2c,self.weak);uc.mem_write(self.weak+4,b'\1');self.ptr(self.state+0xa0,self.definition);self.ptr(self.state+0xa8,1)
  self.ptr(self.player+0xac,self.context);self.ptr(self.context+0xc,self.provider);self.ptr(self.provider+4,cfg[1]);uc.mem_write(self.player+0x98,bytes((bool(self.flags&4),)))
  uc.mem_write(self.root+0x85,bytes((bool(self.flags&1),bool(self.flags&2))))
  uc.mem_write(self.definition+0x4e,bytes((bool(self.flags&32),)));self.ptr(self.definition+0x94,bool(self.flags&64));uc.mem_write(self.definition+0x24,words(*cfg[8:12]));uc.mem_write(self.matrix,words(*cfg[2:8]))
  self.ptr(self.state+0x194,cfg[12]);self.ptr(self.state+0x154,cfg[13]);self.ptr(self.state+0x158,cfg[14]);self.ptr(self.state+0x174,cfg[15]);uc.mem_write(self.state+0x14c,bytes((bool(self.flags&128),)))
  self.ptr(self.state+0x54,self.context+0x100 if self.flags&256 else 0);self.ptr(self.context+0x160,1)
  self.ptr(self.renderglobal,self.renderer if self.flags&512 else 0);self.ptr(self.filterglobal,1 if self.flags&16 else 0)
  self.ptr(self.state+0x50,self.effect);self.ptr(self.effect+4,self.effect+0x100);self.ptr(self.effect+8,len(own));uc.mem_write(self.effect+0x100,b''.join(own) or bytes(44))
  self.ptr(self.state+0x40,self.parent);self.ptr(self.state+0x3c,self.weak);self.ptr(self.parent+0x50,self.parent_effect);self.ptr(self.parent_effect+4,self.parent_effect+0x100);self.ptr(self.parent_effect+8,len(parent));uc.mem_write(self.parent_effect+0x100,b''.join(parent) or bytes(44))
  self.active=True;c.invoke(0x7928cc,[self.state],budget=1000000);self.active=False
  return words(len(self.events))+self.events+words(uc.mem_read(self.renderer+4,1)[0])+words(len(self.trig))+self.trig
def filt(kind,x,y,quality=5):
 b=bytearray(44);struct.pack_into('<I',b,0,kind);b[4:8]=bytes((30,60,90,120));struct.pack_into('<ffI',b,8,0.4,3.5,quality);struct.pack_into('<ff',b,32,x,y);return bytes(b)
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert not a.output.exists();m=Original();cases=[]
 choices=[[],[filt(0,2.5,4.75)],[filt(1,0,0)],[filt(1,3.25,5.75)],[filt(2,2.5,7.25)],[filt(2,7.25,2.5,0)],[filt(0,2,4),filt(1,5,8),filt(2,3,7,30)],[filt(3,2,8)]]
 for flags in range(1024):
  for i in range(2):
   cfg=[flags,fw(1 if i==0 else 0.75),fw(1.25),fw(0.5),fw(33.75),fw(-0.25),fw(0.75),fw(44.5),fw(-3.5),fw(250.25),fw(8.75),fw(330.5),0xff123456,fw(20.5),fw(105.25),fw(240.5)]
   own=choices[(flags//2)%len(choices)];parent=choices[(flags//8)%len(choices)];out=m.run(cfg,own,parent);cases.append(words(*cfg,len(own))+b''.join(own)+words(len(parent))+b''.join(parent)+words(len(out))+out)
 blob=words(0x31584454,len(cases))+b''.join(cases);a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(blob);print(json.dumps({'validation':'PASS','original_cases':len(cases),'gold_sha256':hashlib.sha256(blob).hexdigest(),'scope':__doc__}))
if __name__=='__main__':main()
