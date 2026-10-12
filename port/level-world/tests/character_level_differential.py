"""Actual SetLevel/GetLevel and class/property/vitals producer instructions versus O2 ARM64.

Constants lookup delivery and DebugSwitch load/query/string block are explicit
services with source order/transitional sheets. No property/class/add mocked.
"""
import argparse,hashlib,json,random,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-level';ASSETS=REPO/'port/android-native/app/src/main/assets/data'
sys.path.insert(0,str(REPO/'port/game-data/tools'))
from inspect_class_tables import parse
from character_property_bindings_differential import Machine as Properties,strings,words,fw,signed
from visual_timeline_differential import TimelineCpu
from unicorn.arm64_const import UC_ARM64_REG_S0
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
class Machine:
 def __init__(self,path,native,manifest,table):
  self.c=TimelineCpu(path,native,manifest);c=self.c;d=c.data;self.native=native;self.owner=d+0x10000;self.properties=self.owner+0x560;self.args=d+0x2000;self.vec=d+0x3000;self.value=d+0x4000;self.out=d+0x5000;self.defaults=d+0x30000;self.types=d+0x31000;self.design=d+0x33000;self.context=d+0x34000;self.services=d+0x35000;self.model=d+0x36000;self.bindings=d+0x37000;self.lookup=d+0x38000;self.effect=d+0x38100;self.sheets=[d+0x40000+0x1000*i for i in range(4)];self.class_rows=d+0x100000;self.class_data=d+0x140000;self.active=False
  if native:
   for p in(self.lookup,self.effect):c.uc.mem_write(p,bytes.fromhex('c0035fd6'))
   c.uc.mem_write(self.design,struct.pack('<QQQ',self.context,self.lookup,0));c.uc.mem_write(self.services,struct.pack('<QQ',self.context,self.effect));c.uc.mem_write(self.model,struct.pack('<QQIIQ',self.properties,self.class_rows,len(table),0,self.design));c.uc.mem_write(self.bindings,struct.pack('<QQQ3Q',self.model,self.context,self.effect,0,0,0))
  else:
   c.pointer(self.args+4,self.vec);c.pointer(0x9a645c,self.defaults)
   def w(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
   got=0x3e2e34+w(0x3e3008);c.pointer(w(got+w(0x3e300c)),len(table));c.pointer(w(got+w(0x3e3010)),self.class_rows)
   got=0x3e2d88+w(0x3e2e18);c.pointer(got+w(0x3e2e1c),self.defaults+0x3000)
   got=0x3b73c0+w(0x3b7498);self.application=w(got+w(0x3b749c));c.pointer(self.application+0x2c,self.design)
  p=self.class_data
  for i,row in enumerate(table):
   entries=row['entries'];c.uc.mem_write(self.class_rows+i*(16 if native else 12),struct.pack('<QII',p,len(entries),0) if native else words(0,len(entries),p))
   for f in entries:c.uc.mem_write(p,words(*f) if native else words(0,*f));p+=20 if native else 24
  c.uc.hook_add(UC_HOOK_CODE,self.hook)
 def ret(self,v=0):c=self.c;c.put(0,v);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def text(self,p):
  out=b''
  while self.c.uc.mem_read(p+len(out),1)!=b'\0':out+=self.c.uc.mem_read(p+len(out),1)
  return out.decode()
 def snapshot(self):
  c=self.c
  return [struct.unpack('<I',c.uc.mem_read((self.sheets[0] if self.native else self.properties+12)+19*4,4))[0]]+list(struct.unpack('<224I',c.uc.mem_read((self.sheets[3] if self.native else self.properties+0xa98),896)))[19:20]+[struct.unpack('<I',c.uc.mem_read((self.sheets[3] if self.native else self.properties+0xa98)+i*4,4))[0] for i in(36,38,41,43)]
 def event(self,op,prop=0,delta=0):self.trace.append([op,prop,delta&0xffffffff,*self.snapshot()])
 def hook(self,uc,address,size,_):
  if not self.active:return
  c=self.c
  if self.native:
   if address==self.lookup:
    assert c.reg(0)==self.context and c.reg(1)==0 and self.text(c.reg(2))=='CharacterDesign' and self.text(c.reg(3))=='MaxLevelDVeryHard';self.event(1);v=self.caps[min(self.constant_calls,1)];self.constant_calls+=1;uc.mem_write(c.reg(4),words(v));self.ret()
   elif address==self.effect:
    assert c.reg(0)==self.context and c.reg(1)==self.model;p=c.reg(2);stage,prop,delta,_=struct.unpack('<4I',uc.mem_read(p,16));name=struct.unpack('<Q',uc.mem_read(p+16,8))[0];assert stage in(1,2) and (name==0 if stage==1 else self.text(name)=='isTracingChar_Stats');self.event(3 if stage==1 else 4,prop,delta);self.ret()
   elif address==c.symbols['dh2_class_recalc_base']:self.event(2)
   elif address==c.symbols['dh2_property_add']:self.event(5,c.reg(1),c.reg(2))
  else:
   if address==0x4c4bdc:
    assert c.reg(0)==self.design and self.text(c.reg(1))=='CharacterDesign' and self.text(c.reg(2))=='MaxLevelDVeryHard';self.event(1);v=self.caps[min(self.constant_calls,1)];self.constant_calls+=1;self.ret(v)
   elif address==0x3e0810:assert c.reg(0)==self.properties and c.reg(1)==1;self.event(2)
   elif address in(0x3bdd14,0x3bdc28):
    prop=36 if address==0x3bdd14 else 41;self.event(3,prop,c.reg(4));self.event(4,prop,c.reg(4));uc.reg_write(c.pc,0x3bdd50 if prop==36 else 0x3bdc64)
   elif address==0x3e0708:self.event(5,c.reg(1),c.reg(2))
 def setup(self,sheets,defaults,types):
  c=self.c
  if self.native:
   for p,s in zip(self.sheets,sheets):c.uc.mem_write(p,words(*s))
   c.uc.mem_write(self.defaults,words(*defaults));c.uc.mem_write(self.types,words(*types));c.uc.mem_write(self.properties,struct.pack('<7QII',self.defaults,self.types,*self.sheets,0,0,0))
  else:
   c.uc.mem_write(self.owner,bytes(0x1600));c.uc.mem_write(self.defaults,bytes(4)+words(*defaults)+bytes(4)+words(*types))
   for offset,s in zip((8,0x38c,0x710,0xa94),sheets):c.uc.mem_write(self.properties+offset,bytes(4)+words(*s))
   sentinel=self.properties+0xe18;c.uc.mem_write(sentinel,words(0,0,sentinel,sentinel));c.pointer(self.properties+0xe28,0)
 def state(self):return b''.join(bytes(self.c.uc.mem_read(p,896)) for p in self.sheets) if self.native else b''.join(bytes(self.c.uc.mem_read(self.properties+off+4,896)) for off in(8,0x38c,0x710,0xa94))
 def execute(self,kind,payload,n,caps):
  c=self.c;self.trace=[];self.constant_calls=0;self.caps=caps;self.active=True
  if self.native:
   c.uc.mem_write(self.value,words(kind,0,payload,0)+bytes(24));c.uc.mem_write(self.out,words(0xdeadbeef));assert c.invoke('dh2_character_set_level_lua',[self.bindings,self.value,n,self.out+128,0,self.out,self.out+256,256])==0;assert struct.unpack('<I',c.uc.mem_read(self.out,4))[0]==0
  else:
   c.uc.mem_write(self.vec,words(self.value,self.value+112*n,self.value+112*n));c.uc.mem_write(self.value,bytes(112*max(n,1)));c.uc.mem_write(self.value+4,words(kind,payload));c.invoke(0x3b73a4,[self.args,self.out,self.owner])
  self.active=False
  if self.native:assert c.invoke('dh2_character_get_level',[self.out,self.properties])==0;level=struct.unpack('<I',c.uc.mem_read(self.out,4))[0]
  else:level=c.invoke(0x3bd120,[self.owner])
  return self.state(),self.trace,level
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-level-discovery/libcharacter_level.so');a=p.parse_args();manifest=json.loads((REF/'original-functions.json').read_text());manifest['functions']+=json.loads((REF/'helpers/original-functions.json').read_text())['functions']
 build_path=a.library.parent/'arm64-build.json';build=json.loads(build_path.read_text(encoding='utf-8-sig'));assert build['library_sha256']==sha(a.library)
 for path,digest in build['source_bindings'].items():assert sha(Path(path))==digest,('stale compiler input',path)
 build_digest=sha(build_path)
 table=parse(ASSETS)['rows'];old=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False,manifest,table);new=Machine(a.library,True,{'functions':[]},table);raw=(ASSETS/'character_properties_pyarray.bin').read_bytes();defaults=list(struct.unpack_from('<224I',raw,4));types=list(struct.unpack_from('<224I',raw,900));names=strings(ASSETS/'character_properties_pyarraynames.bin');profiles=[];profile_names=[]
 for i,name in enumerate(names):
  if name.startswith('Crypt') or name=='KnightPlayerBase':profiles.append([list(struct.unpack_from('<224I',raw,4+i*896)),defaults.copy(),defaults.copy(),defaults.copy()]);profile_names.append(name)
 # Native32 extremes on live HP/MP expose retained positive-add wrap and caps.
 for hp,mp in((0,0),(0x7fffffff,0x80000000),(0xffffffff,0xffffffff)):
  s=[v.copy() for v in profiles[0]];s[3][36]=hp;s[3][41]=mp;profiles.append(s);profile_names.append('synthetic-vitals-'+str(len(profiles)))
 records=[];calls=0;rng=random.Random(0x3b73a4)
 def compare(pi,kind,payload,n,caps,carry=False):
  nonlocal calls
  if not carry:old.setup(profiles[pi],defaults,types);new.setup(profiles[pi],defaults,types)
  before=old.state();expected=old.execute(kind,payload,n,caps);actual=new.execute(kind,payload,n,caps);assert expected==actual,(len(records),pi,kind,hex(payload),n,caps,[(i//224,i%224,x,y) for i,(x,y) in enumerate(zip(struct.unpack('<896I',expected[0]),struct.unpack('<896I',actual[0]))) if x!=y][:6],expected[1:],actual[1:]);calls+=len(expected[1]);records.append([pi,kind,payload,n,*caps,int(carry),before,expected])
 edges=[0,0x80000000,1,0x3f000000,0xbf000000,fw(256),fw(25600),fw(25601),fw(-256),fw(100000),0x4effffff,0x4f000000,0xcf000000,0x7f800000,0xff800000,0x7fc12345,0x7f812345]
 for pi in range(len(profiles)):
  for payload in edges:compare(pi,3,payload,1,(100,100))
  for kind in range(9):
   for n in(0,1,3):compare(pi,kind,fw(1024),n,(100,100))
  for cap in(0,-1,1,100,0x7fffffff,-2147483648,0x800000):
   for value in(0,256,25601):compare(pi,3,fw(float(value)),1,(cap,cap))
  compare(pi,3,fw(99999),1,(100,80))
  for value in(256,1024,512,25601,0,-256):compare(pi,3,fw(float(value)),1,(100,100),True)
 for i in range(128):compare(i%len(profiles),3,rng.getrandbits(32),1,(100,100))
 gold=words(0x314c4843,len(profiles),len(records),len(table))+words(*defaults,*types)+b''.join(words(*s) for profile in profiles for s in profile)
 for row in table:gold+=words(len(row['entries']))+b''.join(words(*f) for f in row['entries'])
 for pi,kind,payload,n,cap1,cap2,carry,before,(after,trace,level) in records:gold+=words(pi,kind,payload,n,cap1,cap2,carry,len(trace),level)+before+after+b''.join(words(*e) for e in trace)
 dest=REF/'level-fixtures.bin';dest.write_bytes(gold);sources=[ROOT/'character_level.cpp',ROOT/'character_level.hpp',Path(__file__),REPO/'port/game-data/properties.cpp',REPO/'port/game-data/class_tables.cpp',REPO/'port/script-runtime/script_design_bindings.h']
 report=dict(validation='PASS',comparisons=len(records),ordered_callbacks=calls,compared_sheet_words=len(records)*896,profiles=profile_names,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),library_sha256=sha(a.library),gold_sha256=sha(dest),source_bindings={str(x.relative_to(REPO)):sha(x) for x in sources},asset_bindings={str(x.relative_to(REPO)):sha(x) for x in ASSETS.glob('character_*') if x.name.startswith(('character_properties_','character_classes_'))},mismatches=0,scope=__doc__,constant_lookup_producer='explicit delivery; cache design MaxLevelDVeryHard100; exact map/loader parent-owned',debug_switch_backend='explicit skipped block with matched pre-add effects and transitional sheet states')
 assert sha(build_path)==build_digest and sha(a.library)==build['library_sha256']
 for path,digest in build['source_bindings'].items():assert sha(Path(path))==digest,('compiler input changed during audit',path)
 report['compiler_input_bindings']=build;report['build_manifest_sha256']=build_digest
 (ROOT/'reports/character-level-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in('validation','comparisons','ordered_callbacks','compared_sheet_words','mismatches')}))
if __name__=='__main__':main()
