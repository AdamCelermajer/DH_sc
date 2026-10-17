"""One-image float32 Lua source status/required failure/return observation audit."""
from pathlib import Path
import argparse
import hashlib
import json
import subprocess
import sys
ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/adam-script-runtime'
CORE='lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib'.split()
MANIFEST=MODULE/'reference/source-compatibility/original-functions.json'
def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler',required=True,type=Path)
    parser.add_argument('--original-elf',required=True,type=Path)
    parser.add_argument('--output',type=Path,default=MODULE/'build/source-compatibility/host.exe')
    parser.add_argument('--report',type=Path,default=MODULE/'build/source-compatibility/validation.json')
    args=parser.parse_args();output=args.output.resolve();output.parent.mkdir(parents=True,exist_ok=True)
    sys.path.insert(0,str(ROOT/'port/level-world/tests'))
    from run_character_ai_set_skills_and_spells_host import source_symbols
    manifest=json.loads(MANIFEST.read_text(encoding='utf-8'));source_symbols(args.original_elf.resolve(),manifest)
    c_sources=[*[MODULE/'lua'/(unit+'.c') for unit in CORE],MODULE/'script_runtime.c']
    test=MODULE/'tests/source_compatibility.cpp'
    inputs=[*c_sources,test,MODULE/'script_runtime.h',*sorted((MODULE/'lua').glob('*.h')),
            Path(__file__).resolve(),MANIFEST,MANIFEST.with_name('NOTES.md'),ROOT/'port/level-world/tests/run_character_ai_set_skills_and_spells_host.py']
    before={p.relative_to(ROOT).as_posix():digest(p) for p in sorted(set(inputs))}
    gcc=args.compiler.with_name('gcc.exe' if args.compiler.suffix=='.exe' else 'gcc')
    commands=[];objects=[];diagnostics=[]
    for path in [*c_sources,test]:
        obj=output.parent/(path.stem+'.o');own=path==test
        command=[str(args.compiler if own else gcc),'-std=c++17' if own else '-std=c99','-O1',
                 '-Wall','-Wextra',*(['-Werror','-pedantic'] if own else []),'-fno-fast-math','-ffp-contract=off',
                 '-I',str(MODULE/'lua'),'-c',str(path),'-o',str(obj)]
        compiled=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
        if compiled.returncode:raise RuntimeError(compiled.stdout+compiled.stderr)
        if compiled.stderr:diagnostics.append({'path':path.relative_to(ROOT).as_posix(),'diagnostics':compiled.stderr})
        commands.append(command);objects.append(obj)
    command=[str(args.compiler),*map(str,objects),'-lm','-o',str(output)]
    linked=subprocess.run(command,cwd=ROOT,capture_output=True,text=True)
    if linked.returncode:raise RuntimeError(linked.stdout+linked.stderr)
    commands.append(command)
    completed=subprocess.run([str(output)],cwd=ROOT,capture_output=True,text=True)
    if completed.returncode:raise RuntimeError(completed.stdout+completed.stderr)
    host=json.loads(completed.stdout)
    assert host['validation']=='PASS' and host['checks']>=100 and host['observers']>=20 and host['required_callbacks']>=5
    after={p.relative_to(ROOT).as_posix():digest(p) for p in sorted(set(inputs))};assert before==after,'source changed during proof'
    report={'validation':'PASS','host_report':host,'compiled_source_sha256':before,'commands':commands,
            'compiler_diagnostics':diagnostics,'executable_sha256':digest(output),'original_sha256':manifest['original_sha256'],
            'new_complete_original_bodies':0,'original_ranges_verified':len(manifest['functions']),
            'scope':'Real retained float32 Lua VM exercises source protocol adapters. Original ELF ranges verified as evidence; original ARM bodies do not execute in this gate. No player native activation.'}
    args.report.parent.mkdir(parents=True,exist_ok=True);args.report.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(completed.stdout.strip())
if __name__=='__main__':main()
