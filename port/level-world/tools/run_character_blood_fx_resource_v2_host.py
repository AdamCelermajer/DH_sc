from pathlib import Path
import subprocess,json,hashlib
r=Path(__file__).resolve().parents[3]
sources=['port/level-world/tests/character_blood_fx_resource_v2.cpp','port/level-world/character_blood_fx_resource_v2.cpp','port/level-world/character_mesh_fx_owner_v2.cpp']+[f'port/engine-animation/{s}.cpp' for s in ['particle_resource_init_v1','particle_force_scene_v1','particle_scene_color_v1','particle_factory','particle_random_v1','particle_emission','particle_cloud_models_v1','particle_cloud_runtime_v1','particle_billboard_v1']]+['port/scene-materials/particle_scene_v1.cpp']+['port/level-world/'+s+'.cpp' for s in ('character_fx_kernels_v1','character_fx_state_v1','fx_texture_animation_v1')]
snapshot='.local-inputs/character-fx-owner-v1/host-snapshot'
results=[]
for opt in ('O1','O2'):
 exe=f'.local-inputs/combat-hit-fx-v1/blood-positive-v2-{opt}-host'
 p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','g++','-std=c++17',f'-{opt}','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,f'-L{snapshot}','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_script_runtime','-o',exe],cwd=r,capture_output=True,text=True)
 assert p.returncode==0,p.stderr
 p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','env',f'LD_LIBRARY_PATH={snapshot}','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',exe,'.local-inputs/combat-hit-fx-v1/bloodsplat.bdae','.local-inputs/combat-hit-fx-v1/bloodsplat_hero.bdae'],cwd=r,capture_output=True,text=True)
 print(opt,p.returncode,p.stdout,p.stderr);assert p.returncode==0
 results.append({'optimization':opt,'exit_code':p.returncode,'stdout':p.stdout,'stderr':p.stderr})
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report={'scope':'Actual original-cache blood79/80 complete native CPU resource: source emitter/force/material scene graph, source segment1000ms and BirthRate scalar sampling, same source RNG/cloud model simulation, billboard/UV/color/index retained snapshots, manager completion/pool/warm reuse. Camera is an explicit host fixture; actual live renderer camera binding and GPU submission are not verified by this host test. Frozen V1 owner sources remain unchanged.','results':results,'sources':{s:digest(r/s) for s in sources},'assets':{n:digest(r/'.local-inputs/combat-hit-fx-v1'/n) for n in ('bloodsplat.bdae','bloodsplat_hero.bdae')},'dependencies':{p.name:digest(p) for p in (r/snapshot).glob('*.so')}}
out=r/'port/level-world/reports/character-blood-fx-resource-v2-host-audit.json';out.parent.mkdir(parents=True,exist_ok=True);out.write_text(json.dumps(report,indent=2)+'\n')
