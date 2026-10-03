"""Bind physical source milestones to real packaged binaries and regression evidence.

This is source reconstruction evidence. It deliberately does not certify the
physics world, shape/broadphase implementation or live controller integration.
"""
import argparse,hashlib,json,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture

SPECS={
 'physical-controls':(5446,REPO/'.local-inputs/physical-controls-reference.bin'),
 'body-transform':(4320,REPO/'.local-inputs/body-transform-reference.bin'),
 'subobjects-update':(1479,REPO/'port/level-world/reference/subobjects-update/service-fixtures.bin'),
 'controller-physical':(1186,REPO/'.local-inputs/controller-physical-reference.bin'),
 'character-body-config':(3864,REPO/'.local-inputs/character-body-config-reference.bin'),
}
REQUIRED={'dh2_physical_set_linear','dh2_physical_add_linear','dh2_physical_set_angular',
 'dh2_physical_query','dh2_physical_request_position','dh2_physical_stop_begin',
 'dh2_physical_stop_finish','dh2_physical_wake','dh2_body_set_transform',
 'dh2_subobjects_update','dh2_nav_update_path_physical','dh2_nav_update_path','dh2_character_body_config'}

def check(value,expected):
 for name,wanted in expected.items():assert value.get(name)==wanted,(name,value.get(name),wanted)

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);a=p.parse_args()
 local=REPO/'.local-inputs';rd=REPO/'port/level-world/reports'
 previous=read(rd/'navigation-controller-source-validation.json')
 engine=local/'libDungeonHunter2.so';engine_sha=digest(engine)
 apks={'packaged':ROOT/'app/build/outputs/apk/debug/app-debug.apk',
       'studio':a.studio/'app/build/outputs/apk/debug/app-debug.apk'}
 archives={name:zipfile.ZipFile(path) for name,path in apks.items()}
 libraries={name:inspect(path) for name,path in apks.items()};reports={};host={}
 for name,z in archives.items():
  assert len(libraries[name])==14
  assert set(z.namelist())==set(archives['packaged'].namelist())
  assert not any('DungeonHunter2.so' in path or 'armeabi' in path for path in z.namelist())
  for row in libraries[name]:
   if row['path'].endswith('/libdh2_level_world.so'):assert REQUIRED<=exports(z.read(row['path']))
  packaged_sha=hashlib.sha256(z.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()
  assert packaged_sha==digest(local/f'physical-movement-{name}-world-arm64.so')
  reports[name]={}
  for module,(cases,gold) in SPECS.items():
   reference=digest(gold);report=read(rd/f'{module}-{name}-arm64-differential.json')
   check(report,{'comparisons':cases,'mismatches':0,'reference_sha256':reference,
    'original_sha256':engine_sha,'arm64_library_sha256':packaged_sha})
   assert digest(local/f'{module}-{name}-reference.bin')==reference
   standalone=read(rd/f'{module}-arm64-differential.json')
   check(standalone,{'comparisons':cases,'mismatches':0,'reference_sha256':reference,'original_sha256':engine_sha})
   assert standalone['scope']==report['scope']
   if module=='subobjects-update':
    check(report['crypt_floor_registry_integration'],{'coordinator_cases':128,'validation_calls':128,
     'floor_queries':240,'registry_relocations':112,'mismatches':0,'floor_library_sha256':packaged_sha,
     'validation_output_sha256':'d02e9a34ba7fe982b9305c6e07b382d4423d300f85abe592c539f971d2b35462'})
   reports[name][module]=report
  for module,cases in [('controller',1132),('heading',4476)]:
   regression=read(rd/f'physical-movement-{module}-{name}-regression.json')
   check(regression,{'comparisons':cases,'mismatches':0,'original_sha256':engine_sha,'arm64_library_sha256':packaged_sha})
   assert regression['reference_sha256']==digest(local/f'navigation-{module}-original.bin')
   reports[name][module]=regression
  for relative,expected in previous['assets_verified'].items():
   raw=z.read('assets/'+relative)
   assert hashlib.sha256(raw).hexdigest()==expected['sha256']
   assert raw==(ROOT/'app/src/main/assets'/relative).read_bytes()
  assert len([path for path in z.namelist() if path.startswith('assets/') and not path.endswith('/')])==126
  old=previous['libraries' if name=='packaged' else 'studio_libraries']
  for row in old:
   if Path(row['path']).name not in {'libdh2_level_world.so','libdh2_native.so'}:
    assert hashlib.sha256(z.read(row['path'])).hexdigest()==row['sha256']
 for module,(cases,gold) in SPECS.items():
  audit=read(rd/f'physical-movement-{module}-host-audit.json')
  check(audit,{'comparisons':cases,'mismatches':0,'reference_sha256':digest(gold),'sanitizers':['address','undefined']})
  host[module]=audit
 for module,cases in [('controller',1132),('heading',4476)]:
  audit=read(rd/f'physical-movement-{module}-host-regression.json')
  check(audit,{'comparisons':cases,'mismatches':0,'reference_sha256':digest(local/f'navigation-{module}-original.bin'),'sanitizers':['address','undefined']})
  host[module]=audit
 for module in ('physical-controls','body-transform','subobjects-update','character-body-config'):
  verify_capture(REPO/f'port/level-world/reference/{module}/original-functions.json',engine)
 for name in ('repo','studio'):assert 'BUILD SUCCESSFUL' in log(local/f'physical-movement-{name}-build.log')
 source={}
 allowed={'level-world/CMakeLists.txt','level-world/README.md','android-native/README.md','android-native/ROADMAP.md'}
 for name,expected in previous['module_source_sha256'].items():
  actual=digest(REPO/'port'/name)
  if name not in allowed:assert actual==expected,name
  source[name]=actual
 for name,expected in previous['studio_source_sha256'].items():
  assert digest(ROOT/'app/src/main'/name)==digest(a.studio/'app/src/main'/name)==expected
 for stem in ('physical_controls','body_transform','subobjects_update','controller_physical','character_body_config'):
  for suffix in ('.hpp','.cpp'):
   path=REPO/f'port/level-world/{stem}{suffix}';source[path.relative_to(REPO/'port').as_posix()]=digest(path)
  for suffix in ('.cpp','_differential.py'):
   path=REPO/f'port/level-world/tests/{stem}{suffix}';source[path.relative_to(REPO/'port').as_posix()]=digest(path)
  path=REPO/f'port/level-world/tools/build_{stem}_oracle.ps1';source[path.relative_to(REPO/'port').as_posix()]=digest(path)
 for module in ('physical-controls','body-transform','subobjects-update','character-body-config','controller-physical'):
  for path in (REPO/f'port/level-world/reference/{module}').rglob('*'):
   if path.is_file():source[path.relative_to(REPO/'port').as_posix()]=digest(path)
 for name in ('validate_physical_movement_source.py','run_physical_movement_checks.py'):
  path=Path(__file__).with_name(name);source[path.relative_to(REPO/'port').as_posix()]=digest(path)
 world=read(local/'world-tests-physical-movement/world-smoke.json')
 check(world,{'apk_sha256':digest(apks['packaged']),'installed_apk_sha256':digest(apks['packaged']),
  'native_heading_used_by_player_movement':True,'original_movement_controller_reconstructed':False,
  'physical_arm64_tested':False,'full_game_playable':False,'frozen_idle_stable':True,
  'rotation_position_preserved':True,'pause_cancels_movement':True})
 assert len(world['cases'])==10
 zipalign=Path.home()/'AppData/Local/Android/Sdk/build-tools/36.0.0/zipalign.exe'
 with (local/'physical-movement-zipalign.log').open('w') as output:
  for apk in apks.values():subprocess.run([str(zipalign),'-c','-P','16','-v','4',str(apk)],stdout=output,stderr=subprocess.STDOUT,check=True)
 assert log(local/'physical-movement-zipalign.log').count('Verification successful')==2
 result={'apk_sha256':{name:digest(path) for name,path in apks.items()},'libraries':libraries,
  'differential_reports':reports,'host_audits':host,'module_source_sha256':source,
  'assets_verified':previous['assets_verified'],'original_input_count':previous['original_input_count'],
  'world_cases':10,'physical_controls_reconstructed':True,'body_transform_kernel_reconstructed':True,
  'update_subobjects_coordinator_reconstructed':True,'physical_stop_orchestration_reconstructed':True,
  'character_body_definition_producer_reconstructed':True,
  'shape_broadphase_backend_reconstructed':False,'physics_world_step_reconstructed':False,
  'original_body_construction_reconstructed':False,'physical_movement_used_by_live_actor':False,
  'physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_game_playable':False,
  'checkpoint_apk_created':False,'goal_status':'active',
  'pending':'Actual body/shape construction and pin/unpin lifecycle, shape synchronization/broadphase/world stepping, visual/root motion and live player/pursuit integration; full rendering/gameplay/progression/UI/audio/saves/assets/device verification.'}
 (rd/'physical-movement-source-validation.json').write_text(json.dumps(result,indent=2)+'\n')
 (ROOT/'reports/world-smoke-physical-movement.json').write_text(json.dumps(world,indent=2)+'\n')
 print(json.dumps({'apk_sha256':result['apk_sha256'],'differential_corpora':len(SPECS),
  'sanitizer_corpora':len(host),'world_cases':10,'physical_movement_used_by_live_actor':False,'goal_status':'active'}))

if __name__=='__main__':main()
