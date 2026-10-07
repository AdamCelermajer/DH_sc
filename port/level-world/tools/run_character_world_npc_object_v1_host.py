from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
sources=['port/level-world/character_world_npc_object_v1.cpp','port/level-world/character_world_npc_room_v1.cpp','port/level-world/character_design_services.cpp','port/level-world/navigation_objects.cpp','port/level-world/tests/character_world_npc_object_v1.cpp']
snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot'
closure={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')};receipts=[]
for opt in ['-O1','-O2']:
 exe='.local-inputs/character_world_npc_object_v1_'+opt[1:]
 for args in [
  ['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,'-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-ldh2_script_runtime','-lz','-o',exe],
  ['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe]]:
  r=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True)
  receipts.append(dict(args=args,exit_code=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,receipts[-1]
assert closure=={name:sha(snapshot/name) for name in closure}
report=dict(validation='PASS',commands=receipts,source_sha256={p:sha(root/p) for p in sources},dependency_sha256=closure,dependency_current_source_attribution=False,production_renderer_connection=False,full_NPC_AI=False)
(root/'port/level-world/reports/character-world-npc-object-v1-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps([json.loads(r['stdout']) for r in receipts if r['args'][0]=='env']))
