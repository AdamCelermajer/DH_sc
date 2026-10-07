"""Complete three original HUD initialization wrappers versus O2 native.
Argument/AS casts and writes, saved getters, localization formatting and skill
VCB endpoints are explicit fixture imports. Actual wrapper branches/order and
explicit synthetic Skill76 projections execute; no genuine saved-session claim.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/hud-initialization-v1'
sys.path.insert(0,str(REPO/'port/level-world/tests'))
from visual_timeline_differential import TimelineCpu,integer
from navigation_search_differential import word
from aggro_differential import float_bits
def words(*v):return struct.pack('<'+'I'*len(v),*(int(x)&0xffffffff for x in v))
def sint(x):return x if x<0x80000000 else x-0x100000000
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def token(s):
 h=2166136261
 for b in s.encode():h=((h^b)*16777619)&0xffffffff
 return h
MEMBERS=['SkillName','SkillDescription','SkillCurrLevel','SkillNextLevel','SkillAssignable','SkillIcon','SkillLevel','SkillAssignedToSlot','SkillUnlocked','Id','Upgraded']
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.inp=d+0x1000;self.svc=d+0x2000;self.actor=d+0x3000;self.object=d+0x7000;self.vt=d+0x8000;self.skill=d+0x9000;self.props=d+0xa000;self.fn=d+0xb000;self.env=d+0xc000;self.args=d+0xd000;self.strings=d+0x20000;self.app=d+0xe000
  c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if not native:
   c.pointer(self.object,self.vt);c.pointer(self.vt+0x1c,c.callback+32)
   # Original application's pointer is loaded through this exact GOT entry.
   got=0x445a28+word(c,0x446648);offset=word(c,0x446658);c.pointer(got+offset,self.app)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def ptr(self,i):return [0,self.actor,self.object,self.fn,self.args0,self.args1,self.skill][i]
 def identity(self,p):
  return {self.actor:1,self.object:2,self.fn:5,self.args0:3,self.args1:4,self.skill:6}.get(p,0)
 def text(self,p):
  if not p:return ''
  b=bytearray()
  while (x:=self.c.uc.mem_read(p,1)[0]):b.append(x);p+=1
  return b.decode()
 def store(self,s):
  if s not in self.texts:
   p=self.strings+len(self.texts)*512;self.c.uc.mem_write(p,s.encode()+b'\0');self.texts[s]=p
  return self.texts[s]
 def event(self,op,i=0,v=0,other=0,t=0,subject=0,obj=0,text='',name='',number=0.):
  self.events.append(words(op,i,v,other,t,subject,obj,token(text) if text else 0,token(name) if name else 0)+struct.pack('<d',number))
 def deliver(self,op,i=0,v=0,other=0,t=0,subject=0,obj=0,text='',name='',number=0.):
  c=self.c;a=self.v
  if op not in(1,2,3,4,5,29):self.event(op,i,v,other,t,subject,obj,text,name,number)
  value=0;identity=0;outtext='';fraction=0.;outnumber=0.
  if op==1:value=a[2+i];identity=self.object if value==5 else 0
  elif op==2:outnumber=float(sint(a[6+i]))
  elif op==3:value=bool(a[9] if i==3 else a[6+i])
  elif op in(4,5):identity=self.object if a[11] and a[2+i]==5 else 0
  elif op==29:value=a[2+i]==2
  elif op==6:identity=self.actor if a[10] else 0
  elif op==7:value=sint(a[33] if other else a[30+i])
  elif op==8:value=123
  elif op==9:identity=self.skill
  elif op==10:
   value=sint(a[12])
   if a[34]&1:
    a[13]=(a[13]+1)&0xffffffff;c.uc.mem_write(self.skill+(16 if self.native else 32),words(a[13]))
  elif op==11:value=sint(a[14])
  elif op==12:value=sint(a[15])
  elif op==13:value=a[16]
  elif op==14:value=sint(a[17])
  elif op==15:value=sint(a[19])
  elif op==16:value=sint(a[18])
  elif op==17:value=5 if text=='CharacterDesign' else 1000+list(('GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL','GAMEPLAYMENUS_NEEDS_SKILL_POINTS','GAMEPLAYMENUS_MAX_SKILL_LEVEL','GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING')).index(name)
  elif op==18:outtext=f'String[{v}]'
  elif op==19:identity=self.args0 if i==0 else self.args1
  elif op==20:self.variants[subject].append((v,number))
  elif op==21:
   fraction=.25
   if a[34]&4:a[27]^=0x1234abcd;c.uc.mem_write(self.props,words(*a[27:30]))
  elif op==22:value=sint(a[27+(i%3)])
  elif op==23:outtext=text+'|'+','.join(f'{x}:{y:g}' for x,y in self.variants[subject])
  elif op==24:
   self.writes.append((MEMBERS[i],t,v,text))
   if a[34]&2 and i==0:
    a[21]=0xffffffff;a[25]=0;c.uc.mem_write(self.skill+(24 if self.native else 52),words(a[21]));c.uc.mem_write(self.skill+(37 if self.native else 44),bytes((0,)))
  elif op==25:self.appended.append(v)
  elif op==26:self.result=('boolean',v)
  elif op==27:self.result=('number',v)
  elif op==28:self.result=('object',obj)
  else:raise AssertionError(op)
  return identity,self.store(outtext) if outtext else 0,outnumber,value,fraction
 def callback(self,address):
  c=self.c
  if self.native:
   op,i,v,other,t,res,subject,obj,text,name,number=struct.unpack('<IIiiIIQQQQd',c.uc.mem_read(c.reg(1),64));assert not res
   identity,p,num,value,f=self.deliver(op,i,v,other,t,self.identity(subject),self.identity(obj),self.text(text),self.text(name),number)
   c.uc.mem_write(c.reg(2),struct.pack('<QQdi f',identity,p,num,value,f));c.put(0,1)
  else:
   name=self.keys[c.reg(1)];i=MEMBERS.index(name);p=c.reg(2);t=c.uc.mem_read(p+1,1)[0]
   text=self.text(word(c,p+4)) if t==3 else '';v=c.uc.mem_read(p+4,1)[0] if t==1 else int(struct.unpack('<d',c.uc.mem_read(p+4,8))[0]) if t==2 else 0
   self.deliver(24,i,v,0,t,1,2,text,name,float(v));c.put(0,1)
 def hook(self,uc,address,size,unused):
  if self.native:return
  c=self.c;r=c.reg
  def get(op,i=0,v=0,other=0,t=0,subject=0,obj=0,text='',name='',number=0.):return self.deliver(op,i,v,other,t,subject,obj,text,name,number)
  if address==0x445adc:
   get(19,0);get(19,1);self.args0=r(13)+0x54 if False else c.uc.reg_read(c.sp)+0x54;self.args1=self.args0+16
  if address in(0x439cb4,0x439ce8):self.ret(self.object if self.v[11] and r(0) else 0)
  elif address==0x439d8c:self.ret(self.v[3]==2)
  elif address in(0x797a54,0x797960):
   i=(r(0)-self.args)//12;i=self.nbase-i
   if address==0x797a54:
    lo,hi=struct.unpack('<2I',struct.pack('<d',float(sint(self.v[6+i]))));c.put(0,lo);c.put(1,hi);uc.reg_write(c.pc,uc.reg_read(c.lr))
   else:self.ret(bool(self.v[9] if i==3 else self.v[6+i]))
  elif address==0x30ea24:self.ret(integer(struct.unpack('<d',words(r(0),r(1)))[0]))
  elif address==0x30ed30:
   lo,hi=struct.unpack('<2I',struct.pack('<d',float(sint(r(0)))));c.put(0,lo);c.put(1,hi);uc.reg_write(c.pc,uc.reg_read(c.lr))
  elif address==0x30e964:self.ret(float_bits(float(sint(r(0)))))
  elif address==0x30ed6c:
   x,y=(struct.unpack('<f',words(r(i)))[0] for i in range(2));self.ret(float_bits(x*y))
  elif address==0x43c388:self.ret(get(6,0,sint(r(0)),sint(r(1)))[0])
  elif address==0x3bbe68:self.ret(get(7,r(1),0,0,0,1)[3])
  elif address==0x3bbe84:self.ret(get(7,r(1),0,1,0,1)[3])
  elif address==0x3bbeec:self.ret(get(8,r(1),0,0,0,1)[3])
  elif address==0x3bc784:self.ret(get(9,r(1),0,0,0,1)[0])
  elif address==0x3bd120:self.ret(get(10,0,0,0,0,1)[3])
  elif address==0x3bbed0:self.ret(get(11,r(1),0,0,0,1)[3])
  elif address==0x3bb918:self.ret(get(12,0,0,0,0,1)[3])
  elif address==0x3bc9ec:self.ret(get(13,r(1),0,0,0,1)[3])
  elif address==0x3bb98c:self.ret(get(14,0,sint(r(1)),0,0,1)[3])
  elif address==0x3aeac0:get(15,r(1),0,0,0,1);c.uc.mem_write(self.skill+0x200+8,words(self.v[19]));self.ret(self.skill+0x200)
  elif address==0x3bbc18:self.ret(get(16,r(1),sint(r(2)),0,0,1)[3])
  elif address==0x4c4bdc:self.ret(get(17,text=self.text(r(1)),name=self.text(r(2)))[3])
  elif address==0x508edc:self.ret(get(18,v=sint(r(1)))[1])
  elif address==0x3d7e88:c.uc.mem_write(r(3),struct.pack('<f',get(21,r(1),sint(r(2)),0,0,1)[4]));self.ret()
  elif address==0x3dedb4:self.ret(get(22,r(2),0,0,0,1)[3])
  elif address==0x509aec:
   text=self.text(r(2));args=3 if r(3)==self.args0 else 4;output=get(23,subject=args,text=text)[1];c.uc.mem_write(r(1)+16,words(output,output));self.ret()
  elif address in(0x3140ec,0x31167c):
   p=self.store('') if address==0x31167c else self.store(self.text(r(1)));c.uc.mem_write(r(0)+16,words(p,p));self.ret(r(0))
  elif address in(0x3109e0,0x33076c):
   p=self.store(self.text(r(1)));c.uc.mem_write(r(0)+16,words(p,p));self.ret(r(0))
  elif address==0x30de54:self.ret(len(self.text(r(0))))
  elif address==0x43f414:
   # Source VarArgs vector append: fixture allocation with exact payload.
   vec=r(0);variant=r(2);base=self.array_memory+(vec%256)*1024;n=self.vector_counts.get(vec,0);self.vector_counts[vec]=n+1;c.uc.mem_write(base+n*12,bytes(c.uc.mem_read(variant,12)));c.uc.mem_write(vec,words(base,base+(n+1)*12,base+(n+1)*12));self.ret()
  elif address in(0x445c98,0x446470):
   pass
  elif address==0x413a7c:self.keys[r(0)]=self.text(r(1));c.uc.mem_write(r(0),bytes(20));self.ret(r(0))
  elif address==0x797350:c.uc.mem_write(r(0)+1,b'\3');c.pointer(r(0)+4,r(1));self.ret()
  elif address in(0x797124,0x3139ac,0x752b38,0x4460f0,0x446100):self.ret()
  elif address==0x799134:
   v=int(struct.unpack('<d',c.uc.mem_read(r(1)+4,8))[0]);get(25,len(self.appended),v,0,2,1,2,number=float(v));self.ret()
  elif address==0x797230:get(26,v=r(1),t=1,subject=5);self.ret()
  elif address==0x797488:get(27,v=int(struct.unpack('<d',words(r(2),r(3)))[0]),t=2,subject=5,number=struct.unpack('<d',words(r(2),r(3)))[0]);self.ret()
  elif address==0x797250:get(28,t=5,subject=5,obj=2 if r(1) else 0);self.ret()
  # Notify source argument appends at the exact instruction following variant
  # installation. It also handles source in-capacity append without import.
  if address==0x445c98 and not self.low_recorded:
   if sint(self.v[12])<sint(self.v[13]):get(20,v=sint(self.v[13]),subject=3,number=float(sint(self.v[13])));self.low_recorded=True
  if address==0x446414:
   f=struct.unpack('<f',words(r(0)))[0];get(20,v=sint(r(10)),subject=3 if word(c,c.uc.reg_read(c.sp)+12)==0 else 4,number=float(f))
 def run(self,v):
  c=self.c;self.v=v[:];self.events=[];self.writes=[];self.appended=[];self.result=None;self.keys={};self.texts={};self.variants={3:[],4:[]};self.vector_counts={};self.low_recorded=False;self.array_memory=c.data+0x40000;self.args0=c.data+0x10000;self.args1=self.args0+16;self.nbase=3
  c.uc.mem_write(self.props,words(*v[27:30]));icon=self.store('authored_icon')
  if self.native:
   # Exact native 96-byte view and pinned dynamic payload.
   c.uc.mem_write(self.skill,struct.pack('<QII5iBBHQQ5Q',self.props,v[26],0,*map(sint,(v[13],v[22],v[21],v[20],v[23])),v[24],v[25],0,icon,123,0,0,0,0,0));c.uc.mem_write(self.inp,struct.pack('<QII',self.fn,v[1],4));c.uc.mem_write(self.svc,struct.pack('<QQ',0,c.callback+32));rc=c.invoke('dh2_ui_hud_initialization_v1',[self.inp,v[0],self.svc]);assert rc==0,rc
  else:
   c.uc.mem_write(self.skill,bytes(76));c.uc.mem_write(self.skill+12,words(v[26],self.props));c.uc.mem_write(self.skill+24,bytes((v[24],)));c.uc.mem_write(self.skill+32,words(v[13]));c.uc.mem_write(self.skill+44,bytes((v[25],)));c.uc.mem_write(self.skill+48,words(v[22],v[21]));c.pointer(self.skill+60,icon);c.uc.mem_write(self.skill+64,words(v[20],v[23]));c.pointer(self.env,self.args)
   for i in range(4):p=self.args+(3-i)*12;c.uc.mem_write(p,bytes(12));c.uc.mem_write(p+1,bytes((v[2+i],)));c.pointer(p+4,self.object if v[2+i]==5 else 0)
   c.uc.mem_write(self.fn,bytes(24));c.pointer(self.fn,self.env+0x200);c.pointer(self.fn+12,self.env);c.pointer(self.fn+16,v[1]);c.pointer(self.fn+20,3);c.invoke((0x4425a0,0x445a10,0x44a820)[v[0]],[self.fn])
  return tuple(self.events),self.result,self.writes,self.appended
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/hud-initialization-v1-oracle.so');p.add_argument('--cases',type=int,default=450);a=p.parse_args();manifest=json.loads((REF/'original-functions.json').read_text());old=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Machine(a.library,True,{'functions':[]});rng=random.Random(20261005);counts=[0,0,0];events=0;records=[]
 for n in range(a.cases):
  v=[n%3,rng.choice((0,1,2,3,4,5)),5,2,2,1,0,0,0,0,1,1,20,1,1,0,1,0,0,100,100,101,102,103,0,1,2,256,-257,100,0,1,2,0,0]
  if v[0]==0:v[2:6]=[rng.choice((0,1,2,3,5)),rng.choice((0,1,2,3,5)),rng.choice((0,1,2,5)),1]
  if v[0]==1:v[2:6]=[2,5,2,1];v[1]=rng.choice((3,4));v[12]=rng.choice((-5,0,1,20,100));v[13]=rng.choice((0,1,20,50));v[14]=rng.choice((-1,0,1,5,10));v[15]=rng.choice((-1,0,1,2,3));v[16]=rng.randrange(2);v[20:24]=[rng.choice((-1,0,102,500)) for _ in range(4)];v[24]=rng.randrange(2);v[25]=rng.choice((0,1,255));v[26]=rng.randrange(4);v[27:30]=[rng.choice((-2147483648,-257,-1,0,256,2147483647)) for _ in range(3)]
  if v[0]==2:v[2:6]=[2,rng.choice((0,1,2,5)),2,1];v[17]=rng.choice((-1,0,1,9));v[18]=rng.choice((-1,0,1,65535))
  v[9]=rng.randrange(2);v[10]=rng.randrange(2) if n<90 else 1;v[11]=1;v[6:9]=[rng.randrange(3) for _ in range(3)];v[30:33]=[rng.choice((-1,0,1,7,11)) for _ in range(3)];v[33]=rng.choice((-1,0,1,2));v[34]=rng.randrange(8)
  expected=old.run(v);actual=new.run(v)
  if expected!=actual:
   print('CASE',n,v);print('EXPECTED',expected);print('ACTUAL',actual);raise AssertionError('wrapper mismatch')
  counts[v[0]]+=1;events+=len(expected[0]);records.append(words(len(v),len(expected[0]))+words(*v)+b''.join(expected[0]))
 gold=REF/'wrappers-gold.bin';gold.write_bytes(words(0x314e4948,len(records))+b''.join(records));report=dict(validation='PASS',comparisons=len(records),entry_counts=counts,ordered_services=events,mismatches=0,original_sha256=manifest['original_sha256'],arm64_sha256=sha(a.library),gold_sha256=sha(gold),source_sha256={p.name:sha(p) for p in (ROOT/'hud_initialization_v1.cpp',ROOT/'hud_initialization_v1.hpp')},scope=__doc__);(ROOT/'reports/hud-initialization-v1-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()

