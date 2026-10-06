from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc';sources=['port/level-world/tests/character_world_clear_aggro_v40.cpp','port/level-world/character_world_clear_aggro_v40.cpp','port/level-world/character_target_bindings.cpp','port/game-data/aggro.cpp'];snapshot='.local-inputs/character-menu-native-v1-host/snapshot'
exe='.local-inputs/world-clear-aggro-v40';receipts=[]
for opt in ('-O1','-O2'):
 for args in [['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,'-L'+snapshot,'-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz','-o',exe],['env','LD_LIBRARY_PATH='+snapshot,'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,'port/level-world/reference/target-cleanup-v40/clear-original.bin']]:
  r=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);print(r.returncode,r.stdout,r.stderr);receipts.append({'command':args,'exit':r.returncode,'stdout':r.stdout,'stderr':r.stderr});assert r.returncode==0
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
(root/'port/level-world/reports/world-clear-aggro-v40-host.json').write_text(json.dumps({'validation':'PASS','commands':receipts,'sources':{p:sha(root/p) for p in sources},'dependencies':{p.name:sha(p) for p in (root/snapshot).glob('*.so')},'live_acceptance':False,'endpoint_facts_are_fixtures':True},indent=2)+'\n')
