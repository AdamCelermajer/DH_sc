#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace dh2::object_creation_map_v1 {

// Static ObjectManager::GetNewObject factory dispatch from the ARM32 source
// image. This maps a source type name to its constructor thunk; it does not
// instantiate an object or stand in for a Character constructor.
enum class Constructor : std::uint8_t {
    decor, animated_decor, dummy, floor, light_point, col_box, billboard,
    module, character, room_zone, quest_move_in_zone, checkpoint_zone,
    trigger_object, trigger_plate, trigger_zone, trigger_zone_exit_level,
    trigger_trap, timer_trap, projectile_trap, door, spawn_point, spawn_spot,
    liftable_object, openable_container, slot_container,
    destructible_container, item_object, projectile, laser_type_projectile,
    sound_emitter, level_config,
};

struct Entry {
    std::string_view source_type;
    Constructor constructor;
    // Original ARM32 GetNewInstance function address, provenance only.
    std::uint32_t source_thunk;
};

// Exact 33-entry `objectCreationMap` order at 0x95c800. `Character` and
// `Player` share GetNewInstance<Character>; `Module` and `Block` share
// GetNewInstance<Module>.
inline constexpr std::array<Entry, 33> entries{{
    {"Decor", Constructor::decor, 0x3410fc},
    {"AnimatedDecor", Constructor::animated_decor, 0x342600},
    {"Dummy", Constructor::dummy, 0x3410a4},
    {"Floor", Constructor::floor, 0x34104c},
    {"LightPoint", Constructor::light_point, 0x34115c},
    {"ColBox", Constructor::col_box, 0x340fe0},
    {"Billboard", Constructor::billboard, 0x340fbc},
    {"Module", Constructor::module, 0x340f98},
    {"Block", Constructor::module, 0x340f98},
    {"Character", Constructor::character, 0x340800},
    {"Player", Constructor::character, 0x340800},
    {"RoomZone", Constructor::room_zone, 0x340f74},
    {"QuestMoveInZone", Constructor::quest_move_in_zone, 0x340f50},
    {"CheckpointZone", Constructor::checkpoint_zone, 0x340f2c},
    {"TriggerObject", Constructor::trigger_object, 0x340f0c},
    {"TriggerPlate", Constructor::trigger_plate, 0x340ee8},
    {"TriggerZone", Constructor::trigger_zone, 0x340ec4},
    {"TriggerZoneExitLevel", Constructor::trigger_zone_exit_level, 0x340ea0},
    {"TriggerTrap", Constructor::trigger_trap, 0x340e7c},
    {"TimerTrap", Constructor::timer_trap, 0x340e58},
    {"ProjectileTrap", Constructor::projectile_trap, 0x340e34},
    {"Door", Constructor::door, 0x340824},
    {"SpawnPoint", Constructor::spawn_point, 0x340e10},
    {"SpawnSpot", Constructor::spawn_spot, 0x340dec},
    {"LiftableObject", Constructor::liftable_object, 0x340dc8},
    {"OpenableContainer", Constructor::openable_container, 0x340da4},
    {"SlotContainer", Constructor::slot_container, 0x340d80},
    {"DestructibleContainer", Constructor::destructible_container, 0x340d5c},
    {"Item", Constructor::item_object, 0x340d38},
    {"Projectile", Constructor::projectile, 0x340d14},
    {"LaserTypeProjectile", Constructor::laser_type_projectile, 0x340cf0},
    {"SoundEmitter", Constructor::sound_emitter, 0x340ccc},
    {"LevelConfig", Constructor::level_config, 0x340ca8},
}};

enum class Status : std::uint8_t { resolved, invalid_name, not_found };
struct Resolution {
    Status status = Status::invalid_name;
    const Entry* entry = nullptr;
    std::uint32_t source_index = 0;
};

// Mirrors GetNewObject's bounded linear, exact strcmp dispatch. The returned
// entry is immutable metadata; production construction stays with a mapped
// class-specific owner.
Resolution resolve(const char* source_type) noexcept;

} // namespace dh2::object_creation_map_v1
