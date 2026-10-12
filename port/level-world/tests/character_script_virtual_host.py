"""Actual Lua/alias DSO proof for original-derived external initial methods and VCB flags."""
import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 scratch=REPO/'.local-inputs/character-script-kinds-discovery';scratch.mkdir(exist_ok=True);commands=[]
 def run(*args):
  result=subprocess.run(['wsl','--cd',linux(REPO),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=args,returncode=result.returncode,stdout=result.stdout,stderr=result.stderr));assert result.returncode==0 and not result.stderr.strip(),commands[-1];return result.stdout.strip()
 sources=[ROOT/x for x in ('character_script_virtual.hpp','character_script_virtual.cpp','tests/character_script_virtual.cpp','tests/character_script_virtual_host.py')]+[REPO/'port/script-runtime'/x for x in ('script_runtime.h','script_runtime.c','script_function_alias.h','script_function_alias.cpp')]
 hashes={str(x.relative_to(REPO)):sha(x) for x in sources};flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 runtime_dir='/home/adampalace/dh2-world-build/script-runtime';runtime=runtime_dir+'/libdh2_script_runtime.so';library=linux(scratch/'libcharacter_script_virtual_audit.so');exe=linux(scratch/'virtual_host');before=run('sha256sum',runtime).split()[0]
 links=['-L'+runtime_dir,'-ldh2_script_runtime','-Wl,-rpath,'+runtime_dir]
 run('g++',*flags,'-shared','-fPIC',linux(ROOT/'character_script_virtual.cpp'),*links,'-o',library)
 run('g++',*flags,linux(ROOT/'tests/character_script_virtual.cpp'),'-I'+linux(ROOT),'-L'+linux(scratch),'-lcharacter_script_virtual_audit',*links,'-Wl,-rpath,'+linux(scratch),'-ldl','-o',exe)
 deps=run('ldd',exe);assert runtime in deps and 'libasan.so' in deps and 'libubsan.so' in deps
 gold=ROOT/'reference/character-script-kinds/virtual-fixtures.bin';proof=ROOT/'reports/character-script-virtual-arm64-differential.json';original=ROOT/'reference/character-script-kinds/kind-probe.json';evidence=json.loads(proof.read_text());assert sha(gold)==evidence['gold_sha256'] and sha(original)==evidence['original_evidence_sha256']
 checks=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(gold)));assert checks['validation']=='PASS' and checks['library']==library and checks['runtime_library']==checks['alias_library']==runtime
 binaries={x:run('sha256sum',x).split()[0] for x in (runtime,library,exe)};assert binaries[runtime]==before and all(sha(REPO/x)==h for x,h in hashes.items())
 report=dict(validation='PASS',scope=__doc__,host_audit=checks,source_sha256=hashes,binary_sha256=binaries,original_sha256=evidence['original_sha256'],original_evidence_sha256=sha(original),arm64_report_sha256=sha(proof),gold_sha256=sha(gold),sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer'],sanitizer_findings=0,actual_runtime_alias_library_executed=True,full_additional_kind_ownership_executed=False,commands=commands,linked_dependencies=deps)
 output=ROOT/'reports/character-script-virtual-host-audit.json';output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',checks=checks)))
if __name__=='__main__':main()
