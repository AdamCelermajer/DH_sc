from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];unix='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
sources=['port/level-world/character_revive_owner_v1.cpp','port/level-world/tests/character_revive_owner_v1.cpp']
gold='port/level-world/reference/character-revive-owner-v1/original-fixtures.bin'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();receipts=[]
for opt in ['-O1','-O2']:
 exe='.local-inputs/character_revive_owner_v1_host_'+opt[1:]
 for args in [['g++','-std=c++17',opt,'-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,'-o',exe],['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,gold]]:
  q=subprocess.run(['wsl.exe','-d','Ubuntu','--cd',unix,'--exec',*args],capture_output=True,text=True,timeout=60);receipts.append({'args':args,'exit_code':q.returncode,'stdout':q.stdout,'stderr':q.stderr});print(q.stdout,q.stderr,flush=True);assert q.returncode==0,receipts[-1]
report={'validation':'PASS','commands':receipts,'source_sha256':{p:sha(root/p) for p in sources},'gold_sha256':sha(root/gold),'original_receipt':'reference/character-revive-owner-v1/original.json','scope':'Native replay of 288 whole original ARM Revive cases with exact store and ordered helper-entry parity, plus failure at every mandatory source request. Helper implementations explicit observer fixtures; not full visual/physical/skills/HP backend proof.'}
(root/'port/level-world/reports/character-revive-owner-v1-host.json').write_text(json.dumps(report,indent=2)+'\n')
