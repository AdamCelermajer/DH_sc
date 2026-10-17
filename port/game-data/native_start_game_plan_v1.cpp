#include "native_start_game_plan_v1.hpp"

namespace dh2::data {
namespace {
constexpr std::int32_t kDifficultyCount = 3;
constexpr std::int32_t kStartGameAlreadyInLevelState = 38;

bool valid_difficulty(std::int32_t value) noexcept {
    return value >= 0 && value < kDifficultyCount;
}
}  // namespace

NativeApplicationLoadSeedV1 resolve_application_load_seed_v1(
    std::uint32_t saved_seed, bool dont_use_player_seed,
    std::uint32_t real_time_seed) noexcept {
    const auto selected = saved_seed != 0 && !dont_use_player_seed
        ? saved_seed : real_time_seed;
    return {selected, selected};
}

bool resolve_native_start_game_plan_v1(const LevelTables& levels,
    const NativeStartGameSaveViewV1& save, const NativeStartGameRequestV1& request,
    NativeStartGamePlanV1& output, std::string& error) {
    error.clear();

    NativeStartGamePlanV1 candidate;
    if (request.current_level_present &&
        request.current_level_state == kStartGameAlreadyInLevelState) {
        candidate.should_launch = false;
        output = std::move(candidate);
        return true;
    }
    if (save.slot < 0) {
        error = "NativeStartGame requires the assigned nonnegative save slot";
        return false;
    }
    if (!valid_difficulty(request.current_difficulty) ||
        !valid_difficulty(save.unlocked_difficulty)) {
        error = "NativeStartGame difficulty index outside the three source rows";
        return false;
    }
    if (request.has_numeric_difficulty && !valid_difficulty(request.requested_difficulty)) {
        // Original code accepts the signed value into CurrentDifficulty before
        // using three-row source state. Reject an unsupported mode at the native
        // adapter boundary rather than forwarding an invalid mode to LoadLevel.
        error = "NativeStartGame requested difficulty outside the three source rows";
        return false;
    }
    if (levels.levels.empty()) {
        error = "NativeStartGame LevelList is unavailable";
        return false;
    }

    candidate.slot = save.slot;
    candidate.difficulty_before_request = request.current_difficulty;
    candidate.difficulty_for_level = request.current_difficulty;
    candidate.requested_difficulty_for_load = request.has_numeric_difficulty
        ? request.requested_difficulty : 0;
    candidate.save_after_numeric_request = request.has_numeric_difficulty;
    if (request.has_numeric_difficulty &&
        save.unlocked_difficulty >= request.requested_difficulty) {
        if (!valid_difficulty(request.requested_difficulty)) {
            error = "Unlocked NativeStartGame difficulty is outside the three source rows";
            return false;
        }
        candidate.difficulty_for_level = request.requested_difficulty;
    }

    auto row = save.level_rows[std::size_t(candidate.difficulty_for_level)];
    if (row == -1) row = request.initial_level_row;
    if (row < 0 || std::size_t(row) >= levels.levels.size()) {
        error = "NativeStartGame selected LevelList row is unavailable";
        return false;
    }
    const auto& declaration = levels.levels[std::size_t(row)];
    if (declaration.level_file.empty()) {
        error = "NativeStartGame LevelList row has no source filename";
        return false;
    }

    candidate.level_row = row;
    candidate.level_name = declaration.name;
    candidate.level_file = declaration.level_file;
    candidate.entry_point = request.online && !request.local_player_hosting
        ? 1 : save.entry_points[std::size_t(candidate.difficulty_for_level)];
    candidate.load_spawn_flag = request.online ? 0
        : save.use_spawn_points[std::size_t(candidate.difficulty_for_level)];
    candidate.clear_saved_spawn_flag_before_load = true;
    candidate.saved_spawn_flag_row_to_clear = candidate.difficulty_for_level;
    candidate.should_launch = true;
    candidate.resume = true;
    output = std::move(candidate);
    return true;
}

}  // namespace dh2::data
