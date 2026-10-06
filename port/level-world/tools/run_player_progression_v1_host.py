from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['port/level-world/player_progression_v1.cpp','port/level-world/player_xp_text_v1.cpp','port/level-world/tests/player_progression_v1_host.cpp']
libs=['-L.local-inputs/character-menu-native-v1-host/snapshot','-ldh2_game_data','-lz'];receipts=[]
for optimization in ['-O1','-O2']:
 exe='.local-inputs/player_progression_v1_host_'+optimization[1:]
 for args in [['g++','-std=c++17',optimization,'-g','-ffp-contract=off','-fno-fast-math','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,*libs,'-o',exe],['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-menu-native-v1-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe]]:
  p=subprocess.run(['wsl.exe','--cd',unix,'--exec',*args],capture_output=True,text=True);receipts.append({'args':args,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr});assert p.returncode==0,receipts[-1]
report={'validation':'PASS','receipts':receipts,'source_sha256':{p:hashlib.sha256((root/p).read_bytes()).hexdigest() for p in sources},'production_connected':False,'scope':'Original math replay; actual source cache/class progression with observer save/presentation; mandatory failure prefixes and death receipt'}
(root/'port/level-world/reports/player-progression-v1-host.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
