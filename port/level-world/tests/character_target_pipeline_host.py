"""Actual monster Lua through source router/prefix/retained target objects.

Host composition audit, not a whole-chain original-instruction differential.
The new test and frozen EnemySpotted shim are compiled here. Other source kernels
execute from the actual central sanitized DSOs; their build provenance belongs
to the central binder. Hashes before/after prohibit moving dependencies.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
WORLD=Path(__file__).resolve().parents[1];ROOT=WORLD.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-world-build');a=p.parse_args()
 scratch=ROOT/'.local-inputs/character-target-pipeline';scratch.mkdir(parents=True,exist_ok=True)
 sources=[WORLD/'tests/character_target_pipeline.cpp',Path(__file__),WORLD/'character_enemy_spotted.cpp',WORLD/'character_enemy_spotted.hpp']
 production=['character_ai_events','character_target_events','character_target_bindings','character_script_objects','character_script_session','character_script_commands','character_controller_commands','character_path_commands','character_design_services','character_game_design']
 interfaces=[WORLD/(n+ext) for n in production for ext in ['.cpp','.hpp']]
 interfaces += [ROOT/'port/script-runtime'/n for n in ['script_runtime.h','script_runtime.c','script_object_bridge.h','script_object_bridge.c']]
 inputs=[WORLD/'reference/character-game-design/real-cache-inputs.bin',ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',ROOT/'.local-inputs/character-script-owner-extension/monster.luac',WORLD/'reference/character-enemy-spotted/enemy-spotted-fixtures.bin']
 proofs=[WORLD/'reports'/n for n in ['character-enemy-spotted-arm64-differential.json','character-target-events-arm64-differential.json','character-target-event-route-arm64-differential.json']]
 proofs += [WORLD/'reference/character-target-event-route/endpoints-original-probe.json']
 allpaths=sources+interfaces+inputs+proofs
 before={x.relative_to(ROOT).as_posix():sha(x) for x in allpaths};commands=[]
 def run(*args,timeout=120):
  r=subprocess.run(['wsl.exe','--',*args],text=True,capture_output=True,timeout=timeout);commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,(r.stdout,r.stderr);return r.stdout
 def hashes(paths):return {line.split(maxsplit=1)[1].strip():line.split()[0] for line in run('sha256sum',*sorted(paths)).splitlines()}
 deps={a.build+'/libdh2_level_world.so',a.build+'/game-data/libdh2_game_data.so',a.build+'/script-runtime/libdh2_script_runtime.so'}
 frozen=hashes(deps)
 flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation']
 prefix=linux(scratch/'libtarget_pipeline_prefix.so');exe=linux(scratch/'character_target_pipeline_audit')
 run('g++',*flags,'-shared','-fPIC',linux(WORLD/'character_enemy_spotted.cpp'),'-o',prefix)
 links=['-L'+linux(scratch),'-ltarget_pipeline_prefix','-Wl,-rpath,'+linux(scratch)]
 for sub,lib in [('', 'level_world'),('/game-data','game_data'),('/script-runtime','script_runtime')]:links += ['-L'+a.build+sub,'-ldh2_'+lib,'-Wl,-rpath,'+a.build+sub]
 run('g++',*flags,linux(sources[0]),*links,'-ldl','-o',exe)
 loaded=run('ldd',exe);assert 'not found' not in loaded and 'libasan.so' in loaded and 'libubsan.so' in loaded
 dependencies={exe}
 for line in loaded.splitlines():
  m=re.search(r'=>\s+(/\S+)|^\s*(/\S+)',line)
  if m:dependencies.add(m.group(1) or m.group(2))
 assert deps.issubset(dependencies) and prefix in dependencies
 binary=hashes(dependencies);assert all(binary[x]==h for x,h in frozen.items())
 result=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,*map(linux,inputs)))
 assert result['validation']=='PASS' and result['gate_cases']==72 and result['original_monster_Init']==1 and result['nested_source_events']==1
 assert result['world_library']==a.build+'/libdh2_level_world.so' and result['prefix_library']==prefix and result['runtime_library']==a.build+'/script-runtime/libdh2_script_runtime.so'
 assert before=={x.relative_to(ROOT).as_posix():sha(x) for x in allpaths} and binary==hashes(dependencies)
 report=dict(validation='PASS',host_audit=result,source_and_input_sha256=before,binary_sha256=binary,commands=commands,dependencies=loaded,sanitizer_findings=0,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],
  scope=__doc__,source_prefix_shim_compiled_here=True,central_DSO_compile_provenance='not inferred; dependency byte hashes and dladdr verified',
  genuine_components=['real cache CharacterGameDesign/constants/property/class/AI decoding','actual commons and monster top-level/Init/events','owned DebugSwitches empty-map/missing-file load','retained CharacterScriptObjects and actual target setter/dead/sight/object methods','full source event9..17 router and target prefixes','Lua command/controller/Character/PathTo kernels','private same-VM nested source-object dispatch and finalizer scope'],
  explicit_fixtures=['filesystem missing-file open','host cached level/current row/difficulty','GroupInfo delivery','spawn/limbus/combat queries','GetAggro/AddAggro/ClearAggro map ownership','controller/remote/Stop/event3f/FSM bodies','FindPath failed result','SoundManager Play3D','test-only ClearAggro Lua wrapper suffix'],
  boundaries=['CreateBuff and possible DoSkill/GetRand remain unsupported with observed prefix errors','retained map is native lifetime owner, not original ObjectManager/handle routing','source threat borrowed from actual original-loader gold, no full native DesignSettings owner','no physics/navigation movement, frame/discovery/sight producer or live enemy AI'],
  whole_chain_original_instruction_differential=False,packaged_APK=False,physical_ARM64=False)
 (WORLD/'reports/character-target-pipeline-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
