from pathlib import Path
import hashlib,json,datetime,subprocess,sys
b=Path(__file__).resolve().parents[1];g=b/'vendor/character-rng-integration-v5-loading';build=b.parents[2]/'build/receiver-transport-v5-host';sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest();w=lambda p:'/mnt/c/'+str(p).replace(chr(92),'/')[3:]
m=json.loads((g/'integration-manifest.json').read_text());mh=sha(g/'integration-manifest.json')
for n,v in m['files'].items():assert sha(g/n)==v['sha256'],n
cache=Path('C:/Users/adamc/Downloads/dungeonhunter2/Dungeon-Hunter-2-HD-v1-0-2-cache.zip');design=b/'tests/native-ctor-v4/design.bin';save=build/'native-ctor-missing-saves'
cases=[('parent_cells','dh2_loader_character_parent_cells_v51',[cache,design]),('name','dh2_loader_character_name_v48',[cache,design]),('spawn','dh2_loader_character_spawn_name_v48',[cache,design]),('model','dh2_loader_character_model_v47',[cache,design]),('source_row_GS','dh2_loader_native_gslevel_loading_source_v52',[cache,design,save]),('source_stage','dh2_loader_source_stage_swamp_v47',[cache,design,save]),('image_lifetime','dh2_loader_actor_image_lifetime_v48_probe',[cache,design,save]),('external_script','dh2_loader_external_script_prefix_v52',[])]
executions={};failures=[]
for label,binary,args in cases:
 command=['wsl.exe','-d','Ubuntu','--',w(build/binary)]+[w(x) for x in args];r=subprocess.run(command,capture_output=True,text=True,timeout=120)
 executions[label]={'command':command,'exit_code':r.returncode,'stdout':r.stdout,'stderr':r.stderr,'binary_sha256':sha(build/binary)}
 if r.returncode:failures.append(label)
 print(label+':'+str(r.returncode)+' '+(r.stdout+r.stderr)[-1050:],flush=True)
assert mh==sha(g/'integration-manifest.json'),'manifest changed during checks'
if not failures:
 gs=json.loads(executions['source_row_GS']['stdout']);assert gs['raw_name']=='001_swamp.mlx' and gs['row3c']==41 and gs['actual_gs_cases']==2
 assert 'original_parent_cells=8' in executions['parent_cells']['stdout']
 assert 'native_external_script_prefix_cases=10' in executions['external_script']['stdout']
receipt={'validation':'FAIL' if failures else 'PASS','recorded_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(timespec='seconds'),'coherent_manifest_files_verified':len(m['files']),'manifest_sha256':mh,'executions':executions,'failed_checks':failures,'source_sha256':{n:sha(b/n) for n in ['tests/cmake-receiver-transport/CMakeLists.txt','tests/native_gslevel_loading_source_v52.cpp','tests/game_object_external_script_prefix_v52.cpp','tools/run_loader_v53_checks.py']},'original_filename_selection':'Actual initialized LevelList row41.file001_swamp.mlx; row42.file022_swamp2.rule.xml checked. Historical worlds argument is a constructor transport fixture, not original resource alias.','source_script_prefix':'Original deleting destructor before empty-name gate;142 ARM cases include68 actual empty container scripts. Nonempty suffix remains real main Lua provider.','whole_GameObject_parent_InitPost_wired':False,'actual_ConditionList_compiled':False,'whole_level_init_verified':False,'gameplay_ready':False,'GPU_verified':False,'emulator_launched':False,'shared_writes':False,'fixtures':'Offline/seed7/menu/log/per-call cheat transport; source selected7/10 entry; host image admission and compatibility URI; deleting destructor/suffix native test observers. No continuous loading/rendering/gameplay proof.'}
(b/'reports/coherent-loader-v53-integrated-verified.json').write_text(json.dumps(receipt,indent=2)+'\n')
print(json.dumps({'validation':receipt['validation'],'coherent_files':len(m['files']),'checks':len(cases),'failures':failures,'whole_init':False}));sys.exit(bool(failures))
