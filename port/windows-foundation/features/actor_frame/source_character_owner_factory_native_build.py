"""Build/run genuine V60 constructor-prefix test; never substitutes native owners.
Run from repo root with a real Python runtime. Compiler and original cache default
are existing workspace inputs. Outputs remain within the factory-owned prefix.
"""
import pathlib,subprocess,json,concurrent.futures,os,sys
root=pathlib.Path.cwd();own=root/'port/windows-foundation/features/actor_frame';out=own/'source_character_owner_factory_native_build';out.mkdir(exist_ok=True)
bin=root/'.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin';compiler=bin/'clang++.exe'
flags=['-std=c++20','-Dfinite=_finite','-DDH2_NATIVE_PROFILE_TRANSPORT_EMBED','-O0','-ffunction-sections','-fdata-sections','-fno-fast-math','-ffp-contract=off','-w','-include','exception']
for d in ['level-world','game-data','engine-audio','engine-textures','engine-ui','engine-animation','engine-skinning','level-loader','level-loader/vendor/tinyxml','scene-materials','script-runtime','script-runtime/lua','engine-resources','engine-math','engine-ui/vendor/gameswf1714']:flags+=['-I'+str(root/'port'/d)]
flags+=['-isystem',str(root/'port/physics-backend/box2d-2.0.1/Include')]
flags+=['-I'+str(root/'port/android-native/app/src/main/cpp'),'-I'+str(root/'port/engine-audio/integration-v42'),'-I'+str(root/'.local-inputs/runtime-test-20261009/windows-include')]
sources=[root/x for x in json.loads((own/'source_character_owner_factory_native_closure.json').read_text())['sources']]
def compile_one(f):
 obj=out/(str(f.relative_to(root)).replace('/','_').replace('\\','_')+'.o')
 unitflags=flags if f.suffix!='.c' else ['-x','c','-std=c11']+[x for x in flags if x not in ['-std=c++20','-include','exception']]
 # Compile every unit: transitive header edits must not reuse stale objects.
 r=subprocess.run([str(compiler),*unitflags,'-c',str(f),'-o',str(obj)],capture_output=True,text=True)
 return f,obj,r
compiled={};failures={}
with concurrent.futures.ThreadPoolExecutor(max_workers=6) as pool:
 for f,obj,r in pool.map(compile_one,sources):
  if r.returncode:failures[f.relative_to(root).as_posix()]=r.stderr
  else:compiled[f]=obj
(out/'compile-errors.json').write_text(json.dumps(failures,indent=2))
if failures:print('Native compile failed:',', '.join(failures));sys.exit(1)
test=own/'source_character_owner_factory_native_test.cpp';archive=out/'native.a';rsp=out/'link.rsp';exe=out/'source_character_owner_factory_native_test.exe'
# Recreate the archive to exclude obsolete source units from prior exploration.
if archive.exists():archive.unlink()
rsp.write_text('\n'.join('"'+x.as_posix()+'"' for f,x in compiled.items() if f!=test))
subprocess.run([str(bin/'llvm-ar.exe'),'rcs',str(archive),'@'+str(rsp)],check=True,capture_output=True)
foundation=root/'.local-inputs/windows-foundation-build'
shared=[foundation/'libfoundation_data.a',foundation/'librecovered_content.a',foundation/'libcontent_xml.a']
if not all(p.is_file() for p in shared):raise RuntimeError('Build current Windows foundation libraries before native visual integration')
r=subprocess.run([str(compiler),str(compiled[test]),str(archive),*[str(p) for p in shared],str(archive),'-Wl,--gc-sections','-Wl,-Map,'+str(out/'native.map'),'-lopengl32','-luser32','-lgdi32','-lwinmm','-o',str(exe)],capture_output=True,text=True)
(out/'link-errors.txt').write_text(r.stderr)
if r.returncode:print(r.stderr);sys.exit(r.returncode)
cache=sys.argv[1] if len(sys.argv)>1 else str(root/'.local-inputs/publication/checkpoint/port/android-native/app/src/main/assets/data')
condition_cache=sys.argv[2] if len(sys.argv)>2 else str(root/'.local-inputs/windows-shared-assets/original-cache')
env=dict(os.environ);env['PATH']=str(bin)+os.pathsep+env.get('PATH','')
r=subprocess.run([str(exe),cache,condition_cache],capture_output=True,text=True,env=env)
(out/'runtime.txt').write_text(r.stdout+r.stderr+'\nnative_exit='+str(r.returncode)+'\n');print(r.stdout+r.stderr,end='');sys.exit(r.returncode)
