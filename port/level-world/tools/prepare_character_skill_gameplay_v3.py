"""Create separate V3 authority copies; frozen V2 files are never modified.

Reversible mechanical rename receipts are recorded before bounded V3 additions.
This tool refuses to overwrite a previously edited V3 destination.
"""
import hashlib,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3];WORLD=ROOT/'port/level-world'
def sha(b):return hashlib.sha256(b).hexdigest()
def main():
 changes={'CharacterScriptSessionV2':'CharacterScriptSessionV3','CharacterScriptSessionInputV2':'CharacterScriptSessionInputV3','CharacterSkillSessionServicesV2':'CharacterSkillSessionServicesV3','SkillInfoSessionsV2':'SkillInfoSessionsV3','skill_info_session_invoke_v2':'skill_info_session_invoke_v3','CharacterPlayerSkillsV2':'CharacterPlayerSkillsV3','PlayerSkillInitServicesV2':'PlayerSkillInitServicesV3'};records=[]
 for name in ['character_script_session','character_skills_session','character_skill_info_session','character_player_skills']:
  for suffix in ['.hpp','.cpp']:
   source=WORLD/(name+'_v2'+suffix);data=source.read_text();out=data
   for old,new in changes.items():out=re.sub(r'\b'+old+r'\b',new,out)
   for stem in ['character_script_session','character_skills_session','character_skill_info_session','character_player_skills']:out=out.replace(stem+'_v2.hpp',stem+'_v3.hpp')
   target=WORLD/(name+'_v3'+suffix)
   if target.exists():raise RuntimeError('Refusing overwrite '+str(target))
   target.write_text(out);records.append(dict(source=source.relative_to(ROOT).as_posix(),source_sha256=sha(source.read_bytes()),target=target.relative_to(ROOT).as_posix(),mechanical_versioned_sha256=sha(target.read_bytes()),renames=changes))
 source=WORLD/'character_skills.hpp';data=source.read_text();start=data.index('class CharacterSkillOwner {');end=data.index('\n};',start)+4;out='#pragma once\n#include "character_skills.hpp"\nnamespace dh2::character::skills {\n'+data[start:end].replace('CharacterSkillOwner','CharacterSkillOwnerV3')+'\n}\n';target=WORLD/'character_skills_owner_v3.hpp';assert not target.exists();target.write_text(out);records.append(dict(source=source.relative_to(ROOT).as_posix(),source_sha256=sha(source.read_bytes()),target=target.relative_to(ROOT).as_posix(),mechanical_versioned_sha256=sha(target.read_bytes()),selection='Only authoritative owner class declaration; source ABI types reused.'))
 source=WORLD/'character_skills_owner.cpp';out=source.read_text().replace('"character_skills.hpp"','"character_skills_owner_v3.hpp"').replace('CharacterSkillOwner','CharacterSkillOwnerV3');target=WORLD/'character_skills_owner_v3.cpp';assert not target.exists();target.write_text(out);records.append(dict(source=source.relative_to(ROOT).as_posix(),source_sha256=sha(source.read_bytes()),target=target.relative_to(ROOT).as_posix(),mechanical_versioned_sha256=sha(target.read_bytes()),renames={'CharacterSkillOwner':'CharacterSkillOwnerV3'}))
 ref=WORLD/'reference/character-skill-gameplay-v3';(ref/'versioning-map.json').write_text(json.dumps(dict(validation='PASS',files=records,scope=__doc__),indent=2)+'\n');print(json.dumps(dict(validation='PASS',versioned_files=len(records))))
if __name__=='__main__':main()
