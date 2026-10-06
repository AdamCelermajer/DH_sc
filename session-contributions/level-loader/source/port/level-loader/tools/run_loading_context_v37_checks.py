from pathlib import Path
import datetime, hashlib, json, subprocess
loader=Path(__file__).resolve().parents[1]
repo=loader.parents[1]
build=repo.parent/'build/receiver-transport-v5-host'
graph=loader/'vendor/character-rng-integration-v5-loading'
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
manifest_path=graph/'integration-manifest.json'
manifest=json.loads(manifest_path.read_text())
for name,item in manifest['files'].items():assert sha(graph/name)==item['sha256'],name
cache_text=(build/'CMakeCache.txt').read_text()
assert 'DH2_LOADER_GRAPH_SNAPSHOT:PATH='+linux(graph) in cache_text
cache=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
def run(arguments):
    result=subprocess.run(['wsl.exe','-d','Ubuntu','--']+arguments,capture_output=True,text=True,timeout=60)
    assert result.returncode==0,result.stdout+'\n'+result.stderr
    return dict(command=['wsl.exe','-d','Ubuntu','--']+arguments,stdout=result.stdout,stderr=result.stderr,returncode=result.returncode)
native=build/'dh2_loader_integrated_loading_v37_probe'
loading=run([linux(native),linux(cache),linux(loader/'tests/native-ctor-v4/design.bin'),linux(build/'native-ctor-missing-saves')])
result=json.loads(loading['stdout'].strip().splitlines()[-1])
assert result['validation']=='PASS' and result['authored_objects_constructed']==195
assert result['actual_character_constructed']==50 and result['actual_container_constructed']==5
assert result['completed_module_source_pairs']==9 and result['render_frame_meshes']==386
assert not result['whole_level_init_verified'] and not result['current_GSLevel_published'] and not result['full_loader_verified']
assert 'LOADING_CONTEXT_V37 PASS actual_C1_alias=1 no_shadow_state=1 incomplete_C1_rejected=1 actual_progress=1 missing_stage_stops=1 world_ready=0' in loading['stderr']
trap=run([linux(build/'dh2_loader_selected_trigger_trap_v37_probe'),linux(loader/'reference/trigger-trap-v37/authored-trigger-trap-v37.tsv')])
assert 'PASS original_authored_cases=41 same_factory_manager_properties=1' in trap['stdout']
symbols=run(['nm','-C','--defined-only',linux(build/'loader/libdh2_loader_canonical_level_context.a')])
for function in ('retain_prepared_source','loading_fields_v26'):
    matches=[line for line in symbols['stdout'].splitlines() if ' T ' in line and 'CanonicalLevelContextV1::'+function+'(' in line]
    assert len(matches)==1,(function,matches)
inputs=[loader/'tests/cmake-receiver-transport/CMakeLists.txt',loader/'tests/canonical_loading_context_source_probe_v37.cpp',loader/'tests/trigger-trap-v37/canonical_trigger_trap_selected_v37_test.cpp',loader/'canonical_auxiliary_families_v16.hpp',loader/'canonical_auxiliary_families_v16.cpp',loader/'lifecycle_v36_counter_borrow.hpp',loader/'lifecycle_v36.cpp',loader/'generic_renderer_input_v36.cpp',loader/'generic_actor_visual_input_v37.cpp']
receipt=dict(validation='PASS',recorded_utc=datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds'),scope='Integrated private coherent Level C1/loading-field aliases and selected TriggerTrap source transport. Source preparation fixture order remains separate from authentic lifecycle activation.',selected_graph=str(graph),manifest_sha256=sha(manifest_path),coherent_files_verified=len(manifest['files']),actual_C1_alias_verified=True,incomplete_C1_rejected=True,progress_updates_actual_C1_storage=True,missing_stage_preserves_actual_state=True,prepared_source_definition_count=1,loading_field_definition_count=1,selected_trigger_trap_authored_cases=41,selected_trigger_trap_same_auxiliary_record=True,source_sha256={str(p.relative_to(loader)).replace('\\','/'):sha(p) for p in inputs},binary_sha256={native.name:sha(native),(build/'dh2_loader_selected_trigger_trap_v37_probe').name:sha(build/'dh2_loader_selected_trigger_trap_v37_probe')},executions=[loading,trap],native_SWAMP_result=result,whole_level_init_verified=False,current_GSLevel_published=False,trap_behavior_verified=False,renderer_backend_verified=False,emulator_launched=False)
path=loader/'reports/loading-context-v37-integrated-verified.json'
path.write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',coherent_files_verified=receipt['coherent_files_verified'],SWAMP_objects=195,trap_cases=41,actual_C1_alias=True,whole_loader=False,receipt=str(path))))
