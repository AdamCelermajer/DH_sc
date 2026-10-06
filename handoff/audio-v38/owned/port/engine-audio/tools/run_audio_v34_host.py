from pathlib import Path
import subprocess,json,hashlib
root=Path(__file__).resolve().parents[3];out=root/'.local-inputs/audio-v34';out.mkdir(parents=True,exist_ok=True)
cache=out/'cache';(cache/'assets.txt').write_text('\n'.join(p.name for p in sorted(cache.iterdir())if p.suffix in ['.wav','.vxn'])+'\n')
sources=['port/engine-audio/tests/audio_v34_host.cpp','port/engine-audio/tests/audio_ima_v34_oracle.cpp','port/level-world/vox_audio_bridge_v34.cpp','port/level-world/vox_play3d_owner_v2.cpp']+['port/engine-audio/'+s+'.cpp'for s in ['audio_sample_v34','audio_mixer_v34','audio_catalog_v34','audio_bank_v34','audio_spatial_v34','audio_native_envelope_v34']]
results=[]
for opt in ['O1','O2']:
 exe=f'.local-inputs/audio-v34/audio-{opt}'
 p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','g++','-std=c++17',f'-{opt}','-g','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,'-o',exe],cwd=root,capture_output=True,text=True,timeout=180);print(p.stderr);assert p.returncode==0
 p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',exe,'.local-inputs/audio-v34/cache'],cwd=root,capture_output=True,text=True,timeout=180);print(p.stdout,p.stderr);assert p.returncode==0
 results.append(dict(optimization=opt,exit_code=p.returncode,stdout=p.stdout,stderr=p.stderr))
report=dict(scope='Actual original cache 261 stream cursor/codec/XML, decoded positive CPU output, timing/pause/pin/bank policy. No Android/audible acceptance.',results=results,sources={s:hashlib.sha256((root/s).read_bytes()).hexdigest()for s in sources})
(root/'port/engine-audio/reports').mkdir(exist_ok=True);(root/'port/engine-audio/reports/audio-v34-host.json').write_text(json.dumps(report,indent=2)+'\n')
