from pathlib import Path
import subprocess,json,hashlib
r=Path(__file__).resolve().parents[3];u='/mnt/c/Users/adamc/Desktop/workspace/DH_sc';results=[]
for optimization in ['-O1','-O2']:
 args=['g++','-std=c++17',optimization,'-g','-fsanitize=address,undefined','-DTU_CONFIG_LINK_TO_THREAD=0','-DTU_CONFIG_LINK_TO_FREETYPE=0','-DTU_CONFIG_LINK_TO_JPEGLIB=0','-DTU_CONFIG_LINK_TO_LIBPNG=0','-Iport/engine-ui/vendor/gameswf1714','port/engine-ui/tests/authored_hud_skill_refresh_v5.cpp','port/engine-ui/authored_gameplay_hud_v1.cpp','-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_engine_ui','-ldh2_script_runtime','-lz','-o','.local-inputs/authored_hud_skill_refresh_v5_host']
 for command in [args,['env','LD_LIBRARY_PATH='+u+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','.local-inputs/authored_hud_skill_refresh_v5_host']]:
  p=subprocess.run(['wsl.exe','-d','Ubuntu','--cd',u,'--exec',*command],capture_output=True,text=True)
  if p.returncode:print(p.stdout+p.stderr);raise SystemExit(p.returncode)
  if command[0]=='env':results.append(dict(optimization=optimization,result=json.loads(p.stdout)))
paths=['port/engine-ui/authored_gameplay_hud_v1.cpp','port/engine-ui/tests/authored_hud_skill_refresh_v5.cpp','port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf']
report=dict(validation='PASS',results=results,sha256={p:hashlib.sha256((r/p).read_bytes()).hexdigest() for p in paths});(r/'port/engine-ui/reports/authored-hud-skill-refresh-v5.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
