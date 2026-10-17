"""Selected-library Player session: actual source assets, Debug IO and Lua."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
    if p.returncode:raise RuntimeError(f'{command}\n{p.stdout}\n{p.stderr}')
    return p.stdout
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',required=True);p.add_argument('--cache',type=Path,required=True);p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--report',type=Path,default=MODULE/'build/player-skill-session-v1/validation.json');a=p.parse_args()
    out=a.output.resolve();wrapper=out/'wrapper';build=out/'selected';wrapper.mkdir(parents=True,exist_ok=True);build.mkdir(exist_ok=True)
    cxx=Path(a.compiler).resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    test=MODULE/'tests/player_skill_session_v1.cpp';backend=ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp'
    text=f'''cmake_minimum_required(VERSION 3.22)
project(player_session_selected_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(player_session_audit "{test.as_posix()}" "{backend.as_posix()}" "{(ROOT/'port/pydata-names/names.c').as_posix()}")
target_compile_features(player_session_audit PRIVATE cxx_std_17)
target_compile_options(player_session_audit PRIVATE -Wall -Wextra -Werror -pedantic -fno-fast-math -ffp-contract=off)
target_link_libraries(player_session_audit PRIVATE dh2_level_world dh2_script_runtime)
'''
    (wrapper/'CMakeLists.txt').write_text(text,encoding='utf-8');commands=[];logs=[]
    config=[cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'];commands.append(list(map(str,config)));logs.append(run(config))
    selected=run([ninja,'-C',build,'-t','commands','player_session_audit'])
    compilation=json.loads((build/'compile_commands.json').read_text());sources={Path(r['file']).resolve() for r in compilation if str(r['file']).replace('\\','/') in selected.replace('\\','/')}
    assert MODULE/'player_skill_session_v1.cpp' in sources,'Parent must select actual source in level-world CMake'
    assert ROOT/'port/adam-script-runtime/script_runtime.c' in sources
    assert not any(x.name=='player_skill_vm_services.cpp' for x in sources),'Paused incorrect draft must remain unselected'
    manifests=[MODULE/'reference'/part/'original-functions.json' for part in ('ais-player-init-vcb','ais-native-bindings','character-native-bindings','player-skills-preparation-v3','lua-script-load-once')]
    sys.path.insert(0,str(MODULE/'tests'));from run_character_ai_set_skills_and_spells_host import source_symbols
    for manifest in manifests:source_symbols(a.original_elf.resolve(),json.loads(manifest.read_text(encoding='utf-8')))
    paths=sources|set(manifests)|{Path(__file__).resolve(),MODULE/'tests/run_character_ai_set_skills_and_spells_host.py',ROOT/'port/pydata-names/corpus-validation.json',ROOT/'port/pydata-names/struct-names.h'}
    for folder in {x.parent for x in sources}-{MODULE/'tests'}:
        paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'))
        if (folder/'CMakeLists.txt').exists():paths.add(folder/'CMakeLists.txt')
    before={path.relative_to(ROOT).as_posix():sha(path) for path in paths if path.is_relative_to(ROOT)}
    cache=a.cache.resolve();inputs={}
    for pattern in ('data/pydata/skills_*.bin','data/pydata/faeries_*.bin','data/pydata/character_properties_*.bin','data/pydata/character_classes_*.bin','data/pydata/effects_pyarraynames.bin','data/pydata/projectiles_pyarraynames.bin','data/pydata/design_pycst.bin','data/pydata/ai_pycst.bin','DebugSwitches.savegame'):
        for file in cache.glob(pattern):inputs[file.relative_to(cache).as_posix()]=sha(file)
    # Replay unchanged actual assets; distinguish generated protocol fixtures in
    # the C++ test from these copied recovered files. No assets copied into Git.
    names=set()
    prior=json.loads((MODULE/'build/player-skills-preparation-v3-final/validation.json').read_text())
    for row in prior['host_report']['cases']:
        if row['scenario']=='normal':names.update(x[0] for x in row['instances'])
    assert len(names)==29
    scripts=['data/scripts/ai/_commons.luac','data/scripts/skills/_commons.luac']+[f'data/scripts/skills/{n}.luac' for n in sorted(names)]
    for name in scripts:
        file=cache/name;assert sha(file)==sha(ROOT/'recovered/scripts/original'/name),name
        inputs[name]=sha(file)
    script_paths=[{'path':name,'sha256':inputs[name]} for name in scripts]
    command=[cmake,'--build',build,'--target','player_session_audit','--parallel','1'];commands.append(list(map(str,command)));logs.append(run(command))
    assert all(sha(ROOT/n)==h for n,h in before.items()),'Selected source changed during build'
    dsos=sorted(build.rglob('*.dll'));exe=build/'player_session_audit.exe';env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(x.parent) for x in dsos),env.get('PATH','')])
    imported={x.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([cxx.with_name('objdump.exe'),'-p',x])) for x in [exe,*dsos]}
    world=next(x for x in dsos if x.name=='libdh2_level_world.dll');vm=next(x for x in dsos if x.name=='libdh2_script_runtime.dll')
    assert sum(x.name==vm.name for x in dsos)==1
    assert world.name in imported[exe.relative_to(out).as_posix()] and vm.name in imported[exe.relative_to(out).as_posix()] and vm.name in imported[world.relative_to(out).as_posix()]
    host=json.loads(run([exe,cache,out/'real-debug-files'],env));assert host['validation']=='PASS' and host['class_cases']==3 and host['registered_class_slots']==24 and host['registered_faery_slots']==15 and host['unchanged_script_files']==31
    assert host['player_combat_same_vm'] and host['player_combat_userdata_order']
    assert all(sha(ROOT/n)==h for n,h in before.items()),'Selected source changed during replay'
    assert all(sha(cache/n)==h for n,h in inputs.items()),'Original cache changed during replay'
    report={'validation':'PASS','host_report':host,'attribution':{'upstream_commit':'c3ae797332a82a30a586b9156cddc25445e36a4c','reused_glue':['character_script_owner_v2.cpp','character_script_session_v3.cpp'],'adaptation':'One current selected VM; current source registration/preparation/load-once/Player VCB, borrowed native providers. No whole upstream Session owner copied.'},'new_complete_original_bodies':0,'original_sha256':sha(a.original_elf),'source_sha256':before,'cache_inputs_sha256':inputs,'cache_path':str(cache),'selected_commands':selected,'selected_dso_imports':imported,'binary_sha256':{x.relative_to(out).as_posix():sha(x) for x in [exe,*dsos]},'compiler_commands':commands,'compiler_version':run([cxx,'--version']).splitlines()[0],'build_stdout':logs,'wrapper_cmake':text,'wrapper_sha256':sha(wrapper/'CMakeLists.txt'),'scope':'Port session composition only. Actual three-class preparation/SetSkill/authored passive checks on unchanged scripts. Debug file/backend and source map runtime are real; host OID/struct queries use source-backed name kernels; property queries use maintained live PropertyView. Native GetCurrentSkillInfo/save/buff and OnSkillUpdate completion remain explicit required failures; no live Player activation or original method Binder/lifecycle body claim.'}
    report['original_script_paths']=script_paths
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps(host))
if __name__=='__main__':main()
