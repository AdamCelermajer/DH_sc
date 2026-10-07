"""Audit current central DSOs for constants, private integers, Level and FSM."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+str(p.resolve()).replace('\\','/')[3:]
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--build',default='/home/adampalace/dh2-world-build');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
 if a.output.exists():raise RuntimeError('Refusing to replace foundation proof')
 runtime=ROOT/'port/script-runtime';world=ROOT/'port/level-world';assets=ROOT/'port/android-native/app/src/main/assets/data';resources=ROOT/'.local-inputs/character-level-constants-discovery'
 files=[runtime/x for x in ['script_runtime.c','script_runtime.h','script_design_bindings.c','script_design_bindings.h','script_constants.cpp','script_constants.hpp','script_int_bindings.cpp','script_int_bindings.hpp','tests/script_constants.cpp','tests/script_int.cpp','CMakeLists.txt']]
 files += [world/x for x in ['character_level.cpp','character_level.hpp','character_native_fsm.cpp','character_native_fsm.hpp','character_state.cpp','character_state.hpp','tests/character_level.cpp','tests/character_level_constants.cpp','tests/character_native_fsm.cpp','CMakeLists.txt']]
 files += [Path(__file__),assets/'ai_pycst.bin',assets/'design_pycst.bin']
 sources={str(x.relative_to(ROOT)):sha(x) for x in files};commands=[]
 def run(*args):
  r=subprocess.run(['wsl.exe','--cd',linux(ROOT),*args],capture_output=True,text=True,timeout=60);commands.append(dict(arguments=list(args),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 constants=runtime/'reference/design-constants-loader/constants-original-gold.bin';ints=runtime/'reference/int-bindings/int-original-gold.bin';level=world/'reference/character-level/level-fixtures.bin';fsm=world/'reference/character-native-fsm/native-update-fixtures.bin'
 suites={'constants':(a.build+'/script-runtime/script_constants_audit',[constants]),'integers':(a.build+'/script-runtime/script_int_audit',[ints]),'level':(a.build+'/character_level_audit',[level,resources/'_commons.luac',resources/'monster.luac',assets]),'level_constants':(a.build+'/character_level_constants_audit',[level,resources/'_commons.luac',resources/'monster.luac',assets,assets/'ai_pycst.bin',assets/'design_pycst.bin']),'native_fsm':(a.build+'/character_native_fsm_audit',[fsm])}
 binaries=[a.build+'/script-runtime/libdh2_script_runtime.so',a.build+'/libdh2_level_world.so',a.build+'/game-data/libdh2_game_data.so']+[x[0] for x in suites.values()]
 def hashes():return {line.split(maxsplit=1)[1]:line.split()[0] for line in run('sha256sum',*binaries).splitlines()}
 before=hashes();audits={};dependencies={}
 compiler=json.loads(run('cat',a.build+'/compile_commands.json'));suffixes=['/script-runtime/script_constants.cpp','/script-runtime/script_int_bindings.cpp','/level-world/character_level.cpp','/level-world/character_native_fsm.cpp','/script-runtime/tests/script_constants.cpp','/script-runtime/tests/script_int.cpp','/level-world/tests/character_level.cpp','/level-world/tests/character_level_constants.cpp','/level-world/tests/character_native_fsm.cpp']
 records=[r for r in compiler if any(r['file'].endswith(s) for s in suffixes)];assert len(records)==9 and all('-fsanitize=address,undefined' in r['command'] for r in records)
 for name,(exe,args) in suites.items():
  dependencies[name]=run('ldd',exe);assert a.build+'/script-runtime/libdh2_script_runtime.so' in dependencies[name] and 'libasan.so' in dependencies[name] and 'libubsan.so' in dependencies[name] and 'not found' not in dependencies[name]
  audits[name]=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,*(linux(x) for x in args)));assert audits[name]['validation']=='PASS'
 assert audits['constants']['original_queries']==5969 and audits['integers']['original_gold_cases']==3163
 assert audits['level_constants']['comparisons']==870 and audits['level_constants']['real_constant_loads']==2
 assert audits['native_fsm']['module_library']==a.build+'/libdh2_level_world.so'
 assert sources=={str(x.relative_to(ROOT)):sha(x) for x in files} and before==hashes()
 report=dict(validation='PASS',scope=__doc__,audits=audits,source_sha256=sources,binary_sha256=before,compiler_records=records,original_gold_sha256={str(x.relative_to(ROOT)):sha(x) for x in [constants,ints,level,fsm]},dependencies=dependencies,commands=commands,sanitizer_findings=0,full_game_verified=False,physical_arm64_verified=False,apk_executed=False,live_enemy_AI=False,debug_and_effect_services_explicit=True)
 a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',audits=audits,binaries=before)))
if __name__=='__main__':main()
