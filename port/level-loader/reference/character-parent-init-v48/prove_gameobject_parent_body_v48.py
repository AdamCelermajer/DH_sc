from pathlib import Path
import sys,struct,json,hashlib,importlib.util
ref=Path('C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/reference/character-parent-init-v48')
spec=importlib.util.spec_from_file_location('parent_fields_v48',ref/'prove_character_parent_v48.py');d=importlib.util.module_from_spec(spec);spec.loader.exec_module(d)
from unicorn import UC_HOOK_CODE
c=d.c;actor=d.actor;read=d.read;word=d.word;ret=d.ret;allocate=d.allocate
def byte(a,v):c.uc.mem_write(a,bytes([v]))
def f(a,v):c.uc.mem_write(a,struct.pack('<f',v))
def fs(a,n):return list(struct.unpack('<'+'f'*n,c.uc.mem_read(a,4*n)))
def table(names,got_base,count_offset,data_offset):
 count=read(got_base+read(count_offset));data=read(got_base+read(data_offset));rows=allocate(4*max(1,len(names)));word(count,len(names));word(data,rows)
 for i,n in enumerate(names):buf=allocate(len(n)+1);c.uc.mem_write(buf,n.encode()+b'\0');word(rows+4*i,buf)
device_base=(read(0x381728)+0x3816bc)&0xffffffff
device_flags=[read(device_base+read(a)) for a in [0x38172c,0x381730,0x381734]]
sound_base=(read(0x38c080)+0x38be84)&0xffffffff;difficulty_base=(read(0x33ec94)+0x33ec3c)&0xffffffff
table(['Easy','Normal','Hard'],difficulty_base,0x33ec98,0x33ec9c);table(['none','idle','other'],sound_base,0x38c084,0x38c088)
vtable=allocate(0x100);word(vtable+0x28,0x3a49f0)
class RequiredService(RuntimeError):pass
calls=[];required=None
def reached(uc,a,size,u):
 global required
 if a in [0x33ec0c,0x33eb28,0x38bd64,0x393db4,0x38aac8,0x38174c,0x3816a8,0x394eb0,0x38ba74]:calls.append(hex(a))
 if a==0x33ff54:ret(actor) # explicit actual manager/class domain fixture; no activation claim
 elif a==0x394eb0:required={'entry':hex(a),'reason':'actual main LoadVisual owner not supplied'};raise RequiredService(required['reason'])
 elif a==0x3816f8:required={'entry':hex(a),'reason':'actual application renderer/mask owner not supplied'};raise RequiredService(required['reason'])
c.uc.hook_add(UC_HOOK_CODE,reached)
def setup(roll,probability,optimization=0,skip=0,idle='',difficulty='',device_disabled=True):
 d.type_value=0;c.invoke(0x33f310,[actor,0]);d.string(actor+0x30,'NPC');d.string(actor+0x48,'PlayerCharacter.fakeGametype');d.string(actor+0x90,'Invalid');d.string(actor+0xb4,'');d.string(actor+0xd4,difficulty);d.string(actor+0x358,idle);word(actor,vtable);word(actor+0x270,roll);word(actor+0x274,probability);word(actor+0x2dc,123);word(actor+0x2e0,0);word(actor+0x2d8,0);word(actor+0x370,0x6677fffe);byte(actor+0x60,1);byte(actor+0x10c,optimization);byte(actor+0x15c,skip);byte(actor+0x28,1)
 for p,v in zip(device_flags,[int(device_disabled),0,0]):byte(p,v)
 for i,v in enumerate([0,-0.00005,-3]):f(actor+0x120+4*i,v)
 for i,v in enumerate([90,180,-90]):f(actor+0x16c+4*i,v)
 for i,v in enumerate([10,20,30]):f(actor+0x160+4*i,v)
 for i,v in enumerate([-100,-100,-100,100,100,100]):f(actor+0x144+4*i,v)
 calls.clear()
def invoke(name,**args):
 global required
 setup(**args);required=None;old_sp=c.uc.reg_read(c.sp)
 try:c.invoke(0x38be5c,[actor]);complete=True
 except RequiredService:complete=False;c.uc.reg_write(c.sp,old_sp)
 row={'name':name,'arguments':args,'whole_qualified_body_completed':complete,'required_service':required,'calls':list(calls),'fields':{'physical2dc':read(actor+0x2dc),'visual2d8':read(actor+0x2d8),'checkpoint28':c.uc.mem_read(actor+0x28,1)[0],'difficulty_ec':struct.unpack('<i',c.uc.mem_read(actor+0xec,4))[0],'sound370_word':hex(read(actor+0x370)),'scale':fs(actor+0x120,3),'rotation':fs(actor+0x16c,3),'heading':fs(actor+0x178,1)[0],'position':fs(actor+0x160,3),'relative':fs(actor+0x144,6),'absolute':fs(actor+0x12c,6),'destination':fs(actor+0x1a8,3)}}
 return row
rows=[]
rejected=invoke('genuine cached-rejection no transform/visual branch',roll=42,probability=0);assert rejected['whole_qualified_body_completed'] and rejected['fields']['physical2dc']==123 and '0x38174c' not in rejected['calls'];rows.append(rejected)
for skip,idle,difficulty in [(0,'',''),(1,'idle','Hard'),(1,'absent','absent')]:
 row=invoke('controlled post-C1 optimization1 genuine omitted-visual branch',roll=-2,probability=100,optimization=1,skip=skip,idle=idle,difficulty=difficulty)
 assert row['whole_qualified_body_completed'] and row['fields']['visual2d8']==0 and row['fields']['physical2dc']==0 and row['fields']['scale']==[1,1,-3] and row['fields']['destination']==[10,20,30] and '0x394eb0' not in row['calls'];assert row['fields']['absolute']==[-90,-80,330,110,120,-270]
 assert row['fields']['checkpoint28']==(0 if skip else 1);assert row['fields']['sound370_word']==('0x66770001' if idle=='idle' else '0x6677ffff' if idle else '0x6677fffe');assert row['fields']['difficulty_ec']==(2 if difficulty=='Hard' else -1);rows.append(row)
 row['scope']='Original omission branch verified over explicit post-C1 optimization/device/checkpoint/catalog inputs. Original constructor optimization10c=0, so this is NOT authentic first-SWAMP visual omission or gameplay activation.'
for args,entry in [({'optimization':0},'0x394eb0'),({'optimization':1,'device_disabled':False},'0x3816f8')]:
 row=invoke('required actual visual/renderer producer stop',roll=-2,probability=100,**args);assert not row['whole_qualified_body_completed'] and row['required_service']['entry']==entry and row['fields']['physical2dc']==0;rows.append(row)
out={'schema':'dh2-gameobject-parent-body-original-v48','validation':'PASS original whole/required-stop branch control','original_elf_sha256':'36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80','entry':'0x38be5c','cases':rows,'executed_bodies':['ObjectBase.InitPost33ec0c','ConditionData.Init33eb28 empty/Invalid','CheckSpawnProbability38bd64 cached path','IsPlayer3a49f0 live NAME path','SetPosition393db4','UpdateAbsoluteAABB38aac8','Device.HighPerformance38174c/IsProgrammablePipeline3816a8'], 'explicit_domains':'Whole source ObjectBase C2 allocation leaves; manager/class handle conversion and type0 domain result; exact imported libc/float arithmetic leaves. Catalogs and live renderer-disable bytes are explicit fixture inputs. Required actual renderer mask/LoadVisual never reported success.', 'original_ctor_optimization10c':0,'first_swamp_nonvisual_or_complete_activation_proven':False,'scope':'Full qualified38be5c body runs to return on cached-rejection and controlled genuinely omitted NULLvisual branches. Normal constructor optimization0 stops at actual visual service; no fake LoadVisual/Sync callback or forced InitFinal.'}
(ref/'gameobject-parent-body-original-v48.json').write_text(json.dumps(out,indent=2)+'\n');(ref/'prove_gameobject_parent_body_v48.py').write_bytes(Path(__file__).read_bytes());print('PASS original38be5c: cachedreject1, controlledNULLvisual3, requiredproducer2; NOT firstSWAMPactivation')
