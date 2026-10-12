"""New teardown-era proof; preserve old alias reports and frozen VM DSO."""
import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[5];MODULE=ROOT/'port/script-runtime';HERE=Path(__file__).resolve().parent
SCRATCH=ROOT/'.local-inputs/script-function-alias/teardown';BUILD='/home/adampalace/dh2-script-alias-teardown-audit';VM_BUILD='/home/adampalace/dh2-lua514-build'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def linux(p):return '/mnt/c/'+p.resolve().as_posix()[3:]
def main():
 SCRATCH.mkdir(parents=True,exist_ok=True);commands=[]
 def run(args):
  r=subprocess.run(['wsl','-d','Ubuntu','--',*args],capture_output=True,text=True,timeout=120)
  commands.append({'arguments':args,'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr});assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 run(['mkdir','-p',BUILD]);audits={};compiler={}
 commons=ROOT/'port/level-world/reference/character-script-update/lua-inputs/ai-commons.luac'
 gold=HERE.parent/'alias-reference.bin';source_files=[MODULE/'script_function_alias.cpp',MODULE/'script_function_alias.h',MODULE/'script_runtime.h',MODULE/'tests/script_function_alias.cpp',MODULE/'tests/script_function_alias_teardown.cpp',Path(__file__)]
 before={p.relative_to(ROOT).as_posix():sha(p) for p in source_files};vmhash=run(['sha256sum',VM_BUILD+'/libdh2_script_runtime.so']).split()[0]
 previous=MODULE/'reports/script-function-alias-host-audit.json';old=json.loads(previous.read_text());assert vmhash==old['runtime_library']['sha256']
 for name,test,args in [('regression','script_function_alias.cpp',[linux(gold),linux(commons)]),('teardown','script_function_alias_teardown.cpp',[])]:
  cmd=['g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer',linux(MODULE/'script_function_alias.cpp'),linux(MODULE/'tests'/test),'-L'+VM_BUILD,'-ldh2_script_runtime','-Wl,-rpath,'+VM_BUILD,'-o',BUILD+'/'+name]
  compiler[name]=cmd;run(cmd);audits[name]=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',BUILD+'/'+name,*args]));assert audits[name]['validation']=='PASS'
  run(['cp',BUILD+'/'+name,linux(SCRATCH/name)])
 assert audits['regression']['original_cases']==4209 and audits['teardown']['actual_gc_callbacks']==8
 assert before=={p:sha(ROOT/p) for p in before};assert vmhash==run(['sha256sum',VM_BUILD+'/libdh2_script_runtime.so']).split()[0]
 instruction=MODULE/'reports/script-function-alias-teardown-arm64-differential.json';proof=json.loads(instruction.read_text());assert proof['validation']=='PASS' and proof['comparisons']==16
 for name,digest in proof['source_sha256'].items():assert sha(ROOT/name)==digest
 report={'validation':'PASS','host_audit':audits,'source_sha256':before,'compiler_arguments':compiler,'executable_sha256':{name:sha(SCRATCH/name) for name in audits},'runtime_library_sha256':vmhash,'preserved_host_report_sha256':sha(previous),'preserved_gold_sha256':sha(gold),'original_teardown_instruction_report_sha256':sha(instruction),'original_teardown_manifest_sha256':sha(HERE/'original-functions.json'),'original_teardown_assembly_sha256':sha(HERE/'reference/original-functions.asm'),'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'commands':commands,'packaged_apk':False,'full_original_teardown_ownership':False,'scope':'Current alias source replay of frozen original4209 corpus plus genuine Lua newproxy __gc Pop/Add callbacks during actual VM close with borrowed wrapper alive. Separate original teardown instruction probe establishes backup/main order and tracking preservation; full cache/path/argument destruction and manager ownership not supplied.'}
 out=MODULE/'reports/script-function-alias-teardown-host-audit.json';out.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host_audit':audits,'report_sha256':sha(out)}))
if __name__=='__main__':main()
