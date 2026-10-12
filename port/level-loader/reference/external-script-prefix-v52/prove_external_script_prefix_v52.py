from pathlib import Path
import sys,struct,json,hashlib,zipfile,subprocess
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(root/'port/engine-resources/tests'))
from cpu import Cpu
from unicorn import UC_HOOK_CODE
b=Path(__file__).resolve().parents[2];ref=Path(__file__).parent;elf=root/'.local-inputs/libDungeonHunter2.so';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(elf)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
c=Cpu(elf,False,{'functions':[]});actor=c.data+0x1000;oldscript=c.data+0x3000;vt=c.data+0x4000;nameptr=c.data+0x5000;path=c.data+0x6000;c.uc.mem_write(path,b'data/scripts/objects/\0');c.pointer(oldscript,vt);c.pointer(vt+4,c.callback)
deletes=[];required=False
callback=c.data+0x1f000;c.pointer(callback,0xe12fff1e);c.pointer(vt+4,callback)
def actual_hook(uc,a,n,u):
 global required
 if a==callback:
  assert c.reg(0)==oldscript;deletes.append(c.reg(0));c.pointer(actor+0x300,77);c.put(0,0);uc.reg_write(c.pc,uc.reg_read(c.lr))
 elif a==0x38efac:required=True;uc.emu_stop()
c.uc.hook_add(UC_HOOK_CODE,actual_hook)
cases=[]
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
with zipfile.ZipFile(cache) as z:raw=z.read('com.gameloft.android.GAND.GloftD2SS/files/data/pydata/game_objects_pyarray.bin')[3031:5007]
at=4;authored=[]
for i in range(68):
 at+=9;loot,n=struct.unpack_from('<ii',raw,at);at+=8;authored.append(raw[at:at+n].decode());at+=n+12
assert at==len(raw)
for name in [None,'','positive-required.lua']+authored:
 for prior in [False,True]:
  c.pointer(actor+0x300,oldscript if prior else 0);deletes.clear();required=False
  if name is not None:c.uc.mem_write(nameptr,name.encode()+b'\0')
  c.uc.reg_write(c.sp,c.stack+0xe000);c.uc.reg_write(c.lr,c.stop);c.put(0,actor);c.put(1,0 if name is None else nameptr);c.put(2,path)
  c.uc.emu_start(0x38ef60,c.stop,count=1000)
  nonempty=bool(name);assert required==nonempty;assert c.uc.reg_read(c.pc)==(0x38efac if nonempty else c.stop)
  assert struct.unpack('<I',c.uc.mem_read(actor+0x300,4))[0]==0;assert len(deletes)==int(prior)
  cases.append({'filename':name,'prior_script':prior,'deleting_destructor_observer_calls':len(deletes),'source300_cleared_before_name_gate':True,'whole_method_completed':not nonempty,'required_positive_suffix':nonempty})
receipt={'validation':'PASS','original_sha256':sha(elf),'cache_sha256':sha(cache),'source_entry':'38ef60','required_suffix':'38efac','original_cases':len(cases),'authored_container_script_names_checked':68,'authored_nonempty_script_names':sum(bool(x) for x in authored),'old_script_delete_before_null_empty_gate':True,'old_field_overwritten_after_callback_mutation':True,'whole_empty_method_verified_with_deleting_dtor_observer':True,'nonempty_suffix_not_executed':True,'observers':'Attached script virtual deleting destructor observer (mutates field300 to77), no actual VM/backend. Original control/stores run. Positive branch stops before allocation38efac. No full Container InitPost claim.','cases':cases}
(b/'reports/external-script-prefix-v52-original.json').write_text(json.dumps(receipt,indent=2)+'\n')
obj='C:/Users/adamc/AppData/Local/Android/Sdk/ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-objdump.exe';asm=subprocess.run([obj,'-d','--start-address=0x38ef60','--stop-address=0x38efac',str(elf)],capture_output=True,text=True,check=True).stdout;(ref/'original-prefix.asm').write_text(asm)
print(json.dumps({k:v for k,v in receipt.items() if k!='cases'}))
