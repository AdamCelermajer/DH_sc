"""Bind the isolated FX owner batch without rebinding historical dependencies."""
import json,hashlib,shutil
from pathlib import Path
R=Path(__file__).resolve().parents[3];S=R/'.local-inputs/character-fx-owner-v1';F=R/'port/level-world/reference/character-fx-owner-v1';P=R/'port/level-world/reports'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def mapping(paths):return {p.relative_to(R).as_posix():sha(p)for p in paths}
def main():
 captures=[]
 for name in ('lifecycle','callers','factory','texture','texture-matrix','controller','manager-constructor','precache'):
  for path in (S/name).rglob('*'):
   if path.is_file():
    dst=F/'captures'/name/path.relative_to(S/name);dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(path,dst);captures.append(dst)
 modules=['character_fx_kernels_v1','fx_texture_animation_v1','character_fx_state_v1','character_mesh_fx_owner_v1'];production=[R/'port/level-world'/f'{m}.{ext}'for m in modules for ext in ('hpp','cpp')]
 tests=[R/'port/level-world/tests'/p for p in ('character_fx_kernels_v1.cpp','character_fx_kernels_v1_differential.py','character_fx_state_v1.cpp','character_fx_state_v1_differential.py','character_mesh_fx_owner_v1.cpp','character_fx_manager_startup_v1_probe.py')]
 inputs=[R/'port/game-data/reference/effects-tables'/p for p in ('effects_pyarray.bin','effects_pyarraynames.bin','effects_pystructnames.bin','effects_dictionary_pyarraynames.bin','effects_dictionary_pyarray.bin')]+list((S/'cache').glob('*.bdae'))+[R/'port/android-native/app/src/main/assets/models/prince_modular.bdae']+list((S/'cache').glob('animations_*.bin'))+[R/'.local-inputs/items-discovery'/p for p in ('loot_table_pyarray.bin','loot_table_pyarraynames.bin','loot_table_pystructnames.bin')]
 deps=json.loads((S/'host-snapshot/manifest.json').read_text());assert all(sha(S/'host-snapshot'/name)==v['sha256']for name,v in deps.items())
 for proof,name in (('state-host.json','state'),('kernels-host.json','kernels'),('mesh-owner-host.json','mesh_owner')):
  raw=json.loads((S/proof).read_text());assert raw['validation']=='PASS'and raw['sanitizer_findings']==0
 proofs=[P/'character-fx-kernels-v1-arm64-differential.json',P/'character-fx-state-v1-arm64-differential.json']
 for p in proofs:
  proof=json.loads(p.read_text());assert proof['validation']=='PASS'and proof['mismatches']==0;assert all(sha(R/q)==h for q,h in proof['source_sha256'].items())
 report={'validation':'PASS','scope':'Isolated native SAN/LSan replay and actual retained mesh resource/scene composition; original/O2 leaf/projection proof is separate. No APK/GPU/particle/full-factory parity claim.','host_audits':{name:json.loads((S/path).read_text())for name,path in (('state','state-host.json'),('kernels','kernels-host.json'),('mesh_owner','mesh-owner-host.json'))},'sanitizers':['address','undefined','leak'],'source_sha256':mapping(production+tests+[Path(__file__)]),'input_sha256':mapping(inputs),'executable_sha256':{n:sha(S/n)for n in ('state-audit','kernels-audit','mesh-owner-audit')},'historical_dependency_sha256':{n:v['sha256']for n,v in deps.items()},'dependency_snapshot_manifest_sha256':sha(S/'host-snapshot/manifest.json'),'reference_sha256':mapping([F/'kernel-fixtures.bin',F/'state-fixtures.bin',F/'manager-startup-probe.json']),'standalone_build_script_sha256':sha(S/'build-host.sh'),'commands':['wsl.exe --exec bash /mnt/c/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/character-fx-owner-v1/build-host.sh'],'source_services':{'DebugSwitches':'genuine owned native module, real missing-file filesystem open','anchor_and_floor':'explicit source input projection services; actual native Prince Scene in fixture','libm':'original captured imported values for kernel replay; modern host real math for owner lifetime composition'}}
 inventory=F/'starter-class-fx-inventory.txt';content=inventory.read_text();assert all(f'CHAR {i} {name} FX 253 254 255' in content for i,name in ((48,'PlayerKnight'),(49,'PlayerMage'),(50,'PlayerRogue')))
 report['native_cache_inventory']={'path':inventory.relative_to(R).as_posix(),'sha256':sha(inventory),'source_sha256':sha(S/'inventory.cpp'),'executable_sha256':sha(S/'inventory'),'scope':'Actual owned native table traversal; original table readers verified separately. No full original animation-event/three-class scene instruction parity claim.'}
 host=P/'character-mesh-fx-owner-v1-host-audit-v2.json';host.write_text(json.dumps(report,indent=2)+'\n');proofs.append(host)
 refs=[F/'kernel-fixtures.bin',F/'state-fixtures.bin',F/'manager-startup-probe.json',F/'NOTES.md',inventory]+captures
 freeze={'validation':'PASS','production_source_sha256':mapping(production),'source_sha256':mapping(production+tests+[Path(__file__)]),'proof_sha256':mapping(proofs),'reference_sha256':mapping(refs),'input_sha256':mapping(inputs),'historical_dependency_sha256':report['historical_dependency_sha256'],'production_tus':[p.relative_to(R).as_posix()for p in production if p.suffix=='.cpp'],'limitations':['mesh type0 single-step nonredirected domain only','scene/entity/floor providers remain mandatory','particle and other constructor families reject','no full historical factory/GPU parity','no APK/device changes']};out=F/'freeze-manifest-v2.json';out.write_text(json.dumps(freeze,indent=2)+'\n');print(json.dumps({'validation':'PASS','freeze_sha256':sha(out),'host_report_sha256':sha(host)}))
if __name__=='__main__':main()
