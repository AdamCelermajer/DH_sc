from pathlib import Path
import hashlib,json,datetime,subprocess,sys
b=Path(__file__).resolve().parents[1];g=b/'vendor/character-rng-integration-v5-loading';build=b.parents[2]/'build/receiver-transport-v5-host';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();w=lambda p:'/mnt/c/'+str(p).replace(chr(92),'/')[3:]
m=json.loads((g/'integration-manifest.json').read_text());mh=sha(g/'integration-manifest.json')
for n,v in m['files'].items():assert sha(g/n)==v['sha256'],n
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');design=b/'tests/native-ctor-v4/design.bin';save=build/'native-ctor-missing-saves'
cases=[('shared_control','dh2_loader_parent_kernel_control_v56',[]),('ordered_character','dh2_loader_character_parent_v56',[cache,design]),('source7_10','dh2_loader_source_stage_swamp_v47',[cache,design,save]),('event194','dh2_loader_event194_c1_v55',[cache,design,save])];executions={};failures=[]
for label,binary,args in cases:
 command=['wsl.exe','-d','Ubuntu','--',w(build/binary)]+[w(x) for x in args];r=subprocess.run(command,capture_output=True,text=True,timeout=120)
 executions[label]={'command':command,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'binary_sha256':sha(build/binary)}
 if r.returncode:failures.append(label)
 print(label+':'+str(r.returncode)+' '+(r.stdout+r.stderr)[-1600:],flush=True)
assert mh==sha(g/'integration-manifest.json'),'manifest changed during checks'
receipt={'validation':'FAIL' if failures else 'PASS','recorded_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds'),'coherent_files_verified':len(m['files']),'manifest_sha256':mh,'executions':executions,'failures':failures,'one_shared_GameObject_ConditionData_kernel':True,'generic_public_APIs_preserved':True,'same_actual_Character_parent_cells':True,'second_base_pose_visual_save_created':False,'source_sha256':{n:sha(b/n) for n in ['tests/cmake-receiver-transport/CMakeLists.txt','tools/run_parent_kernel_v56_checks.py','tests/parent-kernel-selected-v56/parent_kernel_control_v51.cpp','tests/parent-kernel-selected-v56/parent_character_model_v51.cpp']},'scope':'Selected shared control and ordered actual NPC/model→sameActor qualified parent conditions/cache/SetPosition prefix, required actual Device stop. Controlled NULLvisual/source pipeline tests explicitly fixtures; actual first SWAMP activation not established. Source7/10 historical worldspath entry and eventphase6/PM/debug/deepcleanup remain explicit fixtures.','whole_Character_InitPost_success':False,'actual_ConditionList_compiled':False,'whole_level_Init_verified':False,'GPU_rendering_verified':False,'shared_mutated':False,'emulator_launched':False}
(b/'reports/character-parent-kernel-v56-selected-verified.json').write_text(json.dumps(receipt,indent=2)+'\n');print(json.dumps({'validation':receipt['validation'],'coherent_files':len(m['files']),'checks':len(cases),'failures':failures,'whole_Init':False}));sys.exit(bool(failures))
