from pathlib import Path
import json,shlex,subprocess,hashlib,os
root=Path(__file__).resolve().parents[3];packet=Path(__file__).parent;out=root/'.local-inputs/audio-v41';out.mkdir(exist_ok=True);os.environ['TMP']=str(out);os.environ['TEMP']=str(out)
results=[];source=packet/'host.cpp'
for abi in ['arm64-v8a','x86_64']:
 db=json.loads((root/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text());e=next(e for e in db if e['file'].replace('\\','/').endswith('/model_renderer.cpp'));args=e.get('arguments')or[s.strip('"')for s in shlex.split(e['command'],posix=False)];flags=[s for s in args[1:]if s.startswith(('--target=','--sysroot=','-I'))]
 p=subprocess.run([args[0],*flags,'-std=c++17','-O2','-Wall','-Wextra','-Werror','-c',str(source),'-o',str(out/(abi+'.o'))],capture_output=True,text=True,timeout=90);assert not p.returncode,p.stderr;results.append(dict(abi=abi,exit_code=0))
exe='.local-inputs/audio-v41/host';wd=['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec']
p=subprocess.run(wd+['g++','-std=c++17','-O2','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer',str(source.relative_to(root)).replace('\\','/'),'-o',exe],capture_output=True,text=True,timeout=90);assert not p.returncode,p.stderr
p=subprocess.run(wd+['env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',exe],capture_output=True,text=True,timeout=20);assert not p.returncode,p.stderr
(packet/'host-validation.json').write_text(json.dumps(dict(strict_abi=results,host_output=p.stdout,scope='Isolated synchronous provider fixtures, no actual manager/World/backend',source_sha256={str(f.relative_to(root)).replace('\\','/'):hashlib.sha256(f.read_bytes()).hexdigest()for f in [source,packet/'audio_precache_v41.hpp']}),indent=2)+'\n');print(p.stdout)
