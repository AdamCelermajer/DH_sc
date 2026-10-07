from pathlib import Path
import json,shlex,subprocess,hashlib,os
root=Path(__file__).resolve().parents[3];packet=Path(__file__).parent;out=root/'.local-inputs/audio-v42';out.mkdir(exist_ok=True);os.environ['TMP']=str(out);os.environ['TEMP']=str(out)
owned=['port/engine-audio/audio_gameplay_runtime_v42.cpp','port/engine-audio/integration-v42/audio_native_session_v42.cpp','port/engine-audio/integration-v42/audio_application_manager_v42.cpp'];strict=[]
for abi in ['arm64-v8a','x86_64']:
 db=json.loads((root/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text());e=next(e for e in db if e['file'].replace('\\','/').endswith('/model_renderer.cpp'));args=e.get('arguments')or[s.strip('"')for s in shlex.split(e['command'],posix=False)];flags=[s for s in args[1:]if s.startswith(('--target=','--sysroot=','-I'))]
 for source in owned:
  p=subprocess.run([args[0],*flags,'-std=c++17','-O2','-Wall','-Wextra','-Werror','-ffp-contract=off','-fno-fast-math','-c',str(root/source),'-o',str(out/(Path(source).stem+abi+'.o'))],capture_output=True,text=True,timeout=90);assert not p.returncode,p.stderr;strict.append(dict(abi=abi,source=source,exit_code=0))
sources=owned+['port/engine-audio/integration-v42/application-fixture.cpp','port/engine-audio/integration-v40/focus/audio_lifecycle_gate_v40.cpp','port/level-world/vox_play3d_owner_v2.cpp']+['port/engine-audio/'+name+'.cpp'for name in ['audio_clock_v40','audio_source_bindings_v38','audio_sample_v34','audio_mixer_v34','audio_catalog_v34','audio_bank_v34','audio_native_envelope_v34']]
wd=['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec'];runs=[]
for opt in ['O1','O2']:
 exe='.local-inputs/audio-v42/application-'+opt
 p=subprocess.run(wd+['g++','-std=c++17','-'+opt,'-DDH2_AUDIO_NATIVE_SESSION_FIXTURE','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread',*sources,'-o',exe],capture_output=True,text=True,timeout=180);assert not p.returncode,p.stderr
 p=subprocess.run(wd+['env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',exe,'.'],capture_output=True,text=True,timeout=20);assert not p.returncode,(p.stdout,p.stderr);print(p.stdout);runs.append(dict(optimization=opt,output=p.stdout,exit_code=0))
(packet/'application-validation.json').write_text(json.dumps(dict(strict_abi=strict,runs=runs,scope='Real exact source parsing and decoded raw UID bank, borrowed control fixture; no Android output/World/audible acceptance',source_sha256={s:hashlib.sha256((root/s).read_bytes()).hexdigest()for s in sources}),indent=2)+'\n')
