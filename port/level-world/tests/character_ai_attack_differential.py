"""Actual melee/Cmd_Attack instructions vs native coordinator; backend services remain explicit."""
import argparse,hashlib,itertools,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from navigation_differential import Cpu
OPS=range(21)
DEAD,RANGE,ATTACKING,REDIRECT,LOG,CREATE,RESET,ANGLE,SEARCH,POP,DESTROY,SET,SYNC,CAN,TDEAD,PLAYER,MELEE,FSM,ONLINE,SEND,DISPATCH=OPS
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def u32(x):return x&0xffffffff
class Pair:
 def __init__(self,engine,library,manifest):
  self.old=Cpu(engine,False,manifest);self.new=Cpu(library,True,{'functions':[]})
  o=self.old;n=self.new
  self.owner=o.data+0x1000;self.ai=self.owner+0x3c8;self.target=o.data+0x4000;self.candidate=o.data+0x6000;self.fallback=o.data+0x8000;self.last=o.data+0x9000;self.vt=o.data+0xa000;self.sm=o.data+0xb000
  self.controller=o.data+0xe000;self.cvt=o.data+0xf000;self.network=o.data+0x10000;self.packet=o.data+0x11000
  self.virtual=[o.data+0xd000+i*16 for i in range(3)]
  for p in self.virtual:o.uc.mem_write(p,bytes.fromhex('1eff2fe1'))
  for slot,p in zip((0x34,0x124,0x28),self.virtual):o.pointer(self.vt+slot,p)
  self.ids=[0,self.owner,self.target,self.candidate,self.fallback,self.last,self.owner+0x374]
  self.nids=[0,0xabcdef0100001000,0xabcdef0100004000,0xabcdef0100006000,0xabcdef0100008000,0xabcdef0100009000,0xabcdef0100001374]
  self.ns=n.data+0x1000;self.nc=n.data+0x2000;self.svc=n.data+0x3000;self.cb=n.data+0x4000;self.list_base=n.data+0x20000
  n.uc.mem_write(self.cb,bytes.fromhex('c0035fd6'));n.uc.mem_write(self.svc,struct.pack('<QQ',0x123456789abcdef0,self.cb))
  o.uc.hook_add(UC_HOOK_CODE,self.old_hook);n.uc.hook_add(UC_HOOK_CODE,self.new_hook)
 def ow(self,p):return struct.unpack('<I',self.old.uc.mem_read(p,4))[0]
 def ob(self,p):return bytes(self.old.uc.mem_read(p,1))[0]
 def ret(self,c,v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def identity(self,p,native=False):
  if p in (self.nids if native else self.ids):return (self.nids if native else self.ids).index(p)
  if p in (self.nlists if native else self.olists):return 7
  raise AssertionError(('unknown identity',hex(p),native))
 def state(self,native=False):
  if native:
   owner,target,last,interest,flags,heading,continued,ending,index,typ,finisher,seeking=struct.unpack('<QQQQIIIIiiII',self.new.uc.mem_read(self.ns,64))
   return [self.identity(target,True),self.identity(last,True),continued,seeking,u32(index),ending,finisher,flags,heading,u32(typ),self.identity(interest,True),self.identity(owner,True)]
  a=self.ai;o=self.owner
  return [self.identity(self.ow(a+0x40)),self.identity(self.ow(a+0x44)),self.ob(a+0x78),self.ob(a+0x4a),self.ow(a+0x74),self.ob(a+0x79),self.ob(a+0x7a),self.ow(o+0x528),self.ob(o+0x1b5),u32(struct.unpack('<b',self.old.uc.mem_read(o+0x14a8,1))[0]),self.identity(self.ow(o+0x14a4)),self.identity(self.ow(a+4))]
 def mutate(self,native,op):
  i=int(native)
  if self.done[i] or op!=self.f[29]:return
  self.done[i]=True;action=self.f[30]
  if native:
   n=self.new;raw=bytearray(n.uc.mem_read(self.ns,64))
   if action==1:struct.pack_into('<I',raw,36,1-struct.unpack_from('<I',raw,36)[0])
   elif action in (2,3):struct.pack_into('<Q',raw,8,self.nids[4] if action==2 else 0)
   elif action==4:struct.pack_into('<I',raw,32,1)
   elif action==5:struct.pack_into('<I',raw,44,255)
   elif action==6:
    for offset,value in ((48,0xdeadbeef),(56,77),(44,19),(60,3),(40,6)):struct.pack_into('<I',raw,offset,value)
   elif action==8:struct.pack_into('<Q',raw,16,self.nids[5])
   n.uc.mem_write(self.ns,bytes(raw))
   if action==7:self.nested(n,'dh2_character_ai_melee_attack',[self.ns,self.nids[4],1,self.svc])
  else:
   o=self.old;a=self.ai
   if action==1:o.uc.mem_write(self.owner+0x1b5,bytes((1-self.ob(self.owner+0x1b5),)))
   elif action in (2,3):o.pointer(a+0x40,self.fallback if action==2 else 0)
   elif action==4:o.pointer(self.owner+0x528,1)
   elif action==5:o.uc.mem_write(a+0x79,b'\xff')
   elif action==6:
    o.pointer(a+0x74,0xdeadbeef)
    for offset,value in ((0x7a,77),(0x79,19),(0x4a,3),(0x78,6)):o.uc.mem_write(a+offset,bytes((value,)))
   elif action==8:o.pointer(a+0x44,self.last)
   if action==7:self.nested(o,0x3d01ac,[a,self.fallback,1])
 def nested(self,c,address,args):
  saved=c.uc.context_save();sp=c.uc.reg_read(c.sp)
  # Cpu.invoke uses stack+0xe000. Preserve the active frame under reentry.
  stack=c.stack;c.stack=sp-0x10000
  try:c.invoke(address,args)
  finally:c.stack=stack;c.uc.context_restore(saved)
 def emit(self,native,op,subject=0,payload=0,a=0,b=0):
  self.traces[int(native)].append([op,a,b,self.identity(subject,native),self.identity(payload,native)]+self.state(native));self.mutate(native,op)
 def old_hook(self,uc,address,size,unused):
  o=self.old;a=self.ai;f=self.f
  if address in self.virtual:
   kind=self.virtual.index(address);subject=o.reg(0)
   op=DEAD if kind==0 and subject==self.owner else TDEAD if kind==0 else RANGE if kind==1 else PLAYER
   self.emit(False,op,self.owner,subject if op==TDEAD else 0);self.ret(o,f[{DEAD:0,TDEAD:9,RANGE:2,PLAYER:10}[op]])
  elif address==0x3c02d0:self.emit(False,ATTACKING,self.owner);self.ret(o,f[3])
  elif address==0x3d076c:self.emit(False,REDIRECT,self.owner,o.reg(1),o.reg(2));self.ret(o)
  elif address in (0x337888,0x3140ec,0x318254,0x4a2240,0x4a191c):self.ret(o)
  elif address==0x337a88:self.emit(False,LOG,self.owner,0,o.uc.reg_read(o.lr));self.ret(o,f[11])
  elif address==0x4a2730:
   h=o.reg(0);self.olists.append(h);self.emit(False,CREATE,self.owner,0,o.reg(2),self.ow(o.uc.reg_read(o.sp)))
  elif address==0x3d015c:self.emit(False,RESET,o.reg(0),o.reg(0))
  elif address==0x4c4bdc:self.emit(False,ANGLE,self.owner);self.ret(o,f[28])
  elif address==0x3d0020:
   h=o.reg(0);self.emit(False,SEARCH,h,h,o.reg(1),o.reg(2));buf=o.data+0x30000+self.searches[0]*0x100;self.searches[0]+=1
   for j in range(f[6]):o.pointer(buf+j*16,self.candidate)
   o.pointer(h,buf);o.pointer(h+0x10,buf+16*f[6]);self.ret(o)
  elif address==0x38fb18:
   h=o.reg(0);self.emit(False,POP,h,h);o.pointer(h,self.ow(h)+16);self.ret(o)
  elif address==0x38d18c:self.emit(False,DESTROY,o.reg(0),o.reg(0));self.ret(o)
  elif address==0x3d6890:
   requested=o.reg(1);mode=o.reg(2);self.emit(False,SET,self.owner,requested,mode);o.pointer(a+0x3c,requested);o.pointer(a+0x40,requested);self.ret(o)
  elif address==0x3d49c4:self.emit(False,SYNC,self.owner)
  elif address==0x3d67f4:self.emit(False,CAN,self.owner);self.ret(o,f[7])
  elif address==0x3d6188:self.emit(False,MELEE,self.owner);self.ret(o,f[8])
  elif address==0x3c6488:self.emit(False,FSM,self.owner,o.reg(1),o.reg(2));self.ret(o)
  elif address==0x7fd794:self.emit(False,ONLINE);self.ret(o,self.network)
  elif address==0x80b1bc:self.emit(False,SEND,self.owner,self.ids[2] if f[13] else 0);self.ret(o,self.network)
  elif address==0x80a244:self.ret(o,self.packet)
  elif address==0x80e2a4:
   assert self.ob(self.packet+0x50)==0 and struct.unpack('<H',uc.mem_read(self.packet+0x52,2))[0]==(0x1234 if f[13] else 0) and self.ob(self.packet+0x54)==0x37
   self.packet_checks+=1;self.ret(o)
  elif address==0x3ad874:self.emit(False,DISPATCH,self.owner+0x374,self.ids[2] if f[13] else 0)
 def new_hook(self,uc,address,size,unused):
  if address!=self.cb:return
  n=self.new;assert n.reg(0)==0x123456789abcdef0 and n.reg(1)==self.ns
  op,a,b,res,subject,payload=struct.unpack('<IIIIQQ',uc.mem_read(n.reg(3),32));assert res==0
  self.emit(True,op,subject,payload,a,b);out=n.reg(4);word=identity=0;f=self.f
  if op in (DEAD,RANGE,ATTACKING,CAN,TDEAD,PLAYER,MELEE,LOG,ANGLE,ONLINE):word=f[{DEAD:0,RANGE:2,ATTACKING:3,CAN:7,TDEAD:9,PLAYER:10,MELEE:8,LOG:11,ANGLE:28,ONLINE:20}[op]]
  elif op==CREATE:
   identity=self.list_base+len(self.nlists)*0x100;self.nlists.append(identity);uc.mem_write(identity,struct.pack('<QQII',identity,identity+32,0,0))
  elif op==SEARCH:
   for j in range(f[6]):uc.mem_write(payload+32+j*8,struct.pack('<Q',self.nids[3]))
   uc.mem_write(payload+16,struct.pack('<II',f[6],0))
  elif op==POP:uc.mem_write(payload+20,struct.pack('<I',struct.unpack('<I',uc.mem_read(payload+20,4))[0]+1))
  elif op==SET:uc.mem_write(self.ns+8,struct.pack('<Q',payload))
  elif op==SYNC:uc.mem_write(self.ns+16,bytes(uc.mem_read(self.ns+8,8)))
  elif op==DISPATCH:self.nested(n,'dh2_character_ai_melee_attack',[self.ns,payload,0,self.svc])
  uc.mem_write(out,struct.pack('<IIQ',u32(word),0,identity));self.ret(n)
 def run(self,f):
  self.f=f;self.traces=[[],[]];self.done=[False,False];self.olists=[];self.nlists=[];self.searches=[0,0]
  o=self.old;n=self.new;a=self.ai
  o.uc.mem_write(self.owner,bytes(0x1800));o.pointer(self.owner,self.vt)
  for p in (self.target,self.candidate,self.fallback,self.last):o.pointer(p,self.vt)
  o.pointer(a+4,self.owner);o.pointer(self.owner+0x528,f[1]);o.uc.mem_write(self.owner+0x1b5,bytes((f[5],)));o.uc.mem_write(a+0x79,bytes((f[4],)));o.uc.mem_write(a+0x78,bytes((f[24],)));o.uc.mem_write(a+0x7a,bytes((f[25],)));o.pointer(a+0x74,f[26]);o.uc.mem_write(a+0x4a,bytes((f[27],)))
  o.pointer(a+0x40,self.target if f[14] else 0);o.pointer(a+0x44,self.last if f[23] else 0);o.pointer(self.owner+0x14a4,self.fallback);o.uc.mem_write(self.owner+0x14a8,bytes((8 if f[15] else 255,)))
  o.pointer(self.owner+0x374,self.cvt);o.pointer(self.cvt+0x38,0x3ad874);o.uc.mem_write(self.owner+0x108,b'\x37');o.uc.mem_write(self.target+0x108,struct.pack('<I',0x1234))
  o.uc.mem_write(self.controller,bytes(32));o.pointer(self.controller+4,self.owner+0x374);o.pointer(self.controller+0xc,self.owner if f[22] else 0);o.uc.mem_write(self.controller+8,bytes((f[17],f[19],f[21])));o.uc.mem_write(0x9a318b,bytes((f[18],)));o.uc.mem_write(self.network+5,bytes((f[20],)))
  n.uc.mem_write(self.ns,struct.pack('<QQQQIIIIiiII',self.nids[1],self.nids[2] if f[14] else 0,self.nids[5] if f[23] else 0,self.nids[4],f[1],f[5],f[24],f[4],struct.unpack('<i',struct.pack('<I',f[26]))[0],8 if f[15] else -1,f[25],f[27]))
  n.uc.mem_write(self.nc,struct.pack('<QQIIII',self.nids[6],self.nids[1] if f[22] else 0,f[18],f[17],f[19],f[21]))
  requested=self.ids[2] if f[13] else 0;nrequested=self.nids[2] if f[13] else 0
  if f[16]:o.invoke(0x405b04,[self.controller,requested]);status=n.invoke('dh2_character_cmd_attack',[self.nc,self.ns,nrequested,self.svc])
  else:o.invoke(0x3d01ac,[a,requested,f[12]]);status=n.invoke('dh2_character_ai_melee_attack',[self.ns,nrequested,f[12],self.svc])
  assert status==0,(f,status)
  assert self.traces[0]==self.traces[1],(f,self.traces)
  assert self.state()==self.state(True),(f,self.state(),self.state(True))
  return f+self.state()+[len(self.traces[0])]+[v for t in self.traces[0] for v in t]
def fixtures():
 base=json.loads((ROOT/'reference/prince-live-attack/melee-probes.json').read_text());keys=('dead','blocked','range','attacking','last','heading','found','can_attack','in_melee','target_dead','player','dump','mode','explicit','current','ooi')
 out=[]
 for r in base['records']:out.append([r['fixture'][k] for k in keys]+[0]*8+[165,90,0x12345678,0,90,0xffffffff,0,0])
 for r in base['controller_records']:
  f=[0]*32;f[6]=f[8]=f[10]=1;f[13]=r['explicit'];f[16]=1;f[17:23]=[r[k] for k in ('locked','blocked','forced','online','enabled','character_present')];f[28]=90;f[29]=0xffffffff;out.append(f)
 # Exact callback-visible field timing, retained identities and nested entry.
 for continued,op,action in itertools.product((0,1),(DEAD,RANGE,ATTACKING,LOG,CREATE,ANGLE,CAN,TDEAD,PLAYER,MELEE,FSM),(1,2,4,5,6,7,8)):
  f=[0]*32;f[3]=continued;f[6]=3;f[7]=f[8]=f[10]=f[11]=f[14]=1;f[24:29]=[165,90,0x12345678,5,90];f[29:31]=[op,action];out.append(f)
 # Speculative save/restore, including existing target/last-target/continued.
 for values in itertools.product((0,1),repeat=8):
  explicit,continued,current,last_target,old_continued,found,interest,heading=values
  f=[0]*32;f[3]=continued;f[5]=heading;f[6]=3 if found else 0;f[7]=f[8]=f[10]=f[11]=1;f[13:16]=[explicit,current,interest];f[16]=1;f[20:23]=[1,1,1];f[23]=last_target;f[24:29]=[165 if old_continued else 0,90,0x12345678,5,90];f[29]=0xffffffff;out.append(f)
 for continued,op,action in itertools.product((0,1),(ONLINE,SET,SYNC,ATTACKING,LOG,FSM,MELEE,SEND),(2,6,7,8)):
  f=[0]*32;f[3]=continued;f[6]=3;f[7]=f[8]=f[10]=f[11]=f[14]=1;f[16]=1;f[20:24]=[1,1,1,1];f[24:29]=[165,90,0x12345678,5,90];f[29:31]=[op,action];out.append(f)
 rng=random.Random(20261005)
 for _ in range(768):
  f=[rng.randrange(2) for _ in range(32)];f[6]=rng.randrange(4);f[4]=rng.choice((0,0,0,1,255));f[23]=rng.randrange(2);f[24:28]=[rng.randrange(256),rng.randrange(256),rng.randrange(2**32),rng.randrange(256)];f[28]=u32(rng.choice((-91,-90,-1,0,1,89,90,91,2147483647,-2147483648)));f[29]=0xffffffff;f[30]=f[31]=0;out.append(f)
 return out
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True);p.add_argument('--report',type=Path,required=True);p.add_argument('--reference-output',type=Path,required=True);a=p.parse_args()
 manifest=json.loads((ROOT/'reference/prince-live-attack/original-functions.json').read_text());assert sha(a.engine)==manifest['original_sha256']
 pair=Pair(a.engine,a.library,manifest);pair.packet_checks=0;records=[];callbacks=0
 for i,f in enumerate(fixtures()):
  try:r=pair.run(f)
  except Exception:print('FAIL CASE',i,f);raise
  records.append(struct.pack('<I',len(r))+struct.pack('<'+'I'*len(r),*map(u32,r)));callbacks+=len(pair.traces[0])
 gold=struct.pack('<III',0x31414d43,len(records),17)+b''.join(records);a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(gold)
 report=dict(validation='PASS',comparisons=len(records),ordered_services=callbacks,network_packet_checks=pair.packet_checks,mismatches=0,original_sha256=sha(a.engine),manifest_sha256=sha(ROOT/'reference/prince-live-attack/original-functions.json'),input_fixture_sha256=sha(ROOT/'reference/prince-live-attack/melee-probes.json'),arm64_library_sha256=sha(a.library),reference_sha256=hashlib.sha256(gold).hexdigest(),source_sha256={str(p.relative_to(ROOT)):sha(p) for p in (ROOT/'character_ai_attack.hpp',ROOT/'character_ai_attack.cpp')},script_sha256=sha(Path(__file__)),scope=__doc__,backend_search_range_fsm_network_services=True,nested_attack_and_mutation_fixtures=218,speculative_restore_grid=256,identities_above_4gib=True)
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
