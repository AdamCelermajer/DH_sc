"""Run original null-geometry, spin2 initializer and >30 billboard instructions.
No legacy execution layer is linked into production; Unicorn is a proof tool.
"""
from pathlib import Path
import sys,json,hashlib,random,struct
sys.path.insert(0,r'C:/Users/adamc/AppData/Local/uv/cache/archive-v0/jc8RoeQ1US_9Gkmi/Lib/site-packages')
root=Path(__file__).resolve().parents[3]
original=root/'port/engine-animation/tests/particle_cloud_models_v1_differential.py'
env={'__file__':str(original),'__name__':'v32_proof'}
exec(compile(original.read_text().split('rng=random.Random')[0],str(original),'exec'),env)
old=env['old'];ModelCpu=env['ModelCpu'];words=env['words'];identity=env['identity'];u32=env['u32']
new=ModelCpu(root/'.local-inputs/authored-fx-v32/source-oracle-arm64.so',True,{'functions':[]})
model,context,vt,seed,particle,matrix,stub=[env[n]for n in ['model','context','vt','seed','particle','matrix','stub']]
particle=old.data+0x60000;ni=new.data+0x10000;no=ni+0x10000
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_SP
from compiled_transforms_differential import relocate
report={'original_sha256':hashlib.sha256((root/'.local-inputs/libDungeonHunter2.so').read_bytes()).hexdigest(),'original_instructions_executed':True,'arm64_instructions_executed':True}
# ConstructGeometry executes the real null branch without any imported calls.
cache=root/'port/level-world/reference/shared-target-facing-v1/cache/general-v5'
geometry=[];base=old.data+0x40000
for index in range(5,10):
 raw=(cache/f'asset_{index:02}.bdae').read_bytes();r=relocate(old,raw,base)
 g=u32(r+108);result=old.data+0x20000
 assert u32(g+8)==1
 old.pointer(result,0xdeadbeef)
 old.invoke(0x60e634,[result,0,0,g],budget=1000)
 assert u32(result)==0
 geometry.append({'asset':f'asset_{index:02}.bdae','geometry_kind':1,'original_mesh_pointer':0})
report['nonrender_geometry']=geometry
# Original initializer reaches source setParameter calls. Explicit registry
# service captures names/values; rendering stage is outside this focused proof.
raw=(cache/'asset_03.bdae').read_bytes();r=relocate(old,raw,base);record=u32(r+124)+144
receiver=old.data+0x21000;gen=receiver+0x1000;gvt=gen+0x100;calls=[];active=False
old.pointer(receiver+0x18c,record);old.pointer(gen,gvt);old.pointer(gvt-12,0)
def ret(value=0):old.put(0,value);old.uc.reg_write(old.pc,old.uc.reg_read(old.lr))
def callback(uc,a,size,user):
 if not active:return
 if a==0x63b3d0:ret(0)
 elif a==0x655314:ret(gen)
 elif a in (0x650400,0x6501d4,0x650278,0x650340):
  name=old.string(old.reg(1)).decode();value=old.reg(2)
  if a==0x650278:value=[value,old.reg(3),u32(uc.reg_read(UC_ARM_REG_SP))]
  elif a==0x650340:value=[u32(value+4*k)for k in range(3)]
  calls.append((name,value));ret()
 elif a==0x656640:old.put(3,0);uc.reg_write(old.pc,0x6566c4) # registry enum write belongs to existing prefix proof
 elif a==0x6568b8:uc.reg_write(old.pc,0x656c88) # source model init complete, renderer outside this proof
old.uc.hook_add(UC_HOOK_CODE,callback)
active=True;old.invoke(0x656450,[receiver,0,1],budget=100000);active=False
assert ('SpinAxisType',2)in calls and ('SpinAxis',[0,0,0])in calls and ('SpinAxisVariation',0)in calls
report['darkqueen_original_parameter_calls']=calls
initgold=root/'port/level-world/reference/authored-fx-v32-init-gold.bin'
initgold.write_bytes(words([len(calls)])+b''.join(words([len(name)+1,len(value) if isinstance(value,list) else 1])+name.encode()+b'\0'+words(value if isinstance(value,list) else [value])for name,value in calls))
# Same-vector sort/BBox and spin2 outputs must be byte-identical to ARM64.
rng=random.Random(0x3206);records=[]
for trial in range(240):
 n=[0,1,16,17,30,31,45,64,127,256][trial%10];local=trial%2
 camera=struct.pack('<3f',*[rng.uniform(-100,100)for _ in range(3)]);p=bytearray(n*100)
 for i in range(n):
  struct.pack_into('<25f',p,i*100,*[rng.uniform(-10,10)for _ in range(25)])
  if trial%5==0:struct.pack_into('<3f',p,i*100,1,2,3)
 old.uc.mem_write(model+0x60,camera);old.uc.mem_write(particle,bytes(p));old.uc.mem_write(matrix,identity+bytes([0]));old.uc.mem_write(context+0x54,bytes([local]))
 old.invoke(0x651eec,[model,particle,particle+n*100],budget=10000000)
 expected=bytes(old.uc.mem_read(model+0x74,24))+bytes(old.uc.mem_read(particle,n*100))
 payload=words([n])+camera+identity+words([local])+bytes(p);new.uc.mem_write(ni,payload)
 length=new.invoke('dh2_fx_source_v32',[0,ni,no],budget=10000000);actual=bytes(new.uc.mem_read(no,length))
 assert actual==expected,('sort',trial,n,next((i for i,(a,b)in enumerate(zip(actual,expected))if a!=b),None))
 records.append(words([0,len(payload)])+payload+words([len(expected)])+expected)
for trial in range(160):
 n=trial%9;initial_seed=rng.randrange(1,2147483647)
 config=struct.pack('<4fI4f',rng.choice([0,1,2]),rng.choice([0,0.15,1]),rng.choice([0,1.5]),rng.choice([0,1]),2,0,0,0,0)
 p=bytearray(n*100)
 for i in range(n):struct.pack_into('<25f',p,i*100,*[rng.uniform(-10,10)for _ in range(25)])
 old.uc.mem_write(particle,bytes(p));old.pointer(seed,initial_seed);old.uc.mem_write(model+4,config[:16]);old.uc.mem_write(model+0x14,config[20:32]);old.uc.mem_write(model+0x20,config[32:36]);old.uc.mem_write(model+0x24,config[16:20])
 old.invoke(0x64e3c8,[model,particle,particle+n*100],budget=5000000)
 expected=words([u32(seed)])+bytes(old.uc.mem_read(particle,n*100));payload=words([n,initial_seed])+config+bytes(p);new.uc.mem_write(ni,payload)
 length=new.invoke('dh2_fx_source_v32',[1,ni,no],budget=5000000);actual=bytes(new.uc.mem_read(no,length))
 assert actual==expected,('spin2',trial,n)
 records.append(words([1,len(payload)])+payload+words([len(expected)])+expected)
gold=root/'port/level-world/reference/authored-fx-v32-source-gold.bin';gold.write_bytes(words([0x32335846,len(records)])+b''.join(records))
report.update(validation='PASS',sort_bbox_comparisons=240,sort_count_domain=[0,1,16,17,30,31,45,64,127,256],spin2_comparisons=160,gold_sha256=hashlib.sha256(gold.read_bytes()).hexdigest(),arm64_sha256=hashlib.sha256((root/'.local-inputs/authored-fx-v32/source-oracle-arm64.so').read_bytes()).hexdigest(),scope='CPU source proofs with explicit camera/registry/libm services. No GPU or live gameplay acceptance.')
(root/'port/level-world/reports/authored-fx-v32-original-proof.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))



