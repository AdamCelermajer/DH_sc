"""Bind native AI source, assets, packaged oracles and live emulator evidence."""
import argparse,hashlib,json,re,subprocess,zipfile
from pathlib import Path
from elftools.elf.elffile import ELFFile
from emulator_smoke import inspect
from validate_combat_checkpoint import exports
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def log(p):
 b=p.read_bytes();return b.decode('utf-16') if b.startswith((b'\xff\xfe',b'\xfe\xff')) else b.decode('utf-8-sig')
def main():
 p=argparse.ArgumentParser();p.add_argument('--studio',type=Path,required=True);p.add_argument('--host-build',default='/home/adampalace/dh2-world-build');a=p.parse_args();local=REPO/'.local-inputs';apk=ROOT/'app/build/outputs/apk/debug/app-debug.apk';studio_apk=a.studio/'app/build/outputs/apk/debug/app-debug.apk';sha=digest(apk);prior=read(ROOT/'reports/build-validation-aggro-module.json');libs=inspect(apk);inspect(studio_apk)
 original=read(REPO/'port/game-data/reports/ai-arm64-differential.json');packaged=read(REPO/'port/game-data/reports/ai-packaged-arm64-differential.json');studio=read(REPO/'port/game-data/reports/ai-studio-arm64-differential.json');host=read(REPO/'port/game-data/reports/ai-host-audit.json');aggro=read(REPO/'port/game-data/reports/ai-aggro-regression.json');aggro_host=read(REPO/'port/game-data/reports/ai-aggro-host-regression.json')
 for r in (original,packaged,studio):assert r['comparisons']==23304 and r['mismatches']==0 and r['reference_sha256']==original['reference_sha256'];assert r['geometry_cases']==1800 and r['faction_cases']==1024 and r['target_event_cases']==20480
 assert host['mismatches']==0 and host['target_event_cases']==20480 and host['original_ai_rows_roundtripped']==76 and host['original_faction_rows_roundtripped']==16
 assert aggro['comparisons']==6114 and aggro['mismatches']==0 and aggro['arm64_library_sha256']==packaged['arm64_library_sha256'] and aggro_host['original_aggro_cases']==6114
 enemy=read(local/'enemy-ai-tests-verified/enemy-ai-smoke.json');player=read(local/'player-combat-tests-ai-verified/player-combat-smoke.json');world=read(local/'world-tests-ai/world-smoke.json')
 assert enemy['apk_sha256']==player['apk_sha256']==world['apk_sha256']==sha;assert enemy['automatic_damage_attempts_verified']==24 and enemy['death_clears_enemy_target_and_stops_controller'] and enemy['original_result_health_rng_owner_checksums_match'] and not enemy['explicit_development_targets_used'];assert player['native_player_attempts_verified']==112 and player['reciprocal_relation_and_rotation_retention_verified'];assert len(world['cases'])==10
 required={'dh2_ai_range','dh2_ai_enemy','dh2_ai_target_update','dh2_aggro_apply','dh2_aggro_query'};assets={};source={};new_provenance=read(ROOT/'app/src/main/assets/ai-provenance.json');assert new_provenance['cache_sha256']==prior['cache_sha256'];assert len(new_provenance['inputs'])==6
 with zipfile.ZipFile(apk) as z,zipfile.ZipFile(studio_apk) as sz:
  assert not any('DungeonHunter2.so' in n for n in z.namelist());assert set(z.namelist())==set(sz.namelist())
  for name in z.namelist():
   if name.startswith('assets/') and not name.endswith('/'):
    raw=z.read(name);relative=name[len('assets/'):];assert raw==sz.read(name)==(ROOT/'app/src/main/assets'/relative).read_bytes()==(a.studio/'app/src/main/assets'/relative).read_bytes();assets[relative]={'bytes':len(raw),'sha256':hashlib.sha256(raw).hexdigest()}
   elif name.startswith('lib/') and name.endswith('/libdh2_game_data.so'):assert required<=exports(z.read(name)) and required<=exports(sz.read(name))
  for name,expected in prior['assets_verified'].items():assert assets[name]==expected,name
  for row in new_provenance['inputs']:assert assets[row['asset']]=={'bytes':row['bytes'],'sha256':row['sha256']}
  assert hashlib.sha256(z.read('lib/arm64-v8a/libdh2_game_data.so')).hexdigest()==packaged['arm64_library_sha256'];assert hashlib.sha256(sz.read('lib/arm64-v8a/libdh2_game_data.so')).hexdigest()==studio['arm64_library_sha256']
 for name in prior['studio_source_sha256']:assert digest(ROOT/'app/src/main'/name)==digest(a.studio/'app/src/main'/name);source[name]=digest(ROOT/'app/src/main'/name)
 for folder in ('ai-target','ai-reader'):
  manifest=read(REPO/'port/game-data/reference'/folder/'original-functions.json');assert manifest['original_sha256']==original['original_sha256']
  with (local/'libDungeonHunter2.so').open('rb') as stream:
   elf=ELFFile(stream)
   for row in manifest['functions']:
    address=int(row['elf_address'],16);segment=next(s for s in elf.iter_segments() if s['p_type']=='PT_LOAD' and s['p_vaddr']<=address<s['p_vaddr']+s['p_filesz']);stream.seek(segment['p_offset']+address-segment['p_vaddr']);assert hashlib.sha256(stream.read(row['size'])).hexdigest()==row['sha256']
 for name in ('ai-android-build.log','ai-studio-build.log'):assert 'BUILD SUCCESSFUL' in log(local/name)
 assert 'Verification successful' in log(local/'ai-zipalign.log')
 for folder in ('enemy-ai-tests-verified','player-combat-tests-ai-verified','world-tests-ai'):
  for f in (local/folder).glob('*.log'):assert not re.search(r'FATAL EXCEPTION|GL error|load failed|sample failed|application failed|animation failed|Shader failed|Link failed',log(f)),f
 cache=subprocess.run(['wsl','cat',a.host_build+'/CMakeCache.txt'],capture_output=True,text=True,check=True).stdout
 for prefix in ('CMAKE_CXX_FLAGS:STRING=','CMAKE_EXE_LINKER_FLAGS:STRING=','CMAKE_SHARED_LINKER_FLAGS:STRING='):assert '-fsanitize=address,undefined' in next(line for line in cache.splitlines() if line.startswith(prefix))
 additions={'game-data/ai.cpp','game-data/ai.hpp','game-data/tests/ai.cpp','game-data/tests/ai_differential.py','game-data/tools/build_ai_oracle.ps1','game-data/reference/ai-target/NOTES.md','level-world/tools/prepare_ai.py','android-native/tools/enemy_ai_smoke.py','android-native/tools/validate_enemy_ai_checkpoint.py'}
 for folder in ('ai-target','ai-reader'):
  additions|={f'game-data/reference/{folder}/original-functions.json',f'game-data/reference/{folder}/reference/original-functions.asm'}
 modules=set(prior['module_source_sha256'])|additions
 reports={name:digest(REPO/'port/game-data/reports'/name) for name in ('ai-arm64-differential.json','ai-packaged-arm64-differential.json','ai-studio-arm64-differential.json','ai-host-audit.json','ai-aggro-regression.json','ai-aggro-host-regression.json')}
 validation={'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'studio_apk_sha256':digest(studio_apk),'libraries':libs,'apk_16k_zip_alignment_verified':True,'assets_verified':assets,'original_input_count':prior['original_input_count']+6,'cache_sha256':prior['cache_sha256'],'studio_source_sha256':source,'module_source_sha256':{name:digest(REPO/'port'/name) for name in sorted(modules)},'original_ai_cases':23304,'packaged_arm64_ai_cases':23304,'studio_packaged_arm64_ai_cases':23304,'ai_asan_ubsan':host,'aggro_regression_cases':6114,'aggro_asan_ubsan':aggro_host,'report_sha256':reports,'automatic_enemy_damage_attempts':24,'isolated_player_attack_attempts':112,'native_aggression_updates_verified':player['native_aggression_updates_verified'],'world_cases':10,'automatic_enemy_melee_default_enabled':True,'target_death_stops_controller':True,'ai_tables_and_target_events_integrated':True,'full_original_enemy_ai_implemented':False,'pending_ai':'Original spatial index/filter/order/timers, pursuit, full FSM/attack delay, callbacks and global producer/RNG interleaving. Threat relations use pre-damage facts but are updated after application while callbacks are pending.','first_automatic_smoke_attempt':'Initial 55-second death deadline was too short for 24 authored attack sequences; observed 19 valid attempts. Same healthy emulator, harness deadline increased, fresh 24-attempt run passed. No emulator restart.','first_player_regression_attempt':'All 112 result/health/RNG checks passed; newly added threat assertion incorrectly assumed zero player threat. Assertion changed to captured original application threat words and finite rounded Add delta. Fresh full regression passed.','physical_arm64_tested':False,'full_cache_bundled':False,'full_game_playable':False,'goal_status':'active'}
 (ROOT/'reports/build-validation-enemy-ai.json').write_text(json.dumps(validation,indent=2)+'\n')
 for filename,report in (('enemy-ai-smoke.json',enemy),('player-combat-smoke-enemy-ai.json',player),('world-smoke-enemy-ai.json',world)):(ROOT/'reports'/filename).write_text(json.dumps(report,indent=2)+'\n')
 checkpoint=ROOT/'build/checkpoints'/f'dh2-native-enemy-ai-{sha[:8]}.apk';checkpoint.write_bytes(apk.read_bytes());assert digest(checkpoint)==sha
 print(json.dumps({'apk_sha256':sha,'apk_bytes':apk.stat().st_size,'assets':len(assets),'original_inputs':validation['original_input_count'],'ai_cases_per_arm64_build':23304,'aggro_regression_cases':6114,'automatic_enemy_damage_attempts':24,'player_attack_regression_attempts':112,'world_cases':10,'full_game_playable':False,'goal_status':'active'}))
if __name__=='__main__':main()
