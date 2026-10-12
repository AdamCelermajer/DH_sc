"""Source/binary-bound independent scalar DSO linked to genuine sanitized Lua VM."""
import hashlib,json,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3];MODULE=ROOT/'port/script-runtime'
BUILD='/home/adampalace/dh2-script-scalar-build';RUNTIME='/home/adampalace/dh2-script-include-build'
OUT=ROOT/'.local-inputs/lua514-source/scalar';OUT.mkdir(parents=True,exist_ok=True)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def rel(p):return p.relative_to(ROOT).as_posix()
def wsl(p):return '/mnt/c/'+p.as_posix()[3:]
commands=[]
def command(args):
    r=subprocess.run(['wsl','-d','Ubuntu','--',*args],text=True,capture_output=True,check=True)
    commands.append(dict(arguments=args,stdout=r.stdout,stderr=r.stderr));return r
command(['mkdir','-p',BUILD])
flags=['-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer']
command(['gcc',*flags,'-shared','-fPIC',wsl(MODULE/'script_scalar_bindings.c'),'-L'+RUNTIME,'-ldh2_script_runtime','-Wl,-rpath,'+RUNTIME,'-o',BUILD+'/libdh2_script_scalar.so'])
command(['g++',*flags,'-std=c++17',wsl(MODULE/'tests/script_scalar.cpp'),'-L'+BUILD,'-L'+RUNTIME,'-ldh2_script_scalar','-ldh2_script_runtime','-Wl,-rpath,'+BUILD,'-Wl,-rpath,'+RUNTIME,'-o',BUILD+'/script_scalar_audit'])
gold=HERE/'scalar-original-gold.bin'
r=command(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',BUILD+'/script_scalar_audit',wsl(gold)])
assert not r.stderr.strip(),r.stderr;audit=json.loads(r.stdout);assert audit['validation']=='PASS' and audit['original_gold_cases']==13978 and audit['mismatches']==0
binaries={}
for name,base in [('libdh2_script_scalar.so',BUILD),('script_scalar_audit',BUILD),('libdh2_script_runtime.so',RUNTIME)]:
    target=OUT/(name if name!='libdh2_script_runtime.so' else 'host-runtime.so');command(['cp',base+'/'+name,wsl(target)]);binaries[name]=dict(path=rel(target),sha256=sha(target))
sources=[MODULE/p for p in ('script_scalar_bindings.c','script_scalar_bindings.h','tests/script_scalar.cpp','tests/script_scalar_differential.py','script_runtime.c','script_runtime.h')]
sources.append(Path(__file__))
sources += [p for p in (MODULE/'lua').iterdir() if p.suffix in ('.c','.h')]
arm=MODULE/'reports/script-scalar-arm64-differential.json';arm_report=json.loads(arm.read_text());assert arm_report['binary_gold']['sha256']==sha(gold)
for name in ('script_scalar_bindings.c','script_scalar_bindings.h'):assert arm_report['source_sha256'][rel(MODULE/name)]==sha(MODULE/name)
report=dict(validation='PASS',scope=__doc__,host_audit=audit,sanitizers=dict(address=True,undefined=True,leaks=True,diagnostics=0),original_sha256=sha(ROOT/'.local-inputs/libDungeonHunter2.so'),original_gold=dict(path=rel(gold),sha256=sha(gold)),arm64_report=dict(path=rel(arm),sha256=sha(arm)),binaries=binaries,source_sha256={rel(p):sha(p) for p in sorted(set(sources))},commands=commands,genuine_float32_Lua=True,main_production_DSO_integrated=False,packaged_APK=False,physical_ARM64=False,boundaries=['Existing source-values VM bridge rejects above16arguments; direct callback fullarity through65 tested.','Unsigned source identity and undefined AEABI divide-zero are explicit services.','Signed float conversion import contract modeled, original Android helper implementation is unavailable.','Fresh temporary VM string coercion is genuine host Lua; ARM oracle names this Lua primitive boundary.','Native allocation failure diagnostics and malformed input validation do not claim original panic parity.'])
(MODULE/'reports/script-scalar-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
