#pragma once

#include <cstdint>
#include <string>

namespace dh::foundation::enemy_ai {

// Float32 values as observed by monster.luac after GetProp/FromFixed and the
// actual host/current-Level Lua wrappers. The provider must have delivered all
// reads in source order before calling the pure policy; missing receivers are
// not replaced with a guessed player level or difficulty.
struct RuntimeMonsterLevelInputsV1 {
    bool source_reads_complete = false;
    float monster_level_max = 0.0f;
    float monster_level_min = 0.0f;
    float monster_level_offset = 0.0f;
    float host_player_level = 0.0f;
    float host_difficulty = 0.0f;
    bool current_range_values_returned = false;
    float current_range_min = 0.0f;
    float current_range_max = 0.0f;
};

struct RuntimeMonsterLevelDecisionV1 {
    bool calls_set_level = false;
    bool uses_current_level_range = false;
    float selected_level_before_offset = 0.0f;
    float level_passed_to_to_fixed = 0.0f;
    float difficulty = 0.0f;
};

// Translates the source dynamic-level block in monster.luac. GetHostPlayerLevel,
// GetHostPlayerDifficulty and GetCurrentLevelRange must already have been
// evaluated by the caller from the same source session and in that order.
// This function performs no RNG calls and does not mutate actor properties.
// Character::SetLevel remains the canonical owner of ToFixed delivery, the
// MaxLevelDVeryHard upper cap, property19 assignment and recalculation.
bool resolve_runtime_monster_level_v1(const RuntimeMonsterLevelInputsV1&,
                                     RuntimeMonsterLevelDecisionV1& output,
                                     std::string& error);

} // namespace dh::foundation::enemy_ai
