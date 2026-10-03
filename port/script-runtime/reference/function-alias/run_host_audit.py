"""Build only the new alias module/audit; link the existing frozen sanitized VM DSO."""
import hashlib,json,shlex,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4];HERE=Path(__file__).resolve().parent
MODULE=ROOT/'port/script-runtime';SCRATCH=ROOT/'.local-inputs/script-function-alias'
BUILD='/home/adampalace/dh2-script-alias-audit';VM_BUILD='/home/adampalace/dh2-lua514-build'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def rel(p):return p.relative_to(ROOT).as_posix()
def wsl(p):return '/mnt/c/'+p.as_posix()[3:]
def run(args,**kw):return subprocess.run(['wsl','-d','Ubuntu','--',*args],text=True,capture_output=True,check=True,**kw)
SCRATCH.mkdir(parents=True,exist_ok=True);run(['mkdir','-p',BUILD])
sources=[MODULE/'script_function_alias.cpp',MODULE/'tests/script_function_alias.cpp']
compiler=['g++','-std=c++17','-O1','-g','-fno-fast-math','-ffp-contract=off',
          '-fsanitize=address,undefined','-fno-omit-frame-pointer',*[wsl(p) for p in sources],
          '-L'+VM_BUILD,'-ldh2_script_runtime','-Wl,-rpath,'+VM_BUILD,'-o',BUILD+'/script_function_alias_audit']
run(compiler)
commons=ROOT/'port/level-world/reference/character-script-update/lua-inputs/ai-commons.luac'
result=run(['env','ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',
            BUILD+'/script_function_alias_audit',wsl(HERE/'alias-reference.bin'),wsl(commons)])
assert not result.stderr.strip(),result.stderr;audit=json.loads(result.stdout)
assert audit['validation']=='PASS' and audit['mismatches']==0
binary=SCRATCH/'script_function_alias_audit';library=SCRATCH/'libdh2_script_runtime.so'
run(['cp',BUILD+'/script_function_alias_audit',wsl(binary)]);run(['cp',VM_BUILD+'/libdh2_script_runtime.so',wsl(library)])
runtime_report_path=MODULE/'reports/lua514-host-audit.json';runtime_report=json.loads(runtime_report_path.read_text())
assert runtime_report['library_sha256']==sha(library),'VM DSO is not the source-bound frozen audited library'
for p,h in runtime_report['source_sha256'].items():assert sha(ROOT/p)==h,p
original=json.loads((HERE/'original-probe.json').read_text());assert original['cases']==audit['original_cases']==4209
assert original['gold_sha256']==sha(HERE/'alias-reference.bin')
sources += [MODULE/'script_function_alias.h',MODULE/'script_runtime.h',Path(__file__),HERE/'probe_original.py',HERE/'inventory_cache.py']
report={'validation':'PASS','host_audit':audit,'sanitizers':{'address':True,'undefined':True,'diagnostics':0},
        'compiler_arguments':compiler,'source_sha256':{rel(p):sha(p) for p in sources},
        'original_sha256':original['original_sha256'],'original_probe_sha256':sha(HERE/'original-probe.json'),
        'gold_sha256':sha(HERE/'alias-reference.bin'),'manifest_sha256':sha(HERE/'original-functions.json'),
        'assembly_sha256':sha(HERE/'reference/original-functions.asm'),'actual_commons_sha256':sha(commons),
        'cache_inventory_sha256':sha(HERE/'cache-alias-inventory.json'),
        'executable':{'path':rel(binary),'sha256':sha(binary)},
        'runtime_library':{'path':rel(library),'sha256':sha(library),'source_report':rel(runtime_report_path),'source_report_sha256':sha(runtime_report_path)},
        'original_globals_lookup_cases':len(original['original_call_globals_lookups']),
        'explicit_original_services':original['explicit_services'],
        'packaged_apk':False,'arm64_instruction_differential':False,'full_lua_manager':False,
        'scope':'Original ARM32 alias kernels with explicit container/string allocation services versus native64 host replay, plus genuine float32 Lua DSO binding/call integration. No original full VM or allocator identity parity.'}
(MODULE/'reports/script-function-alias-host-audit.json').write_text(json.dumps(report,indent=2)+'\n')
print(json.dumps({'validation':report['validation'],'host_audit':audit,'executable_sha256':sha(binary)}))
