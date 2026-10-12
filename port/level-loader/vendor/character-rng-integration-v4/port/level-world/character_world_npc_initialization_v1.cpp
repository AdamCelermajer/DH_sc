#include "character_world_npc_initialization_v1.hpp"
#include <cstring>
namespace dh2::character {
int CharacterWorldNpcInitializationV1::initialize(const WorldNpcInitializationBorrowV1& in){
 error_.clear();
 if(!in.identity||!in.name||!in.state_owner||!in.script){error_="Required actual NPC initialization borrows";return -1;}
 const auto* record=actor_initialization(initialization_,in.room,in.name);
 skills::WorldTargetActorBorrowV1 actor{};
 if(!record||world_.actor(in.identity,&actor)||!actor.character||!actor.life||
    actor.life!=in.script->combat_state().get()||
    actor.character->resolved!=in.script->properties()->resolved.data()||
    world_.state_borrow(in.identity)!=&in.state_owner->state()||
    in.state_owner->native_fsm().character!=in.identity||
    in.script->timers().owner!=in.identity){error_="NPC initialization must borrow the same World, FSM, ScriptSession properties and life";return -1;}
 std::int32_t preset=-1;
 if(dh2_character_native_fsm_preset_state(&preset,record->resolved.ai_state.c_str())!=1||preset!=record->resolved.preset_state){error_="Required genuine authored NPC preset producer";return -1;}
 const auto result=in.state_owner->initialize_level(record->resolved.ai_state.c_str());
 if(result<0){error_=in.state_owner->error();return -2;}
 return result;
}
CharacterWorldNpcStateChangedV1::CharacterWorldNpcStateChangedV1(CharacterWorldNpcStateOwnerV1& states,AIEventState64& ai,const AIEventServices24* remaining):states_(states),ai_(ai),remaining_(remaining){services_={this,service,63,0};}
std::int32_t CharacterWorldNpcStateChangedV1::service(void* p,AIEventState64* ai,const AIEventRequest40* q,std::uint32_t* out){
 auto& self=*static_cast<CharacterWorldNpcStateChangedV1*>(p);if(ai!=&self.ai_||!q||!out)return -1;
 if(q->service==ai_event_state_getter&&q->subject==reinterpret_cast<std::uintptr_t>(&self.states_.native_fsm())){std::int32_t current;if(dh2_character_native_fsm_get_integer(&current,&self.states_.native_fsm(),0)!=1)return -1;*out=std::uint32_t(current);return 0;}
 if(q->service==ai_event_virtual&&q->operation==0x20&&q->callee==0x3d0bec&&q->subject==ai->ai){std::uint32_t previous=std::uint32_t(q->payload),next=q->argument;std::int32_t old_state,new_state;std::memcpy(&old_state,&previous,4);std::memcpy(&new_state,&next,4);return dh2_character_ai_state_changed(ai,new_state,old_state,&self.services_);}
 if(q->service==ai_event_state_event&&q->subject==reinterpret_cast<std::uintptr_t>(&self.states_.native_fsm())){const int status=self.states_.event(std::int32_t(q->event),q->payload);if(status<0)return -1;*out=std::uint32_t(status);return 0;}
 if(self.remaining_&&self.remaining_->invoke&&(self.remaining_->available&(1u<<q->service)))return self.remaining_->invoke(self.remaining_->context,ai,q,out);
 self.error_="Required genuine nonempty NPC AI/AIS state-change endpoint";return -1;
}
int CharacterWorldNpcStateChangedV1::raise(std::uintptr_t previous){
 error_.clear();if(!ai_.owner||ai_.owner->owner!=states_.native_fsm().character||ai_.owner->state_machine!=reinterpret_cast<std::uintptr_t>(&states_.native_fsm())){error_="State-change must borrow the same actual NPC FSM";return -1;}
 AIEventResult16 result{};const AIEventPayload24 payload{previous,0,0,0};const int status=dh2_character_ai_event(&result,&ai_,0x1d,&payload,&services_);if(status){if(error_.empty())error_="Required source NPC Character event1d continuation";return -2;}return 1;
}
int CharacterWorldNpcStateChangedV1::notify(const StateOwnerRequest48& q){
 if(q.operation!=state_owner_character_event||q.source_function!=0x3a4d5c||q.event!=0x1d||q.character!=states_.native_fsm().character||q.reserved[0]||q.reserved[1])return -1;
 return raise(q.payload)==1?0:-1;
}
}
