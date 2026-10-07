#pragma once
#include "player_gameplay_binding.hpp"
namespace dh2::android_ui {
struct FaeryHudV1 {int selected{-1},level{};bool unlocked{},usable{};float fraction{};};
// Synchronous borrow of the SAME actor. Callers must retain real renderer
// services; absent cast/model services are failures, never accepted no-ops.
struct FaeryWorldServicesV1 {
 void* context{};
 int(*controller_allowed)(void*,std::uintptr_t,bool*){};
 int(*begin_cast)(void*,std::uintptr_t,bool network_origin){};
 int(*end_cast)(void*,std::uintptr_t,bool network_origin){};
 int(*faery_character)(void*,std::uintptr_t,std::uintptr_t*){};
 int(*refresh_model)(void*,std::uintptr_t faery){};
 int(*add_animation_set)(void*,std::uintptr_t faery){};
 int(*place_faery_and_followers)(void*,std::uintptr_t player){};
};
// Delivered0, malformed-1, required-producer-2. On failure retains the actual
// reached writes/calls. No campaign state is initialized or unlocked here.
int faery_hud_v1(const model_renderer::PlayerGameplayBinding&,FaeryHudV1&,std::string&);
int faery_spell_info_v1(const model_renderer::PlayerGameplayBinding&,float*,std::string&);
int faery_spell_usable_v1(const model_renderer::PlayerGameplayBinding&,bool*,std::string&);
int faery_spell_callback_v1(const model_renderer::PlayerGameplayBinding&,unsigned operation,unsigned* result,std::string&);
int faery_hud_use_v1(const model_renderer::PlayerGameplayBinding&,const FaeryWorldServicesV1&,std::string&);
int faery_select_v1(const model_renderer::PlayerGameplayBinding&,int id,const FaeryWorldServicesV1&,std::string&);
}
