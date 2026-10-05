"""Retain genuine target policies with direct live resolved-array borrowing."""
from pathlib import Path
import hashlib,json
root=Path(__file__).resolve().parents[3];base=root/'port/level-world'
old=base/'character_target_providers.cpp';text=old.read_text();text=text[:text.index('extern "C" int dh2_character_sneak_fields')]
text=text.replace('#include "character_target_providers.hpp"','#include "character_skill_target_queries_v6.hpp"\nusing dh2::character::skills::SkillTargetCharacterV6;')
text=text.replace('Character32','SkillTargetCharacterV6').replace('c->properties->words','c->resolved')
text=text.replace('p->properties','p->resolved').replace('c->properties','c->resolved').replace('other->properties','other->resolved')
text=text.replace('sizeof(*c->resolved)','896').replace('sizeof(*other->resolved)','896')
text=text.replace('namespace dh2::target_providers {namespace {','namespace dh2::character::skills {using namespace dh2::target_providers;namespace {')
text=text.replace('extern "C" int dh2_character_target_query','extern "C" int dh2_character_skill_target_query_v6')+'}\n'
path=base/'character_skill_target_queries_v6.cpp'
if path.exists():assert path.read_text()==text,'Inspect changed target policy before regeneration'
else:path.write_text(text)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
(base/'reference/character-skill-combat-v6/target-query-versioning-map-v6.json').write_text(json.dumps({'source_sha256':sha(old),'new_sha256':sha(path),'change':'Only native cached sheet pointer type; reads actual resolved int32 array, no property copying or struct type-punning','entry':'dh2_character_skill_target_query_v6'},indent=2)+'\n')
print('Prepared direct live-array V6 target policy kernel')
