from pathlib import Path
import subprocess,json,hashlib
r=Path(__file__).resolve().parents[3]
sources=['port/level-world/tests/combat_sound_runtime_v2.cpp','port/level-world/character_combat_sound_tables_v2.cpp','port/level-world/vox_play3d_owner_v2.cpp']
results=[]
for opt in ('O1','O2'):
 exe=f'.local-inputs/combat-sound-v2/runtime-{opt}'
 cmd=['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec']
 p=subprocess.run(cmd+['g++','-std=c++17',f'-{opt}','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,'-o',exe],cwd=r,capture_output=True,text=True);assert p.returncode==0,p.stderr
 p=subprocess.run(cmd+['env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',exe],cwd=r,capture_output=True,text=True);print(opt,p.returncode,p.stdout,p.stderr);assert p.returncode==0
 results.append(dict(optimization=opt,stdout=p.stdout,stderr=p.stderr))
 report=dict(scope='Actual three original CharSounds rows and source Play3D ordered gates, bank parameter preservation, fixed native floats, required failure prefix. Platform/native bank deliveries are observer fixtures; no actual audio playback claim.',results=results,sources={s:hashlib.sha256((r/s).read_bytes()).hexdigest() for s in sources})
 (r/'port/level-world/reports/combat-sound-runtime-v2-host.json').write_text(json.dumps(report,indent=2)+'\n')
