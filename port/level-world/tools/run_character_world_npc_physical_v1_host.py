from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
names=['character_world_npc_properties_v1.cpp','character_world_npc_object_v1.cpp','navigation_objects.cpp','character_world_npc_physical_v1.cpp','character_npc_body.cpp','physical_world.cpp','native_body.cpp','character_world_npc_collision_v1.cpp','character_script_collision.cpp','character_ai_events.cpp','character_world_npc_initialization_v1.cpp','character_world_npc_state_owner_v1.cpp','character_world_runtime_v1.cpp','character_world_target_owner_v1.cpp','character_world_ai_relationship_v1.cpp','character_world_handle_v1.cpp','character_skill_target_queries_v6.cpp','character_state_owner_extensions.cpp','character_state_owner_behavior.cpp','character_state_owner_frame.cpp','tests/character_world_npc_physical_v1_host.cpp']
names.append('character_world_npc_init_final_v1.cpp')
names.append('character_world_npc_scene_v1.cpp')
names.append('character_world_npc_bounds_v1.cpp')
sources=['port/level-world/'+p for p in names];snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot';closure={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')}
inputs=['port/level-world/reference/character-world-npc-object-v1/crypt01-object-properties.bin','port/level-world/reference/character-world-npc-object-v1/crypt01-object-properties.json','port/level-world/tools/produce_character_world_npc_properties_v1.py','port/level-world/reference/character-game-design/real-cache-inputs.bin','port/level-world/reference/actor-initialization/crypt01-actor-initialization.bin','port/android-native/app/src/main/assets/worlds/crypt01.dact','port/android-native/app/src/main/assets/actors/skeleton.bdae','port/android-native/app/src/main/assets/actors/slime_green_v2.bdae','port/android-native/app/src/main/assets/actors/ghost.bdae','port/android-native/app/src/main/assets/data/scripts/ai/_commons.luac','port/android-native/app/src/main/assets/data/scripts/ai/monster.luac']
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz'];receipts=[]
for opt in ['-O1','-O2']:
 exe='.local-inputs/character_world_npc_physical_v1_host_'+opt[1:]
 commands=[['g++','-Iport/physics-backend/box2d-2.0.1/Include','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,unix]]
 for args in commands:
  result=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);receipts.append({'args':args,'exit_code':result.returncode,'stdout':result.stdout,'stderr':result.stderr});assert result.returncode==0,receipts[-1]
 assert closure=={name:sha(snapshot/name) for name in closure}
sources.append('port/level-world/character_world_npc_scene_bridge_v1.inc')
report={'validation':'PASS','commands':receipts,'source_sha256':{p:sha(root/p) for p in sources+['port/level-world/tests/character_script_session.cpp']},'input_sha256':{p:sha(root/p) for p in inputs},'dependency_sha256':closure,'dependency_current_source_attribution':False,'full_Character_InitPost':False,'full_NPC_AI':False,'production_renderer_connection':False}
(root/'port/level-world/reports/character-world-npc-physical-v1-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps([json.loads(r['stdout']) for r in receipts if r['args'][0]=='env']))
