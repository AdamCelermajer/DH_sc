from pathlib import Path
import hashlib,json,datetime,subprocess,sys
loader=Path(__file__).resolve().parents[1];build=loader.parents[2]/'build/receiver-transport-v5-host';graph=loader/'vendor/character-rng-integration-v5-loading'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();linux=lambda p:'/mnt/c/'+str(p).replace(chr(92),'/')[3:]
manifest_path=graph/'integration-manifest.json';manifest=json.loads(manifest_path.read_text())
for n,item in manifest['files'].items():assert sha(graph/n)==item['sha256'],n
start_manifest=sha(manifest_path)
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');design=loader/'tests/native-ctor-v4/design.bin';saved=build/'native-ctor-missing-saves'
cases=[('character_name','dh2_loader_character_name_v48',[cache,design]),('character_spawn','dh2_loader_character_spawn_name_v48',[cache,design]),('character_model','dh2_loader_character_model_v47',[cache,design]),('gs_frame','dh2_loader_native_gslevel_frame_v45',[cache,design,saved]),('source_stage','dh2_loader_source_stage_swamp_v47',[cache,design,saved]),('integrated_loading','dh2_loader_integrated_loading_v47_probe',[cache,design,saved]),('actor_image_lifetime','dh2_loader_actor_image_lifetime_v48_probe',[cache,design,saved]),('cancel_reentry','dh2_loader_cancel_reentry_v43',[])]
cases += [('dynamic_module_'+str(mode),'dh2_loader_dynamic_module_v44',[cache,design,saved,str(mode)]) for mode in range(3)]
executions={};failures=[]
for label,binary,args in cases:
 cmd=['wsl.exe','-d','Ubuntu','--',linux(build/binary)]+[linux(x) if isinstance(x,Path) else x for x in args]
 r=subprocess.run(cmd,capture_output=True,text=True,timeout=120)
 executions[label]={'command':cmd,'stdout':r.stdout,'stderr':r.stderr,'returncode':r.returncode,'binary_sha256':sha(build/binary)}
 if r.returncode:failures.append(label)
 print(label+': '+str(r.returncode)+' '+(r.stdout+r.stderr)[-1400:],flush=True)
assert start_manifest==sha(manifest_path),'Manifest changed during verification'
sources=['native_gslevel_frame_v45.hpp','native_gslevel_frame_v45.cpp','retained_level_module_graph_v1.hpp','retained_level_module_graph_v1.cpp','generic_actor_image_binding_v45.hpp','generic_actor_image_binding_v45.cpp','tests/generic_actor_image_lifetime_v48_probe.cpp','tests/source_stage_swamp_v47.cpp','tests/canonical_loading_context_source_probe_v47.cpp','tests/name-selected-v48/character_name_predicate_v48_test.cpp','tests/name-selected-v48/character_spawn_name_v48_regression.cpp','tests/cmake-receiver-transport/CMakeLists.txt']
receipt=dict(validation='FAIL' if failures else 'PASS',recorded_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds'),coherent_files_verified=len(manifest['files']),manifest_sha256=start_manifest,source_sha256={n:sha(loader/n) for n in sources},executions=executions,failed_checks=failures,scope='Current coherent selected native components; original NAME predicate with same factory receiver, ordered model continuation, qualified base condition, exact source7/10 preparation, real chest image ownership and retained GS prefix. Engine platform/condition/RNG-domain transports remain explicit test fixtures.',prior_predicate_claim_retracted='Original IsPlayer reads NAME30/buffer44, not archetype48/buffer5c. V41/V42 archived semantic labels superseded; no frozen packet overwritten.',whole_level_init_verified=False,actual_gameplay_level_update_verified=False,actual_abort_teardown_verified=False,gpu_rendering_verified=False,emulator_launched=False)
out=loader/'reports/coherent-loader-v49-integrated-verified.json';out.write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps({'validation':receipt['validation'],'coherent_files':len(manifest['files']),'checks':len(cases),'failures':failures,'whole_loader':False}))
sys.exit(bool(failures))
