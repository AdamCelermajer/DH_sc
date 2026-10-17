#include "../character_distribute_xp_v1.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace xp = dh2::character_distribute_xp_v1;
namespace data = dh2::data;

namespace {
constexpr std::uintptr_t KILLER = 0x111001;
constexpr std::uintptr_t VICTIM = 0x222002;
constexpr std::uintptr_t FIRST = 0x333003;
constexpr std::uintptr_t SECOND = 0x444004;
constexpr std::uintptr_t THIRD = 0x555005;

void check(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}

struct CharacterOwner {
    data::PropertyRules rules{};
    data::PropertyState state{};
    data::PropertyView properties{};
    data::PlayerSavegameV1 save{};
    xp::CharacterView view{};

    CharacterOwner(std::uintptr_t identity, int level, int xp_bonus,
                   float x, float y, bool player) {
        rules.defaults.fill(0);
        rules.types.fill(0);
        rules.types[19] = 32;
        rules.types[33] = 32;
        rules.types[34] = 16;
        rules.types[35] = 32;
        rules.types[200] = 4;
        data::reset_properties(rules, state);
        properties = data::property_view(rules, state);
        state.saved[19] = level * 256;
        state.saved[33] = 0;
        state.saved[35] = 100 * 256;
        state.base[34] = 10000 * 256;
        state.base[200] = xp_bonus * 256;
        std::string error;
        check(data::recalc_properties(rules, state, error),
              "fixture properties did not resolve");
        if (player) {
            save.set_character(identity);
            save.set_unlocked_difficulty(0);
        }
        view = {identity, &properties, player ? &save : nullptr, x, y};
    }
};

struct Fixture {
    CharacterOwner killer{KILLER, 1, 0, 0, 0, false};
    CharacterOwner victim{VICTIM, 10, 0, 0, 0, false};
    CharacterOwner first{FIRST, 8, 0, 0, 0, true};
    CharacterOwner second{SECOND, 12, 0, 8, 0, true};
    CharacterOwner third{THIRD, 8, 0, 11, 0, true};
    std::vector<CharacterOwner*> players{&first, &second, &third};
    xp::State state{&killer.view, &victim.view};
    xp::Services services{};
    std::vector<std::uint32_t> design_reads;
    std::vector<std::int32_t> awards;
    std::vector<std::int32_t> text_amounts;
    std::uint32_t get_player_calls{};
    std::uint32_t local_checks{};
    bool install_give_xp = true;
    std::uintptr_t local_character = FIRST;
    std::int32_t level_difficulty = 1;

    Fixture() {
        services = {this, design_setting, player_count, player_by_friendly_ordinal,
                    give_xp, is_local_player, current_level_difficulty,
                    scrolling_xp_text};
    }

    static std::int32_t design_setting(void* raw, std::uint32_t offset,
                                       float* value, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        self.design_reads.push_back(offset);
        switch (offset) {
        case 140: *value = 5.0f; break;
        case 144: *value = -2.0f; break;
        case 148: *value = 120.0f; break;
        case 152: *value = 50.0f; break;
        case 156: *value = 5.0f; break;
        case 160: *value = 10.0f; break;
        case 164: *value = 10.0f; break;
        default: return 1;
        }
        return 0;
    }

    static std::int32_t player_count(void* raw, std::int32_t* count,
                                    std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        *count = static_cast<std::int32_t>(self.players.size());
        return 0;
    }

    static std::int32_t player_by_friendly_ordinal(
        void* raw, std::uint32_t index, std::uint32_t require_character,
        xp::CharacterView* character, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(require_character == 1, "GetPlayer did not request an actual Character");
        ++self.get_player_calls;
        if (index >= self.players.size()) return 1;
        *character = self.players[index]->view;
        return 0;
    }

    static std::int32_t give_xp(void* raw, xp::CharacterView* character,
                                std::int32_t amount, std::uint32_t update_stat,
                                std::uint32_t* source_return, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(character && character->properties && character->savegame &&
              character->savegame->character() == character->identity,
              "_GiveXP left the canonical Character/Save/PropertyView owners");
        check(update_stat == 1, "DistributeXP did not pass _GiveXP a3=1");
        self.awards.push_back(amount);
        check(!dh2_property_add(character->properties, 33, amount),
              "_GiveXP fixture could not add through the canonical PropertyView");
        *source_return = 1;
        return 0;
    }

    static std::int32_t is_local_player(void* raw, std::uintptr_t character,
                                        std::uint32_t* local, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        ++self.local_checks;
        *local = character == self.local_character ? 1u : 0u;
        return 0;
    }

    static std::int32_t current_level_difficulty(void* raw,
                                                  std::int32_t* difficulty,
                                                  std::string&) {
        *difficulty = static_cast<Fixture*>(raw)->level_difficulty;
        return 0;
    }

    static std::int32_t scrolling_xp_text(void* raw, std::uintptr_t victim,
                                          std::int32_t amount,
                                          std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(victim == VICTIM, "XP scrolling text targeted something but the killed Character");
        self.text_amounts.push_back(amount);
        return 0;
    }
};

void matches_source_scaling_range_coop_share_and_text_tail() {
    Fixture f;
    xp::Result result{};
    std::string error;
    check(xp::distribute(&f.state, &f.services, &result, error) == xp::Status::complete,
          "source XP distribution did not complete");
    check(result.victim_base_xp == 100 && result.player_slots == 3 &&
          result.player_characters == 3 && result.range_eligible == 2 &&
          result.xp_share_percent == 90.0f,
          "recipient eligibility or cooperative share percentage changed");
    // The source's +1 before integer conversion gives a one-point minimum to
    // a zeroed out-of-radius entry in the second pass.
    check(f.awards == std::vector<std::int32_t>{100 * 256, 94 * 256, 256},
          "level scaling, radius zeroing, or source fixed-point rounding changed");
    check(f.get_player_calls == 6 && result.give_xp_calls == 3 &&
          result.xp_awarded == 3 && result.local_player_checks == 3 &&
          result.scrolling_text_calls == 1,
          "two source roster passes or local combat-text gate changed");
    check(f.text_amounts == std::vector<std::int32_t>{1} &&
          result.last_displayed_xp == 1,
          "low-difficulty local XP combat-text path changed");
    check(f.first.state.resolved[33] == 100 * 256 &&
          f.second.state.resolved[33] == 94 * 256 &&
          f.third.state.resolved[33] == 256,
          "XP provider did not receive the canonical PropertyView owners");
    const std::vector<std::uint32_t> expected{
        160, 156, 140, 144, 152, 148,
        156, 140, 144, 152, 148,
        156, 140, 144, 152, 148, 164};
    check(f.design_reads == expected,
          "DesignSettings reads did not follow source field/candidate order");
}

void keeps_missing_xp_owner_explicit_and_preserves_reached_prefix() {
    Fixture f;
    f.players = {&f.first};
    f.services.give_xp = nullptr;
    xp::Result result{};
    std::string error;
    check(xp::distribute(&f.state, &f.services, &result, error) ==
              xp::Status::service_unavailable &&
          result.range_eligible == 1 && result.give_xp_calls == 0 &&
          result.last_operation == xp::Operation::give_xp &&
          error.find("canonical XP Runtime") != std::string::npos,
          "missing _GiveXP owner was fabricated or crossed silently");
}

void rejects_roster_overrun_before_indexing_the_original_four_slot_array() {
    Fixture f;
    xp::Result result{};
    std::string error;
    struct Oversize {
        static std::int32_t count(void*, std::int32_t* value, std::string&) {
            *value = 5;
            return 0;
        }
    };
    f.services.player_count = Oversize::count;
    check(xp::distribute(&f.state, &f.services, &result, error) ==
              xp::Status::invalid_source_fact && result.player_slots == 0,
          "source four-entry stack bound was not guarded");
}

void reads_the_active_level_difficulty_from_its_existing_owner() {
    dh2::level_construction_fields::State fields{-1,-1,0,{0,0,0},2};
    bool ready=true;
    xp::CurrentLevelDifficultyOwner owner{&fields,&ready};
    std::int32_t difficulty=-99;
    std::string error;
    check(xp::current_level_difficulty_from_owner(&owner,&difficulty,error)==0 &&
          difficulty==2 && error.empty(),
          "XP text difficulty did not read current Level+0x118");

    fields.difficulty_118=-1;
    check(xp::current_level_difficulty_from_owner(&owner,&difficulty,error)==0 &&
          difficulty==-1,
          "current-Level difficulty was cached or normalized instead of freshly read");

    ready=false;
    difficulty=777;
    check(xp::current_level_difficulty_from_owner(&owner,&difficulty,error)!=0 &&
          difficulty==777 && !error.empty(),
          "retired/current-level-not-ready read changed output instead of failing closed");
}
}

int main() {
    matches_source_scaling_range_coop_share_and_text_tail();
    keeps_missing_xp_owner_explicit_and_preserves_reached_prefix();
    rejects_roster_overrun_before_indexing_the_original_four_slot_array();
    reads_the_active_level_difficulty_from_its_existing_owner();
    std::puts("PASS: Character::DistributeXP recipient order, scaling/range/co-op share, canonical XP callbacks, and current-Level difficulty owner");
}
