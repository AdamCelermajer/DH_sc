"""Unchanged monster OnInit with source debug callers and real file effects.

PlayerInfo/current Level producers remain explicit host fixtures. No Android,
active-AIS/full-lifecycle, original OS-filebody or new caller-body claim.
"""
import argparse,hashlib,json,pathlib,subprocess
from run_monster_external_script_session_host import CORE
ROOT=pathlib.Path(__file__).resolve().parents[3];MODULE=ROOT/'port/level-world';RUNTIME=ROOT/'port/adam-script-runtime'
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--compiler',type=pathlib.Path,required=True);p.add_argument('--cache',type=pathlib.Path,required=True);p.add_argument('--output',type=pathlib.Path,default=MODULE/'build/monster-initialization-debug-persistence/host.exe');p.add_argument('--report',type=pathlib.Path,default=MODULE/'build/monster-initialization-debug-persistence/validation.json');a=p.parse_args()
    exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True);objects=exe.parent/'objects';objects.mkdir(exist_ok=True);cc=a.compiler.with_name(a.compiler.name.replace('g++','gcc'))
    c_sources=[RUNTIME/'lua'/(name+'.c') for name in CORE]+[RUNTIME/'script_runtime.c',ROOT/'port/lua-numeric/numeric.c',ROOT/'port/pydata-constants/constants.c']
    names=['ais_native_bindings','monster_external_script_session','lua_script_load_once','ais_state_callbacks','lua_script_level_queries','character_script_set_level','character_regeneration','debug_switches_runtime','debug_switches_persistence']
    cpp_sources=[RUNTIME/'script_function_alias.cpp',*[MODULE/(name+'.cpp') for name in names],*[ROOT/'port/game-data'/(name+'.cpp') for name in ['data','class_tables','properties','vitals','level_tables']],ROOT/'port/gameplay-object-callbacks/gameplay_object_callbacks.cpp',MODULE/'tests/monster_initialization_debug_persistence.cpp']
    commands=[];linked=[];warnings=[]
    for source in c_sources+cpp_sources:
        obj=objects/(source.parent.name+'-'+source.stem+'.o');command=[str(a.compiler if source.suffix=='.cpp' else cc),'-std=c++17' if source.suffix=='.cpp' else '-std=c99','-O1','-Wall','-Wextra','-fno-fast-math','-ffp-contract=off','-I',str(RUNTIME/'lua'),'-c',str(source),'-o',str(obj)]
        result=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
        if result.returncode:raise RuntimeError(result.stdout+result.stderr)
        if result.stderr:warnings.append({'path':source.relative_to(ROOT).as_posix(),'diagnostics':result.stderr})
        commands.append(command);linked.append(obj)
    command=[str(a.compiler),*map(str,linked),'-lm','-o',str(exe)];subprocess.run(command,cwd=ROOT,check=True,capture_output=True);commands.append(command)
    scripts=ROOT/'recovered/scripts/original/data/scripts/ai';common,monster=scripts/'_commons.luac',scripts/'monster.luac'
    assert sha(common)=='20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c' and sha(monster)=='84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d'
    cache=a.cache.resolve();configuration=cache/'DebugSwitches.savegame';original_configuration_sha=sha(configuration);assert original_configuration_sha=='51a3827f0109e16d1520e76d5b19736df3afe38375b91b955ac519f954234d6b'
    folder=exe.parent/'native-files';folder.mkdir(exist_ok=True);result=subprocess.run([str(exe),str(cache),str(common),str(monster),str(folder)],cwd=ROOT,capture_output=True,text=True)
    if result.returncode:raise RuntimeError(result.stdout+result.stderr)
    host=json.loads(result.stdout);assert host['validation']=='PASS' and host['host_cases']==24 and host['actual_catalogue_positive_cases']==18 and sha(configuration)==original_configuration_sha
    deps=c_sources+cpp_sources+list((RUNTIME/'lua').glob('*.h'))+[MODULE/(name+'.hpp') for name in names]+list((ROOT/'port/game-data').glob('*.hpp'))+[RUNTIME/'script_runtime.h',RUNTIME/'script_function_alias.h',ROOT/'port/lua-numeric/numeric.h',ROOT/'port/pydata-constants/constants.h',ROOT/'port/gameplay-object-callbacks/gameplay_object_callbacks.hpp',ROOT/'port/persistence/binary.h',MODULE/'tests/monster_initialization_session.cpp',pathlib.Path(__file__).resolve(),MODULE/'reference/monster-initialization-debug-persistence/NOTES.md']
    files=[cache/'data/pydata'/name for name in ['character_properties_pyarray.bin','character_properties_pyarraynames.bin','character_properties_pystructnames.bin','character_classes_pyarray.bin','character_classes_pyarraynames.bin','character_classes_pystructnames.bin','design_pycst.bin','levels_pyarray.bin','levels_pyarraynames.bin','levels_pystructnames.bin']]+[configuration]
    persisted={f.relative_to(folder).as_posix():{'sha256':sha(f),'bytes':f.stat().st_size} for f in sorted(folder.glob('*/DebugSwitches.savegame'))};assert len(persisted)==24
    for i in range(18):assert persisted[f'{i}/DebugSwitches.savegame']['sha256']==original_configuration_sha
    assert persisted['21/DebugSwitches.savegame']['bytes']==4
    report={'validation':'PASS','host_report':host,'commands':commands,'warnings':warnings,'compiled_source_sha256':{f.relative_to(ROOT).as_posix():sha(f) for f in deps},'original_script_sha256':{f.relative_to(ROOT).as_posix():sha(f) for f in (common,monster)},'cache_sha256':{f.relative_to(cache).as_posix():sha(f) for f in files},'real_generated_configuration_files':persisted,'original_evidence_configuration_unchanged':True,'executable_sha256':sha(exe),'native_wired':False,'new_complete_caller_bodies':0,'scope':'Unchanged original monster OnInit composes maintained source queries/SetLevel/class/property/HP/MP/debug Runtime/live-map save writer.18actualcatalogue cases use both Crypt Ghost records and Crypt/Swamp/InfectedVillage three difficulties, with both positive regeneration branches. Real temporary files and native string ownership supplied by host adapters; PlayerInfo/currentLevel producers explicitly fixtures. No original OS filebody/full AI/lifecycle/native pursuit claim.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(host));print('report:',a.report)
if __name__=='__main__':main()
