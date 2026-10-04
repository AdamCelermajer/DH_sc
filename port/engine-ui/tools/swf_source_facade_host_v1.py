"""Isolated source facade/core migration against unchanged central UI executables."""
import argparse,ast,hashlib,json,os,re,shutil,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];UI=ROOT/'port/engine-ui'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--build-directory',type=Path,required=True);p.add_argument('--central-build',type=Path,required=True);p.add_argument('--report',type=Path,required=True);a=p.parse_args()
 out=a.build_directory.resolve();assert out.is_relative_to(ROOT)and not a.report.exists();out.mkdir(parents=True,exist_ok=True)
 libs=sorted(a.central_build.rglob('*.so'));assert len(libs)==7
 before={str(x):sha(x)for x in libs};snapshot=out/'dependencies';snapshot.mkdir(exist_ok=True)
 for lib in libs:shutil.copyfile(lib,snapshot/lib.name)
 assert all(sha(Path(x))==h for x,h in before.items()),'Central libraries changed during snapshot'
 cmake=(UI/'CMakeLists.txt').read_text();block=re.search(r'add_library\(dh2_engine_ui SHARED (.*?)\)',cmake,re.S).group(1)
 sources=[UI/s for s in block.split()];sources.remove(UI/'swf_movie.cpp');sources.append(UI/'overlays/source-facade-v1/swf_movie.cpp')
 sources += [UI/(s+'.cpp')for s in ['swf_cursor_input','swf_input_geometry','swf_input_policy','swf_input_history','swf_input_connection','swf_input_connection_v2','swf_event_dispatch','swf_event_core','swf_frame_schedule','swf_frame_connection','swf_drag_values','swf_source_startup_v1']]
 sources += [UI/'overlays/source-facade-v1'/(s+'.cpp')for s in ['swf_source_movie','swf_input_session_v1','swf_input_session_v2']]
 sources += [ROOT/'port/android-native/app/src/main/cpp/original_ui_input_session_v1.cpp']
 recipes=['gameswf_sources.cmake','gameswf_font_overlay_v1.cmake','gameswf_input_overlay_v1.cmake','gameswf_frame_overlay_v1.cmake','gameswf_player_lifetime_overlay_v1.cmake','gameswf_loader_lifetime_overlay_v1.cmake','freetype237-hud.cmake']
 tracked=[UI/'tests/overlays/source-facade-v1/swf_viewport_connection.cpp']+sources+[UI/s for s in recipes]+[Path(__file__)]
 tracked += [x for x in UI.rglob('*')if x.is_file()and x.suffix in('.hpp','.h','.inl')]
 tracked += [x for folder in ['vendor/gameswf1714','vendor/freetype-2.3.7-hud','overlays']for x in(UI/folder).rglob('*')if x.is_file()and x.suffix in('.cpp','.c')]
 frozen={str(x):sha(x)for x in tracked}
 text='''cmake_minimum_required(VERSION 3.22)
project(source_facade LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-fast-math -ffp-contract=off)
add_link_options(-fsanitize=address,undefined)
'''
 for recipe in recipes:text+=f'include("{UI/recipe}")\n'
 text+='add_library(dh2_engine_ui SHARED ${DH2_GAMESWF_SOURCES}\n'+''.join(f' "{s}"\n'for s in sources)+')\n'
 text+=f'target_include_directories(dh2_engine_ui PUBLIC "{UI}" "${{DH2_GAMESWF_ROOT}}" "{ROOT/"port/android-native/app/src/main/cpp"}")\n'
 text+='target_compile_definitions(dh2_engine_ui PUBLIC TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0 TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0)\n'
 text+='target_compile_options(dh2_engine_ui PRIVATE -w -fpermissive -fno-sanitize=vptr)\ntarget_link_libraries(dh2_engine_ui PUBLIC dh2_freetype237 z ${CMAKE_DL_LIBS})\n'
 for target,test in [('session_v1','swf_input_session_v1'),('session_v2','swf_source_session_v2'),('viewport_source','overlays/source-facade-v1/swf_viewport_connection')]:text+=f'add_executable({target} "{UI/"tests"/(test+".cpp")}")\ntarget_link_libraries({target} PRIVATE dh2_engine_ui)\ntarget_compile_options({target} PRIVATE -fno-sanitize=vptr)\n'
 (out/'CMakeLists.txt').write_text(text);subprocess.run(['cmake','-S',str(out),'-B',str(out/'build')],check=True);subprocess.run(['cmake','--build',str(out/'build'),'-j','4'],check=True)
 # Reuse the actual central runner's UI command definitions, not rewritten
 # frozen tests. Only their private writable diagnostic files are relocated.
 runner=ROOT/'port/level-world/tests/character_initialization_main_host.py';source=runner.read_text();tree=ast.parse(source)
 fn=next(n for n in tree.body if isinstance(n,ast.FunctionDef)and n.name=='main')
 first=next(n.lineno for n in fn.body if isinstance(n,ast.Assign)and any(isinstance(t,ast.Subscript)and ast.unparse(t)=="suites['GFNT']"for t in n.targets))
 last=next(n.lineno for n in fn.body if isinstance(n,ast.Assign)and any(isinstance(t,ast.Subscript)and ast.unparse(t)=="suites['Player_savegame_v1']"for t in n.targets))
 env=dict(ROOT=ROOT,ui=UI,world=ROOT/'port/level-world',scene=ROOT/'port/scene-materials',data=ROOT/'port/game-data',assets=ROOT/'port/android-native/app/src/main/assets',suites={})
 env['scene']=ROOT/'port/scene-materials'
 for n in fn.body:
  if first<=n.lineno<last:exec(compile(ast.Module(body=[n],type_ignores=[]),str(runner),'exec'),env)
 suites={n:v for n,v in env['suites'].items()if v[0].startswith('engine-ui/')}
 private_files=out/'files';private_files.mkdir(exist_ok=True)
 # Keep every fixture read path exact. Relocate only explicitly writable host
 # outputs which are not part of the authored resources or source gold.
 for name,(target,args)in suites.items():
  if name in ['Freetype_font','Freetype_HUD']:args[-1]=private_files/(name+'.pgm')
  if name=='Owned_HUD_settings':args[-1]=private_files/'settings';assert not args[-1].exists()
  if name=='Localization_connection':args[-1]=private_files/'debug';args[-1].mkdir(exist_ok=True)
 runtime_env=dict(os.environ,LD_LIBRARY_PATH=str(out/'build')+':'+str(snapshot),ASAN_OPTIONS='detect_leaks=1:halt_on_error=1',UBSAN_OPTIONS='halt_on_error=1')
 results={};executables={};dependencies={}
 runs=[('session_v1',out/'build/session_v1',[]),('session_v2',out/'build/session_v2',[])]+[(n,a.central_build/t,args)for n,(t,args)in suites.items()]
 for name,exe,args in runs:
  if name=='Swf_viewport_connection':exe=out/'build/viewport_source'
  dependencies[name]=subprocess.check_output(['ldd',str(exe)],text=True,env=runtime_env)
  assert str(out/'build/libdh2_engine_ui.so')in dependencies[name]and'not found'not in dependencies[name],dependencies[name]
  r=subprocess.run([str(exe)]+[str(x)for x in args],text=True,capture_output=True,env=runtime_env)
  if r.returncode:print(name,r.stdout,r.stderr);r.check_returncode()
  expected="error: can't create a movie from 'missing'\n"if name=='session_v1'else'Unresolved genuine font: Arial\n'if name=='Freetype_HUD'else'wqy P: size=12,mode=1,width=5,rows=8,pitch=1\nsource localization: World Map\n'if name in['HUD_freetype_provider','Gameswf_font_overlay']else'wqy P: size=12,mode=1,width=5,rows=8,pitch=1\nsource localization: World Map\nsource localization: World Map\n'if name=='Player_status_HUD'else''
  assert r.stderr==expected,(name,r.stderr)
  result=json.loads(r.stdout);assert result.get('validation','PASS')=='PASS',name
  assert result.get('mismatches',0)==0,name
  results[name]=result;executables[name]=dict(path=str(exe),sha256=sha(exe));print(name,'PASS')
 assert all(sha(Path(x))==h for x,h in frozen.items()),'Compiled sources changed'
 assert all(sha(Path(x))==h for x,h in before.items()),'Central libraries changed during audit'
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(dict(validation='PASS',source_sha256={str(Path(p).relative_to(ROOT)):h for p,h in frozen.items()},actual_ui_library_sha256=sha(out/'build/libdh2_engine_ui.so'),borrowed_dependency_sha256={str(p):h for p,h in before.items()},executables=executables,host_audits=results,ldd=dependencies,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],sanitizer_findings=0,frozen_tests_edited=False,central_build_edited=False,whole_original_application_parity=False,live_Android=False),indent=2)+'\n')
 print(json.dumps(dict(validation='PASS',legacy_suites=len(suites),session_suites=2,sanitizer_findings=0)))
if __name__=='__main__':main()
