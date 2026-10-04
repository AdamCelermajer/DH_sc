"""Actual registered map insertion/GetOID and six real name getter bodies vs O2 ARM64.

Names are borrowed from genuine decoded cache byte streams. Original owned RB
map/string helpers execute; allocator/libc imports are explicit services.
Whole Application/loader ownership and other registered getters are not claimed.
"""
import argparse,hashlib,json,struct,sys
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1];REF=ROOT/'reference/game-design-tables';ASSETS=REPO/'port/android-native/app/src/main/assets/data'
sys.path.insert(0,str(REPO/'port/script-runtime/tests'));sys.path.insert(0,str(REPO/'port/level-world/tests'))
from script_constants_original_probe import Oracle,words
from items_differential import strings
from visual_timeline_differential import TimelineCpu
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def field(b):return words(len(b))+b
class Native(TimelineCpu):
 def external(self,uc,address,size,unused):
  if self.imports.get(address)=='strcmp':
   def text(p):
    b=b''
    while uc.mem_read(p+len(b),1)!=b'\0':b+=bytes(uc.mem_read(p+len(b),1))
    return b
   a,b=text(self.reg(0)),text(self.reg(1));self.put(0,(a>b)-(a<b));uc.reg_write(self.pc,uc.reg_read(self.lr))
  else:super().external(uc,address,size,unused)
def main():
 p=argparse.ArgumentParser();p.add_argument('--library',type=Path,default=REPO/'.local-inputs/game-design-tables-discovery/libgame_design_tables.so');a=p.parse_args();buildpath=a.library.parent/'arm64-build.json';build=json.loads(buildpath.read_text(encoding='utf-8-sig'));assert sha(a.library)==build['library_sha256'];assert all(sha(Path(x))==h for x,h in build['source_bindings'].items())
 registry_path=REPO/'port/script-runtime/reference/design-bindings/registry-probe.json';registry=json.loads(registry_path.read_text());wanted={'CharacterProperties','CharacterTable','AIProps','AITable','ClassTable','ClassFuncList'}
 records=[r for r in registry['registrations'] if r['name'] in wanted];assert len(records)==6
 specs={
 'CharacterProperties':('character_properties_pystructnames.bin',0,0),
 'CharacterTable':('character_properties_pyarraynames.bin',0,1),
 'AIProps':('ai_pystructnames.bin',0,0),'AITable':('ai_pyarraynames.bin',0,1),
 'ClassTable':('character_classes_pyarraynames.bin',0,1),'ClassFuncList':('character_classes_pystructnames.bin',1,0)}
 tables=[dict(name=r['name'],address=int(r['getter'],0),kind=specs[r['name']][2],names=strings((ASSETS/specs[r['name']][0]).read_bytes())[specs[r['name']][1]]) for r in records]
 old=Oracle();header=old.manager+0x1c;old.uc.mem_write(old.manager,bytes(256));old.uc.mem_write(header,words(0,0,header,header,0));new=Native(a.library,True,{'functions':[]});out=new.data+0x1000;view=out+32;registered=out+128;arg0=new.data+0x80000;arg1=arg0+1024;oldarg0=old.data+0x90000;oldarg1=oldarg0+1024
 used=set();getter_calls=0
 def observe(uc,address,size,_):
  nonlocal getter_calls
  if address in [t['address'] for t in tables]:getter_calls+=1
 old.uc.hook_add(UC_HOOK_CODE,observe)
 def table_setup(index,names):
  t=tables[index];f=t['address'];op=old.data+0x200000+index*0x40000;np=new.data+0x200000+index*0x40000;array=np+128;oldarray=op+128
  for j,s in enumerate(names):
   oldp=op+4096+j*256;newp=np+8192+j*256;old.uc.mem_write(oldp,s+b'\0');new.uc.mem_write(newp,s+b'\0');new.pointer(array+8*j,newp);old.pointer(oldarray+4*j,oldp)
  new.uc.mem_write(np,struct.pack('<QII',array,len(names),0));t['native']=np
  if t['kind']:
   got=f+0x14+old.word(f+0x68);old.pointer(old.word(got+old.word(f+0x6c)),len(names));old.pointer(old.word(got+old.word(f+0x70)),oldarray)
  else:
   offsets={0x4af110:(0x54,0x58),0x4af590:(0x50,0x54),0x4af1e0:(0x28,0x2c)};x,y=offsets[f];got=f+0x14+old.word(f+x);base=old.word(got+old.word(f+y))
   for j,s in enumerate(names):old.pointer(base+24*j+20,op+4096+j*256)
 for i,t in enumerate(tables):table_setup(i,t['names'])
 registrations=[];actions=[];queries=direct=0
 def lookup(group,key):
  nonlocal queries
  old.uc.mem_write(oldarg0,group+b'\0');old.uc.mem_write(oldarg1,key+b'\0');new.uc.mem_write(arg0,group+b'\0');new.uc.mem_write(arg1,key+b'\0');expected=old.invoke(0x4bd640,[old.manager,oldarg0,oldarg1]);new.uc.mem_write(out,words(0xabcdef12));assert new.invoke('dh2_game_design_tables_lookup',[view,1,arg0,arg1,out])==0;actual=struct.unpack('<I',new.uc.mem_read(out,4))[0];assert actual==expected,(group,key,expected,actual)
  actions.append(words(1)+field(group)+field(key)+words(expected));queries+=1
 def register(group,index):
  old.uc.mem_write(oldarg0,group+b'\0');old.invoke(0x4bdccc,[old.manager,oldarg0,tables[index]['address']]);registrations.append((group,index));j=len(registrations)-1;newp=new.data+0x90000+j*1024;new.uc.mem_write(newp,group+b'\0');new.uc.mem_write(registered+24*j,struct.pack('<3Q',newp,tables[index]['native'],0));new.uc.mem_write(view,struct.pack('<QII',registered,len(registrations),0));actions.append(words(0,index)+field(group))
 for i,t in enumerate(tables):
  register(t['name'].encode(),i)
  keys=t['names']+[b'',b'not_an_original_member',t['names'][0].swapcase(),t['names'][0]+b'\0ignored',b'\xff']
  for key in keys:
   lookup(t['name'].encode(),key);old.uc.mem_write(oldarg1,key+b'\0');expected=old.invoke(t['address'],[oldarg1]);new.uc.mem_write(arg1,key+b'\0');assert new.invoke('dh2_game_design_find',[out,t['native'],arg1])==0 and struct.unpack('<I',new.uc.mem_read(out,4))[0]==expected;direct+=1
  lookup(t['name'].encode().swapcase(),t['names'][0]);lookup(t['name'].encode()+b'\0ignored',t['names'][-1])
 # Same key registrations replace getter selection; order, case and first NUL.
 for group,index in [(b'Duplicate',0),(b'Duplicate',2),(b'Duplicate\0tail',3),(b'duplicate',1),(b'bytes\xff',5)]:
  register(group,index)
  for t in tables:lookup(group,t['names'][0])
 # Borrowed duplicate name rows use the earliest match, not map overwriting.
 changes=[]
 for index in (i for i,t in enumerate(tables) if len(t['names'])>1):
  names=tables[index]['names'].copy();names[1]=names[0];table_setup(index,names);actions.append(words(2,index,1)+field(names[0]));changes.append(index);lookup(tables[index]['name'].encode(),names[0]);lookup(tables[index]['name'].encode(),tables[index]['names'][1])
 # Actual array count producers are reread by each getter; zero-row miss.
 for index in (i for i,t in enumerate(tables) if t['kind']):
  table_setup(index,[]);actions.append(words(3,index));lookup(tables[index]['name'].encode(),b'')
 lookup(b'absent_group',b'absent_member')
 gold=b'GDT1'+words(len(tables),len(actions))
 for t in tables:gold+=field(t['name'].encode())+words(t['address'],t['kind'],len(t['names']))+b''.join(field(s) for s in t['names'])
 gold+=b''.join(actions);dest=REF/'table-fixtures.bin';dest.write_bytes(gold)
 assert sha(a.library)==build['library_sha256'] and all(sha(Path(x))==h for x,h in build['source_bindings'].items())
 report=dict(validation='PASS',scope=__doc__,original_sha256=sha(REPO/'.local-inputs/libDungeonHunter2.so'),compiler_inputs=build,manifest_sha256=sha(REF/'original-functions.json'),registry_producer_sha256=sha(registry_path),gold_sha256=sha(dest),query_comparisons=queries,direct_getter_comparisons=direct,original_registrations=len(registrations),original_getter_calls=getter_calls,original_allocator_calls=old.allocations,mutable_duplicate_tables=changes,asset_bindings={specs[t['name']][0]:sha(ASSETS/specs[t['name']][0]) for t in tables},source_bindings={str(x.relative_to(REPO)):sha(x) for x in (ROOT/'game_design_tables.cpp',ROOT/'game_design_tables.hpp',Path(__file__))},mismatches=0,other_registered_getters_proved=False,whole_application_ownership_proved=False)
 (ROOT/'reports/game-design-tables-arm64-differential.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','query_comparisons','direct_getter_comparisons','original_registrations','original_getter_calls','mismatches')}))
if __name__=='__main__':main()
