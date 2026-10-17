"""Selected-library source option queries versus the original ARM callers."""
import argparse,hashlib,itertools,json,os,pathlib,re,shutil,struct,subprocess,sys,tempfile
ROOT=pathlib.Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/game-data'
ELF_SHA='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80'
SYMBOLS=['_ZNK15SavegameManager9hasOptionEPKc','_ZNK15SavegameManager9getOptionEPKc',
 '_ZNK15SavegameManager15isOptionToggledEPKc','_ZN11Application14GetSavedOptionEPKc',
 '_ZN11Application15IsSavedOptionOnEPKc','_ZN15SavegameManagerC1Ev']
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(argv,env=None):
 p=subprocess.run(list(map(str,argv)),cwd=ROOT,env=env,text=True,capture_output=True)
 if p.returncode:raise RuntimeError(f'{argv}\n{p.stdout}\n{p.stderr}')
 return p.stdout
def main():
 a=argparse.ArgumentParser();a.add_argument('--compiler',type=pathlib.Path,required=True);a.add_argument('--original-elf',type=pathlib.Path,required=True)
 a.add_argument('--output',type=pathlib.Path,default=pathlib.Path(tempfile.gettempdir())/'dh2-opt-v1')
 a.add_argument('--report',type=pathlib.Path,default=MODULE/'build/savegame-options-v1/validation.json');args=a.parse_args()
 original=args.original_elf.resolve();assert sha(original)==ELF_SHA
 from elftools.elf.elffile import ELFFile
 raw=original.read_bytes()
 with original.open('rb') as f:
  elf=ELFFile(f);symbols={s.name:s for s in elf.get_section_by_name('.symtab').iter_symbols()}
  loads=[s for s in elf.iter_segments() if s['p_type']=='PT_LOAD'];rows=[]
  for name in SYMBOLS:
   s=symbols[name];at,n=int(s['st_value']),int(s['st_size']);seg=next(s for s in loads if s['p_vaddr']<=at and at+n<=s['p_vaddr']+s['p_filesz'])
   off=int(seg['p_offset'])+at-int(seg['p_vaddr']);rows.append({'symbol':name,'address':hex(at),'bytes':n,'sha256':hashlib.sha256(raw[off:off+n]).hexdigest(),'scope':'caller' if name!=SYMBOLS[-1] else 'empty-map constructor field evidence only'})
 out=args.output.resolve();wrapper=out/'wrapper';build=out/'selected';wrapper.mkdir(parents=True,exist_ok=True)
 cc=args.compiler.with_name(args.compiler.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja');assert cmake and ninja
 cm=f'''cmake_minimum_required(VERSION 3.22)
project(options_selected LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_subdirectory("{MODULE.as_posix()}" selected-game-data)
'''
 (wrapper/'CMakeLists.txt').write_text(cm,encoding='utf-8')
 logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={pathlib.Path(ninja).as_posix()}',f'-DCMAKE_CXX_COMPILER={args.compiler.as_posix()}',f'-DCMAKE_C_COMPILER={cc.as_posix()}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1'])]
 commands=run([ninja,'-C',build,'-t','commands','savegame_options_v1_audit'])
 db=json.loads((build/'compile_commands.json').read_bytes());paths={pathlib.Path(r['file']).resolve() for r in db if r['file'].replace('\\','/') in commands.replace('\\','/')}
 assert MODULE/'savegame_options_v1.cpp' in paths
 paths.update([pathlib.Path(__file__).resolve(),MODULE/'CMakeLists.txt',MODULE/'savegame_options_v1.hpp'])
 # Only recorded dependencies participate; no unrelated draft-header snapshots.
 before={p.relative_to(ROOT).as_posix():sha(p) for p in paths if p.is_relative_to(ROOT)}
 logs.append(run([cmake,'--build',build,'--target','savegame_options_v1_audit','--parallel','2']))
 exe=build/'selected-game-data/savegame_options_v1_audit.exe';dsos=sorted(build.rglob('*.dll'))
 env=os.environ.copy();env['PATH']=os.pathsep.join([str(args.compiler.parent),*(str(p.parent) for p in dsos),env.get('PATH','')])
 imports=re.findall(r'DLL Name: (\S+)',run([args.compiler.with_name('objdump.exe'),'-p',exe]));assert 'libdh2_game_data.dll' in imports
 host=json.loads(run([exe],env));assert host['validation']=='PASS'
 sys.path.insert(0,str(ROOT/'port/engine-resources/tests'));from cpu import Cpu
 from unicorn import UC_HOOK_CODE
 cpu=Cpu(original,False,{'functions':[]});app=cpu.data+0x1000;manager=cpu.data+0x2000;node=cpu.data+0x3000;definition=cpu.data+0x4000;key=cpu.data+0x5000
 cpu.uc.mem_write(key,b'GOD_MANA\0');cpu.pointer(app+0x4c,manager);cpu.pointer(node+0x28,definition)
 covered=set();calls=[];present=False
 def observe(_,at,__,___):
  if any(int(r['address'],0)<=at<int(r['address'],0)+r['bytes'] for r in rows[:-1]):covered.add(at)
  if at==0x46ce64:
   assert cpu.reg(0)==manager+0x10
   actual_key=struct.unpack('<I',cpu.uc.mem_read(cpu.reg(1),4))[0]
   assert bytes(cpu.uc.mem_read(actual_key,9))==b'GOD_MANA\0'
   calls.append(at);cpu.put(0,node if present else manager+0x10);cpu.uc.reg_write(cpu.pc,cpu.uc.reg_read(cpu.lr))
 hook=cpu.uc.hook_add(UC_HOOK_CODE,observe);comparisons=[]
 def signed(v):return v if v<0x80000000 else v-0x100000000
 for present,on,kind,value in itertools.product([0,1],[-7,0,1,7],[-1,0,1,7],[-7,0,1,7]):
  cpu.pointer(definition+0xc,on&0xffffffff);cpu.pointer(definition+0x18,kind&0xffffffff);cpu.pointer(node+0x2c,value&0xffffffff)
  expected=[];traces=[]
  for i,r in enumerate(rows[:-1]):
   calls.clear();v=cpu.invoke(int(r['address'],0),[manager if i<3 else app,key]);expected.append(signed(v));traces.append(len(calls))
  actual=json.loads(run([exe,present,on,kind,value],env));assert actual==expected,(present,on,kind,value,expected,actual)
  assert traces==([1,1,1,2,2] if present else [1,1,1,1,1])
  comparisons.append({'input':[present,on,kind,value],'original':expected,'compiled':actual,'map_lookups':traces})
 cpu.uc.hook_del(hook)
 required={at for r in rows[:-1] for at in range(int(r['address'],0),int(r['address'],0)+r['bytes'],4)};assert required<=covered
 assert all(sha(ROOT/p)==h for p,h in before.items())
 ref=MODULE/'reference/savegame-options-v1/original-functions.json';ref.parent.mkdir(parents=True,exist_ok=True)
 manifest={'original_sha256':ELF_SHA,'functions':rows,'lookup_boundary':'Original _M_find is explicitly modeled as the genuine owner map lookup. Native uses its one maintained std::map. No whole original allocator/tree or constructor body credit.'}
 ref.write_text(json.dumps(manifest,indent=2)+'\n',encoding='utf-8')
 report={'validation':'PASS','host':host,'comparisons':len(comparisons),'original_caller_executions':len(comparisons)*5,'mismatches':0,'instructions':len(required),'source_sha256':before,'original_sha256':ELF_SHA,'original_functions':rows,'original_results':comparisons,'selected_commands':commands,'executable_imports':imports,'wrapper_cmake':cm,'build_stdout':logs,'scope':'Selected library five option query callers; original tree lookup modeled. Fresh map defaults and source toggled predicate. Full option registration/settings-file/profile lifecycle and networking are not claimed.'}
 args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
 print(json.dumps({'validation':'PASS','host':host,'comparisons':len(comparisons),'original_caller_executions':len(comparisons)*5,'instructions':len(required),'mismatches':0}))
if __name__=='__main__':main()
