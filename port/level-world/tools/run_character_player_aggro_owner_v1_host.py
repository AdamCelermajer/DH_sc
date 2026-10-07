from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
sources=['port/level-world/character_player_aggro_owner_v1.cpp','port/level-world/vox_music_state_owner_v1.cpp','port/level-world/level_config_music_owner_v1.cpp','port/level-world/tests/character_player_aggro_owner_v1_host.cpp'];snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot';closure={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')}
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz'];receipts=[]
for mode,opt in [('SAN','-O1'),('O2','-O2')]:
 exe='.local-inputs/character_player_aggro_owner_v1_'+mode
 for args in [['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,unix]]:
  run=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);receipts.append(dict(mode=mode,args=args,exit_code=run.returncode,stdout=run.stdout,stderr=run.stderr));assert not run.returncode,receipts[-1]
 assert closure=={name:sha(snapshot/name) for name in closure}
report={'validation':'PASS','commands':receipts,'source_sha256':{p:sha(root/p) for p in sources},'oracle_sha256':sha(root/'port/level-world/reference/character-player-aggro-v1/source-fixture.bin'),'dependency_sha256':closure,'dependency_current_source_attribution':False,'production_music_provider':False}
(root/'port/level-world/reports/character-player-aggro-v1-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps([json.loads(r['stdout']) for r in receipts if r['args'][0]=='env']))
