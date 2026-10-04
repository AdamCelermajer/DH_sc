"""Compile/run only the NEW feasibility probe against the existing real Lua DSO."""
import hashlib,json,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3]
SCRATCH=ROOT/'.local-inputs/lua514-source';BUILD='/home/adampalace/dh2-lua514-build'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def rel(p):return p.relative_to(ROOT).as_posix()
def wsl(p):return '/mnt/c/'+p.as_posix()[3:]
exe=SCRATCH/'include_vm_probe';library=SCRATCH/'include_probe_lua_dso.so'
command=['wsl','-d','Ubuntu','--','/usr/bin/g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off',wsl(HERE/'include_vm_probe.cpp'),'-I'+wsl(ROOT/'port/script-runtime/lua'),'-L'+BUILD,'-ldh2_script_runtime','-Wl,-rpath,'+BUILD,'-o',wsl(exe)]
subprocess.run(command,text=True,capture_output=True,check=True)
run=['wsl','-d','Ubuntu','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',wsl(exe)]
out=subprocess.run(run,text=True,capture_output=True,check=True);assert not out.stderr.strip(),out.stderr
audit=json.loads(out.stdout);assert audit['validation']=='PASS' and audit['mismatches']==0
subprocess.run(['wsl','-d','Ubuntu','--','cp',BUILD+'/libdh2_script_runtime.so',wsl(library)],check=True)
ownership=ROOT/'port/script-runtime/reports/script-game-bindings-host-audit.json';binding=json.loads(ownership.read_text())
# Bind the actually loaded DSO to its historical source proof, not current wrapper hashes.
assert sha(library)==binding['library_sha256'],(sha(library),binding['library_sha256'])
sources={p:h for p,h in binding['source_sha256'].items() if p.startswith('port/script-runtime/lua/')}
for p,h in sources.items():assert sha(ROOT/p)==h,p
report=dict(validation='PASS',host_audit=audit,sanitizers=dict(address=True,undefined=True,diagnostics=0),
 original_sha256=sha(ROOT/'.local-inputs/libDungeonHunter2.so'),original_probe_sha256=sha(HERE/'include-original-probe.json'),
 original_service_boundary_cases=188,source_sha256={rel(HERE/'include_vm_probe.cpp'):sha(HERE/'include_vm_probe.cpp'),rel(Path(__file__)):sha(Path(__file__))},
 lua_core_source_sha256=sources,library_sha256=sha(library),library_path=rel(library),executable_sha256=sha(exe),executable_path=rel(exe),
 historical_library_proof=dict(path=rel(ownership),sha256=sha(ownership)),build_command=command,run_command=run,
 production_Include_binding_implemented=False,full_LuaManager_backend=False,actual_external_AI_acceptance=False,packaged_APK=False,
 scope='Genuine float32 Lua nested-operation feasibility; immutable cache/loaded set fixtures explicitly model observed source operations. No production binding, owner extension, ARM64 or complete external-AI proof.')
(HERE/'include-host-probe.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(audit))
