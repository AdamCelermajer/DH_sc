from pathlib import Path
import subprocess,json,shlex,os
root=Path(__file__).resolve().parents[3];out=root/'.local-inputs/authored-fx-v32';out.mkdir(exist_ok=True)
os.environ['TEMP']=str(out);os.environ['TMP']=str(out)
files=['port/level-world/character_authored_resource_v32.cpp','port/level-world/authored_fx_mesh_graph_v32.cpp','port/level-world/authored_fx_nonrender_geometry_v32.cpp','port/engine-animation/particle_resource_init_v32.cpp','port/engine-animation/particle_billboard_v32.cpp']
results=[]
for abi in ['arm64-v8a','x86_64']:
 db=json.loads((root/f'port/android-native/app/.cxx/Debug/5a1n3w3m/{abi}/compile_commands.json').read_text())
 entry=next(e for e in db if e['file'].replace('\\','/').endswith('/model_renderer.cpp'));args=entry.get('arguments') or [s.strip('"') for s in shlex.split(entry['command'],posix=False)]
 flags=[s for s in args[1:] if s.startswith(('--target=','--sysroot=','-I','-isystem'))]
 for i,s in enumerate(args):
  if s=='-isystem':flags.append(args[i+1])
 for f in files:
  p=subprocess.run([args[0],*flags,'-std=c++17','-Wall','-Wextra','-Werror','-Wno-misleading-indentation','-c',str(root/f),'-o',str(out/(Path(f).stem+'-'+abi+'.o'))],capture_output=True,text=True)
  (out/(Path(f).stem+'-'+abi+'.log')).write_text(p.stdout+p.stderr);print(abi,f,p.returncode,p.stderr);assert p.returncode==0
  results.append(dict(abi=abi,file=f,exit_code=p.returncode))
 if abi=='arm64-v8a':
  oracle=['port/level-world/tests/authored_fx_source_v32_oracle.cpp','port/engine-animation/particle_billboard_v32.cpp','port/engine-animation/particle_cloud_models_v1.cpp','port/engine-animation/particle_random_v1.cpp','port/engine-math/math.cpp']
  p=subprocess.run([args[0],*flags,'-std=c++17','-O2','-fno-fast-math','-ffp-contract=off','-fPIC','-shared',*[str(root/f)for f in oracle],'-Wl,-Bsymbolic','-o',str(out/'source-oracle-arm64.so')],capture_output=True,text=True)
  print(p.stderr);assert p.returncode==0
(root/'port/level-world/reports/authored-fx-v32-strict-compile.json').write_text(json.dumps(results,indent=2)+'\n')
