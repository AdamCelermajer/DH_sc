"""Bind original heading reconstruction to both APKs and real movement/melee."""
import argparse,hashlib,json,math,re,subprocess,zipfile
from pathlib import Path
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_navigation_checkpoint import ROOT,REPO,digest,read,log
from validate_collision_checkpoint import verify_capture
from validate_selector_checkpoint import fields
from validate_navigation_producers_checkpoint import EXPORTS as PRODUCER_EXPORTS

TOTALS={'look':2132,'set_heading':2344,'self_alias':2171,'look_calls':2996,'active':2674,'normalized':984}
MARKER='Native heading control | player movement and melee facing use recovered source | full UpdatePath and physics pending'

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 local=REPO/'.local-inputs';rd=REPO/'port/level-world/reports';prior_path=ROOT/'reports/build-validation-navigation-producers.json';prior=read(prior_path)
 apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);libs=inspect(apk);studio_libs=inspect(studio_apk);assert len(libs)==len(studio_libs)==14
 engine=local/'libDungeonHunter2.so';engine_sha=digest(engine);reference=digest(local/'navigation-heading-original.bin');reports={};library_sha={}
 for tag in ('oracle','packaged','studio'):
  suffix='' if tag=='oracle' else '-'+tag;library=local/('navigation-heading-oracle.so' if tag=='oracle' else f'navigation-heading-{tag}-world-arm64.so');library_sha[tag]=digest(library)
  report=read(rd/f'navigation-heading{suffix}-arm64-differential.json')
  fields(report,{'comparisons':4476,'mismatches':0,'totals':TOTALS,'original_sha256':engine_sha,'arm64_library_sha256':library_sha[tag],'reference_sha256':reference,'original_heading_branches_execute':True,'self_alias_semantics_compared':True,'imported_libm_modeled':True})
  assert digest(local/f'navigation-heading{suffix}-original.bin')==reference;reports[tag]=report
 host={'heading':read(rd/'navigation-heading-host-audit.json')}
 fields(host['heading'],{'cases':4476,'look':2132,'set_heading':2344,'self_alias':2171,'active':2674,'atomic_rejection_checks':6,'angle_tolerance_ulp':2,'reference_sha256':reference})
 assert host['heading']['maximum_host_angle_ulp']<=2
 for name in ('producers','avoidance','objects','motion','find','path','world','search'):
  report=read(rd/f'navigation-heading-{name}-regression-host-audit.json');old=name if name=='producers' else 'producers-'+name+'-regression'
  fields(report,prior['host_audits'][old]);assert report['reference_sha256']==digest(local/f'navigation-{name}-original.bin');host[name]=report
 for report in host.values():fields(report,{'mismatches':0,'sanitizers':['address','undefined']})
 world_dir=local/'world-tests-navigation-heading';world=read(world_dir/'world-smoke.json')
 fields(world,{'apk_sha256':sha,'installed_apk_sha256':sha,'native_floor_records':8,'native_graph_nodes':335,'native_graph_edges':838,'original_heading_reconstructed':True,'native_heading_used_by_player_movement':True,'original_movement_controller_reconstructed':False,'frozen_idle_stable':True,'live_idle_changed_pixels':True,'rotation_position_preserved':True,'pause_cancels_movement':True,'physical_arm64_tested':False,'full_game_playable':False})
 assert len(world['cases'])==10 and world['floor_geometry_sha256']==prior['floor_geometry_sha256']
 for name,angle in [('stairs',0.),('rubble_boundary',math.pi/2)]:
  row=next(r for r in world['movement'] if r['case']==name);assert row['native_heading_updates']>0 and abs(row['heading_angle']-angle)<1e-6
 old_world=read(ROOT/'reports/world-smoke-navigation-producers.json')
 for key in ('crypt_route_probe','crypt_world_route_probe','crypt_findpath_probe','crypt_motion_probe','crypt_objects_probe','crypt_avoidance_probe','crypt_producers_probe'):assert world[key]==old_world[key]
 combat_dir=local/'player-combat-tests-navigation-heading';combat=read(combat_dir/'player-combat-smoke.json');melee=read(combat_dir/'player-heading.json')
 fields(combat,{'apk_sha256':sha,'installed_apk_sha256':sha,'native_player_attempts_verified':112,'out_of_reach_rejected':True,'frozen_attack_no_hit':True,'busy_attack_does_not_restart':True,'original_player_sheet_checksum_verified':True,'automatic_enemy_ai_disabled_for_isolated_regression':True,'physical_arm64_tested':False,'full_game_playable':False})
 fields(melee,{'apk_sha256':sha,'native_heading_used_by_player_melee':True,'angle_tolerance_radians':2e-6});assert melee['attack_heading_checks']==len(melee['directions_and_angles'])>0
 fields(combat,{'original_player_application_report_sha256':digest(REPO/'port/game-data/reports/player-application-arm64-differential.json')})
 assert len(combat['attempts'])==112 and combat['attempts']==read(ROOT/'reports/player-combat-smoke-enemy-ai.json')['attempts']
 assets={};unchanged=[]
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(sz.namelist()) and not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[7:];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes();assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_level_world.so'):
    required=PRODUCER_EXPORTS|{'dh2_nav_look_towards','dh2_nav_set_heading'};assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  assert assets==prior['assets_verified'] and len(assets)==126
  for tag,archive in [('packaged',z),('studio',sz)]:assert hashlib.sha256(archive.read('lib/arm64-v8a/libdh2_level_world.so')).hexdigest()==library_sha[tag]
  for rows,archive in [(prior['libraries'],z),(prior['studio_libraries'],sz)]:
   for row in rows:
    if Path(row['path']).name not in {'libdh2_level_world.so','libdh2_native.so'}:
     assert hashlib.sha256(archive.read(row['path'])).hexdigest()==row['sha256'],row['path']
     if archive is z:unchanged.append(row['path'])
 source={}
 for name,expected in prior['studio_source_sha256'].items():
  value=digest(ROOT/'app/src/main'/name);assert value==digest(a.studio/'app/src/main'/name)
  if name!='cpp/model_renderer.cpp':assert value==expected,name
  source[name]=value
 changed={'android-native/README.md','android-native/ROADMAP.md','android-native/tools/world_smoke.py','android-native/tools/player_combat_smoke.py','level-world/CMakeLists.txt','level-world/README.md'}
 for name,expected in prior['module_source_sha256'].items():
  if name not in changed:assert digest(REPO/'port'/name)==expected,name
 additions={'level-world/navigation_heading.cpp','level-world/navigation_heading.hpp','level-world/tests/navigation_heading.cpp','level-world/tests/navigation_heading_differential.py','level-world/tools/build_navigation_heading_oracle.ps1','android-native/tools/validate_navigation_heading_checkpoint.py'}
 capture=REPO/'port/level-world/reference/navigation-heading';additions|={f.relative_to(REPO/'port').as_posix() for f in capture.rglob('*') if f.is_file()};verify_capture(capture/'original-functions.json',engine)
 for name in ('navigation-heading-repo-build.log','navigation-heading-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert log(local/'navigation-heading-zipalign.log').count('Verification successful')==2
 for path in [*world_dir.glob('*.log'),combat_dir/'player-combat.log']:
  text=log(path);assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|Shader failed|Link failed',text),path;assert MARKER in text,path
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 result={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'studio_libraries':studio_libs,'unchanged_libraries':unchanged,'assets_verified':assets,'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{n:digest(REPO/'port'/n) for n in sorted(set(prior['module_source_sha256'])|changed|additions)},'heading_reports':reports,'reference_sha256':reference,'host_audits':host,'world_cases':10,'player_attempts_verified':112,'melee_heading_checks':melee['attack_heading_checks'],'floor_geometry_sha256':prior['floor_geometry_sha256'],'apk_16k_zip_alignment_verified':True,'prior_producers_checkpoint':{'apk_sha256':prior['apk_sha256'],'validation_sha256':digest(prior_path)},'prior_combat_checkpoint':prior['prior_combat_checkpoint'],'original_heading_reconstructed':True,'native_heading_used_by_player_movement':True,'native_heading_used_by_player_melee':True,'original_movement_controller_reconstructed':False,'original_physical_body_construction_reconstructed':False,'pending_navigation':'Full UpdatePath/physical body/root motion and pursuit, physical shape/bounds/capability producers and lifecycle.','physical_arm64_tested':False,'original_gpu_parity_verified':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 for name,data in [('build-validation-navigation-heading',result),('world-smoke-navigation-heading',world),('player-combat-smoke-navigation-heading',combat),('player-heading-smoke',melee)]:(ROOT/f'reports/{name}.json').write_text(json.dumps(data,indent=2)+'\n',encoding='utf-8')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-heading-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'checkpoint':str(checkpoint),'heading_comparisons_per_arm64_binary':4476,'world_cases':10,'player_attempts_verified':112,'melee_heading_checks':melee['attack_heading_checks'],'goal_status':'active'}))

if __name__=='__main__':main()
