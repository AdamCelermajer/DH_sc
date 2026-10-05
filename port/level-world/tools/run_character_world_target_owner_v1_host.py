"""Private borrowed World targeting/controller proof; no central mutation."""
from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
closure={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')};assert len(closure)==8
sources=['port/level-world/character_world_target_owner_v1.cpp','port/level-world/character_skill_target_queries_v6.cpp','port/level-world/tests/character_world_target_owner_v1_host.cpp']
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz']
receipts=[]
for mode,opt in [('SAN','-O1'),('O2','-O2')]:
 exe='.local-inputs/character_world_target_owner_v1_'+mode
 for command in [['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,unix]]:
  result=subprocess.run(['wsl.exe','--cd',unix,'--exec',*command],capture_output=True,text=True);receipts.append(dict(mode=mode,args=command,exit_code=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0,receipts[-1]
 assert {name:sha(snapshot/name) for name in closure}==closure
files=sources+['port/level-world/character_world_target_owner_v1.hpp','port/level-world/tools/run_character_world_target_owner_v1_host.py']
report={'validation':'PASS','commands':receipts,'results':[json.loads(x) for r in receipts if r['args'][0]=='env' for x in r['stdout'].splitlines()],'source_sha256':{p:sha(root/p) for p in files},'private_dependency_sha256':closure,'dependency_current_source_attribution':False,'production_renderer_connection':False}
(root/'port/level-world/reports/character-world-target-owner-v1-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report['results']))
