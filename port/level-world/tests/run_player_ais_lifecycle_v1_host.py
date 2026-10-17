"""Borrowed Player lifecycle source over the actual selected dependency DSOs."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,subprocess,tempfile
from pathlib import Path

MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(command,env=None):
    result=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
    if result.returncode:raise RuntimeError(f'{command}\n{result.stdout}\n{result.stderr}')
    return result.stdout
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler',type=Path,required=True);parser.add_argument('--cache',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True);parser.add_argument('--original-elf',type=Path,required=True)
    parser.add_argument('--report',type=Path,default=MODULE/'reports/player-ais-lifecycle-v1-host-audit.json')
    args=parser.parse_args();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    compiler=args.compiler.resolve();cc=compiler.with_name(compiler.name.replace('g++','gcc'))
    cache=args.cache.resolve();elf=args.original_elf.resolve()
    from player_ais_lifecycle_v1_original import capture
    original=capture(elf,out/'original-constructor.json')
    cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    selected='player_ais_lifecycle_v1.cpp' in (MODULE/'CMakeLists.txt').read_text()
    source=MODULE/'player_ais_lifecycle_v1.cpp';test=MODULE/'tests/player_ais_lifecycle_v1.cpp'
    backend=ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp'
    names=ROOT/'port/pydata-names/names.c'
    sources=[test,backend,names]+([] if selected else [source])
    wrapper=out/'wrapper';wrapper.mkdir(exist_ok=True);build=out/'build'
    body=f'''cmake_minimum_required(VERSION 3.22)
project(player_ais_lifecycle_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(player_ais_lifecycle_audit {' '.join('"'+p.as_posix()+'"' for p in sources)})
target_compile_features(player_ais_lifecycle_audit PRIVATE cxx_std_17)
target_compile_options(player_ais_lifecycle_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
target_link_libraries(player_ais_lifecycle_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
    (wrapper/'CMakeLists.txt').write_text(body)
    configure=[cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={compiler}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1']
    logs=[run(configure)]
    commands=run([ninja,'-C',build,'-t','commands','player_ais_lifecycle_audit'])
    compiled_entries=[Path(row['file']).resolve() for row in json.loads((build/'compile_commands.json').read_text()) if str(row['file']).replace('\\','/') in commands.replace('\\','/')]
    compiled=set(compiled_entries)
    for name in ['character_script_lifecycle','character_script_selection','ais_external_initialization','ais_external_init_callbacks','ais_player_init_vcb','player_skill_session_v1','player_skill_update_session_v1','player_skill_use_session_v1','character_player_skills_preparation_v3','character_ai_initialization','character_ai_association','character_coordinator','character_level_runtime']:
        assert MODULE/(name+'.cpp') in compiled,f'selected dependency missing {name}'
    assert compiled_entries.count(source)==1 and sum(p.name=='script_runtime.c' for p in compiled_entries)==1
    if selected:assert 'selected-world/CMakeFiles/dh2_level_world.dir/player_ais_lifecycle_v1.cpp.obj' in commands.replace('\\','/')
    paths=set(compiled)|{source,source.with_suffix('.hpp'),test,test.with_name('player_skill_session_v1.cpp'),Path(__file__).resolve(),MODULE/'tests/player_ais_lifecycle_v1_original.py',MODULE/'CMakeLists.txt'}
    for folder in {p.parent for p in compiled}:paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'))
    before={p.relative_to(ROOT).as_posix():sha(p) for p in paths if p.is_relative_to(ROOT)}
    build_command=[cmake,'--build',build,'--target','player_ais_lifecycle_audit','--parallel','2'];logs.append(run(build_command))
    dlls=list(build.rglob('*.dll'));executable=build/'player_ais_lifecycle_audit.exe'
    env=os.environ.copy();env['PATH']=os.pathsep.join([str(compiler.parent),*(str(p.parent) for p in dlls),env.get('PATH','')])
    host=json.loads(run([executable,cache,out/'runtime'],env))
    assert host['validation']=='PASS' and host['completed_lifecycles']==5 and host['failure_prefix_cases']==4 and host['split_phase_cases']==2 and host['guard_cases']==7
    split=original['original_split_load_init']
    assert not split['load_performs_init_process'] and split['repeated_load_no_services'] and split['load_and_init_active_guard_no_services']
    assert split['constructor_calls']==split['vitals_calls']==split['configure_calls']==1
    assert host['constructor_scalar_words']==original['constructor_scalar_words'] and host['constructor_tree_empty']==original['tree_empty']
    assert before=={name:sha(ROOT/name) for name in before},'compiled source changed during gate'
    imports={p.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([compiler.with_name('objdump.exe'),'-p',p])) for p in [executable,*dlls]}
    exe_imports=imports[executable.relative_to(out).as_posix()]
    assert 'libdh2_level_world.dll' in exe_imports and 'libdh2_script_runtime.dll' in exe_imports
    assert len([p for p in dlls if p.name=='libdh2_script_runtime.dll'])==1
    report={'validation':'PASS','host_report':host,'original_capture':original,'source_sha256':before,'selected_existing_dependencies':True,'new_module_selected':selected,'scoped_source_tu_count':compiled_entries.count(source),'script_runtime_tu_count':sum(p.name=='script_runtime.c' for p in compiled_entries),'selected_native_build':False,'live_gameplay':False,
        'commands':[list(map(str,configure)),list(map(str,build_command))],'selected_commands':commands,'build_stdout':logs,'wrapper_cmake':body,
        'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [executable,*dlls]},'dso_imports':imports,
        'scope':'Actual existing selected level-world/VM dependencies with scoped new Player composition source. Real Character construction/association, same VM, actual cache Player declarations/classes/ticks, Debug-backed HP/MP, real preparation and update/use owners. Separate source LoadScriptProcess and InitScriptProcess expose the real publication boundary; no profile/equipment/initial-grant bodies are claimed. Skill update bodies use explicit test overlays to isolate lifecycle choreography; full native gameplay callbacks are a separate gate. Original constructor instruction capture executes source CharAIScript, empty vector and inline PlayerIPhone stores, with Lua construction/allocation as explicit service boundaries.'}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(host));print('new module selected:',selected)
if __name__=='__main__':main()
