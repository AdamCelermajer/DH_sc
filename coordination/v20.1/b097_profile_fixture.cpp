#include "../../port/windows-foundation/save_store.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    using namespace dh::foundation;
    if (argc != 4 && argc != 2) return 2;
    CharacterState state;
    std::string error;
    if (!load_character(argv[1], state, error)) { std::cerr << error; return 1; }
    if (argc == 2) {
        std::cout << "profile level=" << state.stats.level << " rank=" << state.skills.at(0).rank
                  << " points=" << state.source_skill_points << " MP=" << state.stats.resource
                  << " main=" << (state.inventory.empty() ? "empty" : state.inventory.at(0).definition_id) << '\n';
        return 0;
    }
    const auto rank = std::stoul(argv[3]);
    if (rank != 1 && rank != 2) return 2;
    if (std::filesystem::exists(argv[2])) { std::cerr << "Refusing fixture overwrite"; return 1; }
    state.id = "b097-isolated-level2-warrior";
    state.skills.at(0).rank = rank;
    state.source_skill_points = rank == 1 ? 1 : 0;
    state.inventory = {{"b097-longblade", "Longsword01", 1}};
    state.equipment = {{"main_hand", "b097-longblade", 0, 1}};
    // Existing source seed has the native BashDown starter grant and logical
    // slot0 mapping. Gear identity uses the recovered main-hand slot1.
    if (!save_character(argv[2], state, error)) { std::cerr << error; return 1; }
    CharacterState check;
    if (!load_character(argv[2], check, error) || check.skills.at(0).rank != rank ||
        check.equipment.size() != 1 || check.source_skill_points != state.source_skill_points) return 1;
    std::cout << "fixture level=" << check.stats.level << " rank=" << rank << " points="
              << check.source_skill_points << " main=" << check.inventory.at(0).definition_id << '\n';
}
