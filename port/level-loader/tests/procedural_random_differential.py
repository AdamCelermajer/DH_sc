"""Execute original rnd::RandomGenerator primitives against native C++.

NextInt/GetInt/shuffle-callback/Hash run original ARM32 instructions. Imported
unsigned division is an exact integer quotient/remainder service, not a modeled
RNG result. Does not establish rule selection or complete procedural layouts.
Zero-bound divide-by-zero is outside the original shuffle caller domain.
"""
import argparse,hashlib,json,os,pathlib,random,struct,subprocess,sys
ap=argparse.ArgumentParser()
ap.add_argument('--engine',type=pathlib.Path,required=True)
ap.add_argument('--dependency-root',type=pathlib.Path,required=True)
ap.add_argument('--probe',type=pathlib.Path,required=True)
ap.add_argument('--out',type=pathlib.Path,required=True)
ap.add_argument('--sanitizers',action='store_true')
a=ap.parse_args()
sys.path.insert(0,str(a.dependency_root))
sys.path.insert(0,str(pathlib.Path(__file__).resolve().parents[2]/'engine-math/tests'))
from differential import Cpu
engine_sha=hashlib.sha256(a.engine.read_bytes()).hexdigest()
assert engine_sha=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Division:
    def call(self,c,name):
        if name=='__aeabi_uldivmod':
            numerator=c.reg(0)|(c.reg(1)<<32);denominator=c.reg(2)|(c.reg(3)<<32)
            quotient,remainder=divmod(numerator,denominator)
            for i,bits in enumerate((quotient&0xffffffff,quotient>>32,remainder&0xffffffff,remainder>>32)):
                c.write_reg(i,bits)
        elif name=='__aeabi_uidivmod':
            quotient,remainder=divmod(c.reg(0),c.reg(1));c.write_reg(0,quotient);c.write_reg(1,remainder)
        else:raise RuntimeError('Unmodeled procedural import: '+name)
        c.uc.reg_write(c.pc_reg,c.uc.reg_read(c.lr_reg))
cpu=Cpu(a.engine,False,Division(),{'functions':[]})
state=cpu.data+0x100;string=cpu.data+0x1000
symbol='_ZN3rnd15RandomGenerator'
def seed(value):cpu.uc.mem_write(state,struct.pack('<I',value))
def current():return struct.unpack('<I',cpu.uc.mem_read(state,4))[0]
def signed(bits):return bits if bits<0x80000000 else bits-0x100000000
def call(suffix,*args):
    cpu.invoke(symbol+suffix,[state,*[v&0xffffffff for v in args]])
    return cpu.reg(0)
rng=random.Random(20261005)
cases=[];inputs=[]
seeds=[0,1,0x7fffffff,0x80000000,0xfffffffa,0xfffffffb,0xfffffffc,0xfffffffd,0xfffffffe,0xffffffff]
seeds.extend(rng.getrandbits(32) for _ in range(128))
for value in seeds:
    seed(value);expected=[call('7NextIntEv') for _ in range(128)]
    cases.append({'operation':'next','seed':value,'expected':expected,'final_state':current()})
    inputs.append(f'next {value} 128\n')
intervals=[(0,1),(0,2),(2,4),(2,5),(-10,10),(-2147483648,2147483647),
    (-2147483648,0),(0,2147483647),(1,0),(7,7),(2147483647,-2147483648)]
intervals.extend((rng.randint(-2147483648,2147483647),rng.randint(-2147483648,2147483647)) for _ in range(128))
for i,(lo,hi) in enumerate(intervals):
    value=seeds[i%len(seeds)];seed(value);expected=signed(call('6GetIntEii',lo,hi))
    cases.append({'operation':'between','seed':value,'minimum':lo,'maximum':hi,'expected':[expected,current()]})
    inputs.append(f'between {value} {lo} {hi}\n')
for value in seeds:
    for bound in (1,2,7,64,0x7fffffff,0x80000000,0xffffffff):
        seed(value);expected=call('clEj',bound)
        cases.append({'operation':'bounded','seed':value,'bound':bound,'expected':[expected,current()]})
        inputs.append(f'bounded {value} {bound}\n')
strings=[b'',b'start',b'path_road',b'SWAMP_02',b'\xff\x80\x01',b'a\0ignored',bytes(range(1,256))]
strings.extend(bytes(rng.randrange(256) for _ in range(rng.randrange(128))) for _ in range(128))
for value in strings:
    cpu.uc.mem_write(string,value+b'\0');expected=call('4HashEPh',string)
    cases.append({'operation':'hash','input_hex':value.hex(),'expected':[expected]})
    inputs.append('hash '+(value.hex() or '-')+'\n')
cmd=[str(a.probe.resolve())]
if os.name=='nt':
    absolute=a.probe.resolve();cmd=['wsl','-d','Ubuntu','--','/mnt/'+absolute.drive[0].lower()+absolute.as_posix()[2:]]
if a.sanitizers:
    prefix=['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
    cmd=cmd[:4]+prefix+cmd[4:] if os.name=='nt' else prefix+cmd
run=subprocess.run(cmd,input=''.join(inputs).encode(),capture_output=True)
if run.returncode:raise RuntimeError(run.stderr.decode())
actual=[[int(v) for v in row.split(',')] for row in run.stdout.decode().splitlines()]
assert len(actual)==len(cases)
for row,got in zip(cases,actual):row.update(actual=got,matches=got==row['expected'])
zero=subprocess.run(cmd,input=b'bounded 1 0\n',capture_output=True)
assert zero.returncode==1 and b'Zero procedural shuffle bound' in zero.stderr
root=pathlib.Path(__file__).resolve().parents[1]
source_paths=[root/'procedural_random_v1.cpp',root/'procedural_random_v1.hpp',root/'tests/procedural_random_probe.cpp',pathlib.Path(__file__)]
report={'validation':'PASS' if all(row['matches'] for row in cases) else 'FAIL','scope':__doc__,
    'engine_sha256':engine_sha,'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'sources_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest() for p in source_paths},
    'cases':cases,'original_import_calls':cpu.import_calls,'zero_bound_rejected':True,
    'sanitizers':a.sanitizers,
    'layout_generation_verified':False,'full_loader_verified':False}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':report['validation'],'cases':len(cases),'sequence_values':128*len(seeds),
    'import_calls':cpu.import_calls,'mismatches':[row for row in cases if not row['matches']]}))
raise SystemExit(0 if report['validation']=='PASS' else 1)
