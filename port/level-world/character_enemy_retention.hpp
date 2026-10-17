#pragma once
#include "character_target_search.hpp"
#include "../game-data/aggro.hpp"
#include <cstdint>

namespace dh2::character_enemy_retention {
using Object = target_search::Object48;
using Target = target_search::Target24;
using List = target_search::List40;
using Registry = target_search::Registry8;
// Source +8a is stored visibility/enabled, projected by Object::visible.
struct Point { float coordinates[3]; };
enum class SearchOperation : std::uint32_t {
    is_character, look_vector, target_position, diagnostic_switch,
    melee_radius, resolve_character, is_zonable, is_interactive,
    is_dead, is_enemy, is_player, angle, interaction_radius,
};
struct SearchRequest {
    SearchOperation operation;
    std::uintptr_t subject, other;
    const Point* first; const Point* second;
};
struct SearchResponse {
    std::uintptr_t identity;
    const void* view;
    float number;
    Point point;
};
struct SearchServices {
    void* context;
    // Zero success; synchronous, exceptions become service_failed. resolve
    // returns borrowed Object* in view; target_position returns borrowed Point*;
    // look_vector returns Point by value; angle models Point3D::angle(delta,look)
    // including its source numerical library, returned in number. Other booleans
    // use identity, and radius services use number. diagnostic return is ignored.
    std::int32_t (*invoke)(void*, List*, const SearchRequest*, SearchResponse*);
};
enum class Status : std::int32_t {
    complete, invalid_argument, service_unavailable, service_failed,
    invalid_source_fact, capacity_exhausted,
};
// Exact supported domain: Character flags=1, object filter=2, closest sort=1.
// Existing melee(flags1/filter0) and aggro(maxflags/filter2) APIs are unchanged.
Status init(List*, Target*, std::uint32_t capacity, Object*, const SearchServices*);
Status search(List*, const Registry*, float radius, const SearchServices*);
Status character_valid(List*, Object*, const SearchServices*, std::uint32_t*);

struct Owner { Object object; const data::AggroTable* outgoing; };
struct State { std::uintptr_t ai; Owner* owner; };
struct Level { const Registry* rooms_70; };
struct Application { Level* level_38; std::uintptr_t player_manager_40; };
struct AiRow { std::uint32_t words[17]; }; // original 0x44-byte AIProps
struct AiTable { const AiRow* rows; std::uint32_t count; };
struct PlayerInfo { std::uintptr_t character_660; }; // Character*, NOT props ID
enum class Operation : std::uint32_t {
    application, ai_table, char_ai_id, get_local_player,
    is_enemy, design_30, add_aggro, clear_aggro, set_target, sync_last_target,
};
enum class Subject : std::uint32_t { global, owner_ai, original_ai, player_manager };
struct Request {
    Operation operation; Subject kind;
    std::uintptr_t subject, peer;
    std::uint32_t word, extra;
};
struct Response { const void* view; std::uintptr_t identity; std::uint32_t word; };
struct Services {
    void* context;
    std::int32_t (*invoke)(void*, State*, const Request*, Response*);
    SearchServices search;
};
enum class Decision : std::uint32_t {
    incomplete, cleared_empty_search, retained_known_player,
    player_not_found, player_not_enemy, added_player_aggro,
};
struct Result {
    Decision decision;
    std::uint32_t service_calls, search_count, unmatched_pops,
        known_player, found_player, radius_word, added_word;
    std::uintptr_t player;
};
// Enter ONLY after the original local Monster existing-target IsEnemy returns
// true. resolved_current is that retained target (may be null via source fallback).
// Scratch holds the local source queue; it must not be reused by a nested call.
// Identities are stable keys and must never be rewritten in borrowed projections.
// Application/Level/AI table/PlayerInfo/owner/map entries/objects/points remain
// borrowed and live, including replaced owners/tables/points, until completion.
// Callbacks may replace State::owner, live view fields and independent state.
// One owning thread; callbacks may change the list's owner/reference Character
// at source rereads, but must not mutate heap/count/capacity/sort or destroy/reenter
// this queue/output. Independent queue/output nesting is allowed. Errors retain completed
// actions and queue changes, without rollback, extra cleanup or fabricated facts.
// PlayerManager internals and source relationship/AddAggro bodies are providers.
Status update(State*, std::uintptr_t resolved_current, Target* scratch,
              std::uint32_t capacity, const Services*, Result*);
} // namespace dh2::character_enemy_retention
