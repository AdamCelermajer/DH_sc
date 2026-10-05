#include "character_world_skill_combat_v6.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::character::skills {
CharacterWorldSkillCombatV6::CharacterWorldSkillCombatV6(CharacterWorldRuntimeV1& world,const data::AiTables& ai,const SkillAttackNativeServicesV6& debug,WorldSkillCombatBackendsV6 backends):world_(world),ai_(ai),debug_(debug),backends_(backends),hit_{this,hit_service},application_{this,apply_service,&debug_}{}
CharacterWorldSkillCombatV6::Entry* CharacterWorldSkillCombatV6::find(std::uintptr_t id){for(auto& r:actors_)if(r.registration.identity==id)return &r;return nullptr;}
int CharacterWorldSkillCombatV6::borrow(std::uintptr_t id,WorldSkillCombatBorrowV6& out){
 auto* entry=find(id);WorldTargetActorBorrowV1 actor{};out={};
 if(!entry||!entry->registration.refresh||world_.actor(id,&actor)||!actor.character||!actor.life||entry->registration.refresh(entry->registration.context,&out))return -1;
 auto* a=out.attack;auto* b=out.application;
 if(!a||!b||a->identity!=id||b->identity!=id||!a->properties||a->properties!=b->properties||a->properties->resolved!=actor.character->resolved||!a->facts||a->facts->properties!=a->properties->resolved||out.life!=actor.life||dh2_property_validate(a->properties)){error_="Required SAME registered World property/facts/life borrow";return -1;}
 if(out.inventory&&(out.inventory->character()!=id||out.inventory->properties()->resolved.data()!=a->properties->resolved)){error_="World combat Gear/property owner mismatch";return -1;}
 target_providers::Handle16* handle=nullptr;target_providers::Registry24* registry=nullptr;if(world_.handle_borrow(id,&handle,&registry))return -1;
 entry->handle={id,handle,registry};b->attacker_handle=&entry->handle;
 if(b->hit){if(b->hit->identity!=id||b->hit->properties!=b->properties)return -1;b->hit_services=&hit_;}
 return 0;
}
int CharacterWorldSkillCombatV6::add(const WorldSkillCombatRegistrationV6& r){if(!r.identity||!r.refresh||find(r.identity))return -1;actors_.push_back({r,{}});WorldSkillCombatBorrowV6 b{};if(borrow(r.identity,b)){actors_.pop_back();return -2;}return 0;}
int CharacterWorldSkillCombatV6::remove(std::uintptr_t id){auto it=std::find_if(actors_.begin(),actors_.end(),[&](const auto& e){return e.registration.identity==id;});if(it==actors_.end())return -1;actors_.erase(it);return 0;}
int CharacterWorldSkillCombatV6::actor_service(void* p,std::uintptr_t id,SkillAttackActorV6** a,SkillApplyActorV6** b){if(!a||!b)return -1;WorldSkillCombatBorrowV6 borrow{};auto& w=*static_cast<CharacterWorldSkillCombatV6*>(p);if(w.borrow(id,borrow))return -1;*a=borrow.attack;*b=borrow.application;return 0;}
int CharacterWorldSkillCombatV6::handle_service(void* p,std::uintptr_t id,target_providers::Handle16** a,target_providers::Registry24** b){return static_cast<CharacterWorldSkillCombatV6*>(p)->world_.handle_borrow(id,a,b);}
SkillCombatWorldV6 CharacterWorldSkillCombatV6::native_world()noexcept{return {this,handle_service,world_.targets().query_services(),actor_service,backends_.other};}
int CharacterWorldSkillCombatV6::player(std::uintptr_t id,std::uintptr_t* out){auto q=world_.targets().query_services();target_providers::Request24 request{target_providers::virtual_player,0,id,0};return q.invoke(q.context,&request,out);}
int CharacterWorldSkillCombatV6::hit_service(void* p,HitActor32* receiver,const HitRequest32* q,std::uintptr_t* out){
 if(!receiver||!q||!out)return -1;auto& w=*static_cast<CharacterWorldSkillCombatV6*>(p);*out=0;
 if(q->service==hit_is_dead||q->service==hit_is_character||q->service==hit_is_monster){WorldTargetActorBorrowV1 b{};if(w.world_.actor(q->subject,&b))return -1;
  if(q->service==hit_is_dead){if(!b.life)return -1;*out=b.life->dead;return 0;}
  if(q->service==hit_is_character){*out=b.character!=nullptr;return 0;}
  if(!b.character||w.ai_.rows.size()<9)return -1;auto id=b.character->resolved[1];auto index=id>=0&&std::size_t(id)<w.ai_.rows.size()?std::size_t(id):8u;*out=w.ai_.rows[index].type==4;return 0;
 }
 if(q->service==hit_is_player)return w.player(q->subject,out);
 if(q->service==hit_debug_load){SkillAttackNativeRequestV6 request{skill_attack_debug_load_v6,0,0,nullptr};return w.debug_.invoke?w.debug_.invoke(w.debug_.context,&request,out):-1;}
 if(q->service==hit_debug_query){
  if(!w.debug_.invoke||!q->name)return -1;std::uintptr_t token=0,ignored=0;SkillAttackNativeRequestV6 construct{skill_attack_string_construct_v6,0,0,q->name};
  if(w.debug_.invoke(w.debug_.context,&construct,&token)||!token)return -1;SkillAttackNativeRequestV6 get{skill_attack_debug_get_v6,0,token,nullptr},destroy{skill_attack_string_destroy_v6,0,token,nullptr};
  if(w.debug_.invoke(w.debug_.context,&get,out))return -1;return w.debug_.invoke(w.debug_.context,&destroy,&ignored);
 }
 if(!w.backends_.hit.invoke){w.error_="Required source Hit service "+std::to_string(q->service)+" after retained HP prefix";return -1;}
 const auto status=w.backends_.hit.invoke(w.backends_.hit.context,receiver,q,out);if(status)w.error_="Required source Hit service "+std::to_string(q->service);return status;
}
int CharacterWorldSkillCombatV6::threat(const SkillApplyRequestV6& q,SkillApplyResponseV6& out){
 WorldSkillCombatBorrowV6 owner{},target{};if(borrow(q.target,owner)||borrow(q.attacker,target)||!owner.outgoing||!target.incoming)return -1;
 std::uintptr_t owner_player=0;if(player(q.target,&owner_player))return -1;
 std::uint32_t amount;std::memcpy(&amount,&q.number,4);const auto facts=(owner_player?data::aggro_owner_player:0u)|(owner.life->dead?data::aggro_owner_dead:0u)|(target.life->dead?data::aggro_target_dead:0u);
 data::AggroRequest request{owner.outgoing,target.incoming,q.target,q.attacker,amount,facts};data::AggroChange change{};
 if(dh2_aggro_apply(&change,&request,data::aggro_add))return -1;std::memcpy(&out.number,&change.returned_bits,4);
 // These are the genuine source callbacks, after reciprocal map writes.
 for(unsigned bit=1;bit<=8;bit<<=1)if(change.requests&bit){if(!backends_.aggro_event||backends_.aggro_event(backends_.aggro_context,bit,q.target,q.attacker)){error_="Required source OnAggro continuation after real threat writes";return -1;}}
 return 0;
}
int CharacterWorldSkillCombatV6::apply_service(void* p,const SkillApplyRequestV6* q,SkillApplyResponseV6* out,data::CombatResult* result){
 if(!q||!out)return -1;auto& w=*static_cast<CharacterWorldSkillCombatV6*>(p);*out={};
 if(q->service==skill_apply_is_dead_v6){WorldTargetActorBorrowV1 b{};if(w.world_.actor(q->subject,&b)||!b.life)return -1;out->word=b.life->dead;return 0;}
 if(q->service==skill_apply_is_player_v6){std::uintptr_t value=0;if(w.player(q->subject,&value))return -1;out->word=std::uint32_t(value);return 0;}
 if(q->service==skill_apply_aggro_v6){WorldSkillCombatBorrowV6 a{},b{};if(!w.borrow(q->target,a)&&!w.borrow(q->attacker,b)&&a.outgoing&&b.incoming)return w.threat(*q,*out);}
 if(!w.backends_.application.invoke){w.error_="Required source SkillApply service "+std::to_string(q->service);return -1;}
 const auto status=w.backends_.application.invoke(w.backends_.application.context,q,out,result);if(status)w.error_="Required source SkillApply service "+std::to_string(q->service);return status;
}
int CharacterWorldSkillCombatV6::apply(SkillApplyOutputV6* out,data::CombatResult* result,std::uintptr_t attacker,std::uintptr_t target){
 error_.clear();WorldSkillCombatBorrowV6 a{},b{};if(!out||!result||borrow(attacker,a)||borrow(target,b))return -1;
 const auto status=dh2_character_skill_apply_result_v6(out,result,a.application,b.application,&application_);
 // Source lifecycle store is a projection of the SAME actor, not a new state.
 if(status!=-1&&out->hit.lifecycle_written&&b.application->hit)b.life->lifecycle=b.application->hit->lifecycle;
 return status;
}
}
