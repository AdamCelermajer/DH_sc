from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc';receipts=[]
sources=['loot_creation_v8.cpp','loot_temporary_inventory_v8.cpp','loot_item_selection_v8.cpp','loot_entry_selection_v8.cpp','loot_table_selection_v8.cpp','loot_tables_v2.cpp','loot_power_resources_v7.cpp','loot_power_creation_v7.cpp','tests/loot_creation_v8.cpp','tests/loot_power_creation_v7_fixture.cpp']
dirs=['/home/adampalace/dh2-world-build','/home/adampalace/dh2-world-build/game-data','/home/adampalace/dh2-world-build/engine-ui','/home/adampalace/dh2-world-build/script-runtime']
for opt in ['-O1','-O2']:
 exe='.local-inputs/loot_creation_v8_'+opt[1:]
 commands=[['g++','-std=c++17',opt,'-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsanitize=address,undefined','-fno-omit-frame-pointer',*['port/game-data/'+s for s in sources],*['-L'+d for d in dirs],'-Wl,-rpath,'+':'.join(dirs),'-ldh2_level_world','-ldh2_engine_ui','-ldh2_game_data','-ldh2_script_runtime','-o',exe],['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,'.local-inputs/player-loot-v7/cache','.local-inputs/actors','port/android-native/app/src/main/assets','.local-inputs/player-item-effects-v5/private-save','port/game-data/reference/loot-creation-v8/fixtures.bin']]
 for args in commands:
  r=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);receipts.append(dict(args=args,exit_code=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,receipts[-1]
sha=lambda p:hashlib.sha256((root/p).read_bytes()).hexdigest()
report=dict(validation='PASS',commands=receipts,source_sha256={p:sha(p)for p in ['port/game-data/'+s for s in sources]},world_spawn_pickup=False,dependency_current_source_attribution=False)
(root/'port/game-data/reports/loot-creation-v8-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps([json.loads(r['stdout'])for r in receipts if r['args'][0]=='env']))
