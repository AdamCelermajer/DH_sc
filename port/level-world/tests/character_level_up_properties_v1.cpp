#include "../character_level_up_properties_v1.hpp"

#include <cstdlib>
#include <iostream>

namespace lp = dh2::character_level_up_properties_v1;
namespace data = dh2::data;

static void check(bool ok, const char* message) {
    if (!ok) {
        std::cerr << message << '\n';
        std::exit(1);
    }
}

int main() {
    data::PropertyRules rules{};
    rules.defaults.fill(0);
    rules.types.fill(32);
    data::PropertyState state{};
    state.base.fill(0);
    state.saved.fill(0);
    state.gear.fill(0);
    state.resolved.fill(0);
    state.saved[19] = 7 << 8;
    state.resolved[19] = state.saved[19];
    state.saved[33] = 91 << 8;
    state.resolved[33] = state.saved[33];
    auto view = data::property_view(rules, state);
    data::PlayerSavegameV1 save;
    constexpr std::uintptr_t character = 0x1000;
    save.set_character(character);
    const lp::Owner owner{character, &view, &save, state.base.data()};
    lp::Result result{};

    check(lp::apply(&owner, &result) == lp::Status::complete,
          "canonical LevelUp property prefix rejected");
    check(result.calls == 2 && result.level_add_reached && result.xp_reset_reached,
          "source prefix call order/receipt differs");
    check(state.saved[19] == (8 << 8) && state.resolved[19] == (8 << 8),
          "PROPS_AddInt(19, 1) differs");
    check(state.saved[33] == 0 && state.resolved[33] == 0,
          "PROPS_SetInt(33, 0) differs");

    const auto old_level = state.saved[19];
    save.set_character(character + 1);
    check(lp::apply(&owner, &result) == lp::Status::invalid_argument &&
              state.saved[19] == old_level,
          "foreign Save identity mutated the Character");

    save.set_character(character);
    data::PropertyView invalid{};
    const lp::Owner bad_view{character, &invalid, &save, state.base.data()};
    check(lp::apply(&bad_view, &result) == lp::Status::invalid_argument &&
              state.saved[19] == old_level,
          "invalid canonical view mutated state");

    data::CharacterTable characters{};
    characters.names = {"AAA_DEFAULTS_DONT_DELETE", "AAA_TYPES_DONT_DELETE",
                        "PlayerClass"};
    characters.rows.resize(3);
    for (auto& row : characters.rows) row.fill(0);
    characters.rows[2][26] = 1;
    characters.rows[2][36] = 4;
    data::ClassTables classes{};
    classes.rows.resize(2);
    classes.rows[1].push_back({36, 9, 77, 0, 0});
    rules.defaults.fill(-3);
    state.base.fill(999);
    const auto update = lp::update_base_properties(
        &owner, &rules, &characters, &classes, 2, &result);
    check(update == lp::Status::complete && result.calls == 3 &&
              result.base_reset_reached && result.base_row_loaded &&
              result.class_recalc_reached && !result.class_result,
          "source UpdateBaseProperties sequence did not complete");
    check(state.base[26] == 1 && state.base[36] == 77 &&
              state.resolved[36] == 77,
          "default reset, CharacterTable row, or live class recalculation differs");

    std::cout << "{\"validation\":\"PASS\",\"cases\":4,"
                 "\"source_prefix\":\"AddInt(19,1)->SetInt(33,0)\","
                 "\"update_base_properties\":\"Reset->LoadRow->Recalc\","
                 "\"save_identity_checked\":true}\n";
}
