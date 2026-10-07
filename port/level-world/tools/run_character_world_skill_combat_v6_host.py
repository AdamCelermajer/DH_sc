from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
names=['character_world_skill_combat_v6.cpp','character_world_runtime_v1.cpp','character_world_target_owner_v1.cpp','character_world_ai_relationship_v1.cpp','character_world_handle_v1.cpp','character_skill_target_queries_v6.cpp','character_skill_application_v6.cpp','character_hit_v6.cpp','character_skill_attack_v6.cpp','character_combat_result_v6.cpp','tests/character_world_skill_combat_v6_host.cpp']
sources=['port/level-world/'+p for p in names];snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot';closure={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')}
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz'];receipts=[]
for mode,optimization in [('SAN','-O1'),('O2','-O2')]:
 exe='.local-inputs/character_world_skill_combat_v6_'+mode
 commands=[['g++','-std=c++17',optimization,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,unix]]
 for args in commands:
  r=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);receipts.append({'mode':mode,'args':args,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr});assert r.returncode==0,receipts[-1]
 assert closure=={name:sha(snapshot/name) for name in closure}
report={'validation':'PASS','commands':receipts,'source_sha256':{p:sha(root/p) for p in sources},'dependency_sha256':closure,'dependency_current_source_attribution':False,'full_skill_application':False,'production_renderer_connection':False}
(root/'port/level-world/reports/character-world-skill-combat-v6-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps([json.loads(r['stdout']) for r in receipts if r['args'][0]=='env']))
