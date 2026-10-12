from pathlib import Path
import hashlib,json,datetime,subprocess
loader=Path(r'C:\Users\adamc\.codex\worktrees\generic-level-loader\DH_sc\port\level-loader');build=loader.parents[2]/'build/receiver-transport-v5-host';graph=loader/'vendor/character-rng-integration-v5-loading'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
manifest=json.loads((graph/'integration-manifest.json').read_text())
for n,item in manifest['files'].items():assert sha(graph/n)==item['sha256'],n
cache=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip');design=loader/'tests/native-ctor-v4/design.bin';saved=build/'native-ctor-missing-saves'
cases=[('gs_frame','dh2_loader_native_gslevel_frame_v45',[cache,design,saved]),('character_spawn','dh2_loader_character_spawn_v42',[cache,design]),('actor_image','dh2_loader_actor_image_v45_probe',[cache,design,saved]),('source_stage','dh2_loader_source_stage_swamp_v40',[cache,design,saved])]
cases += [('dynamic_module_'+str(mode),'dh2_loader_dynamic_module_v44',[cache,design,saved,str(mode)]) for mode in range(3)]
executions={}
for label,binary,args in cases:
 cmd=['wsl.exe','-d','Ubuntu','--',linux(build/binary)]+[linux(x) if isinstance(x,Path) else x for x in args]
 r=subprocess.run(cmd,capture_output=True,text=True,timeout=120);assert r.returncode==0,label+':'+r.stdout+r.stderr
 executions[label]={'command':cmd,'stdout':r.stdout,'stderr':r.stderr,'returncode':r.returncode,'binary_sha256':sha(build/binary)}
assert '"logical_identity":"SWAMP"' in executions['gs_frame']['stdout']
assert 'actual_actor270=1' in executions['character_spawn']['stdout']
assert 'samplers=8 no_image=3 actual_original_cache_reads=5' in executions['actor_image']['stderr']
sources=['native_gslevel_frame_v45.hpp','native_gslevel_frame_v45.cpp','retained_level_module_graph_v1.hpp','retained_level_module_graph_v1.cpp','generic_actor_image_binding_v45.hpp','generic_actor_image_binding_v45.cpp','material_image_identity_v43.hpp','material_image_identity_v43.cpp','tests/generic_actor_image_binding_v45_capture.inc','tests/character_spawn_selected_v45.cpp','tests/dynamic_module_source_v44.cpp','tests/cmake-receiver-transport/CMakeLists.txt']
receipt=dict(validation='PASS',recorded_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds'),coherent_files_verified=len(manifest['files']),manifest_sha256=sha(graph/'integration-manifest.json'),source_sha256={n:sha(loader/n) for n in sources},executions=executions,scope='Selected coherent native source/components; genuine GS C1 publication and frame1..3 prefix, real Character spawn cache/predicate, actual scoped chest samplers and checked dynamic-module record. Fixture providers do not establish whole lifecycle or gameplay.',source_image_counts={'samplers':8,'no_image':3,'actual_original_cache_reads':5},source_no_image_has_no_fallback=True,image_admission='required provider exercised with explicit component fixture; genuine main allocator binding pending',compatibility_uri_resolver='supplied-cache fixture, original full mount route remains required',dynamic_module_record_bound=True,dynamic_reused_bank_full_load=False,dynamic_source_stop='Required duplicate virtual deleting destructor',whole_level_init_verified=False,actual_gameplay_level_update_verified=False,actual_abort_teardown_verified=False,gpu_rendering_verified=False,emulator_launched=False)
out=loader/'reports/coherent-loader-v45-integrated-verified.json';out.write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps({'validation':'PASS','coherent_files':len(manifest['files']),'checks':len(cases),'GS_frame_prefix':True,'actual_spawn_cache':True,'chest_image_reads':5,'whole_loader':False}))
