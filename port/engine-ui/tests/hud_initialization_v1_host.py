"""Source-gold replay and actual retained authored HUD lifecycle.
Uses a pre-copied coherent native DSO snapshot, never rebuilding shared DSOs.
The receipt binds exact new compiler inputs and actual linked DSO bytes; it
does not relabel the snapshot as current-source compiler provenance.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
REPO=Path(__file__).resolve().parents[3];UI=REPO/'port/engine-ui'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wp(p):return '/mnt/c/'+str(p)[3:].replace('\\','/')
def run(args):
 q=subprocess.run(args,capture_output=True,text=True)
 if q.returncode:raise RuntimeError(json.dumps(dict(command=args,stdout=q.stdout,stderr=q.stderr)))
 return q.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--snapshot',default='/home/adampalace/dh2-hud-initialization-v1-snapshot');p.add_argument('--build-dir',default='/home/adampalace/dh2-hud-initialization-v1-audit');p.add_argument('--output',type=Path,default=UI/'reports/hud-initialization-v1-host-audit.json');a=p.parse_args();r=wp(REPO)
 sources=['hud_manager_core_v2.hpp','hud_manager_core_v2.cpp','hud_initialization_v1.cpp','hud_initialization_v1.hpp','hud_initialization_core_v1.cpp','hud_initialization_core_v1.hpp','hud_initialization_owned_v1.cpp','hud_initialization_owned_v1.hpp','tests/hud_initialization_v1.cpp','tests/hud_initialization_core_v1.cpp','tests/hud_manager_core.cpp','swf_movie.hpp','swf_actionscript_connection.hpp','owned_hud_settings_v1.hpp','../game-data/player_savegame_v1.cpp','../game-data/player_savegame_v1.hpp','../game-data/skill_tables.hpp','../game-data/properties.hpp']
 before={str((UI/x).relative_to(REPO)).replace('\\','/'):sha(UI/x) for x in sources};manifest=json.loads(run(['wsl','-e','cat',a.snapshot+'/manifest.json']));libs={v['snapshot']:v['sha256'] for v in manifest.values()}
 for path,h in libs.items():assert run(['wsl','-e','sha256sum',path]).split()[0]==h
 run(['wsl','-e','mkdir','-p',a.build_dir]);base=['wsl','-e','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-I'+r+'/port/engine-ui'];commands={};results={};exehashes={};ldd={}
 for name,src in [('pure',['hud_initialization_v1.cpp','tests/hud_initialization_v1.cpp']),('core',['hud_manager_core_v2.cpp','hud_initialization_v1.cpp','hud_initialization_core_v1.cpp','hud_initialization_owned_v1.cpp','../game-data/player_savegame_v1.cpp','tests/hud_initialization_core_v1.cpp'])]:
  cmd=base[:];exe=a.build_dir+'/hud_initialization_'+name+'_v1_audit'
  if name=='core':cmd+=['-w','-fno-sanitize=vptr','-fpermissive','-isystem',r+'/port/engine-ui/vendor/gameswf1714','-DTU_CONFIG_LINK_TO_JPEGLIB=0','-DTU_CONFIG_LINK_TO_LIBPNG=0','-DTU_CONFIG_LINK_TO_FREETYPE=0','-DTU_CONFIG_LINK_TO_THREAD=0']
  cmd += [wp(UI/x) for x in src]
  if name=='core':cmd+=['-L'+a.snapshot,'-Wl,-rpath,'+a.snapshot,'-Wl,-rpath-link,'+a.snapshot,'-ldh2_engine_ui','-ldh2_level_world','-ldh2_game_data','-lz','-ldl']
  cmd+=['-o',exe];commands[name]=cmd;run(cmd);args=[r+'/port/engine-ui/reference/hud-initialization-v1/wrappers-gold.bin',r+'/port/engine-ui/reference/hud-initialization-v1/options-gold.bin'] if name=='pure' else [r+'/.local-inputs/ui-layout-discovery',r+'/port/android-native/app/src/main/assets'];results[name]=json.loads(run(['wsl','-e','env','LD_LIBRARY_PATH='+a.snapshot,'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,*args]));
  if name=='core':results['base_authored']=json.loads(run(['wsl','-e','env','LD_LIBRARY_PATH='+a.snapshot,'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,*args,'dqhud.swf']))
  exehashes[name]=run(['wsl','-e','sha256sum',exe]).split()[0];ldd[name]=run(['wsl','-e','env','LD_LIBRARY_PATH='+a.snapshot,'ldd',exe])
 assert before=={str((UI/x).relative_to(REPO)).replace('\\','/'):sha(UI/x) for x in sources},'Inputs changed during compile/test'
 for path,h in libs.items():assert run(['wsl','-e','sha256sum',path]).split()[0]==h
 assets=['data/skills_pyarray.bin','data/skills_pyarraynames.bin','data/skills_pystructnames.bin','data/character_properties_pyarray.bin','data/character_properties_pyarraynames.bin','data/character_properties_pystructnames.bin','data/design_pyarray.bin','data/design_pyarraynames.bin','data/design_pystructnames.bin','original-cache/data/pydata/common_text_pyarray.bin','original-cache/data/pydata/common_text_pyarraynames.bin','original-cache/data/pydata/common_text_pystructnames.bin']
 report=dict(validation='PASS',results=results,source_sha256=before,dependency_snapshot=manifest,actual_linked_dependencies=ldd,executable_sha256=exehashes,compiler_commands=commands,gold_sha256={n:sha(UI/'reference/hud-initialization-v1'/n) for n in ('wrappers-gold.bin','options-gold.bin')},asset_sha256={n:sha(REPO/'port/android-native/app/src/main/assets'/n) for n in assets},swf_sha256={n:sha(REPO/'.local-inputs/ui-layout-discovery'/n) for n in ('dqshared_droid.swf','dqhud_droid.swf','dqshared.swf','dqhud.swf')},sanitizers=['address','undefined(vptr excluded for core ABI)','leak'],sanitizer_findings=0,scope=__doc__,limits=dict(live_GPU=False,full_original_ActionScript_fork_parity=False,loaded_skill_profile_is_explicit_fixture=True,parseEx_integer_localization_skill_VCB_platform_scene_PlayerManager_are_fixtures=True,snapshot_compiler_provenance_claimed=False))
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
