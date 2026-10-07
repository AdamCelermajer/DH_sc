"""Complete original four-slot input coordination; graph/virtual endpoints explicit."""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
from unicorn.arm64_const import UC_ARM64_REG_S0
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'tests'),str(ROOT/'../level-world/tests')]
from swf_event_dispatch_differential import EventCpu,string,words,sha,METHODS
from body_transform_differential import equal
from navigation_search_differential import word
NAMES=['root','btn_A','btnDelete','ordinary','btn_Legend','hitzone','btn_deadzone','disabled']
LABELS=['focus_out','focus_in','pressed','released','clicked']
def f32(x):
 try:return struct.unpack('<f',struct.pack('<f',x))[0]
 except OverflowError:return math.copysign(math.inf,x)
def floats(p):return struct.unpack('<'+'f'*(len(p)//4),p)
class InputCpu(EventCpu):
 def external(self,uc,address,size,unused):
  if self.callback+32<=address<self.callback+256:
   self.handler(address)
   if uc.reg_read(self.pc)==address:uc.reg_write(self.pc,uc.reg_read(self.lr))
   return
  name=self.imports.get(address)
  if name=='__aeabi_fdiv':
   a,b=floats(words(self.reg(0),self.reg(1)));v=a/b if b else math.nan if not a else math.copysign(math.inf,a*b if b else math.copysign(1,a)*math.copysign(1,b));self.put(0,struct.unpack('<I',struct.pack('<f',f32(v)))[0]);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name=='sincosf':
   a=floats(words(uc.reg_read(UC_ARM64_REG_S0)))[0]
   ss=f32(math.sin(a))if math.isfinite(a)else math.nan;cc=f32(math.cos(a))if math.isfinite(a)else math.nan
   uc.mem_write(self.reg(0),struct.pack('<f',ss));uc.mem_write(self.reg(1),struct.pack('<f',cc));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name in ('__aeabi_fcmple','__aeabi_fcmpge','__aeabi_fcmplt','__aeabi_fcmpgt','__aeabi_fcmpeq'):
   a,b=floats(words(self.reg(0),self.reg(1)));self.put(0,int({'__aeabi_fcmple':a<=b,'__aeabi_fcmpge':a>=b,'__aeabi_fcmplt':a<b,'__aeabi_fcmpgt':a>b,'__aeabi_fcmpeq':a==b}[name]));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name=='__aeabi_f2iz':
   a=floats(words(self.reg(0)))[0];result=0 if math.isnan(a) else max(-2147483648,min(2147483647,int(a))) if math.isfinite(a) else 2147483647 if a>0 else -2147483648
   self.put(0,result&0xffffffff);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  if name=='strstr':
   a,b=string(self,self.reg(0)),string(self,self.reg(1));i=a.find(b);self.put(0,self.reg(0)+i if i>=0 else 0);uc.reg_write(self.pc,uc.reg_read(self.lr));return
  return super().external(uc,address,size,unused)
class Machine:
 def __init__(self,p,native):
  c=self.c=InputCpu(p,native,{'functions':[]});self.native=native;d=c.data
  self.state=d+0x1000;self.root=d+0x3000;self.owner=d+0x4000;self.vt=d+0x5000;self.services=d+0x6000;self.global_=d+0x7000;self.views=d+0x8000;self.list=d+0x9000;self.cursor=d+0xa000
  self.ch=[d+0x10000+j*0x1000 for j in range(8)];self.names=[d+0x20000+j*0x100 for j in range(8)];self.matrix=[d+0x30000+j*0x100 for j in range(8)]
  for a,n in zip(self.names,NAMES):c.uc.mem_write(a,n.encode()+b'\0')
  c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if native:c.uc.mem_write(self.services,struct.pack('<QQ',0,c.callback+32))
  else:
   c.pointer(self.owner,self.vt);c.pointer(self.vt,c.callback+96);c.pointer(self.vt+8,c.callback+80)
   for ch,n,m in zip(self.ch,self.names,self.matrix):
    c.pointer(ch,self.vt+0x100);c.pointer(ch+0x44,n+0x80);c.uc.mem_write(n+0x80,b'\0'+string(c,n)+b'\0');c.pointer(ch+0x4c,m)
   for off,cb in [(8,48),(0x68,64),(0x94,144),(0x98,160),(0x9c,128)]:c.pointer(self.vt+0x100+off,c.callback+cb)
   c.pointer(self.root,self.vt+0x100)
   table=(0x7abf6c+word(c,0x7ac1e4))&0xffffffff;c.pointer(table+word(c,0x7ac1f0),self.global_)
 def id(self,p):return 0 if not p else 9 if p==self.root else self.ch.index(p)+1
 def ptr(self,k):return 0 if not k else self.root if k==9 else self.ch[k-1]
 def slot(self,i):return self.state+i*64 if self.native else self.state+0x58+i*40
 def field(self,i,k):return self.slot(i)+16+k*(8 if self.native else 4)
 def flags(self):return word(self.c,self.state+(280 if self.native else 0xf8))
 def read_event(self,a):
  c=self.c;w=8 if self.native else 4;ch=self.id(struct.unpack('<Q'if self.native else'<I',c.uc.mem_read(a,w))[0]);n=struct.unpack('<Q'if self.native else'<I',c.uc.mem_read(a+w,w))[0]
  # Original constructors initialize consumed/flag, not trailing struct pad.
  tail=bytearray(c.uc.mem_read(a+2*w,32));tail[30:32]=b'\0\0'
  return words(ch,NAMES.index(string(c,n).decode()))+tail
 def mutate(self,a):
  c=self.c;off=16 if self.native else 8;kind=word(c,a+off)
  if self.action==1 and kind in(3,4,6):c.uc.mem_write(a+off+28,b'\1')
  if self.action==2 and kind==6:c.uc.mem_write(a+off,words(2));c.pointer(a+(8 if self.native else 4),self.names[2])
  if self.action==3 and kind in(0,8):c.uc.mem_write(self.state+(280 if self.native else 0xf8),words(self.flags()^64))
 def native_event(self,a):
  self.trace.append(words(15)+self.read_event(a));self.mutate(a)
  if self.action==4 and not self.depth and word(self.c,a+(16 if self.native else 8))==4:self.reenter_pending=word(self.c,a+(40 if self.native else 32))
 def redirect(self):
  if self.reenter_pending is None:return
  self.reentries+=1;c=self.c;index=self.reenter_pending;self.reenter_pending=None;self.depth=1;self.outer_lr=c.uc.reg_read(c.lr)
  c.put(0,self.state);c.put(1,index)
  if self.native:c.put(2,self.global_);c.put(3,self.services)
  c.uc.reg_write(c.lr,c.callback+224);c.uc.reg_write(c.pc,c.symbols['dh2_ui_swf_reset_focus']if self.native else 0x7ac410)
 def can(self,a):self.trace.append(words(14)+self.read_event(a));return int(self.accept)
 def play(self,ch,label):
  # Native service executes original GotoFrame's class/label/play-state policy.
  if not self.traits[self.id(ch)-1][0]:return 0
  self.trace.append(words(13,self.id(ch),LABELS.index(label)))
  yes=bool(self.labels&(1<<LABELS.index(label)))
  if yes:self.trace.append(words(21,self.id(ch),0));self.playing[self.id(ch)]=0
  return int(yes)
 def projection(self,ch):
  j=self.id(ch)-1;a=self.views+j*64;sp,vis,en,mouse=self.traits[j]
  self.c.uc.mem_write(a,struct.pack('<QI4BQQ',self.names[j],sp,vis,en,mouse,0,0,0));return a
 def callback(self,address):
  c=self.c
  if address==c.callback+224:
   self.depth=0;c.put(0,1);c.uc.reg_write(c.lr,self.outer_lr);return
  if self.native:
   q=c.reg(2);out=c.reg(3);op,i,ch,n,e=struct.unpack('<IIQQQ',c.uc.mem_read(q,32));v=floats(bytes(c.uc.mem_read(q+32,24)));integer=struct.unpack('<i',c.uc.mem_read(q+56,4))[0];result=0;identity=0;data=[0.]*6;view=ls=0;count=0
   if op in(1,2):self.trace.append(words(op,self.id(ch)));self.refs[self.id(ch)]+=1 if op==1 else-1
   elif op==3:view=self.projection(ch)
   elif op==4:data=list(self.matrices[self.id(ch)-1])
   elif op==5:
    m=self.matrices[self.id(ch)-1];data[:2]=[f32(f32(v[0]*20)-m[2]),f32(f32(v[1]*20)-m[5])]
   elif op==6:data[:2]=v[:2]
   elif op==7:self.raw=(v[0],v[1],i)
   elif op==8:self.mouse=(int(v[0]),int(v[1]),integer)
   elif op==9:identity=self.ch[0]
   elif op==10:self.trace.append(words(10,self.id(ch)));identity=self.ptr(self.hit)
   elif op==11:self.trace.append(words(11,self.id(ch))+struct.pack('<6f',*v))
   elif op==12:
    self.trace.append(words(12,self.id(ch)));ls=self.list;count=len(self.buttons);c.uc.mem_write(ls,b''.join(struct.pack('<Q',self.ptr(j))for j in self.buttons))
   elif op==13:result=self.play(ch,string(c,n).decode())
   elif op==14:result=self.can(e)
   elif op==15:self.native_event(e)
   elif op==16:self.trace.append(words(16,self.id(ch),METHODS.index(string(c,n).decode())))
   elif op==17:self.trace.append(words(17,self.id(ch)));result=self.playing[self.id(ch)]
   elif op==18:self.trace.append(words(18,integer)+struct.pack('<f',v[0]))
   else:raise AssertionError(op)
   c.uc.mem_write(out,struct.pack('<QQQ6fii',view,ls,identity,*data,count,result));c.put(0,1);self.redirect();return
  cb=address-c.callback
  if cb==48:c.put(0,self.traits[self.id(c.reg(0))-1][0])
  elif cb==64:self.trace.append(words(10,self.id(c.reg(0))));c.put(0,self.ptr(self.hit))
  elif cb==80:c.put(0,self.can(c.reg(1)))
  elif cb==96:self.native_event(c.reg(1))
  elif cb==128:
   a=c.reg(1);label=string(c,a+1 if c.uc.mem_read(a,1)!=b'\xff'else word(c,a+12)).decode();self.trace.append(words(13,self.id(c.reg(0)),LABELS.index(label)));c.put(0,int(bool(self.labels&(1<<LABELS.index(label)))))
  elif cb==144:self.trace.append(words(21,self.id(c.reg(0)),c.reg(1)));self.playing[self.id(c.reg(0))]=c.reg(1)
  elif cb==160:self.trace.append(words(17,self.id(c.reg(0))));c.put(0,self.playing[self.id(c.reg(0))])
  else:raise AssertionError(cb)
  self.redirect()
 def hook(self,uc,a,size,unused):
  if self.native:return
  c=self.c;ret=True
  if a in(0x759c64,0x75a240):
   op=1 if a==0x759c64 else 2;self.trace.append(words(op,self.id(c.reg(0))));self.refs[self.id(c.reg(0))]+=1 if op==1 else-1;return
  elif a==0x753f74:c.put(0,self.matrix[self.id(c.reg(0))-1])
  elif a==0x773dc0:return # Hook exit below publishes an identity mapping provider.
  elif a==0x773dc4:
   uc.reg_write(c.pc,uc.reg_read(c.lr));return
  elif a==0x774128:self.mouse=(struct.unpack('<i',words(c.reg(1)))[0],struct.unpack('<i',words(c.reg(2)))[0],c.reg(3))
  elif a==0x4121f8:self.trace.append(words(11,self.id(c.reg(0)))+bytes(c.uc.mem_read(c.reg(1),24)))
  elif a==0x7a8c08:
   self.trace.append(words(12,self.id(c.reg(1))));c.uc.mem_write(self.list,words(*[self.ptr(j)for j in self.buttons]));c.uc.mem_write(self.state+4,words(self.list,len(self.buttons),16));c.put(0,self.state+4)
  elif a==0x7abe0c:self.trace.append(words(16,self.id(c.reg(1)),METHODS.index(string(c,c.reg(2)).decode())));c.put(0,1)
  elif a==0x76d5b4:c.put(0,self.root)
  elif a==0x775304:self.trace.append(words(18,c.reg(2),c.reg(1)))
  elif a==0x413a7c:
   # Exact short-label tu_string constructor result; allocation is fixture.
   c.uc.mem_write(c.reg(0),b'\0'+string(c,c.reg(1))+b'\0')
  else:ret=False
  if ret:uc.reg_write(c.pc,uc.reg_read(c.lr))
 def snapshot(self):
  c=self.c;parts=[]
  for i in range(4):
   a=self.slot(i);parts.append(bytes(c.uc.mem_read(a,16)));parts.append(words(*[self.id(struct.unpack('<Q'if self.native else'<I',c.uc.mem_read(self.field(i,k),8 if self.native else 4))[0])for k in range(5)]));parts.append(words(c.uc.mem_read(a+(56 if self.native else 36),1)[0]))
  raw=struct.pack('<2fI',*self.raw)if self.native else bytes(c.uc.mem_read(self.root+0x48,12))
  return raw+words(*self.mouse)+b''.join(parts)+words(self.flags(),word(c,self.global_),*self.refs[1:])+words(*[self.playing[j]for j in range(1,9)])
 def run(self,row):
  c=self.c;self.trace=[];self.depth=0;self.reenter_pending=None;self.reentries=0;c.uc.mem_write(self.root+0x48,bytes(12));self.raw=(0.,0.,0);self.mouse=(0,0,0);self.refs=[0]+[1000]*9;self.playing={j:int(row['stop']&(1<<(j-1))!=0)for j in range(1,9)};self.action=row['action'];self.accept=row['accept'];self.hit=row['hit'];self.labels=row['labels'];self.buttons=row['buttons'];self.traits=row['traits'];self.matrices=row['matrices']
  if self.native:c.uc.mem_write(self.state,bytes(288));c.uc.mem_write(self.state+256,struct.pack('<QQQII',self.root,self.ch[0]if row['context']else 0,self.owner,row['flags'],0))
  else:c.uc.mem_write(self.state,bytes(0x110));c.pointer(self.state+0x38,self.owner);c.pointer(self.state+0x3c,self.root);c.pointer(self.state+0x40,self.ch[0]if row['context']else 0);c.pointer(self.state+0xfc,self.owner);c.uc.mem_write(self.state+0xf8,words(row['flags']));c.pointer(self.root+0x10,self.ch[0])
  for j,ch in enumerate(self.ch):
   c.uc.mem_write(ch+4,words(1000));c.uc.mem_write(self.matrix[j],struct.pack('<6f',*self.matrices[j]));c.uc.mem_write(ch+0x9b,bytes(self.traits[j][1:3]));c.uc.mem_write(ch+0xea,bytes([self.traits[j][3]]))
  c.uc.mem_write(self.root+4,words(1000));c.uc.mem_write(self.global_,words(row['selection']))
  for i,(xy,fields,en)in enumerate(row['slots']):
   c.uc.mem_write(self.slot(i),struct.pack('<3fi',*xy));
   for k,x in enumerate(fields):c.pointer(self.field(i,k),self.ptr(x))
   c.uc.mem_write(self.slot(i)+(56 if self.native else 36),bytes([en]))
  operation=row['operation'];i=row['index'];c.uc.mem_write(self.cursor,struct.pack('<3fi',*row['next']))
  if operation==0:args=[self.state,self.cursor,i,self.global_,self.services]if self.native else[self.state,self.cursor,i];name='dh2_ui_swf_update_cursor'if self.native else 0x7ac924
  elif operation==1:args=[self.state,row['mask'],i,self.global_,self.services]if self.native else[self.state,row['mask'],i];name='dh2_ui_swf_update_input'if self.native else 0x7ac4bc
  elif operation==2:args=[self.state,self.ptr(row['hit']),i,self.global_,self.services]if self.native else[self.state,self.ptr(row['hit']),i];name='dh2_ui_swf_set_focus'if self.native else 0x7ac228
  elif operation==3:args=[self.state,i,self.global_,self.services]if self.native else[self.state,i];name='dh2_ui_swf_reset_focus'if self.native else 0x7ac410
  else:args=[self.state,row['mask'],row['advance'],self.global_,self.services]if self.native else[self.state,row['mask'],row['advance']];name='dh2_ui_swf_update_pending'if self.native else 0x7ad68c
  result=c.invoke(name,args)
  if self.native:assert result==0,(operation,result)
  return self.snapshot(),self.trace
def main():
 p=argparse.ArgumentParser()
 for key in('engine','library','gold','report'):p.add_argument('--'+key,type=Path,required=True)
 p.add_argument('--cases',type=int,default=5000);a=p.parse_args();old=Machine(a.engine,False);new=Machine(a.library,True);rng=random.Random(20261005);records=[];counts={};services=0;reentries=0
 for k in range(a.cases):
  row={'operation':k%5,'index':k%4,'flags':rng.randrange(256),'hit':rng.randrange(9),'labels':rng.randrange(32),'stop':rng.randrange(256),'action':(k//5)%5,'accept':k%5!=0,'context':k%13!=0,'selection':rng.getrandbits(32),'mask':rng.randrange(32),'advance':k%2,'next':[rng.randrange(-50,80),rng.randrange(-50,80),[0.,.3,-.7,math.pi][k%4],k%3!=0],'traits':[[rng.randrange(2),1,rng.randrange(2),rng.randrange(2)]for j in range(8)],'matrices':[[1.,0.,float(rng.randrange(-500,500)),0.,1.,float(rng.randrange(-500,500))]for j in range(8)],'buttons':rng.sample(range(1,9),rng.randrange(9)),'slots':[]}
  for i in range(4):row['slots'].append(([rng.randrange(-20,30),rng.randrange(-20,30),0.,rng.randrange(2)],[rng.randrange(9),rng.randrange(9),0,rng.randrange(9),rng.randrange(9)],int(k%11!=0)))
  # Graphic cases execute actual source trigonometry/sanitization.
  if k%19==0:row['slots'][row['index']][1][2]=2
  e,trace=old.run(row);v,calls=new.run(row)
  assert equal(e,v),(k,row,e.hex(),v.hex());assert len(trace)==len(calls)and all(equal(x,y)for x,y in zip(trace,calls)),(k,row,[x.hex()for x in trace],[x.hex()for x in calls])
  records.append({'input':row,'state':e.hex(),'trace':[x.hex()for x in trace]});counts[str(k%5)]=counts.get(str(k%5),0)+1;services+=len(trace);assert old.reentries==new.reentries;reentries+=old.reentries
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_text(json.dumps(records,separators=(',',':'))+'\n')
 report={'validation':'PASS','comparisons':len(records),'ordered_services':services,'original_recursive_reset_focus_calls':reentries,'operation_counts':counts,'mismatches':0,'original_sha256':sha(a.engine),'arm64_library_sha256':sha(a.library),'gold_sha256':sha(a.gold),'source_sha256':{str(x.relative_to(ROOT.parents[1])).replace('\\','/'):sha(x)for x in[ROOT/'swf_cursor_input.hpp',ROOT/'swf_cursor_input.cpp',Path(__file__)]},'scope':__doc__,'whole_game_input_parity':False}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
