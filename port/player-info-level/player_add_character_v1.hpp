#pragma once
#include "player_info_record_v1.hpp"
#include "player_locality_v1.hpp"
#include <array>

namespace dh2::player_add_character_v1 {
using Registry = player_locality_v1::Registry;
using Player = player_locality_v1::PlayerInfo;
using Record = player_info_record_v1::Record;

// Borrow actual Character words. This view creates no Character, Savegame,
// transform, inventory or controller owner. Only reached fields are required.
struct CharacterFields {
    std::uintptr_t identity = 0;
    std::int32_t *member_1f88 = nullptr, *internal_id_1f8c = nullptr;
    std::array<std::uint32_t*, 3> position_145c{};
    const std::uintptr_t* room_2f4 = nullptr;
    std::uint8_t* no_room_2ef = nullptr;
};
// GetBuffer leaves short/missing payload tails untouched in the source stack.
// Native callers supply captured residues or a defined allocation policy.
struct StackResidues {
    std::array<std::uint8_t, 3> skill_slots{};
    std::array<std::uint8_t, 30> skill_levels{};
};
enum class Operation : std::uint32_t {
    none, internal_id_player, record, spawn, resolve_character, debug_mode,
    source_assertion, initialize_save, is_local, save_slot, save_class,
    character_fields, online, is_host, hosting_player, set_position,
    set_rotation, set_initial_position, room_add, add_no_room, zone_entered,
    init_all, is_active, init_camera, set_idle, get_slots, save_skill_slot,
    save_skill_count, get_levels, save_skill_level, set_state, current_level,
    quick_save, remove_character, attach_controller, attach_light
};
enum class Assertion : std::uint32_t {spawn_failed, too_many_skills};
struct Services {
    void* context = nullptr;
    const player_locality_v1::Services* queries = nullptr;
    std::int32_t (*record)(void*, Player*, Record**) = nullptr;
    std::int32_t (*spawn)(void*, const char* type, const char* name,
                           bool immediate, bool initialize, std::uintptr_t* handle) = nullptr;
    std::int32_t (*resolve_character)(void*, std::uintptr_t handle, std::uintptr_t*) = nullptr;
    std::int32_t (*initialize_save)(void*, std::uintptr_t character) = nullptr;
    std::int32_t (*save_slot)(void*, std::uintptr_t, std::uint32_t) = nullptr;
    std::int32_t (*save_class)(void*, std::uintptr_t, std::int32_t) = nullptr;
    std::int32_t (*character_fields)(void*, std::uintptr_t, CharacterFields*) = nullptr;
    std::int32_t (*is_host)(void*, Player*, std::int32_t*) = nullptr;
    std::int32_t (*hosting_player)(void*, const Registry*, Player**) = nullptr;
    std::int32_t (*set_position)(void*, std::uintptr_t destination,
                                 std::uintptr_t source, bool force) = nullptr;
    std::int32_t (*set_rotation)(void*, std::uintptr_t destination, std::uintptr_t source) = nullptr;
    std::int32_t (*set_initial_position)(void*, std::uintptr_t destination, std::uintptr_t source) = nullptr;
    std::int32_t (*room_add)(void*, std::uintptr_t room, std::uintptr_t character, std::int32_t*) = nullptr;
    std::int32_t (*add_no_room)(void*, std::uintptr_t) = nullptr;
    std::int32_t (*zone_entered)(void*, std::uintptr_t) = nullptr;
    std::int32_t (*init_all)(void*, std::uintptr_t) = nullptr;
    std::int32_t (*is_active)(void*, Player*, std::int32_t*) = nullptr;
    std::int32_t (*init_camera)(void*, std::uintptr_t) = nullptr;
    std::int32_t (*set_idle)(void*, std::uintptr_t, bool) = nullptr;
    std::int32_t (*save_skill_slot)(void*, std::uintptr_t, std::int32_t slot, std::uint32_t skill) = nullptr;
    // Fresh reads of Character+14e8 and its unsigned +84 count on every call.
    std::int32_t (*save_skill_count)(void*, std::uintptr_t, bool* present, std::uint32_t*) = nullptr;
    std::int32_t (*save_skill_level)(void*, std::uintptr_t, std::uint32_t skill, std::int32_t level) = nullptr;
    std::int32_t (*set_state)(void*, std::uintptr_t, std::uint8_t) = nullptr;
    std::int32_t (*current_level)(void*, std::uintptr_t*) = nullptr;
    std::int32_t (*quick_save)(void*, std::uintptr_t level, bool) = nullptr;
    std::int32_t (*remove_character)(void*, const Registry*, std::uintptr_t) = nullptr;
    std::int32_t (*attach_controller)(void*, const Registry*, std::int32_t internal_id) = nullptr;
    std::int32_t (*attach_light)(void*, const Registry*, std::int32_t internal_id) = nullptr;
    // Source diagnostic mode is only read on source assertion branches.
    // Mode2's null write is reported as source_fault; it is never executed.
    std::int32_t (*debug_mode)(void*, std::int32_t*) = nullptr;
    std::int32_t (*source_assertion)(void*, Assertion) = nullptr;
};
enum class Status : std::uint32_t {
    complete, invalid_argument, invalid_registry, service_unavailable,
    service_failed, missing_projection, unsafe_buffer, source_fault, reentrant
};
struct Result {
    Status status = Status::complete;
    Operation last_operation = Operation::none;
    Player* player = nullptr;
    std::uintptr_t character = 0;
    std::uint32_t service_calls = 0, association_writes = 0, source_writes = 0;
    std::uint32_t skill_slot_calls = 0, skill_level_calls = 0, count_writes = 0;
    bool skipped = false, removed_online_mismatch = false;
};
class Runtime {
    Registry& registry_;
    std::uint32_t& character_count_6c4_;
    Services services_;
    bool busy_ = false;
public:
    Runtime(Registry& registry, std::uint32_t& character_count_6c4, Services services)
        : registry_(registry), character_count_6c4_(character_count_6c4), services_(services) {}
    Runtime(const Runtime&) = delete;
    Runtime& operator=(const Runtime&) = delete;
    // Exact _AddCharacter caller over the sole canonical manager/records.
    // Reached provider failures retain completed stores and add no rollback.
    Status add_character(std::int32_t internal_id, const StackResidues&, Result*);
};
} // namespace dh2::player_add_character_v1
