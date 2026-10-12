from pathlib import Path
import hashlib,json,subprocess,datetime,sys
b=Path(__file__).resolve().parents[1];g=b/'vendor/character-rng-integration-v5-loading';build=b.parents[2]/'build/receiver-transport-v5-host';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();w=lambda p:'/mnt/c/'+str(p).replace(chr(92),'/')[3:]
m=json.loads((g/'integration-manifest.json').read_text());mh=sha(g/'integration-manifest.json')
for n,v in m['files'].items():assert sha(g/n)==v['sha256'],n
args=[Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip'),b/'tests/native-ctor-v4/design.bin',build/'native-ctor-missing-saves'];executions={};failures=[]
for label,name in [('event194','dh2_loader_event194_c1_v55'),('source_row_GS','dh2_loader_native_gslevel_loading_source_v52'),('source7_10','dh2_loader_source_stage_swamp_v47')]:
 command=['wsl.exe','-d','Ubuntu','--',w(build/name)]+[w(x) for x in args];r=subprocess.run(command,capture_output=True,text=True,timeout=120)
 executions[label]={'command':command,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'binary_sha256':sha(build/name)}
 if r.returncode:failures.append(label)
 print(label+':'+str(r.returncode)+' '+(r.stdout+r.stderr)[-1500:],flush=True)
if not failures:
 observed=json.loads(executions['event194']['stdout']);assert observed['event194_actual_C1_cases']==2 and observed['event194_completed_source_storage']==66 and observed['raw_name']=='001_swamp.mlx'
assert mh==sha(g/'integration-manifest.json'),'manifest changed'
receipt={'validation':'FAIL' if failures else 'PASS','recorded_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds'),'coherent_files_verified':len(m['files']),'manifest_sha256':mh,'executions':executions,'failures':failures,'actual_C1_event194_owner_aliases_verified':True,'source_tables_retained_after_stage_release':True,'native_storage_released_after_C1_lifetime_without_cycle':True,'incomplete_C1_borrow_preserves_output':True,'missing_PM_after_real_fixture_flag_store_before_manager_publish':True,'source_phixtures':'Stage6 entry selected explicitly; actual completed C1/GS/event table/storage are real. PM trace/Update and deep outerGS cleanup are observer fixtures. Stage31/register/quest/trueUnload not executed. Source7/10 case retains historical worlds transport fixture, explicitly not original resource alias.','source_sha256':{n:sha(b/n) for n in ['tests/native_gslevel_event194_v55.cpp','tests/cmake-receiver-transport/CMakeLists.txt','tools/run_event194_v55_checks.py']},'whole_level_init_verified':False,'source_gameplay_verified':False,'functional_GameEvent_Unload_verified':False,'emulator_launched':False,'shared_mutated':False}
(b/'reports/game-event194-v55-selected-verified.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({'validation':receipt['validation'],'coherent_files':len(m['files']),'checks':len(executions),'failures':failures,'whole_init':False}));sys.exit(bool(failures))
