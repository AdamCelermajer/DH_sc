"""Execute original UserProperties parsing through its AddProperty boundary.

Actual line/key/value parsing executes original instructions. The owning map
assignment is an observed service; last writes win. Full floor loading does
not execute. Imported strchr/strstr are byte-string services, not parse results.
"""
import argparse,hashlib,json,os,pathlib,random,re,struct,subprocess,sys,zipfile
ap=argparse.ArgumentParser();ap.add_argument('--engine',type=pathlib.Path,required=True)
ap.add_argument('--dependency-root',type=pathlib.Path,required=True)
ap.add_argument('--cache',type=pathlib.Path,required=True);ap.add_argument('--probe',type=pathlib.Path,required=True)
ap.add_argument('--out',type=pathlib.Path,required=True);a=ap.parse_args()
sys.path.insert(0,str(a.dependency_root));sys.path.insert(0,str(pathlib.Path(__file__).parent))
from xml_original_probe import Parser
from unicorn import UC_HOOK_CODE
assert hashlib.sha256(a.engine.read_bytes()).hexdigest()=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
class Properties(Parser):
    def __init__(self):
        super().__init__(a.engine,{'functions':[]});self.values={};self.calls=[]
        self.uc.hook_add(UC_HOOK_CODE,self.allocation);self.uc.hook_add(UC_HOOK_CODE,self.assign)
    def external(self,uc,addr,size,unused):
        name=self.imports.get(addr)
        if name in ('strstr','strchr'):
            at=self.reg(0);raw=self.text(at).encode();needle=self.text(self.reg(1)).encode() if name=='strstr' else bytes([self.reg(1)&255])
            index=raw.find(needle);self.returned(at+index if index>=0 else 0)
        else:super().external(uc,addr,size,unused)
    def assign(self,uc,addr,size,unused):
        if addr==0x318e18:
            key,value=self.text(self.reg(1)),self.text(self.reg(2));self.calls.append([key,value]);self.values[key]=value;self.returned()
    def run(self,raw):
        self.values={};self.calls=[]
        at=self.data+0x18000;obj=self.data+0x19000;self.uc.mem_write(at,raw.encode()+b'\0')
        self.uc.mem_write(obj,bytes(32));self.invoke(0x319158,[obj,at],budget=10000000)
        return dict(self.values)
cases=['floortypes = %22hole%22','floortypes = %22water%22','floortypes = %22wood%22','floortypes = %22door%22\r\n',
    '', 'key', 'key=', '=value','!!!', '_floor_types=water', 'floortypes = "water"',
    'floortypes = %22void water%22 trailing', 'x=a=b', 'floortypes=%22open',
    'floortypes=hole\nfloortypes=water', 'Floortypes=hole', '123= value\r\n', 'a\nb\nc',
    'a=%22%22','a=%22x%22%22y%22','a\rb=hello']
rng=random.Random(20261005)
for _ in range(64):
    key=rng.choice(['floor','floor_types','!Floor!','1x',' key ','']);value=rng.choice(['water','%22hole%22',' %22wood%22\r','void=wall','%22open',''])
    cases.append(key+'='+value)
with a.cache.open('rb') as f:cache_sha=hashlib.file_digest(f,'sha256').hexdigest()
assert cache_sha=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
with zipfile.ZipFile(a.cache) as z:
    bdae=z.read('com.gameloft.android.GAND.GloftD2SS/files/data/3d/modules/swamp/swamp.bdae')
source_properties=[m.group().decode('ascii') for m in re.finditer(rb'[\x20-\x7e\r\n\t]+',bdae) if b'floortypes' in m.group()]
assert len(source_properties)==4
cases.extend(source_properties)
cmd=[str(a.probe.resolve()),'--batch']
if os.name=='nt':
    absolute=a.probe.resolve();linux='/mnt/'+absolute.drive[0].lower()+absolute.as_posix()[2:]
    cmd=['wsl','-d','Ubuntu','--',linux,'--batch']
result=subprocess.run(cmd,input=''.join(str(len(c))+'\n'+c for c in cases).encode(),capture_output=True)
if result.returncode:raise RuntimeError((result.returncode,result.stderr.decode(),result.stdout.decode()))
native=[json.loads(line) for line in result.stdout.decode().splitlines()];assert len(native)==len(cases)
old=Properties();rows=[]
for i,raw in enumerate(cases):
    if i and i%16==0:old=Properties()
    expected=old.run(raw)
    actual=native[i];rows.append({'input':raw,'expected':expected,'actual':actual,'matches':expected==actual})
report={'validation':'PASS' if all(r['matches'] for r in rows) else 'FAIL','scope':__doc__,
    'engine_sha256':hashlib.sha256(a.engine.read_bytes()).hexdigest(),
    'probe_sha256':hashlib.sha256(a.probe.read_bytes()).hexdigest(),
    'cache_sha256':cache_sha,'swamp_bdae_sha256':hashlib.sha256(bdae).hexdigest(),'authored_property_strings':source_properties,
    'script_sha256':hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),'cases':rows}
a.out.write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':report['validation'],'comparisons':len(rows),'mismatches':[r for r in rows if not r['matches']]},indent=2))
raise SystemExit(0 if report['validation']=='PASS' else 1)
