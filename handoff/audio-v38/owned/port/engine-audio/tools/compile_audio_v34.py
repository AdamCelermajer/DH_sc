from pathlib import Path
import json,subprocess,shlex,os
root=Path(__file__).resolve().parents[3];out=root/'.local-inputs/audio-v34';out.mkdir(parents=True,exist_ok=True);os.environ['TMP']=str(out);os.environ['TEMP']=str(out)
files=['port/engine-audio/'+s+'.cpp'for s in ['audio_sample_v34','audio_mixer_v34','audio_catalog_v34','audio_bank_v34','audio_spatial_v34','audio_native_envelope_v34']]+['port/android-native/app/src/main/cpp/audio_output_v34.cpp','port/level-world/vox_audio_bridge_v34.cpp']
results=[]
for abi in ['arm64-v8a','x86_64']:
 db=json.loads((root/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text());entry=next(e for e in db if e['file'].replace('\\','/').endswith('/model_renderer.cpp'));args=entry.get('arguments')or[s.strip('"')for s in shlex.split(entry['command'],posix=False)];flags=[s for s in args[1:]if s.startswith(('--target=','--sysroot=','-I','-isystem'))]
 for i,s in enumerate(args):
  if s=='-isystem':flags.append(args[i+1])
 for f in files:
  p=subprocess.run([args[0],*flags,'-std=c++17','-O2','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-c',str(root/f),'-o',str(out/(Path(f).stem+'-'+abi+'.o'))],capture_output=True,text=True,timeout=90);print(abi,f,p.returncode,p.stderr);assert p.returncode==0;results.append(dict(abi=abi,file=f,exit_code=p.returncode))
 if abi=='arm64-v8a':
  oracle=['port/engine-audio/tests/audio_ima_v34_oracle.cpp','port/engine-audio/audio_sample_v34.cpp','port/engine-audio/audio_spatial_v34.cpp','port/engine-audio/audio_native_envelope_v34.cpp']
  p=subprocess.run([args[0],*flags,'-std=c++17','-O2','-fPIC','-shared',*[str(root/f)for f in oracle],'-Wl,-Bsymbolic','-o',str(out/'ima-oracle-arm64.so')],capture_output=True,text=True,timeout=90);print(p.stderr);assert p.returncode==0
java='port/android-native/app/src/main/java/com/example/dh2/AudioLifecycleV34.java'
p=subprocess.run([r'C:/Program Files/Android/Android Studio/jbr/bin/javac.exe','-source','11','-target','11','-Xlint:all','-Xlint:-options','-Werror','-classpath',r'C:/Users/adamc/AppData/Local/Android/Sdk/platforms/android-37.0/android.jar','-d',str(out/'java-classes'),str(root/java)],capture_output=True,text=True,timeout=90);print(p.stdout,p.stderr);assert p.returncode==0
(root/'port/engine-audio/reports').mkdir(exist_ok=True);(root/'port/engine-audio/reports/audio-v34-strict-compile.json').write_text(json.dumps(dict(native=results,java=dict(source=java,exit_code=p.returncode)),indent=2)+'\n')
