"""Read actual effects tables with original row/step loaders and trace FX77.

Allocator and raw stream reads are explicit desktop fixtures. Registration,
typed row loading, module lookup/default insertion and ordered unique queue
instructions execute; no scene factory/rendering service is accepted here.
"""
import argparse, hashlib, json, struct, sys, zipfile
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent; REPO=HERE.parents[3]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
sys.path.insert(0,str(REPO/'port/level-world/tools'))
from compiled_transforms_differential import Cpu,words,word
from prepare_actors import strings
ORIGINAL='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
CACHE='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
def sha(b):return hashlib.sha256(b).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--engine',type=Path,default=REPO/'.local-inputs/libDungeonHunter2.so');p.add_argument('--cache',type=Path,default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'));p.add_argument('--output',type=Path,default=HERE/'probe.json');a=p.parse_args()
 assert sha(a.engine.read_bytes())==ORIGINAL
 with a.cache.open('rb')as f:assert hashlib.file_digest(f,'sha256').hexdigest()==CACHE
 manifest=json.loads((HERE/'original-functions.json').read_text());cpu=Cpu(a.engine,False,manifest)
 inputs={};resources={}
 with zipfile.ZipFile(a.cache)as z:
  index={}
  for i in z.infolist():index.setdefault(i.filename.lower(),[]).append(i)
  def read(path):
   rows=index['com.gameloft.android.gand.gloftd2ss/files/'+path.lower()];assert len(rows)==1
   b=z.read(rows[0]);inputs[path]={'entry':rows[0].filename,'bytes':len(b),'sha256':sha(b)};return b
  raw=read('data/pydata/effects_pyarray.bin');names,_=strings(read('data/pydata/effects_pyarraynames.bin'));paths,_=strings(read('data/pydata/effects_dictionary_pyarray.bin'));dictnames,_=strings(read('data/pydata/effects_dictionary_pyarraynames.bin'));schema=read('data/pydata/effects_pystructnames.bin')
  stream={'raw':raw,'offset':0};trace=[];step_calls=0
  def ret(v=0):cpu.put(0,v);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
  def alloc(n):
   assert 0<n<0x200000
   q=(cpu.heap+15)&~15;cpu.heap=q+n;cpu.uc.mem_write(q,bytes(n));return q
  def take(n):
   o=stream['offset'];b=stream['raw'][o:o+n];assert len(b)==n;stream['offset']+=n;return b
  def service(uc,address,size,user):
   nonlocal step_calls
   if address==0x506990:step_calls+=1
   elif address in (0x313a90,0x4db89c,0x459090,0x3df1a0,0x4db94c):
    n=1 if address==0x4db89c else 4;b=take(n)
    if address==0x313a90:ret(word(b,0))
    else:cpu.uc.mem_write(cpu.reg(1),b);ret()
   elif address==0x317454:cpu.uc.mem_write(cpu.reg(1),take(cpu.reg(2)));ret()
   elif address in (0x31056c,0x310570):ret(alloc(cpu.reg(0)))
   elif address==0x310440:ret()
  cpu.uc.hook_add(UC_HOOK_CODE,service)
  cpu.invoke(0x4bc504,[cpu.data+0x1000]);consumed=stream['offset']
  sets_count=word(bytes(cpu.uc.mem_read(cpu.symbols['_ZN6Arrays19AnimatedEffectTable4sizeE'],4)),0)
  sets_ptr=word(bytes(cpu.uc.mem_read(cpu.symbols['_ZN6Arrays19AnimatedEffectTable7membersE'],4)),0)
  assert sets_count==len(names)==276
  sets=[]
  for i in range(sets_count):
   b=bytes(cpu.uc.mem_read(sets_ptr+i*24,24));steps=[]
   for j in range(word(b,12)):
    sb=bytes(cpu.uc.mem_read(word(b,16)+j*48,48));s=list(struct.unpack('<12I',sb));s[0]=0;s[11]=0
    text=cpu.string(word(sb,44)).decode();steps.append({'words':s,'subobject':text,'effect_id':struct.unpack_from('<i',sb,4)[0],'type':struct.unpack_from('<i',sb,28)[0]})
   sets.append({'index':i,'name':names[i],'force_cache':b[4],'loop':struct.unpack_from('<i',b,8)[0],'type':struct.unpack_from('<i',b,20)[0],'steps':steps})
  assert sets[77]['name']=='Zombie_spawn';selected=sets[77]
  for s in selected['steps']:
   if s['type']!=1 and s['effect_id']>=0:
    i=s['effect_id'];blob=read(paths[i]);resources[str(i)]={'dictionary_id':i,'name':dictnames[i],'path':paths[i],**inputs[paths[i]],'header':blob[:40].hex()}
  # Original registration invokes genuine GetModule. String/STL insertion is
  # explicit allocation/storage fixture, with original miss/default body run.
  manager=cpu.data+0x2000;cpu.uc.mem_write(manager,bytes(56));queue=cpu.data+0x3000;cpu.uc.mem_write(queue,bytes(4096));cpu.uc.mem_write(manager+16,words([queue,queue,queue+4096]))
  debug=cpu.symbols['_ZN13DebugSwitches6s_instE'];cpu.invoke(0x335fcc,[debug]);module={};switch={};queue_calls=[]
  def cstring(p):return cpu.string(p).decode()
  def strview(p):return cstring(word(bytes(cpu.uc.mem_read(p,4)),0))
  def registration(uc,address,size,user):
   if address==0x337888:trace.append(['load']);ret()
   elif address==0x3140ec:
    dst,src=cpu.reg(0),cpu.reg(1);cpu.uc.mem_write(dst,words([src,src+len(cpu.string(src)),0,0,0,dst]));ret(dst)
   elif address in (0x318254,0x3139ac):ret()
   elif address==0x3369a8:
    key=strview(cpu.reg(1));trace.append(['module_find',key]);ret(module.get(key,cpu.reg(0)))
   elif address==0x337288:
    key=strview(cpu.reg(1));slot=alloc(48);module[key]=slot;trace.append(['module_insert',key]);ret(slot+40)
   elif address==0x337a88:
    key=strview(cpu.reg(1));trace.append(['switch',key]);ret(switch.setdefault(key,0))
   elif address==0x49437c:
    i=struct.unpack('<i',bytes(cpu.uc.mem_read(cpu.reg(1),4)))[0];queue_calls.append(i);trace.append(['queue',i]);ret()
  # EffectDict row count is enough here; dictionary loading is separately
  # described by its actual read body, no factory is invoked.
  cpu.uc.mem_write(cpu.symbols['_ZN6Arrays10EffectDict4sizeE'],words([len(paths)]))
  cpu.uc.hook_add(UC_HOOK_CODE,registration)
  cpu.invoke(0x4967e8,[manager,77]);first=list(trace);first_calls=list(queue_calls)
  trace.clear();queue_calls.clear();cpu.invoke(0x4967e8,[manager,77]);second=list(trace)
  assert first_calls==[283],first_calls
  assert word(bytes(cpu.uc.mem_read(module['AnimatedFX']+40,4)),0)&255==1
  expected_names=[];o=0
  while o<len(schema):s,n=strings(schema[o:]);expected_names.append(s);o+=n
  report={'validation':'PASS','original_sha256':ORIGINAL,'cache_sha256':CACHE,'script_sha256':sha(Path(__file__).read_bytes()),'manifest_sha256':sha((HERE/'original-functions.json').read_bytes()),'inputs':inputs,'schema':expected_names,'table_count':sets_count,'data_consumed':consumed,'actual_step_read_calls':step_calls,'selected_set':selected,'resources':resources,'first_registration_trace':first,'second_registration_trace':second,'queue_requests':first_calls,'unique_queue_expected':list(dict.fromkeys(first_calls)),'scope':__doc__}
  a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:report[k]for k in ('validation','table_count','data_consumed','actual_step_read_calls','queue_requests')}))
if __name__=='__main__':main()
