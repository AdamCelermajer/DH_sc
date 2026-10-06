from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['port/level-world/character_npc_initpost_owner_v1.cpp','port/level-world/tests/character_npc_initpost_owner_v1.cpp']
snapshot=root/'.local-inputs/character-menu-native-v1-host/snapshot'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
closure={p.name:sha(p) for p in snapshot.iterdir() if p.suffix in('.so','.a')}
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_zip_asset_pack_v1','-lz'];receipts=[]
for opt in ['-O1','-O2']:
 exe='.local-inputs/character_npc_initpost_owner_v1_host_'+opt[1:]
 for args in [['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe]]:
  q=subprocess.run(['wsl.exe','-d','Ubuntu','--cd',unix,'--exec',*args],capture_output=True,text=True,timeout=60)
  receipts.append({'args':args,'exit_code':q.returncode,'stdout':q.stdout,'stderr':q.stderr});print(q.stdout,q.stderr,flush=True);assert q.returncode==0,receipts[-1]
 assert closure=={n:sha(snapshot/n) for n in closure}
report={'validation':'PASS','commands':receipts,'source_sha256':{p:sha(root/p) for p in sources},'dependencies':closure,'scope':'NPC Character InitPost source control-flow coordinator; ordered original helper endpoints represented by explicit fixtures. Complete visual, physical, save, FX, sound and script backend not claimed. No original differential claim.'}
(root/'port/level-world/reports/character-npc-initpost-owner-v1-host.json').write_text(json.dumps(report,indent=2)+'\n')
