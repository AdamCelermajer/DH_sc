"""Publish the new batch receipt once; never replace an existing frozen receipt."""
import hashlib,json,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
REF=ROOT/'port/game-data/reference/player-creation-v2'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def binding(paths):return {str(p.relative_to(ROOT)).replace('\\','/'):sha(p) for p in paths}
def main():
 receipt=REF/'freeze-manifest.json'
 if receipt.exists():raise RuntimeError('Frozen receipt already exists; create a versioned successor')
 source=[ROOT/p for p in ('port/game-data/loot_tables_v2.hpp','port/game-data/loot_tables_v2.cpp','port/game-data/fresh_inventory_v2.hpp','port/game-data/fresh_inventory_v2.cpp','port/level-world/player_initial_grants_v2.hpp','port/level-world/player_initial_grants_v2.cpp')]
 reports=[ROOT/p for p in ('port/game-data/reports/player-creation-v2-loot-arm64-differential.json','port/game-data/reports/player-creation-v2-fresh-arm64-differential.json','port/level-world/reports/player-initial-grants-v2-arm64-differential.json','port/level-world/reports/player-skill-increment-v2-arm64-differential.json','port/game-data/reports/player-creation-v2-host-audit.json')]
 for p in reports:
  r=json.loads(p.read_text());assert r['validation']=='PASS'
  for name,digest in r.get('source_sha256',{}).items():assert sha(ROOT/name)==digest,(p,name)
  assert r.get('mismatches',0)==0 and r.get('sanitizer_findings',0)==0
 captures=ROOT/'.local-inputs/player-creation-v2'
 for name in ('entrypoints','initial-grants','loot','fresh-item-services','autoequip','equipment-choice','equipment-effects','integer-properties'):
  shutil.copytree(captures/name,REF/'original'/name)
 shutil.copy2(captures/'fresh-class-producers.json',REF/'original-class-producers.json')
 producer=json.loads((REF/'original-class-producers.json').read_text());assert producer['validation']=='PASS' and len(producer['rows'])==9
 evidence=list((REF/'original').rglob('*'));evidence=[p for p in evidence if p.is_file()]+[REF/'NOTES.md',REF/'original-class-producers.json']
 tests=[ROOT/p for p in ('port/game-data/tests/loot_tables_v2.cpp','port/game-data/tests/loot_tables_v2_original.py','port/game-data/tests/fresh_inventory_v2.cpp','port/game-data/tests/fresh_inventory_v2_original.py','port/game-data/tests/fresh_inventory_v2_arm64_fixture.cpp','port/game-data/tests/fresh_inventory_v2_arm64.py','port/level-world/tests/player_initial_grants_v2.cpp','port/level-world/tests/player_initial_grants_v2_original.py','port/level-world/tests/player_skill_increment_v2_original.py','port/game-data/tools/run_player_creation_v2_host.py','port/game-data/tools/freeze_player_creation_v2.py','.local-inputs/player-creation-v2/fresh_class_producers.py')]
 gold=[REF/'fixtures.bin',REF/'fresh-fixtures.bin',ROOT/'port/level-world/reference/player-initial-grants-v2/fixtures.bin',ROOT/'port/level-world/reference/player-initial-grants-v2/skill-fixtures.bin',REF/'fresh-original-gold.json']
 libraries=[captures/n for n in ('libloot_tables_v2_arm64.so','libfresh_inventory_v2_arm64.so','libplayer_initial_grants_v2_arm64.so','loot-host-audit','fresh-host-audit','grants-host-audit')]
 host=json.loads(reports[-1].read_text())
 out={'validation':'PASS','version':2,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'source_sha256':binding(source),'proof_sha256':binding(reports),'gold_sha256':binding(gold),'test_and_tool_sha256':binding(tests),'source_evidence_sha256':binding(evidence),'library_sha256':binding(libraries),'cache_input_sha256':host['cache_input_sha256'],'sanitizer_findings':0,'comparisons':{'loot_reader_rng':979,'owned_starting_inventory':60,'initial_grants':768,'skill_increment':768,'class_cache_producers':9},'scope':'Original source caller/owned fixed-loot proof with explicit native effect providers. Campaign/profile creation, complete auto-equip/item effects and live skill VM grants remain required. No packaged/live player readiness claim.'}
 receipt.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps({'receipt':str(receipt.relative_to(ROOT)),'sha256':sha(receipt)}))
if __name__=='__main__':main()
