"""Run only after root announces the combined APK snapshot is ready.
Reuses canonical current-APK linked runner and isolated5554 test directory;
does not install/start/stop an app. Production helpers come from APK libraries.
"""
from pathlib import Path
import sys,subprocess,json,hashlib
root=Path(__file__).resolve().parents[3]
assert '--run' in sys.argv,'Explicit --run after combined-build readiness required'
shader=root/'.local-inputs/effect-cache-v4/shaders.pak'
adb=r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe'
remote='/data/local/tmp/dh2-effect-material-v4-linked-shaders.pak'
subprocess.run([adb,'-s','emulator-5554','push',str(shader),remote],check=True,capture_output=True,timeout=20)
runner=root/'.local-inputs/root_android_linked_owner_test.py'
script=runner.read_text().replace("'-o',str(binary)","'-lEGL','-lGLESv2','-llog','-o',str(binary)")
script=script.replace("cmd=[args[0],*flags,","cmd=[args[0],*flags,'-I'+str(ROOT/'port/android-native/app/src/main/cpp'),'-I'+str(ROOT/'port/engine-textures'),")
script=script.replace('actual APK-linked native object owners, same base/property storage with declared external service fixtures; not full live level acceptance','actual current APK-linked GLES shared source-key program/reflection/typed material comparison/source equal-distance sort and actual FIRE atlas draw/context lifecycle; quad/current-color/node-distance fixtures, not whole native scene or skill acceptance')
sys.argv=['root_android_linked_owner_test.py','effect-material-gles-v4-linked','port/scene-materials/tests/effect_material_gles_v4.cpp','--',remote]
exec(compile(script,str(runner),'exec'),{'__file__':str(runner),'__name__':'__main__'})
