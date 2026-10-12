#include "character_ai_state_changed.hpp"
namespace {
template<class T>bool aligned(const T* p){return p&&reinterpret_cast<std::uintptr_t>(p)%alignof(T)==0;}
int relay(dh2::character::AIEventState64* state,std::uint32_t slot,std::uintptr_t empty,
 std::uint32_t event,std::uint32_t first,std::uint32_t second,const dh2::character::AIEventServices24* services){
 using namespace dh2::character;
 if(!aligned(state)||!state->ai||state->reserved0||state->reserved1||state->reserved2)return 1;
 const auto receiver=state->active;if(!receiver)return 0;
 if(!aligned(state->ais_virtuals))return 1;
 const auto callee=state->ais_virtuals[slot/4];if(callee==empty)return 0;
 if(!callee)return 2;
 if(!services)return 2;
 if(!aligned(services)||services->reserved)return 1;
 if(!(services->available&(1u<<ai_event_ais_virtual))||!services->invoke)return 2;
 const AIEventRequest40 request{ai_event_ais_virtual,slot,event,second,receiver,callee,first};
 std::uint32_t ignored=0;return services->invoke(services->context,state,&request,&ignored)?3:0;
}
}
extern "C" int dh2_character_ai_state_changed(dh2::character::AIEventState64* state,
 std::int32_t next,std::int32_t old,const dh2::character::AIEventServices24* services){
 return relay(state,0x20,0x3dbe8c,0x1d,static_cast<std::uint32_t>(next),static_cast<std::uint32_t>(old),services);
}
extern "C" int dh2_character_ai_end_anim(dh2::character::AIEventState64* state,
 const dh2::character::AIEventServices24* services){return relay(state,0x98,0x3dbeec,0,0,0,services);}
extern "C" int dh2_character_ais_external_end_anim(const dh2::object_identity::TargetScript16* script,
 const dh2::object_identity::TargetServices16* services){
 if(!aligned(script)||!script->identity||script->reserved||!aligned(services)||!services->invoke)return 1;
 const dh2::object_identity::TargetCall32 call{script->identity,"OnEndOfAnim",0,0,0};
 return services->invoke(services->context,&call)?2:0;
}
