"""Execute original factory/C1/schema and bounded InitPost branches in ARM32.
Inherited GameObject/allocator/string and InitPost domain services are explicit
fixtures. No collision/AI/loot or full retained-engine readiness is claimed.
"""
from pathlib import Path
import sys,json,struct,hashlib,argparse
sys.path.insert(0,'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path('C:/Users/adamc/Desktop/workspace/DH_sc');sys.path.insert(0,str(root/'port/game-data/tests'))
from aggro_differential import Cpu
from unicorn import UC_HOOK_CODE
elf=root/'.local-inputs/libDungeonHunter2.so'
EXPECTED='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
assert hashlib.sha256(elf.read_bytes()).hexdigest()==EXPECTED
c=Cpu(elf,False,{'functions':[]});obj=c.data+0x1000;heapstart=c.data+0x4000;heap=heapstart;mode='ctor';calls=[];properties={};init_fixture={};remove_address=0
def ret(value=None):
 if value is not None:c.put(0,value)
 c.uc.reg_write(c.pc,c.uc.reg_read(c.lr))
def text(p):
 b=bytearray()
 while c.uc.mem_read(p,1)!=b'\0':b+=c.uc.mem_read(p,1);p+=1
 return b.decode()
def word(p):return struct.unpack('<I',c.uc.mem_read(p,4))[0]
def hook(uc,address,size,user):
 global heap
 if address==0x310570:
  size=c.reg(0);assert c.reg(1)==0
  if mode=='ctor':assert size==0x404;out=obj
  else:assert size in (44,56);out=heap;heap+=0x100;c.uc.mem_write(out,b'\xa5'*size)
  calls.append({'allocation':size});ret(out)
 elif address==0x38c398:
  assert c.reg(0)==obj and c.reg(1)==15;calls.append({'base_GO_ID':15});ret()
 elif address==0x31167c:
  target=c.reg(0);assert c.reg(1)==16;c.pointer(target+0x10,target);c.pointer(target+0x14,target);ret()
 elif address==0x3116e8:
  target,begin,end=[c.reg(i) for i in range(3)];assert begin==end
  c.pointer(target+0x10,target);c.pointer(target+0x14,target);c.uc.mem_write(target,b'\0');ret()
 elif address==0x3139ac:ret()
 elif address==0x38cee8:
  assert c.reg(0)==obj;calls.append({'inherited_GameObject_properties':True});ret()
 elif address==0x3140ec:ret()
 elif address==0x513ce4:
  assert c.reg(0)==obj+4;name=text(c.reg(1));p=c.reg(2);offset=word(p+4)+4
  if offset==0x374:default=list(struct.unpack('<fff',c.uc.mem_read(p+0x20,12)))
  else:assert offset==0x3a8;default=''
  properties[name]={'source_offset':offset,'default':default};ret()
 elif mode=='init' and address==0x38bd64:calls.append({'service':'actual_spawn_roll_fixture','roll':init_fixture['roll']});ret(init_fixture['roll'])
 elif mode=='init' and address==0x39de6c:calls.append({'service':'actual_GetDataId_fixture','id':-1});ret(0xffffffff)
 elif mode=='init' and address==0x39771c:calls.append({'service':'actual_Zone_InitPost_fixture'});ret()
 elif mode=='init' and address==0x38ab60:calls.append({'service':'actual_MeetCondition_fixture','result':init_fixture['meets']});ret(init_fixture['meets'])
 elif mode=='init' and address==remove_address:calls.append({'service':'actual_Remove_fixture'});ret()
 elif mode=='init' and address==0x39dbc0:
  calls.append({'service':'actual_GetScript_fixture'});c.uc.mem_write(heapstart,b'trap\0');ret(heapstart)
 elif mode=='init' and address==0x38ef60:
  calls.append({'service':'actual_LoadExternalScript_fixture','name':text(c.reg(1)),'prefix':text(c.reg(2))});ret()
c.uc.hook_add(UC_HOOK_CODE,hook);cases=[]
for poison in (0,0xa5,0x7f):
 calls.clear();heap=heapstart;c.uc.mem_write(obj,bytes([poison])*0x404);c.invoke(0x340e7c,[]);assert c.reg(0)==obj
 fields={'dimensions':list(struct.unpack('<fff',c.uc.mem_read(obj+0x374,12))),'physical380':c.uc.mem_read(obj+0x380,1)[0],'trigger381':c.uc.mem_read(obj+0x381,1)[0],'colzone384':word(obj+0x384),'static84':c.uc.mem_read(obj+0x84,1)[0],'updating85':c.uc.mem_read(obj+0x85,1)[0],'data_id3c0':word(obj+0x3c0),'byte3c4':c.uc.mem_read(obj+0x3c4,1)[0],'word3f0':word(obj+0x3f0),'word3f8':word(obj+0x3f8),'id3fc':word(obj+0x3fc),'byte400':c.uc.mem_read(obj+0x400,1)[0],'byte401':c.uc.mem_read(obj+0x401,1)[0]}
 assert fields=={'dimensions':[0.0]*3,'physical380':1,'trigger381':0,'colzone384':0,'static84':1,'updating85':1,'data_id3c0':0xffffffff,'byte3c4':0,'word3f0':0,'word3f8':0,'id3fc':0xffffffff,'byte400':0,'byte401':0},fields
 assert c.uc.mem_read(obj+0x3a8,1)[0]==0
 trees=[]
 for o in (0x388,0x3c8,0x3e0):
  tree={'offset':o,'color':c.uc.mem_read(obj+o,1)[0],'parent':word(obj+o+4),'left':word(obj+o+8)-obj,'right':word(obj+o+12)-obj,'size':word(obj+o+16)}
  assert tree=={'offset':o,'color':0,'parent':0,'left':o,'right':o,'size':0},tree;trees.append(tree)
 unproduced={hex(o):word(obj+o)==int.from_bytes(bytes([poison])*4,'little') for o in (0x39c,0x3f4)};assert all(unproduced.values()),unproduced
 assert word(obj+0x3a0)==0 and word(obj+0x3a4)==0
 fields['word3a0']=0;fields['word3a4']=0
 cases.append({'poison':poison,'fields':fields,'unproduced_words':unproduced,'empty_data_desc3a8':True,'trees':trees,'calls':list(calls)})
virtuals={hex(i*4):hex(word(word(obj)+i*4)) for i in range(55)};assert virtuals['0x18']=='0x39e8ec';remove_address=int(virtuals['0x40'],16)
mode='properties';calls.clear();properties.clear();heap=heapstart;c.invoke(0x39e8ec,[obj]);assert properties=={'dimensions':{'source_offset':0x374,'default':[200.0]*3},'data':{'source_offset':0x3a8,'default':''}},properties
init_cases=[]
mode='init'
for roll,meets in [(100,1),(10,0),(10,1)]:
 init_fixture={'roll':roll,'meets':meets};calls.clear();c.pointer(obj+0x274,100);c.pointer(obj+0x2d8,0);c.invoke(0x39dee0,[obj]);init_cases.append({'fixture':init_fixture.copy(),'calls':list(calls)})
assert len(init_cases[0]['calls'])==1
assert [x['service'] for x in init_cases[1]['calls']]==['actual_spawn_roll_fixture','actual_GetDataId_fixture','actual_Zone_InitPost_fixture','actual_MeetCondition_fixture','actual_Remove_fixture']
assert [x['service'] for x in init_cases[2]['calls']]==['actual_spawn_roll_fixture','actual_GetDataId_fixture','actual_Zone_InitPost_fixture','actual_MeetCondition_fixture','actual_GetScript_fixture','actual_LoadExternalScript_fixture']
report={'validation':'PASS','original_sha256':EXPECTED,'factory':{'address':'0x340e7c','allocation':0x404,'GO_ID':15,'C1':'0x39ecd4','parent':'ZoneEx397f28 physical=true trigger=false'},'factory_cases':cases,'properties':properties,'virtuals':virtuals,'initpost_branch_cases':init_cases,'scope':'Whole original factory/TriggerTrap C1/ZoneEx/Zone stores and property routines execute. GameObject C1/DeclareProperties and allocator/string services explicit fixture interceptions. InitPost rejection/condition/remove/external-script control flow executes with explicit domain fixtures; visual/timeline/audio, actual RNG/conditions and trap effects are not proved.'}
p=argparse.ArgumentParser();p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':report['validation'],'properties':properties,'factory_poison_cases':len(cases),'initpost_fixture_cases':len(init_cases),'output':str(a.output)}))
