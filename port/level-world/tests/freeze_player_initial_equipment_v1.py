"""Emit the scoped completed equipment proof whitelist without changing source."""
import hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
REF=ROOT/'port/level-world/reference/player-initial-equipment-v1'
PATHS=[
 'port/level-world/player_initial_equipment_v1.hpp',
 'port/level-world/player_initial_equipment_v1.cpp',
 'port/level-world/tests/player_initial_equipment_v1.cpp',
 'port/level-world/tests/player_initial_equipment_v1_original.py',
 'port/level-world/tests/run_player_initial_equipment_v1_host.py',
 'port/level-world/tests/freeze_player_initial_equipment_v1.py',
 'port/level-world/reference/player-initial-equipment-v1/original-functions.json',
 'port/level-world/reference/player-initial-equipment-v1/original-capture.json',
 'port/level-world/reference/player-initial-equipment-v1/original-cases.bin',
 'port/level-world/reference/player-initial-equipment-v1/NOTES.md',
 'port/level-world/reports/player-initial-equipment-v1-selected-host.json']
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 report=json.loads((ROOT/PATHS[-1]).read_text());assert report['validation']=='PASS' and report['source_before_after_equal']
 for path in PATHS[:2]:assert report['source_sha256'][path]==sha(ROOT/path)
 for name in ['original-functions.json','original-capture.json','original-cases.bin']:
  assert sha(REF/name)==sha(Path('C:/tmp/dh2-player-initial-equipment-selected-host/original')/name)
 manifest={'validation':'PASS','upstream_author':'Adam Celermajer','upstream_commit':'791e961b12233100b303038c961666834f4beb9d','module':'Borrowed exact Character::_InitEquipment caller','files_sha256':{path:sha(ROOT/path) for path in PATHS},'whitelist':[*PATHS,'port/level-world/reference/player-initial-equipment-v1/freeze-manifest.json'],'host_output':'C:/tmp/dh2-player-initial-equipment-selected-host','host_report':report['host_report'],'original_arm_cases':len(report['original_capture']['cases']),'original_distinct_words':report['original_capture']['distinct_pinned_words'],'selected_project_inputs':len(report['source_sha256']),'reached_cache_inputs':len(report['cache_inputs_sha256']),'selected_cmake_sha256':report['source_sha256']['port/level-world/CMakeLists.txt'],'source_before_after_equal':True,'scope':report['scope']}
 (REF/'freeze-manifest.json').write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps({'whitelist_paths':len(manifest['whitelist']),'freeze_manifest_sha256':sha(REF/'freeze-manifest.json'),'production_sha256':{p:sha(ROOT/p) for p in PATHS[:2]},'report_sha256':sha(ROOT/PATHS[-1])}))
if __name__=='__main__':main()
