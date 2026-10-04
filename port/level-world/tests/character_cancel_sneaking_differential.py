"""Complete CancelSneaking/AI_CancelSkill instructions vs O2 native coordinator.

Original IsSneaking, resolved GetProperty, cached list-ID fallback, ordered list
and GetCharSkill execute. Player, complete DelBuff and Active/Pre script bodies
are explicit synchronous services. Their state/reload/order are compared.
"""
import argparse,hashlib,itertools,json,random,struct,sys,zipfile
from pathlib import Path
from unicorn import UC_HOOK_CODE
WORLD=Path(__file__).resolve().parents[1];ROOT=WORLD.parents[1];REF=WORLD/'reference/character-cancel-sneaking'
sys.path.insert(0,str(ROOT/'port/game-data/tests'))
from aggro_differential import Cpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def pack(v):return struct.pack('<'+'I'*len(v),*[x&0xffffffff for x in v])
def cache_tables():
 cache=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
 assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 with zipfile.ZipFile(cache) as z:b=z.read(next(n for n in z.namelist() if n.endswith('/skills_pyarray.bin')))
 p=0
 def word():
  nonlocal p
  x=struct.unpack_from('<I',b,p)[0];p+=4;return x
 def byte():
  nonlocal p
  x=b[p];p+=1;return x
 def text():
  nonlocal p
  n=word();p+=n;return n
 lists=[[word() for _ in range(word())] for _ in range(word())];skills=[]
 for _ in range(word()):
  r=[0]*19;r[1]=word();r[2]=byte();r[3]=word()
  for i in range(r[3]):word()
  r[5]=word();r[6]=byte();r[7]=word();r[8]=word();r[9]=text();r[11]=byte();r[12]=word();r[13]=word();r[14]=text();r[16]=word();r[17]=word();r[18]=word();skills.append(r)
 assert p==len(b) and len(lists)==36 and len(skills)==127 and all(not(r[7]&0x2000000) for r in skills)
 out=ROOT/'.local-inputs/character-cancel-sneaking/skills_pyarray.bin';out.parent.mkdir(parents=True,exist_ok=True);out.write_bytes(b);return lists,skills,out
class Machine:
 def __init__(self,path,native,manifest):
  self.c=Cpu(path,native,manifest);self.native=native;c=self.c;d=c.data
  self.chars=[d+0x1000,d+0x4000];self.ai=d+0x8000;self.props=[d+0x9000,d+0xa000];self.skills=d+0xb000;self.lists=d+0xc000;self.ids=d+0xd000;self.scripts=d+0xe000;self.table=d+0xf000;self.services=d+0x10000;self.request=d+0x11000;self.callback=d+0x12000;self.vtable=d+0x13000;self.sp=[d+0x15000+i*0x100 for i in range(8)];self.trace=[];self.input=[];self.selected=0
  if native:c.uc.mem_write(self.callback,bytes.fromhex('c0035fd6'));c.uc.mem_write(self.services,struct.pack('<QQ',0,self.callback))
  else:
   word=self.word
   got=0x3bc6dc+word(0x3bc77c);self.skills_global=word(got+word(0x3bc780));c.pointer(self.skills_global,self.skills)
   got=0x3bc610+word(0x3bc624);self.lists_global=word(got+word(0x3bc628));c.pointer(self.lists_global,self.lists)
   got=0x3bc5d8+word(0x3bc5f4);self.count_global=word(got+word(0x3bc5f8));c.pointer(self.count_global,4)
   got=0x3dedcc+word(0x3deeb0);offset=word(got+word(0x3deeb8));assert struct.unpack('<224I',c.uc.mem_read(offset,896))==tuple(4*i for i in range(224))
   c.pointer(self.vtable+0x28,self.callback);c.uc.mem_write(self.callback,bytes.fromhex('1eff2fe1'))
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def word(self,p):return struct.unpack('<I',self.c.uc.mem_read(p,4))[0]
 def pointer(self,p):return struct.unpack('<Q' if self.native else '<I',self.c.uc.mem_read(p,8 if self.native else 4))[0]
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def snap(self):
  p=self.chars[0];prop=self.props[0]+198*4 if self.native else p+0xff8+198*4
  aiowner=self.pointer(self.ai if self.native else p+0x3cc)
  return [bytes(self.c.uc.mem_read(p+(40 if self.native else 0x415),1))[0],self.word(prop),self.chars.index(aiowner)]
 def write_sneak(self,x):self.c.uc.mem_write(self.props[0]+198*4 if self.native else self.chars[0]+0xff8+198*4,pack([x]))
 def mutate(self,op,index):
  mode=self.input[7]
  if op==1 and mode in (1,2,6) or op==2 and mode in (3,4,5):self.mutated=True
  if op==1 and mode in (1,2):self.write_sneak(0 if mode==1 else 1)
  if op==1 and mode==6:self.c.pointer(self.ai if self.native else self.chars[0]+0x3cc,self.chars[1])
  if op==2 and mode in (3,5):self.c.pointer(self.scripts+index*(8 if self.native else 4),0 if mode==5 else (0x200000007 if self.native else self.sp[7]))
  if op==2 and mode==4:self.c.pointer(self.ai if self.native else self.chars[0]+0x3cc,self.chars[1])
 def hook(self,uc,address,size,_):
  c=self.c
  if self.native:
   if address!=self.callback:return
   op,index,receiver,arg,reserved=struct.unpack('<IIQII',c.uc.mem_read(c.reg(1),24));assert not reserved
   recv=receiver-0x100000001 if op<2 else -1 if not receiver else receiver-0x200000000
   self.trace.append([op,recv,index,arg,*self.snap()]);self.mutate(op,index);c.uc.mem_write(c.reg(2),pack([self.input[0] if op==0 else self.input[6] if op==2 else 0]));self.ret();return
  if address not in (self.callback,0x3e101c,0x3db16c,0x3da8b8):return
  op={self.callback:0,0x3e101c:1,0x3db16c:2,0x3da8b8:3}[address]
  if op<2:
   receiver=c.reg(0)-(0x560 if op else 0);recv=self.chars.index(receiver);index=0;arg=c.reg(1) if op else 0
   if op:assert c.reg(2)==0 and arg==0x92
  else:receiver=c.reg(0);recv=-1 if not receiver else self.sp.index(receiver);index=self.selected;arg=0
  self.trace.append([op,recv,index,arg,*self.snap()]);self.mutate(op,index);self.ret(self.input[0] if op==0 else self.input[6] if op==2 else 0)
 def run(self,v,authored=None):
  self.input=v;self.trace=[];self.mutated=False;player,sneak,listid,flags,mask,kind,active,mutation,changed,direct,slot,other=v;c=self.c
  sequences=[[],[0],[2,4,1],[3,0,5,4,2]];chosen=sequences[listid if 0<=listid<4 else 3]
  self.selected=slot if direct else next((i for i,id in enumerate(chosen) if flags&(1<<id)),0)
  for i,p in enumerate(self.chars):
   c.uc.mem_write(p,bytes(0x2000 if not self.native else 48));prop=[0]*224;prop[198]=sneak;prop[28]=listid if i==0 else 3
   if self.native:
    c.uc.mem_write(self.props[i],pack(prop));c.uc.mem_write(p,struct.pack('<QQIIQQB7x',0x100000001+i,self.props[i],224,0,self.table,self.ai,changed))
   else:c.pointer(p,self.vtable);c.uc.mem_write(p+0xff8,pack(prop));c.uc.mem_write(p+0x415,bytes([changed]));c.pointer(p+0x3cc,p)
  for i in range(6):
   row=[other]*19;row[7]=(other&~0x2000000)|(0x2000000 if flags&(1<<i) else 0);row[18]=kind;c.uc.mem_write(self.skills+76*i,pack(row))
  for i,ids in enumerate(sequences):
   c.uc.mem_write(self.ids+i*32,pack(ids)+bytes(32-4*len(ids)))
   if self.native:c.uc.mem_write(self.lists+i*16,struct.pack('<QII',self.ids+i*32,len(ids),0))
   else:c.uc.mem_write(self.lists+i*12,pack([0,len(ids),self.ids+i*32]))
  if authored:
   al,ar=authored;sp=c.data+0x30000;lp=c.data+0x34000;ip=c.data+0x38000
   c.uc.mem_write(sp,b''.join(pack(r) for r in ar))
   for i,ids in enumerate(al):
    c.uc.mem_write(ip+i*128,pack(ids));c.uc.mem_write(lp+i*(16 if self.native else 12),struct.pack('<QII',ip+i*128,len(ids),0) if self.native else pack([0,len(ids),ip+i*128]))
   if self.native:c.uc.mem_write(self.table,struct.pack('<QIIQII',lp,len(al),0,sp,len(ar),0))
   else:c.pointer(self.skills_global,sp);c.pointer(self.lists_global,lp);c.pointer(self.count_global,len(al))
  if self.native:
   if not authored:
    c.uc.mem_write(self.table,struct.pack('<QIIQII',self.lists,4,0,self.skills,6,0))
   c.uc.mem_write(self.ai,struct.pack('<QQII',self.chars[0],self.scripts,6,0));c.uc.mem_write(self.scripts,b''.join(struct.pack('<Q',0x200000000+i+1 if mask&(1<<i) else 0) for i in range(6)))
   status=c.invoke('dh2_character_cancel_skill',[self.ai,slot,self.services]) if direct else c.invoke('dh2_character_cancel_sneaking',[self.chars[0],self.services]);assert not status,(v,status)
  else:
   if not authored:c.pointer(self.skills_global,self.skills);c.pointer(self.lists_global,self.lists);c.pointer(self.count_global,4)
   c.pointer(self.chars[0]+0x3cc,self.chars[0]);c.pointer(self.chars[0]+0x3c8+0xb4,self.scripts);c.pointer(self.chars[0]+0x3c8+0xb8,self.scripts+24);c.uc.mem_write(self.scripts,pack([self.sp[i+1] if mask&(1<<i) else 0 for i in range(6)]))
   c.invoke(0x3d84e0,[self.chars[0]+0x3c8,slot]) if direct else c.invoke(0x3bc6b8,[self.chars[0]])
  return self.snap(),self.trace
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--library',type=Path,default=ROOT/'.local-inputs/character-cancel-sneaking/libcharacter_cancel_sneaking.so');a=ap.parse_args();manifest=json.loads((REF/'original-functions.json').read_text());engine=ROOT/'.local-inputs/libDungeonHunter2.so';assert sha(engine)==manifest['original_sha256']
 original=Machine(engine,False,manifest);native=Machine(a.library,True,{'functions':[]});rng=random.Random(0x3bc6b8);fixtures=[]
 for player,sneak,listid,flags in itertools.product((0,1),(-0x80000000,-256,-1,0,1,256,0x7fffffff),(0,1,2,3,-1,4),(0,1,8,32)):
  fixtures.append([player,sneak,listid,flags,63,1,1,0,0xa5,0,0,0xfffdffff])
 for _ in range(512):fixtures.append([rng.choice([0,1,2]),rng.choice([-1,0,1,256,0x80000000]),rng.choice([-1,0,1,2,3,4,0x80000000,0x7fffffff]),rng.randrange(64),rng.randrange(64),rng.choice([0,1,2,0xffffffff]),rng.choice([0,1,2]),rng.randrange(7),rng.randrange(256),0,0,rng.getrandbits(32)])
 for _ in range(128):fixtures.append([0,1,3,rng.randrange(64),rng.randrange(64),rng.choice([0,1,2]),rng.choice([0,1,2]),rng.randrange(7),rng.randrange(256),1,rng.randrange(5),rng.getrandbits(32)])
 gold=[];calls=0;mutations=0;configured=0
 for v in fixtures:
  expected=original.run(v);actual=native.run(v);assert actual==expected and original.mutated==native.mutated,(v,expected,actual);after,trace=expected;calls+=len(trace);mutations+=original.mutated;configured+=bool(v[7]);gold.append(pack(v+after+[len(trace)])+b''.join(pack(t) for t in trace))
 al,ar,actual=cache_tables();authored_cases=0;authored_calls=0
 for id,player,sneak in itertools.product(range(-1,37),(0,1),(0,1)):
  v=[player,sneak,id,0,0,0,0,0,0xa5,0,0,0];expected=original.run(v,(al,ar));observed=native.run(v,(al,ar));assert expected==observed,(v,expected,observed);authored_cases+=1;authored_calls+=len(expected[1])
 corpus=REF/'cancel-sneaking-fixtures.bin';corpus.write_bytes(pack([0x314b5343,len(gold)])+b''.join(gold));sources=[WORLD/'character_cancel_sneaking.cpp',WORLD/'character_cancel_sneaking.hpp',Path(__file__)]
 report=dict(validation='PASS',comparisons=len(gold),ordered_calls=calls,mutating_callback_cases=mutations,mutation_configuration_cases=configured,actual_cache_cases=authored_cases,actual_cache_calls=authored_calls,actual_skill_lists=len(al),actual_skills=len(ar),actual_sneak_skills=0,actual_input_sha256=sha(actual),skill_layout_manifest_sha256=sha(REF/'skill-layout/original-functions.json'),skill_layout_asm_sha256=sha(REF/'skill-layout/reference/original-functions.asm'),mismatches=0,original_sha256=sha(engine),manifest_sha256=sha(REF/'original-functions.json'),arm64_sha256=sha(a.library),corpus_sha256=sha(corpus),source_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in sources},scope=__doc__,full_DelBuff=False,full_skill_VM=False,source_GetProperty=True,source_skill_list_ID_fallback=True,actual_cache_projection='Test-only complete serialized traversal; integer/byte fields retained, unused owned-array/string pointers projected null. Not a production SkillTable loader.')
 (WORLD/'reports/character-cancel-sneaking-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
