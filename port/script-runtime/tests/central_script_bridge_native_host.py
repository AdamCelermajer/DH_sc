"""Observe the corrected native-storage central bridge without rebuilding it.

This is source/binary-bound host evidence for the current integration stage,
not Android packaging, complete design-manager ownership or gameplay proof.
"""
import argparse, hashlib, json, subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]

def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def linux(path):return '/mnt/c/'+str(path.resolve()).replace('\\','/')[3:]

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--build',default='/home/adampalace/dh2-world-build')
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    if a.output.exists():raise RuntimeError('Refusing to replace a central proof')
    runtime=ROOT/'port/script-runtime';world=ROOT/'port/level-world'
    inputs=[runtime/x for x in ['script_runtime.h','script_runtime.c','CMakeLists.txt',
        'script_design_bindings.h','script_design_bindings.c','tests/script_design.cpp',
        'tests/script_callback_scope.cpp','tests/script_include.cpp','tests/central_script_bridge_native_host.py',
        'tests/script_scalar.cpp','tests/script_scalar_source.cpp','script_scalar_bindings.h','script_scalar_bindings.c',
        'reference/scalar-bindings/scalar-original-gold.bin',
        'reference/design-bindings/registry-probe.json','reference/design-bindings/differential.json',
        'reference/design-bindings/design-original-gold.bin']]
    inputs += [world/x for x in ['CMakeLists.txt','character_script_owner.hpp',
        'character_script_owner.cpp','tests/character_script_owner_include.cpp']]
    assets=ROOT/'port/android-native/app/src/main/assets/data'
    inputs += [assets/x for x in ['character_properties_pyarray.bin',
        'character_properties_pyarraynames.bin','character_properties_pystructnames.bin']]
    inputs += [ROOT/'port/game-data/data.cpp',ROOT/'port/game-data/data.hpp']
    sources={str(x.relative_to(ROOT)):sha(x) for x in inputs};commands=[]
    def run(*cmd):
        r=subprocess.run(['wsl.exe','--',*cmd],capture_output=True,text=True,timeout=60)
        commands.append(dict(arguments=cmd,returncode=r.returncode,stdout=r.stdout,stderr=r.stderr))
        if r.returncode or r.stderr.strip():raise RuntimeError(commands[-1])
        return r.stdout
    resources=ROOT/'.local-inputs/character-script-owner-extension'
    scalar_gold=runtime/'reference/scalar-bindings/scalar-original-gold.bin'
    suites={
        'scalar':(a.build+'/script-runtime/script_scalar_audit',[scalar_gold]),
        'scalar_source':(a.build+'/script-runtime/script_scalar_source_audit',[scalar_gold]),
        'design':(a.build+'/script-runtime/script_design_audit',
                  [runtime/'reference/design-bindings/design-original-gold.bin',assets]),
        'scope':(a.build+'/script-runtime/script_callback_scope_audit',[]),
        'include':(a.build+'/script-runtime/script_include_audit',[]),
        'owner_include':(a.build+'/character_script_owner_include_audit',
                         [resources/'_commons.luac',resources/'follower.luac'])}
    paths=[a.build+'/libdh2_level_world.so',a.build+'/script-runtime/libdh2_script_runtime.so']
    paths += [exe for exe,args in suites.values()]
    def binary_hashes():return {s.split(maxsplit=1)[1].strip():s.split()[0]
                               for s in run('sha256sum',*paths).splitlines()}
    before=binary_hashes();audits={};deps={}
    for tag,(exe,args) in suites.items():
        deps[tag]=run('ldd',exe)
        assert a.build+'/script-runtime/libdh2_script_runtime.so' in deps[tag]
        assert 'libasan.so' in deps[tag] and 'libubsan.so' in deps[tag] and 'not found' not in deps[tag]
        audits[tag]=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1',
            'UBSAN_OPTIONS=halt_on_error=1',exe,*(linux(x) for x in args)))
        assert audits[tag]['validation']=='PASS'
    assert audits['design']['original_gold_cases']==1200 and audits['design']['actual_VM_checks']==457
    assert audits['scalar']['original_gold_cases']==13978 and audits['scalar']['actual_VM_checks']==37
    assert audits['scalar_source']['original_gold_cases']==13978 and audits['scalar_source']['actual_VM_checks']==40
    assert audits['scope']['actual_scoped_callbacks']==16 and audits['scope']['scope_busy_guards']==543
    assert audits['scope']['native_argument_storage_checks']==1
    assert audits['scope']['argument_projection_error_cleanup_cases']==64
    assert audits['include']['source_root_file_checks']==17
    assert audits['owner_include']['checks']==2005 and audits['owner_include']['actual_follower_flags']==962
    assert audits['owner_include']['owner_library']==a.build+'/libdh2_level_world.so'
    assert sources=={str(x.relative_to(ROOT)):sha(x) for x in inputs} and before==binary_hashes()
    report=dict(validation='PASS',scope=__doc__,audits=audits,source_sha256=sources,
        binary_sha256=before,linked_dependencies=deps,sanitizer_findings=0,
        authored_resource_sha256={x.name:sha(x) for x in [resources/'_commons.luac',resources/'follower.luac']},
        full_game_verified=False,physical_arm64_verified=False,apk_executed=False,commands=commands)
    a.output.write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(validation='PASS',audits=audits,binary_sha256=before)))

if __name__=='__main__':main()
