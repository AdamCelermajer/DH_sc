"""Persistent real-cache ownership composition; underlying kernels retain their
separate original-instruction proofs. No Application startup-order claim.
"""
import argparse, hashlib, json, struct, subprocess
from pathlib import Path
WORLD=Path(__file__).resolve().parents[1]; ROOT=WORLD.parents[1]
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p): return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def field(data): return struct.pack('<I',len(data))+data
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--build',default='/home/adampalace/dh2-world-build');a=ap.parse_args()
    scratch=ROOT/'.local-inputs/character-game-design';scratch.mkdir(parents=True,exist_ok=True)
    ref=WORLD/'reference/character-game-design';ref.mkdir(parents=True,exist_ok=True)
    asset=ROOT/'port/android-native/app/src/main/assets/data'
    paths=[]
    for stem in ['character_properties','character_classes','ai','ai_factions']:
        paths.extend(asset/(stem+suffix+'.bin') for suffix in ['_pyarray','_pyarraynames','_pystructnames'])
    paths.extend(ROOT/'port/game-data/reference/level-tables'/('levels'+suffix+'.bin') for suffix in ['_pyarray','_pyarraynames','_pystructnames'])
    bundle_path=ROOT/'port/script-runtime/reports/design-constants-bundled-assets.json';bundle=json.loads(bundle_path.read_text())
    constant_paths=[ROOT/entry['path'].replace('\\','/') for entry in bundle['bundled_assets']]
    assert len(constant_paths)==26
    for p,e in zip(constant_paths,bundle['bundled_assets']):assert sha(p)==e['sha256']
    catalog_path=ROOT/'port/script-runtime/reference/design-bindings/registry-probe.json';catalog=json.loads(catalog_path.read_text())
    delivered=[entry['name'] for entry in catalog['registrations']];assert len(delivered)==142 and len(set(delivered))==138
    assert (ref/'registered_names.inc').read_text()==''.join(' '+json.dumps(name)+',\n' for name in delivered)
    fixture=b'GDO1'+b''.join(field(p.read_bytes()) for p in paths)+struct.pack('<I',26)
    for p in constant_paths:fixture+=field(p.name.encode())+field(p.read_bytes())
    fixture+=struct.pack('<I',len(delivered))+b''.join(field(s.encode()) for s in delivered)
    fixture_path=ref/'real-cache-inputs.bin';fixture_path.write_bytes(fixture)
    original_table=ROOT/'port/game-data/reference/game-design-tables/table-fixtures.bin'
    evidence=[ROOT/p for p in ['port/game-data/reports/game-design-tables-arm64-differential.json',
        'port/game-data/reports/level-tables-arm64-differential.json',
        'port/script-runtime/reference/design-bindings/differential.json',
        'port/script-runtime/reference/design-constants-loader/original-loader-probe.json']]
    for p in evidence:assert json.loads(p.read_text())['validation']=='PASS'
    inputs=[WORLD/p for p in ['character_game_design.hpp','character_game_design.cpp','tests/character_game_design.cpp',
        'tests/character_game_design_host.py','reference/character-game-design/registered_names.inc']]
    inputs += [ROOT/'port/game-data'/p for p in ['data.hpp','data.cpp','class_tables.hpp','class_tables.cpp','properties.hpp','properties.cpp','ai.hpp','ai.cpp','level_tables.hpp','level_tables.cpp','game_design_tables.hpp','game_design_tables.cpp']]
    inputs += [ROOT/'port/script-runtime'/p for p in ['script_runtime.h','script_runtime.c','script_constants.hpp','script_constants.cpp','script_design_bindings.h','script_design_bindings.c']]
    inputs += [WORLD/p for p in ['character_level.hpp','character_level.cpp','character_property_bindings.hpp','character_property_bindings.cpp']]
    inputs += [fixture_path,original_table,catalog_path,bundle_path,*paths,*constant_paths,*evidence]
    hashes={p.relative_to(ROOT).as_posix():sha(p) for p in inputs};commands=[]
    def run(*args):
        r=subprocess.run(['wsl','--cd',linux(ROOT),*args],capture_output=True,text=True,timeout=120)
        commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr))
        assert not r.returncode and not r.stderr.strip(),commands[-1]
        return r.stdout.strip()
    runtime=a.build+'/script-runtime/libdh2_script_runtime.so';data=a.build+'/game-data/libdh2_game_data.so';world=a.build+'/libdh2_level_world.so'
    before={p:run('sha256sum',p).split()[0] for p in [runtime,data,world]}
    lib=linux(scratch/'libcharacter_game_design.so');exe=linux(scratch/'game_design_audit')
    flags=['-O1','-g','-std=c++17','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
    links=['-L'+a.build,'-ldh2_level_world','-L'+a.build+'/game-data','-ldh2_game_data','-L'+a.build+'/script-runtime','-ldh2_script_runtime',
        '-Wl,-rpath,'+a.build,'-Wl,-rpath,'+a.build+'/game-data','-Wl,-rpath,'+a.build+'/script-runtime']
    run('g++','-shared','-fPIC',*flags,linux(WORLD/'character_game_design.cpp'),*links,'-o',lib)
    run('g++',*flags,'-Wno-misleading-indentation',linux(WORLD/'tests/character_game_design.cpp'),
        '-L'+linux(scratch),'-lcharacter_game_design','-Wl,-rpath,'+linux(scratch),*links,'-ldl','-o',exe)
    audit=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(fixture_path),linux(original_table)))
    assert audit['validation']=='PASS' and audit['registered_namespaces']==6 and audit['real_constant_assignments']==5608
    assert audit['data_library']==data and audit['world_library']==world and audit['runtime_library']==runtime
    deps=run('ldd',exe);assert all(p in deps for p in [lib,runtime,data,world]) and 'libasan.so' in deps and 'libubsan.so' in deps
    binaries={p:run('sha256sum',p).split()[0] for p in [lib,exe,runtime,data,world]}
    assert all(binaries[p]==h for p,h in before.items())
    assert hashes=={p.relative_to(ROOT).as_posix():sha(p) for p in inputs}
    report=dict(validation='PASS',scope=__doc__,host_audit=audit,source_and_input_sha256=hashes,
        binary_sha256=binaries,dependencies=deps,commands=commands,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],
        sanitizer_findings=0,original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',
        underlying_original_instruction_evidence=[p.relative_to(ROOT).as_posix() for p in evidence],
        owned_decoded_tables=True,owned_input_streams=True,persistent_registry_and_constant_context=True,
        caller_supplied_constant_order=[p.name for p in constant_paths],source_Application_load_order=False,
        full_design_manager=False,full_Application_ownership=False,whole_owner_original_instruction_differential=False,
        actual_central_VM_data_and_level_kernels=True,central_dependencies_rebuilt=False,packaged_APK=False,physical_ARM64=False)
    (WORLD/'reports/character-game-design-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(audit))
if __name__=='__main__':main()
