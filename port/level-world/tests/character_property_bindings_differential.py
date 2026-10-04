"""Full source GetProp/getter helpers versus O2 ARM64, actual property-cache producers.

ReturnValues STL append is a signed-word numeric projection service. Getter,
GetUInteger/AEABI conversion, Arguments operator[] and getPointer/getBool execute.
Native external identity resolution is a borrowed sheet provider, not GetProp.
"""
import argparse,hashlib,json,random,struct
from pathlib import Path
from unicorn import UC_HOOK_CODE
from visual_timeline_differential import TimelineCpu
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/character-property-bindings';ASSETS=REPO/'port/android-native/app/src/main/assets/data'
def words(*v):return struct.pack('<'+'I'*len(v),*(x&0xffffffff for x in v))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def fw(v):return struct.unpack('<I',struct.pack('<f',v))[0]
def signed(v):return v if v<0x80000000 else v-0x100000000
class Machine:
 def __init__(self,path,native,manifest):
  self.c=TimelineCpu(path,native,manifest);c=self.c;d=c.data;self.native=native
  self.args=d+0x1000;self.vector=d+0x2000;self.records=d+0x3000;self.owner=d+0x10000;self.default=d+0x20000;self.external=d+0x24000;self.bindings=d+0x28000;self.sheet=d+0x28100;self.resolver=d+0x28200;self.out=d+0x29000;self.returned=d+0x29100;self.error=d+0x29200
  if native:c.uc.mem_write(self.resolver,bytes.fromhex('c0035fd6'))
  else:
   c.pointer(self.args+4,self.vector);c.pointer(0x9a645c,self.default)
   def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
   got=0x3dedcc+word(0x3deeb0);self.offsets=word(got+word(0x3deeb8))
   # This table is linked initialized data, not a fixture-invented mapping.
   self.offset_words=list(struct.unpack('<224I',c.uc.mem_read(self.offsets,896)))
   assert self.offset_words==[4*i for i in range(224)],self.offset_words[:5]
   assert bytes(c.uc.mem_read(0x9a2c78+4,896))==bytes(896)
  c.uc.hook_add(UC_HOOK_CODE,self.hook);self.active=False
 def ret(self,value=0):c=self.c;c.put(0,value);c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
 def hook(self,uc,address,size,_):
  if not self.active:return
  c=self.c
  if not self.native:
   if address==0x3dedb4:
    target=c.reg(1);kind=0 if target==self.owner+0xff4 else 1 if target==0x9a2c78 else 2
    assert target in(self.owner+0xff4,0x9a2c78,self.external),(hex(target),hex(self.default));self.trace.append([kind,c.reg(2)])
   elif address==0x37cb24:self.result=[1,fw(float(signed(c.reg(1))))];self.ret()
  elif address==self.resolver:
   assert c.reg(0)==self.bindings and c.reg(1)==self.external
   uc.mem_write(c.reg(2),struct.pack('<QII',self.external,224,0));self.ret()
  elif address==c.symbols.get('dh2_character_property_word'):
   p=c.reg(1);target=struct.unpack('<Q',uc.mem_read(p,8))[0]
   kind=0 if target==self.owner else 1 if target==self.default else 2
   assert target in(self.owner,self.default,self.external);self.trace.append([kind,c.reg(2)])
 def setup(self,profile):
  c=self.c;base,saved,gear,cached,defaults,types,temporary=profile
  if self.native:
   for p,values in ((self.owner,cached),(self.default,temporary),(self.external,gear)):c.uc.mem_write(p,words(*values))
   c.uc.mem_write(self.bindings,struct.pack('<QIIQIIQQ',self.owner,224,0,self.default,224,0,self.bindings,self.resolver))
  else:
   c.uc.mem_write(self.owner,bytes(0x1600));c.uc.mem_write(self.default,bytes(4)+words(*defaults)+bytes(4)+words(*types));c.uc.mem_write(self.external,bytes(4)+words(*gear));c.uc.mem_write(0x9a2c78,bytes(4)+words(*temporary))
   for offset,values in zip((0x568,0x8ec,0xc70,0xff4),(base,saved,gear,cached)):c.uc.mem_write(self.owner+offset,bytes(4)+words(*values))
 def execute(self,row):
  c=self.c;kind,payload,second,boolean,identity,n=row;self.trace=[];self.result=[0,0];self.active=True
  if self.native:
   for i in range(n):c.uc.mem_write(self.records+40*i,words(kind if i==0 else second,0,payload if i==0 else 0,boolean if i==1 else 0)+struct.pack('<QQQ',0,0,self.external if i==1 and identity else 0))
   c.uc.mem_write(self.out,bytes(40));c.uc.mem_write(self.returned,words(0xdeadbeef))
   assert c.invoke('dh2_character_get_prop',[self.bindings,self.records,n,self.out,1,self.returned,self.error,256])==0
   count=struct.unpack('<I',c.uc.mem_read(self.returned,4))[0];self.result=[count,struct.unpack('<I',c.uc.mem_read(self.out+8,4))[0] if count else 0]
  else:
   c.uc.mem_write(self.vector,words(self.records,self.records+112*n,self.records+112*n))
   for i in range(n):
    p=self.records+112*i;c.uc.mem_write(p,bytes(112));c.uc.mem_write(p+4,words(kind if i==0 else second,payload if i==0 else fw(float(boolean))));c.pointer(p+0x6c,self.external if i==1 and identity else 0)
   c.invoke(0x3b9d8c,[self.args,self.out,self.owner])
  self.active=False;return self.result,self.trace
def strings(p):
 b=p.read_bytes();at=4;out=[]
 for _ in range(struct.unpack_from('<I',b)[0]):n=struct.unpack_from('<I',b,at)[0];at+=4;out.append(b[at:at+n].decode());at+=n
 return out
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/character-property-bindings-discovery/libcharacter_property_bindings.so');a=p.parse_args()
 manifest=json.loads((REF/'original-functions.json').read_text());manifest['functions']+=json.loads((REF/'value-helpers/original-functions.json').read_text())['functions']+json.loads((REF/'conversions/original-functions.json').read_text())['functions']+json.loads((REPO/'port/game-data/reference/properties/original-functions.json').read_text())['functions']
 old=Machine(REPO/'.local-inputs/libDungeonHunter2.so',False,manifest);new=Machine(a.library,True,{'functions':[]});c=old.c
 raw=(ASSETS/'character_properties_pyarray.bin').read_bytes();names=strings(ASSETS/'character_properties_pyarraynames.bin');fields=strings(ASSETS/'character_properties_pystructnames.bin');defaults=list(struct.unpack_from('<224I',raw,4));types=list(struct.unpack_from('<224I',raw,900));profiles=[];producers=[]
 for index,name in enumerate(names):
  if not name.startswith('Crypt') and name!='KnightPlayerBase':continue
  base=list(struct.unpack_from('<224I',raw,4+index*896));profile=[base,defaults.copy(),defaults.copy(),defaults.copy(),defaults,types,[0]*224];old.setup(profile)
  prop=c.data+0x40000;c.uc.mem_write(prop,bytes(0xf00))
  for off,s in zip((8,0x38c,0x710,0xa94),profile[:4]):c.uc.mem_write(prop+off,bytes(4)+words(*s))
  sentinel=prop+0xe18;c.uc.mem_write(sentinel,words(0,0,sentinel,sentinel));c.pointer(prop+0xe28,0)
  cache=[c.invoke(0x3dfe60,[prop,i]) for i in range(224)];profile[3]=cache;profiles.append(profile);producers.append(dict(row=index,name=name,cached_sha256=hashlib.sha256(words(*cache)).hexdigest()))
 # Distinct saved and gear values make external selection and cached precedence observable.
 rng=random.Random(0x3b9d8c);base=[rng.getrandbits(32) for _ in range(224)];profiles.append([base,defaults.copy(),[rng.getrandbits(32) for _ in range(224)],[rng.getrandbits(32) for _ in range(224)],defaults,types,[rng.getrandbits(32) for _ in range(224)]])
 records=[];calls=0;direct=[]
 for pi,profile in enumerate(profiles):
  old.setup(profile);new.setup(profile)
  def compare(row):
   nonlocal calls
   expected=old.execute(row);actual=new.execute(row);assert expected==actual,(pi,row,expected,actual);calls+=len(expected[1]);records.append([pi,row,expected])
  for prop in range(224):
   for second,boolean,identity,n in ((0,0,0,1),(1,0,0,2),(1,1,0,2),(2,0,1,2),(7,0,1,2),(2,0,0,2),(3,1,0,2),(4,0,0,2)):
    compare([3,fw(float(prop)),second,boolean,identity,n])
  for kind in range(9):
   for n in (0,1,2,3):compare([kind,fw(198.75),1,1,0,n])
  edges=[0,0x80000000,1,0x007fffff,0x3f000000,0xbf800000,0x435fffff,0x43600000,0x4360ffff,0x4effffff,0x4f000000,0x4f800000,0x7f800000,0xff800000,0x7fc12345,0x7f812345]
  for value in edges:
   for second in range(9):compare([3,value,second,1,1,2])
  for value in [rng.getrandbits(32) for _ in range(128)]:compare([3,value,1,1,0,2])
  for prop in (-2147483648,-1,0,1,198,199,223,224,2147483647):
   for si,ptr in enumerate((old.owner+0xff4,0x9a2c78,old.external,0)):
    # Disable fatal diagnostics; source nonfatal invalid index return is -1.
    got=0x3dedcc+struct.unpack('<I',c.uc.mem_read(0x3deeb0,4))[0];gp=struct.unpack('<I',c.uc.mem_read(got+struct.unpack('<I',c.uc.mem_read(0x3deeb4,4))[0],4))[0];c.pointer(gp,0)
    expected=c.invoke(0x3dedb4,[old.owner+0x560,ptr,prop]) if ptr else c.invoke(0x3b55d4,[old.owner+0x560,prop,0]);target=(new.owner,new.default,new.external,0)[si];new.c.uc.mem_write(new.sheet,struct.pack('<QII',target,224,0));assert new.c.invoke('dh2_character_property_word',[new.out,new.sheet if target else 0,prop])==0;actual=struct.unpack('<I',new.c.uc.mem_read(new.out,4))[0];assert expected==actual;direct.append([pi,si,prop&0xffffffff,expected])
 corpus=words(0x31425043,len(profiles),len(records),len(direct))+b''.join(words(*sheet) for profile in profiles for sheet in profile)
 for pi,row,(output,trace) in records:corpus+=words(pi,*row,*output,len(trace))+b''.join(words(*t) for t in trace)
 corpus+=b''.join(words(*r) for r in direct);gold=REF/'property-bindings-fixtures.bin';gold.write_bytes(corpus)
 bindings=[ROOT/'character_property_bindings.cpp',ROOT/'character_property_bindings.hpp',Path(__file__),REF/'original-functions.json',REF/'value-helpers/original-functions.json',REF/'conversions/original-functions.json',REF/'temp-producers/original-functions.json',REPO/'port/game-data/reference/properties/original-functions.json',REPO/'port/script-runtime/script_runtime.h']
 report=dict(validation='PASS',comparisons=len(records),direct_sheet_comparisons=len(direct),ordered_sheet_getters=calls,genuine_original_cache_resolutions=len(producers)*224,cache_producers=producers,offsets=old.offset_words,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),library_sha256=sha(a.library),corpus_sha256=sha(gold),source_bindings={str(x.relative_to(REPO)):sha(x) for x in bindings},asset_bindings={str(x.relative_to(REPO)):sha(x) for x in (ASSETS/'character_properties_pyarray.bin',ASSETS/'character_properties_pyarraynames.bin',ASSETS/'character_properties_pystructnames.bin')},monster_property_indices={s:fields.index(s) for s in ('SkillTree','LevelMax','LevelMin','LevelOffset')},mismatches=0,scope=__doc__)
 (ROOT/'reports/character-property-bindings-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','comparisons','direct_sheet_comparisons','ordered_sheet_getters','genuine_original_cache_resolutions','mismatches')}))
if __name__=='__main__':main()
