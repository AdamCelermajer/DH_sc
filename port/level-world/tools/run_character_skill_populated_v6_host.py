"""Private same-owner populated prefix proof; never mutates central DSOs."""
from pathlib import Path
import hashlib,json,subprocess
root=Path(__file__).resolve().parents[3]
snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
closure={p.name:sha(p)for p in snapshot.iterdir()if p.is_file()and(p.suffix in('.so','.a'))}
assert len(closure)==8,closure
unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['character_player_skills_v6.cpp','character_skill_mana_v5.cpp','character_skill_readonly_v6.cpp','character_target_search_v5.cpp','character_skill_native_v6.cpp','character_skill_combat_v6.cpp','character_skill_attack_v6.cpp','character_combat_result_v6.cpp','character_skill_application_v6.cpp','character_hit_v6.cpp','character_skill_target_queries_v6.cpp','tests/character_skill_populated_v6_host.cpp']
sources=['port/level-world/'+p for p in sources]
sources.append('port/level-world/character_skill_aggro_v6.cpp')
sources.append('port/level-world/character_skill_save_reload_v6.cpp')
sources.append('port/level-world/character_skills_owner_v6.cpp')
sources.append('port/level-world/character_skill_reload_v6.cpp')
sources.append('port/game-data/player_save_load_owner_v1.cpp')
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz']
receipts=[]
for mode,optimization in [('SAN','-O1'),('O2','-O2')]:
 exe='.local-inputs/character_skill_populated_v6_'+mode
 commands=[['g++','-std=c++17',optimization,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,unix,'/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip']]
 for command in commands:
  result=subprocess.run(['wsl.exe','--cd',unix,'--exec',*command],capture_output=True,text=True)
  receipts.append(dict(mode=mode,args=command,exit_code=result.returncode,stdout=result.stdout,stderr=result.stderr))
  assert result.returncode==0,receipts[-1]
 assert {name:sha(snapshot/name)for name in closure}==closure
files=sources+['port/level-world/tools/run_character_skill_populated_v6_host.py','port/level-world/tools/prepare_character_skill_populated_v6_host.py','port/level-world/tests/character_skill_populated_v6_body.inc','port/level-world/tests/character_skill_populated_v6_main.inc']
files+=['port/level-world/'+p for p in ['character_skill_combat_v6.hpp','character_skill_native_v6.hpp','character_skill_readonly_v6.hpp','character_skill_target_queries_v6.hpp','character_player_skills_v6.hpp']]
report=dict(validation='PASS',commands=receipts,results=[json.loads(x)for r in receipts if r['args'][0]=='env' for x in r['stdout'].splitlines()],source_sha256={p:sha(root/p)for p in files},private_dependency_sha256=closure,dependency_current_source_attribution=False,full_target_application=False,full_campaign=False)
(root/'port/level-world/reports/character-skill-populated-v6-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report['results']))
