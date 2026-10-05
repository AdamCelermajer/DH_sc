from pathlib import Path
import subprocess,json,hashlib
r=Path(__file__).resolve().parents[3]
sources=['port/engine-animation/tests/particle_resource_init_v1.cpp','port/engine-animation/particle_resource_init_v1.cpp','port/engine-animation/particle_force_scene_v1.cpp','port/engine-animation/particle_scene_color_v1.cpp','port/engine-animation/particle_factory.cpp','port/scene-materials/particle_scene_v1.cpp']
snapshot='.local-inputs/character-fx-owner-v1/host-snapshot'
results=[]
for opt in ('O1','O2'):
 exe=f'.local-inputs/combat-hit-fx-v1/particle-resource-{opt}-host'
 compile=['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','g++','-std=c++17',f'-{opt}','-g','-Wall','-Wextra','-Werror','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,f'-L{snapshot}','-ldh2_engine_animation','-ldh2_scene_materials','-o',exe]
 p=subprocess.run(compile,cwd=r,capture_output=True,text=True);assert p.returncode==0,p.stderr
 p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','env',f'LD_LIBRARY_PATH={snapshot}','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',exe,'.local-inputs/combat-hit-fx-v1/bloodsplat.bdae','.local-inputs/combat-hit-fx-v1/bloodsplat_hero.bdae'],cwd=r,capture_output=True,text=True)
 print(opt,p.returncode,p.stdout,p.stderr);assert p.returncode==0
 results.append({'optimization':opt,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
 digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 report={'scope':'Actual original-cache blood79/80 emitter parameter initialization, authored scene/material graph and named gravity-node binding. Particle simulation/billboard baking/positive manager creation and GPU draw are required continuations, not tested as completed here.','results':results,'sources':{s:digest(r/s) for s in sources},'assets':{n:digest(r/'.local-inputs/combat-hit-fx-v1'/n) for n in ('bloodsplat.bdae','bloodsplat_hero.bdae')},'dependencies':{p.name:digest(p) for p in (r/snapshot).glob('*.so')}}
 out=r/'port/engine-animation/reports/particle-resource-init-v1-host-audit.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n')
