from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
sources=['port/level-world/trophy_manager_owner_v1.cpp','port/level-world/tests/trophy_manager_owner_v1_host.cpp']
snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot';closure={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')}
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz'];receipts=[]
for optimization in ['-O1','-O2']:
 exe='.local-inputs/trophy_manager_owner_v1_host_'+optimization[1:]
 commands=[['g++','-std=c++17',optimization,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe]]
 for args in commands:
  result=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);receipts.append({'args':args,'exit_code':result.returncode,'stdout':result.stdout,'stderr':result.stderr});assert result.returncode==0,receipts[-1]
 assert closure=={name:sha(snapshot/name) for name in closure}
report={'validation':'PASS','commands':receipts,'source_sha256':{p:sha(root/p) for p in sources},'dependency_sha256':closure,'dependency_current_source_attribution':False,'production_unlock_services':False,'renderer_connection':False}
(root/'port/level-world/reports/trophy-manager-owner-v1-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps([json.loads(r['stdout']) for r in receipts if r['args'][0]=='env']))
