"""Owned complete DesignSettings prefix over actual cache under ASan/UBSan.

Builds only new owner/test plus frozen EnemySpotted source. No central DSO is
rebuilt. Unrelated design stream suffixes and Application registration are
explicitly outside this owner; native pin/atomic bounds are separately tested.
"""
import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+p.as_posix()[2:]
def main():
 scratch=REPO/'.local-inputs/design-settings';sources=[ROOT/'design_settings.cpp',ROOT/'design_settings.hpp',Path(__file__),ROOT/'tests/design_settings.cpp',REPO/'port/level-world/character_enemy_spotted.cpp',REPO/'port/level-world/character_enemy_spotted.hpp'];inputs=[ROOT/'reference/design-settings/design-settings-fixtures.bin',scratch/'design_pyarray.bin',scratch/'design_pyarraynames.bin',scratch/'design_pystructnames.bin'];proof=ROOT/'reports/design-settings-arm64-differential.json';paths=sources+inputs+[proof,ROOT/'reference/design-settings/original-functions.json',ROOT/'reference/design-settings/reference/original-functions.asm'];before={p.relative_to(REPO).as_posix():sha(p) for p in paths}
 executable=scratch/'design_settings_host';command=['wsl.exe','--','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-Wall','-Wextra','-Werror','-Wno-misleading-indentation',linux(sources[0]),linux(sources[3]),linux(sources[4]),'-o',linux(executable)];r=subprocess.run(command,text=True,capture_output=True,timeout=60);assert not r.returncode,(r.stdout,r.stderr)
 run=['wsl.exe','--','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',linux(executable),*map(linux,inputs)];r=subprocess.run(run,text=True,capture_output=True,timeout=60);assert not r.returncode and not r.stderr,(r.stdout,r.stderr);audit=json.loads(r.stdout);assert audit['validation']=='PASS' and audit['record_comparisons']==385 and audit['table_comparisons']==97 and audit['lookup_comparisons']==52
 assert before=={p.relative_to(REPO).as_posix():sha(p) for p in paths};report=dict(validation='PASS',host_audit=audit,source_and_input_sha256=before,executable_sha256=sha(executable),compiler_command=command,run_command=run,sanitizer_findings=0,sanitizers=['AddressSanitizer','UndefinedBehaviorSanitizer','LeakSanitizer'],scope=__doc__,actual_cache_prefix_decoded=True,full_design_file=False,whole_Application_registration=False);(ROOT/'reports/design-settings-host-audit.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(audit))
if __name__=='__main__':main()
