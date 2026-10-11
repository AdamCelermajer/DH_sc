#pragma once

#include "character_apply_result.hpp"
#include "character_player_buffs_v1.hpp"
#include "character_state.hpp"
#include <cstdint>
#include <string>

namespace dh2::character { class Coordinator; }

namespace dh2::character_combat_status_v1 {

// App lookups use the same loaded ClassTable / AnimatedEffectTable as the
// Character's CharProperties. A missing FX is represented by -1, as in the
// source table search; missing required class names fail at the reached call.
struct Catalog {
    void* context = nullptr;
    int (*class_id)(void*, const char*, std::int32_t*) = nullptr;
    int (*effect_id)(void*, const char*, std::int32_t*) = nullptr;
};

// These callbacks are the original CharProperties/CharStateMachine operations
// which are still owned by the app adapter. They must act on `character` and
// the same bound Coordinator/property/buff owners in Bindings. Status
// operations passed here are only the source calls that the local buff kernel
// does not own (for example IsBoss, state-machine result handlers and targeted
// RecalcProperty); this is not an alternate combat or state owner.
struct NativeServices {
    void* context = nullptr;
    int (*is_boss)(void*, std::uintptr_t character, std::uint32_t*) = nullptr;
    // CharStateMachine reaction gates read Character+0x14fc as a float. For
    // dodge/block/hurt, prepare_reaction performs the original Character
// Character's virtual IsPlayer call at vtable byte offset +0x28, followed by
// the source write to Character+0x14fc. This is a byte offset, not a vtable
// slot index.
    int (*reaction_value)(void*, std::uintptr_t character, float*) = nullptr;
    int (*prepare_reaction)(void*, std::uintptr_t character) = nullptr;
    int (*animation_table_valid)(void*, std::uintptr_t character,
                                 std::uint32_t* valid_table) = nullptr;
    // Selects a row/stance after animation_table_valid has succeeded. The
    // sequence can legitimately be -1 for source paths without a null guard.
    int (*select_animation)(void*, std::uintptr_t character,
                            std::int32_t row_offset, std::uint32_t stance_mask,
                            std::int32_t* animation) = nullptr;
    int (*set_machine_animation)(void*, std::uintptr_t character,
                                 std::int32_t animation) = nullptr;
    int (*read_machine_flags)(void*, std::uintptr_t character,
                              std::uint32_t* flags) = nullptr;
    int (*write_machine_flags)(void*, std::uintptr_t character,
                               std::uint32_t flags) = nullptr;
    int (*set_character_flags)(void*, std::uintptr_t character,
                               std::uint32_t mask) = nullptr;
    int (*recalculate_property)(void*, std::uintptr_t character,
                               data::PropertyView*, std::int32_t property) = nullptr;
};

struct Bindings {
    std::uintptr_t character = 0;
    // This is the Record's one canonical Coordinator. Its owner identity must
    // match `character`; status handlers never create another state machine.
    dh2::character::Coordinator* coordinator = nullptr;
    data::PropertyView* properties = nullptr;
    character_player_buffs_v1::Owner* buffs = nullptr;
    Catalog catalog{};
    NativeServices native{};
};

enum class Status : std::int32_t {
    complete = 0, invalid_argument = -1, unavailable = -2,
    provider_failed = -3, unsupported_operation = -4
};

// Handles source ApplyResult's DoT, slow, and CharStateMachine status calls
// against the Character's existing property/buff/timer/Coordinator owners.
// App callbacks expose raw Character, CharAnimTable, and CharStateMachine facts;
// this closure owns source ordering, same-Coordinator event/state routing, and
// state/timer choices. No call is successful when a reached provider is absent.
Status execute(const character_apply_result::Request*, const Bindings*,
               character_apply_result::Reply*, std::string& error);

} // namespace dh2::character_combat_status_v1
