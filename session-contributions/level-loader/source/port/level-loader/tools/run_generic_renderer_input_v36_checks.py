from pathlib import Path
import subprocess,shlex,json,hashlib
root=Path(__file__).resolve().parents[3]
loader=root/'port/level-loader';build=root.parent/'build/receiver-transport-host'
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
source=(loader/'tests/canonical_module_graph_source_probe.cpp').read_text()
anchor='// Explicit graph/map fixture teardown, not whole Level/candidate destruction.'
start=r'''
 GenericRendererInputV36 generic_input;
 require(capture_generic_renderer_input_v36(preparation,roots,generic_input,e),e);
 require(generic_input.validate_live(preparation,e),e);
 require(generic_input.level_identity()==level->identity()&&generic_input.module_frames().size()==9,"generic transport replaced SAME Level/module graph");
 require(generic_input.authored_sources().size()==205,"generic transport lost original MLX/MGP/MVP source occurrences");
 unsigned generic_spawn_sources=0;
 for(const auto& authored:generic_input.authored_sources()){
  const auto* kind=authored.authored.source().attribute("gametype");
  if(kind&&*kind=="SpawnPoint")++generic_spawn_sources;
 }
 require(generic_spawn_sources==11,"generic transport lost authored spawn configuration");
 require(generic_input.registry_objects().size()==fixture.manager.source_count50(),"generic transport skipped actual registry entries");
 std::shared_ptr<floors::World> generic_navigation;
 require(generic_input.borrow_navigation(preparation,generic_navigation,e)&&generic_navigation==floors,"generic navigation copied/substituted floor world");
 world::CanonicalObjectBorrowV1 generic_actor;
 const auto registered=generic_input.registry_objects().front();
 require(generic_input.borrow_object(preparation,registered.registry_key,generic_actor,e)&&generic_actor.identity==registered.receiver.identity&&generic_actor.lease==registered.receiver.lease,"generic transport replaced canonical object lease");
 require(!generic_input.borrow_object(preparation,0,generic_actor,e)&&generic_actor.identity==registered.receiver.identity,"generic missing key corrupted caller borrow");
 const auto original_key=registered.receiver.shared_handle->key;
 registered.receiver.shared_handle->key=original_key+10000;
 require(!generic_input.validate_live(preparation,e),"generic transport accepted stale actual shared handle");
 registered.receiver.shared_handle->key=original_key;
 require(generic_input.validate_live(preparation,e),e);

 GenericRendererInputV36 generic_preserved=generic_input;
 require(!capture_generic_renderer_input_v36({},roots,generic_preserved,e)&&generic_preserved.level_identity()==generic_input.level_identity(),"invalid capture destroyed prior input");
 auto foreign_roots=std::make_shared<GameObjectSceneRootRegistryV1>();
 require(!capture_generic_renderer_input_v36(preparation,foreign_roots,generic_preserved,e)&&generic_preserved.module_frames().size()==9,"generic capture accepted foreign root registry");
 require(!generic_input.actor_visual_producer_bound()&&!generic_input.gameplay_readiness_claimed(),"generic transport fabricated actor visuals/readiness");
 // Explicit host-only dynamic-registration fixture, using genuine Dummy C1
 // and SAME ObjectManager::Add. It is not an authored SWAMP actor.
 auto dynamic_runtime=std::make_shared<actor::RuntimeState>();
 auto dynamic_dummy=std::make_shared<CanonicalDummyOwnerV14>(dynamic_runtime,*dynamic_runtime,GameObjectInitializationServicesV1{},DummyContinuationServicesV14{});
 target_providers::Handle16 dynamic_handle{};
 require(fixture.manager.add(dynamic_dummy->canonical(dynamic_dummy),"renderer_v36_runtime_registration_fixture","Dummy",-1,false,dynamic_handle,e),e);
 require(!generic_input.validate_live(preparation,e),"generic transport accepted changed canonical registry membership");
 require(capture_generic_renderer_input_v36(preparation,roots,generic_input,e)&&e.empty(),e);
 require(generic_input.validate_live(preparation,e)&&e.empty(),e);

'''
end=r'''
 require(!generic_input.validate_live(preparation,e),"released generic graph accepted stale submission");
 require(!generic_input.borrow_navigation(preparation,generic_navigation,e)&&generic_navigation==floors,"released generic graph returned navigation or corrupted prior borrow");
 require(!generic_input.borrow_object(preparation,registered.registry_key,generic_actor,e),"released generic graph returned stale actor");
 for(const auto& frame:generic_input.module_frames())for(const auto& draw:frame.meshes){assets::Mesh payload{};require(dh2_mesh_open(&payload,&frame.bres,draw.geometry)==assets::Error::ok,"generic resource pin expired after visual release");}
 std::cerr<<"GENERIC_RENDERER_V36 PASS module_frames="<<generic_input.module_frames().size()<<" authored_sources="<<generic_input.authored_sources().size()<<" registry_objects="<<generic_input.registry_objects().size()<<" spawn_sources="<<generic_spawn_sources<<" same_navigation=1 stale_rejected=1 resources_survive_release=1 actor_visuals_bound=0 gameplay_ready=0\n";
'''
assert source.count(anchor)==1
assert source.count('std::cout<<"{\\"validation\\":\\"PASS\\"')==1
source=source.replace(anchor,start+'\n'+anchor).replace('std::cout<<"{\\"validation\\":\\"PASS\\"',end+'\nstd::cout<<"{\\"validation\\":\\"PASS\\"')
source='#include "../generic_renderer_input_v36.hpp"\n#include "../generic_renderer_input_v36.cpp"\n'+source
probe=loader/'tests/generic_renderer_input_v36_probe.cpp';probe.write_text(source)
commands=subprocess.check_output(['wsl.exe','-d','Ubuntu','--','ninja','-C',linux(build),'-t','commands','dh2_loader_module_graph_source_probe'],text=True)
commands=commands.splitlines();compile_cmd=next(c for c in commands if ' -c ' in c and 'canonical_module_graph_source_probe.cpp' in c)
link_cmd=next(c for c in commands if ' -o dh2_loader_module_graph_source_probe ' in c)
compile_args=shlex.split(compile_cmd);oldobj=compile_args[compile_args.index('-o')+1];newobj=linux(build/'generic_renderer_input_v36_probe.o')
compile_args[compile_args.index('-o')+1]=newobj;compile_args[compile_args.index('-c')+1]=linux(probe)
if '-MF' in compile_args:compile_args[compile_args.index('-MF')+1]=newobj+'.d'
if '-MT' in compile_args:compile_args[compile_args.index('-MT')+1]=newobj
link_args=shlex.split(link_cmd)
if link_args[0]==':':link_args=link_args[link_args.index('&&')+1:]
if '&&' in link_args:link_args=link_args[:link_args.index('&&')]
link_args=[newobj if a==oldobj else a for a in link_args]
target=build/'generic_renderer_input_v36_probe';link_args[link_args.index('-o')+1]=linux(target)
def run(args,timeout=120):
 result=subprocess.run(['wsl.exe','-d','Ubuntu','--','sh','-c','cd '+shlex.quote(linux(build))+' && '+shlex.join(args)],capture_output=True,text=True,timeout=timeout)
 print(result.stdout,end='');print(result.stderr,end='')
 assert result.returncode==0,result.returncode
 return result
run(compile_args);run(link_args)

cache=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip')
fixture=loader/'tests/native-ctor-v4/design.bin'
saves=build/'native-ctor-missing-saves'
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
result=run([linux(target),linux(cache),linux(fixture),linux(saves)],timeout=30)
source_result=json.loads(result.stdout.strip().splitlines()[-1])
assert source_result['validation']=='PASS' and source_result['authored_objects_constructed']==195
assert source_result['actual_character_constructed']==50 and source_result['actual_container_constructed']==5
assert source_result['render_frame_meshes']==386 and not source_result['whole_level_init_verified']
assert 'GENERIC_RENDERER_V36 PASS' in result.stderr
receipt={
 'validation':'PASS',
 'scope':'Same retained Level/Module registry, original source XML, floor-world and mesh-resource transport; source-only host proof',
 'cache_sha256':sha(cache),'probe_sha256':sha(target),
 'source_hashes':{str(f.relative_to(loader)):sha(f) for f in [loader/'generic_renderer_input_v36.hpp',loader/'generic_renderer_input_v36.cpp',probe,loader/'tests/canonical_module_graph_source_probe.cpp']},
 'original_modules':9,'mesh_submissions':386,'authored_source_occurrences':205,
 'authored_spawn_points':11,'canonical_registry_before_fixture':214,'canonical_registry_after_fixture':215,
 'dynamic_dummy_registration_is_explicit_host_fixture':True,
 'same_floor_world_identity_verified':True,'same_actor_lease_verified':True,
 'foreign_root_registry_rejected':True,'shared_handle_key_change_rejected':True,
 'registry_new_key_requires_recapture':True,'released_graph_rejected':True,
 'navigation_and_object_failures_preserve_out':True,'invalid_capture_preserves_out':True,
 'mesh_resource_pins_survive_root_release':True,
 'actor_visual_producer_bound':False,'current_GSLevel_published':False,
 'whole_level_init_verified':False,'full_loader_verified':False,
 'original_source_result':source_result,
 'generic_probe_evidence':result.stderr.strip()
}
(loader/'reports/generic-renderer-input-v36-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps({'validation':'PASS','receipt':str(loader/'reports/generic-renderer-input-v36-verified.json')}))
