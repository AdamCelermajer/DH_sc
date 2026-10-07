"""Version frozen HitFor core and append recovered distinct-player tail."""
from pathlib import Path
import hashlib,json
root=Path(__file__).resolve().parents[3];source=root/'port/level-world/character_hit.cpp';target=root/'port/level-world/character_hit_v6.cpp'
text=source.read_text().replace('#include "character_hit.hpp"','#include "character_skill_combat_v6.hpp"',1)
text=text.replace('extern "C" int dh2_character_hit_for(','extern "C" int dh2::character::skills::dh2_character_hit_for_v6(',1)
text=text.replace('const char* name=nullptr){\n  result.phase=op+1;', 'const char* name=nullptr,std::uint32_t force=0){\n  result.phase=op+1;',1)
text=text.replace('HitRequest32 q{op,0,subject,','HitRequest32 q{op,force,subject,',1)
old=' if(!resolved)return run.end(out,1);\n if(!run.call(hit_is_player,value,resolved))return run.end(out,-2);\n return run.end(out,value&&resolved!=actor->identity?-3:1);'
assert text.count(old)==1
tail=''' using namespace dh2::character::skills;
 // Source captures the global trophy manager immediately after handle cast,
 // even when resolution is null; keep it across every later query/unlock.
 std::uintptr_t manager=0;
 if(!run.call(static_cast<HitService>(hit_trophy_manager_v6),manager,0))return run.end(out,-2);
 if(!resolved)return run.end(out,1);
 if(!run.call(hit_is_player,value,resolved))return run.end(out,-2);
 if(!value||resolved==actor->identity)return run.end(out,1);
 if(!run.call(static_cast<HitService>(hit_local_player_v6),value,resolved))return run.end(out,-2);
 if(!value)return run.end(out,1);
 auto unlock=[&](const char* name){
  std::uintptr_t index=0,ignored=0;
  return run.call(static_cast<HitService>(hit_trophy_index_v6),index,0,name)&&
   run.call(static_cast<HitService>(hit_unlock_v6),ignored,manager,nullptr,std::uint32_t(index));
 };
 if(r.credited_damage>199&&!unlock("power_200dam"))return run.end(out,-2);
 if(r.credited_damage>149&&!unlock("power_150dam"))return run.end(out,-2);
 if(r.credited_damage>99&&!unlock("power_100dam"))return run.end(out,-2);
 if(r.credited_damage>49&&!unlock("power_50dam"))return run.end(out,-2);
 if(r.before_hp!=r.maximum_hp||!r.kill_called)return run.end(out,1);
 if(!run.call(static_cast<HitService>(hit_major_enemy_v6),value,actor->identity))return run.end(out,-2);
 if(!value){if(!run.call(static_cast<HitService>(hit_minor_enemy_v6),value,actor->identity))return run.end(out,-2);}
 if(value&&!unlock("power_onehit"))return run.end(out,-2);
 return run.end(out,1);'''
text=text.replace(old,tail,1)
if target.exists():assert target.read_text()==text,'Inspect modified version before regeneration'
else:target.write_text(text)
ref=root/'port/level-world/reference/character-skill-combat-v6'
(ref/'hit-versioning-map-v6.json').write_text(json.dumps({'source':str(source.relative_to(root)),'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'target':str(target.relative_to(root)),'target_sha256':hashlib.sha256(target.read_bytes()).hexdigest(),'changes':['versioned export','source global trophy capture after cast','distinct-player local query','descending damage achievements','one-hit major/minor query and achievement'],'unsupported':['online receiver policy','player receiver low-health/tutorial/achievements']},indent=2)+'\n')
print('Prepared V6 HitFor player-attacker continuation')
