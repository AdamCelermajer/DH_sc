"""Complete original option/iPod wrapper ordering vs native O2.
AS, settings, integer localization and JNI services are explicit fixtures.
Genuine original global reads/stores and reentrant AS setter reloads execute.
"""
import argparse,json,random,struct
from pathlib import Path
from hud_initialization_v1_differential import Machine,ROOT,REPO,REF,words,word,sint,sha,token,MEMBERS
import hud_initialization_v1_differential as base
class OptionsCpu(base.TimelineCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='strcmp':
   def string(p):
    b=bytearray()
    while (v:=uc.mem_read(p,1)[0]):b.append(v);p+=1
    return bytes(b)
   self.put(0,0 if string(self.reg(0))==string(self.reg(1)) else 1);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  super().external(uc,address,size,unused)
base.TimelineCpu=OptionsCpu
MEMBERS.extend(['NumOptions','CurrentOption','OptionString'])
class Options(Machine):
 def __init__(self,path,native,manifest):
  super().__init__(path,native,manifest);c=self.c
  self.flag=c.data+0x11000;self.language=c.data+0x11008
  if not native:
   c.imports.pop(0x30e31c,None)
   got=0x44a2b0+word(c,0x44a598)
   c.pointer(got+word(c,0x44a5a0),self.app)
   c.pointer(got+word(c,0x44a5a4),self.flag)
   c.pointer(got+word(c,0x44a5b8),self.language)
 def deliver(self,op,i=0,v=0,other=0,t=0,subject=0,obj=0,text='',name='',number=0.):
  c=self.c;a=self.v
  if op==30:return 0,self.store(self.key),0.,0,0.
  if op in(31,32,33,34,35,36):
   self.event(op,i,v,other,t,subject,obj,text,name,number)
   value=a[{31:4,32:5,33:6,34:7,36:9}.get(op,4)]
   if op==34:value=c.uc.mem_read(self.flag,1)[0]
   if op==35:c.uc.mem_write(self.language,words(v))
   return 0,0,0.,sint(value),0.
  if op==24:
   self.event(op,i,v,other,t,subject,obj,text,name,number);self.writes.append((MEMBERS[i],t,v,text))
   if i==11 and a[8]:c.uc.mem_write(self.flag,bytes((1-c.uc.mem_read(self.flag,1)[0],)))
   return 0,0,0.,0,0.
  if op==18:
   self.event(op,i,v,other,t,subject,obj,text,name,number)
   return 0,self.store('Option['+str(v)+']') if v>=0 else 0,0.,0,0.
  if op==4:return self.object if a[3] and a[2]==5 else 0,0,0.,0,0.
  return super().deliver(op,i,v,other,t,subject,obj,text,name,number)
 def callback(self,address):
  if self.native:return super().callback(address)
  c=self.c;name=self.keys[c.reg(1)];i=MEMBERS.index(name);p=c.reg(2);t=c.uc.mem_read(p+1,1)[0]
  text=self.text(word(c,p+4)) if t==3 else '';v=int(struct.unpack('<d',c.uc.mem_read(p+4,8))[0]) if t==2 else 0
  self.deliver(24,i,v,0,t,0,2,text,name,float(v));c.put(0,1)
 def hook(self,uc,address,size,unused):
  if self.native:return
  c=self.c;r=c.reg
  if address==0x796fb4:self.ret(self.store(self.key));return
  if address==0x439cb4:self.ret(self.object if self.v[3] and r(0) else 0);return
  if address in(0x46d474,0x46d330,0x46d2b8):
   op={0x46d474:31,0x46d330:32,0x46d2b8:33}[address];self.ret(self.deliver(op,text=self.text(r(1)))[3]);return
  if address in(0x44a368,0x44a4dc):self.event(34)
  if address==0x44a508:self.event(35,v=sint(r(10)))
  if address==0x533570:self.ret(self.deliver(36)[3]);return
  if address==0x30e31c:self.ret(0 if self.text(r(0))==self.text(r(1)) else 1);return
  super().hook(uc,address,size,unused)
 def run(self,a):
  c=self.c;self.v=a[:];self.events=[];self.writes=[];self.result=None;self.appended=[];self.keys={};self.texts={};self.args0=c.data+0x10000;self.args1=self.args0+16;self.nbase=3;self.key=['HUDStyle','Language','language','missing'][a[1]]
  c.uc.mem_write(self.flag,bytes((a[7],)));c.uc.mem_write(self.language,words(77))
  if self.native:
   c.uc.mem_write(self.inp,struct.pack('<QII',self.fn,2 if a[0]==3 else 0,2));c.uc.mem_write(self.svc,struct.pack('<QQ',0,c.callback+32));rc=c.invoke('dh2_ui_hud_initialization_v1',[self.inp,a[0],self.svc]);assert rc==0,rc
  else:
   c.pointer(self.env,self.args);c.uc.mem_write(self.args+36,bytes(12));c.uc.mem_write(self.args+24,bytes(12));c.uc.mem_write(self.args+25,bytes((a[2],)));c.pointer(self.args+28,self.object if a[2]==5 else 0)
   c.uc.mem_write(self.fn,bytes(24));c.pointer(self.fn,self.env+0x200);c.pointer(self.fn+12,self.env);c.pointer(self.fn+16,2 if a[0]==3 else 0);c.pointer(self.fn+20,3)
   c.invoke(0x44a298 if a[0]==3 else 0x43a974,[self.fn])
  return tuple(self.events),self.result,tuple(self.writes),word(c,self.language)
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/hud-initialization-v1-oracle.so');a=p.parse_args();manifest=json.loads((REF/'option-ipod/original-functions.json').read_text());old=Options(REPO/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Options(a.library,True,{'functions':[]});rng=random.Random(20261006);records=[];services=0
 for n in range(512):
  v=[3 if n<448 else 4,rng.randrange(4),rng.choice((0,1,2,5)),rng.randrange(2),rng.choice((-2147483648,-1,0,1,3,2147483647)),rng.choice((-1,0,3,9)),rng.choice((-1,0,100,2147483647)),rng.randrange(2),rng.randrange(2),rng.choice((-1,0,1,2,255))]
  if n%7==0:v[6]=(v[4]-1)&0xffffffff
  x=old.run(v);y=new.run(v)
  if x!=y:print('CASE',n,v,'OLD',x,'NEW',y);raise AssertionError('option wrapper mismatch')
  services+=len(x[0]);records.append(words(len(v),len(x[0]),x[3])+words(*v)+b''.join(x[0]))
 gold=REF/'options-gold.bin';gold.write_bytes(words(0x314f4948,len(records))+b''.join(records));report=dict(validation='PASS',comparisons=len(records),ordered_services=services,mismatches=0,original_sha256=manifest['original_sha256'],arm64_sha256=sha(a.library),gold_sha256=sha(gold),source_sha256={f.name:sha(f) for f in (ROOT/'hud_initialization_v1.cpp',ROOT/'hud_initialization_v1.hpp')},scope=__doc__);(ROOT/'reports/hud-initialization-options-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
