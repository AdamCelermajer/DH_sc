#pragma once

#include "level_tables.hpp"

#include <array>
#include <cstdint>
#include <string>

namespace dh2::data {

// Values read from the temporary PlayerSavegame in NativeStartGame. The caller
// projects these from that one Save owner; this struct owns no save/profile.
struct NativeStartGameSaveViewV1 {
    std::int32_t slot = -1;
    std::int32_t unlocked_difficulty = 0;
    std::array<std::int32_t, 3> level_rows{};
    std::array<std::int32_t, 3> entry_points{};
    std::array<std::uint8_t, 3> use_spawn_points{};
};

struct NativeStartGameRequestV1 {
    std::int32_t current_difficulty = 0;
    bool has_numeric_difficulty = false;
    std::int32_t requested_difficulty = 0;
    bool online = false;
    bool local_player_hosting = false;
    // DesignSettingsTable.members[0] + 0x24, used only when LNAM word50 is -1.
    std::int32_t initial_level_row = -1;
    bool current_level_present = false;
    std::int32_t current_level_state = 0;
};

struct NativeStartGamePlanV1 {
    bool should_launch = false;
    std::int32_t slot = -1;
    std::int32_t difficulty_before_request = 0;
    std::int32_t difficulty_for_level = 0;
    std::int32_t requested_difficulty_for_load = 0;
    std::int32_t level_row = -1;
    std::string level_name;
    std::string level_file;
    std::int32_t entry_point = 0;
    // Source LoadLevel's spawn/pending byte: offline copies LUSP; online clears it.
    std::uint8_t load_spawn_flag = 0;
    bool save_after_numeric_request = false;
    bool clear_saved_spawn_flag_before_load = false;
    std::int32_t saved_spawn_flag_row_to_clear = -1;
    bool resume = false;
    bool remotely_triggered = false;
    // Raw NativeStartGame arguments to Application::LoadLevel; the menu path
    // passes zero for both. Application's later local seed choice is separate.
    std::uint32_t seed = 0;
    std::uint32_t synchronized_seed = 0;
};

// Application::LoadLevel's local path selects the campaign seed after
// NativeStartGame. Both Random channels receive it; Level::_LoadProcess chooses
// the channel consumed by the current mode. Remote-triggered loads are separate.
struct NativeApplicationLoadSeedV1 {
    std::uint32_t ordinary_seed = 0;
    std::uint32_t synchronized_seed = 0;
};

NativeApplicationLoadSeedV1 resolve_application_load_seed_v1(
    std::uint32_t saved_seed, bool dont_use_player_seed,
    std::uint32_t real_time_seed) noexcept;

// Resolve the bounded offline/host/client argument selection recovered from
// NativeStartGame (0x43e0d0) before Application::LoadLevel (0x32bdc8).
// This creates a plan only: it does not execute Save reads/writes or load MLX/XML.
bool resolve_native_start_game_plan_v1(const LevelTables&,
    const NativeStartGameSaveViewV1&, const NativeStartGameRequestV1&,
    NativeStartGamePlanV1&, std::string& error);

}  // namespace dh2::data
