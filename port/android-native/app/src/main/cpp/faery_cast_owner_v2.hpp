#pragma once
#include "faery_gameplay_v1.hpp"
namespace dh2::android_ui {
enum class CastBodyV2 {focus,blur,event,update};
// Shared source +d0/+d1 live in V6.skill_ai().continued/last. No additional
// spell flags, FSM, VM or TimerStore are owned by this adapter.
struct FaeryCastServicesV2 {
 void* context{};
 int(*faery_type)(void*,std::uintptr_t,unsigned faery,unsigned* source_type){};
 int(*cast_animation)(void*,std::uintptr_t,unsigned faery,int* animation){};
 int(*stance)(void*,std::uintptr_t,unsigned*){};
 int(*raise_event)(void*,std::uintptr_t,unsigned,std::uintptr_t){};
 int(*animation)(void*,std::uintptr_t,int){};
 int(*speed)(void*,std::uintptr_t,float){};
 int(*disable_heading)(void*,std::uintptr_t){};
 int(*cancel_sneaking)(void*,std::uintptr_t){};
 int(*animation_stop_loop)(void*,std::uintptr_t,bool immediate){};
 // Actual network-enabled field +5 must be delivered; a missing provider
 // cannot be silently treated as offline.
 int(*network_enabled)(void*,bool*){};
 int(*network_spell)(void*,std::uintptr_t,unsigned operation,unsigned faery){};
 const dh2::character::StateOwnerServices16* state_services{};
};
class FaeryCastOwnerV2 {
 FaeryCastServicesV2 services_;std::string error_;
 int selected(const model_renderer::PlayerGameplayBinding&,int&,unsigned&);
 int network(const model_renderer::PlayerGameplayBinding&,bool,unsigned,unsigned);
public:
 explicit FaeryCastOwnerV2(FaeryCastServicesV2 services):services_(services){}
 const std::string& error()const noexcept{return error_;}
 //0 delivered(rejected gates leave acceptedfalse),-1 malformed,-2 required.
 int controller_allowed(const model_renderer::PlayerGameplayBinding&,bool*);
 int can_begin_casting(const model_renderer::PlayerGameplayBinding&,bool*);
 int begin(const model_renderer::PlayerGameplayBinding&,bool network_origin,bool* accepted);
 int end(const model_renderer::PlayerGameplayBinding&,bool network_origin);
 int command(const model_renderer::PlayerGameplayBinding&,bool begin,bool network_origin);
 int state_body(const model_renderer::PlayerGameplayBinding&,CastBodyV2);
 // Hook source Character events20/21, actual named do_spell event, and
 // original step Begin/End_SkillSpell using real Animator step/mode facts.
 int character_event(const model_renderer::PlayerGameplayBinding&,unsigned event);
 int animation_event(const model_renderer::PlayerGameplayBinding&,const char*);
 int animation_step(const model_renderer::PlayerGameplayBinding&,bool begin,unsigned step,unsigned mode);
};
}
