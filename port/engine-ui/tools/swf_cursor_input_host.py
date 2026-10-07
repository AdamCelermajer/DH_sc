"""Build isolated actual GameSWF input overlay and replay under sanitizers (WSL)."""
import argparse, json, subprocess, hashlib, os
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
UI=ROOT/'port/engine-ui'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--build-directory',type=Path,required=True)
 p.add_argument('--configure-only',action='store_true')
 p.add_argument('--report',type=Path)
 p.add_argument('--gold',type=Path)
 p.add_argument('--frame-gold',type=Path)
 a=p.parse_args();build=a.build_directory.resolve()
 assert build.is_relative_to(ROOT), 'Build only inside workspace'
 build.mkdir(parents=True,exist_ok=True)
 sources=['swf_cursor_input','swf_input_geometry','swf_input_policy','swf_input_history','swf_input_connection','swf_event_dispatch','swf_event_core','swf_viewport_connection','viewport','swf_frame_schedule','swf_frame_connection','swf_drag_values']
 cmake='''cmake_minimum_required(VERSION 3.22)
project(input_audit LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off)
add_link_options(-fsanitize=address,undefined)
'''
 for f in ['gameswf_sources.cmake','gameswf_input_overlay_v1.cmake','gameswf_frame_overlay_v1.cmake']:cmake+=f'include("{UI/f}")\n'
 cmake+='add_library(input_core SHARED ${DH2_GAMESWF_SOURCES}\n'+''.join(f' "{UI/(s+".cpp")}"\n'for s in sources)+')\n'
 cmake+=f'target_include_directories(input_core PUBLIC "{UI}" "${{DH2_GAMESWF_ROOT}}")\n'
 cmake+='target_compile_definitions(input_core PUBLIC TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0 TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)\n'
 cmake+='target_compile_options(input_core PRIVATE -w -fpermissive -fno-sanitize=vptr)\ntarget_link_libraries(input_core PUBLIC z ${CMAKE_DL_LIBS})\n'
 cmake+=f'add_executable(audit "{UI/"tests/swf_cursor_input.cpp"}")\ntarget_link_libraries(audit PRIVATE input_core)\ntarget_compile_options(audit PRIVATE -fno-sanitize=vptr)\n'
 cmake+=f'add_executable(frame_audit "{UI/"tests/swf_frame_connection.cpp"}")\ntarget_link_libraries(frame_audit PRIVATE input_core)\ntarget_compile_options(frame_audit PRIVATE -fno-sanitize=vptr)\n'
 cmake+=f'add_executable(frame_gold_audit "{UI/"tests/swf_frame_gold.cpp"}")\ntarget_link_libraries(frame_gold_audit PRIVATE input_core)\ntarget_compile_options(frame_gold_audit PRIVATE -fno-sanitize=vptr)\n'
 cmake+=f'add_executable(gold_audit "{UI/"tests/swf_cursor_input_gold.cpp"}" "{UI/"swf_cursor_input.cpp"}" "{UI/"swf_event_dispatch.cpp"}")\ntarget_include_directories(gold_audit PRIVATE "{UI}")\ntarget_link_options(gold_audit PRIVATE -Wl,--wrap=sinf -Wl,--wrap=cosf -Wl,--wrap=sincosf)\n'
 (build/'CMakeLists.txt').write_text(cmake)
 subprocess.run(['cmake','-S',str(build),'-B',str(build/'build')],check=True)
 subprocess.run(['cmake','--build',str(build/'build'),'-j','4','--target','input_core'if a.configure_only else'audit'],check=True)
 if a.configure_only:return
 exe=build/'build/audit';lib=build/'build/libinput_core.so'
 env=dict(os.environ,ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1')
 result=subprocess.run([str(exe)],check=True,capture_output=True,text=True,env=env)
 print(result.stdout,end='');assert not result.stderr,result.stderr
 audit=json.loads(result.stdout);assert audit['validation']=='PASS'
 subprocess.run(['cmake','--build',str(build/'build'),'-j','4','--target','frame_audit'],check=True)
 frame=subprocess.run([str(build/'build/frame_audit')],capture_output=True,text=True,env=env)
 if frame.returncode:print(frame.stderr);frame.check_returncode()
 assert not frame.stderr,frame.stderr
 audit['actual_core_scheduler']=json.loads(frame.stdout);print(frame.stdout,end='')
 if a.gold:
  subprocess.run(['cmake','--build',str(build/'build'),'-j','4','--target','gold_audit'],check=True)
  replay=subprocess.run([str(build/'build/gold_audit'),str(a.gold.resolve())],capture_output=True,text=True,env=env)
  if replay.returncode:print(replay.stderr);replay.check_returncode()
  assert not replay.stderr,replay.stderr
  audit['original_gold_replay']=json.loads(replay.stdout)
  print(replay.stdout,end='')
 if a.frame_gold:
  subprocess.run(['cmake','--build',str(build/'build'),'-j','4','--target','frame_gold_audit'],check=True)
  replay=subprocess.run([str(build/'build/frame_gold_audit'),str(a.frame_gold.resolve())],capture_output=True,text=True,env=env)
  if replay.returncode:print(replay.stderr);replay.check_returncode()
  assert not replay.stderr,replay.stderr
  audit['original_frame_drag_gold_replay']=json.loads(replay.stdout);print(replay.stdout,end='')
 if a.report:
  assert not a.report.exists(),'Preserve historical proof'
  a.report.parent.mkdir(parents=True,exist_ok=True)
  paths=[UI/(s+ext)for s in sources for ext in('.hpp','.cpp')if (UI/(s+ext)).exists()]
  paths +=[UI/f for f in ['tests/swf_cursor_input.cpp','tests/swf_cursor_input_gold.cpp','tests/swf_frame_connection.cpp','tests/swf_frame_gold.cpp','tools/export_swf_frame_gold.py','tools/export_swf_input_gold.py','gameswf_sources.cmake','gameswf_input_overlay_v1.cmake','gameswf_frame_overlay_v1.cmake','overlays/input-v1/gameswf_character.cpp','overlays/input-v1/gameswf_object.cpp','overlays/frame-v1/gameswf_sprite.cpp','overlays/frame-v1/gameswf_root.cpp','overlays/frame-v1/gameswf_button.cpp']]+[Path(__file__).resolve()]
  vendor=[x for x in sorted((UI/'vendor/gameswf1714').rglob('*'))if x.is_file()and x.suffix in('.h','.cpp','.inl')]
  a.report.write_text(json.dumps(dict(validation='PASS',host_audit=audit,source_sha256={str(x.relative_to(ROOT)):sha(x)for x in paths},vendor_source_sha256={str(x.relative_to(ROOT)):sha(x)for x in vendor},actual_core_library_sha256=sha(lib),executable_sha256={name:sha(build/'build'/name)for name in ['audit','frame_audit','gold_audit','frame_gold_audit']if(build/'build'/name).exists()},gold_sha256={str(x.resolve().relative_to(ROOT)):sha(x)for x in [a.gold,a.frame_gold]if x},sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],sanitizer_findings=0,whole_original_advance_parity=False,live_Android=False),indent=2)+'\n')
if __name__=='__main__':main()
