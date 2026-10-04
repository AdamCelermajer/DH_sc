"""ClearAggro original-gold replay and actual monster OutSight Lua composition.

Builds only new ClearAggro shim/test. Central data/world/runtime kernels and
retained CharacterScriptObjects execute from dependency-bound sanitized DSOs.
Notification/Stop/host/FindPath/sound bodies remain explicit fixtures.
"""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
WORLD=Path(__file__).resolve().parents[1];ROOT=WORLD.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-world-build');a=p.parse_args();scratch=ROOT/'.local-inputs/character-clear-aggro';scratch.mkdir(exist_ok=True)
 sources=[WORLD/p for p in ['character_clear_aggro.cpp','character_clear_aggro.hpp','tests/character_clear_aggro.cpp','tests/character_clear_aggro_host.py','tests/character_clear_aggro_differential.py','tests/character_target_pipeline.cpp']]
 interfaces=[WORLD/(name+ext) for name in ['character_script_objects','character_script_session','character_target_bindings','character_target_events','character_ai_events','character_enemy_spotted'] for ext in ['.cpp','.hpp']]+[ROOT/'port/game-data/aggro.cpp',ROOT/'port/game-data/aggro.hpp',ROOT/'port/script-runtime/script_runtime.c',ROOT/'port/script-runtime/script_runtime.h',ROOT/'port/script-runtime/script_object_bridge.c',ROOT/'port/script-runtime/script_object_bridge.h']
 inputs=[WORLD/'reference/character-clear-aggro/clear-aggro-fixtures.bin',WORLD/'reference/character-game-design/real-cache-inputs.bin',ROOT/'.local-inputs/character-script-owner-discovery/ai-commons-source.luac',ROOT/'.local-inputs/character-script-owner-extension/monster.luac',WORLD/'reference/character-enemy-spotted/enemy-spotted-fixtures.bin']
 proofs=[WORLD/'reference/character-clear-aggro/source-probes.json',WORLD/'reference/character-clear-aggro/original-functions.json',WORLD/'reference/character-clear-aggro/reference/original-functions.asm',WORLD/'reports/character-clear-aggro-arm64-differential.json']
 paths=sources+interfaces+inputs+proofs;before={p.relative_to(ROOT).as_posix():sha(p) for p in paths};commands=[]
 def run(*argv):
  r=subprocess.run(['wsl.exe','--',*argv],capture_output=True,text=True,timeout=120);commands.append(dict(arguments=list(argv),returncode=r.returncode,stdout=r.stdout,stderr=r.stderr));assert not r.returncode,(r.stdout,r.stderr);return r.stdout
 def hashes(paths):return {line.split(maxsplit=1)[1].strip():line.split()[0] for line in run('sha256sum',*sorted(paths)).splitlines()}
 deps={a.build+'/libdh2_level_world.so',a.build+'/game-data/libdh2_game_data.so',a.build+'/script-runtime/libdh2_script_runtime.so'};frozen=hashes(deps)
 flags=['-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation']
 links=[]
 for sub,lib in [('', 'level_world'),('/game-data','game_data'),('/script-runtime','script_runtime')]:links+=['-L'+a.build+sub,'-ldh2_'+lib,'-Wl,-rpath,'+a.build+sub]
 lib=linux(scratch/'libcharacter_clear_aggro_host.so');exe=linux(scratch/'character_clear_aggro_host')
 run('g++',*flags,'-shared','-fPIC',linux(sources[0]),*links,'-Wl,--no-undefined','-o',lib)
 run('g++',*flags,linux(sources[2]),'-L'+linux(scratch),'-lcharacter_clear_aggro_host','-Wl,-rpath,'+linux(scratch),*links,'-ldl','-o',exe)
 loaded=run('ldd',exe);assert 'not found' not in loaded and 'libasan.so' in loaded and 'libubsan.so' in loaded;dependencies={exe}
 for line in loaded.splitlines():
  m=re.search(r'=>\s+(/\S+)|^\s*(/\S+)',line)
  if m:dependencies.add(m.group(1) or m.group(2))
 assert deps.issubset(dependencies) and lib in dependencies;binary=hashes(dependencies);assert all(binary[p]==h for p,h in frozen.items())
 audit=json.loads(run('env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,*map(linux,inputs)))
 assert audit['validation']=='PASS' and audit['source_records']==1792 and audit['ordered_calls']==200 and audit['actual_monster_OutSight']==1 and audit['nested_VM_events']==1 and audit['library']==lib
 assert before=={p.relative_to(ROOT).as_posix():sha(p) for p in paths} and binary==hashes(dependencies)
 report=dict(validation='PASS',host_audit=audit,source_and_input_sha256=before,binary_sha256=binary,commands=commands,dependencies=loaded,sanitizer_findings=0,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],scope=__doc__,central_compiler_provenance='actual dependency hashes only; parent owns central compiler binding',original_instruction_differential_report_sha256=sha(proofs[-1]),full_enemy_AI=False,whole_chain_original_differential=False,packaged_APK=False,physical_ARM64=False)
 (WORLD/'reports/character-clear-aggro-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
