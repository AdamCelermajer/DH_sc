"""Additive source callback scope: genuine Lua and old runtime regression audit."""
import hashlib,json,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3];MODULE=ROOT/'port/script-runtime'
BUILD='/home/adampalace/dh2-script-callback-native-build';OUT=ROOT/'.local-inputs/lua514-source/callback-scope-native'
OUT.mkdir(parents=True,exist_ok=True)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def rel(p):return p.relative_to(ROOT).as_posix()
def wsl(p):return '/mnt/c/'+p.as_posix()[3:]
commands=[]
def command(args):
    r=subprocess.run(['wsl','-d','Ubuntu','--',*args],text=True,capture_output=True,check=True)
    commands.append(dict(arguments=args,stdout=r.stdout,stderr=r.stderr));return r
frozen={rel(MODULE/p):sha(MODULE/p) for p in ('script_runtime.c','script_runtime.h','tests/script_callback_scope.cpp')}
command(['cmake','-S',wsl(MODULE),'-B',BUILD,'-DCMAKE_BUILD_TYPE=Debug','-DDH2_SCRIPT_SANITIZERS=ON'])
command(['cmake','--build',BUILD,'--parallel','6'])
command(['g++','-O1','-g','-std=c++17','-fsanitize=address,undefined','-fno-omit-frame-pointer',wsl(MODULE/'tests/script_callback_scope.cpp'),'-L'+BUILD,'-ldh2_script_runtime','-Wl,-rpath,'+BUILD,'-o',BUILD+'/script_callback_scope_audit'])
command(['g++','-O1','-g','-std=c++17','-fsanitize=address,undefined','-fno-omit-frame-pointer',wsl(MODULE/'tests/script_scalar_source.cpp'),'-L'+BUILD,'-ldh2_script_runtime','-Wl,-rpath,'+BUILD,'-o',BUILD+'/script_scalar_source_audit'])
commons=ROOT/'port/level-world/reference/character-script-update/lua-inputs/ai-commons.luac'
timer_gold=MODULE/'reference/game-bindings/callback-reference.bin';scalar_gold=MODULE/'reference/scalar-bindings/scalar-original-gold.bin'
audits={};binaries={}
for target,args in [('script_callback_scope_audit',[]),('script_include_audit',[]),('script_vm_ownership_audit',[]),('lua514_audit',[wsl(commons),wsl(OUT/'commons-roundtrip.luac')]),('script_game_bindings_audit',[wsl(commons)]),('script_game_bindings_replay',[wsl(timer_gold)]),('script_scalar_audit',[wsl(scalar_gold)]),('script_scalar_source_audit',[wsl(scalar_gold)])]:
    result=command(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',BUILD+'/'+target,*args]);assert not result.stderr.strip(),result.stderr
    audit=json.loads(result.stdout);assert audit['validation']=='PASS' and audit.get('mismatches',0)==0;audits[target]=audit
    target_path=OUT/target;command(['cp',BUILD+'/'+target,wsl(target_path)]);binaries[target]=dict(path=rel(target_path),sha256=sha(target_path))
library=OUT/'libdh2_script_runtime.so';command(['cp',BUILD+'/libdh2_script_runtime.so',wsl(library)])
assert frozen=={p:sha(ROOT/p) for p in frozen},'core/test source mutated during audit'
sources=[MODULE/p for p in ('script_runtime.c','script_runtime.h','script_game_bindings.c','script_game_bindings.h','script_scalar_bindings.c','script_scalar_bindings.h','script_function_alias.cpp','script_function_alias.h','tests/script_callback_scope.cpp','tests/script_include.cpp','tests/script_vm_ownership.cpp','tests/lua514.cpp','tests/script_game_bindings.cpp','tests/script_game_bindings_replay.cpp','tests/script_scalar.cpp','tests/script_scalar_source.cpp')]
sources += [p for p in (MODULE/'lua').iterdir() if p.suffix in ('.c','.h')]
sources += [Path(__file__),ROOT/'port/level-world/character_timers.cpp',ROOT/'port/level-world/character_timers.hpp']
original=MODULE/'reference/game-bindings/return-discard/original-functions.json';include=MODULE/'reference/base-bindings/include-original-probe.json'
report=dict(validation='PASS',scope=__doc__,host_audits=audits,sanitizers=dict(address=True,undefined=True,leaks=True,diagnostics=0),source_sha256={rel(p):sha(p) for p in sorted(set(sources))},original_sha256=sha(ROOT/'.local-inputs/libDungeonHunter2.so'),source_call_capture=dict(path=rel(original),sha256=sha(original)),source_Include_probe=dict(path=rel(include),sha256=sha(include)),original_timer_gold_sha256=sha(timer_gold),original_scalar_gold_sha256=sha(scalar_gold),actual_commons_sha256=sha(commons),library=dict(path=rel(library),sha256=sha(library)),executables=binaries,commands=commands,generic_busy_guards_preserved=True,source_Include_regression_passed=True,synchronous_same_binding_reentry=True,source_call_only_capability=True,full_StateRegistry_original_differential=False,packaged_APK=False,physical_ARM64=False,boundaries=['Scoped capability is an additive native64 wrapper, not a claimed original wrapper ABI.','Fresh alias lookup belongs to genuine borrowed receiver/registry; supplied resolved function is looked up freshly in Lua.','Coroutine/VM-close scoped callbacks are explicitly unsupported.','Nonstring/nonnumeric source error object returns unsupported -4; original crash-domain parity is not claimed.','Native provider cannot throw/destroy receiver or use raw Lua API; same VM/receiver must remain alive through outer call.'])
(MODULE/'reports/script-callback-native-storage-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',scoped=audits['script_callback_scope_audit'],regressions={k:v for k,v in audits.items() if k!='script_callback_scope_audit'})))
