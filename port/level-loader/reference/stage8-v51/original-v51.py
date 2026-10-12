"""Original Level._LoadScriptFile reverse-search/normalization and original
stage8 control flow execute. Standard std::string transports, debug/fog/global
ScriptManager command bodies and fallback body providers are explicit fixtures.
No LuaScript44 or command execution is supplied/verified."""
from pathlib import Path
import sys,struct,json,re,hashlib
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages');sys.path.insert(0,r'C:/Users/adamc/Desktop/workspace/DH_sc/port/game-data/tests')
from items_differential import Original as Base
from unicorn import UC_HOOK_CODE
d=Path(__file__).parent;elf=Path(r'C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/libDungeonHunter2.so');c=Base(elf,{'functions':[]});level=c.data+0x4000;config=c.data+0x5000;second=c.data+0x6000;argument=c.data+0x7000;vector=c.data+0x8000
events=[];mode='';swap=False;fallback='';attempts=0;stopped=False
def ret(value=0):c.returned(value)
def cstring(at):
 b=bytearray()
 while c.uc.mem_read(at+len(b),1)!=b'\0':b.extend(c.uc.mem_read(at+len(b),1))
 return bytes(b).decode()
def string(at):return bytes(c.uc.mem_read(c.word(at+0x14),c.word(at+0x10)-c.word(at+0x14))).decode()
def setstring(at,value):
 data=value.encode();p=c.heap;c.heap+=(len(data)+32)&~15;c.uc.mem_write(p,data+b'\0');c.uc.mem_write(at,bytes(24));c.pointer(at+0x14,p);c.pointer(at+0x10,p+len(data))
asm=(d.parent/'level-init-v36/level-3f6990.asm').read_text();add=re.search(r'\s+([0-9a-f]+): [0-9a-f]+\s+add\s+r5, pc, r5',asm);ldr=re.search(r'\s+[0-9a-f]+: [0-9a-f]+\s+ldr\s+r5, \[pc, #[^\]]+\].*?@ 0x([0-9a-f]+)',asm);got=int(add.group(1),16)+8+c.word(int(ldr.group(1),16));app=c.word(got+c.word(0x3f7a18));c.uc.mem_write(c.word(got+c.word(0x3f7924)),b'\0')
def hook(uc,at,size,unused):
 global attempts,stopped
 if at==0x3140ec:setstring(c.reg(0),cstring(c.reg(1)));ret(c.reg(0))
 elif at==0x32b918:setstring(c.reg(0),string(c.reg(1)));ret(c.reg(0))
 elif at==0x3139ac:ret()
 elif at==0x3f1ff8:
  obj,pos,n,replacement=c.reg(0),c.reg(1),c.reg(2),c.reg(3);length=c.word(c.uc.reg_read(c.sp));text=bytes(c.uc.mem_read(replacement,length));value=string(obj).encode();setstring(obj,(value[:pos]+text+value[pos+n:]).decode());ret(obj)
 elif at==0x3f1b80:obj=c.reg(0);setstring(obj,string(obj)+cstring(c.reg(1)));ret(obj)
 elif at in (0x45b264,0x45ad40):
  name=cstring(c.reg(1));common=c.reg(2);events.append(('commands' if at==0x45b264 else 'names',name,common))
  if swap and at==0x45ad40 and common:c.pointer(level+0x38,second)
  ret()
 elif at==0x45a1c8:events.append(('unload',));ret()
 elif mode=='stage':
  if at==0x3f6e9c:stopped=True;uc.reg_write(c.sp,c.stack+0xe000);uc.reg_write(c.pc,c.stop)
  elif at==0x337888:ret()
  elif at==0x324114:ret()
  elif at==0x330740:ret(c.data+0x9000)
  elif at==0x337a88:events.append(('debug',string(c.reg(1))));ret(0)
  elif at==0x347100:events.append(('fog',c.reg(2)));ret()
  elif at==0x3f3b40:
   attempts+=1;events.append(('fallback_file',string(c.reg(1)),string(c.reg(2))))
   if attempts==1:ret(0)
   else:
    if fallback=='override':c.pointer(level+0x38,config)
    ret(1)
  elif at==0x34b724:
   events.append(('spawn',cstring(c.reg(2)),cstring(c.reg(3)),c.word(c.uc.reg_read(c.sp)),c.word(c.uc.reg_read(c.sp)+4)));c.pointer(c.reg(0)+8,config);ret(c.reg(0))
  elif at==0x33fdc0:events.append(('resolve',c.reg(1)));ret(config)
  elif at==0x3f150c:events.append(('setconfig',c.reg(1)));c.pointer(level+0x38,c.reg(1));ret()
c.uc.hook_add(UC_HOOK_CODE,hook)
paths=['','x','12345678','.pyscript','a.pyscript','a.PYSCRIPT','data\\scene\\swamp.pyscript','data\\scene\\swamp.pyscript.tail','a.pyscript/b.pyscript','none\\file.lua','abc.pyscrip','abc.pyscripts','abc\\.pyscript\\tail','\\.pyscript','a.pyscript.pyscript','some/relative.pyscript','actual lua .luac','unicode_Ã©.pyscript']
rows=[]
for value in paths:
 setstring(argument,value);events.clear();mode='file';c.invoke(0x3f38a0,[level,argument,0,0],budget=100000);mode='';calls=list(events);assert len(calls) in (0,2);rows.append({'input':value,'normalized':string(argument),'calls':calls})
stages=[]
for fog,missing,variant,change in [(False,False,'',False),(True,False,'',False),(False,True,'default',False),(False,True,'override',False),(False,True,'override_miss',False),(False,False,'',True)]:
 c.uc.mem_write(level,bytes(0x240));c.uc.mem_write(config,bytes(0x300));c.uc.mem_write(second,bytes(0x300));c.pointer(level+0x130,8);c.pointer(level+0x44,0x12345678);c.pointer(config+0xf4,4);setstring(config+0x150,'data\\scene\\swamp.pyscript');setstring(second+0x150,'later\\config.pyscript');c.pointer(level+0x38,0 if missing else config)
 c.pointer(config+0x204,vector);c.pointer(config+0x208,vector+(12 if fog else 0));c.uc.mem_write(config+0x210,struct.pack('<f',200.75));c.pointer(app+0xcc,vector if variant.startswith('override') else 0);c.pointer(app+0xd0,vector+4 if variant.startswith('override') else 0);c.pointer(app+0xe8,c.heap);c.uc.mem_write(c.heap,b'override/source\0');c.heap+=32;events.clear();attempts=0;swap=change;fallback=variant;mode='stage';stopped=False;c.invoke(0x3f6990,[level,0,0,0],budget=300000);mode='';assert stopped and c.word(level+0x130)==9 and c.word(level+0x44)==0x12345678
 stages.append({'fog':fog,'initial_config_missing':missing,'fallback':variant,'common_names_changes_config':change,'events':list(events),'final130':c.word(level+0x130),'lua44_unchanged':True})
report={'validation':'PASS','elf_sha256':hashlib.sha256(elf.read_bytes()).hexdigest(),'original_path_cases':len(rows),'original_stage8_cases':len(stages),'standard_string_transports_are_fixtures':True,'fog_and_ScriptManager_bodies_are_fixtures':True,'lua44_untouched':True,'whole_stage8_body_services_verified':False,'paths':rows,'stages':stages}
(d/'original-v51.json').write_bytes((json.dumps(report,indent=2)+'\n').encode())
def word(v):return struct.pack('<I',v)
def text(v):b=v.encode();return word(len(b))+b
parts=[word(len(rows))]
for row in rows:parts.append(text(row['input'])+text(row['normalized'])+word(bool(row['calls']))+text(row['calls'][0][1] if row['calls'] else '')+text(row['calls'][1][1] if row['calls'] else ''))
(d/'original-path-gold.bin').write_bytes(b''.join(parts));print(json.dumps({k:v for k,v in report.items() if k not in ('paths','stages')}))
