"""Isolated whole HUD manager/backend and retained real dqhud composition.
No central CMake, facade or vendor mutation. Exact compiler sources and the core
archive are hashed before/after; reached game/script/camera providers remain
explicit fixtures in the retained-resource test.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wp(p):return '/mnt/c/'+str(p)[3:].replace('\\','/')
def run(args):
 q=subprocess.run(args,capture_output=True,text=True)
 if q.returncode:raise RuntimeError(json.dumps({'command':args,'stdout':q.stdout,'stderr':q.stderr}))
 return q.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--core-library',default='/home/adampalace/dh2-gameswf-asan/libgameswf_core.a');p.add_argument('--build-dir',default='/home/adampalace/dh2-hud-manager-audit');p.add_argument('--output',type=Path,default=ROOT/'port/engine-ui/reports/hud-manager-host-audit.json');a=p.parse_args();ui=ROOT/'port/engine-ui';r=wp(ROOT)
 manager=['hud_manager.cpp','hud_player_values.cpp','tests/hud_manager.cpp']
 reentry=['hud_manager.cpp','hud_player_values.cpp','tests/hud_manager_reentry.cpp']
 backend=['hud_manager_backends.cpp','../level-world/character_timers.cpp','tests/hud_manager_backends.cpp']
 core=['swf_movie.cpp','swf_actionscript_connection.cpp','renderfx_text_connection.cpp','swf_viewport_connection.cpp','viewport.cpp','hud_sprite_core.cpp','hud_sprite_timeline.cpp','hud_advance_owner.cpp','hud_manager.cpp','hud_player_values.cpp','hud_manager_core.cpp','hud_manager_backends.cpp','../level-world/character_timers.cpp','tests/hud_manager_core.cpp']
 files={ui/x for x in manager+reentry+backend+core};files.update(ui/x for x in ('hud_manager.hpp','hud_manager_backends.hpp','hud_manager_core.hpp','swf_movie.hpp','swf_actionscript_connection.hpp','renderfx_text_connection.hpp'));before={str(x.relative_to(ROOT)).replace('\\','/'):sha(x) for x in files};library_before=run(['wsl','-e','sha256sum',a.core_library]).split()[0];run(['wsl','-e','mkdir','-p',a.build_dir]);base=['wsl','-e','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-I'+r+'/port/engine-ui'];results={};executables={};commands={}
 for name,sources in (('manager',manager),('backends',backend),('reentry',reentry),('core',core)):
  exe=a.build_dir+'/hud_manager_'+name+'_audit';cmd=base[:]
  if name=='core':cmd+=['-w','-fPIC','-fpermissive','-fno-sanitize=vptr','-isystem',r+'/port/engine-ui/vendor/gameswf1714','-DTU_CONFIG_LINK_TO_JPEGLIB=0','-DTU_CONFIG_LINK_TO_LIBPNG=0','-DTU_CONFIG_LINK_TO_FREETYPE=0','-DTU_CONFIG_LINK_TO_THREAD=0']
  cmd+=[wp(ui/x) for x in sources]
  if name=='core':cmd+=[a.core_library,'-lz','-ldl']
  cmd+=['-o',exe];run(cmd);args=[r+'/port/engine-ui/reference/hud-manager/'+name+'-gold.bin'] if name!='core' else [r+'/.local-inputs/ui-layout-discovery'];results[name]=json.loads(run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,*args]));executables[name]=run(['wsl','-e','sha256sum',exe]).split()[0];commands[name]=cmd
 assert before=={str(x.relative_to(ROOT)).replace('\\','/'):sha(x) for x in files},'Source changed during isolated build'
 assert library_before==run(['wsl','-e','sha256sum',a.core_library]).split()[0],'Core changed during isolated build'
 report=dict(validation='PASS',results=results,source_sha256=before,core_library_sha256=library_before,executable_sha256=executables,compiler_commands=commands,gold_sha256={x:sha(ui/'reference/hud-manager'/x) for x in ('manager-gold.bin','backends-gold.bin','reentry-gold.bin')},asset_sha256={x:sha(ROOT/'.local-inputs/ui-layout-discovery'/x) for x in ('dqshared_droid.swf','dqhud_droid.swf')},sanitizers=['address','undefined(core upstream vptr excluded)','leak'],sanitizer_findings=0,limits={'full_original_AS_or_layout_parity':False,'live_GPU':False,'world_script_camera_settings_localization_texture_providers_are_fixtures':True,'core_plain_text_uses_source_gate_and_upstream_formatter':True,'skill_private_VM_is_required_service':True},scope=__doc__);a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
