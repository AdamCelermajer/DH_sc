"""Verify and freeze V6 only; does not rebuild or edit shared build inputs."""
import hashlib,json,shutil
from pathlib import Path
R=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 base=R/'port/engine-skinning';ref=base/'reference/visual-skin-owner-v6';scratch=R/'.local-inputs/visual-skin-owner-v6'
 for name in('factories','construction','deeper','wrappers'):
  shutil.copytree(scratch/name,ref/name,dirs_exist_ok=True)
 for src,dst in(('catalog.json','catalog.json'),('weapons/manifest.json','weapon-manifest.json')):shutil.copyfile(scratch/src,ref/dst)
 host=base/'reports/visual-skin-owner-v6-host-audit-v1.json';arm=base/'reports/visual-skin-selection-v6-arm64-differential.json';original=ref/'inventory-original-gold.json'
 reports=[host,arm,original]
 for p in reports:
  obj=json.loads(p.read_text());assert obj['validation']=='PASS'
  for k,v in obj.get('source_sha256',{}).items():assert sha(R/k)==v,(k,'source changed')
 audit=json.loads(host.read_text());assert audit['sanitizers']['findings']==0
 for k,v in audit['executable_sha256'].items():assert sha(R/k)==v
 for k,v in audit['borrowed_dependency_sha256'].items():assert sha(R/'.local-inputs/player-item-effects-v5/host-snapshot'/k)==v
 for k,v in audit['input_sha256'].items():assert sha(R/k)==v
 for k,v in audit['frozen_source_sha256'].items():assert sha(R/k)==v
 for k,v in audit['proof_sha256'].items():assert sha(R/k)==v
 assert sha(ref/'selection-fixtures.json')==json.loads(arm.read_text())['gold_sha256']
 assert sha(ref/'inventory-fixtures.bin')==json.loads(original.read_text())['gold_sha256']
 assert sha(R/'.local-inputs/libDungeonHunter2.so')==json.loads(arm.read_text())['original_sha256']
 assert sha(scratch/'libselection-arm64.so')==json.loads(arm.read_text())['library_sha256']
 weapon_manifest=json.loads((ref/'weapon-manifest.json').read_text());assert len(weapon_manifest)==781
 for item in weapon_manifest:assert sha(scratch/'weapons'/item['name'])==item['sha256']
 production=[base/(name+suffix)for name in('visual_skin_selection_v6','visual_skin_owner_v6')for suffix in('.hpp','.cpp')]
 tests=list((base/'tests').glob('*_v6.*'));tools=list((base/'tools').glob('*_v6*.py'))
 evidence=[p for p in ref.rglob('*')if p.is_file()and p.name!='freeze-manifest.json']
 def mapping(paths):return {p.relative_to(R).as_posix():sha(p)for p in sorted(paths)}
 dependencies=['port/game-data/tests/'+p+'.py'for p in('fresh_inventory_owned_v4_original','fresh_inventory_v2_original','item_inventory_v1_original','player_savegame_v1_original','items_differential','fresh_inventory_owned_v4_arm64','fresh_inventory_v2_arm64')]+['port/engine-resources/tests/cpu.py','port/level-world/tests/navigation_differential.py','port/engine-skinning/tests/visual_skin_selection_v6_differential.py']
 prior=json.loads((R/'port/game-data/reports/player-item-effects-v5-host-audit-v3.json').read_text())
 localization=prior['host_audits']['cache']['actual_localization_files']
 for p in localization:assert sha(R/p)==prior['input_sha256'][p]
 freeze=dict(validation='PASS',production_source_sha256=mapping(production),source_sha256=mapping([*production,*tests,*tools]),proof_sha256=mapping(reports),reference_sha256=mapping(evidence),original_sha256=sha(R/'.local-inputs/libDungeonHunter2.so'),original_probe_dependency_sha256={p:sha(R/p)for p in dependencies},additional_localization_input_sha256={p:sha(R/p)for p in localization},borrowed_dependency_sha256=audit['borrowed_dependency_sha256'],scope=audit['scope'])
 p=ref/'freeze-manifest.json';p.write_text(json.dumps(freeze,indent=2)+'\n');print(json.dumps(dict(validation='PASS',freeze=p.relative_to(R).as_posix(),freeze_sha256=sha(p),production_source_sha256=freeze['production_source_sha256'],proof_sha256=freeze['proof_sha256'])))
if __name__=='__main__':main()
