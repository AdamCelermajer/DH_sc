"""Private CharacterScriptSession composition on actual native sanitized DSOs.

This builds only the new session adapter and the separately source-proved
DebugSwitches missing-file backend. Central VM/data/world dependencies are
historical frozen libraries bound through their existing compiler report;
current runtime edits are not represented as those libraries' implementations.
Host rows/current-level/cache are explicit borrowed fixture projections of
decoded real data. Debug open returns an explicit missing-file fixture. Neither
provider establishes live Application selection or Android filesystem behavior.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
WORLD=Path(__file__).resolve().parents[1];ROOT=WORLD.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--build',default='/home/adampalace/dh2-world-build');args=ap.parse_args()
    scratch=ROOT/'.local-inputs/character-script-session';scratch.mkdir(parents=True,exist_ok=True)
    proof=WORLD/'reports/character-init-services-main-linked-host-audit.json';historical=json.loads(proof.read_text());assert historical['validation']=='PASS'
    runtime=args.build+'/script-runtime/libdh2_script_runtime.so';data=args.build+'/game-data/libdh2_game_data.so';world=args.build+'/libdh2_level_world.so'
    stage17=ROOT/'.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/ai/swampking_core.luac'
    assert stage17.is_file(),'Pinned cached Stage17 swampking_core source is required for the Init regression'
    source_member='data/scripts/ai/swampking_core.luac'
    inputs=[WORLD/'reference/character-game-design/real-cache-inputs.bin',ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',ROOT/'.local-inputs/character-script-owner-extension/monster.luac',ROOT/'port/android-native/app/src/main/assets/worlds/crypt01.dact',stage17,ROOT/'port/android-native/app/src/main/assets/data/animations_dictionary_pyarraynames.bin']
    sources=[WORLD/p for p in ['character_script_session.hpp','character_script_session.cpp','tests/character_script_session.cpp','tests/character_script_session_host.py','character_design_services.hpp','character_design_services.cpp']]
    headers=[WORLD/p for p in ['character_game_design.hpp','character_script_owner.hpp','character_host_context.hpp','character_timers.hpp','character_property_bindings.hpp','character_spatial_bindings.hpp','character_level.hpp']]
    headers += [ROOT/'port/script-runtime'/p for p in ['script_runtime.h','script_function_alias.h','script_int_bindings.hpp','script_scalar_bindings.h','script_game_bindings.h','script_design_bindings.h']]
    source_hashes={p.relative_to(ROOT).as_posix():sha(p) for p in sources}
    interface_hashes={p.relative_to(ROOT).as_posix():sha(p) for p in headers}
    input_hashes={p.relative_to(ROOT).as_posix():sha(p) for p in inputs}
    commands=[]
    def run(*argv):
        r=subprocess.run(['wsl','--cd',linux(ROOT),*argv],text=True,capture_output=True,timeout=120)
        commands.append(dict(arguments=list(argv),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr))
        assert not r.returncode and not r.stderr.strip(),commands[-1]
        return r.stdout.strip()
    before={p:run('sha256sum',p).split()[0] for p in [runtime,data,world]}
    assert all(historical['binary_sha256'][p]==h for p,h in before.items()),'Central dependency stage changed; refresh its real compiler binding before test execution.'
    flags=['-O1','-g','-std=c++17','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
    links=['-L'+args.build,'-ldh2_level_world','-L'+args.build+'/game-data','-ldh2_game_data','-L'+args.build+'/script-runtime','-ldh2_script_runtime','-Wl,-rpath,'+args.build,'-Wl,-rpath,'+args.build+'/game-data','-Wl,-rpath,'+args.build+'/script-runtime']
    lib=linux(scratch/'libcharacter_script_session.so');exe=linux(scratch/'character_script_session_audit')
    run('g++','-shared','-fPIC',*flags,linux(WORLD/'character_script_session.cpp'),linux(WORLD/'character_design_services.cpp'),*links,'-o',lib)
    run('g++',*flags,'-Wno-misleading-indentation',linux(WORLD/'tests/character_script_session.cpp'),'-L'+linux(scratch),'-lcharacter_script_session','-Wl,-rpath,'+linux(scratch),*links,'-ldl','-o',exe)
    audit=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,*map(linux,inputs)))
    assert audit['validation']=='PASS' and audit['actual_swampking_core_oninit'] is True
    assert audit['actual_swampking_core_get_prophp'] is True
    assert audit['actual_swampking_core_virtual16'] is True and audit['stage17_required_failure_prefixes']==3
    assert audit['actual_swampking_core_mark_as_flying_calls']==1 and audit['actual_swampking_core_register_anim_calls']==7
    assert audit['actual_Crypt_monster_initializations']==11 and audit['source_registrations']==1870 and audit['mismatches']==0
    assert audit['runtime_library']==runtime and audit['world_library']==world and audit['data_library']==data
    deps=run('ldd',exe);assert all(p in deps for p in [lib,runtime,data,world]) and 'libasan.so' in deps and 'libubsan.so' in deps
    hashes={p:run('sha256sum',p).split()[0] for p in [lib,exe,runtime,data,world]};assert all(hashes[p]==h for p,h in before.items())
    assert source_hashes=={p.relative_to(ROOT).as_posix():sha(p) for p in sources}
    assert interface_hashes=={p.relative_to(ROOT).as_posix():sha(p) for p in headers}
    assert input_hashes=={p.relative_to(ROOT).as_posix():sha(p) for p in inputs}
    evidence=[WORLD/'reports'/name for name in ['character-script-owner-integers-host-audit.json','character-script-owner-external-arm64-differential.json','character-design-services-arm64-differential.json','character-spatial-bindings-arm64-differential.json','character-host-context-arm64-differential.json','character-game-design-host-audit.json']]
    for p in evidence:assert json.loads(p.read_text())['validation']=='PASS'
    report=dict(validation='PASS',scope=__doc__,host_audit=audit,source_sha256=source_hashes,compile_interface_sha256=interface_hashes,input_sha256=input_hashes,binary_sha256=hashes,
      historical_central_dependency_proof=dict(path=proof.relative_to(ROOT).as_posix(),sha256=sha(proof),binary_sha256=before),
      underlying_kernel_proof_sha256={p.relative_to(ROOT).as_posix():sha(p) for p in evidence},commands=commands,dependencies=deps,
      sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],sanitizer_findings=0,
      original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',
      actual_common_and_monster_executed=True,actual_swampking_core_stage17_init_executed=True,
      actual_swampking_core_get_prophp_called=True,
      actual_swampking_core_source_member=source_member,actual_swampking_core_source_sha256=sha(stage17),
      actual_Crypt_descriptor_positions=True,
      initial_native_timer_records=True,genuine_DebugSwitches_missing_file_backend=True,
      DebugSwitches_file_provider_fixture=True,host_context_provider_fixture=True,
      full_AI_timer_expiry=False,Application_session_selection=False,original_Application_constant_load_order=False,
      whole_session_original_instruction_differential=False,current_runtime_source_rebuilt=False,packaged_APK=False,physical_ARM64=False)
    path=WORLD/'reports/character-script-session-host-audit.json';path.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
