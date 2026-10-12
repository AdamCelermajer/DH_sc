from pathlib import Path
import json, shlex, subprocess
root=Path(__file__).resolve().parents[1]
out=root/'port/level-loader/reports/native-gslevel-v27';out.mkdir(parents=True,exist_ok=True)
sources=['port/level-loader/native_gslevel_runtime_v27.cpp','port/engine-ui/authored_shared_menu_roster_v27.cpp','port/engine-ui/menu_stack_owner_v1.cpp','port/engine-ui/authored_character_panel_v2.cpp','port/android-native/app/src/main/cpp/native_app.cpp','port/android-native/app/src/main/cpp/model_renderer.cpp']
results=[]
for abi in ['arm64-v8a','x86_64']:
 databases=list((root/'port/android-native/app/.cxx/Debug').glob('*/'+abi+'/compile_commands.json'));assert len(databases)==1
 rows=json.loads(databases[0].read_text());row=next(r for r in rows if Path(r['file']).name=='model_renderer.cpp')
 for source in sources:
  selected=next((r for r in rows if Path(r['file']).name==Path(source).name),row)
  args=selected.get('arguments') or [s.strip('"') for s in shlex.split(selected['command'],posix=False)]
  flags=[]
  for i,s in enumerate(args):
   if s in ['-isystem','-I']:flags.extend([s,args[i+1]])
   elif s.startswith(('--target=','--sysroot=','-I','-isystem','-D')):flags.append(s)
  obj=out/(Path(source).stem+'-'+abi+'.o')
  cmd=[args[0],*flags,'-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-c',str(root/source),'-o',str(obj)]
  result=subprocess.run(cmd,capture_output=True,text=True,timeout=180)
  (out/(obj.stem+'.log')).write_text(result.stdout+result.stderr)
  results.append({'abi':abi,'source':source,'exit_code':result.returncode,'command':cmd})
  print(abi,source,result.returncode,result.stderr[:4000],flush=True)
(out/'compile.json').write_text(json.dumps({'scope':'Strict coherent changed GS/shared-menu consumers; no app build/install/runtime acceptance','status':'PASS' if all(r['exit_code']==0 for r in results) else 'FAIL','results':results},indent=2)+'\n')
raise SystemExit(0 if all(r['exit_code']==0 for r in results) else 1)
