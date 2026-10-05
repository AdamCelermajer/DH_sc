"""Declare V6 compatibility successor; compile the unchanged V3 source text."""
from pathlib import Path
import hashlib,json
root=Path(__file__).resolve().parents[3];base=root/'port/level-world'
old=base/'character_player_skills_v3.hpp';text=old.read_text()
start=text.index('class CharacterPlayerSkillsV3 {');end=text.index('\n};',start)+3
declaration=text[start:end].replace('CharacterPlayerSkillsV3','CharacterPlayerSkillsV6')
needle=' const std::string& error()const noexcept;'
assert needle in declaration
declaration=declaration.replace(needle,needle+'''
 // Borrow only through this retained player lifetime; never close/destroy it
 // or reenter buff mutation during a Buff service callback. Native callers
 // use the same property groups and TimerStore as the VM's Lua Buff bindings.
 BuffOwner* native_buffs()noexcept;
 // Preferred narrow same-owner native PROPS_DelBuff operation. 1 delivered,
 // -1 malformed/reentrant, -2 reached required provider or unavailable player.
 int native_delete_buff(BuffResult24*,std::int32_t id,std::uintptr_t instance=0);
 // Source Character::CancelSneaking on this retained owner. source_byte415
 // borrows the actual Character field; required AI_EndSkill is same graph.
 int native_cancel_sneaking(std::uint8_t* source_byte415);
 const data::PlayerSavegameV1* native_savegame()const noexcept;
 int native_reload_skills(const SkillSaveReloadServicesV6*);
''',1)
header='#pragma once\n#include "character_player_skills_v3.hpp"\n#include "character_skill_save_reload_v6.hpp"\nnamespace dh2::character::skills {\n'+declaration+'\n}\n'
cpp='''#include "character_player_skills_v6.hpp"
#include "character_skills_owner_v6.hpp"
#include "character_skill_reload_v6.hpp"
// Consume all V3 declarations before remapping implementation class tokens.
// Both versions compile exactly the same frozen implementation source text;
// any composition instantiates one player graph, never both for one Character.
#define CharacterPlayerSkillsV3 CharacterPlayerSkillsV6
#define CharacterSkillOwnerV3 CharacterSkillOwnerV6
#include "character_player_skills_v3.cpp"
#undef CharacterSkillOwnerV3
#undef CharacterPlayerSkillsV3
namespace dh2::character::skills {
BuffOwner* CharacterPlayerSkillsV6::native_buffs()noexcept{return impl_->buffs;}
const data::PlayerSavegameV1* CharacterPlayerSkillsV6::native_savegame()const noexcept{return impl_->saved.get();}
int CharacterPlayerSkillsV6::native_reload_skills(const SkillSaveReloadServicesV6* services){
 if(!ready())return -2;
 struct Reload {
  Impl& t;const SkillSaveReloadServicesV6* save_services;
  static int invoke(void* p,State40* state,const SkillReloadRequestV6* q){
   auto& r=*static_cast<Reload*>(p);auto& t=r.t;
   if(state!=&t.owner->state()||q->character!=t.session->timers().owner)return -1;
   if(q->service==skill_reload_delete_v6)return t.owner->native_delete_skill(q->index,q->instance)==1?0:-1;
   if(q->service==skill_reload_reset_end_v6)return t.owner->native_reset_skill_end()==1?0:-1;
   if(q->service==skill_reload_save_v6){
    if(!t.saved)return 0; // Original Character.SG_ReloadSkills null-Save guard.
    SkillSaveReloadOutputV6 reloaded{};
    if(dh2_character_skill_save_reload_v6(&reloaded,t.saved.get(),r.save_services)){
     t.error="Required SAME Save SG_ReloadSkills failed at phase "+std::to_string(reloaded.phase);return -1;}return 0;
   }
   const auto status=q->service==skill_reload_configure_v6?t.owner->configure():q->service==skill_reload_update_v6?t.owner->update():-1;
   if(status!=1){t.error=t.owner->error()+t.service->error();return -1;}return 0;
  }
 }reload{*impl_,services};
 SkillReloadServicesV6 providers{&reload,Reload::invoke};SkillReloadOutputV6 result{};
 const auto code=dh2_character_skill_reload_v6(&result,const_cast<State40*>(&impl_->owner->state()),&providers);
 if(code!=1){if(impl_->error.empty())impl_->error="Required native AI_ReloadSkills failed at phase "+std::to_string(result.phase);return -2;}
 impl_->error.clear();return 1;
}
int CharacterPlayerSkillsV6::native_delete_buff(BuffResult24* out,std::int32_t id,std::uintptr_t instance){
 if(!out)return -1;if(!ready())return -2;
 return dh2_character_buff_delete(out,impl_->buffs,id,instance);
}
int CharacterPlayerSkillsV6::native_cancel_sneaking(std::uint8_t* source_byte415){
 if(!source_byte415)return -1;if(!ready())return -2;
 std::uint32_t player=0;if(session().source_is_player(player))return -2;
 if(player){BuffResult24 result{};if(native_delete_buff(&result,146)!=1)return -2;*source_byte415=1;}
 if(session().property_view().resolved[198]<=0)return 0;
 std::uint32_t count=0;if(impl_->owner->list_count(0,&count))return -2;
 for(std::uint32_t i=0;i<count;++i){
  const auto* row=impl_->owner->skill(i);if(!row)return -2;
  if(row->scalar.words[7]&0x2000000u){std::uint32_t result=0;return skill_ai(skill_ai_end_v3,i,&result);}
 }
 return 0;
}
}
'''
outputs={'character_player_skills_v6.hpp':header,'character_player_skills_v6.cpp':cpp}
for name,payload in outputs.items():
 path=base/name
 if path.exists():assert path.read_text()==payload,name+' changed; inspect before regeneration'
 else:path.write_text(payload)
ref=base/'reference/character-skill-combat-v6'
(ref/'player-owner-versioning-map-v6.json').write_text(json.dumps({'source_sha256':{str(p.relative_to(root)):hashlib.sha256(p.read_bytes()).hexdigest()for p in [old,base/'character_player_skills_v3.cpp']},'new_sha256':{name:hashlib.sha256((base/name).read_bytes()).hexdigest()for name in outputs},'construction':'one V6 facade with one original V3 Session/timer/property/buff graph; V3 implementation compiled unchanged under class-name macro','new_operations':['native_buffs borrowed lifetime','native_delete_buff same owned BuffOwner','native_cancel_sneaking source class146 deletion and same SkillAI end']},indent=2)+'\n')
mapping=json.loads((ref/'player-owner-versioning-map-v6.json').read_text())
mapping['new_operations']+=['native_savegame same Save borrow','native_reload_skills shared original-order coordinator; same owned V6 Skill vector and same Save SG lifecycle']
mapping['construction']='one V6 player and one V6 SkillOwner; frozen V3 implementation text reused with class-token remapping; same Session/timer/property/buff graph'
(ref/'player-owner-versioning-map-v6.json').write_text(json.dumps(mapping,indent=2)+'\n')
print('Prepared V6 player successor sharing frozen source text')
