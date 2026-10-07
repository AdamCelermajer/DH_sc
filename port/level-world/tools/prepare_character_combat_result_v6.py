"""Create isolated ordered continuation from unchanged genuine result kernel."""
from pathlib import Path
import hashlib,json
root=Path(__file__).resolve().parents[3]
source=root/'port/game-data/combat_result.cpp'
target=root/'port/level-world/character_combat_result_v6.cpp'
text=source.read_text()
text=text.replace('#include "combat_result.hpp"','#include "../game-data/combat_result.hpp"\n#include "character_skill_combat_v6.hpp"',1)
text=text.replace('extern "C" unsigned dh2_combat_result(dh2::data::CombatResult* out,const dh2::data::CombatResultRequest* request){','extern "C" unsigned dh2::character::skills::dh2_combat_result_ordered_v6(dh2::data::CombatResult* out,const dh2::data::CombatResultRequest* request,void* debug_context,int(*debug)(void*)){',1)
needle=' if(mask&0xe0000){Damage damage;'
assert text.count(needle)==1
text=text.replace(needle,' *out=result;*request->random=random;\n if(!debug||debug(debug_context))return 2;\n'+needle,1)
start=text.index('extern "C" unsigned dh2_combat_melee(')
text=text[:start]
if target.exists():assert target.read_text()==text,'Versioned kernel changed; inspect before regeneration'
else:target.write_text(text)
ref=root/'port/level-world/reference/character-skill-combat-v6'
(ref/'result-versioning-map-v6.json').write_text(json.dumps({'source':str(source.relative_to(root)),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'target':str(target.relative_to(root)),'target_sha256':hashlib.sha256(target.read_bytes()).hexdigest(),'changes':['versioned export','Debug continuation after status before damage','reached failure commits result/RNG prefix','omit unrelated melee export']},indent=2)+'\n')
print('Prepared ordered V6 result kernel')
