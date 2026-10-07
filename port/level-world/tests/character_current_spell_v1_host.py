"""Sanitize original current-spell gold and real source-VM callback delivery.

Builds one isolated production query DSO; parent owns central World selection.
SG/table services are declared fixtures here, not a complete saved-player proof.
"""
import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
BUILD='/home/adampalace/dh2-current-spell-v1-build'
RUNTIME='/home/adampalace/dh2-world-build/script-runtime'
OUTPUT=ROOT/'port/level-world/reports/character-current-spell-v1-host-audit.json'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
    assert not OUTPUT.exists(),'Preserve accepted receipts'
    paths=[ROOT/p for p in (
      'port/level-world/character_current_spell_v1.hpp','port/level-world/character_current_spell_v1.cpp',
      'port/level-world/tests/character_current_spell_v1.cpp','port/level-world/tests/character_current_spell_v1_host.py',
      'port/level-world/tests/character_current_spell_v1_oracle.cpp','port/level-world/tests/character_current_spell_v1_differential.py',
      'port/level-world/reference/character-current-spell-v1/source-gold.bin',
      'port/level-world/reference/character-current-spell-v1/source-capture.json',
      'port/level-world/reports/character-current-spell-v1-arm64-differential.json',
      'port/script-runtime/script_runtime.h','port/script-runtime/script_return_observer_v1.h')]
    before={p.relative_to(ROOT).as_posix():sha(p) for p in paths};commands=[]
    def run(argv):
        p=subprocess.run(['wsl.exe','--exec',*argv],capture_output=True,text=True,timeout=120)
        commands.append(dict(arguments=argv,returncode=p.returncode,stdout=p.stdout,stderr=p.stderr))
        assert p.returncode==0 and not p.stderr.strip(),commands[-1];return p.stdout.strip()
    run(['mkdir','-p',BUILD])
    flags=['-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer']
    run(['g++',*flags,'-shared','-fPIC',linux(paths[1]),'-o',BUILD+'/libcurrent_spell_v1_host.so'])
    run(['g++',*flags,linux(paths[2]),'-L'+BUILD,'-lcurrent_spell_v1_host','-L'+RUNTIME,'-ldh2_script_runtime',
         '-Wl,-rpath,'+BUILD,'-Wl,-rpath,'+RUNTIME,'-o',BUILD+'/audit'])
    result=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',BUILD+'/audit',linux(paths[6])]))
    assert result['validation']=='PASS' and result['original_cases']==512 and result['real_VM_queries']==2
    result.update(source_sha256=before,commands=commands,sanitizer_findings=0,
      runtime_sha256=run(['sha256sum',RUNTIME+'/libdh2_script_runtime.so']).split()[0],
      query_library_sha256=run(['sha256sum',BUILD+'/libcurrent_spell_v1_host.so']).split()[0],
      binary_sha256=run(['sha256sum',BUILD+'/audit']).split()[0],scope=__doc__,
      central_World_linked=False,live_Android=False)
    assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in paths}
    OUTPUT.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps({k:result[k] for k in ('validation','original_cases','ordered_services','real_VM_queries','guards','checks','sanitizer_findings')}))
if __name__=='__main__':main()
