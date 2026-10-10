import pathlib,subprocess,json
root=pathlib.Path.cwd();own=root/'port/windows-foundation/features/actor_frame';out=own/'source_backend_build';out.mkdir(exist_ok=True)
compiler=root/'.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
flags=['-std=c++20','-Dfinite=_finite','-include','exception','-Wall','-Wextra','-Werror','-Wno-unused-value','-ffunction-sections','-fdata-sections']
for p in (root/'port').iterdir():
 if p.is_dir():flags+=['-I'+str(p)]
flags+=['-I'+str(root/'port/android-native/app/src/main/cpp'),'-I'+str(root/'port/engine-audio/integration-v42'),'-I'+str(root/'.local-inputs/runtime-test-20261009/windows-include'),'-isystem',str(root/'port/physics-backend/box2d-2.0.1/Include')]
results={}
for source in [own/'source_campaign_backend_v1.cpp',root/'port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp']:
 r=subprocess.run([str(compiler),*flags,'-c',str(source),'-o',str(out/(source.stem+'.o'))],capture_output=True,text=True)
 results[source.name]={'exit':r.returncode,'stderr':r.stderr};print(source.name,r.returncode,r.stderr)
(out/'compile.json').write_text(json.dumps(results,indent=2))
source=own/'source_backend_link_test.cpp'
r=subprocess.run([str(compiler),*flags,'-c',str(source),'-o',str(out/'source_backend_link_test.o')],capture_output=True,text=True)
results[source.name]={'exit':r.returncode,'stderr':r.stderr}
if r.returncode:print(r.stderr)
else:
 current=root/'port/windows-foundation/features/actor_frame/source_current_level_backend_v1.cpp'
 r=subprocess.run([str(compiler),*flags,'-c',str(current),'-o',str(out/'source_backend_current_level.o')],capture_output=True,text=True)
 if r.returncode:print(r.stderr)
 else:
  libs=[own/'source_character_owner_factory_native_build/native.a']+[root/'.local-inputs/windows-foundation-build'/n for n in ['libfoundation_data.a','librecovered_content.a','libcontent_xml.a']]
  r=subprocess.run([str(compiler),str(out/'source_backend_link_test.o'),str(out/'source_campaign_character_fsm_v101.o'),str(out/'source_campaign_backend_v1.o'),str(out/'source_backend_current_level.o'),*[str(p) for p in libs],str(libs[0]),'-Wl,--gc-sections','-Wl,--error-limit=0','-lopengl32','-luser32','-lgdi32','-o',str(out/'source_backend_link_test.exe')],capture_output=True,text=True)
  results['native_link']={'exit':r.returncode,'stderr':r.stderr};(out/'link-errors.txt').write_text(r.stderr);print('native_link',r.returncode,r.stderr[:2500])
(out/'compile.json').write_text(json.dumps(results,indent=2))

# A contract object authenticates typed provider ownership; it is not a runtime proof.
contract=own/'source_backend_contract_test.cpp'
r=subprocess.run([str(compiler),*flags,'-c',str(contract),'-o',str(out/'source_backend_contract_test.o')],capture_output=True,text=True)
print('contract',r.returncode,r.stderr)

results['contract']={'exit':r.returncode,'stderr':r.stderr}
(out/'compile.json').write_text(json.dumps(results,indent=2))
raise SystemExit(0 if all(v['exit']==0 for v in results.values()) else 1)

