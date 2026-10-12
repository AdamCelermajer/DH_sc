"""Bind complete power-generation/valuation subsystem and its exact evidence."""
import hashlib,json
from pathlib import Path
R=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def binding(names):return {n:sha(R/n)for n in names}
def main():
 ref='port/game-data/reference/loot-power-creation-v7/';output=R/ref/'freeze-manifest.json';assert not output.exists()
 prod=['port/game-data/'+n+e for n in('loot_power_resources_v7','loot_power_creation_v7')for e in('.hpp','.cpp')]
 tests=['port/game-data/tests/loot_power_creation_v7.cpp','port/game-data/tests/loot_power_creation_v7_fixture.cpp','port/game-data/tests/loot_power_creation_v7_differential.py']
 tools=['port/game-data/tools/'+n for n in('build_loot_power_v7_oracle.ps1','audit_loot_power_v7_host.py','prepare_loot_power_v7_inputs.py','freeze_loot_power_v7.py')]
 proofs=['port/game-data/reports/loot-power-creation-v7-arm64-differential.json','port/game-data/reports/loot-power-creation-v7-host-audit.json']
 old=R/'port/game-data/reference/player-item-effects-v5/freeze-manifest.json';frozen=json.loads(old.read_text());assert frozen['validation']=='PASS'
 for k in('production_source_sha256','source_sha256','proof_sha256','reference_sha256'):
  for n,d in frozen[k].items():assert sha(R/n)==d,n
 report=json.loads((R/proofs[0]).read_text());host=json.loads((R/proofs[1]).read_text());assert report['validation']==host['validation']=='PASS';assert host['original_differential_sha256']==sha(R/proofs[0]);assert host['sanitizers']['findings']==0
 for n,d in report['source_sha256'].items():assert sha(R/n)==d,n
 build=R/'.local-inputs/player-loot-v7/arm64-build.json';b=json.loads(build.read_text());assert b['library_sha256']==report['library_sha256']
 for n,d in b['source_sha256'].items():assert sha(Path(n))==d,n
 references=[ref+n for n in('cache-inputs.json','original-functions.json','fixtures.bin')]
 dependencies=['port/game-data/reference/player-item-effects-v5/freeze-manifest.json','port/game-data/loot_tables_v2.hpp','port/game-data/loot_tables_v2.cpp','port/game-data/items.hpp','port/game-data/items.cpp','port/game-data/tests/item_power_instance_v5_differential.py','port/game-data/tests/item_presentation_v5_original.py','port/game-data/tests/item_presentation_v5_differential.py','port/game-data/tests/items_differential.py','port/game-data/tests/player_savegame_v1_original.py','port/game-data/tests/fresh_inventory_owned_v4_arm64.py','port/game-data/tests/fresh_inventory_v2_arm64.py','port/level-world/tests/navigation_differential.py']
 manifest=dict(validation='PASS',production_source_sha256=binding(prod),source_sha256=binding(tests+tools),proof_sha256=binding(proofs),reference_sha256=binding(references),borrowed_dependency_sha256=binding(dependencies),original_cases=report['comparisons'],original_counts=report['counts'],native_host_result=host['result'],scope=dict(actual_power_lists=121,actual_quantity_lists=39,pinned_same_V5_power_authority=True,ordered_conflicts_retries_fallback=True,real_V5_Power_append_and_difficulty_variants=True,real_V4_inventory_storage_and_localized_descriptions=True,full_AddLoot_random_subloot_pickup_merchant=False,live_Android_connected=False),isolated_build_capture_sha256=sha(build),isolated_ARM64_sha256=report['library_sha256'])
 output.write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps(dict(validation='PASS',path=str(output),sha256=sha(output),original_cases=report['comparisons'])))
if __name__=='__main__':main()
