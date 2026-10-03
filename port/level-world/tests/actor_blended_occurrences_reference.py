"""Focused original PlayClip engine-index comparison with resolved map services.

Reuses the audited original selector fixture prefix without running its writer.
Actual Blend/SetCurrentAnimation/setCurrentAnimation/SetClip/jump instructions
execute. Resource resolution supplies the registered engine index; registration
map correctness has separate actual LoadAnimation/map/refresh instruction gold.
"""
import argparse,hashlib,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];REPO=ROOT.parents[1]
def sha(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,default=ROOT/'reference/actor-blended-occurrences/playclip-fixtures.bin');p.add_argument('--report',type=Path,default=ROOT/'reference/actor-blended-occurrences/original-playclip.json');a=p.parse_args()
 dependency=ROOT/'reference/animation-blend-composition-reentry/probe_selection.py'
 prefix=dependency.read_text().split('records=[]',1)[0];namespace={'__file__':str(dependency)};exec(compile(prefix,str(dependency),'exec'),namespace)
 c=namespace['c'];records=[];gold=b'BPO1';cases=[]
 for previous in (0,2,7):
  for dictionary,index in ((1040,2),(1041,7),(20001,2)):
   for ended in (0,1):
    for extra in (0,17,-17,2147483647):
     namespace['configure'](previous,ended);namespace['replay_check']=False
     c.pointer(namespace['resource']+0x20,index);c.pointer(namespace['agg']+0x10,extra & 0xffffffff)
     assert c.invoke(0x47680c,[namespace['controller'],dictionary,1,0,0])==1
     result=namespace['snapshot'](namespace['times'][1]);offset=(result['current_ms']-100)&0xffffffff
     assert offset==(extra&0xffffffff if previous==index else 0)
     assert result['initialized']==result['ended']==0 and result['loop']==1
     assert namespace['w'](namespace['slots'][1]+0x50)==index
     cases.append((previous,index,dictionary,ended,extra,offset));records.append({'previous_engine':previous,'mapped_engine':index,'dictionary_id':dictionary,'initial_ended':ended,'extra_ms':extra,'result':result,'calls':list(namespace['calls'])})
 gold+=struct.pack('<I',len(cases))+b''.join(struct.pack('<6I',*[v&0xffffffff for v in row]) for row in cases)
 a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_bytes(gold)
 report={'validation':'PASS','original_sha256':namespace['manifest']['original_sha256'],'script_sha256':sha(Path(__file__)),'fixture_dependency_sha256':sha(dependency),'manifest_sha256':sha(namespace['HERE']/'original-functions.json'),'reference_sha256':sha(a.output),'original_instruction_cases':len(cases),'records':records,'service_boundaries':['resolved AnimationSet map entry with caller-supplied engine index','timeline identity/bound virtuals start100/end900','event callback installation','root NewAnim observer'],'scope':__doc__}
 a.report.write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({'validation':'PASS','original_instruction_cases':len(cases),'reference_sha256':sha(a.output)}))
if __name__=='__main__':main()
