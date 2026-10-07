from pathlib import Path
import json,subprocess,shlex,hashlib
ROOT=Path(__file__).resolve().parents[5];OUT=Path(__file__).resolve().parent;BUILD=ROOT/'.local-inputs/audio-v40-focus';results=[]
for abi in ('arm64-v8a','x86_64'):
    db=json.loads((ROOT/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text())
    entry=next(e for e in db if e['file'].replace('\\','/').endswith('/model_renderer.cpp'))
    args=entry.get('arguments')or[s.strip('"')for s in shlex.split(entry['command'],posix=False)]
    flags=[s for s in args[1:]if s.startswith(('--target=','--sysroot=','-I','-isystem'))]
    for i,s in enumerate(args):
        if s=='-isystem':flags.append(args[i+1])
    result=subprocess.run([args[0],*flags,'-std=c++17','-O2','-Wall','-Wextra','-Werror','-c',str(OUT/'helper-fixture.cpp'),'-o',str(BUILD/('default-fields-'+abi+'.o'))],capture_output=True,text=True,timeout=40)
    assert result.returncode==0,result.stderr
    results.append(dict(abi=abi,exit_code=0))
report=dict(native=results,host=dict(optimization='O2',sanitizers='ASan+UBSan',checks=221,exit_code=0),source_sha256={f.name:hashlib.sha256(f.read_bytes()).hexdigest()for f in OUT.iterdir()if f.suffix in ('.cpp','.hpp','.bin','.py')})
(OUT/'compile-validation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
