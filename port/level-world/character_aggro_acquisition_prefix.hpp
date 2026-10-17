#pragma once

#include "character_aggro_delay.hpp"

namespace dh2::character_aggro_acquisition_prefix {

struct State {
    std::uintptr_t ai, owner;
    character_aggro_delay::State timing;
};
enum class Query : std::uint32_t {
    is_player, is_faerie, is_npc, is_monster, is_remotely_updated,
    target_408_present, target_418_present, get_char_ai_id, state_awaiting_to_spawn,
    has_aggro, spawn_radius_143c, is_my_turn, disable_optimization,
    frame_delta,
};
// Explicit borrowed views, not an overlay of the original 0x44-byte AIProps.
// row-array identity/capacity are stable bounds; radius words remain live.
struct AiPropsRow { std::uint32_t aggro_radius_3c, view_radius_40; };
struct AiPropsTable { const AiPropsRow* rows; std::uint32_t capacity; };
struct Services {
    void* context;
    // Return zero on success, with the raw source word in output. Subject is
    // the source Character/AI identity; debug/GetDt use subject zero. Each
    // field query is read exactly at its source point, not cached with booleans.
    // The two target-present queries preserve source null/non-null, without
    // truncating a 64-bit port object identity into this 32-bit result word.
    // get_char_ai_id preserves the real getter's fallback8 on every call.
    std::int32_t (*invoke)(void*, State*, Query, std::uintptr_t subject,
                           std::uint32_t* output);
    // Source global AIProps backing array captured BEFORE GetCharAIId.
    // Caller retains retired table/rows after replacement through return.
    std::int32_t (*capture_ai_props)(void*, State*, const AiPropsTable**);
};
enum class Decision : std::uint32_t {
    incomplete, skip_player, waiting_turn, waiting_delay, skip_npc,
    monster_target_branch, skip_faerie_target, ready_normal_acquisition,
};
struct Result {
    Decision decision;
    std::uint32_t service_calls, table_captures, radius_word;
    std::uintptr_t list_owner;
    character_aggro_delay::Result timing;
};
enum class Status : std::int32_t {
    complete = 0, invalid_argument = 1, service_unavailable = 2,
    service_failed = 3, invalid_source_fact = 4, unsupported_branch = 5,
};

// Bounded original _UpdateAggro entry/acquisition prefix, through preparation
// of TargetList(owner,0x7fffffff,2,1). Reuses the verified timed branch; does
// not construct/search/consume the list. Local Monster with target+0x408
// takes a separate retarget branch and returns unsupported_branch explicitly.
// The first owner IsPlayer truthy result skips; a separately fresh second
// IsPlayer or IsFaerie truthy result bypasses timing, not the later NPC gates.
// One owning thread; providers keep all owners/context/table views live,
// refresh owner synchronously and must not overwrite this Result or services.
// Captured AI is fixed; independent nested outputs are allowed. Failures retain
// completed effects and never convert an unfinished branch into a scan.
// RNG may be null only on a path that never enters the reused timed kernel.
Status prepare(State*, dh2_random_state*, const Services*, Result*);

}  // namespace dh2::character_aggro_acquisition_prefix
