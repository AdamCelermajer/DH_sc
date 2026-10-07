"""Isolated current-APK-linked original SWF flow; never launch/touch the game."""
from pathlib import Path
import subprocess,hashlib,json
ROOT=Path(__file__).resolve().parents[3]
ADB=r'C:\Users\adamc\AppData\Local\Android\Sdk\platform-tools\adb.exe'
PYTHON=r'C:\Users\adamc\.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
cache=ROOT/'port/android-native/app/src/main/assets/original-cache'
remote='/data/local/tmp/dh2-authored-panel-v2-cache'
files=[cache/'data/menus'/n for n in ['dqshared_droid.swf','dqcharmenu_droid.swf']]
files += [cache/'data/pydata'/n for n in ['common_text_pyarray.bin','common_text_pyarraynames.bin','common_text_pystructnames.bin','common_text_pycst.bin']]
files += sorted((cache/'data/text').glob('*.english'))+sorted((cache/'data/text').glob('*.symbols'))
files += sorted((cache/'data').glob('*.ttf'))
manifest=[]
def adb(*args):return subprocess.run([ADB,'-s','emulator-5554',*args],check=True,capture_output=True,text=True,timeout=40)
for group in ['menus','pydata','text']:adb('shell','mkdir','-p',remote+'/data/'+group)
for p in files:
 target=remote+'/'+p.relative_to(cache).as_posix();adb('push',str(p),target)
 manifest.append({'path':p.relative_to(ROOT).as_posix(),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()})
sources=['port/engine-ui/'+name+'.cpp' for name in ['authored_character_panel_v2','character_menu_movie_v1','menu_stack_owner_v1','menu_stack_actions_v1','menu_stack_v1','authored_character_menu_session_v1','authored_menu_lifecycle_v1','authored_menu_localization_v1','authored_character_menu_bridge_v1','authored_character_menu_routes_v1','character_menu_reload_action_v1','character_menu_reload_v1','menu_rollover_input_v1']]
sources+=['port/engine-ui/tests/authored_character_panel_v2.cpp']
runner=ROOT/'.local-inputs/panel_android_linked_v2.py'
code=(ROOT/'.local-inputs/root_android_linked_owner_test.py').read_text().replace("endswith('/model_renderer.cpp')","endswith('/authored_gameplay_hud_v1.cpp')")
code=code.replace("'-static-libstdc++'","'-static-libstdc++','-Wno-unused-parameter','-Wno-overloaded-virtual','-DTU_CONFIG_LINK_TO_FREETYPE=0','-DTU_CONFIG_LINK_TO_JPEGLIB=0','-DTU_CONFIG_LINK_TO_LIBPNG=0','-DTU_CONFIG_LINK_TO_THREAD=0'")
start=code.index('flags=[s for s in args[1:]')
end=code.index('hashes=[',start)
code=code[:start]+'''flags=[]
for i,s in enumerate(args[1:]):
 if s.startswith(('--target=','--sysroot=','-I')):flags.append(s)
 elif s=='-isystem':flags.extend([s,args[i+2]])
flags += ['-isystem',str(ROOT/'port/engine-ui/vendor/gameswf1714')]
'''+code[end:]
runner.write_text(code)
run=subprocess.run([PYTHON,str(runner),'authored-character-panel-v2',*sources,'--',remote],cwd=ROOT,capture_output=True,text=True,timeout=180)
out=ROOT/'port/engine-ui/reports/authored-character-panel-v2';out.mkdir(parents=True,exist_ok=True)
(out/'input-manifest.json').write_text(json.dumps({'assets':manifest,'scope':'Actual SWF and source MenuStack; external application/profile/GPU fixture hooks. No live game menu acceptance.'},indent=2))
(out/'run.log').write_text(run.stdout+run.stderr);print(run.stdout+run.stderr);raise SystemExit(run.returncode)
