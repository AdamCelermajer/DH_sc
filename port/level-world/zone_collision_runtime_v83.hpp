#pragma once
#include "canonical_checkpoint_zone_v26.hpp"
#include "canonical_quest_move_zone_v31.hpp"
#include "canonical_exit_zone_v29.hpp"
#include "../level-loader/game_event_runtime_v75.hpp"
namespace dh2::world {
struct ZonePeerBorrowV83 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 const float* position160{};
 const float* absolute12c{};
 const std::uintptr_t* physical2dc{};
 std::function<bool(float&,std::string&)> physical_radius;
};
struct ZoneLevelBorrowV83 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 events::EventManagerOwnerV12* events{};
 const std::shared_ptr<void>* save_ec{};
 const std::uint32_t* phase130{};
 const std::uint8_t* transitions144{};
 const std::uint32_t* seed114{};
 const std::int32_t* difficulty40{};
 const std::int32_t* row3c{};
};
struct ZonePlayerBorrowV83 {
 std::shared_ptr<void> receiver;
 std::uintptr_t identity{};
 const float* position160{};
 float* checkpoint1468{};
 float* save_position1474{};
 std::function<bool(bool&,std::string&)> dead;
};
// Real leaves only; all control/storage lives on the existing canonical Zone,
// Character, Level and EventManager. No second zone/contact/world authority.
struct ZoneCollisionServicesV83 {
 std::shared_ptr<void> provider;
 std::function<bool(std::uintptr_t,bool&,std::string&)> is_character,is_player;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> handle_character;
 std::function<bool(std::uintptr_t,ZonePeerBorrowV83&,std::string&)> peer;
 std::function<bool(std::int32_t,bool,std::uintptr_t&,std::string&)> local_player;
 std::function<bool(ZoneLevelBorrowV83&,std::string&)> current_level;
 std::function<bool(std::uintptr_t,ZonePlayerBorrowV83&,std::string&)> player;
 std::function<bool(const char*,const char*,std::int32_t&,std::string&)> constant;
 // Actual _colzone384 triangle-selector ray collision + reached diagnostic
 // visualization. Its NULL branch is implemented below, never approximated.
 std::function<bool(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,const ZonePeerBorrowV83&,bool&,std::string&)> mesh_inside;
 std::function<bool(std::uintptr_t,std::string&)> save_player_checkpoint;
 std::function<bool(const std::shared_ptr<void>&,std::uint32_t,std::int32_t,std::int32_t,std::string&)> save_level_checkpoint;
 std::function<bool(const char*,std::int32_t,const char*,std::string&)> assertion;
};
// Source395940/395944/395948/39649c are literal BX LR void methods.
bool zone_collision_notification_v83(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,std::string&);
bool zone_is_inside_v83(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t colzone384,
 std::uintptr_t,const ZoneCollisionServicesV83&,bool&,std::string&);
bool zone_is_touching_v83(CanonicalGameObjectBaseOwnerV1&,std::uintptr_t,
 const ZoneCollisionServicesV83&,bool&,std::string&);
bool level_checkpoint_save_v83(const ZoneLevelBorrowV83&,const float*,bool,
 const ZoneCollisionServicesV83&,std::string&);
bool checkpoint_collision_begin_v83(CanonicalCheckpointZoneV26&,std::uintptr_t,
 const ZoneCollisionServicesV83&,std::string&);
bool quest_move_collision_begin_v83(CanonicalQuestMoveZoneV31&,std::uintptr_t,
 const ZoneCollisionServicesV83&,std::string&);
struct ExitZoneRuntimeServicesV83 {
 //Pure SAME-owner/epoch admission; weak actual receiver/World captures.
 std::function<bool(CanonicalExitZoneV29&,std::string&)> current_v114;
 //Actual UpdateIdleSound38ae2c, after native signed-short370 gate.
 std::function<bool(CanonicalExitZoneV29&,std::string&)> update_idle_sound_v114;
 ZoneCollisionServicesV83 collision;
 std::function<bool(std::string&)> play_idle_sound,require_online_update;
 std::function<bool(bool&,std::string&)> online,virtual54,local_hosting;
 std::function<bool(std::uintptr_t,const char*,bool,std::int32_t,std::string&)> unlock_fasttravel;
 // Actual Arrays::LevelList ordered name lookup then StringManager.getString
 // row24. Data and localization owners must be the existing App tables.
 std::function<bool(const char*,std::string&,std::string&)> level_display_name;
 std::function<bool(bool,const char*,const char*,std::int32_t,std::string&)> display_fasttravel;
};
bool exit_zone_update_v83(CanonicalExitZoneV29&,const ExitZoneRuntimeServicesV83&,std::string&);
}
