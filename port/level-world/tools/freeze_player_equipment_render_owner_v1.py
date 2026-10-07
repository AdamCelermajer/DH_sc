"""Freeze the passing isolated equipment owner without mutating old receipts."""
import hashlib,json,shutil
from pathlib import Path
R=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def rel(p):return p.relative_to(R).as_posix()
def main():
 ref=R/'port/level-world/reference/player-equipment-render-owner-v1'
 scratch=R/'.local-inputs/player-equipment-render-owner-v1'
 for group in ('capture','stance','requirements','dual','combat'):
  src=scratch/group
  for p in src.rglob('*'):
   if p.is_file():
    target=ref/'original-capture'/group/p.relative_to(src);target.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(p,target)
 menu=R/'port/engine-ui/reference/character-menu-native-v1/original-functions.asm'
 if menu.exists():
  # Preserve the exact captured NativeSwapEquipment evidence owned by UI work.
  target=ref/'original-capture/menu-original-functions.asm';shutil.copyfile(menu,target)
 production=['port/level-world/player_equipment_render_owner_v1.hpp','port/level-world/player_equipment_render_owner_v1.cpp','port/level-world/player_equipment_queries_v1.hpp','port/level-world/player_equipment_queries_v1.cpp']
 tests=['port/level-world/tests/player_equipment_render_owner_v1.cpp','port/level-world/tests/player_equipment_queries_v1_differential.py','port/level-world/tools/audit_player_equipment_render_owner_v1_host.py',rel(Path(__file__))]
 proofs=['port/level-world/reports/player-equipment-render-owner-v1-host-audit-v1.json','port/level-world/reports/player-equipment-queries-v1-arm64-differential.json','port/engine-skinning/reference/visual-skin-owner-v6/freeze-manifest.json','port/engine-skinning/reports/visual-skin-owner-v6-host-audit-v1.json']
 for name in proofs:assert json.loads((R/name).read_text())['validation']=='PASS',name
 host=json.loads((R/proofs[0]).read_text());oracle=json.loads((R/proofs[1]).read_text())
 assert host['host_audit']['original_query_gold_cases']==2000
 assert host['sanitizers']['findings']==0 and oracle['original_predicate_comparisons']==8000
 for receipt in (host,oracle):
  for name,wanted in receipt['source_sha256'].items():assert sha(R/name)==wanted,name
 for name,wanted in host['dependency_sha256'].items():assert sha(R/name)==wanted,name
 for name,wanted in host['input_sha256'].items():assert sha(R/name)==wanted,name
 reference={rel(p):sha(p)for p in sorted(ref.rglob('*'))if p.is_file() and p.name!='freeze-manifest.json'}
 manifest={'validation':'PASS','production_source_sha256':{p:sha(R/p)for p in production},'source_sha256':{p:sha(R/p)for p in production+tests},'proof_sha256':{p:sha(R/p)for p in proofs},'reference_sha256':reference,'input_sha256':host['input_sha256'],'dependency_sha256':host['dependency_sha256'],'standalone_binary_sha256':{'.local-inputs/player-equipment-render-owner-v1/owner-audit':sha(scratch/'owner-audit'),'.local-inputs/player-equipment-render-owner-v1/libqueries-arm64.so':sha(scratch/'libqueries-arm64.so')},'host_audit':host['host_audit'],'original_sha256':oracle['original_sha256'],'scope':'Native same inventory/property/live-scene composition. World/campaign selection and RNG clock remain caller projections; no packaged/GPU/full-campaign claim. V4/V5/V6 and old receipts untouched.'}
 p=ref/'freeze-manifest.json';assert not p.exists(),'Existing freeze is immutable; use a new version for a changed batch'
 p.write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps({'freeze':rel(p),'sha256':sha(p),'host_report_sha256':sha(R/proofs[0]),'production_source_sha256':manifest['production_source_sha256'],'host_audit':host['host_audit']}))
if __name__=='__main__':main()
