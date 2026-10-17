#include "../character_give_xp_v1.hpp"
#include "../character_distribute_give_xp_dispatch_v1.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace xp = dh2::character_give_xp_v1;
namespace data = dh2::data;

namespace {
constexpr std::uintptr_t CHARACTER = 0x120001;

void check(bool value, const char* message) {
    if (!value) {
        std::fprintf(stderr, "FAIL: %s\n", message);
        std::exit(1);
    }
}

struct PlayerLookupFixture {
    dh2::character_level_member::IntMember levels[2]{};
    dh2::player_manager_host_level::PlayerInfoProjection host{};
    dh2::player_manager_host_level::PlayerInfoProjection selected{};
    dh2::player_manager_host_level::PlayerInfoProjection* entries[1]{};
    dh2::player_manager_host_level::PlayerRegistry registry{};
    std::uintptr_t selected_character=0xabc001;
    std::uint32_t character_reads=0;
    dh2::player_locality_v1::Services services{};

    PlayerLookupFixture() {
        host={0x10001008,-1,&levels[0]};
        selected={0x10002008,77,&levels[1]};
        entries[0]=&selected;
        registry={0x10001000,entries,1,&host};
        services.context=this;
        services.online=[](void*,std::uint8_t* online)->std::int32_t {
            *online=0;
            return 0;
        };
        services.character_660=[](void* raw,
            dh2::player_locality_v1::PlayerInfo* player,
            std::uintptr_t* character)->std::int32_t {
            auto& self=*static_cast<PlayerLookupFixture*>(raw);
            ++self.character_reads;
            *character=player==&self.selected?self.selected_character:0;
            return 0;
        };
    }
};

void word(std::vector<unsigned char>& bytes, std::uint32_t value) {
    for (unsigned i = 0; i < 4; ++i) bytes.push_back(static_cast<unsigned char>(value >> (i * 8)));
}

void string(std::vector<unsigned char>& bytes, const char* value) {
    const auto size = std::char_traits<char>::length(value);
    word(bytes, static_cast<std::uint32_t>(size));
    bytes.insert(bytes.end(), value, value + size);
}

struct Fixture {
    data::PropertyRules rules{};
    data::PropertyState state{};
    data::PropertyView view{};
    data::PlayerSavegameV1 save{};
    std::vector<unsigned char> constant_bytes;
    dh2_pycst_view constants{};
    std::vector<xp::Operation> trace;
    std::vector<std::string> max_level_trace;
    std::vector<std::int32_t> max_level_difficulty_script;
    std::size_t max_level_difficulty_reads = 0;
    bool is_player = true;
    bool ineligible_virtual = false;
    bool level_suppressed = false;
    bool one_kill = false;
    bool fail_trace_switch = false;
    std::int32_t level_difficulty = 0;
    std::int32_t level_difficulty_after_debug = -1;
    std::int32_t save_difficulty_after_debug = -1;
    bool fail_level_up = false;
    bool mutate_properties_in_level_up = false;
    std::int32_t level_up_argument = -1;

    Fixture() {
        rules.defaults.fill(0);
        rules.types.fill(0);
        rules.types[19] = 32;  // Level, saved/additive fixed-point property.
        rules.types[33] = 32;  // XP, the same canonical saved-property path.
        rules.types[34] = 16;  // Max_XP, base-derived runtime property.
        rules.types[200] = 4;  // Special_Bonus_To_XP, additive resolved property.
        data::reset_properties(rules, state);
        view = data::property_view(rules, state);
        save.set_character(CHARACTER);
        save.set_unlocked_difficulty(0);
        set_constants(50, 50, 50);
        state.saved[19] = 10 * 256;
        state.saved[33] = 100 * 256;
        state.base[34] = 1000 * 256;
        recalc();
    }

    void recalc() {
        std::string error;
        check(data::recalc_properties(rules, state, error), "property fixture failed to resolve");
    }

    void set_constants(std::int32_t normal, std::int32_t hard,
                       std::int32_t very_hard) {
        constant_bytes.clear();
        word(constant_bytes, 1);
        string(constant_bytes, "CharacterDesign");
        word(constant_bytes, 3);
        string(constant_bytes, "MaxLevelBNormal"); word(constant_bytes, normal);
        string(constant_bytes, "MaxLevelCHard"); word(constant_bytes, hard);
        string(constant_bytes, "MaxLevelDVeryHard"); word(constant_bytes, very_hard);
        check(!dh2_pycst_open(&constants, constant_bytes.data(),
                              static_cast<std::uint32_t>(constant_bytes.size())),
              "constant fixture is invalid");
    }

    void set_empty_character_design() {
        constant_bytes.clear();
        word(constant_bytes, 1);
        string(constant_bytes, "CharacterDesign");
        word(constant_bytes, 0);
        check(!dh2_pycst_open(&constants, constant_bytes.data(),
                              static_cast<std::uint32_t>(constant_bytes.size())),
              "empty CharacterDesign fixture is invalid");
    }

    static std::int32_t audit_constant(void* raw, const dh2_pycst_view* view,
                                       const char* group, const char* key,
                                       dh2_pycst_result* result, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(view == &self.constants && group && key && result,
              "max-level lookup left the canonical constants view");
        self.max_level_trace.emplace_back(std::string(group) + "/" + key);
        return dh2_pycst_get(view, group,
                             static_cast<std::uint32_t>(std::char_traits<char>::length(group)),
                             key, static_cast<std::uint32_t>(std::char_traits<char>::length(key)),
                             result);
    }

    static std::int32_t audit_unlocked_difficulty(
        void* raw, std::uintptr_t character, const data::PlayerSavegameV1* save,
        std::int32_t* value, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(character == CHARACTER && save == &self.save && value,
              "max-level difficulty read left the canonical Character/Save owners");
        self.max_level_trace.emplace_back("SG_GetGameDifficultyUnlocked");
        if (self.max_level_difficulty_reads < self.max_level_difficulty_script.size())
            *value = self.max_level_difficulty_script[self.max_level_difficulty_reads++];
        else {
            ++self.max_level_difficulty_reads;
            *value = save->unlocked_difficulty();
        }
        return 0;
    }

    static std::int32_t generic(void* raw, const xp::Request* request,
                                xp::Reply* reply, std::string&) {
        auto& self = *static_cast<Fixture*>(raw);
        check(request && reply && request->character == CHARACTER &&
              request->properties == &self.view && request->save == &self.save,
              "request left the canonical Character/Save/PropertyView owners");
        self.trace.push_back(request->operation);
        switch (request->operation) {
        case xp::Operation::character_virtual_40:
            reply->value = self.is_player ? 1 : 0;
            break;
        case xp::Operation::character_virtual_84:
            reply->value = self.ineligible_virtual ? 1 : 0;
            break;
        case xp::Operation::current_level_suppression:
            reply->word = self.level_suppressed ? 1u : 0u;
            break;
        case xp::Operation::one_kill_level_up:
            check(request->name && std::string(request->name) == "OneKillLevelUp",
                  "debug switch name changed");
            reply->word = self.one_kill ? 1u : 0u;
            if (self.save_difficulty_after_debug >= 0)
                self.save.set_unlocked_difficulty(self.save_difficulty_after_debug);
            if (self.level_difficulty_after_debug >= 0)
                self.level_difficulty = self.level_difficulty_after_debug;
            break;
        case xp::Operation::current_level_difficulty:
            reply->value = self.level_difficulty;
            break;
        case xp::Operation::trace_character_stats:
            check(request->name && std::string(request->name) == "isTracingChar_Stats",
                  "post-award debug switch name changed");
            return self.fail_trace_switch ? 1 : 0;
        case xp::Operation::player_by_character:
            reply->value = 77;
            break;
        case xp::Operation::level_up:
            return 1; // LevelUp must use its separately declared full provider.
        }
        return 0;
    }

    static std::int32_t full_level_up(void* raw, const xp::Request* request,
                                      xp::Reply*, std::string& error) {
        auto& self = *static_cast<Fixture*>(raw);
        check(request && request->operation == xp::Operation::level_up &&
              request->character == CHARACTER && request->properties == &self.view &&
              request->save == &self.save,
              "LevelUp callback did not receive the source owners");
        self.trace.push_back(request->operation);
        self.level_up_argument = request->argument;
        if (self.fail_level_up) {
            error = "fixture full LevelUp owner failed";
            return 1;
        }
        if (self.mutate_properties_in_level_up) {
            auto* properties = request->properties;
            check(!dh2_property_set(properties, 33, 15000 * 256),
                  "fixture LevelUp could not update canonical XP");
            self.state.base[34] = 12000 * 256;
            self.recalc();
            check(properties->resolved[34] == 12000 * 256,
                  "fixture LevelUp could not recalculate canonical Max_XP");
        }
        return 0;
    }

    xp::Runtime runtime(std::int32_t amount_fixed, bool install_level_up = false,
                        bool update_player_stat = true,
                        const xp::PlayerByCharacterBinding* player_lookup = nullptr) {
        const xp::Bindings bindings{CHARACTER, amount_fixed, &view, &save,
                                    &constants, update_player_stat};
        const xp::Backend backend{this, generic, install_level_up ? full_level_up : nullptr,
                                  audit_constant, audit_unlocked_difficulty,
                                  player_lookup};
        return xp::Runtime(bindings, backend);
    }
};

void awards_modified_xp_to_the_live_player() {
    Fixture f;
    f.state.base[200] = 10 * 256;
    f.recalc();
    auto runtime = f.runtime(2 * 256);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result, error) == xp::Status::complete,
          "eligible offline player did not receive XP");
    check(result.xp_added == 1 && result.raw_amount_fixed == 512 &&
          result.modified_amount_fixed == 562 && result.xp_after == 100 * 256 + 562,
          "source XP bonus arithmetic or canonical XP add changed");
    check(f.state.resolved[33] == result.xp_after && result.player_internal_id == 77 &&
          result.source_return == 1 && result.level_up_called == 0 &&
          result.stat_player_lookups == 1,
          "XP result did not retain canonical state or reached PlayerManager");
    const std::vector<xp::Operation> expected{
        xp::Operation::character_virtual_40, xp::Operation::character_virtual_84,
        xp::Operation::current_level_suppression, xp::Operation::one_kill_level_up,
        xp::Operation::current_level_difficulty,
        xp::Operation::trace_character_stats, xp::Operation::player_by_character};
    check(f.trace == expected, "ordinary _GiveXP service order changed");
}

void preserves_the_a3_stat_lookup_gate() {
    Fixture f;
    auto runtime = f.runtime(256, false, false);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result, error) == xp::Status::complete &&
          result.source_return == 1 && result.stat_player_lookups == 0 &&
          result.player_internal_id == -1 &&
          f.state.saved[33] == 100 * 256 + 256 &&
          f.trace.back() == xp::Operation::trace_character_stats,
          "_GiveXP a3=0 incorrectly reached PlayerManager::GetPlayerByCharacter");
}

void requeries_level_and_unlocked_difficulty_after_one_kill_switch() {
    Fixture f;
    f.level_difficulty = 2;
    f.level_difficulty_after_debug = 1;
    f.save_difficulty_after_debug = 0;
    // First read selects Normal max level, second read is the source's fresh
    // VeryHard override query, and the post-OneKillLevelUp Character method
    // must perform a third fresh read that disagrees with the mutated Save.
    f.max_level_difficulty_script = {0, 2, 2};
    f.one_kill = false;
    auto runtime = f.runtime(5 * 256);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result, error) == xp::Status::complete &&
          result.raw_amount_fixed == 5 * 256 && result.xp_after == 105 * 256,
          "post-OneKillLevelUp SG difficulty reread reused a stale Save difficulty");
    const std::vector<xp::Operation> expected{
        xp::Operation::character_virtual_40, xp::Operation::character_virtual_84,
        xp::Operation::current_level_suppression, xp::Operation::one_kill_level_up,
        xp::Operation::current_level_difficulty, xp::Operation::trace_character_stats,
        xp::Operation::player_by_character};
    check(f.trace == expected,
          "CurrentLevel difficulty was not freshly queried after OneKillLevelUp");
    check(f.max_level_difficulty_reads == 3 &&
          f.max_level_trace == std::vector<std::string>{
              "CharacterDesign/MaxLevelBNormal", "SG_GetGameDifficultyUnlocked",
              "SG_GetGameDifficultyUnlocked", "CharacterDesign/MaxLevelDVeryHard",
              "SG_GetGameDifficultyUnlocked"},
          "source Character::SG_GetGameDifficultyUnlocked call order changed");
}

void preserves_prefix_and_requires_real_level_up_owner() {
    Fixture f;
    f.state.saved[33] = 9000 * 256;
    f.state.base[34] = 10000 * 256;
    f.state.base[200] = 10 * 256;
    f.recalc();
    f.one_kill = true;
    auto runtime = f.runtime(256);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result, error) == xp::Status::level_up_required,
          "missing full LevelUp owner did not fail explicitly at threshold");
    check(error.find("full save/UI/trophy/script provider") != std::string::npos &&
          result.xp_added == 1 && result.level_up_called == 1 &&
          result.xp_after == f.state.resolved[33] && result.xp_after > 10000 * 256 &&
          result.player_internal_id == -1 && f.trace.back() == xp::Operation::trace_character_stats,
          "missing LevelUp provider rolled back XP or continued source tail");
}

void preserves_xp_if_the_post_add_debug_owner_fails() {
    Fixture f;
    f.fail_trace_switch = true;
    auto runtime = f.runtime(256);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result, error) == xp::Status::service_failed &&
          result.xp_added == 1 && result.stat_player_lookups == 0 &&
          result.player_internal_id == -1 &&
          f.state.saved[33] == 100 * 256 + 256 &&
          f.trace.back() == xp::Operation::trace_character_stats,
          "post-Add trace-switch failure did not preserve only the reached XP prefix");
}

void full_level_up_owner_runs_between_xp_and_player_lookup() {
    Fixture f;
    f.state.saved[33] = 9000 * 256;
    f.state.base[34] = 10000 * 256;
    f.state.base[200] = 10 * 256;
    f.recalc();
    f.one_kill = true;
    f.fail_level_up = true;
    auto failed = f.runtime(256, true);
    xp::Result result{};
    std::string error;
    check(failed.give_xp(&result, error) == xp::Status::service_failed &&
          error == "fixture full LevelUp owner failed" && result.xp_added == 1 &&
          result.xp_after == f.state.resolved[33] && result.player_internal_id == -1 &&
          f.trace.back() == xp::Operation::level_up,
          "failed full LevelUp owner did not preserve the reached prefix");

    Fixture success;
    success.state.saved[33] = 9000 * 256;
    success.state.base[34] = 10000 * 256;
    success.state.base[200] = 10 * 256;
    success.recalc();
    success.one_kill = true;
    success.mutate_properties_in_level_up = true;
    auto runtime = success.runtime(256, true);
    error.clear();
    check(runtime.give_xp(&result, error) == xp::Status::complete &&
          result.level_up_called == 1 && result.player_internal_id == 77 &&
          success.level_up_argument == result.level_up_overage &&
          result.level_up_overage == 97 &&
          result.xp_clamped == 1 && result.max_xp == 12000 * 256 &&
          result.xp_after == result.max_xp &&
          success.state.saved[33] == result.max_xp &&
          success.trace.back() == xp::Operation::player_by_character,
          "full LevelUp callback, post-level clamp, or stat tail is disconnected");
    check(success.trace[success.trace.size() - 2] == xp::Operation::level_up,
          "PlayerManager lookup ran before the full LevelUp callback");
}

void preserves_max_level_and_virtual_gates() {
    Fixture f;
    f.state.saved[19] = 50 * 256;
    f.recalc();
    auto runtime = f.runtime(256);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result, error) == xp::Status::complete &&
          f.trace.empty() && f.state.resolved[33] == 100 * 256,
          "max-level source short-circuit ran player XP services");

    Fixture not_player;
    not_player.is_player = false;
    auto player_gate = not_player.runtime(256);
    check(player_gate.give_xp(&result, error) == xp::Status::complete &&
          not_player.trace == std::vector<xp::Operation>{xp::Operation::character_virtual_40} &&
          not_player.state.resolved[33] == 100 * 256,
          "Character virtual+40 gate did not stop XP services");

    Fixture suppressed;
    suppressed.level_suppressed = true;
    auto suppressed_runtime = suppressed.runtime(256);
    check(suppressed_runtime.give_xp(&result, error) == xp::Status::complete &&
          suppressed.trace.back() == xp::Operation::current_level_suppression &&
          suppressed.state.resolved[33] == 100 * 256,
          "current Level XP suppression did not gate the award");
}

void reads_normal_constant_before_fresh_difficulty_and_override() {
    Fixture f;
    f.set_constants(100, 30, 40);
    f.state.saved[19] = 50 * 256;
    f.recalc();
    // The first source read is Normal; the else-if performs a fresh second
    // difficulty query, which selects VeryHard despite the initial value 0.
    f.max_level_difficulty_script = {0, 2};
    auto runtime = f.runtime(256);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result, error) == xp::Status::complete &&
          result.max_level == 40 && result.xp_added == 0 && f.trace.empty(),
          "fresh VeryHard difficulty did not override the previously loaded Normal max level");
    const std::vector<std::string> expected{
        "CharacterDesign/MaxLevelBNormal", "SG_GetGameDifficultyUnlocked",
        "SG_GetGameDifficultyUnlocked", "CharacterDesign/MaxLevelDVeryHard"};
    check(f.max_level_trace == expected && f.max_level_difficulty_reads == 2,
          "_GiveXP max-level constant/difficulty lookup order or fresh read changed");
}

void hard_override_uses_only_the_first_difficulty_read() {
    Fixture f;
    f.set_constants(100, 30, 40);
    f.state.saved[19] = 50 * 256;
    f.recalc();
    f.max_level_difficulty_script = {1, 2};
    auto runtime = f.runtime(256);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result, error) == xp::Status::complete &&
          result.max_level == 30 && result.xp_added == 0 && f.trace.empty(),
          "Hard difficulty did not select its source max-level override");
    const std::vector<std::string> expected{
        "CharacterDesign/MaxLevelBNormal", "SG_GetGameDifficultyUnlocked",
        "CharacterDesign/MaxLevelCHard"};
    check(f.max_level_trace == expected && f.max_level_difficulty_reads == 1,
          "Hard branch performed an extra difficulty read or changed lookup order");
}

void absent_max_level_constant_is_the_source_zero_value() {
    Fixture f;
    f.set_empty_character_design();
    f.max_level_difficulty_script = {1};
    auto runtime = f.runtime(256);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result, error) == xp::Status::complete &&
          result.max_level == 0 && result.xp_added == 0 && f.trace.empty(),
          "missing PyDataConstants max-level entry was treated as an error instead of source value zero");
    const std::vector<std::string> expected{
        "CharacterDesign/MaxLevelBNormal", "SG_GetGameDifficultyUnlocked",
        "CharacterDesign/MaxLevelCHard"};
    check(f.max_level_trace == expected,
          "missing max-level constants bypassed the source override lookup order");
}

void resolves_the_exact_player_manager_xp_tail() {
    PlayerLookupFixture fixture;
    xp::PlayerByCharacterBinding binding{&fixture.registry,&fixture.services};
    xp::PlayerByCharacterResult result{};
    check(xp::player_by_character_internal_id(&binding,
              fixture.selected_character,&result)==
              xp::PlayerByCharacterStatus::complete &&
          result.route==dh2::player_locality_v1::Route::registered_player &&
          result.player==&fixture.selected && result.internal_id==77,
          "_GiveXP tail did not use the source GetPlayerByCharacter selection and +0x670 identity");

    check(xp::player_by_character_internal_id(&binding,0xabc002,&result)==
              xp::PlayerByCharacterStatus::complete &&
          result.route==dh2::player_locality_v1::Route::manager_plus_8 &&
          result.player==&fixture.host && result.internal_id==-1,
          "GetPlayerByCharacter miss did not preserve the source manager+8 fallback");

    const auto fallback=result;
    binding.player_services=nullptr;
    check(xp::player_by_character_internal_id(&binding,fixture.selected_character,
              &result)==xp::PlayerByCharacterStatus::invalid_argument &&
          result.player==fallback.player && result.internal_id==fallback.internal_id,
          "invalid XP-tail binding replaced the last committed PlayerInfo projection");
}

void give_xp_can_use_the_canonical_player_manager_tail() {
    Fixture f;
    f.state.saved[33]=9000*256;
    f.state.base[34]=10000*256;
    f.state.base[200]=10*256;
    f.recalc();
    f.one_kill=true;

    PlayerLookupFixture players;
    players.selected_character=CHARACTER;
    xp::PlayerByCharacterBinding lookup{&players.registry,&players.services};
    auto runtime=f.runtime(256,true,true,&lookup);
    xp::Result result{};
    std::string error;
    check(runtime.give_xp(&result,error)==xp::Status::complete &&
          result.level_up_called==1 && result.stat_player_lookups==1 &&
          result.player_internal_id==77 && players.character_reads==1,
          "_GiveXP did not run LevelUp then the canonical GetPlayerByCharacter tail");
    check(f.trace.back()==xp::Operation::level_up,
          "generic fallback stole the PlayerManager-owned _GiveXP tail");
}

void distribute_dispatch_reuses_the_live_give_xp_runtime() {
    Fixture f;
    PlayerLookupFixture players;
    players.selected_character=CHARACTER;
    xp::PlayerByCharacterBinding lookup{&players.registry,&players.services};
    const xp::Backend backend{&f,Fixture::generic,nullptr,Fixture::audit_constant,
                              Fixture::audit_unlocked_difficulty,&lookup};
    dh2::character_distribute_give_xp_dispatch_v1::Owner owner{
        &f.constants,backend};
    dh2::character_distribute_xp_v1::CharacterView character{
        CHARACTER,&f.view,&f.save,0,0};
    std::uint32_t source_return=99;
    std::string error;
    check(dh2::character_distribute_give_xp_dispatch_v1::give_xp(
              &owner,&character,256,1,&source_return,error)==0 &&
          source_return==1 &&
          f.state.saved[33]==100*256+256 && players.character_reads==1 &&
          f.trace.back()==xp::Operation::trace_character_stats && error.empty(),
          "DistributeXP did not dispatch through the same canonical _GiveXP runtime and PlayerManager tail");
}
}

int main() {
    awards_modified_xp_to_the_live_player();
    preserves_the_a3_stat_lookup_gate();
    requeries_level_and_unlocked_difficulty_after_one_kill_switch();
    preserves_prefix_and_requires_real_level_up_owner();
    preserves_xp_if_the_post_add_debug_owner_fails();
    full_level_up_owner_runs_between_xp_and_player_lookup();
    preserves_max_level_and_virtual_gates();
    reads_normal_constant_before_fresh_difficulty_and_override();
    hard_override_uses_only_the_first_difficulty_read();
    absent_max_level_constant_is_the_source_zero_value();
    resolves_the_exact_player_manager_xp_tail();
    give_xp_can_use_the_canonical_player_manager_tail();
    distribute_dispatch_reuses_the_live_give_xp_runtime();
    std::puts("PASS: eligible single-player _GiveXP through canonical PropertyView/Save; XP modifier, source gates, explicit LevelUp dependency, prefix retention, and canonical PlayerManager tail");
}
