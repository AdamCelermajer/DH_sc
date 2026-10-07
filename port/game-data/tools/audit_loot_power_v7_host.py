"""Isolated V7 source connected to actual central inventory/text providers.
The exact current DSOs are bound before/after; this is not a live APK claim.
"""
import hashlib,json,shlex,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+str(p)[2:].replace('\\','/')
def run(c):
 p=subprocess.run(['wsl.exe','-e','bash','-lc',c],capture_output=True,text=True)
 if p.returncode or p.stderr.strip():raise RuntimeError(p.stdout+p.stderr)
 return p.stdout.strip()
def main():
 src=['port/game-data/loot_power_resources_v7.cpp','port/game-data/loot_power_creation_v7.cpp','port/game-data/tests/loot_power_creation_v7.cpp','port/game-data/tests/loot_power_creation_v7_fixture.cpp']
 tracked=[R/x for x in src]+[R/'port/game-data/loot_power_resources_v7.hpp',R/'port/game-data/loot_power_creation_v7.hpp',Path(__file__)]
 # Compiler headers: explicitly include all transitive project headers instead
 # of attributing historical dependency object code to these moving sources.
 tracked+=list((R/'port').rglob('*.hpp'))+list((R/'port/script-runtime').rglob('*.h'))
 before={p.relative_to(R).as_posix():sha(p)for p in sorted(set(tracked))}
 build='/home/adampalace/dh2-world-build';dirs=[build,build+'/game-data',build+'/engine-ui',build+'/script-runtime'];deps=[build+'/libdh2_level_world.so',dirs[1]+'/libdh2_game_data.so',dirs[2]+'/libdh2_engine_ui.so',dirs[3]+'/libdh2_script_runtime.so'];dsos=run(shlex.join(['sha256sum',*deps]))
 flags=['g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer']
 exe=R/'.local-inputs/player-loot-v7/host-audit';compile=shlex.join([*flags,*src,*['-L'+p for p in dirs],'-Wl,-rpath,'+':'.join(dirs),'-ldh2_level_world','-ldh2_engine_ui','-ldh2_game_data','-ldh2_script_runtime','-o',linux(exe)])
 prefix='cd '+shlex.quote(linux(R))+' && ';run(prefix+compile)
 args=['port/game-data/reference/loot-power-creation-v7/fixtures.bin','.local-inputs/player-loot-v7/cache','.local-inputs/actors','port/android-native/app/src/main/assets','.local-inputs/player-item-effects-v5/private-save'];command='ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+shlex.join([linux(exe),*args]);result=json.loads(run(prefix+command));assert result['validation']=='PASS'and result['mismatches']==0
 assert before=={p.relative_to(R).as_posix():sha(p)for p in sorted(set(tracked))}
 assert dsos==run(shlex.join(['sha256sum',*deps]))
 input_files=list((R/args[1]).glob('*.bin'))+list((R/args[2]).glob('*.bin'))+[R/args[0]]+list((R/args[3]/'original-cache/data/pydata').glob('common_text_*.bin'))+list((R/args[3]/'original-cache/data/text').rglob('*'))
 evidence=R/'port/game-data/reports/loot-power-creation-v7-arm64-differential.json';assert json.loads(evidence.read_text())['validation']=='PASS'
 report=dict(validation='PASS',result=result,source_sha256=before,compiler_command=compile,run_command=command,binary_sha256=sha(exe),linked_dependencies=run(shlex.join(['ldd',linux(exe)])),dependency_sha256={x.split(maxsplit=1)[1]:x.split()[0]for x in dsos.splitlines()},input_sha256={p.relative_to(R).as_posix():sha(p)for p in input_files if p.is_file()},original_differential_sha256=sha(evidence),sanitizers=dict(address=True,undefined=True,leaks=True,findings=0),scope=__doc__)
 path=R/'port/game-data/reports/loot-power-creation-v7-host-audit.json';path.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',result=result,report=str(path))))
if __name__=='__main__':main()
