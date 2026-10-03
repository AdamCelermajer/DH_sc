"""Original LoadAnimation/map/vector/identity instructions versus O2 ARM64.

Cache resource identity is an explicit loader service. All ordered appends,
unique map insertion/lookup and first-identity index refresh execute original
instructions. No original map/lookup or registration operation is intercepted.
"""
import argparse,hashlib,json,random,struct,sys,time,zipfile
from pathlib import Path
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'));sys.path.insert(0,str(REPO/'port/level-world/tools'))
from compiled_transforms_differential import Cpu,word,words,sha,i32
from prepare_actors import strings

def token_words(token):return words([token&0xffffffff,token>>32])
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--engine',type=Path,required=True);p.add_argument('--library',type=Path,required=True)
 p.add_argument('--report',type=Path,default=ROOT/'reports/animation-registration-arm64-differential.json');p.add_argument('--reference-output',type=Path,default=ROOT/'reference/animation-registration/original-corpus.bin');a=p.parse_args();started=time.monotonic()
 producer_path=ROOT/'reference/prince-registration/probe.json';producer=json.loads(producer_path.read_text());assert producer['validation']=='PASS'
 manifest_path=ROOT/'reference/prince-registration/original-functions.json';manifest=json.loads(manifest_path.read_text());assert sha(a.engine)==manifest['original_sha256']
 old=Cpu(a.engine,False,manifest);dictionary=old.data+0x10000;chars=old.data+0x20000
 with zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as archive:
  paths,_=strings(archive.read('com.gameloft.android.GAND.GloftD2SS/files/data/pydata/animations_dictionary_pyarray.bin'))
 for index,path in enumerate(paths):
  raw=path.encode()+b'\0';old.uc.mem_write(chars,raw);old.uc.mem_write(dictionary+index*12,words([0,0,chars]));chars+=len(raw)
 got=0x3659fc+8+word(bytes(old.uc.mem_read(0x365cf4,4)),0)
 for literal,value in ((0x365d00,len(paths)),(0x365d0c,dictionary)):
  slot=got+word(bytes(old.uc.mem_read(literal,4)),0);global_=word(bytes(old.uc.mem_read(slot,4)),0);old.uc.mem_write(global_,words([value]))
 obj=old.data+0x1000;tree=obj+8;dyn=obj+0x200;storage=obj+0x1000;key=obj+0x1800;input_db=obj+0x1900
 identity_handles={};active_token=1
 def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
 def services(uc,address,size,user):
  if address in (0x3136b4,0x3136b8,0x3139ac,0x337888,0x337a88):ret()
  elif address in (0x3116e8,0x3109e0):
   dest,begin,end=[old.reg(i) for i in range(3)];n=end-begin;assert 0<=n<4096
   raw=bytes(uc.mem_read(begin,n));pointer=old.heap;old.heap+=(n+16)&~15;uc.mem_write(pointer,raw+b'\0');uc.mem_write(dest,bytes(24));old.pointer(dest+16,pointer+n);old.pointer(dest+20,pointer);ret(dest)
  elif address==0x60f25c:
   assert old.string(old.reg(1)).decode() in paths
   if active_token not in identity_handles:
    pointer=old.data+0x500000+len(identity_handles)*0x100;uc.mem_write(pointer,bytes(0x100));uc.mem_write(pointer+4,words([100000]));identity_handles[active_token]=pointer
   pointer=identity_handles[active_token];uc.mem_write(old.reg(0),words([pointer,pointer+0x80]));ret(old.reg(0))
  elif address==0x60b0cc:ret(123)
  elif address in (0x310568,0x310570):
   n=old.reg(0);pointer=old.heap;old.heap+=(n+15)&~15;uc.mem_write(pointer,bytes(n));ret(pointer)
 old.uc.hook_add(UC_HOOK_CODE,services)
 rng=random.Random(0xA1580116);cases=[]
 actual=[(r['clip_id'],r['clip_id']+1) for r in producer['registration_calls']];cases.append(actual)
 cases.extend([[],[(1,1)],[(10,1),(20,1),(10,2)],[(10,1),(10,2),(20,2),(30,1)],[(0,1),(1200,(1<<48)+2),(0,(1<<48)+3)]])
 for case in range(36):cases.append([(rng.randrange(0,35),rng.choice([1,2,3,4,5,(1<<48)+1,(1<<63)+7])) for _ in range(rng.randrange(1,129))])
 gold=words([0x31524741,len(cases)]);appends=lookups=entry_checks=refreshes=default_checks=0;summaries=[]
 for ci,requests in enumerate(cases):
  old.heap=old.data+0x800000;identity_handles={};old.uc.mem_write(obj,bytes(0x100));old.uc.mem_write(tree,words([0,0,tree,tree,0]));old.pointer(obj+0x20,dyn)
  old.invoke(0x3648c4,[dyn]);old.pointer(dyn+0x24,storage);old.pointer(dyn+0x28,storage);old.pointer(dyn+0x2c,storage+8*1024)
  new=Cpu(a.library,True,{'functions':[]});fixture=new.invoke('dh2_registration_create',[]);scratch=new.data+0x1000;gold+=words([len(requests)])
  for index,(cid,token) in enumerate(requests):
   active_token=token;value=old.invoke(0x3659ec,[obj,cid],budget=2000000)
   source_index=word(bytes(old.uc.mem_read(value+0x20,4)),0);assert new.invoke('dh2_registration_append',[fixture,cid,token,1])==1
   assert i32(new.invoke('dh2_registration_lookup',[fixture,cid]))==i32(source_index),(ci,index,cid,token,source_index)
   assert (word(bytes(old.uc.mem_read(dyn+0x28,4)),0)-storage)//8==index+1
   assert new.invoke('dh2_registration_record',[fixture,index,0,scratch])==1
   assert bytes(new.uc.mem_read(scratch,16))==words([cid,index])+token_words(token)
   gold+=words([cid])+token_words(token)+words([source_index]);appends+=1;lookups+=1
  def inspect():
   nonlocal gold,entry_checks
   reverse={v:k for k,v in identity_handles.items()};entries=[]
   def visit(pointer):
    if not pointer:return
    visit(word(bytes(old.uc.mem_read(pointer+8,4)),0))
    cid=word(bytes(old.uc.mem_read(pointer+16,4)),0);index=word(bytes(old.uc.mem_read(pointer+0x34,4)),0);handle=word(bytes(old.uc.mem_read(pointer+0x2c,4)),0)
    entries.append(words([cid,index])+token_words(reverse[handle]));visit(word(bytes(old.uc.mem_read(pointer+12,4)),0))
   visit(word(bytes(old.uc.mem_read(tree+4,4)),0));assert new.invoke('dh2_registration_count',[fixture,1])==len(entries)
   gold+=words([len(entries)])
   for ei,expected in enumerate(entries):
    assert new.invoke('dh2_registration_record',[fixture,ei,1,scratch])==1
    assert bytes(new.uc.mem_read(scratch,16))==expected,(ci,ei,expected.hex(),bytes(new.uc.mem_read(scratch,16)).hex())
    gold+=expected;entry_checks+=1
   return len(entries)
  count=inspect();old.invoke(0x364af4,[obj]);new.invoke('dh2_registration_refresh',[fixture]);inspect();refreshes+=1
  active_token=0x7000000000000001+ci
  handle=old.data+0x600000;old.uc.mem_write(handle,bytes(0x100));old.uc.mem_write(handle+4,words([1000]));old.uc.mem_write(input_db,words([handle,handle+0x80]));old.invoke(0x62fc90,[dyn,input_db]);assert word(bytes(old.uc.mem_read(dyn+0x68,4)),0)==handle
  assert new.invoke('dh2_registration_default',[fixture,active_token,1])==1 and new.invoke('dh2_registration_default_identity',[fixture])==active_token;gold+=token_words(active_token);default_checks+=1
  before=new.invoke('dh2_registration_count',[fixture,0]);beforemap=new.invoke('dh2_registration_count',[fixture,1]);beforedefault=new.invoke('dh2_registration_default_identity',[fixture])
  for function,args in [('dh2_registration_append',[fixture,0xffffffff,1,1]),('dh2_registration_append',[fixture,1,0,1]),('dh2_registration_append',[fixture,1,1,0]),('dh2_registration_default',[fixture,0,1]),('dh2_registration_default',[fixture,1,0])]:
   assert new.invoke(function,args)==0 and new.invoke('dh2_registration_count',[fixture,0])==before and new.invoke('dh2_registration_count',[fixture,1])==beforemap and new.invoke('dh2_registration_default_identity',[fixture])==beforedefault
  new.invoke('dh2_registration_destroy',[fixture]);summaries.append({'case':ci,'requests':len(requests),'unique_dictionary_entries':count})
  if ci%10==0:print(json.dumps(summaries[-1]),flush=True)
 a.reference_output.parent.mkdir(parents=True,exist_ok=True);a.reference_output.write_bytes(gold)
 sources=[ROOT/'animation_registration.hpp',ROOT/'animation_registration.cpp',ROOT/'tests/animation_registration.cpp',Path(__file__)]
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'original_instructions_executed':True,'compiled_arm64_instructions_executed':True,'manifest_sha256':sha(manifest_path),'producer_report_sha256':sha(producer_path),'arm64_library_sha256':sha(a.library),'reference_sha256':sha(a.reference_output),'source_sha256':{str(path.relative_to(REPO)).replace('\\','/'):sha(path) for path in sources},'cases':summaries,'appends':appends,'lookups':lookups,'entry_checks':entry_checks,'refreshes':refreshes,'default_checks':default_checks,'atomic_rejections':len(cases)*5,'mismatches':0,'elapsed_seconds':round(time.monotonic()-started,2),'scope':'Actual original LoadAnimation, database append, unique red-black game-ID insertion/lookup, UpdateAnimationIndices and first-word CCDB identity matching versus native occurrence adapter. Full158 Prince registration requests are replayed, plus aliases/different resource identities/unsorted/repeated IDs and high64-bit migrated identity tokens. Resource loading/identity, string storage, allocation/profiling/clock are explicit services. No pose compiler/component support, cache destruction, events, timeline or renderer integration claim.'}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k] for k in ('validation','appends','lookups','entry_checks','refreshes','default_checks','mismatches','elapsed_seconds')}))
if __name__=='__main__':main()
