"""Audit the actual centrally linked constants backend and source Lua bridge."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 if a.output.exists():raise RuntimeError('Refusing to replace linked constants proof')
 sources=[ROOT/'port/script-runtime'/x for x in ('script_constants.hpp','script_constants.cpp','script_runtime.h','script_runtime.c','script_design_bindings.c','script_design_bindings.h','tests/script_constants.cpp','CMakeLists.txt')]+[Path(__file__)]
 before={str(x.relative_to(ROOT)):sha(x) for x in sources};commands=[]
 def run(*args):
  r=subprocess.run(['wsl.exe','--cd',linux(ROOT),*args],capture_output=True,text=True,timeout=60);commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 exe=a.build+'/script-runtime/script_constants_audit';lib=a.build+'/script-runtime/libdh2_script_runtime.so'
 binary={x:run('sha256sum',x).split()[0] for x in (exe,lib)};deps=run('ldd',exe);assert lib in deps and 'libasan.so' in deps and 'libubsan.so' in deps and 'not found' not in deps
 compiler=json.loads(run('cat',a.build+'/compile_commands.json'));entries=[r for r in compiler if r['file'].endswith(('/script_constants.cpp','/tests/script_constants.cpp'))];assert len(entries)==2
 assert all('-fsanitize=address,undefined' in r['command'] for r in entries)
 gold=ROOT/'port/script-runtime/reference/design-constants-loader/constants-original-gold.bin';proof=gold.with_name('original-loader-probe.json');original=json.loads(proof.read_text());assert sha(gold)==original['gold_sha256']
 result=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(gold)));assert result['validation']=='PASS' and result['original_queries']==original['original_queries']
 assert before=={str(x.relative_to(ROOT)):sha(x) for x in sources} and binary=={x:run('sha256sum',x).split()[0] for x in (exe,lib)}
 report=dict(validation='PASS',scope=__doc__,host_audit=result,source_bindings=before,binary_bindings=binary,original_proof_sha256=sha(proof),original_gold_sha256=sha(gold),compiler_records=entries,commands=commands,sanitizer_findings=0,APK_executed=False,original_application_order_proved=False,full_sound_format_decoded=False,physical_device_verified=False)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',host_audit=result,report=str(a.output))))
if __name__=='__main__':main()
