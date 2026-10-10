from pathlib import Path
import hashlib, json, subprocess, sys

root = Path(__file__).resolve().parents[4]
out = root / '.local-inputs/windows-effects-feature'
out.mkdir(exist_ok=True)
cache = root / 'port/level-world/reference/shared-target-facing-v1/cache/general-v5'
mapping = {}
receipts = []
for row in json.loads((cache/'manifest.json').read_text()):
    path = cache/row['local']
    assert hashlib.sha256(path.read_bytes()).hexdigest() == row['sha256']
    mapping[row['uri'].lower()] = path.relative_to(root).as_posix()
    receipts.append({'uri':row['uri'],'path':mapping[row['uri'].lower()],'sha256':row['sha256']})
for directory in ('port/level-world/reference/shared-target-facing-v1/cache/melee-v8',
                  '.local-inputs/combat-hit-fx-v1'):
    for path in (root/directory).glob('*.bdae'):
        uri = 'data/3d/interface/'+path.name.lower()
        mapping[uri] = path.relative_to(root).as_posix()
        receipts.append({'uri':uri,'path':mapping[uri],'sha256':hashlib.sha256(path.read_bytes()).hexdigest()})
(out/'assets.tsv').write_text(''.join(uri+'\t'+path+'\n' for uri,path in mapping.items()))
feature = 'port/windows-foundation/features/effects/'
sources = [feature+'effects_executor_tests.cpp', feature+'effects_executor.cpp']
render = '--render' in sys.argv
if render:
    sources[0]=feature+'effects_render_stream_tests.cpp'
    sources += [feature+'effects_render_bridge.cpp',feature+'effects_material_binding.cpp']
compiler = root/'.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe'
native_results = []
if compiler.exists():
    for source in sources:
        result = subprocess.run([str(compiler),'-std=c++17','-Wall','-Wextra','-Werror',
            '-c',source,'-o',str(out/(Path(source).stem+'.o'))],cwd=root,capture_output=True,text=True)
        native_results.append({'source':source,'exit_code':result.returncode,'stderr':result.stderr})
        assert result.returncode == 0, result.stderr
sources += ['port/level-world/'+name+'.cpp' for name in [
    'character_authored_resource_v32','authored_fx_mesh_graph_v32',
    'authored_fx_nonrender_geometry_v32','character_mesh_fx_owner_v4',
    'visual_fx_manager_libraries_v63','character_fx_state_v1','character_animation_step_fx_v2',
    'authored_fx_transform_v5','character_authored_fx_forces_v4','source_fx_node_matrix_v4',
    'fx_texture_animation_v1','character_fx_kernels_v1']]
sources += ['port/engine-animation/'+name+'.cpp' for name in [
    'particle_scalar_animation_v6','particle_cloud_runtime_v1','particle_cloud_runtime_v3',
    'particle_deflector_v1','particle_force_scene_v2','particle_bound_forces_v4',
    'particle_resource_init_v2','particle_cloud_models_v1','particle_billboard_v1',
    'particle_force_scene_v1','particle_scene_color_v1','particle_factory','particle_random_v1',
    'particle_emission','particle_box_v2','material_color','material_color_v3',
    'particle_resource_init_v32','particle_billboard_v32']]
sources += ['port/scene-materials/particle_scene_v1.cpp']
sources += ['port/scene-materials/scene.cpp','port/engine-skinning/skinning.cpp',
            'port/engine-animation/animation.cpp','port/engine-animation/component_applicator.cpp']
if render:
    sources += ['port/windows-foundation/'+name+'.cpp' for name in
        ('asset_catalog','content_paths','texture_loader','source_material_pass','actor_lighting')]
    sources += ['port/engine-textures/textures.cpp','port/engine-textures/pvrtc.cpp',
                'port/scene-materials/effect_render_pass_v4.cpp','port/scene-materials/blood_render_pass_v3.cpp']
    sources += ['port/level-loader/vendor/tinyxml/'+name+'.cpp' for name in ('tinyxml','tinyxmlerror','tinyxmlparser','tinystr')]
snapshot = '.local-inputs/character-fx-owner-v1/host-snapshot'
exe = '.local-inputs/windows-effects-feature/'+('effects-render-tests' if render else 'effects-tests')
cmd = ['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec','g++',
       *['-Iport/'+d for d in ('game-data','engine-animation','engine-resources','engine-skinning',
                             'scene-materials','asset-payloads','engine-math','level-world')],
       '-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined',
       '-fno-omit-frame-pointer',*sources,'-L'+snapshot,'-ldh2_level_world','-ldh2_game_data',
       '-ldh2_engine_skinning','-ldh2_engine_animation','-ldh2_scene_materials','-ldh2_script_runtime',
       '-o',exe]
build = subprocess.run(cmd,cwd=root,capture_output=True,text=True)
(out/'build.log').write_text(build.stdout+build.stderr)
if build.returncode:
    print(build.stdout+build.stderr)
    raise SystemExit(build.returncode)
args = [exe,'port/game-data/reference/effects-tables','.local-inputs/windows-effects-feature/assets.tsv']
if render: args += ['.local-inputs/windows-shared-assets','.local-inputs/windows-effects-feature/render-fixture.bin']
run = subprocess.run(['wsl.exe','--cd','/mnt/c/Users/adamc/Desktop/workspace/DH_sc','--exec',
    'env','LD_LIBRARY_PATH='+snapshot,'ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=halt_on_error=1',
    *args],
    cwd=root,capture_output=True,text=True)
(out/'run.log').write_text(run.stdout+run.stderr)
print(run.stdout+run.stderr)
report = {'feature':'effects','validation':'PASS' if run.returncode==0 else 'FAIL',
    'test_exit_code':run.returncode,'stdout':run.stdout,'stderr':run.stderr,
    'required_original_continuations':[line for line in run.stdout.splitlines() if line.startswith('REQUIRED | ')],
    'passed_original_set_bindings':sum(line.startswith('PASS | ') for line in run.stdout.splitlines()),
    'distinct_passed_original_uris':sorted({line.removeprefix('PASS | ') for line in run.stdout.splitlines() if line.startswith('PASS | ')}),
    'sources':{s:hashlib.sha256((root/s).read_bytes()).hexdigest() for s in sources},
    'actual_assets':receipts,'native_windows_strict_compile':native_results,
    'runtime_backend':'borrowed CharacterMeshFxOwnerV4, source resource factory V32',
    'live_renderer_integrated':False,'same_scene_anchor_camera_fixture':True,
    'unsupported':['Missing original resources explicitly reject; no replacements.',
                   'FaeryGlow source producer is not established by EffectsTables.',
                   'Equipment Swoosh gate and actual socket identity must be supplied by original producer.',
                   'Renderer must submit source mesh/particle metadata in actual SceneManager order.']}
if render:
    prior=json.loads((root/'reports/feature-effects.json').read_text())
    prior['render_stream_validation']=report
    report=prior
(root/'reports/feature-effects.json').write_text(json.dumps(report,indent=2)+'\n')
raise SystemExit(run.returncode)
