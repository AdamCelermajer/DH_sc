#include "../native_level_transition_plan_v1.hpp"

#include "../../game-data/player_profile_index_v1.hpp"
#include "../../game-data/player_savegame_v1.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace {
unsigned checks{};
void require(bool condition, const char* message) {
    ++checks;
    if (!condition) throw std::runtime_error(message);
}
void append_word(std::vector<std::uint8_t>& bytes, std::uint32_t value) {
    for (unsigned i = 0; i != 4; ++i)
        bytes.push_back(static_cast<std::uint8_t>(value >> (i * 8)));
}

struct ActivePlayer {
    dh2::data::PlayerSavegameV1 save;
    std::shared_ptr<dh2::data::PlayerProfileIndexV1> index =
        std::make_shared<dh2::data::PlayerProfileIndexV1>();
    dh2::data::PlayerSaveProfileV1 profile;
    dh2::data::PlayerSaveLoadOwnerV1 loader{save, profile};

    ActivePlayer() {
        save.set_character(0x1234);
        save.set_slot(2);
        const std::array<std::uint8_t, 4> empty_index{};
        std::string error;
        require(index->load({empty_index.data(), empty_index.size()}, error),
                "empty campaign index fixture failed");
        profile.identity = reinterpret_cast<std::uintptr_t>(index.get());
        profile.owner = index;
        profile.campaign = index->borrow();
        std::vector<std::uint8_t> lnam;
        append_word(lnam, 77);
        append_word(lnam, 4);   append_word(lnam, 101); append_word(lnam, 10);
        append_word(lnam, 5);   append_word(lnam, 202); append_word(lnam, 11);
        append_word(lnam, 6);   append_word(lnam, 303); append_word(lnam, 12);
        std::size_t consumed{};
        require(save.load_level_name({lnam.data(), lnam.size()}, consumed, error),
                "active Save LNAM fixture failed");
        require(consumed == lnam.size(), "active Save LNAM fixture was not fully read");
    }
};

dh2::data::LevelDeclaration level(const char* name, const char* file) {
    dh2::data::LevelDeclaration row{};
    row.name = name;
    row.level_file = file;
    return row;
}
}

int main() {
    try {
        ActivePlayer player;
        dh2::data::LevelTables levels;
        levels.levels.resize(42);
        levels.levels[5] = level("SOURCE_ONLY_LEVEL", "source-only.mlx");
        levels.levels[6] = level("SOURCE_ONLY_LEVEL", "later-duplicate.mlx");
        levels.levels[23] = level("GOTHICUS_CRYPT_01", "007_crypt_01.rule.xml");
        levels.levels[41] = level("SWAMP", "001_swamp.mlx");
        const auto fields_before = player.save.level_name_fields();

        dh2::ui::NativeLevelTransitionPlanV1 plan;
        std::string error;
        require(dh2::ui::build_native_level_transition_plan_v1(
                    levels, player.loader, 2, "SWAMP", 7, 1, false, 999, plan, error),
                error.c_str());
        require(plan.level_row == 41 && plan.level_name == "SWAMP" &&
                    plan.load.file == "001_swamp.mlx" && plan.load.entry_point == 7 &&
                    plan.load.player_slot == 2 && plan.load.difficulty == 1,
                "SWAMP destination fields differ from the selected LevelList row");
        require(plan.runtime_kind == dh2::ui::NativeDestinationRuntimeKindV1::swamp_static &&
                    plan.descriptor == "001_swamp.dwld" &&
                    plan.bres == "data/3d/modules/swamp/swamp.bdae" &&
                    plan.spawns == "001_swamp.spwn" && plan.objects == "001_swamp.dact",
                "SWAMP runtime configuration differs from the selected loader");
        require(plan.saved_seed == 202 && plan.used_saved_seed &&
                    plan.load.seed == 202 && plan.load.synchronized_seed == 202,
                "transition did not select the active Save's difficulty seed for both Random channels");
        require(player.save.level_name_fields().word5c == fields_before.word5c &&
                    player.save.slot() == 2 && player.loader.profile().identity ==
                        reinterpret_cast<std::uintptr_t>(player.index.get()),
                "destination planning mutated or rebound the active Save/profile");

        require(dh2::ui::build_native_level_transition_plan_v1(
                    levels, player.loader, 2, "GOTHICUS_CRYPT_01", 9, 2,
                    true, 444, plan, error), error.c_str());
        require(plan.level_row == 23 &&
                    plan.runtime_kind == dh2::ui::NativeDestinationRuntimeKindV1::crypt_generated &&
                    plan.load.file == "007_crypt_01.rule.xml" && plan.load.entry_point == 9 &&
                    plan.saved_seed == 303 && !plan.used_saved_seed &&
                    plan.load.seed == 444 && plan.load.synchronized_seed == 444,
                "Crypt transition failed to honor runtime-seed override or source row");

        require(dh2::ui::build_native_level_transition_plan_v1(
                    levels, player.loader, 2, "SOURCE_ONLY_LEVEL", -3, 0,
                    false, 1, plan, error), error.c_str());
        require(plan.level_row == 5 && plan.level_name == "SOURCE_ONLY_LEVEL" &&
                    plan.load.file == "source-only.mlx" && plan.load.entry_point == -3 &&
                    plan.runtime_kind == dh2::ui::NativeDestinationRuntimeKindV1::unsupported &&
                    plan.descriptor.empty(),
                "first-match LevelList row or unsupported-runtime plan was lost");
        std::cout << "PASS native_level_transition_plan_v1 checks=" << checks << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL native_level_transition_plan_v1: " << error.what() << '\n';
        return 1;
    }
}
