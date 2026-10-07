"""Parent-authorized isolated EGL runner. Default only builds; --run deploys
and executes standalone fixture, never touches app/APK/process state."""
from pathlib import Path
import subprocess,sys,hashlib,json,os
root=Path(__file__).resolve().parents[3]
ndk=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\ndk\29.0.14206865\toolchains\llvm\prebuilt\windows-x86_64\bin\clang++.exe')
adb=Path(r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe')
sources=['port/engine-textures/tests/renderer_effect_texture_v5_gles_test.cpp','port/engine-textures/blood_texture_image_v1.cpp','port/engine-textures/texture_owner_v1.cpp','port/engine-textures/texture_driver_fields_v1.cpp','port/engine-textures/texture_binding_owner_v1.cpp','port/engine-textures/texture_mipmap_v1.cpp','port/engine-textures/texture_unbind_v1.cpp','port/engine-textures/textures.cpp','port/engine-textures/pvrtc.cpp','port/engine-resources/resources.cpp','port/scene-materials/particle_scene_v1.cpp','port/scene-materials/scene.cpp','port/engine-math/math.cpp']
out=root/'port/engine-textures/reference/texture-owner-v1/gles-v5-test';out.mkdir(parents=True,exist_ok=True)
binary=out/'effect-texture-v5-gles-test.bin'
cmd=[str(ndk),'--target=x86_64-linux-android26','-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-static-libstdc++','-I'+str(root/'port/engine-textures'),'-I'+str(root/'port/android-native/app/src/main/cpp'),*[str(root/p) for p in sources],'-lEGL','-lGLESv2','-llog','-o',str(binary)]
temporary=out/'tmp';temporary.mkdir(exist_ok=True);environment=os.environ.copy()
for key in ('TMP','TEMP','TMPDIR'):environment[key]=str(temporary)
build=subprocess.run(cmd,capture_output=True,text=True,timeout=60,env=environment);(out/'compile.log').write_text(build.stdout+build.stderr)
if build.returncode:print(build.stderr);raise SystemExit(build.returncode)
print('BUILD PASS',binary)
if '--run' not in sys.argv:raise SystemExit(0)
remote='/data/local/tmp/dh2-effect-texture-v5-gles-test'
subprocess.run([str(adb),'-s','emulator-5554','push',str(binary),remote],check=True,capture_output=True,timeout=20)
subprocess.run([str(adb),'-s','emulator-5554','shell','chmod','700',remote],check=True,capture_output=True,timeout=10)
run=subprocess.run([str(adb),'-s','emulator-5554','shell',remote],capture_output=True,text=True,timeout=60)
receipt={'status':'PASS' if run.returncode==0 else 'FAIL','scope':'actual packaged FIRE material atlas, isolated EGL/GLES V5 upload/mipmap/draw/readback/unbind/contextloss and renderer GLstate restore; not whole gameFX acceptance','binary_sha256':hashlib.sha256(binary.read_bytes()).hexdigest(),'sources':[{'path':p,'sha256':hashlib.sha256((root/p).read_bytes()).hexdigest()} for p in sources],'adapter_sha256':hashlib.sha256((root/'port/android-native/app/src/main/cpp/renderer_effect_texture_v5.inc').read_bytes()).hexdigest(),'exit_code':run.returncode,'stdout':run.stdout,'stderr':run.stderr}
(out/'receipt.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps(receipt,indent=2));raise SystemExit(run.returncode)
