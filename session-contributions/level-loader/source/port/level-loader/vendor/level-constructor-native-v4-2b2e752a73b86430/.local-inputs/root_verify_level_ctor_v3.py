from pathlib import Path
import sys,subprocess,struct,json,hashlib,itertools,os,random
root=Path(__file__).resolve().parent.parent
sys.path.insert(0,r'C:\Users\adamc\AppData\Local\uv\cache\archive-v0\jc8RoeQ1US_9Gkmi\Lib\site-packages')
sys.path.insert(0,str(root/'port/engine-math/tests'))
from differential import Cpu
from unicorn import UC_HOOK_CODE
elf=root/'.local-inputs/libDungeonHunter2.so'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(elf)=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
report=root/'port/level-loader/reports/level-constructor-v3';report.mkdir(parents=True,exist_ok=True)
temporary=report/'compiler-temp';temporary.mkdir(exist_ok=True)
environment=dict(os.environ,TMP=str(temporary),TEMP=str(temporary))
sdk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk')
toolchain=sdk/'ndk/29.0.14206865/toolchains/llvm/prebuilt/windows-x86_64/bin'
sources=[root/'port/level-loader/tests/level_constructor_v3.cpp',root/'port/level-loader/level_constructor_v3.cpp',root/'port/level-world/event_manager_owner_v12.cpp']
builds={}
for abi,compiler in [('x86_64','x86_64-linux-android24-clang++.cmd'),('arm64-v8a','aarch64-linux-android24-clang++.cmd')]:
 out=report/('level-ctor-'+abi)
 run=subprocess.run([str(toolchain/compiler),'-std=c++17','-O2','-Wall','-Wextra','-Werror','-Wno-misleading-indentation',*map(str,sources),'-static-libstdc++','-o',str(out)],capture_output=True,env=environment)
 (report/(abi+'-build.log')).write_bytes(run.stdout+run.stderr)
 assert run.returncode==0,(abi,run.stdout.decode(errors='replace'),run.stderr.decode(errors='replace'))
 builds[abi]=out
adb=[str(sdk/'platform-tools/adb.exe'),'-s','emulator-5554']
remote='/data/local/tmp/dh2-level-constructor-v3'
def device(args):
 r=subprocess.run(adb+args,capture_output=True,timeout=55)
 assert r.returncode==0,(args,r.stdout,r.stderr)
 return r.stdout.decode(errors='replace').replace('\r','').strip()
device(['push',str(builds['x86_64']),remote]);device(['shell','chmod','755',remote])
def text_at(c,at):
 if not at:return ''
 return bytes(c.uc.mem_read(at,1024)).split(b'\0')[0].decode('ascii')
class Imports:
 def call(self,c,name):
  if name=='strcpy':
   value=text_at(c,c.reg(1)).encode()+b'\0';c.uc.mem_write(c.reg(0),value)
  elif name=='strstr':
   position=text_at(c,c.reg(0)).find(text_at(c,c.reg(1)));c.write_reg(0,c.reg(0)+position if position>=0 else 0)
  else:raise AssertionError('Unmodelled import '+name)
  c.uc.reg_write(c.pc_reg,c.uc.reg_read(c.lr_reg))
c=Cpu(elf,False,Imports(),{'functions':[{'elf_address':'0x3f3128','size':876}]})
level=c.data+0x100;name=c.data+0x1000;rows=c.data+0x2000;rowstrings=c.data+0x3000;script_path=c.data+0x4000
online=c.data+0x5000;app=c.data+0x5800;pm=c.data+0x6000;state=c.data+0x7000;save=c.data+0x8000
names=['worlds/swamp01.dwld','worlds/crypt01.dwld','SWAMP','absent','worlds/swamp01_crypt.dwld']
def u32(at):return struct.unpack('<I',c.uc.mem_read(at,4))[0]
def write(at,value):c.uc.mem_write(at,struct.pack('<I',value&0xffffffff))
def string(at,value):c.uc.mem_write(at,value.encode()+b'\0')
case={};events=[];save_args=[]
def hook(uc,at,size,unused):
 global save_args
 r=[c.reg(i) for i in range(4)]
 if at==0x33805c:events.append('event');assert r[0]==level
 elif at==0x37c584:
  events.append('script:'+str(r[1]));assert r[0]==level+0x44
  if case['mutation']&1:write(level+0x30,17);write(level+0x40,91)
 elif at==0x3140ec:assert r[0]==level+0xf8;write(level+0x10c,r[1])
 elif at==0x3109e0:assert r[0]==level+0xac;string(script_path,bytes(c.uc.mem_read(r[1],r[2]-r[1])).decode());write(level+0xc0,script_path)
 elif at==0x37b574:
  assert r[0]==level+0x44 and text_at(c,u32(level+0xc0))=='data/scripts/'
  script=text_at(c,r[1]);events.append('load:'+script)
  if case['mutation']&2 and script=='level/combat_formulas':write(level+0x114,123);write(level+0x30,18);string(name+0x400,names[1]);write(level+0x10c,name+0x400)
 elif at==0x34e414:assert r[1]==0 and r[2]==0xffffffff;string(r[0],text_at(c,r[0]).lower())
 elif at==0x7fd794:
  events.append('online');c.write_reg(0,online)
  if case['mutation']&8:write(level+0x3c,-1)
 elif at==0x36f074:assert r[0]==pm;events.append('hosting');c.write_reg(0,int(case['hosting']==1))
 elif at==0x320e98:events.append('state');c.write_reg(0,state)
 elif at==0x800f8c:events.append('matching_get');c.write_reg(0,state+0x100)
 elif at==0x81f524:assert r[0]==state+0x100;events.append('matching_host');c.write_reg(0,int(case['hosting']==2))
 elif at==0x310570:
  events.append('allocate:'+str(r[0])+':'+str(r[1]));c.write_reg(0,save)
  if case['mutation']&4:
   for offset,value in [(0x114,234),(0x40,8),(0x3c,9),(0x118,5)]:write(level+offset,value)
 elif at==0x462934:
  assert r[0]==save and r[1]==level;events.append('save');stack=struct.unpack('<3I',c.uc.mem_read(c.uc.reg_read(c.sp_reg),12));save_args=[r[2],r[3],*stack]
 else:return
 c.uc.reg_write(c.pc_reg,c.uc.reg_read(c.lr_reg))
c.uc.hook_add(UC_HOOK_CODE,hook)
symbol=next(k for k,v in c.symbols.items() if v==0x3f3128)
cases=[]
for table,n,requested,onlineflag,hosting,pmvalue,onlinestate in itertools.product([0,1,2],range(5),[-1,0,1,3],[0,1],range(3),[0,255],[-1,3,4,5]):
 cases.append([table,n,requested,onlineflag,hosting,pmvalue,onlinestate,128,127,0,0xffffffff])
rng=random.Random(0x3f3128)
for _ in range(300):cases.append([rng.randrange(3),rng.randrange(5),rng.choice([-2147483648,-1,0,1,3]),rng.choice([0,1,127]),rng.randrange(3),rng.choice([0,1,255]),rng.choice([-2147483648,-1,2,3,4,5]),rng.choice([0,1,128]),rng.choice([0,1,127]),rng.randrange(16),rng.randrange(0x100000000)])
fixture=report/'cases.txt';fixture.write_text('\n'.join(' '.join(map(str,x)) for x in cases)+'\n')
device(['push',str(fixture),remote+'.cases'])
native=[json.loads(x) for x in device(['shell',remote,remote+'.cases']).splitlines()]
assert len(native)==len(cases)
word_offsets=[0x30,0x38,0x3c,0x40,0xdc,0xe0,0xe4]
tailword=[0x110,0x114,0x118,0x11c,0x120,0x124,0x128,0x12c,0x130,0x134,0x138,0x13c,0x140]
results=[]
for args,actual in zip(cases,native):
 case=dict(zip(['table','name','requested','online','hosting','pm','state','f1','f2','mutation','debug'],args));events=[];save_args=[]
 c.uc.mem_write(level,b'\xa5'*0x200);string(name,names[case['name']]);write(0x99f72c+0x40,pm);write(state+0x34,case['state'])
 c.uc.mem_write(online+5,bytes([case['online']]));c.uc.mem_write(pm+0x719,bytes([case['pm']]))
 write(0x9a30e8,case['debug']);write(0x9a26d0,123)
 table=[]
 if case['table']:
  if case['table']==2:table.append(('',-1,0x1234))
  for file,diff in [('SWAMP',0),('crypt',3),('swamp',2)]:table.append((file,diff,0x1234+len(table)))
 write(0x9a65e4,len(table));write(0x9a65e8,rows)
 for i,(file,diff,flag) in enumerate(table):
  address=rows+72*i;c.uc.mem_write(address,b'\0'*72);string(rowstrings+128*i,file);write(address+0x20,rowstrings+128*i);write(address+0x10,diff);write(address+0x14,flag)
 returned=c.invoke(symbol,(level,name,0xfffffff9,0xfedcba98),stack=struct.pack('<6I',0x12345678,0x87654321,case['f1'],case['f2'],case['requested']&0xffffffff,6))
 assert returned==level
 byte=lambda o:bytes(c.uc.mem_read(level+o,1))[0]
 fields=[u32(level+o) for o in word_offsets]+[byte(0xe8),int(u32(level+0xec)==save)]+[byte(o) for o in range(0xf0,0xf6)]
 fields += [u32(level+o) for o in tailword]+[byte(0x144),byte(0x145)]+[u32(level+o) for o in [0x148,0x14c,0x150,0x154,0x158,0x15c,0x160,0x164,0x168]]
 fields += [byte(0x16c),u32(level+0x18c),u32(level+0x194),byte(0x198),u32(level+0x19c),u32(level+0x1a0),u32(level+0x1a4),byte(0x1a8),u32(0x9a30e8),u32(0x9a26d0)]
 expected={'fields':fields,'events':events,'save_args':save_args,'name':text_at(c,u32(level+0x10c)),'path':text_at(c,u32(level+0xc0))}
 assert expected==actual,{'case':args,'original':expected,'native':actual}
 results.append({'case':args,'result':actual})
failures=device(['shell',remote,'failures']);assert failures.startswith('PASS failures '),failures
receipt={'status':'PASS','original_complete_Level_C1_cases':len(cases),'mismatches':0,'native_failure_guards':int(failures.rsplit(' ',1)[1]),'instruction_words_executed':len(c.seen),'original_sha256':sha(elf),'sources':{p.relative_to(root).as_posix():sha(p) for p in sources+[root/'port/level-loader/level_constructor_v3.hpp']},'binaries':{a:sha(p) for a,p in builds.items()},'explicit_deeper_fixtures':['LuaScript C1/load','source-compatible immutable LevelList rows','online/player/matching services','Save allocation/C1','ASCII ToLowerCase and C-string imports'],'real_EventManager_native_owner':True,'full_lua_save_file_backend_proven':False,'live_app_unchanged':True,'gameplay_ready':False}
(report/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');(report/'case-results.json').write_text(json.dumps(results)+'\n')
print(json.dumps(receipt))
