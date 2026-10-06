#include "character_player_skills_v6.hpp"
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
std::shared_ptr<data::PlayerSavegameV1> CharacterPlayerSkillsV6::native_saved_owner()const noexcept{return impl_->saved;}
int CharacterPlayerSkillsV6::native_initialize_skill_instances(){
 if(!ready())return -2;auto& t=*impl_;t.error.clear();
 const int code=t.owner->configure();
 if(code!=1)t.error=t.owner->error()+"; "+t.service->error();
 return code;
}
int CharacterPlayerSkillsV6::native_skill_animation_event(std::uint32_t* result){
 auto& t=*impl_;if(!result)return -1;if(!t.ready||t.failed||!t.init.gameplay.skill_owner)return -2;
 t.error.clear();SkillAIContextV3 state{t.init.gameplay.skill_owner,const_cast<State40*>(&t.owner->state()),&t.skill_ai,t.session->owner().lifecycle().load_step,0};
 const SkillAIServices16V3 services{&t,Impl::skill_service};
 const auto code=dh2_character_skill_ai_v3(result,&state,skill_ai_event_v3,0,&services);
 if(code&&t.error.empty())t.error="Required source AI_Event skill callback at current index "+std::to_string(t.skill_ai.current);
 return code;
}
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
