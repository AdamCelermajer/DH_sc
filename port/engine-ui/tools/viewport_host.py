"""Build/replay the isolated source-bound viewport gold under ASan/UBSan."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):
 p=Path(p).resolve();return '/mnt/'+p.drive[0].lower()+str(p)[2:].replace('\\','/')
def main():
 p=argparse.ArgumentParser();p.add_argument('--executable',default='/home/adampalace/dh2-viewport-audit/viewport_audit');p.add_argument('--output',type=Path,default=ROOT/'reports/viewport-host-audit.json');p.add_argument('--build',action='store_true');a=p.parse_args()
 sources=(ROOT/'viewport.hpp',ROOT/'viewport.cpp',ROOT/'tests/viewport.cpp');before={str(x.relative_to(ROOT)):sha(x) for x in sources};gold=ROOT/'reference/viewport/viewport-gold.bin'
 flags=['-std=c++17','-O1','-g','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror']
 command=['wsl','-e','g++',*flags,'-I'+linux(ROOT),linux(sources[1]),linux(sources[2]),'-o',a.executable]
 if a.build:
  subprocess.run(['wsl','-e','mkdir','-p',a.executable.rsplit('/',1)[0]],check=True)
  build=subprocess.run(command,text=True,capture_output=True);assert build.returncode==0,build.stdout+build.stderr
 run=subprocess.run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1','UBSAN_OPTIONS=print_stacktrace=1',a.executable,linux(gold)],text=True,capture_output=True)
 assert run.returncode==0 and not run.stderr,run.stdout+run.stderr
 proof=json.loads(run.stdout);assert proof['comparisons']==5200 and proof['ordered_services']==12683 and proof['failure_reentry_guards']==12 and proof['mismatches']==0
 after={str(x.relative_to(ROOT)):sha(x) for x in sources};assert before==after
 binary=subprocess.run(['wsl','-e','sha256sum',a.executable],text=True,capture_output=True,check=True).stdout.split()[0]
 dependencies=subprocess.run(['wsl','-e','ldd',a.executable],text=True,capture_output=True,check=True).stdout;assert 'libasan' in dependencies and 'libubsan' in dependencies
 report=dict(validation='PASS',**proof,sanitizer_findings=0,sanitizers=['address','undefined','leak'],source_sha256=before,gold_sha256=sha(gold),executable=a.executable,executable_sha256=binary,compile_command=command if a.build else None,compiler_provenance=a.build,dependency_inventory=dependencies,runner_sha256=sha(Path(__file__)),limits={'gpu_pixels':False,'whole_menu_camera_owner':False,'original_as_object_publication':False,'driver_orientation_producer':False,'original_weak_player_ownership':False},scope='Real original-gold coupled viewport arithmetic/state/services; required provider failures and synchronous publication reentry are native guards.')
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');a.output.with_suffix('.stderr.txt').write_text(run.stderr);print(json.dumps(report))
if __name__=='__main__':main()
