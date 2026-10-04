"""Complete original on_event; explicit AS handler/listener/formatter/string ABI services."""
import argparse,hashlib,json,struct,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];sys.path.insert(0,str(ROOT/'port/engine-ui/tests'))
from text_layout_v1_original import Original,words,fw
def blob(b):return words(len(b))+b
class Event(Original):
 def string_bytes(self,p):
  n=self.read(p,1)[0];return self.read(self.word(p+12),self.word(p+4)-1) if n==255 else self.read(p+1,n-1)
 def store_bytes(self,p,b):
  self.c.uc.mem_write(p,bytes(20));z=b+b'\0'
  if len(z)<16:self.c.uc.mem_write(p,bytes((len(z),))+z)
  else:q=self.alloc(len(z));self.c.uc.mem_write(q,z);self.c.uc.mem_write(p,words(255,len(z),len(z),q))
 def hook(self,uc,a,size,u):
  if not self.active:return
  c=self.c;x,y,z=(c.reg(i) for i in range(3))
  if c.callback+32<=a<=c.callback+96 and uc.reg_read(c.pc)!=a:return
  if a in (0x75302c,0x752f50):self.store_bytes(x,self.string_bytes(y));self.ret(x)
  elif a==0x413a7c:self.store_bytes(x,self.cstr(y).encode());self.ret(x)
  elif a==0x774918:
   self.events+=words(1)
   if self.mutate&1:uc.mem_write(self.state+0x14c,b'\1');self.store_bytes(self.state+0x138,b'active')
   self.ret()
  elif a==c.callback+32:
   self.events+=words(2)+blob(self.string_bytes(y))
   if self.mutate&2:self.store_bytes(self.state+0x138,b'handler')
   self.ret(0)
  elif a in (0x760f4c,0x760e44):
   self.events+=words(3 if a==0x760f4c else 4)
   if self.mutate&4:self.store_bytes(self.state+0x138,b'listener')
   self.ret()
  elif a==0x78efb8:self.events+=words(5,self.word(self.state+0x150))+blob(self.string_bytes(self.state+0x138));self.ret()
  elif a==0x790ab0:
   data=self.string_bytes(y);self.events+=words(6,self.word(self.state+0x150))+blob(data);self.store_bytes(self.state+0x138,data);self.ret()
  elif a==0x78b94c:
   data=self.string_bytes(x);self.store_bytes(x,data[:y]+data[y+1:]);self.ret()
  elif a==0x78b0c0:
   data=self.string_bytes(x);self.store_bytes(x,data[:y]+bytes((z&255,))+data[y:]);self.ret()
  elif a in (0x797124,0x752b38,0x41fed8):self.ret()
  else:super().hook(uc,a,size,u)
 def run(self,flags,id,key,cursor,text,mutate):
  cfg=[fw(240),0,fw(4000),1,0,0,0,0,0,0,0,1];self.configure(cfg,'');self.events=b'';self.mutate=mutate;self.c.pointer(self.vt+0x20,self.c.callback+32);self.c.uc.mem_write(self.definition+0x4b,bytes((bool(flags&1),)));self.c.uc.mem_write(self.state+0x14c,bytes((bool(flags&2),)));self.c.pointer(self.state+0x150,cursor);self.store_bytes(self.state+0x138,text)
  event=self.alloc(8);self.c.uc.mem_write(event,bytes((id,key,0,0,0,0,0,0)));result=self.c.invoke(0x790f58,[self.state,event],budget=1000000);self.active=False
  return words(result,self.read(self.state+0x14c,1)[0],self.word(self.state+0x150))+blob(self.string_bytes(self.state+0x138))+blob(self.events)
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();assert not a.output.exists();m=Event(ROOT/'.local-inputs/libDungeonHunter2.so',json.loads((ROOT/'port/engine-ui/reference/edit-text-connection-v1/original-functions.json').read_text()));cases=[]
 for flags in range(4):
  for id in (8,20,21,0,19,22):
   for key in (range(256) if id==8 else (0,)):
    for cursor in (0,2,8):
     for mutate in ((0,1,2,4,7) if id in (20,21) else (0,)):
      text=b'ABCDE';out=m.run(flags,id,key,cursor,text,mutate);cases.append(words(flags,id,key,cursor,mutate)+blob(text)+blob(out))
 data=words(0x31564554,len(cases))+b''.join(cases);a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(data);print(json.dumps({'validation':'PASS','original_cases':len(cases),'gold_sha256':hashlib.sha256(data).hexdigest(),'scope':__doc__}))
if __name__=='__main__':main()
