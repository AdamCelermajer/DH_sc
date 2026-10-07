from pathlib import Path
import json,shlex,subprocess,os,hashlib
root=Path(__file__).resolve().parents[3];out=root/'.local-inputs/audio-v40';out.mkdir(exist_ok=True);os.environ['TMP']=str(out);os.environ['TEMP']=str(out)
files=['port/engine-audio/audio_clock_v40.cpp','port/engine-audio/audio_gameplay_runtime_v40.cpp','port/engine-audio/audio_source_command_v40.cpp'];result=[]
for abi in ['arm64-v8a','x86_64']:
 db=json.loads((root/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text());e=next(e for e in db if e['file'].replace('\\','/').endswith('/model_renderer.cpp'));args=e.get('arguments')or[s.strip('"')for s in shlex.split(e['command'],posix=False)];flags=[s for s in args[1:]if s.startswith(('--target=','--sysroot=','-I'))]
 for source in files:
  p=subprocess.run([args[0],*flags,'-std=c++17','-O2','-ffp-contract=off','-fno-fast-math','-Wall','-Wextra','-Werror','-c',str(root/source),'-o',str(out/(Path(source).stem+abi+'.o'))],capture_output=True,text=True,timeout=90);print(p.stderr);assert p.returncode==0;result.append(dict(abi=abi,source=source,exit_code=p.returncode,sha256=hashlib.sha256((root/source).read_bytes()).hexdigest()))
dest=root/'port/engine-audio/integration-v40/runtime';dest.mkdir(parents=True,exist_ok=True);(dest/'strict-compile.json').write_text(json.dumps(dict(flags=['-O2','-Wall','-Wextra','-Werror','-fno-fast-math','-ffp-contract=off'],results=result),indent=2)+'\n');print(json.dumps(result))
