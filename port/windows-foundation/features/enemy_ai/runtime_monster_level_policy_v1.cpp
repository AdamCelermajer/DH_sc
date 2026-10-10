#include "runtime_monster_level_policy_v1.hpp"

namespace dh::foundation::enemy_ai {

bool resolve_runtime_monster_level_v1(const RuntimeMonsterLevelInputsV1& input,
                                     RuntimeMonsterLevelDecisionV1& output,
                                     std::string& error) {
    if (!input.source_reads_complete) {
        error = "monster_OnInit requires delivered GetHostPlayerLevel, difficulty, and current-level-range reads";
        return false;
    }

    RuntimeMonsterLevelDecisionV1 next;
    next.difficulty = input.host_difficulty;
    if (input.monster_level_max > -1.0f) {
        next.calls_set_level = true;
        const bool use_current = input.current_range_values_returned &&
                                 input.current_range_max > -1.0f;
        next.uses_current_level_range = use_current;
        const float level_min = use_current ? input.current_range_min : input.monster_level_min;
        const float level_max = use_current ? input.current_range_max : input.monster_level_max;
        float level = input.host_player_level;
        if (level <= level_min) {
            level = level_min;
        } else if (level >= level_max) {
            level = level_max;
        }
        next.selected_level_before_offset = level;
        // Both operands are Lua float32 values. Keep the single source addition
        // here; integer fixed conversion and cap belong to existing ToFixed and
        // Character::SetLevel implementations.
        next.level_passed_to_to_fixed = level + input.monster_level_offset;
    }
    output = next;
    error.clear();
    return true;
}

} // namespace dh::foundation::enemy_ai
