"""Focused original-instruction ownership probes; allocator/VM/library/register services explicit."""
import hashlib,json,struct,sys,zipfile
from pathlib import Path
from elftools.elf.elffile import ELFFile
from unicorn import UC_HOOK_CODE
ROOT=Path(__file__).resolve().parents[2]
REPO=ROOT.parents[1]
sys.path.insert(0,str(ROOT/'tests'))
from character_script_selection_differential import Cpu,string
HERE=Path(__file__).resolve().parent
ENGINE=REPO/'.local-inputs/libDungeonHunter2.so'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
manifest=json.loads((HERE/'original-functions.json').read_text())
cpu=Cpu(ENGINE,False,manifest)
def w(a):return struct.unpack('<I',cpu.uc.mem_read(a,4))[0]
def ret(value=0):cpu.put(0,value);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
names={v:k for k,v in cpu.symbols.items()}
trace=[];next_vm=cpu.data+0x80000;next_alloc=cpu.data+0xa0000
AI=cpu.data+0x1000;CHAR=cpu.data+0x5000;VT=cpu.data+0x7000
ROW=cpu.data+0x30000;ROW_SLOT=cpu.data+0x37000;NAME=cpu.data+0x39000
load_result=1;design_counter=0;fail_vm=False
def hook(uc,a,size,unused):
 global next_vm,next_alloc,design_counter
 if a==0x8579f8:
  trace.append(dict(service='lua_newstate',allocator=hex(cpu.reg(0)),allocator_symbol=names.get(cpu.reg(0)),userdata=hex(cpu.reg(1)),state=hex(0 if fail_vm else next_vm)))
  ret(0 if fail_vm else next_vm);next_vm+=0x1000
 elif a==0x84b11c:
  trace.append(dict(service='lua_atpanic',state=hex(cpu.reg(0)),panic=hex(cpu.reg(1)),panic_symbol=names.get(cpu.reg(1))));ret()
 elif a==0x85797c:
  trace.append(dict(service='lua_close',state=hex(cpu.reg(0))));ret()
 elif a==0x310570:
  trace.append(dict(service='allocate',size=cpu.reg(0),tag=cpu.reg(1),result=hex(next_alloc)))
  result=next_alloc;next_alloc+=0x1000;uc.mem_write(result,bytes(0x1000));ret(result)
 elif a in (0x310440,0x708f00):
  trace.append(dict(service='free',pointer=hex(cpu.reg(0))));ret()
 elif a==0x31167c:
  trace.append(dict(service='string_reserve',object=hex(cpu.reg(0)),size=cpu.reg(1)));ret(cpu.reg(0))
 elif a in (0x31b010,0x31b000,0x31aff8,0x31b008):
  trace.append(dict(service='open_library',function=hex(a),symbol=names.get(a),instance=hex(cpu.reg(0)),state=hex(w(cpu.reg(0)+4))));ret()
 elif a==0x31a4d4:
  trace.append(dict(service='register_function',arguments=hex(cpu.reg(0)),name=string(cpu,cpu.reg(1)).decode(),function=hex(cpu.reg(2)),symbol=names.get(cpu.reg(2)),userdata=hex(cpu.reg(3))));ret()
 elif a==0x319af4:
  trace.append(dict(service='register_method',arguments=hex(cpu.reg(0)),name=string(cpu,cpu.reg(1)).decode(),function=hex(cpu.reg(2)),symbol=names.get(cpu.reg(2))));ret()
 elif a==0x37b23c:
  trace.append(dict(service='manager_add_file',manager=hex(cpu.reg(0)),script=hex(cpu.reg(1)),state=hex(w(cpu.reg(1)+8)),filename=string(cpu,cpu.reg(2)).decode(),fixture_return=load_result,active_before=hex(w(AI+0x1c))));ret(load_result)
 elif a==0x4c4bdc:
  trace.append(dict(service='design_integer',category=string(cpu,cpu.reg(1)).decode(),key=string(cpu,cpu.reg(2)).decode()));design_counter+=1;ret(1000+design_counter)
 elif a==0x3dbe24:
  trace.append(dict(service='character_timer_start',timers=hex(cpu.reg(0)),duration=cpu.reg(1),repeat=cpu.reg(2),event=cpu.reg(3),user_ref=w(cpu.uc.reg_read(cpu.sp)),active_before=hex(w(AI+0x1c))));ret(10+design_counter)
 elif a==0x3dbe78:
  trace.append(dict(service='actual_empty_selected_on_init',receiver=hex(cpu.reg(0)),active_before=hex(w(AI+0x1c))))
 elif a==0x3a2fec:ret(44)
 elif a in (0x337888,0x337a88):ret()
 elif a==0x3140ec:uc.mem_write(cpu.reg(0),bytes(24));ret(cpu.reg(0))
 elif a==cpu.data+0x19000:
  trace.append(dict(service='character_bind',character=hex(cpu.reg(0)),arguments=hex(cpu.reg(1))));ret()
 elif a==0x3109e0:
  trace.append(dict(service='string_assign',object=hex(cpu.reg(0)),value=bytes(uc.mem_read(cpu.reg(1),cpu.reg(2)-cpu.reg(1))).decode()));ret(cpu.reg(0))
cpu.uc.hook_add(UC_HOOK_CODE,hook)
cases=[]
def invoke(label,fn,args,fields=()):
 trace.clear();cpu.invoke(fn,args)
 row=dict(case=label,function=hex(fn),trace=trace.copy(),fields={hex(x):hex(w(args[0]+x)) for x in fields})
 cases.append(row);return row
for ctor in (0x31b268,0x31b2a8):
 for i in range(2):
  obj=cpu.data+0x10000+i*0x100;cpu.uc.mem_write(obj,bytes([0xa5])*32)
  row=invoke('owned-instance',ctor,[obj],(4,8));assert w(obj+4)==int(row['trace'][0]['state'],16) and cpu.uc.mem_read(obj+8,1)==b'\x01'
  row=invoke('owned-instance-dtor',0x31b180,[obj]);assert [r['service'] for r in row['trace']]==['lua_close']
for ctor in (0x31aa20,0x31a9f0):
 obj=cpu.data+0x12000;cpu.uc.mem_write(obj,bytes([0xa5])*32)
 invoke('borrowed-instance',ctor,[obj,next_vm],(4,8));assert w(obj+4)==next_vm and cpu.uc.mem_read(obj+8,1)==b'\0'
 row=invoke('borrowed-instance-dtor',0x31b1e0,[obj]);assert not row['trace']
for ctor in (0x3d8fb0,0x3d8f44):
 for flag in (0,1,255):
  obj=cpu.data+0x15000;cpu.uc.mem_write(obj,bytes(0x1000))
  row=invoke('char-ai-script-constructor-'+str(flag),ctor,[obj,flag],(8,0x14,0x38,0x44,0x50,0x5c,0x84,0x90,0x98,0xa0,0xac,0xb4))
  regs=[r for r in row['trace'] if r['service']=='register_function']
  assert bool(regs)==(flag==0)
  assert cpu.uc.mem_read(obj+0xc,1)==b'\x01'
  assert sum(r['service']=='lua_newstate' for r in row['trace'])==1
  row=invoke('char-ai-script-dtor',0x3d926c,[obj]);assert sum(r['service']=='lua_close' for r in row['trace'])==1
cpu.uc.mem_write(AI,bytes(0x1000));cpu.pointer(AI+4,CHAR)
row=invoke('iphone-factory',0x3ccfe4,[AI],(0x1c,0x20,0x28,0x2c,0x30))
pending=w(AI+0x20);assert w(AI+0x1c)==0 and w(pending)==cpu.symbols['_ZTV15AISPlayerIPhone']+8
assert [r['service'] for r in row['trace']]==['allocate','lua_newstate','lua_atpanic','string_reserve']
row['pending_fields']={hex(x):hex(w(pending+x)) for x in (0,8,0x14,0x98,0xb4,0xb8,0xbc,0xc0,0xc4,0xc8,0xcc,0xd0,0xd4)}
row=invoke('step1-bind',0x3cc278,[AI]);assert len([r for r in row['trace'] if r['service']=='open_library'])==4
cpu.uc.mem_write(CHAR,bytes(0x1000));cpu.pointer(CHAR,VT);cpu.pointer(VT+0xc,cpu.data+0x19000)
row=invoke('step2-set-character',0x3cc26c,[AI]);assert w(pending+0x98)==CHAR
assert row['trace'][0]['service']=='character_bind' and row['trace'][1]['value']=='data/scripts/ai/'
row=invoke('iphone-deleting-dtor',0x3de294,[pending]);assert [r['service'] for r in row['trace']]==['lua_close','free']
cpu.uc.mem_write(AI,bytes(0x1000));cpu.pointer(AI+4,CHAR)
base=(0x3cf05c+8+w(0x3cf1d4))&0xffffffff
cpu.pointer(base+w(0x3cf1dc),ROW_SLOT);cpu.pointer(ROW_SLOT,ROW)
cpu.uc.mem_write(ROW,bytes(76*68));cpu.uc.mem_write(NAME,b'__player__\0')
cpu.pointer(ROW+44*68+0x28,10);cpu.pointer(ROW+44*68+0x2c,NAME)
row=invoke('authored-row44-actual-selection-and-factory',0x3cf04c,[AI],(0x1c,0x20,0x28,0x2c,0x30))
assert cpu.uc.mem_read(AI+0x2c,1)==b'\x01' and w(AI+0x30)==0
pending=w(AI+0x20);assert w(pending)==cpu.symbols['_ZTV15AISPlayerIPhone']+8
row['pending_state']=hex(w(pending+8))
assert sum(r['service']=='lua_newstate' for r in row['trace'])==1
cpu.pointer(VT+0xc,0x3b56bc)
row=invoke('step2-real-character-and-gameobject-registration',0x3cc26c,[AI]);assert w(pending+0x98)==CHAR
row=invoke('selected-iphone-destroy',0x3de294,[pending]);assert [r['service'] for r in row['trace']]==['lua_close','free']
for load_result in (0,1):
 cpu.uc.mem_write(AI,bytes(0x1000));cpu.pointer(AI+4,CHAR);cpu.pointer(AI,cpu.symbols['_ZTV6CharAI']+8)
 cpu.pointer(AI+0x10,0xffffffff);cpu.pointer(AI+0x14,0xffffffff)
 cpu.pointer(VT+0x34,0x3a2ed4);cpu.uc.mem_write(CHAR+0x1449,b'\0')
 row=invoke('full-player-pending-stages-common-return-'+str(load_result),0x3cf1f0,[AI],(0x1c,0x20,0x28,0x2c,0x30,0x10,0x14))
 pending=w(AI+0x20);assert w(AI+0x1c)==pending and w(AI+0x28)==7 and cpu.uc.mem_read(AI+0x2c,1)==b'\x01'
 load=next(x for x in row['trace'] if x['service']=='manager_add_file');assert load['filename']=='_commons' and load['active_before']=='0x0'
 init=next(x for x in row['trace'] if x['service']=='actual_empty_selected_on_init');assert init['receiver']==hex(pending) and init['active_before']=='0x0'
 assert [x['event'] for x in row['trace'] if x['service']=='character_timer_start']==[0x33,0x34]
 row['pending_state']=hex(w(pending+8))
 invoke('full-player-pending-stages-destroy-'+str(load_result),0x3de294,[pending])
for factory,kind in ((0x3cce14,'AISDefault'),(0x3cd11c,'AISPlayer')):
 cpu.uc.mem_write(AI,bytes(0x1000));cpu.pointer(AI+4,CHAR)
 row=invoke('actual-factory-'+kind,factory,[AI],(0x1c,0x20,0x28,0x2c,0x30))
 pending=w(AI+0x20);assert w(pending)==cpu.symbols['_ZTV'+str(len(kind))+kind]+8
 assert cpu.uc.mem_read(pending+12,1)==b'\x01' and w(pending+8)!=0
 assert not any(x['service']=='register_function' for x in row['trace'])
 invoke('actual-deleting-dtor-'+kind,w(w(pending)+4),[pending])
fail_vm=True
for ctor in (0x31b268,0x31b2a8):
 obj=cpu.data+0x1a000;cpu.uc.mem_write(obj,bytes(32))
 row=invoke('failed-state-constructor',ctor,[obj],(4,8));assert w(obj+4)==0 and cpu.uc.mem_read(obj+8,1)==b'\x01'
 assert [x['service'] for x in row['trace']]==['lua_newstate']
 row=invoke('failed-state-dtor-close-service-null',0x31b180,[obj]);assert row['trace'][0]['state']=='0x0'
fail_vm=False
for ctor in (0x379eb0,0x379e68):
 obj=cpu.data+0x18000;cpu.uc.mem_write(obj,bytes(64))
 row=invoke('manager-constructor',ctor,[obj],(8,0xc,0x10,0x14));assert not row['trace']
 row=invoke('empty-manager-destructor',0x37a0ac,[obj]);assert not row['trace']
selection=json.loads((ROOT/'reference/character-script-selection/authored-inputs.json').read_text())
authored=[]
for item in selection['inputs']:
 p=REPO/item['asset'];assert sha(p)==item['sha256'];authored.append(dict(path=item['asset'],sha256=sha(p)))
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache)==selection['cache_sha256']
entry='com.gameloft.android.GAND.GloftD2SS/files/data/scripts/ai/_commons.luac'
with zipfile.ZipFile(cache) as archive:commons=archive.read(entry)
assert hashlib.sha256(commons).hexdigest()=='20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
markers={hex(pc):string(cpu,(pc+8+w(literal))&0xffffffff).decode() for pc,literal in ((0x37b2c0,0x37b530),(0x37b2d8,0x37b534),(0x37b374,0x37b54c),(0x37b400,0x37b550))}
report=dict(validation='PASS',original_sha256=sha(ENGINE),probe_sha256=sha(Path(__file__)),manifest_sha256={str(p.relative_to(HERE)):sha(p) for p in HERE.rglob('original-functions.json')},original_instructions_executed=True,cases=cases,authored_inputs=authored,authored_player_row=next(x for x in selection['selected_rows'] if x['id']==44),cache_sha256=selection['cache_sha256'],commons=dict(entry=entry,size=len(commons),sha256=hashlib.sha256(commons).hexdigest()),manager_extension_markers=markers,scope=__doc__,service_boundaries=['lua_newstate allocator','lua_atpanic','lua_close','open-library bodies','Arguments.registerFunction','Binder.bindMethod','Manager.AddFile VM/I/O result','design integer','Character timer start','string reserve/assign','object allocation/free'])
(HERE/'ownership-probe.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',cases=len(cases),register_names=[r['name'] for row in cases if row['case']=='step1-bind' for r in row['trace'] if r['service']=='register_function'])))
