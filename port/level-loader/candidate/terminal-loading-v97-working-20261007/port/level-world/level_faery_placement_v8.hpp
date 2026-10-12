#pragma once
#include "canonical_object_manager_v1.hpp"
#include <array>
namespace dh2::world {
struct LevelFaeryPlacementServicesV8 {
 std::shared_ptr<void> owner;
 // SAME actual Character list in retained canonical ObjectManager, including
 // player/NPCs. Required even when none qualify; never an empty replacement.
 CanonicalObjectManagerV1* objects{};
 std::function<bool(std::uintptr_t,std::int32_t&,std::string&)> char_type;
 std::function<bool(std::uintptr_t,std::array<float,3>&,std::string&)> look_at_vec,target_position;
 std::function<bool(std::uintptr_t,const std::array<float,3>&,bool,std::string&)> set_position;
 std::function<bool(std::uintptr_t,std::string&)> force_position,disable_zoning;
 std::function<bool(std::int32_t&,std::string&)> source_player_count6c4;
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> player;
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> local_player;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> master50;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> set_master;
 std::function<bool(std::uintptr_t,std::uintptr_t*&,std::string&)> faery420_field;
 std::function<bool(std::uintptr_t,std::uint32_t&,std::string&)> selected_faery;
 // Character.ChangeFaery ONLY; excludes outer NativeHUD currentLevel tail,
 // which would recursively Place. SAME original Save/V6/visual owners.
 std::function<bool(std::uintptr_t,std::uint32_t,std::string&)> change_faery;
 //Reached ONLY after authentic IsFaerie/IsFollower, when original inputNULL.
 //Executes3f0a64..ac8 actual Online/hosting/local0 selection +online110/114.
 std::function<bool(std::uintptr_t actual_follower,std::uintptr_t&,std::string&)> default_target_v97;
};
//Original3f0898 receiver loop. NULL selection is a reached typed source
//branch per qualifying actor; a genuinely empty/nonqualifying list skips it.
bool level_place_faery_followers_v8(std::uintptr_t selected_character,
 const LevelFaeryPlacementServicesV8&,std::string&);
}
