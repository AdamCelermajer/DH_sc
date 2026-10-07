"""Freeze completed owned player skill Session production/evidence, no shared edits."""
import hashlib,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];WORLD=ROOT/'port/level-world';REF=WORLD/'reference/character-skill-session-v2'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
def main():
 names=['character_script_owner_v2','character_script_session_v2','character_skills_session_v2','character_skill_info_session_v2','character_script_player_vcb_v2','character_current_skill_v2','character_player_skills_v2']
 sources=[WORLD/(n+s)for n in names for s in('.hpp','.cpp')];tests=[WORLD/'tests'/n for n in ['character_skill_session_v2.cpp','character_player_skills_v2.cpp','character_skill_session_v2_differential.py','character_skill_session_v2_host.py']];tools=[WORLD/'tools'/n for n in ['prepare_character_skill_session_v2.py','extract_character_skill_session_v2.py','build_character_skill_session_v2_oracle.ps1','check_character_skill_session_v2_android.py','freeze_character_skill_session_v2.py']]
 reports=[WORLD/'reports'/('character-skill-session-v2-'+n+'.json')for n in ['arm64-differential','host-audit','android-readiness']];data=[json.loads(p.read_text())for p in reports];assert all(x['validation']=='PASS'for x in data)
 host=data[1];diff=data[0]
 for p in sources+tests[:2]+tests[3:]:
  key=p.relative_to(ROOT).as_posix();assert host['source_and_inputs_sha256'][key]==sha(p),(key,'Host source receipt mismatch')
 for key,value in diff['source_sha256'].items():assert sha(ROOT/key)==value,(key,'Differential source mismatch')
 for row in json.loads((REF/'versioning-map.json').read_text())['files']:
  assert sha(ROOT/row['path'])==row['versioned_sha256'];assert sha(ROOT/row['frozen_path'])==row['frozen_sha256']
 assert diff['mismatches']==0 and host['sanitizers']['findings']==0
 evidence=[p for p in REF.rglob('*')if p.is_file()and p.name!='freeze-manifest.json']
 all_files=sorted(set(sources+tests+tools+reports+evidence));manifest=dict(validation='PASS',source_and_evidence_sha256={p.relative_to(ROOT).as_posix():sha(p)for p in all_files},production_sources=[p.relative_to(ROOT).as_posix()for p in sources if p.suffix=='.cpp'],reports={p.relative_to(ROOT).as_posix():sha(p)for p in reports},original_arm64=dict(player_vcb=256,current_skill=768,mismatches=0),host_results=host['results'],scope='One authoritative retained player skill Session/owner/VM/timer/property publication and source skill Info. Exact source, native checks and required-provider boundaries in NOTES.md; no whole frame/campaign/gameplay/package claim.')
 p=REF/'freeze-manifest.json';p.write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps(dict(validation='PASS',files=len(all_files),path=p.relative_to(ROOT).as_posix(),sha256=sha(p))))
if __name__=='__main__':main()
