"""Original GameObject transform block and VisualObject::Sync versus native.

The transform block starts after the spawn/probability gate and stops before
physical position side effects. Actual visual position/rotation/scaling sync
and quaternion instructions run. Scene setters observe TRS writes; mesh-box
updates are services. Imported arithmetic/libm use host arithmetic/Python libm.
No object activation, complete InitPost, historical Android libm or renderer
behavior is claimed. Position/scale compare exactly; quaternion tolerance 2e-6.
"""
import argparse,hashlib,json,math,os,pathlib,random,struct,subprocess,sys
ap=argparse.ArgumentParser();ap.add_argument('--engine',type=pathlib.Path,required=True)
ap.add_argument('--dependency-root',type=pathlib.Path,required=True)
ap.add_argument('--probe',type=pathlib.Path,required=True);ap.add_argument('--out',type=pathlib.Path,required=True);a=ap.parse_args()
sys.path.insert(0,str(a.dependency_root));sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]/'engine-math/tests'))
from differential import Cpu,f32,scalar,bits
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R4
assert hashlib.sha256(a.engine.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Dependencies:
    def call(self,c,name):
        n=name.removeprefix('__aeabi_')
        if n in ('fadd','fsub','fmul','fdiv','dadd','dsub','dmul','ddiv'):
            k=n[0];x,y=[c.get_float(k,i) for i in range(2)]
            value=x+y if n.endswith('add') else x-y if n.endswith('sub') else x*y if n.endswith('mul') else x/y
            c.put_float(value,k)
        elif n in ('f2d','d2f'):c.put_float(c.get_float(n[0],0),n[-1])
        elif n.startswith(('fcmp','dcmp')):
            x,y=[c.get_float(n[0],i) for i in range(2)]
            values={'eq':x==y,'lt':x<y,'le':x<=y,'gt':x>y,'ge':x>=y,'un':math.isnan(x) or math.isnan(y)}
            c.write_reg(0,int(values[n[4:]]))
        elif n in ('sin','cos','sinf','cosf','sqrtf'):
            k='f' if n.endswith('f') else 'd';x=c.get_float(k,0)
            c.put_float(math.sin(x) if n.startswith('sin') else math.cos(x) if n.startswith('cos') else math.sqrt(x),k)
        elif n in ('memcpy','memset'):
            dst,src,count=[c.reg(i) for i in range(3)]
            c.uc.mem_write(dst,bytes(c.uc.mem_read(src,count)) if n=='memcpy' else bytes([src&255])*count);c.write_reg(0,dst)
        else:raise RuntimeError('Unmodeled arithmetic service: '+name)
        c.uc.reg_write(c.pc_reg,c.uc.reg_read(c.lr_reg))
cpu=Cpu(a.engine,False,Dependencies(),{'functions':[]})
owner=cpu.data+0x1000;visual=cpu.data+0x2000;root=cpu.data+0x3000;vt=cpu.data+0x4000
set_position=cpu.stop+0x100;set_rotation=cpu.stop+0x110;set_scale=cpu.stop+0x120;update=cpu.stop+0x130
def pointer(at,p):cpu.uc.mem_write(at,struct.pack('<I',p))
pointer(root,vt);pointer(visual+4,owner);pointer(visual+8,root)
for slot,address in ((0x90,0x5970bc),(0x94,set_scale),(0x98,0x5970ec),(0x9c,set_rotation),(0xa4,set_position),(0xb8,update)):
    pointer(vt+slot,address)
seen=[]
def hook(uc,address,size,unused):
    if address==0x38bf30:uc.reg_write(cpu.pc_reg,cpu.stop)
    elif address in (set_position,set_rotation,set_scale,update,0x47211c,0x470a54):
        if address in (set_position,set_rotation,set_scale):
            assert cpu.reg(0)==root
            offset,n=(0xac,12) if address==set_position else (0xb8,16) if address==set_rotation else (0xc8,12)
            uc.mem_write(root+offset,bytes(uc.mem_read(cpu.reg(1),n)));seen.append(address)
        uc.reg_write(cpu.pc_reg,uc.reg_read(cpu.lr_reg))
cpu.uc.hook_add(UC_HOOK_CODE,hook)
cases=[([0,0,0],[0,0,0],[1,1,1]),([1,-2,3],[10,20,30],[2,3,4]),
    ([100,200,-300],[90,0,0],[0,-0.,.00001]),([0,0,0],[0,90,0],[.0001,-.0001,-2]),
    ([0,0,0],[0,0,90],[-3,2,1]),([1,2,3],[180,-90,360],[1,1,1])]
threshold=scalar(0x38d1b717)
for s in (scalar(0x38d1b716),threshold,scalar(0x38d1b718),-threshold):cases.append(([1,2,3],[1,2,3],[s,s,s]))
rng=random.Random(20261005)
for _ in range(128):cases.append(([f32(rng.uniform(-20000,20000)) for _ in range(3)],
    [f32(rng.uniform(-720,720)) for _ in range(3)],[f32(rng.uniform(-4,4)) for _ in range(3)]))
def original(position,rotation,scale):
    cpu.uc.mem_write(owner,bytes(0x400));cpu.uc.mem_write(owner+0x160,struct.pack('<3f',*position))
    cpu.uc.mem_write(owner+0x16c,struct.pack('<3f',*rotation));cpu.uc.mem_write(owner+0x120,struct.pack('<3f',*scale))
    for offset,n in ((0xac,12),(0xb8,16),(0xc8,12)):cpu.uc.mem_write(root+offset,b'\xff'*n)
    sp=cpu.stack+0xe000;cpu.uc.reg_write(cpu.sp_reg,sp);cpu.uc.reg_write(cpu.lr_reg,cpu.stop);cpu.uc.reg_write(UC_ARM_REG_R4,owner)
    cpu.uc.emu_start(0x38be84,cpu.stop,count=10000);assert cpu.uc.reg_read(cpu.pc_reg)==cpu.stop
    seen.clear();cpu.invoke('_ZN12VisualObject4SyncEv',[visual]);assert set(seen)=={set_position,set_rotation,set_scale}
    return list(struct.unpack('<3f',cpu.uc.mem_read(root+0xac,12)))+list(struct.unpack('<4f',cpu.uc.mem_read(root+0xb8,16)))+list(struct.unpack('<3f',cpu.uc.mem_read(root+0xc8,12)))
expected=[original(*case) for case in cases]
command=[str(a.probe.resolve())]
if os.name=='nt':
    absolute=a.probe.resolve();command=['wsl','-d','Ubuntu','--','/mnt/'+absolute.drive[0].lower()+absolute.as_posix()[2:]]
# Native transform probe accepts one record per invocation until EOF.
inputs=''.join(' '.join(str(f) for group in case for f in group)+'\n' for case in cases).encode()
result=subprocess.run(command,input=inputs,capture_output=True)
if result.returncode:raise RuntimeError(result.stderr.decode())
actual=[[float(f) for f in line.split()] for line in result.stdout.decode().splitlines()];assert len(actual)==len(expected)
rows=[];maximum=0
for case,want,got in zip(cases,expected,actual):
    got=[f32(x) for x in got];error=max(abs(x-y) for x,y in zip(want[3:7],got[3:7]));maximum=max(maximum,error)
    match=struct.pack('<3f3f',*want[:3],*want[7:])==struct.pack('<3f3f',*got[:3],*got[7:]) and error<=2e-6
    rows.append({'input':case,'original':want,'native':got,'quaternion_max_error':error,'matches':match})
report={'validation':'PASS' if all(r['matches'] for r in rows) else 'FAIL','scope':__doc__,
    'engine_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),'cases':rows,
    'maximum_quaternion_error':maximum,'original_import_calls':cpu.import_calls}
a.out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':report['validation'],'cases':len(rows),'maximum_quaternion_error':maximum,'mismatches':[r for r in rows if not r['matches']]}))
raise SystemExit(0 if report['validation']=='PASS' else 1)
