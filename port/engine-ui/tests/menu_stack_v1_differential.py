"""Actual original MenuManager/MultiMenu bodies versus optimized ARM64 projections.
SWF AS/animation, MenuBase virtual bodies, debug/touch/listener calls are services;
the original weak-pointer, stack search and array removal instructions execute.
"""
import argparse,json,struct,hashlib,sys,random
from pathlib import Path
from unicorn import UC_HOOK_CODE
R=Path(__file__).resolve().parents[3];sys.path.insert(0,str(R/'port/engine-animation/tests'))
from compiled_transforms_differential import Cpu,words,word
NAMES=['menu_Ingame','menu_CharacterMenu','menu_InventorySheetMain','menu_SkillTreeSheetNew','menu_Help','menu_confirm']
POOL=NAMES+['menu_MultiLogin','menu_MultiplayerConnectivity','menu_MainMenu','menu_VerificationLoading','menu_StartGame','menu_Options','menu_info','menu_HelpButtons','menu_hud_confirm','menu_Loading','menu_FadeFromBlackScreen','menu_splash','menu_About','menu_EnterName','menu_SelectClass','menu_playlist','menu_confirm2','menu_CharacterSheetNew','menu_CharacterSheetStats','menu_FaerySheet','menu_QuestLogSheetNEW','menu_MapSheet','unknown']
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def w(c,a):return word(bytes(c.uc.mem_read(a,4)),0)
class Machine:
 def __init__(self,p,native):
  self.c=Cpu(p,native,{'functions':[]});self.native=native;c=self.c;d=c.data
  self.s=d+0x1000;self.g=d+0x2000;self.sv=d+0x2800;self.service=d+0x2900;self.m=[d+0x4000+i*256 for i in range(6)];self.r=[d+0x8000+i*1024 for i in range(3)];self.ch=[d+0xc000+i*512 for i in range(10)];self.st=[d+0x10000+i*512 for i in range(3)];self.cat=[d+0x12000+i*128 for i in range(3)];self.outer=d+0x14000;self.reg=d+0x15000;self.vt=d+0x16000;self.names=[d+0x17000+i*128 for i in range(6)];self.cb=[d+0x19000+i*16 for i in range(7)]
  c.uc.mem_write(self.service,words([0xd65f03c0]if native else[0xe12fff1e]));
  for a in self.cb:c.uc.mem_write(a,words([0xe12fff1e]))
  for a,n in zip(self.names,NAMES):c.uc.mem_write(a,n.encode()+b'\0')
  self.globals=[0x9f63fc,0x9f6400,0x9f6401,0x9a5c09,0x9a5b59,0x9a5b5a,0x9f640e]
  self.logs=[];self.kind=0;self.mutation=False;self.mutated=False;self.nested=False;self.did_nested=False;self.outer_lr=0;c.uc.hook_add(UC_HOOK_CODE,self.hook)
  self.trampoline=self.service+16;c.uc.mem_write(self.trampoline,words([0xd65f03c0]if native else[0xe12fff1e]))
 def nesting(self,op):
  if op!=6 or not self.nested or self.did_nested:return False
  self.did_nested=True;c=self.c;self.outer_lr=c.uc.reg_read(c.lr);c.put(0,self.s if self.native else self.s+0x400);c.put(1,self.m[4]);
  if self.native:c.put(2,self.sv)
  c.uc.reg_write(c.lr,self.trampoline);c.uc.reg_write(c.pc,c.symbols['dh2_menu_stack_manager_push_v1']if self.native else 0x4317e8);return True
 def ptr(self,a):return int.from_bytes(self.c.uc.mem_read(a,8 if self.native else 4),'little')
 def enum(self,p,values):return values.index(p)+1 if p else 0
 def snapshot(self):
  c=self.c;n=self.native;out=[]
  count=w(c,self.s+8 if n else self.s+0x128);out+=[count]+[self.enum(self.ptr(self.outer+i*(8 if n else 4)),self.r)for i in range(count)]
  for a,t in zip(self.r,self.st):
   count=w(c,a+48 if n else a+0x118);out+=[count]+[self.enum(self.ptr(t+i*(8 if n else 4)),self.m)for i in range(count)]
   out += [w(c,a+8 if n else a+0xf8),self.enum(self.ptr(a+16 if n else a+0x40),self.ch)]
  for a in self.m:out += [w(c,a+40 if n else a+0x58),self.enum(self.ptr(a+32 if n else a+0x54),self.ch),w(c,a+44)if n else c.uc.mem_read(a+0x74,1)[0]]
  for a in self.ch:out += [w(c,a+12)if n else c.uc.mem_read(a+0x9b,1)[0],w(c,a+20)if n else c.uc.mem_read(a+0xea,1)[0]]
  out += [w(c,self.g+4*i)if n else w(c,a)if i==0 else c.uc.mem_read(a,1)[0]for i,a in enumerate(self.globals)]
  if self.kind>=10:out.append(self.query_result)
  return words(out)
 def emit(self,op,render=0,menu=0,char=0,text='',value=0):
  self.logs.append((op,self.enum(render,self.r),self.enum(menu,self.m),self.enum(char,self.ch),text,value,self.snapshot()))
  if self.mutation and not self.mutated and op==7:
   self.mutated=True;rr=render or self.r[0];self.c.uc.mem_write(rr+(8 if self.native else 0xf8),words([0x41]))
 def ret(self,x=0):c=self.c;c.put(0,x);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(self,uc,a,z,u):
  c=self.c;n=self.native
  if a==self.trampoline:c.uc.reg_write(c.lr,self.outer_lr);self.ret();return
  if n:
   if a!=self.service:return
   assert c.reg(0)==0xabcdef0123456789 and c.reg(1)==self.s
   op,value,r,m,ch,t,result=struct.unpack('<IIQQQQQ',bytes(uc.mem_read(c.reg(2),48)))
   text=c.string(t).decode()if t else '';self.emit(op,r,m,ch,text,value)
   if op==8:uc.mem_write(c.reg(2)+40,struct.pack('<Q',self.playfound))
   if op==18:uc.mem_write(c.reg(2)+40,struct.pack('<Q',1))
   if op==19:
    uc.mem_write(m+44,words([0]));ch=self.ptr(m+24);uc.mem_write(ch+12,words([0]))
   if self.nesting(op):return
   self.ret();return
  if a in self.cb:
   ix=self.cb.index(a);m=c.reg(0);r=self.ptr(m+4)if m in self.m else m
   if ix<4:
    op=(5,4,6,3)[ix];self.emit(op,r,m)
    if self.nesting(op):return
   elif ix==4:self.emit(18,r,m);self.ret(1);return
   elif ix==5:self.emit(11,r)
   elif ix==6:self.ret(1);return
   self.ret();return
  if a==0x7ad7e8:
   r=c.reg(0);nm=c.string(c.reg(1)).decode();m=next((m for m in self.m if c.string(m+8).decode()==nm),0);self.emit(7,r,m,0,c.string(c.reg(2)).decode());self.ret(1)
  elif a==0x7aba04:
   r,ch=c.reg(0),c.reg(1);m=next((m for m in self.m if self.ptr(m+0x4c)==ch),0);self.emit(8,r,m,ch,c.string(c.reg(2)).decode(),c.reg(3));self.ret(self.playfound)
  elif a==0x7a7c74:self.ret(c.reg(0)+0x300)
  elif a==0x7a7cac:self.ret(self.ch[6+self.r.index(c.reg(0))])
  elif a==0x7ac410:self.emit(9,c.reg(0));self.ret()
  elif a==0x7ac228:self.emit(10,c.reg(0),0,c.reg(1),value=c.reg(2));self.ret()
  elif a==0x324114:
   site=c.uc.reg_read(c.lr)-4;labels={0x4382b4:'push',0x438530:'multilogin',0x4385b8:'multiplayer_connectivity',0x43858c:'main_menu',0x438998:'classification',0x438a08:'classification',0x4389e8:'classification',0x438c70:'pop',0x438ce4:'pop'}
   m=next((m for m in self.m if m+8==c.reg(1)),0)if site==0x4382b4 else 0;self.emit(1,self.ptr(m+4)if m else 0,m,text=labels[site],value=site);self.ret()
  elif a==0x89becc:self.emit(2);self.ret()
  elif a==0x42ca8c:self.ret(self.s+0x400)
  elif a==0x328f40:self.emit(12);self.ret()
  elif a==0x33c568:self.emit(13);self.ret()
  elif a==0x337888:self.emit(14);self.ret()
  elif a==0x337a88:self.emit(15,text='isTracingMenuManager');self.ret()
  elif a==0x3140ec:uc.mem_write(c.reg(0),bytes(24));self.ret()
  elif a==0x318254:self.ret()
  elif a==0x431750:self.emit(16,self.ptr(c.reg(1)+4),c.reg(1));self.ret()
  elif a==0x42e110:self.emit(17,self.ptr(c.reg(1)+4),c.reg(1));self.ret()
  elif a==0x42cb8c:self.ret(self.r[0])
  elif a==0x796f5c:self.ret(self.names[self.arg])
  elif a==0x4223bc:
   m=c.reg(0);self.emit(19,self.ptr(m+4),m,value=c.reg(1));uc.mem_write(m+0x74,b'\0');uc.mem_write(self.ptr(m+0x4c)+0x9b,b'\0');self.ret()
 def setup(self,seq,flags,globalwords,playfound,mutation):
  c=self.c;n=self.native;self.logs=[];self.mutated=False;self.did_nested=False;self.mutation=mutation;self.playfound=playfound
  for i,a in enumerate(self.ch):
   uc=c.uc;uc.mem_write(a,bytes(512));
   if n:uc.mem_write(a,struct.pack('<Q4I',0x100000000+i,1,1,1,1))
   else:
    c.pointer(a,self.vt);uc.mem_write(a+0x9b,b'\x01');uc.mem_write(a+0xea,b'\x01');c.pointer(a+0x100,a+0x180);uc.mem_write(a+0x180,words([2,1]))
  for i,(a,nm)in enumerate(zip(self.m,NAMES)):
   ri=i//2
   c.uc.mem_write(a,bytes(256))
   c.uc.mem_write(self.names[i],nm.encode()+b'\0')
   if n:c.uc.mem_write(a,struct.pack('<5Q4I',0x200000000+i,self.r[ri],self.names[i],self.ch[i],0,0,1,1,0))
   else:
    c.pointer(a,self.vt);c.pointer(a+4,self.r[ri]);c.uc.mem_write(a+8,nm.encode()+b'\0');c.pointer(a+0x48,self.ch[i]+0x180);c.pointer(a+0x4c,self.ch[i]);c.uc.mem_write(a+0x74,b'\x01')
  for off,cb in zip((0xc,0x10,0x14,0x18,0x3c,0x24,8),self.cb):c.pointer(self.vt+off,cb)
  c.uc.mem_write(self.outer,bytes(512));c.uc.mem_write(self.reg,bytes(128))
  for i,m in enumerate(self.m):c.pointer(self.reg+i*(8 if n else 4),m)
  groups=[[]for _ in self.r]
  for mid in seq:groups[mid//2].append(self.m[mid])
  for i,r in enumerate(self.r):
   c.uc.mem_write(r,bytes(1024));c.uc.mem_write(self.st[i],bytes(512));c.uc.mem_write(self.cat[i],bytes(128))
   for j,m in enumerate(groups[i]):c.pointer(self.st[i]+j*(8 if n else 4),m)
   for j,m in enumerate(self.m[i*2:i*2+2]):c.pointer(self.cat[i]+j*(8 if n else 4),m)
   if n:c.uc.mem_write(r,struct.pack('<QII4QIIQII',0x300000000+i,flags[i],0,self.ch[i*2]if groups[i]else 0,self.ch[6+i],self.ch[9],self.st[i],len(groups[i]),32,self.cat[i],2,0))
   else:
    c.pointer(r,self.vt);c.uc.mem_write(r+0xf8,words([flags[i]]));c.pointer(r+0x40,self.ch[i*2]if groups[i]else 0);c.pointer(r+0x104,self.cat[i]);c.uc.mem_write(r+0x108,words([2]));c.pointer(r+0x114,self.st[i]);c.uc.mem_write(r+0x118,words([len(groups[i]),32]));c.pointer(r+0x300+0x10,self.ch[9]);c.pointer(r+0x8,self.ch[6+i]);c.pointer(self.ch[6+i]+0x10,self.ch[6+i])
  for i,mid in enumerate(seq):c.pointer(self.outer+i*(8 if n else 4),self.r[mid//2])
  if n:
   c.uc.mem_write(self.s,struct.pack('<QIIQIIQQQ',self.outer,len(seq),32,self.reg,6,0,self.r[0],self.r[0],self.g));c.uc.mem_write(self.g,words(globalwords+[0]));c.uc.mem_write(self.sv,struct.pack('<QQ',0xabcdef0123456789,self.service))
   base=self.s+0x800;bst=self.s+0x900;c.pointer(bst,self.m[0]);c.uc.mem_write(base,struct.pack('<QII4QIIQII',0x300000010,0,0,0,self.ch[6],0,bst,1,32,self.cat[0],2,0));c.pointer(self.s+32,base)
  else:
   c.uc.mem_write(self.s,bytes(1024));c.pointer(self.s+0x124,self.outer);c.uc.mem_write(self.s+0x128,words([len(seq),32]));c.pointer(self.s+0x114,self.st[0]);c.uc.mem_write(self.s+0x118,words([len(groups[0]),32]));c.pointer(self.s+0x400+0xf4,self.s)
   c.pointer(self.s,self.vt+0x100);c.pointer(self.vt+0x140,0x437924);c.pointer(self.vt+0x134,0x438278);c.pointer(self.vt+0x138,0x438c14)
   c.pointer(self.s+0x400+0x64,self.reg);c.pointer(self.s+0x400+0x68,self.reg+24)
   c.pointer(self.s+0x114,self.s+0x900);c.pointer(self.s+0x900,self.m[0]);c.uc.mem_write(self.s+0x118,words([1,32]));c.pointer(self.vt+0x13c,0x439270)
   for i,(a,x)in enumerate(zip(self.globals,globalwords)):c.uc.mem_write(a,words([x])if i==0 else bytes([x]))
 def run(self,kind,arg,seq,flags,globals,playfound,mutation):
  self.kind=kind;self.arg=arg;self.query_result=0;self.setup(seq,flags,globals,playfound,mutation);c=self.c;n=self.native
  names=['dh2_menu_stack_push_v1','dh2_menu_stack_pop_v1','dh2_menu_stack_pop_name_v1','dh2_menu_stack_manager_push_v1','dh2_menu_stack_manager_pop_v1','dh2_menu_stack_hide_all_v1']
  old=[0x438278,0x438c14,0x439270,0x4317e8,0x42e208,0x42d234]
  args=[self.s+0x400 if not n and kind in(3,4,5)else self.s]
  if kind<10 and kind!=5:args.append(self.m[arg]if kind in(0,3,4)else arg if kind==1 else self.names[arg])
  if kind==2:args.append(self.above)
  if n:args.append(self.sv)
  if 6<=kind<10:
   op=kind-6
   if n:args=[self.s,op,self.above,3 if playfound else 2,self.names[arg],self.sv]
   else:
    fn=self.s+0x600;vals=self.s+0x680;header=self.s+0x640;c.uc.mem_write(fn,bytes(32));c.pointer(fn+12,header);c.pointer(header,vals);c.uc.mem_write(fn+16,words([self.above]));c.uc.mem_write(vals,bytes([0,3 if playfound else 2])+bytes(10));args=[fn]
   address='dh2_menu_stack_native_v1'if n else [0x43b1b4,0x43b158,0x43ac28,0x439dd8][op]
  elif kind>=10:
   scratch=self.s+0xa00
   if kind==10:
    name=self.names[arg]if arg<6 else scratch+32
    if arg>=6:c.uc.mem_write(name,b'__absent_menu__\0')
    args=[self.s,name,scratch]if n else[self.s+0x400,name];address='dh2_menu_stack_find_v1'if n else 0x42d1f0
   else:args=[self.s,self.m[arg]if arg<6 else 0]+([scratch]if n else[]);address='dh2_menu_stack_contains_v1'if n else 0x437924
  else:address=names[kind]if n else old[kind]
  try:result=c.invoke(address,args)
  except Exception:
   print('FAULT',n,kind,arg,seq,flags,hex(c.uc.reg_read(c.pc)),[hex(c.reg(i))for i in range(4)],self.logs);raise
  if n:assert result==0,('native return',kind,arg,seq,flags,result,self.logs)
  if kind==10:self.query_result=self.enum(self.ptr(scratch)if n else result,self.m)
  elif kind==11:self.query_result=w(c,scratch)if n else result
  return self.snapshot(),self.logs
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,required=True);p.add_argument('--gold',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 old=Machine(R/'.local-inputs/libDungeonHunter2.so',False);new=Machine(a.library,True);rng=random.Random(438278);records=[];calls=0;counts={};mutations=0;nested=0;flag8=0
 for i in range(4800):
  global NAMES
  ni=[(i+k*3)%len(POOL)for k in range(6)];NAMES=[POOL[x]for x in ni]
  kind=i%12;arg=rng.randrange(7 if kind>=10 else 6)if kind!=1 else i%2;seq=[rng.randrange(6)for _ in range(rng.randrange(0,6))];flags=[rng.choice((0,1,8,9,0x40,0x41,0x48,0x49))for _ in range(3)];globals=[rng.randrange(20),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(2),rng.randrange(2),i%2];pf=(i//12)%2;mut=i%11==0
  old.above=new.above=(i//5)%2
  if 6<=kind<10:old.above=new.above=1 if kind==6 else (i//12)%3
  old.nested=new.nested=i%37==0 and kind in(0,3)
  if kind==2 and not old.above:seq=list(dict.fromkeys(seq))
  expected,el=old.run(kind,arg,seq,flags,globals,pf,mut);actual,al=new.run(kind,arg,seq,flags,globals,pf,mut)
  counts[str(kind)]=counts.get(str(kind),0)+1;mutations+=old.mutated;nested+=old.did_nested;flag8+=any(x&8 for x in flags)
  if expected!=actual:
   ew=struct.unpack('<'+'I'*(len(expected)//4),expected);aw=struct.unpack('<'+'I'*(len(actual)//4),actual);print('STATE DIFFERENCE',i,kind,arg,NAMES,seq,[(k,x,y)for k,(x,y)in enumerate(zip(ew,aw))if x!=y],ew[-7:],aw[-7:]);raise AssertionError('state mismatch')
  if el!=al:
   print('CALL DIFFERENCE',i,kind,arg,seq,flags,[(x[:6],x[6].hex())for x in el if x not in al][:2],[(x[:6],x[6].hex())for x in al if x not in el][:2]);raise AssertionError('ordered service mismatch')
  inp=words([kind,arg,old.above,pf,int(mut),int(old.nested),*ni,len(seq),*seq,*flags,*globals]);records.append(words([len(inp),len(expected),len(el)])+inp+expected+b''.join(words([*x[:4],x[5],len(x[4].encode()),len(x[6])])+x[4].encode()+x[6]for x in el));calls+=len(el)
 a.gold.parent.mkdir(parents=True,exist_ok=True);a.gold.write_bytes(words([0x3154534d,len(records)])+b''.join(records));report={'validation':'PASS','comparisons':len(records),'ordered_service_comparisons':calls,'operation_counts':counts,'actual_synchronous_mutations':mutations,'actual_nested_push_cases':nested,'flag8_cases':flag8,'mismatches':0,'original_sha256':sha(R/'.local-inputs/libDungeonHunter2.so'),'arm64_library_sha256':sha(a.library),'gold_sha256':sha(a.gold),'source_sha256':{x.relative_to(R).as_posix():sha(x)for x in [R/'port/engine-ui/menu_stack_v1.hpp',R/'port/engine-ui/menu_stack_v1.cpp',Path(__file__)]},'scope':__doc__,'debug_call_positions_compared':True,'printf_backend_reconstructed':False};a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
