from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[1];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
out=root/'port/level-loader/reports/native-gslevel-v27';out.mkdir(parents=True,exist_ok=True)
sources=['port/level-loader/tests/native_gslevel_runtime_v27.cpp','port/level-loader/native_gslevel_runtime_v27.cpp','port/level-loader/canonical_level_loading_v26.cpp','port/level-loader/native_level_application_v25.cpp','port/level-loader/canonical_level_context_v1.cpp','port/level-loader/level_constructor_bindings_v4.cpp','port/level-loader/level_constructor_v3.cpp','port/engine-ui/loading_menu_v1.cpp']
sources += ['port/level-world/generic_lua_script_owner_v13.cpp','port/level-world/event_manager_owner_v12.cpp','port/level-world/level_savegame_runtime_v1.cpp','port/level-world/level_savegame_owner_v1.cpp','port/level-world/level_savegame_cache_v1.cpp']
libdirs=['.local-inputs/gslevel-v27-script-host','/home/adampalace/dh2-world-build','/home/adampalace/dh2-world-build/game-data','/home/adampalace/dh2-world-build/engine-ui','/home/adampalace/dh2-world-build/engine-skinning','/home/adampalace/dh2-world-build/engine-skinning/engine-animation']
exe='.local-inputs/native_gslevel_v27_host'
command=['g++','-std=c++17','-O1','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-g','-fsanitize=address,undefined','-ffunction-sections','-fdata-sections','-Iport/level-world','-Iport/game-data','-Iport/script-runtime','-Iport/engine-ui',*sources,*['-L'+p for p in libdirs],'-Wl,--start-group','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_ui','-ldh2_script_runtime','-Wl,--end-group','-Wl,--gc-sections','-lz','-o',exe]
run=['env','LD_LIBRARY_PATH='+':'.join(libdirs),'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,'port/level-world/reference/character-game-design/real-cache-inputs.bin','.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/level','port/level-loader/reports/native-gslevel-v27/missing-save-files']
(out/'missing-save-files').mkdir(exist_ok=True)
receipts=[]
for args in [['cmake','-S','port/script-runtime','-B','.local-inputs/gslevel-v27-script-host','-DDH2_SCRIPT_SANITIZERS=ON'],['cmake','--build','.local-inputs/gslevel-v27-script-host','--target','dh2_script_runtime','--parallel','2'],command,run]:
 p=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True,timeout=120)
 receipts.append({'command':args,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr});print(p.returncode,p.stdout,p.stderr[-5000:],flush=True)
 (out/'host-receipt.json').write_text(json.dumps({'status':'PASS' if p.returncode==0 else 'FAIL','scope':'Actual cache/C1/current storage/Loading composition; explicitly declared Menu and animation-manager deeper fixtures, not live UI or Level.Init acceptance','receipts':receipts,'sources':[{'path':s,'sha256':hashlib.sha256((root/s).read_bytes()).hexdigest()} for s in sources]},indent=2)+'\n')
 if p.returncode:raise SystemExit(p.returncode)
