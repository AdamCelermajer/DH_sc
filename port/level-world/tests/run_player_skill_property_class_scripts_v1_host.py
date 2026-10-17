"""Actual selected DSOs, unchanged three-class Lua updates, and original class loader."""
from __future__ import annotations
import argparse,hashlib,json,os,re,shutil,struct,subprocess,sys
from pathlib import Path
MODULE=Path(__file__).resolve().parents[1];ROOT=MODULE.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def run(command,env=None):
    p=subprocess.run(list(map(str,command)),cwd=ROOT,env=env,capture_output=True,text=True)
    if p.returncode:raise RuntimeError(f'{command}\n{p.stdout}\n{p.stderr}')
    return p.stdout
def original_calls(elf,cache,path):
    sys.path[:0]=[str(ROOT/'port/engine-resources/tests'),str(ROOT/'port/game-data/tools'),str(ROOT/'port/level-world/tools')]
    from cpu import Cpu
    from inspect_class_tables import parse
    manifest=json.loads((ROOT/'port/game-data/reference/classes/original-functions.json').read_text())
    old=Cpu(elf,False,manifest);table=parse(cache/'data/pydata')['rows'];count=len(table)
    def word(address):return struct.unpack('<I',old.uc.mem_read(address,4))[0]
    got=(0x3e2e34+word(0x3e3008))&0xffffffff
    old.uc.mem_write(word(got+word(0x3e300c)),struct.pack('<I',count))
    rows,at=old.data+0x100000,old.data+0x140000;old.pointer(word(got+word(0x3e3010)),rows)
    for index,row in enumerate(table):
        old.uc.mem_write(rows+index*12,struct.pack('<III',0,len(row['entries']),at))
        for formula in row['entries']:old.uc.mem_write(at,struct.pack('<i5i',0,*formula));at+=24
    target,owner=old.data+0x1000,old.data+0x4000
    linear_got=(0x3e2d88+word(0x3e2e18))&0xffffffff
    old.pointer(linear_got+word(0x3e2e1c),target)
    raw=path.read_bytes();stride=4+896*3;assert len(raw)%stride==0
    cases=[]
    for offset in range(0,len(raw),stride):
        row=raw[offset:offset+stride];class_id=struct.unpack('<i',row[:4])[0]
        before,source,expected=row[4:900],row[900:1796],row[1796:]
        old.uc.mem_write(target,b'\0'*4+before);old.uc.mem_write(owner+0xa94,b'\0'*4+source)
        old.invoke(0x3e2e20,[owner,target,class_id,1])
        actual=bytes(old.uc.mem_read(target+4,896))
        assert actual==expected,(class_id,[(i,x,y) for i,(x,y) in enumerate(zip(struct.unpack('<224i',actual),struct.unpack('<224i',expected))) if x!=y][:8])
        cases.append({'class_id':class_id,'all_224_words_sha256':hashlib.sha256(actual).hexdigest()})
    return {'cases':len(cases),'distinct_classes':sorted({r['class_id'] for r in cases}),'all_224_words_equal':True,
            'distinct_original_instruction_words':len(old.seen),'reached_functions':sorted({old.address_owner[p] for p in old.seen}),
            'original_loader':'0x3e2e20','original_getters_setters_group_arithmetic_unmocked':True,'cases_sha256':hashlib.sha256(json.dumps(cases,sort_keys=True).encode()).hexdigest(),
            'scope':'Exact original temporary-mode _LoadClass over actual inputs/outputs captured from unchanged Lua calls. Callback wrappers are byte-pinned; ordinary RecalcProperty composition is covered by selected-host live-view tests, not this ARM execution.'}
def main():
    a=argparse.ArgumentParser(description=__doc__);a.add_argument('--compiler',type=Path,required=True);a.add_argument('--cache',type=Path,required=True);a.add_argument('--original-elf',type=Path,required=True);a.add_argument('--output',type=Path,required=True);a.add_argument('--report',type=Path,default=MODULE/'reports/player-skill-property-class-scripts-v1-host.json');args=a.parse_args()
    compiler=args.compiler.resolve();cc=compiler.with_name(compiler.name.replace('g++','gcc'));cache=args.cache.resolve();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    source=MODULE/'player_skill_property_services_v1.cpp';kernel=ROOT/'port/game-data/class_tables.cpp'
    assert source.name in (MODULE/'CMakeLists.txt').read_text(),'module must be actually selected'
    from run_player_skill_property_services_v1_host import check_original,MANIFEST
    original=check_original(args.original_elf.resolve(),json.loads(MANIFEST.read_text()))
    wrapper=out/'wrapper';wrapper.mkdir(exist_ok=True);build=out/'build';cmake=shutil.which('cmake');ninja=shutil.which('ninja')
    body=f'''cmake_minimum_required(VERSION 3.22)
project(player_skill_property_class_audit LANGUAGES C CXX)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("{MODULE.as_posix()}" selected-world)
add_executable(property_callback_audit "{(MODULE/'tests/player_skill_property_services_v1.cpp').as_posix()}")
add_executable(property_class_scripts_audit "{(MODULE/'tests/player_skill_property_class_scripts_v1.cpp').as_posix()}" "{(ROOT/'port/android-native/app/src/main/cpp/native_debug_files.cpp').as_posix()}" "{(ROOT/'port/pydata-names/names.c').as_posix()}")
foreach(target property_callback_audit property_class_scripts_audit)
 target_compile_features(${{target}} PRIVATE cxx_std_17)
 target_compile_options(${{target}} PRIVATE -Wall -Wextra -Werror -fno-fast-math -ffp-contract=off)
 target_link_libraries(${{target}} PRIVATE dh2_level_world dh2_script_runtime)
endforeach()
'''
    (wrapper/'CMakeLists.txt').write_text(body)
    configure=[cmake,'-S',wrapper,'-B',build,'-G','Ninja',f'-DCMAKE_MAKE_PROGRAM={ninja}',f'-DCMAKE_CXX_COMPILER={compiler}',f'-DCMAKE_C_COMPILER={cc}','-DCMAKE_BUILD_TYPE=Release','-DCMAKE_CXX_FLAGS_RELEASE=-O1','-DCMAKE_C_FLAGS_RELEASE=-O1'];logs=[run(configure)]
    commands=run([ninja,'-C',build,'-t','commands','property_class_scripts_audit'])
    entries=[Path(r['file']).resolve() for r in json.loads((build/'compile_commands.json').read_text()) if str(r['file']).replace('\\','/') in commands.replace('\\','/')]
    assert entries.count(source)==entries.count(kernel)==1 and sum(p.name=='script_runtime.c' for p in entries)==1
    paths=set(entries)|{source.with_suffix('.hpp'),kernel.with_suffix('.hpp'),MODULE/'tests/player_skill_property_services_v1.cpp',MODULE/'tests/player_skill_session_v1.cpp',Path(__file__).resolve(),Path(__file__).with_name('run_player_skill_property_services_v1_host.py'),MANIFEST,ROOT/'port/engine-resources/tests/cpu.py',ROOT/'port/game-data/tools/inspect_class_tables.py',ROOT/'port/game-data/reference/classes/original-functions.json'}
    for folder in {p.parent for p in entries}:paths.update(folder.glob('*.h'));paths.update(folder.glob('*.hpp'));paths.update(folder.glob('CMakeLists.txt'))
    paths.update(ROOT.glob('port/*/CMakeLists.txt'))
    before={p.relative_to(ROOT).as_posix():sha(p) for p in paths if p.is_relative_to(ROOT)}
    cache_before={p.relative_to(cache).as_posix():sha(p) for folder in [cache/'data/pydata',cache/'data/scripts/skills'] for p in folder.glob('*') if p.is_file()}
    cache_before['data/scripts/ai/_commons.luac']=sha(cache/'data/scripts/ai/_commons.luac')
    build_command=[cmake,'--build',build,'--target','property_callback_audit','property_class_scripts_audit','--parallel','2'];logs.append(run(build_command))
    dlls=list(build.rglob('*.dll'));exes=[build/'property_callback_audit.exe',build/'property_class_scripts_audit.exe'];env=os.environ.copy();env['PATH']=os.pathsep.join([str(compiler.parent),*(str(p.parent) for p in dlls),env.get('PATH','')])
    callbacks=run([exes[0],cache/'data/pydata'],env).strip();assert 'PASS checks=29' in callbacks,callbacks
    host=json.loads(run([exes[1],cache,out/'runtime'],env));assert host['validation']=='PASS' and host['class_cases']==3 and host['unchanged_lua_update_callbacks']==156 and host['selected_faery_script_updates']==3 and host['change_faery_all_skill_callbacks']==39 and host['required_failure_prefixes']==1
    differential=original_calls(args.original_elf.resolve(),cache,out/'runtime/temporary-class-calls.bin');assert differential['cases']==host['temporary_class_calls']
    data={path:sha(cache/path) for path in host['resources']}
    for path in (cache/'data/pydata').glob('*.bin'):
        if path.name.startswith(('character_classes_','character_properties_','skills_','faeries_','effects_','projectiles_','ai_','design_')):data[path.relative_to(cache).as_posix()]=sha(path)
    assert all(cache_before[name]==digest for name,digest in data.items()),'reached cache changed during gate'
    assert before=={name:sha(ROOT/name) for name in before},'selected inputs changed during gate'
    imports={p.relative_to(out).as_posix():re.findall(r'DLL Name: (\S+)',run([compiler.with_name('objdump.exe'),'-p',p])) for p in [*exes,*dlls]}
    assert 'libdh2_level_world.dll' in imports[exes[1].relative_to(out).as_posix()] and 'libdh2_script_runtime.dll' in imports[exes[1].relative_to(out).as_posix()]
    report={'validation':'PASS','host':host,'callback_output':callbacks,'original':original,'original_temporary_classes':differential,'source_sha256':before,'cache_sha256':data,'actual_selected_world_data_vm':True,'property_service_tu_count':entries.count(source),'class_kernel_tu_count':entries.count(kernel),'script_runtime_tu_count':sum(p.name=='script_runtime.c' for p in entries),'unchanged_script_bytes':True,'native_build_or_live_gameplay':False,'dso_imports':imports,'binary_sha256':{p.relative_to(out).as_posix():sha(p) for p in [*exes,*dlls]},'wrapper_cmake':body,'selected_commands':commands,'build_output':logs,'commands':[list(map(str,configure)),list(map(str,build_command))],'scope':'Three actual Player property rows, genuine saved skill/faery constructors, same single Session/preparation/update/PropertyView/buff/Coordinator graph per Player; saved hotbar slot write followed by source UpdateSkills (slot skill then selected Celest), Save-selected ChangeFaery followed by 13 UpdateAllSkills callbacks on that same VM, and original SetTempProps common helper at positive preview level2; required Roundhouse failure prefix. Positive whole passive updates requiring target acquisition remain outside this fixture. No native activation, profile InitPost, animation/control, inventory or live gameplay completion claim.'}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','callbacks':callbacks,'host':host,'original':differential,'source_inputs':len(before)}))
if __name__=='__main__':main()
