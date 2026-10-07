from pathlib import Path
import subprocess,json,hashlib
r=Path(__file__).resolve().parents[3]
sources=['port/level-world/tests/character_authored_particle_fx_v3.cpp','port/level-world/character_authored_particle_fx_v3.cpp']+[f'port/engine-animation/{s}.cpp' for s in ['particle_resource_init_v2','particle_force_scene_v1','particle_scene_color_v1','particle_factory','particle_random_v1','particle_emission','particle_cloud_models_v1','particle_cloud_runtime_v1','particle_cloud_runtime_v2','particle_billboard_v1','particle_box_v2','material_color','material_color_v3']]+['port/scene-materials/particle_scene_v1.cpp']
snapshot='.local-inputs/character-fx-owner-v1/host-snapshot';results=[]
assets=['bloodsplat.bdae','bloodsplat_hero.bdae','spell_dh2_hurt_fire.bdae']
for opt in ('O1','O2'):
 exe=f'.local-inputs/combat-hit-fx-v1/authored-positive-v3-{opt}-host'
 p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','g++','-std=c++17',f'-{opt}','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,f'-L{snapshot}','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_script_runtime','-o',exe],cwd=r,capture_output=True,text=True)
 assert p.returncode==0,p.stderr
 p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','env',f'LD_LIBRARY_PATH={snapshot}','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',exe,*['.local-inputs/combat-hit-fx-v1/'+s for s in assets]],cwd=r,capture_output=True,text=True)
 print(opt,p.returncode,p.stdout,p.stderr);assert p.returncode==0
 results.append(dict(optimization=opt,exit_code=p.returncode,stdout=p.stdout,stderr=p.stderr))
digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report=dict(scope='Actual original-cache blood79/80 and FIRE124 descriptor3/mode0 sphere/box resource initialization, same generation/RNG/model simulation, force world/billboard retained snapshots, actual full uchar4 material animation application and source completion. Original full color600 gold replay. Camera explicit fixture; no actual GPU/live receipt.',results=results,sources={s:digest(r/s) for s in sources},assets={s:digest(r/'.local-inputs/combat-hit-fx-v1'/s) for s in assets})
report['source_headers']={s:digest(r/s) for s in ['port/level-world/character_authored_particle_fx_v3.hpp','port/level-world/character_particle_fx_resource_v2.hpp','port/engine-animation/material_color_v3.hpp','port/engine-animation/particle_box_v2.hpp','port/engine-animation/particle_cloud_runtime_v2.hpp','port/engine-animation/particle_resource_init_v2.hpp']}
(r/'port/level-world/reports/character-authored-particle-fx-v3-host.json').write_text(json.dumps(report,indent=2)+'\n')
