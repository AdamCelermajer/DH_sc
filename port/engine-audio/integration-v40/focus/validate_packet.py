from pathlib import Path
import json,shlex,subprocess,hashlib
ROOT=Path(__file__).resolve().parents[4];OUT=Path(__file__).resolve().parent
BUILD=ROOT/'.local-inputs/audio-v40-focus';BUILD.mkdir(parents=True,exist_ok=True)
JAVAC=r'C:/Program Files/Android/Android Studio/jbr/bin/javac.exe'
JAVA=r'C:/Program Files/Android/Android Studio/jbr/bin/java.exe'
ANDROID=r'C:/Users/adamc/AppData/Local/Android/Sdk/platforms/android-37.0/android.jar'
def run(args):
    result=subprocess.run(args,capture_output=True,text=True,timeout=45)
    if result.returncode:print(result.stdout,result.stderr);raise RuntimeError(args)
    return result.stdout.strip()
report={}
run([JAVAC,'-Xlint:all','-Werror','-d',str(BUILD/'policy-java'),str(OUT/'AudioFocusPolicyV40.java'),str(OUT/'AudioFocusPolicyV40Test.java')])
report['java_policy']=run([JAVA,'-cp',str(BUILD/'policy-java'),'com.example.dh2.AudioFocusPolicyV40Test'])
# New focus owner compiles independently with all lint warnings fatal.
run([JAVAC,'-source','11','-target','11','-Xlint:all','-Xlint:-options','-Werror','-classpath',ANDROID,'-d',str(BUILD/'helper-java'),str(OUT/'AudioFocusPolicyV40.java'),str(OUT/'SharedAudioFocusV40.java')])
report['new_java_helpers']='PASS strict all lint'
staged=OUT/'staged/java/com/example/dh2';app=ROOT/'port/android-native/app/src/main/java/com/example/dh2'
src=[f for f in app.glob('*.java')if not(staged/f.name).exists()]+list(staged.glob('*.java'))
run([JAVAC,'-source','11','-target','11','-Xlint:all','-Xlint:-options','-Xlint:-deprecation','-Xlint:-restricted','-Werror','-classpath',ANDROID,'-d',str(BUILD/'app-java'),*[str(f)for f in src]])
report['staged_java_compile']='PASS; existing deprecated/restricted SDK API warnings excluded'
report['native']=[]
for abi in ['arm64-v8a','x86_64']:
    db=json.loads((ROOT/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text())
    entry=next(e for e in db if e['file'].replace('\\','/').endswith('/model_renderer.cpp'))
    args=entry.get('arguments')or[s.strip('"')for s in shlex.split(entry['command'],posix=False)]
    flags=[s for s in args[1:]if s.startswith(('--target=','--sysroot=','-I','-isystem'))]
    for i,s in enumerate(args):
        if s=='-isystem':flags.append(args[i+1])
    for name in ['audio_lifecycle_gate_v40','audio_lifecycle_jni_v40','audio_output_v40','audio_control_v40','audio_native_session_v40']:
        run([args[0],*flags,'-std=c++17','-Wall','-Wextra','-Werror','-c',str(OUT/(name+'.cpp')),'-o',str(BUILD/(name+'-'+abi+'.o'))])
        report['native'].append(dict(abi=abi,target=next(s for s in flags if s.startswith('--target=')),source=name,exit_code=0))
report['source_sha256']={str(f.relative_to(ROOT)).replace('\\','/'):hashlib.sha256(f.read_bytes()).hexdigest()for f in OUT.iterdir()if f.suffix in ('.java','.cpp','.hpp','.py','.patch')}
(OUT/'compile-validation.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(report))
