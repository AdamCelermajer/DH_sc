"""Isolated native retained-monster Idle composition; source kernels have separate original gold."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--assets',type=Path,default=REPO/'.local-inputs/character-monster-bank-assets/assets');p.add_argument('--reuse-snapshot',action='store_true');a=p.parse_args()
 scratch=REPO/'.local-inputs/character-idle-events';scratch.mkdir(exist_ok=True);snapshot=scratch/'stable-56-dependencies';snapshot.mkdir(exist_ok=True)
 commands=[]
 def run(args):
  r=subprocess.run(['wsl.exe','--',*args],text=True,capture_output=True,timeout=60);commands.append(dict(arguments=args,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr,(r.stdout,r.stderr);return r.stdout.strip()
 world='/home/adampalace/dh2-world-build/libdh2_level_world.so'
 snapshot_manifest=scratch/'stable-56-dependencies.json'
 if a.reuse_snapshot:
  original_hashes=json.loads(snapshot_manifest.read_text());libraries=list(original_hashes)
 else:
  inventory=run(['ldd',world]);libraries=[world]+re.findall(r'=> (/home/adampalace/dh2-world-build/\S+)',inventory)
  original_hashes={x:run(['sha256sum',x]).split()[0] for x in libraries}
  assert original_hashes[world]=='6ecf806974095c9ed4b3620483e2714cceb50d1a94b34f80d3ecffa513efc0a8','Expected parent stable56; refuse mutable central version'
  for source in libraries:run(['cp',source,linux(snapshot/Path(source).name)])
  assert original_hashes=={x:run(['sha256sum',x]).split()[0] for x in libraries}
  snapshot_manifest.write_text(json.dumps(original_hashes,indent=2)+'\n')
 assert original_hashes[world]=='6ecf806974095c9ed4b3620483e2714cceb50d1a94b34f80d3ecffa513efc0a8'
 copies={x.name:sha(x) for x in snapshot.glob('*.so')};assert len(copies)==len(libraries)
 for source,digest in original_hashes.items():assert copies[Path(source).name]==digest
 sources=[ROOT/x for x in ['character_idle_events.hpp','character_idle_events.cpp','character_idle_event_tables.inc','tests/character_idle_events.cpp','tests/character_idle_events_host.py','tests/character_script_session.cpp','character_ai_events.hpp','character_ai_events.cpp','character_ai_state_changed.hpp','character_ai_state_changed.cpp','character_ai_state_changed_vm.hpp','character_ai_state_changed_vm.cpp','character_animation_ai.hpp','character_animation_ai.cpp','character_state_owner.hpp','character_state_owner.cpp','character_state_owner_behavior.hpp','character_state_owner_behavior.cpp','character_native_fsm.hpp','character_native_fsm.cpp','character_animation_instance.hpp','character_animation_instance.cpp','actor_blended_playback.hpp','actor_blended_playback.cpp','character_script_session.hpp','character_script_session.cpp']]
 common=REPO/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac';monster=REPO/'.local-inputs/character-script-owner-extension/monster.luac'
 corpus=ROOT/'reference/character-game-design/real-cache-inputs.bin';dact=REPO/'port/android-native/app/src/main/assets/worlds/crypt01.dact';data=REPO/'port/android-native/app/src/main/assets/data'
 inputs=[common,monster,corpus,dact]+sorted(a.assets.rglob('*.bdae'))+sorted(a.assets.glob('data/*.bin'))+[data/x for x in ['animations_dictionary_pyarraynames.bin','animations_dictionary_pyarray.bin','animations_pyarray.bin','animations_pyarraynames.bin','animations_pystructnames.bin']]
 evidence=[ROOT/'reference/character-idle-events'/x for x in ['vtables.json','original-functions.json','reference/original-functions.asm']]
 paths=sources+inputs+evidence+[snapshot_manifest];before={p.relative_to(REPO).as_posix():sha(p) for p in paths}
 flags=['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror']
 exe=scratch/'idle_events_host';run(['g++',*flags,linux(ROOT/'character_idle_events.cpp'),linux(ROOT/'tests/character_idle_events.cpp'),'-L'+linux(snapshot),'-ldh2_level_world','-ldh2_script_runtime','-ldh2_game_data','-ldh2_engine_animation','-ldh2_scene_materials','-Wl,-rpath,'+linux(snapshot),'-ldl','-o',linux(exe)])
 environment=['env','LD_LIBRARY_PATH='+linux(snapshot),'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
 dependencies=run(environment+['ldd',linux(exe)])
 for x in libraries:assert linux(snapshot/Path(x).name) in dependencies
 result=json.loads(run(environment+[linux(exe),linux(corpus),linux(common),linux(monster),linux(dact),linux(a.assets),linux(data)]));assert result['validation']=='PASS' and result['actual_monster_Init']==11 and result['banks']==4
 assert before=={p.relative_to(REPO).as_posix():sha(p) for p in paths};assert copies=={x.name:sha(x) for x in snapshot.glob('*.so')}
 report=dict(validation='PASS',audit=result,source_and_input_sha256=before,parent_stable56_source_DSO_sha256=original_hashes,private_snapshot_sha256=copies,executable_sha256=sha(exe),commands=commands,linked_dependencies=dependencies,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],sanitizer_findings=0,central_rebuild=False,central_DSOs_private_copy=True,new_adapter_compiler_inputs_captured=True,existing_DSO_compiler_provenance_claim=False,whole_composition_original_instruction_differential=False,full_frame_or_physics_navigation=False,scope=__doc__)
 (ROOT/'reports/character-idle-events-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
