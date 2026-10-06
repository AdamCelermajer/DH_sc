from pathlib import Path
import json,shlex,subprocess,os
root=Path(__file__).resolve().parents[3];out=root/'.local-inputs/audio-v38';out.mkdir(exist_ok=True);os.environ['TMP']=str(out);os.environ['TEMP']=str(out)
sources=['port/engine-audio/vox_source_fields_v38.cpp','port/engine-audio/audio_listener_rows_v38.cpp','port/engine-audio/audio_spatial_v34.cpp'];result=[]
for abi in ['arm64-v8a','x86_64']:
 db=json.loads((root/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text());e=next(e for e in db if e['file'].replace('\\','/').endswith('/model_renderer.cpp'));args=e.get('arguments')or[s.strip('"')for s in shlex.split(e['command'],posix=False)];flags=[s for s in args[1:]if s.startswith(('--target=','--sysroot=','-I'))]
 for source in sources[:2]:
  p=subprocess.run([args[0],*flags,'-std=c++17','-O2','-ffp-contract=off','-fno-fast-math','-Wall','-Wextra','-Werror','-include',str(root/'port/engine-audio/audio_constructor_fields_v38.hpp'),'-c',str(root/source),'-o',str(out/(Path(source).stem+abi+'.o'))],capture_output=True,text=True,timeout=90);assert p.returncode==0,p.stderr;result.append(dict(abi=abi,source=source,checked_header='port/engine-audio/audio_constructor_fields_v38.hpp',exit_code=p.returncode))
 if abi=='arm64-v8a':
  p=subprocess.run([args[0],*flags,'-std=c++17','-O2','-ffp-contract=off','-fno-fast-math','-fPIC','-shared',*[str(root/s)for s in sources],str(root/'port/engine-audio/tests/vox_source_fields_v38_oracle.cpp'),'-Wl,-Bsymbolic','-o',str(out/'vox-fields-arm64.so')],capture_output=True,text=True,timeout=90);assert p.returncode==0,p.stderr
(root/'port/engine-audio/reference/authorities-v38/strict-compile.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
