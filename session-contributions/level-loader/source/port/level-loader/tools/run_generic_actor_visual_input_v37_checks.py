from pathlib import Path
import subprocess,shlex,json,hashlib,time
root=Path(__file__).resolve().parents[3];loader=root/'port/level-loader';build=root.parent/'build/receiver-transport-host'
linux=lambda p:'/mnt/c/'+str(p).replace('\\','/')[3:]
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
selected=[loader/'vendor/character-rng-integration-v4/port/level-loader/canonical_level_context_v1.hpp',loader/'vendor/character-rng-integration-v4/port/level-loader/canonical_level_context_v1.cpp',loader/'vendor/character-rng-integration-v4/port/level-loader/level_constructor_v3.hpp',loader/'vendor/character-rng-integration-v4/port/level-world/retained_gameobject_visual_v1.hpp',loader/'vendor/character-rng-integration-v4/port/level-world/retained_gameobject_visual_v1.cpp']
before={str(p.relative_to(loader)):sha(p) for p in selected}
source=(loader/'tests/canonical_module_graph_source_probe.cpp').read_text()
anchor='require(*containers.front()->receiver().base().string(0x290)'
teardown='// Explicit graph/map fixture teardown, not whole Level/candidate destruction.'
assert source.count(anchor)==1 and source.count(teardown)==1
source=source.replace(anchor,'\n#include "generic_actor_visual_input_v37_capture.inc"\n'+anchor)
source=source.replace(teardown,'\n#include "generic_actor_visual_input_v37_release.inc"\n'+teardown)
source='#include "../generic_actor_visual_input_v37.hpp"\n#include "../generic_actor_visual_input_v37.cpp"\n'+source
probe=loader/'tests/generic_actor_visual_input_v37_probe.cpp';probe.write_text(source)
commands=subprocess.check_output(['wsl.exe','-d','Ubuntu','--','ninja','-C',linux(build),'-t','commands','dh2_loader_module_graph_source_probe'],text=True).splitlines()
compile_cmd=next(c for c in commands if ' -c ' in c and 'canonical_module_graph_source_probe.cpp' in c)
link_cmd=next(c for c in commands if ' -o dh2_loader_module_graph_source_probe ' in c)
compile_args=shlex.split(compile_cmd);oldobj=compile_args[compile_args.index('-o')+1];newobj=linux(build/'generic_actor_visual_input_v37_probe.o')
compile_args[compile_args.index('-o')+1]=newobj;compile_args[compile_args.index('-c')+1]=linux(probe)
if '-MF' in compile_args:compile_args[compile_args.index('-MF')+1]=newobj+'.d'
if '-MT' in compile_args:compile_args[compile_args.index('-MT')+1]=newobj
link_args=shlex.split(link_cmd)
if link_args[0]==':':link_args=link_args[link_args.index('&&')+1:]
if '&&' in link_args:link_args=link_args[:link_args.index('&&')]
link_args=[newobj if a==oldobj else a for a in link_args]
target=build/'generic_actor_visual_input_v37_probe';link_args[link_args.index('-o')+1]=linux(target)
def run(args,timeout=120):
 result=subprocess.run(['wsl.exe','-d','Ubuntu','--','sh','-c','cd '+shlex.quote(linux(build))+' && '+shlex.join(args)],capture_output=True,text=True,timeout=timeout)
 print(result.stdout,end='');print(result.stderr,end='');assert result.returncode==0,result.returncode
 return result
run(compile_args);run(link_args)
cache=Path(r'C:\Users\adamc\Downloads\dungeonhunter2\Dungeon-Hunter-2-HD-v1-0-2-cache.zip');assert sha(cache)=='3fdf4e4c21d45a780a7c35fb4042abde0e88e76bf75416aad1f227481560b679'
start=time.monotonic();result=run([linux(target),linux(cache),linux(loader/'tests/native-ctor-v4/design.bin'),linux(build/'native-ctor-missing-saves')],30);elapsed=time.monotonic()-start
assert before=={str(p.relative_to(loader)):sha(p) for p in selected},'Selected producer tree changed during proof'
base=json.loads(result.stdout.strip().splitlines()[-1]);assert base['validation']=='PASS' and base['authored_objects_constructed']==195 and not base['whole_level_init_verified']
assert 'GENERIC_ACTOR_VISUAL_V37 PASS' in result.stderr
receipt={'validation':'PASS','scope':'checked synchronous read of SAME canonical actor Visual/scene/skin/BRES, not a persistent render frame or gameplay activation',
 'selected_graph_producer_hashes':before,'probe_sha256':sha(target),'cache_sha256':sha(cache),'elapsed_host_invocation_seconds':elapsed,
 'source_hashes':{str(p.relative_to(loader)):sha(p) for p in [loader/'generic_actor_visual_input_v37.hpp',loader/'generic_actor_visual_input_v37.cpp',loader/'tests/generic_actor_visual_input_v37_capture.inc',loader/'tests/generic_actor_visual_input_v37_release.inc',probe]},
 'actual_initialized_container_visual_consumed':True,'actual_scene_skin_refs_verified':True,'fresh_character_null_visual_animation_reported':True,
 'foreign_canonical_owner_rejected':True,'foreign_root_registry_rejected':True,'stale_released_visual_rejected':True,'reentrancy_rejected':True,
 'callback_and_transport_failure_preserve_out':True,'released_visual_clears_borrowed_BRES_confirmed':True,
 'independent_immutable_resource_pin_available':False,'positive_character_visual_verified':False,'current_GSLevel_published':False,'whole_level_init_verified':False,'full_loader_verified':False,
 'original_source_result':base,'actor_probe_evidence':result.stderr.strip()}
(loader/'reports/generic-actor-visual-input-v37-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps({'validation':'PASS','receipt':str(loader/'reports/generic-actor-visual-input-v37-verified.json')}))
