"""Bind recovered physics/visual source, original audits and packaged artifacts."""
import argparse,hashlib,json,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import REPO,ROOT,digest,read,log
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True)
 p.add_argument('--build-prefix',default='physics-backend');p.add_argument('--world-output',type=Path);a=p.parse_args()
 local=REPO/'.local-inputs';reports=REPO/'port/level-world/reports';backend=REPO/'port/physics-backend'
 previous=read(reports/'physical-movement-source-validation.json');engine=digest(local/'libDungeonHunter2.so')
 provenance=read(backend/'reference/provenance.json');assert provenance['original_engine_sha256']==engine and provenance['original_version']==[2,0,1]
 assert provenance['vendor_files']==65 and len(provenance['modified_files'])==2
 for row in provenance['files']:assert digest(backend/'box2d-2.0.1'/row['file'])==row['native_sha256']
 assert len(list((backend/'box2d-2.0.1/Source').rglob('*.cpp')))==31
 full=read(backend/'reports/full-world-arm64-differential.json');full_host=read(backend/'reports/full-world-host-audit.json')
 full_gold=digest(local/'physics-backend-discovery/full-world-reference.bin')
 assert full['scenes']==full_host['scenes']==40 and full['mismatches']==full_host['mismatches']==0
 assert full['original_physics_functions_executed']==104 and full['reference_sha256']==full_host['reference_sha256']==full_gold
 assert full_host['world_steps']==157 and full_host['contact_listener_events']==68 and full_host['allocation_accounting_balanced']
 native=read(reports/'native-body-arm64-differential.json');native_host=read(reports/'physics-backend-native-body-replay-host-audit.json');real=read(reports/'physics-backend-native-body-host-audit.json')
 assert native['comparisons']==native_host['original_derived_replay']==1776 and native['world_steps']==native_host['genuine_world_steps']==576
 assert native['reference_sha256']==native_host['reference_sha256']==digest(local/'native-body-reference.bin')
 assert native['mismatches']==native_host['mismatches']==real['mismatches']==0 and real['genuine_world_steps']==163 and real['atomic_rejection_checks']==5
 specs={'physical-world':(7000,local/'physical-world-reference.bin'),'physical-lifecycle':(4846,local/'physical-lifecycle-reference.bin'),'visual-motion':(1796,REPO/'port/level-world/reference/visual-motion/kernel-fixtures.bin')}
 required={'dh2_physical_world_should_collide','dh2_physical_world_contact','dh2_physical_world_step_arguments','dh2_physical_pin','dh2_physical_unpin','dh2_native_body_stop','dh2_native_body_subobject_service','dh2_visual_displace','dh2_visual_rotation','_ZN7b2World4StepEfi'}
 apk_paths={'packaged':ROOT/'app/build/outputs/apk/debug/app-debug.apk','studio':a.studio/'app/build/outputs/apk/debug/app-debug.apk'}
 archives={tag:zipfile.ZipFile(path) for tag,path in apk_paths.items()};libraries={tag:inspect(path) for tag,path in apk_paths.items()};verified={};assets={}
 for tag,z in archives.items():
  assert len(libraries[tag])==14 and set(z.namelist())==set(archives['packaged'].namelist())
  assert not any('DungeonHunter2.so' in n or 'armeabi' in n for n in z.namelist())
  raw=z.read('lib/arm64-v8a/libdh2_level_world.so');world_sha=hashlib.sha256(raw).hexdigest();math_sha=hashlib.sha256(z.read('lib/arm64-v8a/libdh2_scene_materials.so')).hexdigest()
  assert required<=exports(raw) and world_sha==digest(local/f'physics-backend-{tag}-world-arm64.so')
  verified[tag]={}
  for module,(count,gold) in specs.items():
   r=read(reports/f'physics-backend-{tag}-{module}-arm64-differential.json');h=read(reports/f'physics-backend-{module}-host-audit.json')
   visual=module=='visual-motion';assert r['cases' if visual else 'comparisons']==h['comparisons']==count and r['mismatches']==h['mismatches']==0
   assert r['native_sha256' if visual else 'arm64_library_sha256']==world_sha and r['original_sha256']==engine
   assert r['corpus_sha256' if visual else 'reference_sha256']==h['reference_sha256']==digest(gold)==digest(local/f'physics-backend-{tag}-{module}-reference.bin')
   if visual:assert r['counts']['asset_samples']==3567 and r['math_dependency_sha256']==math_sha
   assert h['sanitizers']==['address','undefined'];verified[tag][module]=r
  # The prior five physical kernels and two live heading/controller contracts
  # were checked against this exact library after adding the native backend.
  for module in ('physical-controls','body-transform','subobjects-update','controller-physical','character-body-config'):
   r=read(reports/f'{module}-{tag}-arm64-differential.json');assert r['arm64_library_sha256']==world_sha and r['mismatches']==0
  for module in ('controller','heading'):
   r=read(reports/f'physical-movement-{module}-{tag}-regression.json');assert r['arm64_library_sha256']==world_sha and r['mismatches']==0
 for name in archives['packaged'].namelist():
  if name.startswith('assets/') and not name.endswith('/'):
   raw=archives['packaged'].read(name);key=name[7:];assert raw==archives['studio'].read(name)==(ROOT/'app/src/main/assets'/key).read_bytes()
   assets[key]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
 assert assets==previous['assets_verified']
 world=read(a.world_output or local/'world-tests-physics-backend/world-smoke.json');assert world['apk_sha256']==world['installed_apk_sha256']==digest(apk_paths['packaged']) and len(world['cases'])==10
 renderer=ROOT/'app/src/main/cpp/model_renderer.cpp';studio_renderer=a.studio/'app/src/main/cpp/model_renderer.cpp';assert renderer.read_bytes()==studio_renderer.read_bytes() and 'move_x*420*dt' in renderer.read_text()
 for tag in ('repo','studio'):assert 'BUILD SUCCESSFUL' in log(local/f'{a.build_prefix}-{tag}-build.log')
 assert 'Verification successful' in log(local/f'{a.build_prefix}-zipalign.log')
 assert 'Verification successful' in log(local/f'{a.build_prefix}-studio-zipalign.log')
 sources={}
 for stem in ('physical_world','physical_lifecycle','native_body','visual_motion'):
  for ext in ('hpp','cpp'):
   file=REPO/f'port/level-world/{stem}.{ext}';sources[file.relative_to(REPO).as_posix()]=digest(file)
 report={'apk_sha256':{tag:digest(path) for tag,path in apk_paths.items()},'libraries':libraries,'source_sha256':sources,'backend_provenance_sha256':digest(backend/'reference/provenance.json'),'full_backend':full,'full_backend_host':full_host,'native_body':native,'native_body_host':native_host,'genuine_body_fixture':real,'packaged_differentials':verified,'assets':assets,'original_input_count':previous['original_input_count'],'world_cases':10,'matching_open_source_physics_recovered':True,'native_shape_mass_broadphase_contacts_solver_toi_step':True,'native_character_body_factory':True,'gameplay_contact_listener_core':True,'visual_root_motion_and_rotation_kernels':True,'physical_movement_used_by_live_actor':False,'live_actor_movement_policy':'Existing development adapter; original movement frame scheduling and body/root-motion/controller integration remain pending.','physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_game_playable':False,'checkpoint_apk_created':False,'goal_status':'active'}
 output=reports/'physics-backend-source-validation.json';output.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({'validation':'PASS','new_packaged_kernel_cases_each':13642,'real_asset_samples_each':3567,'full_native_world_scenes':40,'native_body_comparisons':1776,'world_cases':10,'full_game_playable':False}))
if __name__=='__main__':main()
