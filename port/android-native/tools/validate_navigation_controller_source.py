"""Record verified coordinator source without claiming live controller integration."""
import argparse,hashlib,json,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import fields
from validate_navigation_producers_checkpoint import EXPORTS

TOTALS={'destination_queries':97,'updates':1035,'move_calls':173,'at_destination':246,'stopped':7,'avoidance_evaluated':75,'adjusted':31,'turn_limited':30,'boundary_checks':318,'direction_valid':202,'physical_stop_requests':7,'floor_queries':605}

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';rd=REPO/'port/level-world/reports';prior_path=ROOT/'reports/build-validation-navigation-heading.json';prior=read(prior_path);engine=local/'libDungeonHunter2.so';reference=digest(local/'navigation-controller-original.bin');heading_reference=prior['reference_sha256'];reports={};heading={};libraries={}
 for tag in ('oracle','packaged','studio'):
  suffix='' if tag=='oracle' else '-'+tag;library=local/('navigation-controller-oracle.so' if tag=='oracle' else f'navigation-controller-{tag}-world-arm64.so');sha=digest(library);libraries[tag]=sha
  report=read(rd/f'navigation-controller{suffix}-arm64-differential.json');fields(report,{'comparisons':1132,'mismatches':0,'totals':TOTALS,'reference_sha256':reference,'arm64_library_sha256':sha,'original_sha256':digest(engine),'floor_source_sha256':prior['floor_geometry_sha256'],'original_coordinator_and_helpers_execute':True,'physical_setters_are_services':True,'physical_backend_reconstructed':False,'imported_libm_modeled':True});assert digest(local/f'navigation-controller{suffix}-original.bin')==reference;reports[tag]=report
  report=read(rd/f'navigation-controller-heading-{tag}-regression.json');fields(report,{'comparisons':4476,'mismatches':0,'arm64_library_sha256':sha,'reference_sha256':heading_reference,'original_sha256':digest(engine)});assert digest(local/f'navigation-controller-heading-{tag}-original.bin')==heading_reference;heading[tag]=report
 host={'controller':read(rd/'navigation-controller-host-audit.json')};fields(host['controller'],{'comparisons':1132,'updates':1035,'destination_queries':97,'move_calls':173,'boundary_checks':318,'physical_stop_requests':7,'floor_queries_compared_by_arm64_oracle':605,'atomic_rejection_checks':8,'reference_sha256':reference,'physical_backend_reconstructed':False});assert host['controller']['maximum_host_angle_ulp']<=2
 for name in ('heading','producers','avoidance','objects','motion','find','path','world','search'):
  report=read(rd/f'navigation-controller-{name}-regression-host-audit.json');fields(report,prior['host_audits'][name]);assert report['reference_sha256']==digest(local/f'navigation-{name}-original.bin');host[name]=report
 for report in host.values():fields(report,{'mismatches':0,'sanitizers':['address','undefined']})
 apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14;assets={}
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist()) and not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes();assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.endswith('/libdh2_level_world.so'):
    required=EXPORTS|{'dh2_nav_look_towards','dh2_nav_set_heading','dh2_nav_update_path','dh2_nav_is_at_destination'};assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  assert assets==prior['assets_verified'] and len(assets)==126
  for tag,archive,old in [('packaged',z,prior['libraries']),('studio',sz,prior['studio_libraries'])]:
   assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==libraries[tag]
   for row in old:
    if Path(row['path']).name not in {'libdh2_level_world.so','libdh2_native.so'}:assert hashlib.sha256(archive.read(row['path'])).hexdigest()==row['sha256']
 world=read(local/'world-tests-navigation-controller/world-smoke.json');fields(world,{'apk_sha256':sha,'installed_apk_sha256':sha,'native_heading_used_by_player_movement':True,'original_movement_controller_reconstructed':False,'physical_arm64_tested':False,'full_game_playable':False,'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True});assert len(world['cases'])==10
 source={}
 for name,expected in prior['studio_source_sha256'].items():
  value=digest(ROOT/'app/src/main'/name);assert value==digest(a.studio/'app/src/main'/name)==expected;source[name]=value
 changed={'android-native/README.md','android-native/ROADMAP.md','level-world/README.md','level-world/CMakeLists.txt','level-world/navigation_heading.cpp','level-world/navigation_heading.hpp'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 additions={'level-world/navigation_controller.cpp','level-world/navigation_controller.hpp','level-world/tests/navigation_controller.cpp','level-world/tests/navigation_controller_differential.py','level-world/tools/build_navigation_controller_oracle.ps1','android-native/tools/validate_navigation_controller_source.py'};capture=REPO/'port/level-world/reference/navigation-controller';additions|={f.relative_to(REPO/'port').as_posix() for f in capture.rglob('*') if f.is_file()};verify_capture(capture/'original-functions.json',engine)
 for name in ('navigation-controller-repo-build.log','navigation-controller-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 zipalign=Path.home()/'AppData/Local/Android/Sdk/build-tools/36.0.0/zipalign.exe'
 with (local/'navigation-controller-zipalign.log').open('w') as output:
  for file in (apk,studio_apk):subprocess.run([str(zipalign),'-c','-P','16','-v','4',str(file)],stdout=output,stderr=subprocess.STDOUT,check=True)
 assert log(local/'navigation-controller-zipalign.log').count('Verification successful')==2
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'assets_verified':assets,'original_input_count':prior['original_input_count'],'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(set(prior['module_source_sha256'])|changed|additions)},'controller_reports':reports,'heading_regressions':heading,'reference_sha256':reference,'host_audits':host,'world_cases':10,'apk_16k_zip_alignment_verified':True,'coordinator_core_reconstructed':True,'coordinator_used_by_actor_movement':False,'physical_backend_reconstructed':False,'original_movement_controller_reconstructed':False,'original_gpu_parity_verified':False,'physical_arm64_tested':False,'full_game_playable':False,'prior_heading_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'pending':'Physical Stop and UpdateSubObjects/body/shape initialization, original input/root motion and live player/pursuit integration. Full game rendering/progression/UI/audio/saves/assets and device verification remain pending.','goal_status':'active'}
 (rd/'navigation-controller-source-validation.json').write_text(json.dumps(result,indent=2)+'\n');(ROOT/'reports/world-smoke-navigation-controller.json').write_text(json.dumps(world,indent=2)+'\n');print(json.dumps({'apk_sha256':sha,'coordinator_comparisons_per_arm64_binary':1132,'heading_comparisons_per_arm64_binary':4476,'sanitizer_corpora':10,'world_cases':10,'coordinator_used_by_actor_movement':False,'goal_status':'active'}))

if __name__=='__main__':main()
