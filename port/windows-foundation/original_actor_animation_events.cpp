#include "original_actor_animation_events.hpp"
namespace dh::foundation {
OriginalNamedAnimationKind classify_original_named_animation_event(const std::string& name,
    std::int32_t state,bool ranged) noexcept {
    if(name.compare(0,3,"ev_")==0)return OriginalNamedAnimationKind::relay;
    if(name.compare(0,3,"an_")==0)return OriginalNamedAnimationKind::object_animation;
    if(name.compare(0,3,"fx_")==0)return OriginalNamedAnimationKind::mesh_fx;
    if(name.compare(0,4,"sfx_")==0)return OriginalNamedAnimationKind::sound_fx;
    if(state==5){
        if(ranged&&(name=="attack_ranged"||name=="attack_mainhand"||name=="do_skill"))return OriginalNamedAnimationKind::projectile;
        if(!ranged&&name=="attack_mainhand")return OriginalNamedAnimationKind::mainhand;
        if(!ranged&&name=="attack_offhand")return OriginalNamedAnimationKind::offhand;
    }
    if(state==6&&name=="do_skill")return OriginalNamedAnimationKind::skill;
    if(state==7&&name=="do_spell")return OriginalNamedAnimationKind::spell;
    if(state==13&&name=="interact")return OriginalNamedAnimationKind::interaction;
    return OriginalNamedAnimationKind::ignored;
}
bool route_original_named_animation_event(dh2::character::AIEventState64& state,
    const RetainedAnimationEvent& event,const dh2::character::AIEventServices24& services,
    std::string& error){
    if(event.name.empty()){error="Original named animation payload missing";return false;}
    const dh2::character::AIEventPayload24 payload{reinterpret_cast<std::uintptr_t>(event.name.c_str()),0,0,0};
    dh2::character::AIEventResult16 result{};
    const int status=dh2_character_ai_event(&result,&state,0x28,&payload,&services);
    if(status){error="Original named animation event28 service failed: "+std::to_string(status);return false;}
    error.clear();return true;
}
} // namespace dh::foundation
