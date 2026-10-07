from pathlib import Path
import subprocess,json,hashlib
r=Path(__file__).resolve().parents[3];u='/mnt/c/Users/adamc/Desktop/workspace/DH_sc';results=[]
sources=['port/engine-ui/authored_hud_options_bridge_v1.cpp','port/engine-ui/owned_hud_settings_v1.cpp','port/engine-ui/tests/authored_hud_options_bridge_v1.cpp']
for opt in ['-O1','-O2']:
 for command in [['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined',*sources,'-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_engine_ui','-ldh2_game_data','-o','.local-inputs/authored_hud_options_bridge_v1_host'],['env','LD_LIBRARY_PATH='+u+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','.local-inputs/authored_hud_options_bridge_v1_host']]:
  try:p=subprocess.run(['wsl.exe','--cd',u,'--exec',*command],capture_output=True,text=True,timeout=45)
  except subprocess.TimeoutExpired:
   report={'validation':'UNAVAILABLE','reason':'WSL command did not return within45seconds; no WSL restart performed','command':command,'source_sha256':{p:hashlib.sha256((r/p).read_bytes()).hexdigest() for p in sources}}
   (r/'port/engine-ui/reports/authored-hud-options-bridge-v1-host.json').write_text(json.dumps(report,indent=2));print(json.dumps(report));raise SystemExit(2)
  if p.returncode:print(p.stdout,p.stderr);raise SystemExit(p.returncode)
  if command[0]=='env':results.append({'optimization':opt,'result':json.loads(p.stdout)})
report={'validation':'PASS','results':results,'source_sha256':{p:hashlib.sha256((r/p).read_bytes()).hexdigest() for p in sources},'scope':'Same actual design settings + frozen whole native options/Ipod kernels; AS member/result and localization/platform observers; no live application claim'}
(r/'port/engine-ui/reports/authored-hud-options-bridge-v1-host.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
