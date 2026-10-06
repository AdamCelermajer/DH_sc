from pathlib import Path
import datetime, hashlib, json, subprocess
loader=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader')
build=loader.parents[2]/'build/receiver-transport-v5-host'
graph=loader/'vendor/character-rng-integration-v5-loading'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
manifest=json.loads((graph/'integration-manifest.json').read_text())
for name,item in manifest['files'].items():assert sha(graph/name)==item['sha256'],name
binary=build/'dh2_loader_source_visual_helper_v41_probe'
cache=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
command=['wsl.exe','-d','Ubuntu','--',linux(binary),linux(cache),linux(loader/'tests/native-ctor-v4/design.bin'),linux(build/'native-ctor-missing-saves')]
r=subprocess.run(command,capture_output=True,text=True,timeout=120)
assert r.returncode==0,r.stdout+'\n'+r.stderr
assert 'SOURCE_VISUAL_HELPER_V41 PASS retained_instances=2 visible_instances=1 hidden_helper=1 primitives=2 native_skinned=2 rigid=0 unresolved_materials=0 helper_bounds_preserved=1 independent_render_byte_guard=1 whole_level_init=0 backend_rendering=0' in r.stderr
receipt=dict(validation='PASS',recorded_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds'),scope='Actual retained chest Visual C1 helper suppression and synchronous native draw submissions in coherent SWAMP source inspection; no GPU backend or whole lifecycle execution.',coherent_files_verified=len(manifest['files']),manifest_sha256=sha(graph/'integration-manifest.json'),binary_sha256=sha(binary),command=command,stdout=r.stdout,stderr=r.stderr,retained_instances=2,visible_instances=1,hidden_helper=1,skinned_primitives=2,rigid_primitives=0,unresolved_materials=0,helper_bounds_preserved=True,independent_render_byte_guard=True,whole_level_init_verified=False,backend_rendering_verified=False,emulator_launched=False)
sources=['tests/source_visual_helper_v41_probe.cpp','tests/source_visual_helper_v41_capture.inc','generic_actor_draw_submission_v38.cpp','scene_material_slot_v39.cpp','render_eligibility_v40.cpp','vendor/character-rng-integration-v5-loading/port/level-world/retained_gameobject_visual_v1.hpp','vendor/character-rng-integration-v5-loading/port/level-world/retained_gameobject_visual_v1.cpp']
receipt['sources']={s:sha(loader/s) for s in sources}
out=loader/'reports/source-visual-helper-v41-integrated-verified.json'
out.write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',coherent_files_verified=len(manifest['files']),hidden_helper=1,skinned_primitives=2,backend_rendering=False,receipt=str(out))))
