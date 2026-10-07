"""Prove the V6 compatibility owner with immutable historical SAN dependencies."""
from pathlib import Path
import hashlib,json,subprocess
root=Path(__file__).resolve().parents[3]
snapshot=root/'.local-inputs/character-skill-native-v5-host/snapshot'
prior=json.loads((root/'port/level-world/reports/character-skill-native-v5-host-audit.json').read_text())
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for name,digest in prior['private_snapshot'].items():assert sha(snapshot/name)==digest,name
unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['port/level-world/character_player_skills_v6.cpp','port/level-world/character_skill_mana_v5.cpp','port/level-world/character_skill_native_v5.cpp','port/level-world/character_target_search_v5.cpp','port/game-data/fresh_inventory_owned_v4.cpp','port/level-world/tests/character_player_skills_v6_host.cpp']
sources.extend(['port/level-world/character_skills_owner_v6.cpp','port/level-world/character_skill_save_reload_v6.cpp'])
sources.append('port/level-world/character_skill_reload_v6.cpp')
commands=[['g++','-std=c++17','-O2','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,'-Iport/level-world','-L.local-inputs/character-skill-native-v5-host/snapshot','.local-inputs/character-skill-native-v5-host/snapshot/libdh2_zip_asset_pack_v1.a','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-lz','-o','.local-inputs/character_player_skills_v6_host'],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-skill-native-v5-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1','.local-inputs/character_player_skills_v6_host','port/level-world/reference/character-game-design/real-cache-inputs.bin','port/android-native/app/src/main/assets','.local-inputs/character-skill-session-v2/cache','/mnt/c/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip','.local-inputs/items-discovery']]
receipts=[]
for command in commands:
 result=subprocess.run(['wsl.exe','--cd',unix,'--exec',*command],capture_output=True,text=True)
 receipts.append(dict(args=command,exit_code=result.returncode,stdout=result.stdout,stderr=result.stderr))
 assert result.returncode==0,receipts[-1]
for name,digest in prior['private_snapshot'].items():assert sha(snapshot/name)==digest,name
files=sources+['port/level-world/character_player_skills_v6.hpp','port/level-world/tools/prepare_character_player_skills_v6.py','port/level-world/tools/prepare_character_player_skills_v6_host.py','port/level-world/tools/run_character_player_skills_v6_host.py']
report=dict(validation='PASS',sanitizers=['address','undefined'],leak_detection=True,commands=receipts,results=[json.loads(x)for x in receipts[-1]['stdout'].splitlines()],source_sha256={p:sha(root/p)for p in files},private_dependency_sha256=prior['private_snapshot'],dependency_current_source_attribution=False,same_owner_native_buff_lifetime=True,populated_world=False,full_target_application=False)
(root/'port/level-world/reports/character-player-skills-v6-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report['results']))
