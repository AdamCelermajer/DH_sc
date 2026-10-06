"""Standalone actual GLES regression; default build only. --run touches only
/data/local/tmp test files on root-authorized5554, never app/APK lifecycle."""
from pathlib import Path
import subprocess,sys,hashlib,json,os
root=Path(__file__).resolve().parents[3]
ndk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe')
adb=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe')
base=root/'port/engine-textures/tools/run_effect_texture_v5_gles_test.py'
namespace={'__file__':str(base)}
exec(base.read_text().split("out=root/")[0],namespace)
sources=namespace['sources'][1:]
sources.insert(0,'port/scene-materials/tests/effect_material_gles_v4.cpp')
sources += ['port/scene-materials/'+name+'.cpp' for name in ['blood_render_pass_v3','effect_render_pass_v4','shader_sources','shader_reflection_v4','shader_program_collection_v4','material_compare_v4','effect_material_directory_v4','transparent_queue_v1','transparent_sort_v2']]
out=root/'port/scene-materials/reports/effect-material-gles-v4';out.mkdir(parents=True,exist_ok=True)
binary=out/'effect-material-gles-v4.bin'
cmd=[str(ndk),'--target=x86_64-linux-android26','-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-static-libstdc++','-I'+str(root/'port/engine-textures'),'-I'+str(root/'port/android-native/app/src/main/cpp'),*[str(root/p) for p in sources],'-lEGL','-lGLESv2','-llog','-o',str(binary)]
temporary=out/'tmp';temporary.mkdir(exist_ok=True);environment=os.environ.copy()
for key in ('TMP','TEMP','TMPDIR'):environment[key]=str(temporary)
if '--existing' not in sys.argv:
 build=subprocess.run(cmd,capture_output=True,text=True,timeout=120,env=environment);(out/'compile.log').write_text(build.stdout+build.stderr)
 if build.returncode:print(build.stderr);raise SystemExit(build.returncode)
else:assert binary.exists()
print('BUILD PASS',binary)
if '--run' not in sys.argv:raise SystemExit(0)
shaders=root/'.local-inputs/effect-cache-v4/shaders.pak';assert shaders.exists()
remote='/data/local/tmp/dh2-effect-material-gles-v4'
for local,destination in [(binary,remote),(shaders,remote+'-shaders.pak')]:subprocess.run([str(adb),'-s','emulator-5554','push',str(local),destination],check=True,capture_output=True,timeout=20)
subprocess.run([str(adb),'-s','emulator-5554','shell','chmod','700',remote],check=True,capture_output=True,timeout=10)
run=subprocess.run([str(adb),'-s','emulator-5554','shell',remote,remote+'-shaders.pak'],capture_output=True,text=True,timeout=60)
receipt={'status':'PASS' if run.returncode==0 else 'FAIL','scope':'standalone actual GLES shader cache/reflection/typed current material directory/source equal-distance sort/actual pinned FIRE atlas draws and shared program cleanup across context cycles; quad and current color mutations declared fixture, not full native scene acceptance','binary_sha256':hashlib.sha256(binary.read_bytes()).hexdigest(),'sources':{p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in sources},'shader_pack_sha256':hashlib.sha256(shaders.read_bytes()).hexdigest(),'exit_code':run.returncode,'stdout':run.stdout,'stderr':run.stderr}
receipt['production_includes']={p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in ['port/android-native/app/src/main/cpp/renderer_effect_draw_v4.inc','port/android-native/app/src/main/cpp/renderer_effect_program_connection_v4.inc','port/android-native/app/src/main/cpp/renderer_effect_material_directory_v4.inc','port/android-native/app/src/main/cpp/renderer_effect_texture_v5.inc','port/scene-materials/material_matrix_v4.hpp']}
receipt['actual_asset_fixtures']={p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in ['.local-inputs/fire_atlas_native_fixture_v5.inc','.local-inputs/fire_bdae_native_fixture_v5.inc']}
(out/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt,indent=2));raise SystemExit(run.returncode)
