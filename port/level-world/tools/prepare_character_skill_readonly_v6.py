"""Compile frozen V5 read-only operations with a const inventory borrow."""
from pathlib import Path
import hashlib,json
root=Path(__file__).resolve().parents[3];base=root/'port/level-world'
old=base/'character_skill_native_v5.hpp';text=old.read_text();start=text.index('class CharacterSkillNativeBindingsV5 final');end=text.index('\n};',start)+3
declaration=text[start:end].replace('CharacterSkillNativeBindingsV5','CharacterSkillNativeReadOnlyBindingsV6').replace('data::FreshInventoryOwnedV4&','const data::FreshInventoryOwnedV4&')
header='#pragma once\n#include "character_skill_native_v5.hpp"\nnamespace dh2::character::skills {\n'+declaration+'\n}\n'
cpp='''#include "character_skill_readonly_v6.hpp"
#include "character_stance.hpp"
#include "character_target_search_v5.hpp"
// Headers are consumed before implementation token remapping. Frozen V5
// operations only query this exact inventory; the const successor adds no
// cloned inventory, alternate Session or new property state.
namespace dh2::data {using SkillReadOnlyInventoryV6=const FreshInventoryOwnedV4;}
#define CharacterSkillNativeBindingsV5 CharacterSkillNativeReadOnlyBindingsV6
#define FreshInventoryOwnedV4 SkillReadOnlyInventoryV6
#include "character_skill_native_v5.cpp"
#undef FreshInventoryOwnedV4
#undef CharacterSkillNativeBindingsV5
'''
outputs={'character_skill_readonly_v6.hpp':header,'character_skill_readonly_v6.cpp':cpp}
for name,payload in outputs.items():
 path=base/name
 if path.exists():assert path.read_text()==payload,name+' changed; inspect before regeneration'
 else:path.write_text(payload)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
(base/'reference/character-skill-combat-v6/readonly-versioning-map-v6.json').write_text(json.dumps({'source_sha256':{str(p.relative_to(root)):sha(p)for p in [old,base/'character_skill_native_v5.cpp']},'new_sha256':{name:sha(base/name)for name in outputs},'construction':'same frozen V5 implementation text under class-name and const inventory type remap; one targetlist/mana/Session graph over the same actual inventory','mutability':'all inventory calls compile through const FreshInventoryOwnedV4&, no const_cast'},indent=2)+'\n')
print('Prepared const inventory-borrow V6 successor')
