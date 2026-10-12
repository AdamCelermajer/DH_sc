"""Source-gold SAN replay plus real Crypt delayed startup through queued entry.

The composition borrows private snapshots of actual central native DSOs. It
executes real FSM/CPU banks/private commons+monster/deferred loader/vitals/debug.
Skills, final Kill/AIUnload and App/CanUpdate upstream producers are explicit
fixtures; this is not a full original Character frame or live world authority.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--assets',type=Path,default=REPO/'.local-inputs/character-monster-bank-assets/assets');p.add_argument('--reuse-snapshot',action='store_true');a=p.parse_args()
 scratch=REPO/'.local-inputs/character-update-queued';scratch.mkdir(exist_ok=True);snapshot=scratch/'dependencies';snapshot.mkdir(exist_ok=True);manifest=scratch/'dependencies.json';commands=[]
 def run(args):
  result=subprocess.run(['wsl.exe','--',*args],text=True,capture_output=True,timeout=60);commands.append(dict(arguments=args,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert not result.returncode and not result.stderr,(result.stdout,result.stderr);return result.stdout.strip()
 world=a.build+'/libdh2_level_world.so'
 if a.reuse_snapshot:parent_hashes=json.loads(manifest.read_text())
 else:
  libraries=[world]+re.findall(r'=> ('+re.escape(a.build)+r'/\S+)',run(['ldd',world]));parent_hashes={x:run(['sha256sum',x]).split()[0] for x in libraries}
  for source in libraries:run(['cp',source,linux(snapshot/Path(source).name)])
  assert parent_hashes=={x:run(['sha256sum',x]).split()[0] for x in libraries},'Changed central inputs; refuse incoherent snapshot'
  manifest.write_text(json.dumps(parent_hashes,indent=2)+'\n')
 copies={x.name:sha(x) for x in snapshot.glob('*.so')};assert len(copies)==len(parent_hashes)
 for source,value in parent_hashes.items():assert copies[Path(source).name]==value
 common=REPO/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac';monster=REPO/'.local-inputs/character-script-owner-extension/monster.luac';corpus=ROOT/'reference/character-game-design/real-cache-inputs.bin';dact=REPO/'port/android-native/app/src/main/assets/worlds/crypt01.dact';data=REPO/'port/android-native/app/src/main/assets/data'
 tests=['tests/character_update_queued.cpp','tests/character_update_queued_session.cpp','tests/character_update_queued_host.py','tests/character_update_queued_differential.py','tests/character_update_startup.cpp','tests/character_script_session.cpp']
 sources=['character_update_queued.hpp','character_update_queued.cpp','character_update_startup.hpp','character_update_startup.cpp','character_can_update.hpp','character_can_update.cpp','character_deferred_queue.hpp','character_deferred_queue.cpp','tools/build_character_update_queued_oracle.ps1']
 evidence=['reference/character-update-queued/queued-prefix-fixtures.bin','reference/character-update-queued/queued-prefix-fixtures.json','reports/character-update-queued-arm64-differential.json','reference/character-update-startup/original-functions.json','reference/character-deferred-queue/original-functions.json']
 headers=sorted((REPO/'port').rglob('*.hpp'))+sorted((REPO/'port/script-runtime').rglob('*.h'))
 inputs=[corpus,common,monster,dact]+sorted(a.assets.rglob('*.bdae'))+sorted(a.assets.glob('data/*.bin'))+[data/x for x in ['animations_dictionary_pyarraynames.bin','animations_dictionary_pyarray.bin','animations_pyarray.bin','animations_pyarraynames.bin','animations_pystructnames.bin']]
 paths=list(dict.fromkeys([ROOT/x for x in tests+sources+evidence]+headers+inputs+[manifest]));before={p.relative_to(REPO).as_posix():sha(p) for p in paths}
 flags=['-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror']
 gold_exe=scratch/'queued_host';gold_sources=['character_update_queued.cpp','character_update_startup.cpp','character_can_update.cpp','character_deferred_queue.cpp','tests/character_update_queued.cpp']
 run(['g++',*flags,*[linux(ROOT/x) for x in gold_sources],'-o',linux(gold_exe)])
 env=['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1']
 gold=json.loads(run(env+[linux(gold_exe),linux(ROOT/evidence[0])]))
 original=json.loads((ROOT/'reports/character-update-queued-arm64-differential.json').read_text())
 for field in ['comparisons','ordered_prefix_services','ordered_eligibility_services','ordered_queue_callbacks','ordered_entries']:assert gold[field]==original[field],field
 exe=scratch/'queued_session_host';libs=['-L'+linux(snapshot),'-ldh2_level_world','-ldh2_script_runtime','-ldh2_game_data','-ldh2_engine_animation','-ldh2_scene_materials','-Wl,-rpath,'+linux(snapshot),'-ldl']
 run(['g++',*flags,linux(ROOT/'character_update_queued.cpp'),linux(ROOT/'tests/character_update_queued_session.cpp'),*libs,'-o',linux(exe)])
 environment=env+['LD_LIBRARY_PATH='+linux(snapshot)];dependencies=run(environment+['ldd',linux(exe)])
 for source in parent_hashes:assert linux(snapshot/Path(source).name) in dependencies
 session=json.loads(run(environment+[linux(exe),*[linux(x) for x in [corpus,common,monster,dact,a.assets,data]]]))
 assert session['validation']=='PASS' and session['actual_delayed_monsters']==11 and session['queued_prefix_calls']==22 and session['application_stats_increments']==22 and session['same_native_FSM_state_queries']==66
 assert before=={p.relative_to(REPO).as_posix():sha(p) for p in paths};assert copies=={x.name:sha(x) for x in snapshot.glob('*.so')}
 report=dict(validation='PASS',gold_audit=gold,session_composition=session,source_and_input_sha256=before,parent_native_DSO_sha256=parent_hashes,private_snapshot_sha256=copies,executable_sha256={p.name:sha(p) for p in [gold_exe,exe]},commands=commands,linked_dependencies=dependencies,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],sanitizer_findings=0,central_rebuilt=False,new_complete_entry_compiler_inputs_bound=True,existing_DSO_compiler_provenance_claim=False,whole_composition_original_instruction_proof=False,original_entry_gold_proof=True,partial_prefix_replayed=False,whole_Character_frame=False,full_Kill_and_AIUnload_providers=False,required_skills_providers=False,upstream_CanUpdate_Application_network_and_clock_fixtures=True,scope=__doc__)
 (ROOT/'reports/character-update-queued-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',gold=gold,session=session)))
if __name__=='__main__':main()
