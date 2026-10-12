"""Isolated sanitized integer module against the frozen genuine Lua runtime DSO.

Does not change CMake, production runtime, or central build outputs.
"""
import argparse,hashlib,json,os,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
    ap=argparse.ArgumentParser();ap.add_argument('--runtime-dir',default='/home/adampalace/dh2-script-callback-native-build');ap.add_argument('--build-dir',default='/home/adampalace/dh2-script-int-build');a=ap.parse_args()
    inputs=[ROOT/'port/script-runtime'/p for p in ['script_int_bindings.hpp','script_int_bindings.cpp','tests/script_int.cpp','tests/run_script_int_host.py','tests/emit_script_int_gold.py','tests/script_int_differential.py','script_runtime.h','script_runtime.c','reference/int-bindings/int-original-gold.bin','reference/int-bindings/int-original-gold.json','reference/int-bindings/binary-projection.json','reports/script-int-arm64-differential.json','reports/script-callback-native-storage-host-audit.json']]
    before={p.relative_to(ROOT).as_posix():sha(p) for p in inputs};commands=[]
    def run(*args):
        commands.append(list(args));r=subprocess.run(args,cwd=ROOT,capture_output=True,text=True);assert r.returncode==0,(args,r.returncode,r.stdout,r.stderr);return r.stdout,r.stderr
    posix='/mnt/'+ROOT.drive[0].lower()+ROOT.as_posix()[2:]
    run('wsl','/usr/bin/mkdir','-p',a.build_dir)
    flags=['-O1','-g','-std=c++17','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror']
    run('wsl','/usr/bin/g++','-shared','-fPIC',*flags,'-Wno-misleading-indentation',posix+'/port/script-runtime/script_int_bindings.cpp','-L'+a.runtime_dir,'-ldh2_script_runtime','-Wl,-rpath,'+a.runtime_dir,'-o',a.build_dir+'/libdh2_script_int.so')
    run('wsl','/usr/bin/g++',*flags,posix+'/port/script-runtime/tests/script_int.cpp','-L'+a.build_dir,'-ldh2_script_int','-L'+a.runtime_dir,'-ldh2_script_runtime','-Wl,-rpath,'+a.build_dir+':'+a.runtime_dir,'-o',a.build_dir+'/script_int_audit')
    stdout,stderr=run('wsl','/usr/bin/env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',a.build_dir+'/script_int_audit',posix+'/port/script-runtime/reference/int-bindings/int-original-gold.bin')
    result=json.loads(stdout);assert result['validation']=='PASS' and not stderr
    saved=ROOT/'.local-inputs/lua514-source/ints/host';saved.mkdir(parents=True,exist_ok=True);bindings={}
    for name,source in [('libdh2_script_int.so',a.build_dir+'/libdh2_script_int.so'),('script_int_audit',a.build_dir+'/script_int_audit'),('libdh2_script_runtime.so',a.runtime_dir+'/libdh2_script_runtime.so')]:
        target=saved/name;run('wsl','/usr/bin/cp',source,posix+'/'+target.relative_to(ROOT).as_posix());bindings[name]=dict(path=target.relative_to(ROOT).as_posix(),sha256=sha(target),executed_path=source)
    assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in inputs},'source/corpus changed while building/running'
    old=json.loads((ROOT/'port/script-runtime/reports/script-int-arm64-differential.json').read_text());assert result['original_gold_cases']==old['comparisons']
    previous=json.loads((ROOT/'port/script-runtime/reports/script-callback-native-storage-host-audit.json').read_text())
    report=dict(validation='PASS',host_audit=result,sanitizers=dict(address=True,undefined=True,leak=True,errors=0),source_sha256=before,binaries=bindings,compiler=run('wsl','/usr/bin/g++','--version')[0].splitlines()[0],commands=commands,original_sha256=old['original_sha256'],actual_runtime_DSO_linked=True,private_per_owner_map=True,original_instruction_report='port/script-runtime/reports/script-int-arm64-differential.json',actual_VM_gameplay_bindings=False,shipping_Android_printf_parity=False,packaged_APK=False,scope='New isolated int DSO and actual frozen float32 Lua DSO; complete source gold replay, genuine SetInt/GetInt Lua binding coercion, errors, separate owners and live map during VM finalization. Core runtime/generic guards unchanged.')
    (ROOT/'port/script-runtime/reports/script-int-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(result))
if __name__=='__main__':main()
