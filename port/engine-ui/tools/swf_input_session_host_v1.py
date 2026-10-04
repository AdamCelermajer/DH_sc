"""Isolated retained-movie input/frame session build; never edits central CMake."""
import argparse,hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
UI=ROOT/'port/engine-ui'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--build-directory',type=Path,required=True);p.add_argument('--report',type=Path);a=p.parse_args()
 build=a.build_directory.resolve();assert build.is_relative_to(ROOT)
 build.mkdir(parents=True,exist_ok=True)
 sources=['swf_cursor_input','swf_input_geometry','swf_input_policy','swf_input_history','swf_input_connection','swf_input_connection_v2','swf_input_session_v1','swf_event_dispatch','swf_event_core','swf_viewport_connection','viewport','swf_frame_schedule','swf_frame_connection','swf_drag_values','swf_movie','swf_actionscript_connection','hud_sprite_core','hud_sprite_timeline']
 initial_paths=[x for x in UI.rglob('*')if x.is_file()and x.suffix in('.cpp','.hpp','.h','.inl','.cmake')]+[Path(__file__).resolve()]
 initial_sha256={str(x):sha(x)for x in initial_paths}
 cmake='''cmake_minimum_required(VERSION 3.22)
project(input_session LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off)
add_link_options(-fsanitize=address,undefined)
'''
 overlays=['gameswf_sources.cmake','gameswf_input_overlay_v1.cmake','gameswf_frame_overlay_v1.cmake','gameswf_player_lifetime_overlay_v1.cmake']
 for f in overlays:cmake+=f'include("{UI/f}")\n'
 cmake+='add_library(session_core SHARED ${DH2_GAMESWF_SOURCES}\n'+''.join(f' "{UI/(s+".cpp")}"\n'for s in sources)+')\n'
 cmake+=f'target_include_directories(session_core PUBLIC "{UI}" "${{DH2_GAMESWF_ROOT}}")\n'
 cmake+='target_compile_definitions(session_core PUBLIC TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0 TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)\n'
 cmake+='target_compile_options(session_core PRIVATE -w -fpermissive -fno-sanitize=vptr)\ntarget_link_libraries(session_core PUBLIC z ${CMAKE_DL_LIBS})\n'
 cmake+=f'add_executable(gold_audit "{UI/"tests/swf_cursor_input_gold.cpp"}" "{UI/"swf_cursor_input.cpp"}" "{UI/"swf_event_dispatch.cpp"}")\n'
 cmake+=f'target_include_directories(gold_audit PRIVATE "{UI}")\ntarget_link_options(gold_audit PRIVATE -Wl,--wrap=sinf -Wl,--wrap=cosf -Wl,--wrap=sincosf)\n'
 for target,test in [('session_audit','swf_input_session_v1'),('input_audit','swf_cursor_input'),('frame_audit','swf_frame_connection'),('frame_gold_audit','swf_frame_gold')]:
  cmake+=f'add_executable({target} "{UI/("tests/"+test+".cpp")}")\ntarget_link_libraries({target} PRIVATE session_core)\ntarget_compile_options({target} PRIVATE -fno-sanitize=vptr)\n'
 (build/'CMakeLists.txt').write_text(cmake)
 subprocess.run(['cmake','-S',str(build),'-B',str(build/'build')],check=True)
 subprocess.run(['cmake','--build',str(build/'build'),'-j','4'],check=True)
 env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1');results={}
 for target in ['session_audit','input_audit','frame_audit','frame_gold_audit','gold_audit']:
  command=[str(build/'build'/target)]
  if target=='gold_audit':command+=[str(UI/'reference/swf-native-input-frame-v1/input-gold.bin')]
  if target=='frame_gold_audit':command+=[str(UI/'reference/swf-native-input-frame-v1/frame-drag-gold.bin')]
  r=subprocess.run(command,capture_output=True,text=True,env=env)
  if r.returncode:print(r.stdout+r.stderr);r.check_returncode()
  expected="error: can't create a movie from 'missing'\n" if target=='session_audit' else ""
  assert r.stderr==expected,r.stderr
  print(r.stdout,end='');results[target]=json.loads(r.stdout);assert results[target]['validation']=='PASS'
 assert all(sha(Path(x))==h for x,h in initial_sha256.items()),'Compiler/source inputs changed during isolated proof'
 if a.report:
  assert not a.report.exists(),'Preserve prior proof'
  paths=[UI/(s+ext)for s in sources for ext in('.cpp','.hpp')if(UI/(s+ext)).exists()]
  paths+=[UI/f for f in overlays]+[UI/f'tests/{s}.cpp'for s in ['swf_input_session_v1','swf_cursor_input','swf_frame_connection','swf_frame_gold','swf_cursor_input_gold']]+[Path(__file__).resolve()]
  paths+=[UI/'overlays/player-lifetime-v1/gameswf_player.cpp']
  paths+=[p for p in(UI/'overlays/input-v1').glob('*.cpp')]+[p for p in(UI/'overlays/frame-v1').glob('*.cpp')]
  vendor=[p for p in sorted((UI/'vendor/gameswf1714').rglob('*'))if p.is_file()and p.suffix in('.cpp','.h','.inl')]
  freeze=UI/'reference/swf-native-input-frame-v1/freeze-manifest.json'
  a.report.parent.mkdir(parents=True,exist_ok=True)
  a.report.write_text(json.dumps(dict(validation='PASS',host_audit=results,source_sha256={str(x.relative_to(ROOT)):sha(x)for x in paths},vendor_source_sha256={str(x.relative_to(ROOT)):sha(x)for x in vendor},actual_core_library_sha256=sha(build/'build/libsession_core.so'),executable_sha256={t:sha(build/'build'/t)for t in results},original_kernel_evidence=dict(manifest=str(freeze.relative_to(ROOT)),sha256=sha(freeze)),sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],sanitizer_findings=0,source_input_frame_kernels_executed=True,whole_original_application_parity=False,live_Android=False),indent=2)+'\n')
if __name__=='__main__':main()
