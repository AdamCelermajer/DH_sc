"""Build and execute the isolated V5 batch with coherent borrowed host DSOs.
Does not build central CMake, Android, install or alter frozen earlier proofs.
"""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def unix(p):
 p=str(p.resolve()).replace('\\','/')
 return '/mnt/'+p[0].lower()+p[2:] if len(p)>2 and p[1]==':' else p
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--snapshot',type=Path,default=R/'.local-inputs/player-item-effects-v5/host-snapshot');ap.add_argument('--output',type=Path,default=R/'port/game-data/reports/player-item-effects-v5-host-audit.json');a=ap.parse_args();a.output=a.output.resolve()
 scratch=R/'.local-inputs/player-item-effects-v5';private=scratch/'private-save';private.mkdir(exist_ok=True)
 files=[]
 for directory in ('port/game-data','port/engine-ui'):
  files.extend(p for p in (R/directory).glob('*_v5.*') if p.suffix in('.cpp','.hpp'))
  files.extend(p for p in (R/directory/'tests').glob('*_v5*') if p.suffix in('.cpp','.py'))
 files.append(Path(__file__))
 sources={p.relative_to(R).as_posix():sha(p) for p in sorted(files)}
 libraries={p.name:sha(p)for p in sorted(a.snapshot.glob('*.so'))};assert len(libraries)==7
 flags=['g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer']
 link=['-L'+unix(a.snapshot),'-Wl,-rpath,'+unix(a.snapshot),'-ldh2_level_world','-ldh2_engine_ui','-ldh2_game_data','-ldh2_script_runtime']
 gd='port/game-data/';ui='port/engine-ui/'
 gear=[gd+'item_gear_properties_v5.cpp',gd+'item_power_tables_v5.cpp']
 presentation=[gd+'item_presentation_v5.cpp',gd+'item_power_tables_v5.cpp']
 effects=[gd+'player_gear_effects_v5.cpp',*gear]
 jobs={
 'gear':([gd+'tests/item_gear_properties_v5.cpp',*gear],[gd+'reference/player-item-effects-v5/gear-fixtures.bin',gd+'reference/player-item-effects-v5/power-fixtures.bin','.local-inputs/player-item-effects-v5/power-cache']),
 'owner':([gd+'tests/player_gear_effects_v5.cpp',*effects],[gd+'reference/player-item-effects-v5/starter-effects-fixtures.bin','.local-inputs/items-discovery','.local-inputs/player-item-effects-v5/power-cache','.local-inputs/actors','.local-inputs/combat-data']),
 'presentation':([gd+'tests/item_presentation_v5.cpp',gd+'tests/item_presentation_v5_fixture.cpp',gd+'tests/item_power_instance_v5_fixture.cpp',*presentation],[gd+'reference/player-item-effects-v5/presentation-fixtures.bin',gd+'reference/player-item-effects-v5/power-instance-fixtures.bin','.local-inputs/player-item-effects-v5/power-cache']),
 'varargs':([ui+'tests/item_text_varargs_v5.cpp',ui+'tests/item_text_varargs_v5_fixture.cpp',ui+'item_text_varargs_v5.cpp'],[ui+'reference/item-text-varargs-v5/fixtures.bin']),
 'cache':([gd+'tests/player_gear_cache_v5.cpp',*effects,gd+'item_presentation_v5.cpp',ui+'item_text_owner_v5.cpp',ui+'item_text_varargs_v5.cpp'],[gd+'reference/player-item-effects-v5/starter-effects-fixtures.bin','.local-inputs/items-discovery','.local-inputs/player-item-effects-v5/power-cache','.local-inputs/actors','.local-inputs/combat-data','port/android-native/app/src/main/assets','.local-inputs/player-item-effects-v5/private-save']),
 'skin':([gd+'tests/player_skin_v5.cpp',gd+'tests/player_skin_v5_fixture.cpp',*effects],[gd+'reference/player-item-effects-v5/skin-fixtures.bin','.local-inputs/items-discovery'])}
 def run(command):
  p=subprocess.run(['wsl.exe','-e','bash','-lc','cd '+shlex.quote(unix(R))+' && '+command],capture_output=True,text=True)
  if p.returncode:raise RuntimeError(p.stdout+p.stderr)
  if p.stderr.strip():raise RuntimeError('Unexpected sanitizer/compiler diagnostics: '+p.stderr)
  return p.stdout.strip()
 results={};binaries={};commands={};inputs=set()
 for key,(src,args)in jobs.items():
  exe=scratch/('host-'+key+'-audit');compile=' '.join(shlex.quote(v)for v in [*flags,*src,*link,'-o',unix(exe)])
  run(compile);command='LD_LIBRARY_PATH='+shlex.quote(unix(a.snapshot))+' ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+' '.join(shlex.quote(v)for v in [unix(exe),*args]);results[key]=json.loads(run(command));assert results[key]['validation']=='PASS'and results[key]['mismatches']==0
  binaries[exe.relative_to(R).as_posix()]=sha(exe);commands[key]={'compile':compile,'run':command};inputs.update(R/v for v in args if (R/v).is_file())
 for directory in('.local-inputs/player-item-effects-v5/power-cache','.local-inputs/items-discovery','.local-inputs/actors','.local-inputs/combat-data'):
  inputs.update((R/directory).glob('*.bin'))
 root=R/'port/android-native/app/src/main/assets/original-cache/data/pydata'
 inputs.update(root.glob('common_text_*.bin'))
 inputs.update(R/p for p in results['cache']['actual_localization_files'])
 proofs=['port/game-data/reports/'+n+'-arm64-differential.json'for n in('item-gear-properties-v5','item-power-tables-v5','item-presentation-v5','item-power-instance-v5','player-skin-v5')]+['port/engine-ui/reports/item-text-varargs-v5-arm64-differential.json']
 for p in proofs:assert json.loads((R/p).read_text())['validation']=='PASS'
 assert sources=={p.relative_to(R).as_posix():sha(p)for p in sorted(files)}
 assert libraries=={p.name:sha(p)for p in sorted(a.snapshot.glob('*.so'))}
 report={'validation':'PASS','host_audits':results,'source_sha256':sources,'executable_sha256':binaries,'borrowed_dependency_sha256':libraries,'input_sha256':{p.relative_to(R).as_posix():sha(p)for p in sorted(inputs)},'original_proof_sha256':{p:sha(R/p)for p in proofs},'commands':commands,'sanitizers':{'address':True,'undefined':True,'leaks':True,'findings':0},'scope':{'same_authoritative_v4_inventory':True,'real_cache_localized_starter_text':True,'real_property_class_vitals':True,'skin_caller_only_explicit_factory_services':True,'full_visual_object_skin_graph':False,'powered_loot_creation_valuation':False,'full_campaign_creation':False,'APK_or_device':False},'dependency_scope':'Coherent copied host DSOs are exact binary dependencies. This receipt does not reattribute their historical object source captures to moving central sources.'}
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','report':a.output.relative_to(R).as_posix(),'host_audits':results}))
if __name__=='__main__':main()
