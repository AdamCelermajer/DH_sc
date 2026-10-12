#include "native_level_transition_plan_v1.hpp"

#include "../game-data/native_start_game_plan_v1.hpp"

#include <limits>

namespace dh2::ui {
namespace {
bool fail(const char* message, std::string& error) {
    error = message;
    return false;
}
}

bool build_native_level_transition_plan_v1(
    const data::LevelTables& levels,
    const data::PlayerSaveLoadOwnerV1& active_player_loader,
    std::int32_t player_slot,
    const std::string& destination_name,
    std::int32_t normalized_entry_point,
    std::int32_t difficulty,
    bool dont_use_player_seed,
    std::uint32_t real_time_seed,
    NativeLevelTransitionPlanV1& output,
    std::string& error) {
    error.clear();
    if (destination_name.empty())
        return fail("Native destination plan requires a nonempty LevelList name", error);
    if (difficulty < 0 || difficulty >= 3)
        return fail("Native destination plan difficulty is outside the three source rows", error);

    const auto& save = active_player_loader.save();
    const auto& profile = active_player_loader.profile();
    if (player_slot < 0 || save.slot() != player_slot || !save.character() ||
        !save.level_name_loaded() ||
        !profile.identity || !profile.owner || !profile.campaign)
    {
        error = std::string("Native destination plan requires the active Character's loaded same-slot Save/profile and LNAM | slot ") +
                    std::to_string(player_slot) + "/" + std::to_string(save.slot()) + " character " +
                    std::to_string(save.character() != 0) + " LNAM " + std::to_string(save.level_name_loaded()) +
                    " profile " + std::to_string(bool(profile.identity)) + std::to_string(bool(profile.owner)) +
                    std::to_string(bool(profile.campaign));
        return false;
    }

    const data::LevelDeclaration* selected = nullptr;
    std::int32_t selected_row = -1;
    for (std::size_t row = 0; row < levels.levels.size(); ++row) {
        const auto& candidate = levels.levels[row];
        if (candidate.name != destination_name) continue;
        if (row > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()))
            return fail("Native destination LevelList row exceeds source signed range", error);
        selected = &candidate;
        selected_row = static_cast<std::int32_t>(row);
        break; // NativeGoToZone and find_level use the first exact-name row.
    }
    if (!selected || selected->level_file.empty())
        return fail("Native destination is absent from the active LevelList or has no source filename", error);

    NativeLevelTransitionPlanV1 candidate;
    candidate.level_row = selected_row;
    candidate.level_name = selected->name;
    candidate.load.file = selected->level_file;
    candidate.load.entry_point = normalized_entry_point;
    candidate.load.player_slot = player_slot;
    candidate.load.resume = true;
    candidate.load.pending = true;
    candidate.load.difficulty = difficulty;
    candidate.load.remote_trigger = false;

    // Authored static MLX levels with offline world/spawn/actor tables.
    struct StaticLevel { std::int32_t row; const char* name; const char* file;
                         const char* prefix; const char* bres; };
    static constexpr StaticLevel static_levels[]{
        {41, "SWAMP", "001_swamp.mlx", "001_swamp", "data/3d/modules/swamp/swamp.bdae"},
        {4, "DARKWOOD", "003_darkwood.mlx", "003_darkwood",
         "data/3d/modules/darkwood/darkwood2.bdae"},
    };
    const StaticLevel* static_level = nullptr;
    for (const auto& level : static_levels)
        if (selected_row == level.row && selected->name == level.name &&
            selected->level_file == level.file) static_level = &level;
    if (static_level) {
        const std::string prefix = static_level->prefix;
        candidate.runtime_kind = NativeDestinationRuntimeKindV1::swamp_static;
        candidate.descriptor = prefix + ".dwld";
        candidate.bres = static_level->bres;
        candidate.spawns = prefix + ".spwn";
        candidate.objects = prefix + ".dact";
    } else if (selected_row == 23 && selected->name == "GOTHICUS_CRYPT_01" &&
               selected->level_file == "007_crypt_01.rule.xml") {
        candidate.runtime_kind = NativeDestinationRuntimeKindV1::crypt_generated;
        candidate.descriptor = "generated-crypt.dwld";
        candidate.bres = "crypt.bdae";
        candidate.spawns = "generated-crypt.spwn";
        candidate.objects = "generated-crypt.dact";
    }

    candidate.saved_seed = static_cast<std::uint32_t>(
        save.level_name_fields().word5c[static_cast<std::size_t>(difficulty)]);
    candidate.used_saved_seed = candidate.saved_seed != 0 && !dont_use_player_seed;
    const auto seeds = data::resolve_application_load_seed_v1(
        candidate.saved_seed, dont_use_player_seed, real_time_seed);
    candidate.load.seed = seeds.ordinary_seed;
    candidate.load.synchronized_seed = seeds.synchronized_seed;
    output = std::move(candidate);
    error.clear();
    return true;
}

} // namespace dh2::ui
