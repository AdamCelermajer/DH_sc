#include "../player_skill_progression_v1.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace progression = dh2::player_skill_progression_v1;
namespace data = dh2::data;

namespace {
constexpr std::uintptr_t kCharacter = UINT64_C(0x100000001);

void require(bool value, const char* reason) {
    if (!value) throw std::runtime_error(reason);
}

std::int32_t signed_word(std::uint32_t bits) {
    std::int32_t value = 0;
    static_assert(sizeof(value) == sizeof(bits));
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

struct Fixture {
    data::SkillTables tables;
    std::array<std::int32_t, 224> resolved{};
    data::PropertyView properties{};
    data::PlayerSavegameV1 savegame;
    std::string error;

    Fixture(std::int32_t character_level, std::int32_t required_level,
            std::int32_t saved_level, std::int32_t selector = 0,
            bool initialize = true) {
        tables.skill_lists.resize(4);
        tables.skill_lists[0].members = {0};
        tables.skill_lists[3].members = {0};
        data::SkillRow row{};
        row.table_name = "FixtureSkill";
        row.level = required_level;
        tables.skills.push_back(row);
        resolved[19] = signed_word(static_cast<std::uint32_t>(character_level)
                                   << 8);
        resolved[28] = selector;
        resolved[157] = 2 * 256;
        resolved[194] = 300 * 256;
        properties.resolved = resolved.data();
        savegame.set_character(kCharacter);
        if (initialize) {
            require(savegame.initialize_skills_from_character_list({0}, error),
                    "fixture saved-skill initialization failed");
            require(savegame.set_skill_level(0, saved_level, error),
                    "fixture saved-skill mutation failed");
        }
    }
};

struct Provider {
    std::uintptr_t character{kCharacter};
    data::PropertyView* properties{};
    data::PlayerSavegameV1* current{};
    data::PlayerSavegameV1* replace_after_add{};
    std::vector<progression::Operation> operations;
    std::vector<std::array<std::int32_t, 4>> arguments;
    std::array<std::int32_t, 3> caps{{20, 25, 30}};
    std::vector<std::int32_t> difficulties{0, 2};
    std::size_t difficulty_index{};
    progression::Operation fail_at{static_cast<progression::Operation>(UINT32_MAX)};
    std::uint32_t potion_capacity{};
    bool has_slots{};
    std::uint32_t increment_return{};

    static int current_savegame(void* opaque, std::uintptr_t character,
                                data::PlayerSavegameV1** savegame) {
        auto& self = *static_cast<Provider*>(opaque);
        if (!savegame || character != self.character) return -1;
        *savegame = self.current;
        return 0;
    }

    static int invoke(void* opaque, const progression::Request* request,
                      progression::Response* response) {
        auto& self = *static_cast<Provider*>(opaque);
        if (!request || !response || request->character != self.character)
            return -1;
        self.operations.push_back(request->operation);
        self.arguments.push_back({request->arguments[0], request->arguments[1],
                                  request->arguments[2], request->arguments[3]});
        *response = {};
        if (request->operation == self.fail_at) return -1;
        switch (request->operation) {
            case progression::Operation::has_skill_slots:
                response->word = self.has_slots;
                break;
            case progression::Operation::increment_skill:
                response->word = self.increment_return;
                break;
            case progression::Operation::skill_limit: {
                const auto difficulty = request->arguments[0];
                if (difficulty < 0 || difficulty >= 3) return -1;
                response->word = static_cast<std::uint32_t>(self.caps[difficulty]);
                break;
            }
            case progression::Operation::unlocked_difficulty:
                if (self.difficulty_index >= self.difficulties.size()) return -1;
                response->word = static_cast<std::uint32_t>(
                    self.difficulties[self.difficulty_index++]);
                break;
            case progression::Operation::add_property:
                if (!self.properties || request->arguments[0] != 157 ||
                    request->arguments[1] != -1)
                    return -1;
                self.properties->resolved[157] -= 256;
                if (self.replace_after_add) self.current = self.replace_after_add;
                break;
            case progression::Operation::recalculate_properties:
                self.properties->resolved[194] = 257 * 256;
                break;
            case progression::Operation::set_potion_capacity:
                self.potion_capacity = static_cast<std::uint32_t>(
                    request->arguments[0]);
                break;
            default:
                break;
        }
        return 0;
    }

    progression::Services services() {
        return {this, current_savegame, invoke};
    }
};

std::int32_t predicate_case(const char* kind, std::int32_t level,
                            std::int32_t required, std::int32_t saved,
                            std::int32_t save_state) {
    Fixture f(level, required, saved, 0, save_state == 1);
    if (std::string(kind) == "available") {
        const auto out = progression::is_skill_available(
            &f.properties, &f.tables, kCharacter, 0);
        return out.status == progression::PredicateStatus::evaluated
                   ? static_cast<std::int32_t>(out.value)
                   : -10 - static_cast<std::int32_t>(out.status);
    }
    if (std::string(kind) != "can") throw std::runtime_error("bad predicate kind");
    const auto* owner = save_state == 0 ? nullptr : &f.savegame;
    const auto out = progression::can_increment_skill(
        &f.properties, &f.tables, owner, kCharacter, 0);
    return out.status == progression::PredicateStatus::evaluated
               ? static_cast<std::int32_t>(out.value)
               : -10 - static_cast<std::int32_t>(out.status);
}

void test_predicates(std::uint32_t& cases) {
    Fixture f(10, 3, 7);
    auto out = progression::can_increment_skill(&f.properties, &f.tables,
                                                 &f.savegame, kCharacter, 0);
    require(out.status == progression::PredicateStatus::evaluated && out.value,
            "CanIncrement equality boundary differs");
    ++cases;
    require(f.savegame.set_skill_level(0, 8, f.error), "set level failed");
    out = progression::can_increment_skill(&f.properties, &f.tables,
                                           &f.savegame, kCharacter, 0);
    require(out.status == progression::PredicateStatus::evaluated && !out.value,
            "CanIncrement saved-level boundary differs");
    ++cases;

    Fixture negative(-2, -3, 0);
    out = progression::can_increment_skill(&negative.properties,
                                           &negative.tables,
                                           &negative.savegame, kCharacter, 0);
    require(out.status == progression::PredicateStatus::evaluated && out.value &&
                out.character_level == -2,
            "signed ASR8 or negative row semantics differ");
    ++cases;
    negative.resolved[19] = -257;
    negative.tables.skills[0].level = -1;
    out = progression::is_skill_available(&negative.properties,
                                          &negative.tables, kCharacter, 0);
    require(out.status == progression::PredicateStatus::evaluated && !out.value &&
                out.character_level == -2,
            "GetLevel arithmetic shift must floor negative fixed values");
    ++cases;

    Fixture wrap(0, INT32_MIN, 0);
    out = progression::can_increment_skill(&wrap.properties, &wrap.tables,
                                           &wrap.savegame, kCharacter, 0);
    require(out.status == progression::PredicateStatus::evaluated && !out.value,
            "CanIncrement subtraction must wrap as ARM32");
    ++cases;

    auto null_save = progression::can_increment_skill(
        nullptr, nullptr, nullptr, kCharacter, 999);
    require(null_save.status == progression::PredicateStatus::evaluated &&
                !null_save.value,
            "source null-save early return must precede skill/property reads");
    ++cases;
    auto no_rows = progression::can_increment_skill(
        nullptr, nullptr, &wrap.savegame, kCharacter, 999);
    require(no_rows.status == progression::PredicateStatus::evaluated &&
                !no_rows.value,
            "source null-row early return must precede skill/property reads");
    ++cases;
    const auto available = progression::is_skill_available(
        &f.properties, &f.tables, kCharacter, 0);
    require(available.status == progression::PredicateStatus::evaluated &&
                available.value,
            "IsSkillAvailable equality should be true");
    ++cases;
    const auto mismatch = progression::can_increment_skill(
        &f.properties, &f.tables, &f.savegame, kCharacter + 1, 0);
    require(mismatch.status == progression::PredicateStatus::owner_mismatch,
            "wrong savegame identity accepted");
    ++cases;
}

void test_increment(std::uint32_t& cases) {
    Fixture actor(10, 3, 7);
    Fixture replacement(10, 3, 9);
    Provider provider{};
    provider.properties = &actor.properties;
    provider.current = &actor.savegame;
    provider.replace_after_add = &replacement.savegame;
    auto services = provider.services();
    progression::Result result{};
    auto status = progression::increment_skill(
        &actor.properties, &actor.tables, kCharacter, 0, false, &services,
        &result);
    const std::vector<progression::Operation> expected{
        progression::Operation::skill_limit,
        progression::Operation::unlocked_difficulty,
        progression::Operation::unlocked_difficulty,
        progression::Operation::skill_limit,
        progression::Operation::add_property,
        progression::Operation::update_all_skills,
        progression::Operation::recalculate_properties,
        progression::Operation::set_potion_capacity,
        progression::Operation::debug_load,
        progression::Operation::debug_query};
    require(status == progression::Status::complete &&
                result.decision == progression::IncrementDecision::incremented &&
                result.source_return == 1 && provider.operations == expected,
            "IncSkill source call sequence/result mismatch");
    require(actor.savegame.skill_level(0) == 7 &&
                replacement.savegame.skill_level(0) == 10 &&
                result.old_saved_level == 9 && result.new_saved_level == 10,
            "IncSkill must reload the one live save owner after AddProperty");
    require(actor.resolved[157] == 256 && provider.potion_capacity == 1,
            "property/potion mutations missing");
    cases += 3;

    Fixture test_only(10, 3, 7);
    Provider dry{};
    dry.properties = &test_only.properties;
    dry.current = &test_only.savegame;
    auto dry_services = dry.services();
    status = progression::increment_skill(&test_only.properties,
                                          &test_only.tables, kCharacter, 0,
                                          true, &dry_services, &result);
    require(status == progression::Status::complete &&
                result.decision == progression::IncrementDecision::test_only_accepted &&
                test_only.savegame.skill_level(0) == 7 &&
                dry.operations.size() == 4,
            "test-only IncSkill path mutated or crossed source boundary");
    ++cases;

    Fixture no_points(10, 3, 7);
    no_points.resolved[157] = 0;
    Provider denied{};
    denied.properties = &no_points.properties;
    denied.current = &no_points.savegame;
    auto denied_services = denied.services();
    status = progression::increment_skill(&no_points.properties,
                                          &no_points.tables, kCharacter, 0,
                                          false, &denied_services, &result);
    require(status == progression::Status::complete &&
                result.decision == progression::IncrementDecision::no_skill_points &&
                denied.operations == std::vector<progression::Operation>{
                    progression::Operation::debug_load,
                    progression::Operation::debug_query},
            "no-points source debug path differs");
    ++cases;

    Fixture partial(10, 3, 7);
    Provider failure{};
    failure.properties = &partial.properties;
    failure.current = &partial.savegame;
    failure.fail_at = progression::Operation::update_all_skills;
    auto failure_services = failure.services();
    status = progression::increment_skill(&partial.properties, &partial.tables,
                                          kCharacter, 0, false,
                                          &failure_services, &result);
    require(status == progression::Status::service_failed &&
                partial.savegame.skill_level(0) == 8 &&
                partial.resolved[157] == 256,
            "later failure must retain completed source mutations");
    ++cases;
}

void test_init_slots(std::uint32_t& cases) {
    Fixture f(10, 3, 0);
    Provider p{};
    p.properties = &f.properties;
    p.current = &f.savegame;
    p.increment_return = 0;
    auto services = p.services();
    progression::InitSlotsResult result{};
    auto status = progression::initialize_skill_slots(kCharacter, &services,
                                                       &result);
    const std::vector<progression::Operation> expected{
        progression::Operation::has_skill_slots,
        progression::Operation::set_skill_in_slot,
        progression::Operation::swap_equipment_set,
        progression::Operation::set_skill_in_slot,
        progression::Operation::swap_equipment_set,
        progression::Operation::increment_skill};
    require(status == progression::Status::complete &&
                p.operations == expected && result.increment_called == 1,
            "_InitSkillsSlots source order differs");
    require(p.arguments[1][0] == 0 && p.arguments[1][1] == 0 &&
                p.arguments[3][0] == 0 && p.arguments[3][1] == 0 &&
                p.arguments[5][0] == 0 && p.arguments[5][1] == 0,
            "_InitSkillsSlots source row/slot arguments differ");
    cases += 2;

    Provider existing{};
    existing.properties = &f.properties;
    existing.current = &f.savegame;
    existing.has_slots = true;
    auto existing_services = existing.services();
    status = progression::initialize_skill_slots(kCharacter, &existing_services,
                                                  &result);
    require(status == progression::Status::complete &&
                existing.operations == std::vector<progression::Operation>{
                    progression::Operation::has_skill_slots},
            "existing source slots should return immediately");
    ++cases;

    Provider no_save_needed{};
    no_save_needed.has_slots = true;
    auto no_save_services = no_save_needed.services();
    no_save_services.current_savegame = nullptr;
    status = progression::initialize_skill_slots(kCharacter, &no_save_services,
                                                  &result);
    require(status == progression::Status::complete &&
                no_save_needed.operations == std::vector<progression::Operation>{
                    progression::Operation::has_skill_slots},
            "source HasSkillSlots early return must not require a later GetSkillLevel provider");
    ++cases;

    Provider failure{};
    failure.properties = &f.properties;
    failure.current = &f.savegame;
    failure.fail_at = progression::Operation::swap_equipment_set;
    auto failure_services = failure.services();
    status = progression::initialize_skill_slots(kCharacter, &failure_services,
                                                  &result);
    require(status == progression::Status::service_failed &&
                result.service_calls == 3 && failure.operations.size() == 3,
            "slot failure must preserve and report reached effects");
    ++cases;
}

void test_real_tables(const std::string& cache, std::uint32_t& cases) {
    if (cache.empty()) return;
    auto read = [](const std::string& path) {
        std::ifstream stream(path, std::ios::binary);
        if (!stream) throw std::runtime_error("missing source cache file");
        return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(stream), {});
    };
    const auto root = cache + "/data/pydata/";
    auto records = read(root + "skills_pyarray.bin");
    auto names = read(root + "skills_pyarraynames.bin");
    auto schema = read(root + "skills_pystructnames.bin");
    data::SkillTables tables;
    std::string error;
    require(data::load_skill_tables({records.data(), records.size()},
                                    {names.data(), names.size()},
                                    {schema.data(), schema.size()}, tables, error),
            "actual SkillTables load failed");
    require(tables.skills.size() == 127 && tables.skill_lists.size() == 36,
            "real SkillTable cache size changed");
    ++cases;
    std::size_t selector = 0;
    while (selector < tables.skill_lists.size() &&
           tables.skill_lists[selector].members.empty())
        ++selector;
    require(selector < tables.skill_lists.size(), "no nonempty source SkillList");
    const auto id = tables.skill_lists[selector].members.front();
    require(id >= 0 && static_cast<std::size_t>(id) < tables.skills.size(),
            "real SkillList references an invalid Skill row");
    ++cases;
    Fixture actual(0, tables.skills[static_cast<std::size_t>(id)].level, 0,
                   static_cast<std::int32_t>(selector), false);
    actual.tables = tables;
    actual.resolved[28] = static_cast<std::int32_t>(selector);
    actual.resolved[19] = signed_word(
        static_cast<std::uint32_t>(actual.tables.skills[static_cast<std::size_t>(id)].level)
        << 8);
    actual.savegame.set_character(kCharacter);
    require(actual.savegame.initialize_skills(actual.tables,
                                               static_cast<std::int32_t>(selector),
                                               error),
            "initialize real source saved rows");
    actual.properties.resolved = actual.resolved.data();
    const auto available = progression::is_skill_available(
        &actual.properties, &actual.tables, kCharacter, 0);
    const auto incrementable = progression::can_increment_skill(
        &actual.properties, &actual.tables, &actual.savegame, kCharacter, 0);
    require(available.status == progression::PredicateStatus::evaluated &&
                available.value &&
                incrementable.status == progression::PredicateStatus::evaluated &&
                incrementable.value,
            "real SkillList/Skill row projection differs");
    ++cases;
}

}  // namespace

int main(int argc, char** argv) {
    try {
        if (argc == 7 && std::string(argv[1]) == "predicate") {
            std::cout << predicate_case(argv[2], std::stoi(argv[3]),
                                        std::stoi(argv[4]), std::stoi(argv[5]),
                                        std::stoi(argv[6]))
                      << '\n';
            return 0;
        }
        std::uint32_t cases = 0;
        test_predicates(cases);
        test_increment(cases);
        test_init_slots(cases);
        test_real_tables(argc >= 2 ? argv[1] : "", cases);
        std::cout << "{\"validation\":\"PASS\",\"host_cases\":"
                  << cases << "}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "player skill progression: " << error.what() << '\n';
        return 1;
    }
}
