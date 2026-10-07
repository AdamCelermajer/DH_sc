"""Compile only the additive bridge/audit against existing genuine sanitized DSOs."""
import argparse,hashlib,json,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/level-world'
REF=MODULE/'reference/character-script-call-timer';SCRATCH=ROOT/'.local-inputs/character-script-call-timer'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def wsl(p):return '/mnt/c/'+p.resolve().as_posix()[3:]
def main():
 ap=argparse.ArgumentParser();ap.add_argument('--build',default='/home/adampalace/dh2-world-build');a=ap.parse_args()
 SCRATCH.mkdir(parents=True,exist_ok=True);commands=[]
 def run(args):
  r=subprocess.run(['wsl','-d','Ubuntu','--',*args],text=True,capture_output=True,timeout=120)
  commands.append({'arguments':args,'returncode':r.returncode,'stdout':r.stdout,'stderr':r.stderr})
  assert r.returncode==0 and not r.stderr.strip(),commands[-1];return r.stdout.strip()
 sources=[MODULE/p for p in ('character_script_call_timer.hpp','character_script_call_timer.cpp','tests/character_script_call_timer.cpp','character_ai_events.hpp','character_ai_events.cpp','character_script_timers.hpp','character_script_timers.cpp','character_timers.hpp','character_timers.cpp')]
 sources += [p for p in (ROOT/'port/script-runtime').rglob('*') if p.is_file() and p.suffix in ('.h','.c','.cpp') and 'tests' not in p.parts]
 sources += [Path(__file__),REF/'probe_original.py']
 before={p.relative_to(ROOT).as_posix():sha(p) for p in sources}
 build='/home/adampalace/dh2-script-call-timer-audit';run(['mkdir','-p',build])
 compiler=['g++','-std=c++17','-O1','-g','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-fno-fast-math','-ffp-contract=off','-fsanitize=address,undefined','-fno-omit-frame-pointer',wsl(MODULE/'character_script_call_timer.cpp'),wsl(MODULE/'tests/character_script_call_timer.cpp'),wsl(ROOT/'port/script-runtime/script_function_alias.cpp'),'-L'+a.build,'-ldh2_level_world','-L'+a.build+'/script-runtime','-ldh2_script_runtime','-Wl,-rpath,'+a.build,'-Wl,-rpath,'+a.build+'/script-runtime','-o',build+'/audit']
 run(compiler);dependencies=run(['ldd',build+'/audit'])
 assert a.build+'/libdh2_level_world.so' in dependencies and a.build+'/script-runtime/libdh2_script_runtime.so' in dependencies
 assert 'libasan.so' in dependencies and 'libubsan.so' in dependencies
 binaries={build+'/audit',*re.findall(r'(/[^\s]+\.so(?:\.\d+)*)',dependencies)}
 binary_before={p:run(['sha256sum',p]).split()[0] for p in sorted(binaries)}
 ninja=run(['ninja','-C',a.build,'-t','commands','dh2_script_runtime','dh2_level_world'])
 for unit in ('script_runtime.c','script_function_alias.cpp','lua/lvm.c','character_ai_events.cpp','character_script_timers.cpp','character_timers.cpp'):
  rows=[line for line in ninja.splitlines() if unit in line and ' -c ' in line];assert rows and all('-fsanitize=address,undefined' in row for row in rows),unit
 commons=MODULE/'reference/character-script-update/lua-inputs/ai-commons.luac'
 assert sha(commons)=='20d34968e9983e14c24223085ab40d55d47f9387dfdbbf3ff7e6b39aea91252c'
 original=json.loads((REF/'original-probe.json').read_text());assert original['validation']=='PASS' and original['gold_sha256']==sha(REF/'timer-call-reference.bin')
 audit=json.loads(run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',build+'/audit',wsl(REF/'timer-call-reference.bin'),wsl(commons)]))
 assert audit['validation']=='PASS' and audit['mismatches']==0 and audit['original_cases']==576 and audit['original_active_calls']==288
 assert before=={p:sha(ROOT/p) for p in before}
 assert binary_before=={p:run(['sha256sum',p]).split()[0] for p in sorted(binaries)}
 run(['cp',build+'/audit',wsl(SCRATCH/'audit')])
 evidence=[ROOT/'port/script-runtime/reports/script-function-alias-teardown-arm64-regression.json',ROOT/'port/script-runtime/reports/script-function-alias-teardown-arm64-differential.json',MODULE/'reports/character-ai-events-arm64-differential.json']
 report={'validation':'PASS','host_audit':audit,'source_sha256':before,'binary_sha256':binary_before,'compiler_arguments':compiler,'linked_dependencies':dependencies,'existing_DSO_compiler_commands':ninja,'original_sha256':original['original_sha256'],'original_probe_sha256':sha(REF/'original-probe.json'),'original_manifest_sha256':sha(REF/'original-functions.json'),'original_assembly_sha256':sha(REF/'reference/original-functions.asm'),'gold_sha256':sha(REF/'timer-call-reference.bin'),'actual_commons_sha256':sha(commons),'preceding_instruction_proof':{p.relative_to(ROOT).as_posix():sha(p) for p in evidence},'sanitizers':['AddressSanitizer','UndefinedBehaviorSanitizer'],'sanitizer_findings':0,'commands':commands,'full_AIS_ownership':False,'packaged_apk':False,'arm64_adapter_instruction_proof':False,'scope':'576 original selected timer/Value/one-pass alias/global lookup probes composed with genuine existing host world and float32 Lua DSOs. Original allocation/container/global-call execution are explicit services; host Lua execution and timer expiry are real. New borrowed bridge does not allocate/publish selected AIS or establish teardown/manager ownership.'}
 output=MODULE/'reports/character-script-call-timer-host-audit.json';output.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','host_audit':audit,'report_sha256':sha(output)}))
if __name__=='__main__':main()
