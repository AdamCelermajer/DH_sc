from pathlib import Path
import subprocess,json,hashlib
r=Path(__file__).resolve().parents[3];u='/mnt/c/Users/adamc/Desktop/workspace/DH_sc';results=[]
for opt in ['-O1','-O2']:
 for command in [['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','port/engine-ui/tests/authored_joystick_v1.cpp','port/engine-ui/authored_joystick_v1.cpp','-o','.local-inputs/authored_joystick_v1_host'],['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','.local-inputs/authored_joystick_v1_host']]:
  p=subprocess.run(['wsl.exe','--cd',u,'--exec',*command],capture_output=True,text=True)
  if p.returncode:print(p.stdout,p.stderr);raise SystemExit(p.returncode)
  if command[0]=='env':results.append({'optimization':opt,'result':json.loads(p.stdout)})
paths=['port/engine-ui/authored_joystick_v1.hpp','port/engine-ui/authored_joystick_v1.cpp','port/engine-ui/tests/authored_joystick_v1.cpp','port/engine-ui/reference/authored-joystick-v1/fixtures.bin']
report={'validation':'PASS','results':results,'source_sha256':{p:hashlib.sha256((r/p).read_bytes()).hexdigest() for p in paths},'scope':'Host replay of 1000 executed original whole event5 math/position/CTRLfalse cases; explicit direction/controller observers and prefix/reset checks, full HUD event selection/outerlevel/attacktransport not yet composed'}
(r/'port/engine-ui/reports/authored-joystick-v1-host.json').write_text(json.dumps(report,indent=2));print(json.dumps(report,indent=2))
