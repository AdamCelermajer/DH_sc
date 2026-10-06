from pathlib import Path
import subprocess,json,hashlib,sys
root=Path(__file__).resolve().parents[3]
out=root/'.local-inputs/authored-fx-v32';out.mkdir(exist_ok=True)
version=sys.argv[1] if len(sys.argv)>1 else '6'
mode=sys.argv[2] if len(sys.argv)>2 else 'census'
sources=[f'port/level-world/tests/character_authored_resource_domains_v{version}.cpp',f'port/level-world/character_authored_resource_v{version}.cpp',f'port/level-world/authored_fx_mesh_graph_v{version}.cpp']
sources+=['port/level-world/'+s+'.cpp' for s in ['authored_fx_transform_v5','character_authored_fx_forces_v4','source_fx_node_matrix_v4','fx_texture_animation_v1','character_fx_kernels_v1']]
sources+=['port/engine-animation/'+s+'.cpp' for s in ['particle_scalar_animation_v6','particle_cloud_runtime_v1','particle_cloud_runtime_v3','particle_deflector_v1','particle_force_scene_v2','particle_bound_forces_v4','particle_resource_init_v2','particle_cloud_models_v1','particle_billboard_v1','particle_force_scene_v1','particle_scene_color_v1','particle_factory','particle_random_v1','particle_emission','particle_box_v2','material_color','material_color_v3']]
sources+=['port/scene-materials/particle_scene_v1.cpp']
if version=='32':sources+=['port/engine-animation/particle_resource_init_v32.cpp','port/engine-animation/particle_billboard_v32.cpp','port/level-world/authored_fx_nonrender_geometry_v32.cpp']
if mode=='packets':
 sources[0]='port/level-world/tests/authored_fx_geometry_packet_v32.cpp';sources+=['port/level-world/authored_fx_geometry_packet_v7.cpp']
if mode=='source':
 sources[0]='port/level-world/tests/authored_fx_source_v32_host.cpp';sources+=['port/level-world/tests/authored_fx_source_v32_oracle.cpp']
if mode=='skin':sources[0]='port/level-world/tests/authored_fx_skin_scalar_v32.cpp'
snapshot='.local-inputs/character-fx-owner-v1/host-snapshot'
exe=f'.local-inputs/authored-fx-v32/{mode}-v{version}'
cmd=['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','g++','-std=c++17','-Iport/game-data','-Iport/engine-animation','-Iport/engine-resources','-Iport/engine-skinning','-Iport/scene-materials','-Iport/asset-payloads','-Iport/engine-math','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer',*sources,f'-L{snapshot}','-ldh2_level_world','-ldh2_game_data','-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_script_runtime','-o',exe]
p=subprocess.run(cmd,cwd=root,capture_output=True,text=True);(out/f'build-{mode}-v{version}.log').write_text(p.stdout+p.stderr);print(p.stderr);assert p.returncode==0
arguments=['port/level-world/reference/shared-target-facing-v1/cache/general-v5']
if mode=='source':arguments+=['port/level-world/reference']
p=subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','env',f'LD_LIBRARY_PATH={snapshot}','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',exe,*arguments],cwd=root,capture_output=True,text=True)
(out/f'{mode}-v{version}.log').write_text(p.stdout+p.stderr);print(p.stdout,p.stderr);assert p.returncode==0
(root/f'port/level-world/reports/authored-fx-v32-{mode}-v{version}.json').write_text(json.dumps(dict(exit_code=p.returncode,stdout=p.stdout,stderr=p.stderr,sources={s:hashlib.sha256((root/s).read_bytes()).hexdigest() for s in sources}),indent=2)+'\n')



