"""Source/binary-bound production Include audit plus existing VM regressions."""
import hashlib,json,subprocess
from pathlib import Path
HERE=Path(__file__).resolve().parent;ROOT=HERE.parents[3];MODULE=ROOT/'port/script-runtime'
BUILD='/home/adampalace/dh2-script-include-build';OUTPUT=ROOT/'.local-inputs/lua514-source/include-production'
OUTPUT.mkdir(parents=True,exist_ok=True)
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def rel(p):return p.relative_to(ROOT).as_posix()
def wsl(p):return '/mnt/c/'+p.as_posix()[3:]
commands=[]
def command(args):
 result=subprocess.run(['wsl','-d','Ubuntu','--',*args],text=True,capture_output=True,check=True)
 commands.append(dict(arguments=args,stdout=result.stdout,stderr=result.stderr));return result
command(['cmake','-S',wsl(MODULE),'-B',BUILD,'-DCMAKE_BUILD_TYPE=Debug','-DDH2_SCRIPT_SANITIZERS=ON'])
command(['cmake','--build',BUILD,'--parallel','6'])
commons=ROOT/'port/level-world/reference/character-script-update/lua-inputs/ai-commons.luac'
gold=MODULE/'reference/game-bindings/callback-reference.bin'
audits={};binaries={}
for target,args in [('script_include_audit',[]),('script_vm_ownership_audit',[]),('lua514_audit',[wsl(commons),wsl(OUTPUT/'commons-roundtrip.luac')]),('script_game_bindings_audit',[wsl(commons)]),('script_game_bindings_replay',[wsl(gold)])]:
 result=command(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',BUILD+'/'+target,*args])
 assert not result.stderr.strip(),result.stderr
 audit=json.loads(result.stdout);assert audit['validation']=='PASS' and audit.get('mismatches',0)==0
 audits[target]=audit
 path=OUTPUT/target;command(['cp',BUILD+'/'+target,wsl(path)]);binaries[target]=dict(path=rel(path),sha256=sha(path))
library=OUTPUT/'libdh2_script_runtime.so';command(['cp',BUILD+'/libdh2_script_runtime.so',wsl(library)])
original=HERE/'include-original-probe.json';probe=json.loads(original.read_text())
assert probe['cases']==188 and audits['script_include_audit']['original_Include_guard_cases']==sum(r['group']=='Include' for r in probe['rows'])==80
sources=[MODULE/'script_runtime.c',MODULE/'script_runtime.h',MODULE/'script_game_bindings.c',MODULE/'script_game_bindings.h',MODULE/'script_function_alias.cpp',MODULE/'script_function_alias.h',MODULE/'CMakeLists.txt',MODULE/'tests/script_include.cpp',Path(__file__)]
sources += [p for p in (MODULE/'lua').iterdir() if p.suffix in ('.c','.h')]
sources += [ROOT/'port/level-world/character_timers.cpp',ROOT/'port/level-world/character_timers.hpp']
sources += [MODULE/'tests'/p for p in ('script_vm_ownership.cpp','lua514.cpp','script_game_bindings.cpp','script_game_bindings_replay.cpp')]
report=dict(validation='PASS',scope=__doc__,host_audits=audits,sanitizers=dict(address=True,undefined=True,leaks=True,diagnostics=0),
 original_sha256=sha(ROOT/'.local-inputs/libDungeonHunter2.so'),original_probe=dict(path=rel(original),sha256=sha(original),service_boundary_cases=188,native_guard_cases=80),
 original_gold_sha256=sha(gold),actual_commons_sha256=sha(commons),source_sha256={rel(p):sha(p) for p in sorted(set(sources))},
 library=dict(path=rel(library),sha256=sha(library)),executables=binaries,commands=commands,production_Include_binding_implemented=True,
 generic_busy_guards_preserved=True,full_LuaManager_backend=False,ScriptOwner_nested_extension=False,actual_external_AI_acceptance=False,
 optimized_ARM64_instructions_verified=False,packaged_APK=False,physical_ARM64=False,
 boundaries=['Genuine receiver-owned cache/loaded-set provider fixture, no fake accepted game functions.','Positive Lua load errors are source AddFile false; unsupported non-string/non-number error objects return explicit -4.','Include from coroutine/VM-close is explicitly rejected.','Owner session/provider lifetime integration remains parent-owned.'])
(MODULE/'reports/script-source-file-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps(dict(validation='PASS',include=audits['script_include_audit'],regressions={k:v for k,v in audits.items() if k!='script_include_audit'})))
