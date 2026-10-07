"""Freeze only the new authoritative inventory batch, verify predecessor bytes."""
import hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(Path(p).read_bytes()).hexdigest()
def main():
 ref=ROOT/'port/game-data/reference/player-inventory-owned-v4';new=['port/game-data/fresh_inventory_owned_v4.hpp','port/game-data/fresh_inventory_owned_v4.cpp'];proofs=['port/game-data/reports/player-inventory-owned-v4-arm64-differential.json','port/game-data/reports/player-inventory-owned-v4-host-audit.json'];current={p:sha(ROOT/p) for p in new};r=[json.loads((ROOT/p).read_text()) for p in proofs]
 for x in r:
  assert x['validation']=='PASS' and all(x['source_sha256'][p]==h for p,h in current.items())
 assert r[0]['comparisons']==1720 and r[0]['cases']==84 and r[0]['source_effect_requests']==10828 and r[0]['mismatches']==0
 assert r[1]['sanitizer_findings']==0 and r[1]['host_audit']['legacy_loot_regressions']==60 and r[1]['host_audit']['original_owned_steps']==1720
 gold='port/game-data/reference/player-inventory-owned-v4/fixtures.bin';assert r[0]['gold_sha256']==sha(ROOT/gold) and r[1]['gold_sha256'][gold]==sha(ROOT/gold)
 predecessors={}
 for n in ('player-creation-v2','player-equipment-v3'):
  p='port/game-data/reference/'+n+'/freeze-manifest.json';x=json.loads((ROOT/p).read_text());assert all(sha(ROOT/s)==h for s,h in x['source_sha256'].items());predecessors[p]={'manifest_sha256':sha(ROOT/p),'unchanged_source_sha256':x['source_sha256']}
 evidence=[p for p in ref.rglob('*') if p.is_file() and p.name!='freeze-manifest.json']
 tests=['port/game-data/tests/fresh_inventory_owned_v4.cpp','port/game-data/tests/fresh_inventory_owned_v4_original.py','port/game-data/tests/fresh_inventory_owned_v4_arm64_fixture.cpp','port/game-data/tests/fresh_inventory_owned_v4_arm64.py','port/game-data/tools/run_fresh_inventory_owned_v4_host.py','port/game-data/tools/freeze_fresh_inventory_owned_v4.py']
 report={'validation':'PASS','version':4,'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),'source_sha256':current,'source_dependency_sha256':{p:h for p,h in r[0]['source_sha256'].items() if p not in current},'proof_sha256':{p:sha(ROOT/p) for p in proofs},'reference_sha256':{p.relative_to(ROOT).as_posix():sha(p) for p in evidence},'test_and_tool_sha256':{p:sha(ROOT/p) for p in tests},'frozen_predecessors_verified':predecessors,'standalone_arm64':{'.local-inputs/player-inventory-owned-v4/libfresh_inventory_owned_v4_arm64.so':r[0]['library_sha256']},'standalone_sanitized_executable':{'.local-inputs/player-inventory-owned-v4/host-audit':r[1]['executable_sha256']},'scope':'Authoritative owned inventory/equipment graph and source caller bodies. Mandatory item formatting/power and Character effect providers remain explicit; no completed campaign/menu Player or APK claim.'};(ref/'freeze-manifest.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','freeze_sha256':sha(ref/'freeze-manifest.json'),'source_sha256':current,'proof_sha256':report['proof_sha256']}))
if __name__=='__main__':main()
