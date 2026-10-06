from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc';receipts=[]
sources=['port/level-world/character_loot_item_manager_v8.cpp','port/game-data/loot_audiovisual_v8.cpp','port/game-data/loot_temporary_inventory_v8.cpp','port/game-data/loot_creation_v8.cpp','port/game-data/loot_item_selection_v8.cpp','port/game-data/loot_entry_selection_v8.cpp','port/game-data/loot_table_selection_v8.cpp','port/game-data/loot_tables_v2.cpp','port/game-data/loot_power_resources_v7.cpp','port/game-data/loot_power_creation_v7.cpp','port/game-data/item_power_tables_v5.cpp','port/game-data/item_presentation_v5.cpp','port/level-world/tests/character_loot_item_manager_v8.cpp']
for opt in ['-O1','-O2']:
 exe='.local-inputs/loot_item_manager_v8_'+opt[1:]
 commands=[['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,'-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_game_data','-Wl,-rpath,'+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','-o',exe],['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,'.local-inputs/player-loot-v7/cache','.local-inputs/loot-world-v8/cache']]
 for args in commands:
  r=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);receipts.append(dict(args=args,exit_code=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,receipts[-1]
sha=lambda p:hashlib.sha256((root/p).read_bytes()).hexdigest()
report=dict(validation='PASS',commands=receipts,source_sha256={p:sha(p)for p in sources},fixture_sha256=sha('port/game-data/reference/loot-audiovisual-v8/fixtures.bin'),dependency_sha256=sha('.local-inputs/character-menu-native-v1-host/snapshot/libdh2_game_data.so'),dependency_current_source_attribution=False,production_world_spawn=False)
(root/'port/level-world/reports/character-loot-item-manager-v8-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps([json.loads(r['stdout'])for r in receipts if r['args'][0]=='env']))
