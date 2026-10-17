#pragma once

#include "character_anim_table_resolver_v1.hpp"
#include "character_coordinator.hpp"
#include "character_dead_animation_selection_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_sm_set_dead_state_v1 {

struct AnimStanceServices {
    void* context = nullptr;
    // Returns zero on success and writes the exact Character::GetAnimStance
    // result. Called only when a selected CharAnimTable animation has its
    // source stance-mask bit set.
    int (*get_anim_stance)(void*, std::uintptr_t character,
                           std::int32_t* stance) = nullptr;
};

struct Bindings {
    character::Coordinator* coordinator = nullptr;
    character_anim_table_resolver_v1::CharacterView character{};
    const data::AnimationTables* tables = nullptr;
    const dh2_pycst_view* constants = nullptr;
    AnimStanceServices anim_stance{};
};

struct Request {
    // Original SM_SetDeadState arguments (a2, a3, a4). `killer` is retained
    // as the exact opaque pointer payload for event 50008.
    std::uint32_t dead = 0;
    std::uintptr_t killer = 0;
    std::uint32_t force_transition = 0;
};

struct Result {
    std::int32_t table_id = 17;
    std::uint32_t table_count = 0;
    std::uint32_t table_valid = 0;
    std::uint32_t death_mask_queries = 0;
    std::uint32_t death_stance_queries = 0;
    std::uint32_t despawn_mask_queries = 0;
    std::uint32_t despawn_stance_queries = 0;
    std::uint32_t state_transitioned = 0;
    std::int32_t death_animation = -1;
    std::int32_t despawn_animation = -1;
};

enum class Status : std::uint32_t {
    complete,
    invalid_argument,
    invalid_source_fact,
    service_failed,
};

// Source SM_SetDeadState on the existing Character's one Coordinator. It
// preserves the source table/mask/GetAnimStance order and mutates only after
// the same source selection point. `force_transition` selects `_SetState`
// versus `RaiseStateEvent`; neither path allocates or owns a second FSM.
Status execute(const Bindings&, const Request&, Result*, std::string& error);

} // namespace dh2::character_sm_set_dead_state_v1
