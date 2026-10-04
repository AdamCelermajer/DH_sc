"""Private versioned indexed-return runtime build; no central library mutation."""
import argparse,hashlib,json,shlex,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/'+p.drive[0].lower()+str(p)[2:].replace('\\','/')
commands=[]
def run(c):
 commands.append(c);p=subprocess.run(['wsl','-e','bash','-lc',c],capture_output=True,text=True,encoding='utf8',errors='replace');assert p.returncode==0,(c,p.stdout,p.stderr);assert not p.stderr.strip(),p.stderr;return p.stdout.strip()
def main():
 p=argparse.ArgumentParser();p.add_argument('--build',default='/home/adampalace/dh2-character-skill-gameplay-v3/runtime');a=p.parse_args();runtime=ROOT/'port/script-runtime';src=['script_runtime_return_v3.c','script_game_bindings.c','script_scalar_bindings.c','script_design_bindings.c','script_object_bridge.c']+['lua/'+x+'.c'for x in 'lapi lcode ldebug ldo ldump lfunc lgc llex lmem lobject lopcodes lparser lstate lstring ltable ltm lundump lvm lzio lauxlib lbaselib ltablib lstrlib lmathlib'.split()];cpp=['script_function_alias.cpp','script_constants.cpp','script_int_bindings.cpp'];test=runtime/'tests/script_indexed_return_v3.cpp';legacy=runtime/'tests/script_first_return_v1.cpp';tracked=sorted({x for x in runtime.rglob('*')if x.is_file()and x.suffix in('.c','.cpp','.h','.hpp')});before={x.relative_to(ROOT).as_posix():sha(x)for x in tracked};run('mkdir -p '+shlex.quote(a.build));common=['-O1','-g','-fsanitize=address,undefined','-fno-omit-frame-pointer','-fno-fast-math','-ffp-contract=off','-fPIC'];objects=[]
 for i,s in enumerate(src+cpp):
  target=a.build+'/'+str(i)+'.o';objects.append(target);compiler='g++'if s in cpp else 'gcc';flags=['-std=c++17']if s in cpp else ['-std=c99'];run(shlex.join([compiler]+flags+common+['-I'+linux(runtime/'lua'),'-c',linux(runtime/s),'-o',target]))
 run(shlex.join(['g++']+common+['-shared']+objects+['-lm','-o',a.build+'/libdh2_script_runtime.so']));results={}
 for name,path in [('indexed',test),('legacy_first',legacy)]:
  exe=a.build+'/'+name+'_audit';run(shlex.join(['g++','-std=c++17']+common+[linux(path),'-L'+a.build,'-ldh2_script_runtime','-Wl,-rpath,'+a.build,'-o',exe]));results[name]=json.loads(run('ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1 '+shlex.quote(exe)))
 assert before=={x.relative_to(ROOT).as_posix():sha(x)for x in tracked},'Runtime compiler input changed';art=run('sha256sum '+shlex.quote(a.build)+'/*.so '+shlex.quote(a.build)+'/*_audit');report=dict(validation='PASS',results=results,sanitizers=dict(address=True,undefined=True,leaks=True,findings=0),compiler_inputs_sha256=before,artifacts=art,commands=commands,scope=__doc__+' Frozen V1 TU included exactly once. New indexed API preserves one actual Call/all ordered source return projections. Original Lua 5.1.4 core is the native retained VM; no ARM32 original Lua runtime execution claim.')
 out=ROOT/'port/level-world/reports/character-skill-gameplay-v3-indexed-runtime-host-audit.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(dict(validation='PASS',results=results,report=str(out))))
if __name__=='__main__':main()
