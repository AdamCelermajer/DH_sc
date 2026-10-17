#pragma once
#include <cstdint>

namespace dh2::character_monster_retarget {
struct State { std::uintptr_t ai, owner; };
enum class Operation : std::uint32_t {
    highest_aggro, resolve_target_408, get_aggro, design_factor_8,
    diagnostic_switch, set_target, is_enemy, clear_aggro, sync_last_target,
};
enum class Subject : std::uint32_t { owner_ai, original_ai, global };
struct Request {
    Operation operation;
    Subject kind;
    std::uintptr_t subject, peer;
    std::uint32_t word;
};
struct Response { std::uintptr_t identity; std::uint32_t word; };
struct Services {
    void* context;
    // Zero means success. Owner-AI requests resolve that Character's genuine
    // embedded CharAI, except resolve_target_408 reads/resolves the Character
    // field via nonconst GetHandle then ObjectHandle::operator Character*.
    // Peer identities are retained original objects.
    // get_aggro/design return raw float words; highest/resolve return identities;
    // is_enemy returns source bool; set_target word is original force=0.
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
};
enum class Decision : std::uint32_t {
    incomplete, keep_current, switched_to_highest, cleared_non_enemy,
    enemy_retention_search_boundary,
};
struct Result {
    Decision decision;
    std::uint32_t service_calls, current_aggro_word, highest_aggro_word,
                  factor_word, threshold_word;
    std::uintptr_t highest, current;
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3, invalid_source_fact = 4, unsupported_branch = 5,
};
// Bounded local Monster target+0x408 branch. entry_owner is the owner already
// captured for the preceding direct+0x408 read; do not invent a fresh reread for
// AI_GetHighestAggro. Other owner loads remain fresh at their source points.
// Reconstructs threat switch and non-enemy clear paths; enemy retention's
// separate flags1/filter2 TargetList/property-registry branch is explicit
// unsupported_branch, before constructing its list. No ordinary scan substitute.
// Borrowed identities/context/State/services stay live on one owning thread,
// including retired peers/owners after replacement. Independent nested outputs
// allowed; providers must not destroy borrowed values or overwrite this result.
// Missing/error/throwing services preserve completed effects, no rollback.
Status update(State*, std::uintptr_t entry_owner, const Services*, Result*);
}  // namespace dh2::character_monster_retarget
