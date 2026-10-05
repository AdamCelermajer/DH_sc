"""Actual effects tables and owned Lua plus bounded source world/scene inputs."""
from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['character_world_runtime_v1.cpp','character_world_target_owner_v1.cpp','character_world_ai_relationship_v1.cpp','character_world_handle_v1.cpp','character_skill_target_queries_v6.cpp','character_fx_kernels_v1.cpp','fx_texture_animation_v1.cpp','character_fx_state_v1.cpp','character_mesh_fx_owner_v1.cpp','character_animation_event_owner_v1.cpp','tests/character_animation_event_owner_v1_host.cpp']
sources=['port/level-world/'+p for p in sources]
snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
before={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')}
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz']
receipts=[]
for mode,opt in [('SAN','-O1'),('O2','-O2')]:
 exe='.local-inputs/character_animation_event_owner_v1_'+mode
 commands=[['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,unix]]
 for args in commands:
  r=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True)
  receipts.append({'mode':mode,'command':args,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr});assert r.returncode==0,receipts[-1]
assert before=={name:sha(snapshot/name) for name in before}
assets=root/'port/android-native/app/src/main/assets/data'
inputs={str(p.relative_to(root)):sha(p) for p in assets.glob('effects_*.bin')}
report={'validation':'PASS','commands':receipts,'dependency_current_source_attribution':False,'source_sha256':{p:sha(root/p) for p in sources},'input_sha256':inputs,'frozen_dependency_sha256':before,'production_renderer_connection':False,'scope':'Actual bundled EffectsTables and owned ScriptOwnerV2 Lua Call(event, live lag); same registered world/scene source fields; CharacterFX row fallback; complete empty-floor invalid-FX domain; forest DFS; unknown cached floor, selected virtual and water positive FX reject; ordinary Lua diagnostics follow original continuation.'}
(root/'port/level-world/reports/character-animation-event-v1-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps([json.loads(r['stdout']) for r in receipts if r['command'][0]=='env']))
