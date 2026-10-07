"""Checks live borrowed registration against private frozen host dependencies."""
from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['character_world_runtime_v1.cpp','character_world_target_owner_v1.cpp','character_world_ai_relationship_v1.cpp','character_world_handle_v1.cpp','character_skill_target_queries_v6.cpp','character_ai_attack.cpp','character_target_search.cpp','character_target_bindings.cpp','character_world_ai_can_attack_v1.cpp','character_world_player_attack_owner_v1.cpp','tests/character_world_player_attack_owner_v1_host.cpp']
sources=['port/level-world/'+p for p in sources]
snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
before={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')}
exe='.local-inputs/character_world_player_attack_owner_v1_host'
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz']
commands=[['g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe]]
receipts=[]
for args in commands:
 r=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True)
 receipts.append({'command':args,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr});assert r.returncode==0,receipts[-1]
assert before=={name:sha(snapshot/name) for name in before}
report={'validation':'PASS','commands':receipts,'dependency_current_source_attribution':False,'source_sha256':{p:sha(root/p) for p in sources},'frozen_dependency_sha256':before,'production_renderer_connection':False,'scope':'Actual registered-world source target search and heap ordering; headed and unheaded selection; original SetTarget/BackupTarget; continued attack; controller gate; required provider failure preserves target prefix and suppresses FSM; named geometry/debug/FSM fixtures are not production providers.'}
(root/'port/level-world/reports/character-world-player-attack-v1-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':'PASS','host_result':json.loads(receipts[-1]['stdout'])}))
