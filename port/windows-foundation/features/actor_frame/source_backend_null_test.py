import pathlib,subprocess,json,os
root=pathlib.Path.cwd();own=root/'port/windows-foundation/features/actor_frame';out=own/'source_backend_build';bin=root/'.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin';compiler=bin/'clang++.exe'
flags=['-std=c++20','-Dfinite=_finite','-include','exception','-Wall','-Wextra','-Werror','-Wno-unused-value','-ffunction-sections','-fdata-sections','-O2','-flto']
for p in (root/'port').iterdir():
 if p.is_dir():flags+=['-I'+str(p)]
flags+=['-I'+str(root/'port/android-native/app/src/main/cpp'),'-I'+str(root/'port/engine-audio/integration-v42'),'-I'+str(root/'.local-inputs/runtime-test-20261009/windows-include'),'-isystem',str(root/'port/physics-backend/box2d-2.0.1/Include')]
libs=[own/'source_character_owner_factory_native_build/native.a']+[root/'.local-inputs/windows-foundation-build'/n for n in ['libfoundation_data.a','librecovered_content.a','libcontent_xml.a']]
r=subprocess.run([str(compiler),*flags,str(own/'source_campaign_backend_v1.cpp'),str(own/'source_backend_null_test.cpp'),str(out/'source_backend_current_level.o'),*[str(p) for p in libs],str(libs[0]),'-Wl,--gc-sections','-Wl,--error-limit=0','-lopengl32','-luser32','-lgdi32','-o',str(out/'source_backend_null_test.exe')],capture_output=True,text=True)
result={'link_exit':r.returncode,'link_stderr':r.stderr};print('link',r.returncode,r.stderr[:1800])
if not r.returncode:
 env=dict(os.environ);env['PATH']=str(bin)+os.pathsep+env.get('PATH','');r=subprocess.run([str(out/'source_backend_null_test.exe')],capture_output=True,text=True,env=env);result['runtime_exit']=r.returncode;result['runtime_output']=r.stdout+r.stderr;print(result['runtime_output'])
(out/'null-test.json').write_text(json.dumps(result,indent=2)+'\n')
raise SystemExit(result.get('runtime_exit',result['link_exit']))

