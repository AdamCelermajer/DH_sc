"""Rebuild the complete isolated V6 batch; never rebuild central or Android targets.

Historical, coherent borrowed DSOs are bound by exact bytes. Current source for
those libraries is deliberately not attributed to the copied older binaries.
"""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def unix(p):
 p=str(p.resolve()).replace('\\','/')
 return '/mnt/'+p[0].lower()+p[2:] if len(p)>2 and p[1]==':' else p
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--snapshot',type=Path,default=R/'.local-inputs/player-item-effects-v5/host-snapshot');ap.add_argument('--output',type=Path,default=R/'port/engine-skinning/reports/visual-skin-owner-v6-host-audit-v1.json');a=ap.parse_args()
 scratch=R/'.local-inputs/visual-skin-owner-v6';(R/'.local-inputs/player-item-effects-v5/private-save').mkdir(exist_ok=True)
 files=list((R/'port/engine-skinning').glob('*_v6.*'))+list((R/'port/engine-skinning/tests').glob('*_v6.*'))+list((R/'port/engine-skinning/tools').glob('*_v6*.py'))
 source={p.relative_to(R).as_posix():sha(p)for p in sorted(files)}
 libraries={p.name:sha(p)for p in sorted(a.snapshot.glob('*.so'))};assert len(libraries)==7
 flags=['g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer']
 link=['-L'+unix(a.snapshot),'-Wl,-rpath,'+unix(a.snapshot),'-ldh2_engine_animation','-ldh2_engine_skinning','-ldh2_scene_materials','-ldh2_level_world','-ldh2_game_data','-ldh2_script_runtime','-ldh2_engine_ui']
 es='port/engine-skinning/';gd='port/game-data/';ui='port/engine-ui/'
 selection=[es+'visual_skin_selection_v6.cpp'];owner=[*selection,es+'visual_skin_owner_v6.cpp']
 v5=[gd+'player_gear_effects_v5.cpp',gd+'item_gear_properties_v5.cpp',gd+'item_power_tables_v5.cpp',gd+'item_presentation_v5.cpp',ui+'item_text_owner_v5.cpp',ui+'item_text_varargs_v5.cpp']
 frozen=[*v5,gd+'tests/player_gear_cache_v5.cpp']
 frozen_hash={p:sha(R/p)for p in frozen}
 jobs={
 'selection':([es+'tests/visual_skin_selection_v6.cpp',*selection],[es+'reference/visual-skin-owner-v6/selection-fixtures.bin']),
 'resources':([es+'tests/visual_skin_owner_v6.cpp',*owner],['port/android-native/app/src/main/assets/models/prince_modular.bdae','.local-inputs/visual-skin-owner-v6/weapons','port/android-native/app/src/main/assets/animations/prince_walk_1hand.bdae']),
 'inventory':([es+'tests/visual_skin_inventory_v6.cpp',*owner,*v5],[es+'reference/visual-skin-owner-v6/inventory-fixtures.bin','.local-inputs/items-discovery','.local-inputs/player-item-effects-v5/power-cache','.local-inputs/actors','.local-inputs/combat-data','port/android-native/app/src/main/assets','.local-inputs/player-item-effects-v5/private-save','port/android-native/app/src/main/assets/models/prince_modular.bdae','.local-inputs/visual-skin-owner-v6/weapons',gd+'reference/player-item-effects-v5/starter-effects-fixtures.bin'])}
 def run(command):
  p=subprocess.run(['wsl.exe','-e','bash','-lc','cd '+shlex.quote(unix(R))+' && '+command],capture_output=True,text=True)
  if p.returncode or p.stderr.strip():raise RuntimeError(p.stdout+'\n'+p.stderr)
  return p.stdout.strip()
 results={};executables={};commands={};inputs=set()
 for key,(src,args)in jobs.items():
  exe=scratch/('final-'+key+'-audit');compile=' '.join(shlex.quote(v)for v in [*flags,*src,*link,'-o',unix(exe)]);run(compile)
  command='LD_LIBRARY_PATH='+shlex.quote(unix(a.snapshot))+' ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+' '.join(shlex.quote(v)for v in [unix(exe),*args]);results[key]=json.loads(run(command));assert results[key]['validation']=='PASS'and results[key]['mismatches']==0
  executables[exe.relative_to(R).as_posix()]=sha(exe);commands[key]={'compile':compile,'run':command};inputs.update(R/p for p in args if(R/p).is_file())
 for directory in ('.local-inputs/items-discovery','.local-inputs/player-item-effects-v5/power-cache','.local-inputs/actors','.local-inputs/combat-data','.local-inputs/visual-skin-owner-v6/weapons'):
  inputs.update((R/directory).glob('*.bin'));inputs.update((R/directory).glob('*.bdae'))
 inputs.update((R/'port/android-native/app/src/main/assets/original-cache/data/pydata').glob('common_text_*.bin'))
 inputs.update((R/'port/android-native/app/src/main/assets/original-cache/data').glob('common_*.loc'))
 proofs=[es+'reports/visual-skin-selection-v6-arm64-differential.json',es+'reference/visual-skin-owner-v6/inventory-original-gold.json',gd+'reference/player-item-effects-v5/freeze-manifest.json']
 for p in proofs:assert json.loads((R/p).read_text())['validation']=='PASS'
 assert source=={p.relative_to(R).as_posix():sha(p)for p in sorted(files)}
 assert frozen_hash=={p:sha(R/p)for p in frozen}
 assert libraries=={p.name:sha(p)for p in sorted(a.snapshot.glob('*.so'))}
 report=dict(validation='PASS',host_audits=results,source_sha256=source,frozen_source_sha256=frozen_hash,executable_sha256=executables,borrowed_dependency_sha256=libraries,input_sha256={p.relative_to(R).as_posix():sha(p)for p in sorted(inputs)},proof_sha256={p:sha(R/p)for p in proofs},commands=commands,sanitizers=dict(address=True,undefined=True,leaks=True,findings=0),scope=dict(same_authoritative_V4_inventory=True,all_three_actual_starter_classes=True,actual_172_module_resources=True,actual_781_weapon_resources=True,retained_native_skin_graph=True,source_selection_original_O2_instruction_parity=True,deep_original_factory_execution=False,original_GPU_packed_buffer_ABI=False,full_render_material_texture_factories=False,APK_or_device=False),dependency_scope=__doc__)
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',report=a.output.relative_to(R).as_posix(),host_audits=results)))
if __name__=='__main__':main()
