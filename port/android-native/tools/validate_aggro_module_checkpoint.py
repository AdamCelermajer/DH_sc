"""Validate the standalone aggression module without claiming AI integration."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from elftools.elf.elffile import ELFFile
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
from validate_world_checkpoint import digest,read,log,ROOT,REPO

def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);prior=read(ROOT/'reports/build-validation-player-defender.json');old_apk=ROOT/'build/checkpoints/dh2-native-player-defender-c47cb955.apk';assert digest(old_apk)==prior['apk_sha256']
 libs=inspect(apk);studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';assert len(libs)==len(inspect(studio_apk))==14
 world=read(local/'world-tests-aggro-coldboot/world-smoke.json');assert len(world['cases'])==10 and world['apk_sha256']==world['installed_apk_sha256']==sha
 original=read(REPO/'port/game-data/reports/aggro-arm64-differential.json');packaged=read(REPO/'port/game-data/reports/aggro-packaged-arm64-differential.json');studio=read(REPO/'port/game-data/reports/aggro-studio-arm64-differential.json');host=read(REPO/'port/game-data/reports/aggro-host-audit.json')
 assert host['original_aggro_cases']==6114 and host['invalid_input_and_capacity_failures_atomic'] and host['finite_float_bits_exact']
 for report,library,fixture in ((original,'aggro-oracle.so','aggro-original.bin'),(packaged,'aggro-packaged-arm64.so','aggro-packaged-original.bin'),(studio,'aggro-studio-arm64.so','aggro-studio-original.bin')):
  assert report['comparisons']==6114 and report['synthetic_cases']==5000 and report['boundary_cases']==675 and report['explicit_behavior_cases']==13 and report['captured_combat_threat_cases']==426 and report['mismatches']==0
  assert report['arm64_library_sha256']==digest(local/library) and report['reference_sha256']==digest(local/fixture)==original['reference_sha256'] and report['original_sha256']==digest(local/'libDungeonHunter2.so')
  for key in ('full_outgoing_and_reciprocal_tables_compared','finite_float_bits_and_all_query_fields_compared','set_preserves_all_supplied_bits','arm64_storage_and_character_keys_above_4gib','original_tree_insertion_rebalancing_and_erase_execute'):assert report[key],key
  assert report['nan_payload_field_differences']==41 and report['arithmetic_nan_comparison']=='unordered NaN class; sign/payload implementation dependent'
  for source in report['combat_reference_sources']:assert source['sha256']==digest(REPO/'port/game-data/reports'/source['file'])
  assert host['inserted']==report['totals']['inserted']==626 and host['removed']==report['totals']['removed']==118 and host['target_clear_requests']==report['totals']['target_clear_requests']==805
 for report in (packaged,studio):assert report['packaged_native_tls_canary_supplied'] and report['native_import_calls']['__memcpy_chk']>0
 required={'dh2_aggro_apply','dh2_aggro_query'};changed=[]
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(old_apk) as old,zipfile.ZipFile(studio_apk) as sz:
  assert set(z.namelist())==set(old.namelist());assert not any('DungeonHunter2.so' in n for n in z.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    data=z.read(name);assert data==old.read(name)==sz.read(name)==(ROOT/'app/src/main'/name).read_bytes()==(a.studio/'app/src/main'/name).read_bytes()
   elif name.startswith('lib/') and name.endswith('.so'):
    if name.endswith('/libdh2_game_data.so'):
     assert required<=exports(z.read(name)) and required<=exports(sz.read(name));changed.append(name)
    else:assert z.read(name)==old.read(name),name
  assert len(changed)==2
  assert hashlib.sha256(z.read('lib/arm64-v8a/libdh2_game_data.so')).hexdigest()==packaged['arm64_library_sha256'] and hashlib.sha256(sz.read('lib/arm64-v8a/libdh2_game_data.so')).hexdigest()==studio['arm64_library_sha256']
 studio_sources={}
 for name,before in prior['studio_source_sha256'].items():assert digest(ROOT/'app/src/main'/name)==digest(a.studio/'app/src/main'/name)==before;studio_sources[name]=before
 changed_sources={'game-data/CMakeLists.txt','game-data/reference/aggro/NOTES.md','game-data/reference/aggro/original-functions.json','game-data/reference/aggro/reference/original-functions.asm'}
 for name,before in prior['module_source_sha256'].items():
  if name not in changed_sources:assert digest(REPO/'port'/name)==before,name
 reference=read(REPO/'port/game-data/reference/aggro/original-functions.json');assert len(reference['functions'])==14 and reference['original_sha256']==original['original_sha256']
 with (local/'libDungeonHunter2.so').open('rb') as stream:
  elf=ELFFile(stream)
  for row in reference['functions']:
   address=int(row['elf_address'],16);segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=address< s['p_vaddr']+s['p_filesz']);stream.seek(segment['p_offset']+address-segment['p_vaddr']);assert hashlib.sha256(stream.read(row['size'])).hexdigest()==row['sha256']
 for name in ('aggro-android-build.log','aggro-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'aggro-zipalign.log')
 for file in (local/'world-tests-aggro-coldboot').glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|application failed|Shader failed|Link failed',log(file)),file
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 additions={'game-data/aggro.cpp','game-data/aggro.hpp','game-data/tests/aggro.cpp','game-data/tests/aggro_differential.py','game-data/tools/build_aggro_oracle.ps1','android-native/tools/validate_aggro_module_checkpoint.py'};modules=set(prior['module_source_sha256'])|changed_sources|additions
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'apk_16k_zip_alignment_verified':True,'assets_verified':prior['assets_verified'],'original_input_count':prior['original_input_count'],'cache_sha256':prior['cache_sha256'],'studio_source_sha256':studio_sources,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'prior_player_defender_checkpoint_sha256':prior['apk_sha256'],'prior_app_sources_and_non_game_data_libraries_unchanged':True,'native_aggro_exports':sorted(required),'original_aggro_cases':6114,'packaged_arm64_aggro_cases':6114,'studio_packaged_arm64_aggro_cases':6114,'aggro_asan_ubsan':host,'aggro_report_sha256':digest(REPO/'port/game-data/reports/aggro-arm64-differential.json'),'packaged_aggro_report_sha256':digest(REPO/'port/game-data/reports/aggro-packaged-arm64-differential.json'),'studio_aggro_report_sha256':digest(REPO/'port/game-data/reports/aggro-studio-arm64-differential.json'),'reference_sha256':original['reference_sha256'],'world_cases':10,'previous_combat_results':'Historical c47cb955 checkpoint: 112 player attacks and 73 player damage attempts. These are not freshly replayed by this module-only validation.','emulator_recovery':'First world regression interrupted by system_server WindowManager/display watchdog. Logs retained; same AVD cold-booted without wiping data; fresh ten-case regression passed.','aggro_scope':original['scope'],'arithmetic_nan_payload_exact':False,'aggro_table_module_implemented':True,'aggro_module_integrated_into_live_combat':False,'automatic_enemy_ai_implemented':False,'physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-aggro-module.json').write_text(json.dumps(validation,indent=2)+'\n');(ROOT/'reports/world-smoke-aggro-module.json').write_text(json.dumps(world,indent=2)+'\n')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-aggro-module-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(prior['assets_verified']),'original_inputs':prior['original_input_count'],'aggro_cases_per_arm64_build':6114,'sanitizer_cases':6114,'world_cases':10,'live_ai_integrated':False,'goal_status':'active'}))
if __name__=='__main__':main()
