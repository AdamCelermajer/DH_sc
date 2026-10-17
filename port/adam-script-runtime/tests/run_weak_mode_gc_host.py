"""Verify Lua weak-table semantics for the bounded modern Android GC fix."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys

ROOT=Path(__file__).resolve().parents[3]
MODULE=ROOT/'port/adam-script-runtime'
sys.path.insert(0,str(ROOT/'port/level-world/tests'))
from run_monster_external_script_session_host import CORE

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--compiler',type=Path,required=True)
    p.add_argument('--output',type=Path,default=MODULE/'build/weak-mode-gc/host.exe')
    p.add_argument('--report',type=Path,default=MODULE/'build/weak-mode-gc/validation.json')
    a=p.parse_args();exe=a.output.resolve();exe.parent.mkdir(parents=True,exist_ok=True)
    cc=a.compiler.with_name(a.compiler.name.replace('g++','gcc'))
    sources=[*[MODULE/'lua'/(n+'.c') for n in CORE],MODULE/'script_runtime.c',MODULE/'tests/weak_mode_gc.cpp']
    objects=[];commands=[]
    for path in sources:
        obj=exe.parent/(path.stem+'.o')
        command=[str(a.compiler if path.suffix=='.cpp' else cc),'-std=c++17' if path.suffix=='.cpp' else '-std=c99',
                 '-O2','-Wall','-Wextra','-fno-fast-math','-ffp-contract=off','-I',str(MODULE/'lua'),'-c',str(path),'-o',str(obj)]
        result=subprocess.run(command,capture_output=True,text=True)
        if result.returncode:raise RuntimeError(result.stdout+result.stderr)
        objects.append(obj);commands.append(command)
    command=[str(a.compiler),*map(str,objects),'-lm','-o',str(exe)]
    subprocess.run(command,check=True,capture_output=True);commands.append(command)
    result=subprocess.run([str(exe)],check=True,capture_output=True,text=True);host=json.loads(result.stdout)
    assert host['validation']=='PASS' and host['mode_cases']==8 and host['native_closure_registrations']==8000
    dependencies=[*sources,*sorted((MODULE/'lua').glob('*.h')),MODULE/'script_runtime.h',Path(__file__).resolve()]
    sha=lambda f:hashlib.sha256(f.read_bytes()).hexdigest()
    report={'validation':'PASS','host_report':host,'commands':commands,'executable_sha256':sha(exe),
            'compiled_source_sha256':{f.relative_to(ROOT).as_posix():sha(f) for f in dependencies},
            'scope':'Bounds-aware weak mode scan replaces Android-fortified strchr on Lua TString trailing storage, preserving first-NUL termination; host behavior and closure-registration GC stress. Android startup is separately tested in the exact APK. No original whole-VM parity claim.'}
    a.report.parent.mkdir(parents=True,exist_ok=True);a.report.write_text(json.dumps(report,indent=2)+'\n')
    print(result.stdout.strip())

if __name__=='__main__':main()
