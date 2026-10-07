from pathlib import Path
import subprocess,json,hashlib
r=Path(__file__).resolve().parents[3];u='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
results=[]
for optimization in ['-O1','-O2']:
 a=['g++','-std=c++17',optimization,'-g','-fsanitize=address,undefined','-DTU_CONFIG_LINK_TO_THREAD=0','-DTU_CONFIG_LINK_TO_FREETYPE=0','-DTU_CONFIG_LINK_TO_JPEGLIB=0','-DTU_CONFIG_LINK_TO_LIBPNG=0','-Iport/engine-ui/vendor/gameswf1714','port/engine-ui/tests/authored_gameplay_hud_v1.cpp','port/engine-ui/authored_gameplay_hud_v1.cpp','-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_engine_ui','-ldh2_script_runtime','-lz','-o','.local-inputs/authored_gameplay_hud_v1_host']
 for command in [a,['env','LD_LIBRARY_PATH='+u+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','.local-inputs/authored_gameplay_hud_v1_host']]:
  p=subprocess.run(['wsl.exe','--cd',u,'--exec',*command],capture_output=True,text=True)
  if p.returncode:print(p.stderr);raise SystemExit(p.returncode)
  if command[0]=='env':results.append({'optimization':optimization,'result':json.loads(p.stdout)})
sources=['port/engine-ui/authored_gameplay_hud_v1.hpp','port/engine-ui/authored_gameplay_hud_v1.cpp','port/engine-ui/tests/authored_gameplay_hud_v1.cpp','port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf']
report={'validation':'PASS','results':results,'sha256':{p:hashlib.sha256((r/p).read_bytes()).hexdigest() for p in sources}}
(r/'port/engine-ui/reports/authored-gameplay-hud-v1-host.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
