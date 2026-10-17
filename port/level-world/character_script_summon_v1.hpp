#pragma once

#include "character_runtime_factory_v1.hpp"

#include <array>
#include <cstdint>
#include <string>
#include <string_view>

namespace dh2::character_script_summon_v1 {

namespace factory = character_runtime_factory_v1;
namespace manager = object_manager_runtime_owner_v1;
namespace aggro = character::aggro_search;

// GameObject::_Summon's six native arguments. The order is the source Lua
// order: CharacterTable OID, spawn-state flag, XYZ offsets, and the final
// relative/look-at flag. Keep this typed at the VM boundary.
struct Arguments {
    std::int32_t character_table_oid{-1};
    std::uint32_t request_spawn_state{};
    std::array<float, 3> offset{};
    std::uint32_t relative_to_summoner{};
};

struct Request {
    std::uintptr_t container_identity{};
    Arguments arguments{};
    std::string_view character_table_name;
};

enum class Stage : std::uint8_t {
    none,
    placement_prepared,
    character_constructed,
    properties_loaded,
    positioned,
    rotated,
    summon_flag_written,
    room_enrolled,
    no_room_fallback,
    zone_entered,
    spawn_state_applied,
    complete,
};

enum class Status : std::uint8_t {
    complete,
    invalid_argument,
    service_unavailable,
    service_failed,
    factory_failed,
    properties_failed,
    placement_failed,
    room_failed,
    no_room_failed,
    zone_failed,
    spawn_state_failed,
};

struct Placement {
    std::array<float, 3> position{};
    std::array<float, 3> rotation{};
    bool floor_hit{};
};

// The adapters borrow the same active Level, ObjectManager, CharacterTable,
// Property owner, RoomZone owner, and Character Coordinator used elsewhere.
// A callback may mutate only the canonical Record/GameObject it receives.
struct Services {
    void* context{};
    // Computes source _Summon placement, performs PFWorld::GetFloorHeightAt,
    // and applies its source fallback when there is no floor hit.
    int (*prepare_placement)(void*, const Request&, Placement*, std::string&){};
    // Character::Spawn's CharacterTable default + named property load path.
    int (*load_default_properties)(void*,
        character_runtime_factory_v1::Record&, std::string&){};
    // Character::SetInitialPosition followed by GameObject::SetPosition.
    int (*set_initial_position)(void*,
        character_runtime_factory_v1::Record&, const float xyz[3],
        std::string&){};
    int (*set_rotation)(void*, character_runtime_factory_v1::Record&,
        const float xyz[3], std::string&){};
    // Character+5348 (decimal, 0x14e4) = 1 after successful placement.
    int (*set_summon_flag_5348)(void*,
        character_runtime_factory_v1::Record&, std::string&){};
    // Performs RoomZone::AddInitialObject against the caller's current room.
    // Return 1 when accepted, 0 when source fallback is required, -1 on error.
    int (*try_room_enrollment)(void*, std::uintptr_t container_identity,
        character_runtime_factory_v1::Record&, std::string&){};
    // Exact fallback source sequence: ObjectManager::AddNoRoomObject, write
    // Character+751, then GameObject::ZoneEntered.
    int (*add_no_room_object)(void*,
        character_runtime_factory_v1::Record&, std::string&){};
    int (*zone_entered)(void*, character_runtime_factory_v1::Record&,
        std::string&){};
    // SM_SetSpawnState(false,false), only when request flag 1 is true.
    int (*set_spawn_state)(void*,
        character_runtime_factory_v1::Record&, std::string&){};
};

struct Result {
    Stage reached{Stage::none};
    std::uintptr_t character_identity{};
    std::int32_t source_handle{-1};
    std::uint32_t factory_stage_mask{};
    bool floor_hit{};
    bool room_accepted{};
    bool no_room_fallback{};
    bool spawn_state_requested{};
};

// Creates one canonical Character through the current factory/manager/roster,
// then runs the source-ordered CreateNPC + _Summon tail. Failures after the
// factory returns preserve the already reached source prefix and expose the
// same identity in Result; they do not invent a second actor or silently retire
// a partially initialized source object.
Status summon(factory::Owner&,
              const Request&, const manager::GameObject&,
              const aggro::GameObject&,
              const factory::Services&,
              const Services&, Result*, std::string& error);

} // namespace dh2::character_script_summon_v1
