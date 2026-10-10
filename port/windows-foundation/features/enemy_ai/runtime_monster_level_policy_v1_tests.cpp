#include "runtime_monster_level_policy_v1.hpp"
#include "../../asset_catalog.hpp"
#include "../../original_actor_properties.hpp"
#include "../../../game-data/level_tables.hpp"
#include "../../../level-world/character_level.hpp"
#include "../../../level-world/character_host_context.hpp"
#include "../../../script-runtime/script_constants.hpp"
#include "../../../script-runtime/script_runtime.h"
#include "../../../script-runtime/script_scalar_bindings.h"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace dh::foundation;
using namespace dh::foundation::enemy_ai;
using namespace dh2::character;

static void check(bool value, const std::string& error) {
    if (!value) throw std::runtime_error(error);
}

static std::vector<std::uint8_t> read_file(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) throw std::runtime_error("Missing test input: " + path.string());
    return {std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>()};
}

static std::int32_t signed_word(std::uint32_t bits) {
    std::int32_t value = 0;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static dh2_script_value source_number(float value) {
    dh2_script_value result{};
    result.type = DH2_SCRIPT_NUMBER;
    result.number = value;
    return result;
}

struct LevelCaseContext {
    const OriginalPropertyDatabase* database = nullptr;
    const dh2::data::LevelRecord* level = nullptr;
    dh2::data::PropertyState* properties = nullptr;
    dh2::data::PropertyView* view = nullptr;
    LevelModel32* model = nullptr;
    LevelServices16* services = nullptr;
    dh2::character::HostPlayer8 host_player{};
    dh2::character::HostLevel8 current_level{};
    const std::vector<dh2::character::LevelRangeRow24>* level_ranges = nullptr;
    unsigned set_level_calls = 0;
    float set_level_argument = 0.0f;
    unsigned random_calls = 0;
    unsigned on_init_calls = 0;
    unsigned table_registration_calls = 0;
};

static int lookup_constant(void* opaque, std::uint32_t kind, const char* group,
                           const char* name, std::int32_t* value) {
    return dh2_script_constants_lookup(opaque, kind, group, name, value);
}

static int source_get_struct(void* opaque, const dh2_script_value* arguments,
                             std::uint32_t count, dh2_script_value* results,
                             std::uint32_t capacity, std::uint32_t* returned,
                             char* error, std::size_t error_capacity) {
    auto& context = *static_cast<LevelCaseContext*>(opaque);
    if (!returned || !results || !capacity || !arguments || count != 2 ||
        arguments[0].type != DH2_SCRIPT_STRING || arguments[1].type != DH2_SCRIPT_STRING ||
        std::strcmp(arguments[0].text, "CharacterProperties")) {
        if (error && error_capacity) std::snprintf(error, error_capacity, "GetPyStruct fixture mismatch");
        return 1;
    }
    const auto& fields = context.database->characters.fields;
    const auto found = std::find(fields.begin(), fields.end(), arguments[1].text);
    if (found == fields.end()) return 1;
    results[0] = source_number(static_cast<float>(found - fields.begin()));
    *returned = 1;
    return 0;
}

static int source_get_oid(void* opaque, const dh2_script_value* arguments,
                          std::uint32_t count, dh2_script_value* results,
                          std::uint32_t capacity, std::uint32_t* returned,
                          char* error, std::size_t error_capacity) {
    auto& context = *static_cast<LevelCaseContext*>(opaque);
    if (!returned || !results || !capacity || !arguments || count != 2 ||
        arguments[0].type != DH2_SCRIPT_STRING || arguments[1].type != DH2_SCRIPT_STRING ||
        std::strcmp(arguments[0].text, "ClassTable")) {
        if (error && error_capacity) std::snprintf(error, error_capacity, "GetPyOID fixture mismatch");
        return 1;
    }
    const auto& names = context.database->classes.names;
    const auto found = std::find(names.begin(), names.end(), arguments[1].text);
    if (found == names.end()) return 1;
    results[0] = source_number(static_cast<float>(found - names.begin()));
    *returned = 1;
    return 0;
}

static int source_get_prop(void* opaque, const dh2_script_value* arguments,
                           std::uint32_t count, dh2_script_value* results,
                           std::uint32_t capacity, std::uint32_t* returned,
                           char* error, std::size_t error_capacity) {
    auto& context = *static_cast<LevelCaseContext*>(opaque);
    if (!returned || !results || !capacity || !arguments || count != 1 ||
        arguments[0].type != DH2_SCRIPT_NUMBER) {
        if (error && error_capacity) std::snprintf(error, error_capacity, "GetProp fixture mismatch");
        return 1;
    }
    const auto index = static_cast<std::int32_t>(arguments[0].number);
    if (index < 0 || index >= 224) return 1;
    results[0] = source_number(static_cast<float>(context.properties->resolved[static_cast<std::size_t>(index)]));
    *returned = 1;
    return 0;
}

static int source_position(void*, const dh2_script_value*, std::uint32_t,
                           dh2_script_value* results, std::uint32_t capacity,
                           std::uint32_t* returned, char*, std::size_t) {
    if (!results || capacity < 2 || !returned) return 1;
    results[0] = source_number(0.0f);
    results[1] = source_number(0.0f);
    *returned = 2;
    return 0;
}

static int source_host_owner(void* opaque, const dh2::character::HostContextRequest16* request,
                             dh2::character::HostContextResponse16* response) {
    auto& context = *static_cast<LevelCaseContext*>(opaque);
    using namespace dh2::character;
    if (!request || !response || request->reserved) return 1;
    if (request->service == host_get_player) {
        response->data = &context.host_player;
    } else if (request->service == host_get_current_level) {
        response->data = &context.current_level;
    } else if (request->service == host_get_range_rows) {
        if (!context.level_ranges || request->value < 0 ||
            static_cast<std::size_t>(request->value) >= context.level_ranges->size()) return 1;
        response->data = context.level_ranges->data();
        response->count = static_cast<std::uint32_t>(context.level_ranges->size());
    } else if (request->service != host_push_integer) {
        return 1;
    }
    return 0;
}

static int source_random(void* opaque, const dh2_script_value*, std::uint32_t,
                         dh2_script_value* results, std::uint32_t capacity,
                         std::uint32_t* returned, char*, std::size_t) {
    auto& context = *static_cast<LevelCaseContext*>(opaque);
    ++context.random_calls;
    if (!results || !capacity || !returned) return 1;
    results[0] = source_number(0.0f);
    *returned = 1;
    return 0;
}

static int source_on_init(void* opaque, const dh2_script_value*, std::uint32_t,
                          dh2_script_value*, std::uint32_t, std::uint32_t* returned,
                          char*, std::size_t) {
    ++static_cast<LevelCaseContext*>(opaque)->on_init_calls;
    *returned = 0;
    return 0;
}

static int source_add_to_vtable(void* opaque, const dh2_script_value*, std::uint32_t,
                                dh2_script_value*, std::uint32_t, std::uint32_t* returned,
                                char*, std::size_t) {
    ++static_cast<LevelCaseContext*>(opaque)->table_registration_calls;
    *returned = 0;
    return 0;
}

static int debug_service(void*, LevelModel32*, const LevelRequest24* request) {
    // This test provider represents successful delivery of the source Debug
    // load/query prefix. The query value is ignored by original SetLevel.
    return request && !request->reserved ? 0 : 1;
}

static int source_set_level(void* opaque, const dh2_script_value* arguments,
                            std::uint32_t count, dh2_script_value* results,
                            std::uint32_t capacity, std::uint32_t* returned,
                            char* error, std::size_t error_capacity) {
    auto& context = *static_cast<LevelCaseContext*>(opaque);
    if (!arguments || count != 1 || arguments[0].type != DH2_SCRIPT_NUMBER || !returned) {
        if (error && error_capacity) std::snprintf(error, error_capacity, "SetLevel fixture mismatch");
        return 1;
    }
    ++context.set_level_calls;
    context.set_level_argument = arguments[0].number;
    LevelBindings48 bindings{context.model, *context.services, {0, 0, 0}};
    const int status = dh2_character_set_level_lua(&bindings, arguments, count,
        results, capacity, returned, error, error_capacity);
    return status;
}

static int source_to_fixed(float value, float& fixed) {
    auto argument = source_number(value);
    dh2_script_value result{};
    std::uint32_t returned = 0;
    char error[128]{};
    const int status = dh2_script_scalar_to_fixed(nullptr, &argument, 1, &result, 1,
                                                  &returned, error, sizeof(error));
    if (status || returned != 1 || result.type != DH2_SCRIPT_NUMBER) return 1;
    fixed = result.number;
    return 0;
}

static float from_fixed_source_value(std::int32_t raw) {
    auto argument = source_number(static_cast<float>(raw));
    dh2_script_value result[2]{};
    std::uint32_t returned = 0;
    char error[128]{};
    check(dh2_script_scalar_from_fixed(nullptr, &argument, 1, result, 2,
                                       &returned, error, sizeof(error)) == 0 && returned == 2,
          "Actual FromFixed scalar kernel rejected a Character property");
    return result[0].number;
}

static RuntimeMonsterLevelDecisionV1 resolve_for_case(
    const OriginalActorProperties& actor, const OriginalPropertyDatabase& database,
    const dh2::data::LevelRecord& level, float host_level, float difficulty,
    bool sentinel, bool no_values) {
    const auto property = [&](const char* name) {
        const auto found = std::find(database.characters.fields.begin(), database.characters.fields.end(), name);
        check(found != database.characters.fields.end(), std::string("Missing Character field ") + name);
        const auto index = static_cast<std::size_t>(found - database.characters.fields.begin());
        return from_fixed_source_value(actor.sheets.resolved[index]);
    };
    RuntimeMonsterLevelInputsV1 input;
    input.source_reads_complete = true;
    input.monster_level_max = property("LevelMax");
    input.monster_level_min = property("LevelMin");
    input.monster_level_offset = property("LevelOffset");
    input.host_player_level = host_level;
    input.host_difficulty = difficulty;
    input.current_range_values_returned = !no_values;
    const int tier = static_cast<int>(difficulty);
    if (!no_values && tier >= 0 && tier <= 2) {
        input.current_range_min = sentinel ? -1.0f :
            static_cast<float>(signed_word(level.scalar.words[15 + static_cast<unsigned>(tier)]));
        input.current_range_max = sentinel ? -1.0f :
            static_cast<float>(signed_word(level.scalar.words[12 + static_cast<unsigned>(tier)]));
    }
    RuntimeMonsterLevelDecisionV1 decision;
    std::string error;
    check(resolve_runtime_monster_level_v1(input, decision, error), error);
    return decision;
}

static void execute_source_monster_init(LevelCaseContext& context,
                                        const std::filesystem::path& source_path) {
    auto* vm = dh2_script_vm_create_empty(8u * 1024u * 1024u);
    check(vm != nullptr, "Could not create source VM");
    check(dh2_script_vm_open_source_libraries(vm) == 0, "Could not open original Lua libraries");
    check(dh2_script_scalar_bind(vm, nullptr) == 0, "Could not bind actual source scalar kernels");
    const std::pair<const char*, dh2_script_function> bindings[] = {
        {"GetPyStruct", source_get_struct}, {"GetPyOID", source_get_oid},
        {"GetProp", source_get_prop}, {"GetPosition", source_position},
        {"GetRand", source_random},
        {"OnInit", source_on_init}, {"AddToVFTable", source_add_to_vtable},
        {"SetLevel", source_set_level}
    };
    for (const auto& binding : bindings)
        check(dh2_script_vm_bind_source_values(vm, binding.first, binding.second, &context) == 0,
              std::string("Could not bind source callback ") + binding.first);
    dh2::character::HostContextBindings16 host_bindings{{&context, &source_host_owner}};
    check(dh2_character_host_context_bind(vm, &host_bindings) == 0,
          "Could not bind actual source host-level/difficulty/range wrappers");
    const auto script = read_file(source_path);
    check(dh2_script_vm_load_source_file(vm, script.data(), script.size()) == 0,
          "Original monster.luac source failed to load");
    check(context.table_registration_calls > 0, "Original monster callback registrations were not executed");
    check(dh2_script_vm_call_discard_source(vm, "monster_OnInit", nullptr, 0) == 0,
          "Original monster_OnInit execution failed");
    dh2_script_vm_destroy(vm);
}

int main(int argc, char** argv) {
    try {
        check(argc == 2, "Supply repository root");
        const std::filesystem::path root(argv[1]);
        AssetCatalog source_assets(root / ".local-inputs/windows-shared-assets");
        OriginalPropertyDatabase database;
        std::string error;
        check(load_original_property_tables(source_assets, "original-cache/data/pydata",
                                             database, error), error);
        OriginalActorProperties source_lizard;
        check(resolve_original_actor_properties(database.characters, database.classes,
              "Swamp_LizadMan_Type1", {}, source_lizard, error), error);
        const auto lizard_row = std::find(database.characters.names.begin(),
                                         database.characters.names.end(),
                                         "Swamp_LizadMan_Type1");
        check(lizard_row != database.characters.names.end(), "Actual Swamp lizard Character row missing");

        const auto level_root = root / "port/android-native/app/src/main/assets/data";
        const auto level_records = read_file(level_root / "levels_pyarray.bin");
        const auto level_names = read_file(level_root / "levels_pyarraynames.bin");
        const auto level_schema = read_file(level_root / "levels_pystructnames.bin");
        dh2::data::LevelTables levels;
        check(dh2::data::load_levels({level_records.data(), level_records.size()},
              {level_names.data(), level_names.size()}, {level_schema.data(), level_schema.size()},
              levels, error), error);
        const auto swamp = std::find(levels.level_names.begin(), levels.level_names.end(), "SWAMP");
        check(swamp != levels.level_names.end(), "Actual LevelTable SWAMP row missing");
        const auto level_index = static_cast<std::size_t>(swamp - levels.level_names.begin());
        const auto& swamp_level = levels.levels[level_index];
        std::vector<dh2::character::LevelRangeRow24> level_ranges(levels.levels.size());
        for (std::size_t i = 0; i < levels.levels.size(); ++i)
            std::memcpy(&level_ranges[i], levels.levels[i].scalar.words + 12,
                        sizeof(dh2::character::LevelRangeRow24));

        auto* constants = dh2_script_constants_create();
        check(constants != nullptr, "Could not create original constant table");
        const auto design_bytes = read_file(level_root / "design_pycst.bin");
        dh2_script_constants_reload reload{};
        check(dh2_script_constants_load(constants, design_bytes.data(),
              static_cast<std::uint32_t>(design_bytes.size()), &reload) == 0,
              "Actual CharacterDesign constants failed to load");
        std::int32_t cap = -1;
        check(dh2_script_constants_get(constants, "CharacterDesign", "MaxLevelDVeryHard", &cap) == 0 && cap > 0,
              "Actual MaxLevelDVeryHard constant missing");

        dh2::data::PropertyRules rules;
        check(dh2::data::load_property_rules(database.characters, rules, error), error);
        std::vector<dh2::data::ClassRow> class_rows;
        class_rows.reserve(database.classes.rows.size());
        for (const auto& row : database.classes.rows)
            class_rows.push_back({row.data(), static_cast<std::uint32_t>(row.size())});
        dh2_script_design_bindings design{constants, &lookup_constant, 0};

        struct Case { float host_level; float difficulty; bool sentinel; bool no_values; };
        const Case cases[] = {
            {0.0f, 0.0f, false, false},
            {17.0f, 0.0f, false, false},
            {1000.0f, 0.0f, false, false},
            {17.0f, 1.0f, false, false},
            {1000.0f, 2.0f, false, false},
            {17.0f, 0.0f, true, false},
            {17.0f, 3.0f, false, true}
        };
        unsigned source_cases = 0;
        unsigned set_level_cases = 0;
        for (const auto& test_case : cases) {
            OriginalActorProperties actor;
            check(resolve_original_actor_properties(database.characters, database.classes,
                  "Swamp_LizadMan_Type1", {}, actor, error), error);
            auto view = dh2::data::property_view(rules, actor.sheets);
            LevelModel32 model{&view, class_rows.data(), static_cast<std::uint32_t>(class_rows.size()),
                               0, &design};
            LevelServices16 services{nullptr, &debug_service};
            LevelCaseContext context;
            context.database = &database;
            context.level = &swamp_level;
            context.properties = &actor.sheets;
            context.view = &view;
            context.model = &model;
            context.services = &services;
            context.host_player = {static_cast<std::int32_t>(test_case.host_level), 0};
            context.current_level = {test_case.sentinel ? -1 : static_cast<std::int32_t>(level_index),
                                     static_cast<std::int32_t>(test_case.difficulty)};
            context.level_ranges = &level_ranges;

            const auto decision = resolve_for_case(actor, database, swamp_level,
                test_case.host_level, test_case.difficulty,
                test_case.sentinel, test_case.no_values);
            execute_source_monster_init(context,
                root / ".local-inputs/character-script-owner-extension/monster.luac");
            check(context.on_init_calls == 1 && context.random_calls == 0,
                  "monster_OnInit changed source order or consumed a random draw");
            const auto property_index = [&](const char* name) {
                const auto found = std::find(database.characters.fields.begin(), database.characters.fields.end(), name);
                check(found != database.characters.fields.end(), std::string("Missing source property ") + name);
                return static_cast<std::size_t>(found - database.characters.fields.begin());
            };
            const auto source_level_max = from_fixed_source_value(actor.sheets.resolved[property_index("LevelMax")]);
            check(decision.calls_set_level == (source_level_max > -1.0f),
                  "LevelMax source gate did not control SetLevel reachability");
            check(context.set_level_calls == (decision.calls_set_level ? 1u : 0u),
                  "Actual monster.luac SetLevel call differs from the pure source resolver");
            if (decision.calls_set_level) {
                float expected_argument = 0.0f;
                check(source_to_fixed(decision.level_passed_to_to_fixed, expected_argument) == 0,
                      "Actual source ToFixed kernel failed");
                check(context.set_level_argument == expected_argument,
                      "Actual monster.luac ToFixed argument differs from the resolver output");
                const auto requested = static_cast<std::int32_t>(context.set_level_argument);
                const auto cap_raw = signed_word(static_cast<std::uint32_t>(cap) << 8);
                const auto expected_base19 = requested > cap_raw ? cap_raw : requested;
                check(actor.sheets.base[19] == expected_base19,
                      "Actual Character::SetLevel did not cap and assign source base property19");
                ++set_level_cases;
            }
            ++source_cases;
        }

        // Exercise the SetLevel cap beyond the dynamic Swamp range as a direct
        // source kernel composition. This verifies the existing Character owner
        // uses the loaded CharacterDesign cap and writes base property19.
        OriginalActorProperties cap_actor;
        check(resolve_original_actor_properties(database.characters, database.classes,
              "Swamp_LizadMan_Type1", {}, cap_actor, error), error);
        auto cap_view = dh2::data::property_view(rules, cap_actor.sheets);
        LevelModel32 cap_model{&cap_view, class_rows.data(), static_cast<std::uint32_t>(class_rows.size()),
                               0, &design};
        LevelServices16 cap_services{nullptr, &debug_service};
        float cap_argument = 0.0f;
        check(source_to_fixed(static_cast<float>(cap + 50), cap_argument) == 0,
              "Actual source ToFixed cap fixture failed");
        const auto lookups_before = 0u;
        check(dh2_character_set_level(&cap_model, cap_argument, &cap_services) == 0,
              "Actual Character::SetLevel cap call failed");
        check(cap_actor.sheets.base[19] == signed_word(static_cast<std::uint32_t>(cap) << 8),
              "Character::SetLevel failed the upper cap or base-property assignment");
        (void)lookups_before;

        const auto source_field = [&](const char* name) {
            const auto found = std::find(database.characters.fields.begin(), database.characters.fields.end(), name);
            check(found != database.characters.fields.end(), std::string("Missing source property ") + name);
            return from_fixed_source_value(source_lizard.sheets.resolved[
                static_cast<std::size_t>(found - database.characters.fields.begin())]);
        };
        std::cout << "PASS character=Swamp_LizadMan_Type1 characterRow="
                  << (lizard_row - database.characters.names.begin()) << " levelRow=SWAMP levelBounds=";
        for (unsigned tier = 0; tier < 3; ++tier) {
            if (tier) std::cout << ';';
            std::cout << signed_word(swamp_level.scalar.words[15 + tier]) << ','
                      << signed_word(swamp_level.scalar.words[12 + tier]);
        }
        std::cout << " authoredLevelMin=" << source_field("LevelMin")
                  << " authoredLevelMax=" << source_field("LevelMax")
                  << " levelOffset=" << source_field("LevelOffset")
                  << " difficultyTiers=0,1,2 sourceInitCases=" << source_cases
                  << " setLevelCalls=" << set_level_cases
                  << " dynamicRngDraws=0 maxCap=" << cap << " baseProperty19=true\n";
        dh2_script_constants_destroy(constants);
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "Runtime monster level policy test FAIL: " << exception.what() << '\n';
        return 1;
    }
}
