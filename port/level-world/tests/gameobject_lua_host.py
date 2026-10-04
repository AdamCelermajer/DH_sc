"""Isolated new source object bridge and actual float32 Lua sanitizer audit.

No central CMake rebuild or whole-original-VM parity claim. Existing frozen
runtime dependency reports remain historical and are not rewritten.
"""
import argparse,hashlib,json,subprocess
from pathlib import Path
WORLD=Path(__file__).resolve().parents[1];ROOT=WORLD.parents[1];RUNTIME=ROOT/'port/script-runtime'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--main-linked');a=ap.parse_args()
 scratch=ROOT/'.local-inputs/gameobject-lua-representation';scratch.mkdir(parents=True,exist_ok=True)
 units='lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib'.split()
 lua=[RUNTIME/'lua'/ (u+'.c') for u in units]
 inputs=[RUNTIME/p for p in ['script_runtime.h','script_runtime.c','script_object_bridge.h','script_object_bridge.c','script_object_bridge_internal.h']]
 inputs += [WORLD/p for p in ['gameobject_lua_representation.hpp','gameobject_lua_representation.cpp','gameobject_lua_catalog.inc','character_target_bindings.hpp','character_target_bindings.cpp','tests/gameobject_lua_representation.cpp','tests/gameobject_lua_host.py','tests/gameobject_lua_original.py','reference/gameobject-lua-representation/original-functions.json','reference/gameobject-lua-representation/reference/original-functions.asm','reference/gameobject-lua-representation/object-producer-probe.json','reference/gameobject-lua-representation/object-methods-original.bin']]
 inputs += lua+list((RUNTIME/'lua').glob('*.h'))
 inputs += [WORLD/p for p in ['tests/gameobject_lua_differential.py','tools/build_gameobject_lua_oracle.ps1','tools/generate_gameobject_lua_catalog.py','reports/gameobject-lua-arm64-differential.json','reference/gameobject-lua-representation/arguments/original-functions.json','reference/gameobject-lua-representation/arguments/reference/original-functions.asm']]
 before={p.relative_to(ROOT).as_posix():sha(p) for p in inputs};commands=[]
 def run(*args):
  r=subprocess.run(['wsl','--cd',linux(ROOT),*args],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0,commands[-1];return r.stdout.strip()
 flags=['-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer']
 lib=linux(scratch/'libdh2_script_object_runtime.so');exe=linux(scratch/'gameobject_lua_audit')
 if a.main_linked:exe=a.main_linked
 else:
  run('gcc','-fsyntax-only','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-I'+linux(RUNTIME/'lua'),linux(RUNTIME/'script_runtime.c'),linux(RUNTIME/'script_object_bridge.c'))
  run('gcc','-shared','-fPIC',*flags,'-I'+linux(RUNTIME/'lua'),linux(RUNTIME/'script_runtime.c'),linux(RUNTIME/'script_object_bridge.c'),*[linux(p) for p in lua],'-lm','-o',lib)
  run('g++','-std=c++17',*flags,'-Wall','-Wextra','-Werror','-Wno-misleading-indentation',linux(WORLD/'gameobject_lua_representation.cpp'),linux(WORLD/'character_target_bindings.cpp'),linux(WORLD/'tests/gameobject_lua_representation.cpp'),'-L'+linux(scratch),'-ldh2_script_object_runtime','-Wl,-rpath,'+linux(scratch),'-ldl','-o',exe)
 audit=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:abort_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,linux(WORLD/'reference/gameobject-lua-representation/object-methods-original.bin')))
 assert audit['validation']=='PASS' and audit['ordered_original_methods']==171
 runtime=audit['runtime_library'];binaries={p:run('sha256sum',p).split()[0] for p in [exe,runtime]};dependencies=run('ldd',exe)
 assert runtime in dependencies and 'libasan.so' in dependencies and 'libubsan.so' in dependencies
 assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in inputs}
 report=dict(validation='PASS',host_audit=audit,source_sha256=before,binary_sha256=binaries,commands=commands,linked_dependencies=dependencies,sanitizer_findings=0,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],main_linked=bool(a.main_linked),scope=__doc__,original_sha256='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80',whole_original_VM=False,packaged_APK=False)
 name='gameobject-lua-main-linked-host-audit.json' if a.main_linked else 'gameobject-lua-host-audit.json'
 (WORLD/'reports'/name).write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
