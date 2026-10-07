#include "character_world_runtime_v1.hpp"
#include "character_world_ai_neutral_v1.hpp"
#include <algorithm>
#include <cstring>
namespace dh2::character::skills {
CharacterWorldRuntimeV1::CharacterWorldRuntimeV1(const data::AiTables& ai,std::uint32_t capacity,const std::int32_t* mode,WorldAiServicesV1 assertion):ai_(ai),records_(std::min(capacity,65536u)),assert_mode_(mode),assertion_(assertion){
 handles_={records_.data(),0,std::uint32_t(records_.size()),0,0};
 for(const auto& row:ai.factions)factions_.push_back({row.data(),std::uint32_t(row.size()),0});
 relink();targets_=std::make_unique<CharacterWorldTargetOwnerV1>(registry_,ai_,WorldTargetServicesV1{this,borrow,control,friendly,enemy,flags,machine_state_v108,neutral_query_v108});
}
CharacterWorldRuntimeV1::Registration* CharacterWorldRuntimeV1::find(std::uintptr_t id){for(auto& r:actors_)if(r.actor.identity==id)return &r;return nullptr;}
void CharacterWorldRuntimeV1::relink(){
 entry_sentinel_={&entry_sentinel_,nullptr};auto* tail=&entry_sentinel_;
 for(auto& r:actors_){tail->next=&r.entry;tail=&r.entry;}tail->next=&entry_sentinel_;
 room_={&room_sentinel_,&entry_sentinel_};room_sentinel_={&room_,nullptr};registry_.rooms=&room_sentinel_;
}
int CharacterWorldRuntimeV1::borrow(void* p,std::uintptr_t id,WorldTargetActorBorrowV1* out){
 auto& w=*static_cast<CharacterWorldRuntimeV1*>(p);auto* r=w.find(id);if(!r||!out||!r->actor.refresh)return -1;
 *out={};if(r->actor.refresh(r->actor.context,out)||out->identity!=id||!out->search||out->search->identity!=id)return -1;
 if(out->character){if(!out->life||!out->character->resolved)return -1;if(r->actor.machine)out->character->flags520=r->actor.machine->flags;out->character->dead1449=std::uint8_t(out->life->dead);}
 r->entry.object=out->search;return 0;
}
int CharacterWorldRuntimeV1::control(void* p,std::uintptr_t id,ControllerCommandState32* out){auto* r=static_cast<CharacterWorldRuntimeV1*>(p)->find(id);return r&&r->actor.controller?r->actor.controller(r->actor.context,out):-1;}
int CharacterWorldRuntimeV1::flags(void* p,std::uintptr_t id,std::uint32_t* out){auto* r=static_cast<CharacterWorldRuntimeV1*>(p)->find(id);if(!r||!r->actor.machine||!out)return -1;*out=r->actor.machine->flags;return 0;}
int CharacterWorldRuntimeV1::machine_state_v108(void* p,std::uintptr_t id,std::uint32_t* out){auto* r=static_cast<CharacterWorldRuntimeV1*>(p)->find(id);return r&&out&&r->actor.machine_state_v108?r->actor.machine_state_v108(r->actor.context,out):-1;}
int CharacterWorldRuntimeV1::neutral_query_v108(void* p,std::uintptr_t owner,std::uintptr_t target,std::uintptr_t* out){return static_cast<CharacterWorldRuntimeV1*>(p)->neutral(owner,target,out);}
int CharacterWorldRuntimeV1::add(const WorldActorRegistrationV1& a){
 error_.clear();if(!a.identity||!a.handle_key||!a.shared_handle||!a.refresh||find(a.identity)){error_="Invalid or duplicate live World registration";return -1;}
 for(const auto& r:actors_)if(r.actor.handle_key==a.handle_key){error_="Duplicate source handle key";return -1;}
 auto at=std::lower_bound(records_.begin(),records_.begin()+handles_.count,a.handle_key,[](const auto& r,int key){return r.key<key;});
 const auto index=std::size_t(at-records_.begin());const bool exists=at!=records_.begin()+handles_.count&&at->key==a.handle_key;
 if(!exists&&handles_.count==handles_.capacity){error_="World handle registry capacity";return -2;}
 actors_.push_back({a,{}});WorldTargetActorBorrowV1 b{};
 if(borrow(this,a.identity,&b)){actors_.pop_back();error_="Required actual actor/property/life/FSM borrow";return -2;}
 if(!exists){std::move_backward(records_.begin()+index,records_.begin()+handles_.count,records_.begin()+handles_.count+1);++handles_.count;}
 records_[index]={a.handle_key,0,a.identity};*a.shared_handle={a.handle_key,handles_.frame,a.identity};relink();return 0;
}
int CharacterWorldRuntimeV1::remove(std::uintptr_t id){
 auto it=std::find_if(actors_.begin(),actors_.end(),[&](const auto& r){return r.actor.identity==id;});if(it==actors_.end())return -1;
 for(unsigned i=0;i<handles_.count;++i)if(records_[i].key==it->actor.handle_key)records_[i].object=0;
 // Teardown advances the cache epoch even when it occurs between frames.
 ++handles_.frame;for(auto& r:actors_)if(r.actor.current_target&&*r.actor.current_target==id)*r.actor.current_target=0;
 *it->actor.shared_handle={};actors_.erase(it);relink();return 0;
}
void CharacterWorldRuntimeV1::clear(){while(!actors_.empty())remove(actors_.front().actor.identity);handles_.count=0;}
int CharacterWorldRuntimeV1::refresh(){for(auto& r:actors_){WorldTargetActorBorrowV1 b{};if(borrow(this,r.actor.identity,&b)){error_="Live World actor refresh failed";return -2;}
 if(b.position)std::memcpy(b.search->position,b.position,12);if(b.heading_angle)b.search->rotation=*b.heading_angle;
 b.search->has_target_position=0;if(b.target_node&&*b.target_node){if(!b.target_enabled){error_="Required source target-enabled byte";return -2;}if(*b.target_enabled){if(!b.cached_target_position){error_="Required actual target-node cache";return -2;}std::memcpy(b.search->target_position,b.cached_target_position,12);b.search->has_target_position=1;}}
 }return 0;}
int CharacterWorldRuntimeV1::get_handle(std::uintptr_t id,target_providers::Handle16* out){auto* r=find(id);if(!r||!out)return -1; r->actor.shared_handle->frame=handles_.frame;*out=*r->actor.shared_handle;return 0;}
int CharacterWorldRuntimeV1::handle_borrow(std::uintptr_t id,target_providers::Handle16** out,target_providers::Registry24** registry){auto* r=find(id);if(!r||!out||!registry)return -1;*out=r->actor.shared_handle;*registry=&handles_;return 0;}
int CharacterWorldRuntimeV1::resolve(target_providers::Handle16* h,std::uintptr_t* out,bool asserted){return dh2_world_handle_object_v1(out,h,&handles_,asserted,assert_mode_,&assertion_);}
int CharacterWorldRuntimeV1::ai_service(void* p,const WorldAiRequestV1* q,WorldAiResponseV1* out){
 if(!q||!out)return -1;auto& w=*static_cast<CharacterWorldRuntimeV1*>(p);*out={};
 if(q->service==world_ai_handle)return w.get_handle(q->subject,&out->handle);
 if(q->service==world_ai_object)return w.resolve(q->handle,&out->identity,false);
 if(q->service==world_ai_assert)return w.assertion_.invoke?w.assertion_.invoke(w.assertion_.context,q,out):-1;
 WorldTargetActorBorrowV1 b{};if(borrow(&w,q->subject,&b))return -1;
 if(q->service==world_ai_kind){out->value=b.character?0:1;return 0;}
 if(q->service==world_ai_faction){if(!b.character)return -1;out->value=dh2_world_ai_faction_v1(b.character->resolved,std::uint32_t(w.factions_.size()));return 0;}
 std::uint32_t op=0;switch(q->service){case world_ai_player:op=target_providers::is_player;break;case world_ai_interactive:op=target_providers::is_interactive;break;case world_ai_interaction_type:op=target_providers::interaction_type;break;default:return -1;}
 auto s=w.targets_->search_services();target_search::Request24 request{op==target_providers::is_player?target_search::is_player:op==target_providers::is_interactive?target_search::is_interactive:target_search::interaction_type,0,q->subject,q->other};target_search::Response16 result{};
 if(s.invoke(s.context,&request,&result))return -1;out->value=std::int32_t(result.word);return 0;
}
int CharacterWorldRuntimeV1::relationship(std::uintptr_t owner,std::uintptr_t target,bool is_enemy,std::uintptr_t* out){
 auto* r=find(owner);if(!r||!out)return -1;WorldAiRelationshipV1 state{owner,r->actor.current_target,factions_.data(),std::uint32_t(factions_.size()),0,assert_mode_};WorldAiServicesV1 service{this,ai_service};WorldAiOutputV1 result{};
 const auto status=dh2_world_ai_relationship_v1(&result,&state,is_enemy,target,&service);if(status){error_="Required full source AI relationship continuation";return status;}*out=result.result;return 0;
}
int CharacterWorldRuntimeV1::enemy(void* p,std::uintptr_t a,std::uintptr_t b,std::uintptr_t* out){return static_cast<CharacterWorldRuntimeV1*>(p)->relationship(a,b,true,out);}
int CharacterWorldRuntimeV1::neutral(std::uintptr_t owner,std::uintptr_t target,std::uintptr_t* out){
 auto* r=find(owner);if(!r||!out)return -1;
 WorldAiRelationshipV1 state{owner,r->actor.current_target,factions_.data(),std::uint32_t(factions_.size()),0,assert_mode_};
 WorldAiServicesV1 service{this,ai_service};WorldAiOutputV1 result{};
 const auto status=dh2_world_ai_neutral_v1(&result,&state,target,&service);
 if(status){error_="Required full source AI neutral continuation";return status;}*out=result.result;return 0;
}
int CharacterWorldRuntimeV1::friendly(void* p,std::uintptr_t a,std::uintptr_t b,std::uintptr_t* out){return static_cast<CharacterWorldRuntimeV1*>(p)->relationship(a,b,false,out);}
int CharacterWorldRuntimeV1::notify_death(std::uintptr_t id){WorldTargetActorBorrowV1 b{};if(borrow(this,id,&b)||!b.life||!b.life->dead)return -1;
 for(auto& r:actors_)if(r.actor.current_target&&*r.actor.current_target==id){if(!r.actor.target_died||r.actor.target_died(r.actor.context,id)){error_="Required source AI target Died delivery";return -2;}}return 0;}
int CharacterWorldRuntimeV1::update_ai(std::uintptr_t id,AIUpdateResult16* out){auto* r=find(id);if(!r||!r->actor.ai_update||!r->actor.ai_services||!out)return -2;return dh2_character_ai_update(out,r->actor.ai_update,r->actor.ai_services);}
}
