from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3]
sources=['port/level-world/player_manager_owner_v1.cpp','port/level-world/player_manager_loot_queries_v8.cpp','port/level-world/tests/player_manager_owner_v1.cpp']
commands=[]
for opt in ['-O1','-O2']:
 exe='.local-inputs/player_manager_owner_v1_'+opt[1:]
 for args in [['g++','-std=c++17',opt,'-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,'-o',exe],['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,'port/level-world/reference/player-manager-owner-v1/query-fixtures.bin']]:
  result=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec',*args],capture_output=True,text=True)
  commands.append(dict(args=args,exit_code=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,commands[-1]
report=dict(validation='PASS',commands=commands,source_sha256={p:hashlib.sha256((root/p).read_bytes()).hexdigest()for p in sources},network_constructor_fixture=True,canonical_world=False)
(root/'port/level-world/reports/player-manager-owner-v1-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps([json.loads(r['stdout'])for r in commands if r['args'][0]=='env']))
