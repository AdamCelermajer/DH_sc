"""Execute unchanged monster OnInit through maintained source queries/stats.

The PlayerInfo/Level producer values are explicit host fixtures. This is not
Android execution or a complete positive-delta debug/AI lifecycle proof.
"""
import argparse, hashlib, json, pathlib, subprocess, sys
from run_monster_external_script_session_host import CORE

ROOT = pathlib.Path(__file__).resolve().parents[3]
MODULE = ROOT / 'port/level-world'
RUNTIME = ROOT / 'port/adam-script-runtime'

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--compiler',required=True,type=pathlib.Path)
    p.add_argument('--cache',required=True,type=pathlib.Path)
    p.add_argument('--output',type=pathlib.Path,default=MODULE/'build/monster-initialization-session/host.exe')
    p.add_argument('--report',type=pathlib.Path,default=MODULE/'build/monster-initialization-session/validation.json')
    a=p.parse_args();exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    objects=exe.parent/'objects';objects.mkdir(exist_ok=True)
    cc=a.compiler.with_name(a.compiler.name.replace('g++','gcc'))
    c_sources=[RUNTIME/'lua'/(n+'.c') for n in CORE]+[
        RUNTIME/'script_runtime.c',ROOT/'port/lua-numeric/numeric.c',ROOT/'port/pydata-constants/constants.c']
    cpp_sources=[RUNTIME/'script_function_alias.cpp',MODULE/'ais_native_bindings.cpp',
        MODULE/'character_oid_cache_v1.cpp',MODULE/'monster_external_script_session.cpp',MODULE/'lua_script_level_queries.cpp',
        MODULE/'lua_script_load_once.cpp',
        MODULE/'ais_state_callbacks.cpp',
        MODULE/'character_script_set_level.cpp',MODULE/'character_regeneration.cpp',
        ROOT/'port/game-data/data.cpp',ROOT/'port/game-data/class_tables.cpp',
        ROOT/'port/game-data/properties.cpp',ROOT/'port/game-data/vitals.cpp',
        ROOT/'port/game-data/level_tables.cpp',
        ROOT/'port/gameplay-object-callbacks/gameplay_object_callbacks.cpp',
        MODULE/'tests/monster_initialization_session.cpp']
    commands=[];linked=[];warnings=[]
    for source in c_sources+cpp_sources:
        obj=objects/(source.parent.name+'-'+source.stem+'.o')
        command=[str(a.compiler if source.suffix=='.cpp' else cc),
            '-std=c++17' if source.suffix=='.cpp' else '-std=c99','-O1','-Wall','-Wextra',
            '-fno-fast-math','-ffp-contract=off','-I',str(RUNTIME/'lua'),'-c',str(source),'-o',str(obj)]
        result=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
        if result.returncode:raise RuntimeError(result.stdout+result.stderr)
        if result.stderr:warnings.append({'path':source.relative_to(ROOT).as_posix(),'diagnostics':result.stderr})
        commands.append(command);linked.append(obj)
    command=[str(a.compiler),*map(str,linked),'-lm','-o',str(exe)]
    subprocess.run(command,cwd=ROOT,check=True,capture_output=True);commands.append(command)
    scripts=ROOT/'recovered/scripts/original/data/scripts/ai'
    common,monster=scripts/'_commons.luac',scripts/'monster.luac'
    sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
    assert sha(common)=='20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
    assert sha(monster)=='84f07caaeb2c04f2024cc3e27d41f33806b2c53d6e8861bb3d8b371118fd0e1d'
    result=subprocess.run([str(exe),str(a.cache.resolve()),str(common),str(monster)],cwd=ROOT,capture_output=True,text=True)
    if result.returncode:raise RuntimeError(result.stdout+result.stderr)
    host=json.loads(result.stdout);assert host['validation']=='PASS' and host['host_cases']==28
    deps=c_sources+cpp_sources+list((RUNTIME/'lua').glob('*.h'))+[
        MODULE/(n+'.hpp') for n in ['monster_external_script_session','lua_script_load_once','ais_state_callbacks','lua_script_level_queries','character_script_set_level','character_regeneration','ais_native_bindings']]
    deps += list((ROOT/'port/game-data').glob('*.hpp'))+[
        RUNTIME/'script_runtime.h',RUNTIME/'script_function_alias.h',ROOT/'port/lua-numeric/numeric.h',
        ROOT/'port/pydata-constants/constants.h',ROOT/'port/gameplay-object-callbacks/gameplay_object_callbacks.hpp',pathlib.Path(__file__).resolve()]
    cache_paths=[a.cache.resolve()/'data/pydata'/n for n in [
        'character_properties_pyarray.bin','character_properties_pyarraynames.bin','character_properties_pystructnames.bin',
        'character_classes_pyarray.bin','character_classes_pyarraynames.bin','character_classes_pystructnames.bin','design_pycst.bin',
        'levels_pyarray.bin','levels_pyarraynames.bin','levels_pystructnames.bin']]
    report={'validation':'PASS','host_report':host,'commands':commands,'warnings':warnings,
        'compiled_source_sha256':{f.relative_to(ROOT).as_posix():sha(f) for f in deps},
        'original_script_sha256':{f.relative_to(ROOT).as_posix():sha(f) for f in [common,monster]},
        'cache_sha256':{f.relative_to(a.cache.resolve()).as_posix():sha(f) for f in cache_paths},
        'executable_sha256':sha(exe),'native_wired':False,
        'scope':'Unchanged original monster OnInit on both Crypt Ghost cache records. Source level-query sentinel and actual Crypt/Swamp/Infected Village difficulty ranges, SetLevel, live class/property recalculation and zero-delta full HP/MP regeneration compose in the same VM. Host PlayerInfo/Level producer fields remain explicit fixtures. Positive regen debug ownership fails closed after real base/class mutations; active AIS, source room acquisition and Android pursuit remain unproven.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(host));print('report:',a.report)

if __name__=='__main__':main()
