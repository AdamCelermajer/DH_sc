"""Original locality callers against the actual selected shared world library."""
from __future__ import annotations
import argparse,json,os,re,shutil,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1];WORLD=ROOT/'port/level-world'
sys.path.insert(0,str(WORLD/'tests'))
from run_player_skill_cleanup_session_v1_host import sha,run,selected_entries,actual_dependencies,hashes
from player_locality_v1_original import capture
def main():
 parser=argparse.ArgumentParser(description=__doc__)
 for name in ['compiler','original-elf','output']:parser.add_argument('--'+name,required=True,type=Path)
 parser.add_argument('--report',type=Path,default=MODULE/'reports/player-locality-v1-selected-host.json');a=parser.parse_args()
 out=a.output.resolve();wrapper=out/'wrapper';build=out/'build';wrapper.mkdir(parents=True,exist_ok=True)
 compiler=a.compiler.resolve();cc=compiler.with_name(compiler.name.replace('g++','gcc'));cmake=shutil.which('cmake');ninja=shutil.which('ninja')
 source=MODULE/'player_locality_v1.cpp';test=MODULE/'tests/player_locality_v1.cpp';reference=MODULE/'reference/player-locality-v1'
 assert source.name in (WORLD/'CMakeLists.txt').read_text(),'Root must centrally select source'
 rows,original,summary=capture(a.original_elf.resolve())
 inputs=out/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n')
 body=f'''cmake_minimum_required(VERSION 3.22)
project(player_locality_selected LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{WORLD.as_posix()}" selected-world)
add_executable(player_locality_audit "{test.as_posix()}")
target_include_directories(player_locality_audit PRIVATE "{MODULE.as_posix()}")
target_compile_features(player_locality_audit PRIVATE cxx_std_17)
target_compile_options(player_locality_audit PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
target_link_libraries(player_locality_audit PRIVATE dh2_level_world)
''';(wrapper/'CMakeLists.txt').write_text(body)
 logs=[run([cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={compiler}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'])]
 print('Building actual selected Player locality library gate',flush=True)
 logs.append(run([cmake,'--build',build,'--target','player_locality_audit','--parallel','2']))
 commands=run([ninja,'-C',build,'-t','commands','player_locality_audit']);entries=selected_entries(build,commands);sources=[Path(e['file']).resolve() for e in entries]
 assert sources.count(source.resolve())==1 and sources.count((MODULE/'player_manager_host_level.cpp').resolve())==1
 dlls=sorted(build.rglob('*.dll'));exe=build/'player_locality_audit.exe';env=os.environ.copy();env['PATH']=os.pathsep.join([str(compiler.parent),*(str(p.parent) for p in dlls),env.get('PATH','')])
 discovered=json.loads(run([exe,inputs],env))
 evidence={Path(__file__).resolve(),MODULE/'tests/player_locality_v1_original.py',WORLD/'tests/run_player_skill_cleanup_session_v1_host.py',reference/'NOTES.md',reference/'original-functions.json'}
 before=hashes(actual_dependencies(build,ninja,entries)|evidence)
 print(f'Clean selected rebuild from {len(before)} actual guarded project inputs',flush=True)
 logs.append(run([cmake,'--build',build,'--target','player_locality_audit','--clean-first','--parallel','2']))
 host=json.loads(run([exe,inputs],env));comparisons=host.pop('results')
 mismatches=[{'index':i,'inputs':rows[i],'original':old,'compiled':new} for i,(old,new) in enumerate(zip(original,comparisons)) if old!=new]
 (out/'raw-comparison.json').write_text(json.dumps({'host':host,'rows':rows,'original':original,'compiled':comparisons,'mismatches':mismatches},indent=2)+'\n')
 assert len(original)==len(comparisons) and not mismatches,('Original locality mismatch',mismatches[:3])
 after_commands=run([ninja,'-C',build,'-t','commands','player_locality_audit']);after=hashes(actual_dependencies(build,ninja,selected_entries(build,after_commands))|evidence)
 assert before==after and commands==after_commands and discovered=={**host,'results':comparisons},'Inputs changed during selected build/execution; raw retained'
 imports={p.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([compiler.with_name('objdump.exe'),'-p',p])) for p in [exe,*dlls]}
 assert 'libdh2_level_world.dll' in imports[exe.relative_to(out).as_posix()]
 report={'validation':'PASS','host_report':host,'original_comparison':{**summary,'comparisons':len(comparisons),'mismatches':0},'source_sha256':before,'source_before_after_equal':True,'new_module_selected':True,'scoped_source_tu_count':1,'staged_source':False,'selected_dso_imports':imports,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [exe,*dlls]},'selected_commands':commands,'selected_compiler_objects':{Path(e['output']).resolve().relative_to(build).as_posix():Path(e['file']).resolve().relative_to(ROOT).as_posix() for e in entries},'wrapper_cmake':body,'build_stdout':logs,'scope':summary['scope']+' Actual selected host manager/PlayerInfo identities are borrowed. Native Player/profile registration and full Matching/NetStruct construction remain outside this host proof.'}
 a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
 print(json.dumps({'host':host,'original':summary,'source_inputs':len(before),'mismatches':0}))
if __name__=='__main__':main()
