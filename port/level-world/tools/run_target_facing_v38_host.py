"""Bounded source targeting tests; no emulator or app lifecycle operations."""
from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['character_world_runtime_v1.cpp','character_world_target_owner_v1.cpp','character_world_ai_relationship_v1.cpp','character_world_ai_neutral_v1.cpp','character_world_handle_v1.cpp','character_skill_target_queries_v6.cpp','character_attack_geometry.cpp','character_world_attack_geometry_v1.cpp','character_close_range_v38.cpp','tests/character_target_facing_v38.cpp']
sources=['port/level-world/'+p for p in sources]
snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
before={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')}
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz']
receipts=[]
for opt in ['-O1','-O2']:
 exe='.local-inputs/target-facing-v38-host'
 for args in [['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe]]:
  r=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);receipts.append({'command':args,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr});print(r.returncode,r.stdout,r.stderr);assert r.returncode==0
assert before=={name:sha(snapshot/name) for name in before}
report={'validation':'PASS','commands':receipts,'source_sha256':{p:sha(root/p) for p in sources},'dependency_sha256':before,'current_renderer_live_acceptance':False,'scope':'Same registered handle/cache/desired heading, moving cardinal targets, removed actor, ranged cached property shortcut. External inventory/interaction/debug endpoint facts are fixtures. No all-skills/camera/sword-trail live proof.'}
(root/'port/level-world/reports/target-facing-v38-host.json').write_text(json.dumps(report,indent=2)+'\n')
