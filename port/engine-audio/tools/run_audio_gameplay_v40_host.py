from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];out=root/'.local-inputs/audio-v40';out.mkdir(exist_ok=True)
sources=['port/engine-audio/tests/audio_gameplay_runtime_v40_host.cpp','port/level-world/vox_play3d_owner_v2.cpp']+['port/engine-audio/'+s+'.cpp'for s in ['audio_clock_v40','audio_gameplay_runtime_v40','audio_source_command_v40','vox_source_fields_v38','audio_spatial_v34','audio_source_bindings_v38','audio_sample_v34','audio_mixer_v34','audio_catalog_v34','audio_bank_v34','audio_native_envelope_v34']]
results=[]
for opt in ['O1','O2']:
 exe=f'.local-inputs/audio-v40/gameplay-{opt}'
 p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','g++','-std=c++17',f'-{opt}','-g','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsanitize=address,undefined','-fno-omit-frame-pointer','-pthread',*sources,'-o',exe],cwd=root,capture_output=True,text=True,timeout=180);print(p.stderr);assert p.returncode==0
 p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',exe,'.'],cwd=root,capture_output=True,text=True,timeout=90);print(p.stdout,p.stderr);assert p.returncode==0;results.append(dict(optimization=opt,stdout=p.stdout,stderr=p.stderr,exit_code=p.returncode))
dest=root/'port/engine-audio/integration-v40/runtime';dest.mkdir(parents=True,exist_ok=True);(dest/'host.json').write_text(json.dumps(dict(scope='CPU decoded real-source positive gameplay transport and atomic/control lifetime fixtures; no output device or fake production GS',runs=results,source_sha256={p:hashlib.sha256((root/p).read_bytes()).hexdigest()for p in sources}),indent=2)+'\n')
