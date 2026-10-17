"""Selected level-world resistance/state oracle plus same-VM source Lua replay."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,subprocess
from pathlib import Path
from character_player_buffs_v1_original import generate,ELF_SHA

MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
MANIFEST=MODULE/'reference/character-player-buffs-v1/original-functions.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
    if p.returncode:raise RuntimeError(f'{command}\n{p.stdout}\n{p.stderr}')
    return p.stdout
def check_original(elf,manifest):
    from elftools.elf.elffile import ELFFile
    assert sha(elf)==ELF_SHA==manifest['original_sha256']
    blob=elf.read_bytes()
    with elf.open('rb') as f:
        e=ELFFile(f);sy={s.name:s for s in e.get_section_by_name('.symtab').iter_symbols()}
        for row in manifest['functions']:
            address,size=int(row['elf_address'],0),row['size'];s=sy[row['original_symbol']]
            assert (int(s['st_value']),int(s['st_size']))==(address,size)
            g=next(g for g in e.iter_segments() if g['p_type']=='PT_LOAD' and int(g['p_vaddr'])<=address and address+size<=int(g['p_vaddr'])+int(g['p_filesz']))
            offset=int(g['p_offset'])+address-int(g['p_vaddr'])
            assert hashlib.sha256(blob[offset:offset+size]).hexdigest()==row['sha256']

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--compiler',required=True);p.add_argument('--cache',type=Path,required=True)
    p.add_argument('--original-elf',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--report',type=Path)
    p.add_argument('--isolated',action='store_true',help='explicit direct-source diagnostic; not selected runtime proof')
    a=p.parse_args()
    report_path=a.report or MODULE/'reports'/('character-player-buffs-v1-host-audit.json' if a.isolated else 'character-player-buffs-v1-selected-host-audit.json')
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=True);cache=a.cache.resolve();elf=a.original_elf.resolve()
    manifest=json.loads(MANIFEST.read_text());check_original(elf,manifest)
    sources=[MODULE/'character_player_buffs_v1.cpp',MODULE/'character_coordinator.cpp',MODULE/'character_state.cpp',MODULE/'character_timers.cpp',ROOT/'port/game-data/properties.cpp',ROOT/'port/game-data/class_tables.cpp',ROOT/'port/game-data/data.cpp',ROOT/'port/pydata-constants/constants.c',MODULE/'tests/character_player_buffs_v1.cpp']
    frozen_fixture=MODULE/'reference/character-player-buffs-v1/original-resistance-cases.bin'
    paths=set(sources)|{Path(__file__).resolve(),MODULE/'tests/character_player_buffs_v1_original.py',MANIFEST,frozen_fixture,MANIFEST.with_name('NOTES.md')}
    for folder in {x.parent for x in sources}-{MODULE/'tests'}:
        paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'))
    vm=ROOT/'port/adam-script-runtime';paths.update(vm.glob('*.c'));paths.update(vm.glob('*.cpp'));paths.update(vm.glob('*.h'));paths.add(vm/'CMakeLists.txt');paths.update((vm/'lua').glob('*.c'));paths.update((vm/'lua').glob('*.h'))
    for part in ('classes','properties','vitals'):paths.add(ROOT/'port/game-data/reference'/part/'original-functions.json')
    before={x.relative_to(ROOT).as_posix():sha(x) for x in paths}
    inputs={x.relative_to(cache).as_posix():sha(x) for stem in ('character_classes','character_properties') for x in (cache/'data/pydata').glob(stem+'_*.bin')}
    script=cache/'data/scripts/skills/faerie_celest.luac';inputs[script.relative_to(cache).as_posix()]=sha(script)
    constants=cache/'data/pydata/design_pycst.bin';inputs[constants.relative_to(cache).as_posix()]=sha(constants)
    assert sha(script)==sha(ROOT/'recovered/scripts/original/data/scripts/skills/faerie_celest.luac')
    original=generate(elf,cache,out/'original-cases.bin',manifest)
    assert original['fixture_sha256']==sha(frozen_fixture),'regenerated original fixture differs from frozen source fixture'
    cxx=Path(a.compiler).resolve();cc=cxx.with_name(cxx.name.replace('g++','gcc'))
    cmake=shutil.which('cmake');ninja=shutil.which('ninja');wrapper=out/'wrapper';build=out/'build';wrapper.mkdir(exist_ok=True)
    text='''cmake_minimum_required(VERSION 3.22)
project(player_buffs_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
'''
    if a.isolated:
        text+=f'add_subdirectory("{vm.as_posix()}" adam-script-runtime)\nadd_executable(player_buffs_audit '+' '.join('"'+x.as_posix()+'"' for x in sources)+')\n'
    else:
        text+=f'add_subdirectory("{MODULE.as_posix()}" selected-world)\nadd_executable(player_buffs_audit "{(MODULE/"tests/character_player_buffs_v1.cpp").as_posix()}")\ntarget_link_libraries(player_buffs_audit PRIVATE dh2_level_world)\n'
    text+='''target_compile_features(player_buffs_audit PRIVATE cxx_std_17)
target_compile_options(player_buffs_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
target_link_libraries(player_buffs_audit PRIVATE dh2_script_runtime)
'''
    (wrapper/'CMakeLists.txt').write_text(text,encoding='utf-8')
    command=[cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={cxx}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1']
    logs=[run(command)]
    selected=run([ninja,'-C',build,'-t','commands','player_buffs_audit'])
    compilation=json.loads((build/'compile_commands.json').read_text())
    selected_sources={Path(r['file']).resolve() for r in compilation if str(r['file']).replace('\\','/') in selected.replace('\\','/')}
    if not a.isolated:
        for source in sources[:-1]:assert source in selected_sources,f'required source absent from selected level-world dependencies: {source}'
        assert sum(x==vm/'script_runtime.c' for x in selected_sources)==1,'actual Lua VM core must be selected exactly once'
        assert not any(x.name=='character_buffs.cpp' for x in selected_sources),'second buff owner selected'
        paths.update(selected_sources)
        for folder in {x.parent for x in selected_sources}:
            paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'))
            if (folder/'CMakeLists.txt').exists():paths.add(folder/'CMakeLists.txt')
        paths.add(MODULE/'CMakeLists.txt');paths.add(ROOT/'port/game-data/CMakeLists.txt')
        before={x.relative_to(ROOT).as_posix():sha(x) for x in paths if x.is_relative_to(ROOT)}
    compile_command=[cmake,'--build',build,'--target','player_buffs_audit','--parallel','2'];logs.append(run(compile_command))
    dlls=list(build.rglob('*.dll'));assert len([x for x in dlls if x.name=='libdh2_script_runtime.dll'])==1
    env=os.environ.copy();env['PATH']=os.pathsep.join([str(cxx.parent),*(str(x.parent) for x in dlls),env.get('PATH','')])
    executable=build/'player_buffs_audit.exe';host=json.loads(run([executable,cache,out/'original-cases.bin'],env))
    assert host['validation']=='PASS' and host['original_whole_state_cases']==40 and host['unchanged_celest_same_vm_cases']==40
    assert before=={n:sha(ROOT/n) for n in before} and inputs=={n:sha(cache/n) for n in inputs}
    imports={x.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([cxx.with_name('objdump.exe'),'-p',x])) for x in [executable,*dlls]}
    if not a.isolated:
        world=next(x for x in dlls if x.name=='libdh2_level_world.dll')
        assert world.name in imports[executable.relative_to(out).as_posix()]
        assert 'libdh2_script_runtime.dll' in imports[executable.relative_to(out).as_posix()] and 'libdh2_script_runtime.dll' in imports[world.relative_to(out).as_posix()]
    host['selected_level_world_dependencies']=not a.isolated
    report={'validation':'PASS','host_report':host,'original_sha256':ELF_SHA,'original_ranges_verified':len(manifest['functions']),
            'original_oracle':original,'attribution':manifest['attribution'],'source_sha256':before,'cache_inputs_sha256':inputs,
            'binary_sha256':{x.relative_to(out).as_posix():sha(x) for x in [executable,*dlls]},
            'compiler_commands':[list(map(str,command)),list(map(str,compile_command))],'compiler_version':run([cxx,'--version']).splitlines()[0],
            'build_stdout':logs,'wrapper_cmake':text,'wrapper_sha256':sha(wrapper/'CMakeLists.txt'),'selected_commands':selected,'dso_imports':imports,
            'selected_level_world_dependencies':not a.isolated,'selected_android_native_build':False,'live_gameplay':False,
            'scope':('Explicit isolated source diagnostic. ' if a.isolated else 'Actual selected dh2_level_world dependencies and shared existing Lua target. ')+ 'One native buff group owner, current PropertyView/ClassTables/Coordinator, existing Adam Lua core exactly once. 40 original full ApplyBuff identity callbacks run all nested class, property and map/deque instructions, including real player rows and base ClassID=-1. Host owner add/apply/remove compared over all 224 words in every property sheet plus the owned buff sheet. 40 unchanged Celest script updates use these same native buff callbacks on one retained real Lua VM; fixture prerequisite helpers and selected-level/element inputs are external test providers. Existing frozen faery wrapper/save integration, selected Android build and actual live gameplay are separate root-owned gates.',
            'limits':['Original AddBuff/DelBuff/allocator/timer/FX STL bodies are byte pinned, not executed by this oracle.','Reached FX calls require actual external provider except source null release no-op.','Value string/identity/table numeric coercions and nonfinite numbers fail explicitly.','Allocator/STL failure instruction equivalence is external; completed native effects are retained.','Caller ends timer delivery and closes its VM before retire; all borrowed backing must outlive callbacks.','Selected host linkage is separate from selected Android/native callback wiring and live gameplay.']}
    report_path.parent.mkdir(parents=True,exist_ok=True);report_path.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps(host))
if __name__=='__main__':main()
