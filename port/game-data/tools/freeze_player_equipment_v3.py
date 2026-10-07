"""Publish the immutable versioned source equipment receipt once."""
import hashlib,json,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];REF=ROOT/'port/game-data/reference/player-equipment-v3'
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def bind(paths):return {p.relative_to(ROOT).as_posix():sha(p) for p in paths}
def main():
 receipt=REF/'freeze-manifest.json'
 if receipt.exists():raise RuntimeError('Frozen receipt exists; create a successor')
 sources=[ROOT/p for p in ('port/game-data/player_equipment_v3.hpp','port/game-data/player_equipment_v3.cpp')];reports=[ROOT/p for p in ('port/game-data/reports/player-equipment-v3-arm64-differential.json','port/game-data/reports/player-equipment-v3-actual-starters-differential.json','port/game-data/reports/player-equipment-v3-host-audit.json')]
 for p in reports:
  r=json.loads(p.read_text());assert r['validation']=='PASS' and r.get('mismatches',0)==0 and r.get('sanitizer_findings',0)==0
  for name,digest in r['source_sha256'].items():assert sha(ROOT/name)==digest,(p,name)
 for name in ('equipment-choice','equipment-effects','autoequip'):
  shutil.copytree(ROOT/'.local-inputs/player-creation-v2'/name,REF/'original'/name)
 shutil.copytree(ROOT/'.local-inputs/player-equipment-v3/dependencies',REF/'original/dependencies')
 evidence=[p for p in (REF/'original').rglob('*') if p.is_file()]+[REF/'NOTES.md'];tests=[ROOT/p for p in ('port/game-data/tests/player_equipment_v3.cpp','port/game-data/tests/player_equipment_v3_original.py','port/game-data/tests/player_equipment_v3_starters.py','port/game-data/tools/run_player_equipment_v3_host.py','port/game-data/tools/freeze_player_equipment_v3.py')]
 out={'validation':'PASS','version':3,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'source_sha256':bind(sources),'proof_sha256':bind(reports),'source_evidence_sha256':bind(evidence),'test_tool_sha256':bind(tests),'gold_sha256':bind([REF/'fixtures.bin']),'library_sha256':bind([ROOT/'.local-inputs/player-equipment-v3/libplayer_equipment_v3_arm64.so',ROOT/'.local-inputs/player-equipment-v3/host-audit']),'cache_sha256':bind([ROOT/'.local-inputs/items-discovery/loot_table_pyarray.bin']),'sanitizer_findings':0,'scope':'Source equipment caller and borrowed live-slot algorithms. Actual cached starters tested with explicit effect/capability providers. Genuine owned split/merge/delete and property/visual providers remain required; no live/campaign readiness.'};receipt.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps({'receipt':receipt.relative_to(ROOT).as_posix(),'sha256':sha(receipt)}))
if __name__=='__main__':main()
