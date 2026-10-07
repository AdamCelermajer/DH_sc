from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['port/level-world/'+p for p in ['character_world_player_aggro_v2.cpp','character_world_aggro_event_v1.cpp','character_player_aggro_owner_v1.cpp','vox_music_state_owner_v1.cpp','level_config_music_owner_v1.cpp','tests/character_world_player_aggro_v2.cpp']]
snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
closure={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in ('.a','.so')}
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz'];receipts=[]
for opt in ['-O1','-O2']:
 exe='.local-inputs/character_world_player_aggro_v2_'+opt[1:]
 commands=[['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,'port/level-world/reference/character-player-aggro-v1/crypt01-level-config-music.bin']]
 for args in commands:
  r=subprocess.run(['wsl.exe','-d','Ubuntu','--cd',unix,'--exec',*args],capture_output=True,text=True,timeout=60)
  receipts.append({'args':args,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr});print(r.stdout,r.stderr,flush=True);assert not r.returncode,receipts[-1]
 assert closure=={p:sha(snapshot/p) for p in closure}
report={'validation':'PASS','commands':receipts,'source_sha256':{p:sha(root/p) for p in sources},'dependency_sha256':closure,'scope':'Source CharAI relay and selected AISPlayer OnAggro composition over distinct live identities, same counters, actual Crypt config, real Vox ctor no-track branch; online/weight/threshold/world providers explicit fixtures. Reached audio/missing receiver failures retain source prefixes. No full Vox audio claim.'}
(root/'port/level-world/reports/character-world-player-aggro-v2-host.json').write_text(json.dumps(report,indent=2)+'\n')
