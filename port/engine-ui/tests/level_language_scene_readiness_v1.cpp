#include "../level_language_scene_readiness_v1.hpp"
#include <iostream>
#include <stdexcept>

using namespace dh2::ui;

namespace {
using Status = LevelLanguageSceneReadinessStatusV1;
unsigned checks{};
void check(bool value) {
    ++checks;
    if (!value) throw std::runtime_error("Level language readiness contract mismatch");
}
LevelLanguageSceneReadinessV1 complete_fixture() {
    LevelLanguageSceneReadinessV1 p{};
    p.level_identity = 0x1000;
    p.level_generation = 7;
    p.object_manager_identity = 0x2000;
    p.object_manager_level_identity = p.level_identity;
    p.object_manager_generation = 7;
    p.object_order_generation = 7;
    p.expected_object_count = p.object_count = 3;
    p.character_list_identity = 0x3000;
    p.character_list_level_identity = p.level_identity;
    p.character_list_generation = 7;
    p.expected_character_count = p.character_count = 2;
    p.source_add_remove_flush_complete = true;
    p.character_roster_complete = true;
    p.role_predicates_complete = true;
    p.item_type3_refresh_complete = true;
    p.type14_cache_byte_complete = true;
    p.one_localization_owner = true;
    return p;
}
}

int main() {
    try {
        LevelLanguageSceneReadinessV1 empty{};
        check(check_level_language_scene_readiness_v1(empty) == Status::missing_owner);

        // The current Crypt map/roster are projections, not the complete
        // ObjectManager Add/Remove/Flush lifecycle or type-14 object store.
        auto current = complete_fixture();
        current.source_add_remove_flush_complete = false;
        check(check_level_language_scene_readiness_v1(current) ==
              Status::incomplete_object_manager_lifecycle);
        current = complete_fixture();
        current.type14_cache_byte_complete = false;
        check(check_level_language_scene_readiness_v1(current) ==
              Status::missing_type14_cache_byte);

        auto p = complete_fixture();
        check(check_level_language_scene_readiness_v1(p) == Status::ready);
        p.object_manager_level_identity++;
        check(check_level_language_scene_readiness_v1(p) == Status::wrong_level_owner);
        p = complete_fixture();
        ++p.object_order_generation;
        check(check_level_language_scene_readiness_v1(p) == Status::stale_generation);
        p = complete_fixture();
        ++p.character_count;
        check(check_level_language_scene_readiness_v1(p) == Status::membership_count_mismatch);
        p = complete_fixture();
        p.character_roster_complete = false;
        check(check_level_language_scene_readiness_v1(p) == Status::incomplete_character_roster);
        p = complete_fixture();
        p.role_predicates_complete = false;
        check(check_level_language_scene_readiness_v1(p) == Status::missing_role_predicates);
        p = complete_fixture();
        p.item_type3_refresh_complete = false;
        check(check_level_language_scene_readiness_v1(p) == Status::missing_item_refresh);
        p = complete_fixture();
        p.one_localization_owner = false;
        check(check_level_language_scene_readiness_v1(p) ==
              Status::duplicate_or_missing_localization_owner);

        std::cout << "{\"validation\":\"PASS\",\"checks\":" << checks
                  << ",\"current_partial_projection_fails_closed\":true}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
