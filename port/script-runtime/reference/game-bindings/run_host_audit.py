"""Bind real-VM/genuine-timer integration and original callback host replay."""
import hashlib,json,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
MODULE=ROOT/'port/script-runtime'
SCRATCH=ROOT/'.local-inputs/lua514-source'
BUILD='/home/adampalace/dh2-lua514-build'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def rel(p):return p.relative_to(ROOT).as_posix()
def wsl(p):return '/mnt/c/'+p.as_posix()[3:]
def run(name,path):
    result=subprocess.run(['wsl','-d','Ubuntu','--','env',
        'ASAN_OPTIONS=detect_leaks=1:halt_on_error=1','UBSAN_OPTIONS=halt_on_error=1',
        BUILD+'/'+name,wsl(path)],text=True,capture_output=True,check=True)
    assert not result.stderr.strip(),result.stderr
    audit=json.loads(result.stdout);assert audit['validation']=='PASS' and audit['mismatches']==0
    binary=SCRATCH/name
    subprocess.run(['wsl','-d','Ubuntu','--','cp',BUILD+'/'+name,wsl(binary)],check=True)
    return audit,{'path':rel(binary),'sha256':sha(binary)}
commons=ROOT/'port/level-world/reference/character-script-update/lua-inputs/ai-commons.luac'
integration,first=run('script_game_bindings_audit',commons)
replay,second=run('script_game_bindings_replay',HERE/'callback-reference.bin')
arm_path=MODULE/'reports/script-game-bindings-arm64-differential.json'
arm=json.loads(arm_path.read_text())
assert replay['original_callback_cases']==arm['comparisons']==5493
assert replay['ordered_timer_service_calls']==arm['ordered_timer_service_calls']==3194
for p,h in arm['source_sha256'].items():assert sha(ROOT/p)==h,p
sources=[p for p in MODULE.rglob('*') if p.is_file() and p.suffix in ('.c','.h','.cpp','.py')
    and 'vendor' not in p.parts and 'reference' not in p.parts]
sources+=[MODULE/'CMakeLists.txt',ROOT/'port/level-world/character_timers.cpp',
    ROOT/'port/level-world/character_timers.hpp',Path(__file__)]
report={'validation':'PASS','integration_audit':integration,'original_callback_replay':replay,
    'sanitizers':{'address':True,'undefined':True,'diagnostics':0},
    'source_sha256':{rel(p):sha(p) for p in sorted(set(sources))},
    'actual_commons_sha256':sha(commons),'original_sha256':sha(ROOT/'.local-inputs/libDungeonHunter2.so'),
    'reference_sha256':sha(HERE/'callback-reference.bin'),'arm64_report_sha256':sha(arm_path),
    'executables':{'integration':first,'original_replay':second},
    'library_sha256':sha(SCRATCH/'libdh2_script_runtime.so'),
    'original_return_projection_gold_sha256':sha(HERE/'return-projection-gold.json'),
    'original_return_projection_cases':7,
    'source_capture_sha256':{rel(p):sha(p) for p in HERE.rglob('*') if p.is_file()
        and (p.suffix=='.asm' or p.name in ('original-functions.json','registration-names.json'))},
    'game_timer_service_stubs':False,'shipping_Trace':'original bxLR/no-op',
    'full_CharAI_VM_ownership':False,'full_LuaManager_dispatch':False,'packaged_APK':False,
    'scope':'Actual commons executes in genuine source Lua VM against native source CharacterTimers; callback original replay uses explicit observed timer-service boundaries.'}
(MODULE/'reports/script-game-bindings-host-audit.json').write_text(json.dumps(report,indent=2,sort_keys=True)+'\n')
print(json.dumps({'integration':integration,'original_replay':replay}))
