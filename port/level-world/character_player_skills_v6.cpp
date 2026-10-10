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
CharacterSkillOwnerV6* CharacterPlayerSkillsV6::native_skill_owner()noexcept{return impl_->owner.get();}
const CharacterSkillOwnerV6* CharacterPlayerSkillsV6::native_skill_owner()const noexcept{return impl_->owner.get();}
int CharacterPlayerSkillsV6::native_unload_script_v105(bool final){
 auto& t=*impl_;auto& source=t.session->owner().lifecycle();
 // 3cc9e0..3cca04: no active AIS, or !final&&!delayed24, is a
 // genuine source return. Never delete the selected VM in that branch.
 if(!source.active||(!final&&!source.delayed))return 1;
 if(t.owner->cleanup_skills()<0||t.owner->cleanup_spells()<0){t.error=t.owner->error()+t.service->error();return -2;}
 if(t.owner->source_destroy_instances_v105(0)<0||t.owner->source_destroy_instances_v105(1)<0){t.error=t.owner->error();return -2;}
 const int status=t.session->owner().source_release_active_v105();
 if(status<0){t.error=t.session->owner().error();return status;}
 t.ready=false;return 1;
}
int CharacterPlayerSkillsV6::native_update_mapped_skills_v70(){
 auto& t=*impl_;if(t.failed)return -2;
 const int code=t.owner->update_mapped_v70([&](const auto& visit){
  if(!t.saved)return 1; // Character.SG_TellSlots null-Save return.
  const auto& slots=t.saved->skill_slots()[0]; // source GetCurrentSkillSet literal0.
  // Borrow the actual ordered map. Reload the successor by key after each
  // callback so native storage never dereferences an erased source iterator.
  auto it=slots.begin();while(it!=slots.end()){const auto key=it->first;const auto row=it->second;const int result=visit(row);if(result<0)return result;it=slots.upper_bound(key);}return 1;
 },[&](std::uint32_t& selected){
  if(!t.saved){selected=0;return 0;}
  std::int32_t difficulty{};
  if(!t.init.gameplay.difficulty||t.init.gameplay.difficulty(t.init.gameplay.difficulty_context,&difficulty)||difficulty<0||difficulty>2){t.error="Required SAME SG_GetCurrentFaerieId difficulty producer";return -1;}
  selected=static_cast<std::uint32_t>(t.saved->current_faery(static_cast<std::uint32_t>(difficulty)));return 0;
 });
 if(code<0&&t.error.empty())t.error=t.owner->error()+t.service->error();return code;
}
int CharacterPlayerSkillsV6::native_source_update_all_skills_v70(){
 // InitPost may call this before InitProcess configures the vectors. The
 // genuine empty-vector branch still executes both FSM predicates; do not
 // synthesize readiness or configure instances ahead of the source order.
 auto& t=*impl_;if(t.failed)return -2;const int code=t.owner->update();
 if(code<0)t.error=t.owner->error()+t.service->error();return code;
}
int CharacterPlayerSkillsV6::native_ai_init_final_v70(){
 auto& t=*impl_;if(t.failed)return -2;auto& source=t.session->owner().lifecycle();
 if(!source.active)return 0;ScriptSessionView active{};
 if(!t.session->owner().find(source.active,active)||active.kind!=script_player_iphone){t.error="Required SAME selected AISPlayerIPhone final receiver";return -2;}
 const ScriptTimerCall16 call{active.vm,active.aliases};
 const auto code=dh2_character_script_initial_virtual(&call,active.kind,16);
 if(code){t.error="Required source AISPlayerIPhone OnInitFinal virtual16";return -2;}return 1;
}
int CharacterPlayerSkillsV6::native_instance_callback_v68(std::uint32_t kind,std::uint32_t index,std::uint32_t operation,std::uint32_t* result){
 auto& t=*impl_;if(kind>1||operation>4||!result)return -1;if(!t.ready||t.failed)return -2;
 const auto& rows=kind?t.owner->state().spells:t.owner->state().skills;
 if(index>=rows.count||!rows.items)return -1;if(!rows.items[index]){*result=0;return 0;}
 return skill_callback_session_v3(*t.session,rows.items[index],operation,result,t.error);
}
int CharacterPlayerSkillsV6::native_init_loaded_script_v62(std::uint32_t final){
 auto& t=*impl_;if(final>1)return -1;if(t.failed)return -2;
 auto& lifecycle=t.session->owner().lifecycle();if(!lifecycle.active)return 0;
 ScriptSessionView active{};if(!t.session->owner().active(active)||active.kind!=script_player_iphone){t.failed=true;t.error="Required actual PlayerIPhone receiver for loaded InitProcess";return -2;}
 const DeferredScriptServices16 services{&t,Impl::invoke};
 const auto status=dh2_character_deferred_script(&lifecycle,script_init_process,final,&services);
 if(status<0){t.failed=true;if(t.error.empty())t.error="Required source loaded player InitProcess continuation";return status;}
 t.ready=true;t.error.clear();return 1;
}
int CharacterPlayerSkillsV6::cleanup_skills(){
 auto& t=*impl_;if(!t.ready||t.failed)return -2;
 const int code=t.owner->cleanup_skills();t.error=t.owner->error()+t.service->error();return code;
}
int CharacterPlayerSkillsV6::cleanup_spells(){
 auto& t=*impl_;if(!t.ready||t.failed)return -2;
 const int code=t.owner->cleanup_spells();t.error=t.owner->error()+t.service->error();return code;
}
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

