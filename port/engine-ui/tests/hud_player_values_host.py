"""Build an isolated sanitizer audit; never rebuild shared central DSOs."""
import argparse,hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser();p.add_argument('--output',type=Path,default=ROOT/'port/engine-ui/reports/hud-player-values-host-audit.json');p.add_argument('--build-dir',default='/home/adampalace/dh2-hud-values-audit');a=p.parse_args();paths=['port/engine-ui/hud_player_values.hpp','port/engine-ui/hud_player_values.cpp','port/engine-ui/tests/hud_player_values.cpp'];before={x:sha(ROOT/x) for x in paths};repo='/mnt/c/'+str(ROOT)[3:].replace('\\','/');exe=a.build_dir+'/hud_player_values_audit';gold=ROOT/'port/engine-ui/reference/hud-player-values/status-frames-gold.bin'
 def run(args):
  q=subprocess.run(args,capture_output=True,text=True);assert q.returncode==0,(q.args,q.stdout,q.stderr);return q.stdout.strip()
 run(['wsl','-e','mkdir','-p',a.build_dir]);run(['wsl','-e','g++','-std=c++17','-O1','-g','-fPIC','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-I'+repo+'/port/engine-ui',repo+'/port/engine-ui/hud_player_values.cpp',repo+'/port/engine-ui/tests/hud_player_values.cpp','-o',exe]);result=json.loads(run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,repo+'/port/engine-ui/reference/hud-player-values/status-frames-gold.bin']));after={x:sha(ROOT/x) for x in paths};assert before==after;report=dict(validation='PASS',**result,source_sha256=before,gold_sha256=sha(gold),executable_sha256=run(['wsl','-e','sha256sum',exe]).split()[0],sanitizers=['address','undefined','leak'],sanitizer_findings=0,scope='Actual original status-region gold replay. Cached clips/sprite backend/runtime-zero result are explicit projections; synchronous nested wrapper/failure-prefix ownership tested. No full FastUpdate, sprite AS scheduling or live GPU claim.');a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
