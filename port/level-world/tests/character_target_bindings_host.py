"""Isolated target coordinator linked to genuine central VM/data/provider DSOs.

The world registry, debug storage and Lua GetTarget object projection remain
caller boundaries. This runner does not rebuild any central CMake dependency.
"""
import argparse, hashlib, json, subprocess
from pathlib import Path
WORLD = Path(__file__).resolve().parents[1]
ROOT = WORLD.parents[1]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p): return '/mnt/' + p.drive[0].lower() + p.as_posix()[2:]
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build', default='/home/adampalace/dh2-world-build')
    ap.add_argument('--main-linked', help='Existing central CMake audit executable; no rebuild')
    a = ap.parse_args()
    scratch = ROOT / '.local-inputs/character-target-bindings-discovery'
    scratch.mkdir(parents=True, exist_ok=True)
    files = ['character_target_bindings.hpp','character_target_bindings.cpp',
             'tests/character_target_bindings.cpp','tests/character_target_bindings_host.py',
             'tests/character_target_bindings_differential.py',
             'reference/character-target-bindings/target-original-gold.bin',
             'reference/character-target-bindings/original-functions.json',
             'reference/character-target-bindings/reference/original-functions.asm',
             'reports/character-target-bindings-arm64-differential.json',
             'character_target_providers.hpp','character_target_providers.cpp']
    inputs = [WORLD / p for p in files]
    inputs += [ROOT / 'port/script-runtime' / p for p in ['script_runtime.h','script_runtime.c']]
    inputs += [ROOT / 'port/game-data' / p for p in ['ai.hpp','ai.cpp']]
    assets = ROOT / 'port/android-native/app/src/main/assets/data'
    inputs += [assets / p for p in ['ai_pyarray.bin','ai_pyarraynames.bin','ai_pystructnames.bin',
                                    'ai_factions_pyarray.bin','ai_factions_pyarraynames.bin','ai_factions_pystructnames.bin']]
    hashes = {p.relative_to(ROOT).as_posix(): sha(p) for p in inputs}
    commands = []
    def run(*args):
        r = subprocess.run(['wsl','--cd',linux(ROOT),*args],capture_output=True,text=True,timeout=120)
        commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr))
        assert r.returncode == 0 and not r.stderr.strip(), commands[-1]
        return r.stdout.strip()
    runtime=a.build+'/script-runtime/libdh2_script_runtime.so'
    data=a.build+'/game-data/libdh2_game_data.so'
    world=a.build+'/libdh2_level_world.so'
    central=[world,data,runtime]
    before={p:run('sha256sum',p).split()[0] for p in central}
    flags=['-O1','-g','-std=c++17','-fno-fast-math','-ffp-contract=off',
           '-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
    links=[]
    for folder,name in [(a.build,'dh2_level_world'),(a.build+'/game-data','dh2_game_data'),(a.build+'/script-runtime','dh2_script_runtime')]:
        links += ['-L'+folder,'-l'+name,'-Wl,-rpath,'+folder]
    if a.main_linked:
        exe=a.main_linked;lib=world
    else:
        lib=linux(scratch/'libcharacter_target_bindings_host.so');exe=linux(scratch/'target_bindings_audit')
        run('g++','-shared','-fPIC',*flags,linux(WORLD/'character_target_bindings.cpp'),*links,'-o',lib)
        run('g++',*flags,'-Wno-misleading-indentation',linux(WORLD/'tests/character_target_bindings.cpp'),
            '-L'+linux(scratch),'-lcharacter_target_bindings_host','-Wl,-rpath,'+linux(scratch),*links,'-ldl','-o',exe)
    audit=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1',
                         'UBSAN_OPTIONS=halt_on_error=1',exe,
                         linux(WORLD/'reference/character-target-bindings/target-original-gold.bin'),linux(assets)))
    assert audit['validation']=='PASS' and audit['original_gold_cases']==3955
    assert audit['ordered_gold_services']==21946 and audit['actual_ai_rows']==76
    assert audit['kernel_library']==lib and audit['runtime_library']==runtime
    deps=run('ldd',exe)
    assert all(p in deps for p in [lib,*central]) and 'libasan.so' in deps and 'libubsan.so' in deps
    binaries={p:run('sha256sum',p).split()[0] for p in [lib,exe,*central]}
    assert all(binaries[p]==before[p] for p in central)
    assert hashes=={p.relative_to(ROOT).as_posix():sha(p) for p in inputs}
    report=dict(validation='PASS',host_audit=audit,source_sha256=hashes,binary_sha256=binaries,
                linked_dependencies=deps,commands=commands,sanitizer_findings=0,
                sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],
                original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',
                scope=__doc__,actual_central_VM_DSO=True,actual_central_data_DSO=True,
                actual_central_Character_IsDead=True,main_linked=bool(a.main_linked),
                central_compiler_provenance_claim=False,full_GetTarget_Lua_projection=False,
                whole_world_registry=False,packaged_APK=False)
    name='character-target-bindings-main-linked-host-audit.json' if a.main_linked else 'character-target-bindings-host-audit.json'
    (WORLD/'reports'/name).write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(audit))
if __name__=='__main__': main()
