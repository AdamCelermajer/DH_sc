from pathlib import Path
import json,subprocess
ROOT=Path(__file__).resolve().parents[3]
OUT=ROOT/'port/level-loader/reports/root-loading-v50'
CPP=ROOT/'port/android-native/app/src/main/cpp'
OUT.mkdir(exist_ok=True)
probe=OUT/'root-candidate-syntax.cpp'
live=(CPP/'model_renderer.cpp').read_text()
candidate_include='' if '#include "renderer_source_candidate_v55.inc"' in live else '#include "renderer_source_candidate_v55.inc"\n'
probe.write_text('#include "front_selected_profile_v50.hpp"\n'+live+'\nnamespace model_renderer {\n'+candidate_include+
 'void root_candidate_compile_only_v55(){std::string error;auto world=create_source_world_v55(nullptr,{},"",error);'
 'RendererSourceCandidateInputsV55 in;std::shared_ptr<RendererSourceCandidateV55> out;'
 'RendererSourceCandidateV55::create(std::move(in),out,error);}\n}\n')
rows=[]
for abi in ['x86_64','arm64-v8a']:
 commands=json.loads((ROOT/'port/android-native/app/.cxx/Debug/5a1n3w3m'/abi/'compile_commands.json').read_text())
 entry=next(c for c in commands if Path(c['file']).name=='model_renderer.cpp')
 # CMake supplies a Windows command line; append overrides while preserving
 # actual ABI/include/compile definitions. Existing -c/-o emit only report obj.
 command=entry['command'].replace(str(CPP/'model_renderer.cpp').replace('\\','/'),str(probe).replace('\\','/'))
 command=command.replace(str(CPP/'model_renderer.cpp'),str(probe))
 # Remove the old object destination and compile syntax only.
 import re
 command=re.sub(r' -o (?:"[^"]+"|\S+)','',command)
 command=command.replace(' -c ',' ')
 command+=' -fsyntax-only -I"'+str(CPP)+'"'
 result=subprocess.run(command,cwd=entry['directory'],text=True,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,timeout=90)
 rows.append({'abi':abi,'exit_code':result.returncode,'command':command,'output':result.stdout})
 (OUT/('root-candidate-'+abi+'.log')).write_text(result.stdout)
 print(abi,result.returncode,result.stdout[:4500],flush=True)
 if result.returncode:break
(OUT/'root-candidate-syntax-proof.json').write_text(json.dumps(rows,indent=2))
raise SystemExit(0 if len(rows)==2 and all(r['exit_code']==0 for r in rows) else 1)
