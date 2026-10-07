from pathlib import Path
import json,subprocess,shlex,hashlib
ROOT=Path(__file__).resolve().parents[4];OUT=Path(__file__).resolve().parent;BUILD=ROOT/'.local-inputs/audio-v42-application-hooks';BUILD.mkdir(parents=True,exist_ok=True)
def run(args):
    r=subprocess.run(args,capture_output=True,text=True,timeout=90)
    if r.returncode:print(r.stderr);raise RuntimeError(args)
    return r.stdout.strip()
report=dict(native=[],patch_checks=[])
overlay=BUILD/'staged-overlay.json'
virtual=[]
for relative in ('port/android-native/app/src/main/cpp/native_app.cpp','port/android-native/app/src/main/cpp/model_renderer.hpp'):
    virtual.append({'type':'file','name':(ROOT/relative).as_posix(),'external-contents':(OUT/'staged'/relative).as_posix()})
overlay.write_text(json.dumps({'version':0,'case-sensitive':False,'use-external-names':False,'roots':virtual}))
for patch in ('native-application.patch','application-java-v42.patch'):
    run(['git','apply','--check',str(OUT/patch)]);report['patch_checks'].append(dict(file=patch,status='PASS'))
for abi in ('arm64-v8a','x86_64'):
    db=json.loads((ROOT/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text())
    entry=next(e for e in db if e['file'].replace('\\','/').endswith('/native_app.cpp'))
    args=entry.get('arguments')or[s.strip('"')for s in shlex.split(entry['command'],posix=False)]
    flags=[]
    for i,s in enumerate(args):
        if s=='-isystem':flags.extend([s,args[i+1]])
        elif s.startswith(('--target=','--sysroot=','-I','-D')):flags.append(s)
    flags+=['-I'+str(ROOT),'-I'+str(ROOT/'port/engine-audio/integration-v42'),'-I'+str(ROOT/'port/android-native/app/src/main/cpp')]
    for source in [OUT/'native_audio_application_v42.cpp',OUT/'staged/port/android-native/app/src/main/cpp/native_app.cpp']:
        actual_source=ROOT/'port/android-native/app/src/main/cpp/native_app.cpp' if source.stem=='native_app' else source
        run([args[0],*flags,'-ivfsoverlay',str(overlay),'-std=c++17','-O2','-Wall','-Wextra','-Werror','-c',str(actual_source),'-o',str(BUILD/(source.stem+'-'+abi+'.o'))])
        report['native'].append(dict(abi=abi,target=next(s for s in flags if s.startswith('--target=')),source=str(source.relative_to(ROOT)).replace('\\','/'),status='PASS'))
staged=OUT/'staged/java/com/example/dh2';app=ROOT/'port/android-native/app/src/main/java/com/example/dh2'
sources=[p for p in app.glob('*.java')if not(staged/p.name).exists()]+list(staged.glob('*.java'))
run([r'C:/Program Files/Android/Android Studio/jbr/bin/javac.exe','-source','11','-target','11','-Xlint:all','-Xlint:-options','-Xlint:-deprecation','-Xlint:-restricted','-Werror','-classpath',r'C:/Users/adamc/AppData/Local/Android/Sdk/platforms/android-37.0/android.jar','-d',str(BUILD/'java'),*[str(p)for p in sources]])
report['staged_java']='PASS; existing deprecated/restricted SDK warnings excluded'
report['source_sha256']={str(p.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(p.read_bytes()).hexdigest()for p in OUT.rglob('*')if p.is_file() and p.suffix in ('.cpp','.hpp','.java','.patch','.py')}
report['scope']='Compilation and patch applicability only; no application APK/link/runtime/device. Parent backend host tests cover exact source pack/precache. GS/RNG/source-command provider seam remains required.'
(OUT/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
