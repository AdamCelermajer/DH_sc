"""Execute original LoadAnimation, database append, unique map insertion/lookup.

File loading supplies identity-preserving CCDB handles. String storage and
allocator/profiling/timer services are fixtures; map and append are actual code.
"""
import hashlib,json,struct,sys,zipfile
from pathlib import Path
from unicorn import UC_HOOK_CODE
HERE=Path(__file__).resolve().parent;REPO=HERE.parents[3]
sys.path.insert(0,str(REPO/'port/engine-animation/tests'))
sys.path.insert(0,str(REPO/'port/level-world/tools'))
from compiled_transforms_differential import Cpu,word,words
from prepare_actors import strings

def main():
 manifest=json.loads((HERE/'original-functions.json').read_text());engine=REPO/'.local-inputs/libDungeonHunter2.so'
 assert hashlib.sha256(engine.read_bytes()).hexdigest()==manifest['original_sha256']
 cpu=Cpu(engine,False,manifest);dictionary=cpu.data+0x10000;chars=cpu.data+0x20000
 with zipfile.ZipFile('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip') as archive:
  paths,_=strings(archive.read('com.gameloft.android.GAND.GloftD2SS/files/data/pydata/animations_dictionary_pyarray.bin'))
 for i,path in enumerate(paths):
  raw=path.encode()+b'\0';cpu.uc.mem_write(chars,raw);cpu.uc.mem_write(dictionary+i*12,words([0,0,chars]));chars+=len(raw)
 got=0x3659fc+8+word(bytes(cpu.uc.mem_read(0x365cf4,4)),0)
 for literal,value in ((0x365d00,len(paths)),(0x365d0c,dictionary)):
  slot=got+word(bytes(cpu.uc.mem_read(literal,4)),0);global_=word(bytes(cpu.uc.mem_read(slot,4)),0);cpu.uc.mem_write(global_,words([value]))
 obj=cpu.data+0x1000;tree=obj+8;dyn=obj+0x200;storage=obj+0x1000
 cpu.uc.mem_write(obj,bytes(0x100));cpu.uc.mem_write(tree,words([0,0,tree,tree,0]));cpu.pointer(obj+0x20,dyn)
 cpu.invoke(0x3648c4,[dyn]);cpu.pointer(dyn+0x24,storage);cpu.pointer(dyn+0x28,storage);cpu.pointer(dyn+0x2c,storage+8*256)
 handles={};loads=[];registrations=[];allocations=[]
 def ret(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 def string(destination,begin,end):
  n=end-begin;assert 0<=n<4096,(hex(begin),hex(end));value=bytes(cpu.uc.mem_read(begin,n));pointer=cpu.heap;cpu.heap+=(n+16)&~15
  cpu.uc.mem_write(pointer,value+b'\0');cpu.uc.mem_write(destination,bytes(24));cpu.pointer(destination+16,pointer+n);cpu.pointer(destination+20,pointer)
 def services(uc,address,size,user):
  if address in (0x3136b4,0x3136b8,0x3139ac):ret()
  elif address==0x475404:ret(1) # already-created manager set; common registration body executes
  elif address==0x476058:ret(obj) # manager set lookup, not the Animation clip-key map
  elif address in (0x3116e8,0x3109e0):string(cpu.reg(0),cpu.reg(1),cpu.reg(2));ret(cpu.reg(0))
  elif address==0x60f25c:
   path=cpu.string(cpu.reg(1)).decode();assert path in paths
   if path not in handles:
    control=cpu.data+0x500000+len(handles)*0x100;cpu.uc.mem_write(control,bytes(0x100));cpu.uc.mem_write(control+4,words([1000]));handles[path]=(control,control+0x80)
   cpu.uc.mem_write(cpu.reg(0),words(handles[path]));loads.append(path);ret(cpu.reg(0))
  elif address==0x60b0cc:ret(123)
  elif address in (0x310568,0x310570):
   n=cpu.reg(0);pointer=cpu.heap;cpu.heap+=(n+15)&~15;cpu.uc.mem_write(pointer,bytes(n));allocations.append(n);ret(pointer)
 cpu.uc.hook_add(UC_HOOK_CODE,services)
 requested=(1111,1040,1040,1040,1040,1040,1041)
 for cid in requested:
  try:value=cpu.invoke(0x3659ec,[obj,cid],budget=2000000)
  except Exception:
   print('PC',hex(cpu.uc.reg_read(cpu.pc)),[hex(cpu.reg(i)) for i in range(4)]);raise
  count=(word(bytes(cpu.uc.mem_read(dyn+0x28,4)),0)-storage)//8
  registrations.append({'clip_id':cid,'library_count':count,'animation_map_index':word(bytes(cpu.uc.mem_read(value+0x20,4)),0),'returned_animation':hex(value)})
 assert [x['library_count'] for x in registrations]==list(range(1,8))
 assert [x['animation_map_index'] for x in registrations]==[0,1,1,1,1,1,6]
 cpu.invoke(0x364af4,[obj]);post=[]
 key=obj+0x100
 for cid in (1111,1040,1041):
  cpu.uc.mem_write(key,words([cid]));value=cpu.invoke(0x36583c,[tree,key]);post.append({'clip_id':cid,'animation_map_index':word(bytes(cpu.uc.mem_read(value+0x20,4)),0)})
 assert [x['animation_map_index'] for x in post]==[0,1,6]
 before=len(loads);cpu.invoke(0x476398,[cpu.data+0x9000,12302,1111],budget=2000000)
 template_loads=loads[before:];assert template_loads==[paths[1111],paths[1111]]
 assert tuple(struct.unpack('<II',cpu.uc.mem_read(dyn+0x68,8)))==handles[paths[1111]]
 assert (word(bytes(cpu.uc.mem_read(dyn+0x28,4)),0)-storage)//8==8
 report={'validation':'PASS','original_sha256':manifest['original_sha256'],'manifest_sha256':hashlib.sha256((HERE/'original-functions.json').read_bytes()).hexdigest(),'script_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'original_instructions_executed':True,'requested_clip_ids':requested,'registrations':registrations,'post_update_indices':post,'resource_loader_paths':loads,'unique_map_entries':word(bytes(cpu.uc.mem_read(tree+16,4)),0),'database_vector_order':[paths[cid] for cid in requested],'add_template':{'manager_set_exists_fixture':True,'clip_id':1111,'loads':template_loads,'registered_library_index':7,'default_resource_path':paths[1111],'default_ref_handle':list(handles[paths[1111]])},'allocations':allocations,'imports':cpu.import_calls,'scope':'Actual LoadAnimation, original dynamic database append, actual unique red-black-map insert/lookup and UpdateAnimationIndices; actual AddTemplateAnim common registration body and setDefaultAnimationLibrary. Manager lookup uses an already-created set fixture; resource loader preserves repeated path identity; string storage, allocation, profiling and clock are explicit services. No real file-cache loader or ownership/destructor parity claim.'}
 (HERE/'load-probe.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','loads':len(loads),'database_entries':len(requested),'unique_map_entries':report['unique_map_entries']}))
if __name__=='__main__':main()
