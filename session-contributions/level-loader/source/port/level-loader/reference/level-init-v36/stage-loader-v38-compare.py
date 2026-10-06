from pathlib import Path
import json,subprocess,hashlib
root=Path(r'C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader');old=json.loads((root/'reports/object-initialization-original.json').read_text());assert old['validation']=='PASS';assert old['engine_sha256']=='36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80';assert old['script_sha256']==hashlib.sha256((root/'tests/object_initialization_original.py').read_bytes()).hexdigest()
lines=[str(len(old['cases']))]
for row in old['cases']:
 f=row['fixture'];objects=f['objects'];ids={o['label']:i+1 for i,o in enumerate(objects)};lines.append(f"{f['name']} {len(objects)} {len(f['modules'])} {int(f.get('preseed',False))}")
 for i,o in enumerate(objects):
  fields=[o['label'],o['type'],o.get('key',i),int(o.get('updating',False)),int(o.get('deleted',False)),int(o.get('a8',0)),int(o.get('ac',0)),int(o.get('cc',0)),int(o.get('d0',0)),ids[o['append']] if o.get('append') else 0,o.get('init_a8',-1),o.get('room_cc',-1),ids[o['insert']] if o.get('insert') else 0,int(o.get('deferred',False))];lines.append(' '.join(map(str,fields)))
 lines.append(' '.join(str(ids[name]) for name in f['modules']))
text='\n'.join(lines)+'\n';cmd=['wsl.exe','-d','Ubuntu','--','/mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-loader-v38/dh2_stage_v38_trace'];r=subprocess.run(cmd,input=text,text=True,capture_output=True,timeout=30);assert r.returncode==0,(r.stdout,r.stderr);new=json.loads(r.stdout);assert len(new['cases'])==len(old['cases'])
for reference,actual in zip(old['cases'],new['cases']):
 assert actual['name']==reference['fixture']['name']
 for key in ('calls','events','room_list','transient_lists','module_list'):assert reference[key]==actual[key],(actual['name'],key,reference[key],actual[key])
manager_cmd=['wsl.exe','-d','Ubuntu','--','/mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-loader-v38/dh2_stage_v38_manager'];manager=subprocess.run(manager_cmd,text=True,capture_output=True,timeout=30);assert manager.returncode==0,(manager.stdout,manager.stderr)
report={'validation':'PASS','original_trace_sha256':hashlib.sha256((root/'reports/object-initialization-original.json').read_bytes()).hexdigest(),'original_trace_fixtures':len(old['cases']),'original_calls':sum(len(r['calls']) for r in old['cases']),'original_events':sum(len(r['events']) for r in old['cases']),'matched':['calls','events','room_list','transient_lists','module_list'],'actual_manager_stdout':manager.stdout.strip(),'trace_command':cmd,'manager_command':manager_cmd,'sanitizers':['ASAN','UBSAN'],'scope':'New typed scheduler borrows one primary manager-shape fixture map/phase/module list; class/handle/module/condition/list effects are explicit fixture observers, compared exactly against actual original ARM traces. Separate actual CanonicalObjectManagerV1 prototype verifies true map1c including null entries, signed ordered iteration and same7c field; real class behavior not executed.','whole_level_init_verified':False}
(root/'reports/stage-loader-v38-init-post-verified.json').write_bytes((json.dumps(report,indent=2)+'\n').encode());stage7=json.loads((root/'reference/level-init-v36/stage-loader-v38-stage7-original.json').read_text());root_cmd=['wsl.exe','-d','Ubuntu','--','/mnt/c/Users/adamc/.codex/worktrees/generic-level-loader/build/stage-loader-v38/dh2_stage_v38_root'];root_run=subprocess.run(root_cmd,text=True,capture_output=True,timeout=30);assert root_run.returncode==0,(root_run.stdout,root_run.stderr)
native_rows=[json.loads(line) for line in root_run.stdout.splitlines() if line.startswith('{')];assert len(native_rows)==len(stage7['cases'])==40
event_map={'construct':'stream_construct','assign':'assign_stream','destroy':'stream_destroy','root':'load_file'}
for reference,actual in zip(stage7['cases'],native_rows):
 for key in ('filename','procedural','online','generated_fixture','final_name'):assert reference[key]==actual[key],(key,reference,actual)
 assert [e['kind'] for e in reference['events']]==[event_map.get(e,e) for e in actual['event_names']]
 assert (actual['state130'],actual['file13c'],actual['current138'])==(8,6,500)
report['original_stage7_cases']=40;report['original_stage7_receipt_sha256']=hashlib.sha256((root/'reference/level-init-v36/stage-loader-v38-stage7-original.json').read_bytes()).hexdigest();report['native_stage7_stdout_sha256']=hashlib.sha256(root_run.stdout.encode()).hexdigest();report['native_stage7_matches_original_filename_and_event_order']=True
(root/'reports/stage-loader-v38-init-post-verified.json').write_bytes((json.dumps(report,indent=2)+'\n').encode());print(json.dumps({k:v for k,v in report.items() if k!='scope'}))
