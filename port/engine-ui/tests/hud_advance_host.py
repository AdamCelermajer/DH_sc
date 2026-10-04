import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 paths=['port/engine-ui/hud_advance.hpp','port/engine-ui/hud_advance.cpp','port/engine-ui/tests/hud_advance.cpp'];before={x:sha(ROOT/x) for x in paths};repo='/mnt/c/'+str(ROOT)[3:].replace('\\','/');exe='/home/adampalace/dh2-hud-timeline-audit/hud_advance_audit';gold=ROOT/'port/engine-ui/reference/hud-sprite-timeline/advance-gold.bin'
 def run(args):
  p=subprocess.run(args,capture_output=True,text=True);assert p.returncode==0,(p.args,p.stdout,p.stderr);return p.stdout.strip()
 run(['wsl','-e','g++','-std=c++17','-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-I'+repo+'/port/engine-ui',repo+'/port/engine-ui/hud_advance.cpp',repo+'/port/engine-ui/tests/hud_advance.cpp','-o',exe]);result=json.loads(run(['wsl','-e','env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',exe,repo+'/port/engine-ui/reference/hud-sprite-timeline/advance-gold.bin']));assert before=={x:sha(ROOT/x) for x in paths};report=dict(validation='PASS',**result,source_sha256=before,gold_sha256=sha(gold),executable_sha256=run(['wsl','-e','sha256sum',exe]).split()[0],sanitizer_findings=0,sanitizers=['address','undefined','leak'],scope='Full original notify_need_advance gold. Borrowed coherent parent/weak storage and weak-control deletion provider; source reloaded/captured clear order preserved.');out=ROOT/'port/engine-ui/reports/hud-advance-host-audit.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))
if __name__=='__main__':main()
