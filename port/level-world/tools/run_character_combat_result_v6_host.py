"""Replay V6 result corpus with the immutable V5 private SAN dependency closure."""
from pathlib import Path
import hashlib,json,subprocess
root=Path(__file__).resolve().parents[3]
snapshot=root/'.local-inputs/character-skill-native-v5-host/snapshot'
prior=json.loads((root/'port/level-world/reports/character-skill-native-v5-host-audit.json').read_text())
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for name,digest in prior['private_snapshot'].items():assert sha(snapshot/name)==digest,name
unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
env=['env','LD_LIBRARY_PATH='+unix+'/.local-inputs/character-skill-native-v5-host/snapshot','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
commands=[['g++','-std=c++17','-O2','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','port/level-world/character_combat_result_v6.cpp','port/level-world/tests/character_combat_result_v6_host.cpp','-Iport/game-data','-L.local-inputs/character-skill-native-v5-host/snapshot','-ldh2_game_data','-o','.local-inputs/character_combat_result_v6_host'],env+['.local-inputs/character_combat_result_v6_host','.local-inputs/combat-result-original-characters.bin']]
receipts=[]
for command in commands:
 result=subprocess.run(['wsl.exe','--cd',unix,'--exec',*command],capture_output=True,text=True)
 receipts.append(dict(args=command,exit_code=result.returncode,stdout=result.stdout,stderr=result.stderr))
 assert result.returncode==0,receipts[-1]
for name,digest in prior['private_snapshot'].items():assert sha(snapshot/name)==digest,name
files=['port/level-world/character_combat_result_v6.cpp','port/level-world/character_skill_combat_v6.hpp','port/level-world/tests/character_combat_result_v6_host.cpp','port/level-world/tools/prepare_character_combat_result_v6.py','port/level-world/tools/run_character_combat_result_v6_host.py']
report=dict(validation='PASS',sanitizers=['address','undefined'],leak_detection=True,commands=receipts,results=[json.loads(x)for x in receipts[-1]['stdout'].splitlines()],source_sha256={p:sha(root/p)for p in files},original_corpus_sha256=sha(root/'.local-inputs/combat-result-original-characters.bin'),private_dependency_sha256=prior['private_snapshot'],dependency_current_source_attribution=False,full_target_application=False)
(root/'port/level-world/reports/character-combat-result-v6-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report['results']))
