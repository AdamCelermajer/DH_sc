"""Isolated sanitized ItemTable replay with compiler and exact cache binding."""
import argparse,hashlib,json,shlex,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
WSL_REPO='/mnt/c/Users/adamc/Desktop/workspace/DH_sc'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(*args):
 r=subprocess.run(['wsl.exe','--cd',WSL_REPO,*args],capture_output=True,text=True,timeout=120)
 assert r.returncode==0,(r.returncode,r.stdout,r.stderr)
 assert not r.stderr.strip(),r.stderr
 return r.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--executable',default='/home/adampalace/dh2-world-build/items_isolated_audit');p.add_argument('--rebuild',action='store_true');p.add_argument('--output',type=Path,default=ROOT/'reports/items-host-audit.json');p.add_argument('--cache',type=Path,default=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'));a=p.parse_args()
 ref=ROOT/'reference/items';evidence_path=ref/'original-arm64.json';evidence=json.loads(evidence_path.read_text());assert evidence['validation']=='PASS' and evidence['mismatches']==0
 assert sha(ref/'record-fixtures.bin')==evidence['corpus_sha256'] and sha(ref/'original-functions.json')==evidence['manifest_sha256']
 for name,value in evidence['source_sha256'].items():assert sha(REPO/name)==value,name
 sources=['port/game-data/items.cpp','port/game-data/tests/items.cpp','port/level-world/character_combat_queries.cpp']
 tracked=sources+['port/game-data/items.hpp','port/game-data/data.hpp','port/game-data/animation_tables.hpp','port/level-world/character_combat_queries.hpp','port/game-data/tests/items_differential.py','port/game-data/tests/items_host.py']
 source_hashes={n:sha(REPO/n) for n in tracked};snapshot=REPO/'.local-inputs/items-discovery/host-build-inputs.json'
 command=['g++','-std=c++17','-Wall','-Wextra','-Werror','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-g',*sources,'-o',a.executable]
 if a.rebuild:run(*command)
 inputs={'source_sha256':source_hashes,'executable_sha256':run('sha256sum',a.executable).split()[0],'compiler':run('g++','--version').splitlines()[0],'command':shlex.join(command)}
 assert all(sha(REPO/n)==v for n,v in source_hashes.items()),'Build inputs changed'
 if a.rebuild:snapshot.write_text(json.dumps(inputs,indent=2)+'\n')
 else:assert snapshot.exists() and json.loads(snapshot.read_text())==inputs,'Rebuild with stable inputs first'
 assert sha(a.cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
 scratch=REPO/'.local-inputs/items-discovery';asset_inputs=[]
 with zipfile.ZipFile(a.cache) as archive:
  for name in ('loot_table_pyarray.bin','loot_table_pyarraynames.bin','loot_table_pystructnames.bin'):
   entries=[entry for entry in archive.namelist() if entry.endswith('/'+name)];assert len(entries)==1,(name,entries)
   raw=archive.read(entries[0]);path=scratch/name;assert path.read_bytes()==raw
   asset_inputs.append({'cache_entry':entries[0],'bytes':len(raw),'sha256':sha(path)})
 host=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.executable,'port/game-data/reference/items/record-fixtures.bin','.local-inputs/items-discovery'))
 assert host['validation']=='PASS' and host['mismatches']==0 and host['sanitizer_findings']==0
 assert host['record_comparisons']==evidence['comparisons'] and host['actual_cache_rows']==host['full_loader_rows']==evidence['actual_cache_rows']
 assert host['identifier_queries']==evidence['original_identifier_queries'] and host['loaded_item_range_queries']==evidence['original_loaded_item_range_queries'] and host['native_guards']==168
 assert all(sha(REPO/n)==v for n,v in source_hashes.items()),'Replay inputs changed'
 report={'validation':'PASS','host_audit':host,'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,**inputs,'build_inputs_sha256':sha(snapshot),'cache_sha256':sha(a.cache),'cache_inputs':asset_inputs,'original_instruction_evidence':{'report_sha256':sha(evidence_path),**evidence},'scope':'Actual cache ItemTable with full native leading-section loader, owned strings, original identifier and genuinely decoded-item combat-query gold. Preceding three table contents are only traversed, trailing loot tables/subclasses remain uninterpreted. Equipment ownership and mutation remain external.'}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS',**host}))
if __name__=='__main__':main()
