"""Source Character target predicates with genuine decoded AI type rows and explicit virtual/faction/string services."""
import argparse,hashlib,itertools,json,struct,sys,random
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];sys.path.insert(0,str(REPO/'port/game-data/tests'))
from items_differential import Original
from navigation_differential import Cpu
ELF=REPO/'.local-inputs/libDungeonHunter2.so';REF=ROOT/'reference/character-target-providers';ASSETS=REPO/'port/android-native/app/src/main/assets/data'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
METHODS=[0x3a3054,0x3a3064,0x3a3094,0x3a30ac,0x3a49f0,0x3a4870,0x3a47e8,0x3a36e4,0x3a2e1c,0x3a2ed4]
NAMES=[b'PlayerCharacter',b'Other',b'PlayerCharacterPrince',b'xPlayerCharacter',b'Player',b'',b'PlayerCharacte',b'playerCharacter']
class Source(Original):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='strstr':
   def text(p):
    out=bytearray()
    while uc.mem_read(p+len(out),1)!=b'\0':out+=uc.mem_read(p+len(out),1)
    return bytes(out)
   a,b=self.reg(0),self.reg(1);offset=text(a).find(text(b));self.returned(0 if offset<0 else a+offset)
  else:super().external(uc,address,size,unused)
class Native(Cpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='strncmp':
   a,b,n=self.reg(0),self.reg(1),self.reg(2);v=0
   for i in range(n):
    x,y=uc.mem_read(a+i,1)[0],uc.mem_read(b+i,1)[0]
    if x!=y:v=x-y;break
    if not x:break
   self.put(0,v);uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
class QueryOracle:
 def __init__(self,library=None):
  manifest=json.loads((REF/'original-functions.json').read_text());self.old=Source(ELF,manifest);c=self.old;self.owners=[c.data+0x10000,c.data+0x14000];self.table=c.data+0x20000;self.vtable=c.data+0x30000;self.names=[c.data+0x40000+i*64 for i in range(len(NAMES))];self.callbacks=[c.data+0x50000,c.data+0x50020];self.trace=[];self.triggered=False
  c.blob=(ASSETS/'ai_pyarray.bin').read_bytes();c.cursor=4;count=struct.unpack_from('<I',c.blob)[0];self.types=[]
  for i in range(count):c.invoke(0x506f3c,[self.table+i*68,c.stream]);self.types.append(c.word(self.table+i*68+0x38))
  assert c.cursor==len(c.blob)
  got=0x3a3038+c.word(0x3a304c);ptr=c.data+0x60000;c.pointer(got+c.word(0x3a3050),ptr);c.pointer(ptr,self.table)
  got=0x3a3000+c.word(0x3a301c);ptr+=16;c.pointer(got+c.word(0x3a3020),ptr);c.pointer(ptr,count)
  for name,raw in zip(self.names,NAMES):c.uc.mem_write(name,raw+b'\0')
  c.pointer(self.vtable+0x34,0x3a2ed4);c.pointer(self.vtable+0x28,self.callbacks[0]);c.pointer(self.vtable+0x24,0x3a2e1c)
  # A real branch to Character::IsPlayer permits entry vs virtual observation.
  pc=self.callbacks[0];offset=(0x3a49f0-pc-8)//4;c.uc.mem_write(pc,words(0xea000000|(offset&0xffffff)))
  c.uc.hook_add(UC_HOOK_CODE,self.old_hook)
  self.new=Native(library,True,{'functions':[]}) if library else None
  if self.new:
   n=self.new;self.ns=[n.data+0x10000,n.data+0x11000];self.np=[n.data+0x12000,n.data+0x13000];self.nt=n.data+0x20000;self.ntypes=n.data+0x21000;self.nsvc=n.data+0x30000;self.ncallback=n.data+0x31000;self.out=n.data+0x40000;self.nnames=[n.data+0x50000+i*64 for i in range(len(NAMES))]
   for name,raw in zip(self.nnames,NAMES):n.uc.mem_write(name,raw+b'\0')
   n.uc.mem_write(self.ncallback,bytes.fromhex('c0035fd6'));n.uc.mem_write(self.nsvc,struct.pack('<QQ',0,self.ncallback));n.uc.hook_add(UC_HOOK_CODE,self.native_hook)
 def ret(self,c,v=0):c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def observed(self,op,subject):
  self.trace.append([op,subject])
  if op==1 and subject==1 and self.case[14] and not self.mutated:
   self.mutated=True
   if self.current=='old':self.old.pointer(self.owners[0]+0xffc,44);self.old.uc.mem_write(self.owners[0]+0x8a,b'\0');self.old.pointer(self.owners[0]+0x520,self.case[10]^0x2000)
   else:self.new.uc.mem_write(self.np[0]+4,words(44));self.new.uc.mem_write(self.ns[0]+30,b'\0');self.new.uc.mem_write(self.ns[0]+24,words(self.case[10]^0x2000))
  if op==1 and self.case[13] and not self.triggered:
   self.triggered=True;self.trace.append([7,1]);self.nested(8);self.trace.append([8,1])
 def nested(self,op):
  c=self.old if self.current=='old' else self.new;saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
  try:
   if self.current=='old':c.invoke(METHODS[op-1],[self.owners[0],self.owners[1] if self.case[4] else 0])
   else:assert c.invoke('dh2_character_target_query',[self.out+16,op,self.ns[0],self.ns[1] if self.case[4] else 0,self.nt,self.nsvc])==0
  finally:c.stack=stack;c.uc.context_restore(saved)
 def old_hook(self,uc,address,size,unused):
  c=self.old
  if address==0x3a2ed4 and self.case[0]!=10:self.observed(1,1 if c.reg(0)==self.owners[0] else 2)
  elif address==self.callbacks[0]:self.observed(2,1 if c.reg(0)==self.owners[0] else 2)
  elif address in (0x3d511c,0x3d574c):
   assert c.reg(0)==self.owners[0]+0x3c8 and c.reg(1)==self.owners[1];op=3 if address==0x3d511c else 4;self.observed(op,1);self.ret(c,self.case[11 if op==3 else 12])
 def native_hook(self,uc,address,size,unused):
  if address!=self.ncallback:return
  c=self.new;op,_,a,b=struct.unpack('<IIQQ',c.uc.mem_read(c.reg(1),24));self.observed(op,a);v=0
  if op==1:v=c.uc.mem_read(self.ns[a-1]+28,1)[0]
  elif op==2:
   saved=c.uc.context_save();stack=c.stack;c.stack=c.uc.reg_read(c.sp)-0x10000
   try:assert c.invoke('dh2_character_target_query',[self.out+32,5,self.ns[a-1],0,self.nt,self.nsvc])==0;v=struct.unpack('<I',c.uc.mem_read(self.out+32,4))[0]
   finally:c.stack=stack;c.uc.context_restore(saved)
  elif op in (3,4):assert a==1 and b==2;v=self.case[11 if op==3 else 12]
  else:raise AssertionError(op)
  c.uc.mem_write(c.reg(2),struct.pack('<Q',v));self.ret(c)
 def execute(self,case,native=False):
  self.case=case.copy();self.current='new' if native else 'old';self.trace=[];self.triggered=False;self.mutated=False;c=self.new if native else self.old;op,aid,override,name,other,other_type,dead,disabled,visible,interactive,flags,friend,enemy,reentry,mutation=case
  types=self.types.copy()
  if override!=0xffffffff:types[0]=override;aid=0
  # Preserve the tested genuine main row; only the distinct other row is a fixture.
  other_id=1 if aid!=1 else 2
  types[other_id]=other_type
  if native:
   c.uc.mem_write(self.ntypes,words(*types));c.uc.mem_write(self.nt,struct.pack('<QII',self.ntypes,len(types),0))
   for i in range(2):
    p=self.np[i];c.uc.mem_write(p,bytes(896));c.uc.mem_write(p+4,words(aid if i==0 else other_id));c.uc.mem_write(self.ns[i],struct.pack('<QQQI4B',i+1,p,self.nnames[(name if i==0 else 1)-1],flags if i==0 else 0x2000,dead if i==0 else 0,disabled if i==0 else 0,visible if i==0 else 1,interactive if i==0 else 1))
   assert c.invoke('dh2_character_target_query',[self.out,op,self.ns[0],self.ns[1] if other else 0,self.nt,self.nsvc])==0;result=struct.unpack('<I',c.uc.mem_read(self.out,4))[0];after=c.uc.mem_read(self.ns[0]+16,8);after_name=self.nnames.index(struct.unpack('<Q',after)[0])+1
  else:
   for i,v in enumerate(types):c.uc.mem_write(self.table+i*68+0x38,words(v))
   for i,p in enumerate(self.owners):
    c.uc.mem_write(p,bytes(0x1800));c.pointer(p,self.vtable);c.pointer(p+0xffc,aid if i==0 else other_id);c.pointer(p+0x44,self.names[(name if i==0 else 1)-1]);c.uc.mem_write(p+0x1449,bytes([dead if i==0 else 0]));c.uc.mem_write(p+0x81,bytes([disabled if i==0 else 0]));c.uc.mem_write(p+0x8a,bytes([visible if i==0 else 1]));c.uc.mem_write(p+0x415,bytes([interactive if i==0 else 1]));c.uc.mem_write(p+0x520,words(flags if i==0 else 0x2000))
   result=c.invoke(METHODS[op-1],[self.owners[0],self.owners[1] if other else 0]);after_name=self.names.index(c.word(self.owners[0]+0x44))+1
  after_fields=[struct.unpack('<I',c.uc.mem_read(self.np[0]+4,4))[0],struct.unpack('<I',c.uc.mem_read(self.ns[0]+24,4))[0],c.uc.mem_read(self.ns[0]+30,1)[0]] if native else [c.word(self.owners[0]+0xffc),c.word(self.owners[0]+0x520),c.uc.mem_read(self.owners[0]+0x8a,1)[0]]
  return result,self.trace.copy(),after_name,after_fields
def cases(types):
 rows=[];base=[1,44,0xffffffff,1,1,1,0,0,1,1,0x2000,0,0,0,0]
 for aid in [*range(len(types)),0xffffffff,len(types),0x80000000,0x7fffffff]:
  for op in range(1,11):v=base.copy();v[0]=op;v[1]=aid;rows.append(v)
 for typ,other,dead,friend,disabled,visible,interactive,flag in itertools.product((0,1,3,4,5,6),(0,1),(0,1),(0,1),(0,1),(0,1),(0,1),(0,0x2000)):
  v=base.copy();v[0]=6;v[2]=typ;v[4]=other;v[6:11]=[dead,disabled,visible,interactive,flag];v[11]=friend;rows.append(v)
 for typ,other,dead,enemy,other_type in itertools.product((0,1,3,4,5,6),(0,1),(0,1),(0,1),(0,1,4)):
  v=base.copy();v[0]=7;v[2]=typ;v[4]=other;v[5]=other_type;v[6]=dead;v[12]=enemy;rows.append(v)
 for op in (5,6,7,8):
  for typ in (0,1,3,4,5,6):
   v=base.copy();v[0]=op;v[2]=typ;v[13]=1;v[14]=1;rows.append(v)
 for name in range(1,len(NAMES)+1):v=base.copy();v[0]=5;v[2]=0;v[3]=name;rows.append(v)
 for op,raw,typ in itertools.product((6,10),(2,127,255),(1,4,5)):
  v=base.copy();v[0]=op;v[2]=typ;v[6]=raw if op==10 else 0;v[9]=raw;v[8]=255;rows.append(v)
 return rows
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path);a=p.parse_args();o=QueryOracle(a.library);rows=cases(o.types);records=[];callbacks=0
 for i,row in enumerate(rows):
  expected=o.execute(row)
  if a.library:actual=o.execute(row,True);assert actual==expected,(i,row,expected,actual)
  records.append({'input':row,'result':expected[0],'trace':expected[1],'after_name':expected[2],'after_fields':expected[3]});callbacks+=len(expected[1])
 from character_target_provider_handles import run as handles
 from character_target_provider_generic import run as generic
 hr=handles(a.library);radius,base=generic(a.library)
 blob=b'PVD1'+words(len(o.types),len(records),len(hr),len(radius),len(base))+words(*o.types)
 for r in records:blob+=words(*r['input'],r['result'],r['after_name'],*r['after_fields'],len(r['trace']))+b''.join(words(*t) for t in r['trace'])
 for r in hr:
  result,local,shared,entries,trace=r['output'];blob+=words(*r['input'],result,*local,*shared,len(entries),len(trace))+b''.join(words(*t) for t in entries)+b''.join(words(*t) for t in trace)
 blob+=b''.join(words(*r) for r in radius+base);gold=REF/'provider-fixtures.bin';gold.write_bytes(blob)
 callbacks+=sum(len(r['output'][-1]) for r in hr)
 report={'validation':'PASS','comparisons':len(records)+len(hr)+len(radius)+len(base) if a.library else 0,'predicate_cases':len(records),'handle_cases':len(hr),'radius_cases':len(radius),'base_cases':len(base),'original_only_cases':len(records)+len(hr)+len(radius)+len(base),'ordered_callbacks':callbacks,'mismatches':0,'original_sha256':sha(ELF),'script_sha256':sha(Path(__file__)),'ai_asset_sha256':sha(ASSETS/'ai_pyarray.bin'),'corpus_sha256':sha(gold),'genuine_decoded_type_rows':len(o.types),'types':o.types,'source_player_literal':{'address':'0x8c3170','text':'PlayerCharacter','import':'strstr','plt':'0x30ebd4'},'scope':__doc__+' Handle map/tree instructions execute with storage-only allocator. Native registry is logical signed-record map projection. Faction friend/enemy remain explicit services.','records':records,'handle_records':hr}
 captures=list(REF.rglob('original-functions.json'))
 report['capture_bindings']={str(x.relative_to(REPO)):sha(x) for x in captures}
 report['harness_bindings']={str(x.relative_to(REPO)):sha(x) for x in (Path(__file__),ROOT/'tests/character_target_provider_handles.py',ROOT/'tests/character_target_provider_generic.py')}
 report['source_player_literal']['bytes_sha256']=hashlib.sha256(bytes(o.old.uc.mem_read(0x8c3170,16))).hexdigest()
 report['source_player_literal']['relocated_imports']=[hex(k) for k,v in o.old.imports.items() if v=='strstr']
 if a.library:report.update(arm64_library_sha256=sha(a.library),source_sha256=sha(ROOT/'character_target_providers.cpp'),header_sha256=sha(ROOT/'character_target_providers.hpp'))
 path=REF/('query-arm64-probes.json' if a.library else 'query-original-probes.json');path.write_text(json.dumps(report,indent=2)+'\n')
 if a.library:
  summary={k:v for k,v in report.items() if k not in ('records','handle_records')};summary['probe_sha256']=sha(path);(ROOT/'reports/character-target-providers-arm64-differential.json').write_text(json.dumps(summary,indent=2)+'\n')
 print(json.dumps({k:v for k,v in report.items() if k not in ('records','handle_records')}))
if __name__=='__main__':main()
