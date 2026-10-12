"""New isolated native-owner mode with actual frozen world/runtime DSOs.

Does not modify CMake or rebuild central dependencies. Historical owner tests
run against the same new owner in legacy mode and remain byte-for-byte intact.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
WORLD=Path(__file__).resolve().parents[1];ROOT=WORLD.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--world-build',default='/home/adampalace/dh2-world-build');ap.add_argument('--runtime-build',default='/home/adampalace/dh2-world-build/script-runtime');a=ap.parse_args()
    scratch=ROOT/'.local-inputs/character-script-owner-integers';scratch.mkdir(parents=True,exist_ok=True)
    inputs=[WORLD/p for p in ['character_script_owner.hpp','character_script_owner.cpp','character_script_owner_bindings.inc','tests/character_script_owner_integers.cpp','tests/character_script_owner_integers_host.py','tests/character_script_owner.cpp','reference/character-script-owner/owner-fixtures.bin']]
    inputs += [ROOT/'port/script-runtime'/p for p in ['script_runtime.h','script_runtime.c','script_int_bindings.hpp','script_int_bindings.cpp','reports/script-int-arm64-differential.json','reports/script-int-host-audit.json']]
    inputs += [WORLD/p for p in ['reference/character-script-ownership/original-functions.json','reference/character-script-ownership/reference/original-functions.asm','reference/character-script-owner-integers/inspection.json']]
    inputs += [ROOT/'port/script-runtime'/p for p in ['reference/int-bindings/teardown/original-functions.json','reference/int-bindings/teardown/reference/original-functions.asm']]
    hashes={p.relative_to(ROOT).as_posix():sha(p) for p in inputs};commands=[]
    inspected=json.loads((WORLD/'reference/character-script-owner-integers/inspection.json').read_text())
    assert sha(WORLD/'tests/character_script_owner.cpp')==inspected['inputs_sha256']['port/level-world/tests/character_script_owner.cpp'],'historical test changed'
    def run(*args):
        r=subprocess.run(['wsl','--cd',linux(ROOT),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
    runtime=a.runtime_build+'/libdh2_script_runtime.so';world=a.world_build+'/libdh2_level_world.so';before={p:run('sha256sum',p).split()[0] for p in [runtime,world]}
    lib=linux(scratch/'libcharacter_script_owner_integers.so');exe=linux(scratch/'integers_audit');legacy=linux(scratch/'legacy_audit')
    flags=['-O1','-g','-std=c++17','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
    links=['-L'+a.world_build,'-ldh2_level_world','-L'+a.runtime_build,'-ldh2_script_runtime','-Wl,-rpath,'+a.world_build,'-Wl,-rpath,'+a.runtime_build]
    run('g++','-shared','-fPIC',*flags,linux(WORLD/'character_script_owner.cpp'),*links,'-o',lib)
    owner_links=['-L'+linux(scratch),'-lcharacter_script_owner_integers','-Wl,-rpath,'+linux(scratch)]
    for source,target in [('tests/character_script_owner_integers.cpp',exe),('tests/character_script_owner.cpp',legacy)]:run('g++',*flags,linux(WORLD/source),'-I'+linux(WORLD),*owner_links,*links,'-ldl','-o',target)
    commons=ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac';assert sha(commons)=='20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
    audit=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(commons)))
    regression=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',legacy,linux(WORLD/'reference/character-script-owner/owner-fixtures.bin'),linux(commons)))
    assert audit['validation']==regression['validation']=='PASS' and audit['owner_library']==regression['owner_library']==lib
    assert audit['runtime_library']==audit['integer_library']==runtime and audit['lifecycle_library']==world
    assert regression['runtime_library']==runtime and regression['lifecycle_library']==regression['timer_library']==world
    deps=run('ldd',exe);assert lib in deps and world in deps and runtime in deps and 'libasan.so' in deps and 'libubsan.so' in deps
    binaries={p:run('sha256sum',p).split()[0] for p in [lib,exe,legacy,world,runtime]};assert all(binaries[p]==h for p,h in before.items());assert hashes=={p.relative_to(ROOT).as_posix():sha(p) for p in inputs}
    report=dict(validation='PASS',host_audit=audit,legacy_unchanged_test_regression=regression,source_sha256=hashes,binary_sha256=binaries,linked_dependencies=deps,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],sanitizer_findings=0,commons_sha256=sha(commons),original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',commands=commands,scope=__doc__,isolated_owner_DSO=True,actual_frozen_central_integer_and_VM_DSO=True,central_lifecycle_executed=True,main_owner_rebuilt=False,source_binding_order='Each native callback installed immediately before its own ordered registration delivery; source prefix and provider override remain observable.',original_instruction_kernels=['port/script-runtime/reports/script-int-arm64-differential.json','port/level-world/reference/character-script-ownership/original-functions.json'],whole_owner_original_instruction_differential=False,generic_VM_close_reentry=False,full_gameplay_namespace=False,packaged_APK=False)
    (WORLD/'reports/character-script-owner-integers-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',new=audit,legacy_checks=regression['checks'])))
if __name__=='__main__':main()
