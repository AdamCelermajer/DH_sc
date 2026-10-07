"""Complete original InfoHUDManager methods vs O2 native services coordinator.
World/player/AS/cache/script/camera services are explicit projections; all captured
manager instructions, arithmetic, field gates and traversal execute. Original
RenderFX GotoFrame body executes genuine virtual order. No full backend claim.
"""
import argparse,hashlib,json,math,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE,UC_HOOK_MEM_WRITE
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'../level-world/tests'))
from visual_timeline_differential import TimelineCpu,integer
from navigation_search_differential import word
from aggro_differential import float_bits
from hud_player_infos_differential import sint,floating,words

def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Cpu(TimelineCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='snprintf':
   p=self.reg(2);b=bytearray()
   while (x:=self.uc.mem_read(p,1)[0]):b.append(x);p+=1
   text=(b.decode()%sint(self.reg(3))).encode();cap=self.reg(1)
   if cap:self.uc.mem_write(self.reg(0),text[:cap-1]+b'\0')
   self.put(0,len(text));uc.reg_write(self.pc,uc.reg_read(self.lr));return
  super().external(uc,address,size,unused)
class Machine:
 def __init__(self,path,native,manifest):
  self.c=Cpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.svc=d+0x1800;self.actor=[d+0x3000+i*0x2000 for i in range(2)];self.player=[d+0x9000+i*0x1000 for i in range(4)];self.clips=[d+0xe000+i*0x200 for i in range(29)];self.root=d+0x12000;self.vt=d+0x13000;self.sheet=d+0x14000;self.scripts=d+0x15000;self.spells=d+0x15100;self.online=d+0x15200;self.level=d+0x15400;self.strbase=d+0x16000;self.alloc=self.strbase;c.handler=self.callback;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  if not native:
   c.uc.hook_add(UC_HOOK_MEM_WRITE,self.write)
   for p in self.clips+self.actor+self.player:c.pointer(p,self.vt)
   for off,delta in ((8,48),(0x14c,64),(0x94,80),(0x20,96),(0x34,112),(0x24,128),(0x50,144)):c.pointer(self.vt+off,c.callback+delta)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def cstr(self,p):
  if not p:return ''
  b=bytearray()
  while (v:=self.c.uc.mem_read(p,1)[0]):b.append(v);p+=1
  return b.decode()
 def string(self,t):
  p=self.alloc;self.alloc+=len(t.encode())+8;self.c.uc.mem_write(p,t.encode()+b'\0');return p
 def ident(self,p):
  if p in self.actor:return self.actor.index(p)+1
  if p in self.player:return self.player.index(p)+10
  if p in self.clips:return self.clips.index(p)+100
  if p==self.root:return 500
  if p in (701,702,703,704):return p
  return p
 def event(self,op,i=0,v=0,o=0,p=0,fx=0,t='',payload=None):
  e=[op,i,sint(v&0xffffffff),sint(o&0xffffffff),self.ident(p),fx,t,payload];self.events.append(e)
  return e
 def deliver(self,op,i=0,v=0,o=0,p=0,fx=0,t='',payload=None):
  self.event(op,i,v,o,p,fx,t,payload);cfg=self.cfg
  if op==6:
   self.last_cache=i
   if cfg[102]&(1<<i):
    if self.native:
     fxnow=struct.unpack('<Q',self.c.uc.mem_read(self.s+16,8))[0];self.c.uc.mem_write(self.s+16,struct.pack('<Q',fxnow+1))
    else:self.c.pointer(self.s+0x57c,word(self.c,self.s+0x57c)+1)
  if op==1:return (self.level if cfg[5] else 0,cfg[6])
  if op==2:return cfg[7]
  if op==3:return cfg[8] if t=='HUDStyle' else cfg[9]
  if op==4:return self.root
  if op==6:return self.clips[i] if (self.mask&(1<<i)) else 0
  if op==7:return self.player[0]
  if op==8:return cfg[12]
  if op==12:return cfg[11]
  if op==13:return cfg[31+i]
  if op==14:return cfg[34+i]
  if op==15:return cfg[37]
  if op==16:return cfg[38+(p-701)]
  if op==17:return cfg[42+i]
  if op==18:return cfg[19]
  if op==19:return cfg[13]
  if op==20:return self.actor[1] if cfg[15] else 0
  if op==21:return cfg[17]
  if op==22:return cfg[16]
  if op==23:return self.string('Enemy')
  if op==24:return cfg[18]
  if op==25:return cfg[21]
  if op==27:return cfg[22]
  if op==28:return cfg[45+int(p==self.actor[1])]
  if op==29:return cfg[20]
  if op==30:return self.player[i]
  if op==31:return cfg[47+self.player.index(p)]
  if op==32:return self.string(t)
  if op==33:return [cfg[51],cfg[52]]
  if op in (34,35):return cfg[53+op-34]
  if op==37:return self.root
  if op==40:return cfg[55]
  if op==41:return cfg[10]
  if op==42:return bool(self.types&(1<<i))
  return 0
 def callback(self,address):
  c=self.c;r=c.reg
  if self.native:
   op,i,v,o,p,fx,t,payload,*tail=struct.unpack('<4I4Q4I',c.uc.mem_read(r(2),64));out=r(3);pv=None
   if op==33:pv=tail[:3]
   elif op==38:
    b=bytes(c.uc.mem_read(payload,32));pv=[b[0],self.cstr(struct.unpack_from('<Q',b,8)[0]),*struct.unpack_from('<4i',b,16)]
   value=self.deliver(op,i,v,o,p,fx,self.cstr(t),pv);identity=val=fraction=x=y=text=0
   if op==1:identity,val=value
   elif op in(4,6,7,20,30,37):identity=value
   elif op in(16,28,34,35):fraction=value
   elif op in(23,32):text=value
   elif op==33:x,y=value
   else:val=value
   c.uc.mem_write(out,struct.pack('<QIII IQ',identity,val&0xffffffff,fraction,x&0xffffffff,y&0xffffffff,text));c.put(0,1);return
  if address==c.callback+48:
   i=self.clips.index(r(0));self.ret(self.deliver(42,i,r(1),p=r(0),fx=self.capturedfx))
  elif address in(c.callback+64,c.callback+80):
   i=self.clips.index(r(0));self.deliver(10 if address==c.callback+64 else 43,i,r(1),p=r(0),fx=self.capturedfx);self.ret(123)
  elif address==c.callback+96:
   i=self.clips.index(r(0))-8;self.deliver(17,i,p=r(0),t='SlotId');c.uc.mem_write(r(2),bytes(12));c.uc.mem_write(r(2)+4,struct.pack('<d',float(sint(self.cfg[42+i]))));self.ret(1)
  elif address in(c.callback+112,c.callback+128):self.ret(self.deliver(19 if address==c.callback+112 else 21,p=r(0)))
  elif address==c.callback+144:self.ret(self.deliver(31,p=r(0)))
  else:raise AssertionError(hex(address))
 def write(self,uc,access,address,size,value,unused):
  if size==1 and address in [p+0x9b for p in self.clips]:self.deliver(9,v=value,p=address-0x9b)
 def cppstring(self,p,text):
  self.strings[p]=text;b=self.string(text);self.c.pointer(p+0x10,b+len(text));self.c.pointer(p+0x14,b);return b
 def hook(self,uc,address,size,unused):
  if self.native:return
  c=self.c;r=c.reg
  if address==0x31f594:p,v=self.deliver(1);self.ret(p)
  elif address==0x31f66c:self.ret(self.deliver(2))
  elif address==0x320e44:self.ret(self.deliver(3,t=self.cstr(r(1))))
  elif address==0x7a9160:self.ret(self.deliver(4,fx=r(0),t=self.cstr(r(1))))
  elif address==0x427ca0:self.deliver(5,(r(0)-self.s-12)//48,self.cfg[8],p=r(3),fx=r(2),t=self.cstr(r(1)));self.ret()
  elif address==0x427d50:
   i=(r(0)-self.s-12)//48;self.capturedfx=word(c,self.s+0x57c);self.ret(self.deliver(6,i))
  elif address==0x36e478:self.ret(self.deliver(7,r(1),r(2)))
  elif address==0x3bb7fc:self.ret(self.deliver(8,p=r(0)))
  elif address in(0x7a92e0,0x7ab924):
   self.deliver(11 if address==0x7a92e0 else 39,self.clips.index(r(1)) if r(1) else self.last_cache,r(3),p=r(1),fx=r(0),t=self.cstr(r(2)));self.ret()
  elif address==0x3df6e0:self.ret(self.deliver(12,r(1),p=r(0)-0x560))
  elif address==0x3bbe68:self.ret(self.deliver(13,r(1),p=r(0)))
  elif address==0x3d8358:self.ret(self.deliver(14,r(1),p=r(0)-0x3c8))
  elif address==0x3d80b4:self.ret(self.deliver(15,p=r(0)-0x3c8))
  elif address==0x3da3d0:self.ret(self.deliver(16,p=r(0)))
  elif address==0x7fd794:self.deliver(18);self.ret(self.online)
  elif address==0x3d5450:self.ret(self.deliver(20,p=r(0)-0x3c8))
  elif address==0x3a3064:self.ret(self.deliver(22,p=r(0)))
  elif address==0x508edc:self.ret(self.deliver(23,v=r(1),p=self.actor[1]))
  elif address==0x3a3158:self.ret(self.deliver(24,p=r(0)))
  elif address==0x3bd120:self.ret(self.deliver(25,p=r(0)))
  elif address==0x337888:self.deliver(26);self.ret()
  elif address==0x337a88:self.ret(self.deliver(27,t=self.strings[r(1)]))
  elif address==0x3bd2dc:self.ret(self.deliver(28,p=r(0)))
  elif address==0x36d7a8:self.ret(self.deliver(29))
  elif address==0x36e744:self.ret(self.deliver(30,r(1),r(2)))
  elif address==0x507c0c:
   text=self.strings[r(2)];p=self.deliver(32,v=r(3),t=text);self.cppstring(r(0),text);self.ret(p)
  elif address==0x50e830:
   xyz=list(struct.unpack('<3I',c.uc.mem_read(r(0),12)));xy=self.deliver(33,payload=xyz);c.uc.mem_write(r(1),words(*xy));self.ret()
  elif address==0x7a7cac:self.ret(1)
  elif address in(0x416538,0x416578):self.ret(self.deliver(34 if address==0x416538 else 35,fx=word(c,self.s+0x57c)))
  elif address==0x7aa3f0:self.deliver(36,self.clips.index(r(1)) if r(1) else self.last_cache,r(2),r(3),r(1),r(0));self.ret()
  elif address==0x7a7ca0:self.ret(self.deliver(37,fx=r(0)))
  elif address==0x7abe0c:
   p=r(3);pv=[c.uc.mem_read(p+4,1)[0],self.asstrings.get(p+12,''),*(int(struct.unpack('<d',c.uc.mem_read(p+12*i+4,8))[0]) for i in range(2,6))];self.deliver(38,self.ally_index+26,r(4) if False else 6,p=r(1),fx=r(0),t=self.cstr(r(2)),payload=pv);self.ally_index+=1;self.ret(1)
  elif address==0x3fc690:self.ret(self.deliver(41,p=r(0)-0x37c))
  elif address==0x30e2a4:
   a,b=sint(r(0)),sint(r(1));index=self.div_index;self.div_index+=1;self.ret((abs(a)//abs(b)*(-1 if (a<0)!=(b<0) else 1)) if b else self.deliver(40,(0,3,4)[index],a))
  elif address==0x30eae4:
   fmt=self.cstr(r(1));s=fmt%sint(r(2));c.uc.mem_write(r(0),s.encode()+b'\0');self.ret(len(s))
  elif address==0x41ddec:c.uc.mem_write(r(0),bytes(20));self.ret(r(0))
  elif address==0x797a54:lo,hi=struct.unpack('<2I',c.uc.mem_read(r(0)+4,8));c.put(0,lo);c.put(1,hi);uc.reg_write(c.pc,uc.reg_read(c.lr))
  elif address==0x30ea24:self.ret(integer(struct.unpack('<d',words(r(0),r(1)))[0]))
  elif address==0x30ed30:lo,hi=struct.unpack('<2I',struct.pack('<d',float(sint(r(0)))));c.put(0,lo);c.put(1,hi);uc.reg_write(c.pc,uc.reg_read(c.lr))
  elif address==0x30e964:self.ret(float_bits(float(sint(r(0)))))
  elif address==0x797124:self.ret()
  elif address==0x31167c:self.cppstring(r(0),'');self.ret(r(0))
  elif address==0x3116e8:self.cppstring(r(0),bytes(c.uc.mem_read(r(1),r(2)-r(1))).decode());self.ret(r(0))
  elif address==0x3109e0:self.cppstring(r(0),bytes(c.uc.mem_read(r(1),r(2)-r(1))).decode());self.ret(r(0))
  elif address==0x3139ac:self.ret()
  elif address==0x797350:self.asstrings[r(0)]=self.cstr(r(1));self.ret()
 def run(self,cfg):
  self.cfg=cfg;self.mask=cfg[56];self.types=cfg[57];self.events=[];self.alloc=self.strbase;self.strings={};self.asstrings={};self.div_index=0;self.ally_index=0;self.last_cache=0;c=self.c
  self.capturedfx=cfg[4]
  for j,a in enumerate(self.actor):
   if self.native:
    raw=struct.pack('<QIIQQIIQIIQi3fQ',self.sheet,44,0,self.actor[1] if cfg[14] else 0,self.scripts,3,0,self.spells,1,88,self.string('EnemyDebug'),7,1.,2.,3.,j+1);c.uc.mem_write(a,raw)
   else:
    c.pointer(a,self.vt);c.uc.mem_write(a+0xff8,words(*cfg[58:102]));c.pointer(a+0x14a4,self.actor[1] if cfg[14] else 0);c.pointer(a+0x47c,self.scripts);c.pointer(a+0x488,self.spells);c.pointer(a+0x1040,88);c.pointer(a+0x108,7);c.pointer(a+0x44,self.string('EnemyDebug'));c.uc.mem_write(a+0x160,struct.pack('<3f',1.,2.,3.))
  c.uc.mem_write(self.sheet,words(*cfg[58:102]));c.uc.mem_write(self.scripts,struct.pack('<3Q',701 if cfg[23] else 0,702 if cfg[24] else 0,703 if cfg[25] else 0) if self.native else words(701 if cfg[23] else 0,702 if cfg[24] else 0,703 if cfg[25] else 0));c.uc.mem_write(self.spells,struct.pack('<Q',704) if self.native else words(704));
  for j,p in enumerate(self.player):
   act=self.actor[0] if cfg[26+j] else 0;death=sint(cfg[30]);name=self.string('Ally')
   if self.native:c.uc.mem_write(p,struct.pack('<Qii8BQ Q',act,death,22,1,0,0,0,0,0,0,0,name,10+j))
   else:c.pointer(p,self.vt);c.pointer(p+0x660,act);c.pointer(p+0x3a8,death&0xffffffff);c.pointer(p+0x330,22);c.uc.mem_write(p+0x4e5,b'\1');c.pointer(p+0x2e0,name+4);c.pointer(p+0x2e4,name)
  if self.native:
   c.uc.mem_write(self.s,struct.pack('<QI iQ',self.actor[1] if cfg[3] else 0,cfg[1],sint(cfg[2]),cfg[4]));c.uc.mem_write(self.svc,struct.pack('<QQ',0,c.callback+32));result=c.invoke('dh2_ui_hud_manager_v1',[self.s,cfg[0],self.svc],budget=20000000);assert result==0,result
   ptr,init,slow,fx=struct.unpack('<QIiQ',c.uc.mem_read(self.s,24))
  else:
   c.uc.mem_write(self.s,bytes(0x600));c.pointer(self.s,self.actor[1] if cfg[3] else 0);c.pointer(self.s+4,cfg[1]);c.pointer(self.s+8,cfg[2]&0xffffffff);c.pointer(self.s+0x57c,cfg[4]);c.uc.mem_write(self.level+0x198,bytes((cfg[6],)));c.uc.mem_write(self.online+5,bytes((cfg[19],)));c.invoke((0x41ec00,0x41d880,0x41d728,0x41e064,0x41de54)[cfg[0]],[self.s],budget=20000000);ptr,init,slow,fx=word(c,self.s),c.uc.mem_read(self.s+4,1)[0],sint(word(c,self.s+8)),word(c,self.s+0x57c)
  return [self.ident(ptr),init,slow,fx],self.events

def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();m=json.loads((ROOT/'reference/hud-player-infos/manager/original-functions.json').read_text());assert sha(a.engine)==m['original_sha256'];old=Machine(a.engine,False,m);new=Machine(a.library,True,{'functions':[]});rng=random.Random(20261006);rows=[]
 for n in range(650):
  cfg=[0]*103;cfg[0]=n%5;cfg[1]=n%2;cfg[2]=rng.choice((0,1,499,500,-1,-2147483648,2147483647));cfg[3]=n%2;cfg[4]=1;cfg[5]=n%3!=0;cfg[6]=n%4!=0;cfg[7]=rng.choice((0,16,500,-1));cfg[8]=n%4;cfg[9]=n%3;cfg[10]=rng.choice((-32768,-1,0,1,123,32767));cfg[11]=n%3;cfg[12]=rng.choice((289,290,291,292,325,326,327,328));cfg[13]=n%3==0;cfg[14]=n%3!=0;cfg[15]=n%4==0;cfg[16]=n%3!=0;cfg[17]=n%3!=0;cfg[18]=n%7==0;cfg[19]=n%2;cfg[20]=rng.randrange(5);cfg[21]=rng.choice((-1,0,20,2147483647));cfg[22]=n%3;cfg[23]=n%3!=0;cfg[24]=n%4!=0;cfg[25]=n%5!=0;cfg[26]=n%13!=0;cfg[27:30]=[n%5!=0,1,n%7!=0];cfg[30]=rng.choice((-1,0,1000,1999,2147483647));cfg[31:34]=[rng.choice((-1,0)),rng.choice((-1,1)),rng.choice((-1,2))];cfg[34:38]=[rng.choice((0,1,255,-1)) for _ in range(4)];cfg[38:42]=[float_bits(rng.choice((0.,.33,1.,1.5,-.5,math.inf,math.nan))) for _ in range(4)];cfg[42:45]=[rng.choice((-1,0,1,2,3,2147483647)) for _ in range(3)];cfg[45:47]=[float_bits(rng.choice((0.,.3,1.,1.5,-.5,math.nan))) for _ in range(2)];cfg[47:51]=[n%3==0,n%5==0,0,n%7==0];cfg[51:53]=[rng.randrange(-1000,1000),rng.randrange(-1000,1000)];cfg[53:55]=[float_bits(.5),float_bits(1.5)];cfg[55]=rng.choice((-1,0,100));cfg[56]=rng.randrange(1<<29)|(1<<7)|(1<<8)|(1<<9)|(1<<10)|(1<<17)|(1<<18)|(1<<19);cfg[57]=rng.randrange(1<<29);cfg[58:102]=[0]*44;cfg[102]=rng.randrange(1<<29) if n>=100 else 0
  for cur,mx in ((36,38),(41,43),(33,34)):cfg[58+cur]=rng.choice((-2147483648,-1,0,1,50,100,2147483647));cfg[58+mx]=rng.choice((-1,0,1,100))
  before,events=old.run(cfg);after,calls=new.run(cfg)
  assert before==after,(n,'state',before,after,cfg)
  if events!=calls:
   k=next((i for i,(x,y) in enumerate(zip(events,calls)) if x!=y),min(len(events),len(calls)));raise AssertionError((n,'calls',k,events[max(0,k-2):k+3],calls[max(0,k-2):k+3],cfg))
  rows.append({'input':cfg,'after':before,'calls':events})
 a.output.mkdir(parents=True,exist_ok=True);(a.output/'gold.json').write_text(json.dumps({'validation':'PASS','rows':rows},separators=(',',':'))+'\n');report={'validation':'PASS','cases':len(rows),'ordered_services':sum(len(x['calls']) for x in rows),'original_sha256':sha(a.engine),'library_sha256':sha(a.library),'gold_sha256':sha(a.output/'gold.json'),'scope':__doc__};(a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
