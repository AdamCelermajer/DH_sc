"""Friendly player selection/count against original ARM and selected native code."""
from __future__ import annotations
import argparse,hashlib,json,os,subprocess
from pathlib import Path
from player_manager_friendly_v1_original import capture
ROOT=Path(__file__).resolve().parents[3];MODULE=ROOT/'port/player-info-level'
def main():
 parser=argparse.ArgumentParser();parser.add_argument('--original',required=True,type=Path);parser.add_argument('--compiler',required=True);parser.add_argument('--output',required=True,type=Path);parser.add_argument('--library',type=Path);a=parser.parse_args()
 a.output.mkdir(parents=True,exist_ok=True);rows,original,summary=capture(a.original)
 inputs=a.output/'inputs.txt';inputs.write_text('\n'.join(' '.join(map(str,row)) for row in rows)+'\n',encoding='utf-8')
 sources=['player_manager_friendly_v1.cpp','player_manager_host_level.cpp','player_local_selection_v1.cpp','player_locality_v1.cpp','character_level_member.cpp']
 test=MODULE/'tests/player_manager_friendly_v1_host.cpp';exe=a.output/'player_manager_friendly_v1_host.exe'
 command=[a.compiler,'-std=c++17','-Wall','-Wextra','-Werror','-I',str(MODULE),str(test)]
 command += [str(a.library)] if a.library else [str(MODULE/x) for x in sources]
 subprocess.run(command+['-o',str(exe)],check=True);env=os.environ.copy();dlls=[]
 if a.library:
  dlls=sorted(a.library.parent.parent.rglob('*.dll'));env['PATH']=os.pathsep.join([*(str(x.parent) for x in dlls),env['PATH']])
 host=json.loads(subprocess.check_output([str(exe),str(inputs)],env=env,text=True));comparisons=host.pop('results')
 mismatches=[dict(index=i,row=rows[i],original=old,compiled=new) for i,(old,new) in enumerate(zip(original,comparisons)) if old!=new]
 (a.output/'comparison.json').write_text(json.dumps(dict(host=host,original=original,compiled=comparisons,mismatches=mismatches),indent=2)+'\n',encoding='utf-8')
 assert len(original)==len(comparisons) and not mismatches,('friendly selection mismatch',mismatches[:3])
 evidence=[*sources,'player_manager_friendly_v1.hpp','tests/player_manager_friendly_v1_host.cpp','tests/player_manager_friendly_v1_original.py','tests/run_player_manager_friendly_v1.py','tests/player_local_selection_v1.cpp','tests/player_local_selection_v1_original.py','tests/player_locality_v1_original.py','reference/player-manager-friendly-v1/original-functions.json']
 report=dict(validation='PASS',host=host,original=summary,comparisons=len(comparisons),mismatches=0,implementation='selected library' if a.library else 'direct translation units',source_sha256={x:hashlib.sha256((MODULE/x).read_bytes()).hexdigest() for x in evidence})
 if a.library:
  commands=json.loads((a.library.parent.parent/'compile_commands.json').read_text())
  native_sources={(MODULE/x).resolve() for x in sources}
  report['selected_commands']=[c for c in commands if Path(c['file']).resolve() in native_sources]
  assert len(report['selected_commands'])==len(sources)
  report['binary_sha256']={x.name:hashlib.sha256(x.read_bytes()).hexdigest() for x in [exe,*dlls]}
 (a.output/'report.json').write_text(json.dumps(report,indent=2)+'\n',encoding='utf-8');print(json.dumps({k:v for k,v in report.items() if k not in ['source_sha256','selected_commands','binary_sha256']}))
if __name__=='__main__':main()
