#include "../player_skill_property_services_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace p = dh2::player_skill_property_services_v1;
namespace d = dh2::data;
using F = dh2::character_native_bindings::Function;

namespace {
void check(bool value, const char* message) { if (!value) throw std::runtime_error(message); }

struct Fixture {
    d::PropertyRules rules{};
    d::PropertyState owner{};
    d::PropertySheet global_temp{};
    d::ClassTables classes;
    d::PropertyView view{};
    p::Bindings bindings{};
    Fixture() {
        for (std::size_t i = 0; i < 224; ++i) {
            rules.defaults[i] = 0;
            rules.types[i] = 0;
        }
        for (std::size_t i = 172; i <= 190; ++i) rules.types[i] = 8;
        owner.resolved[19] = 10 * 256;
        owner.resolved[79] = 2 * 256;
        owner.resolved[80] = 3 * 256;
        owner.resolved[172] = 1 * 256;
        owner.resolved[173] = 777;
        owner.resolved[174] = 888;
        owner.resolved[175] = 999;
        owner.resolved[190] = 1;
        classes.rows.resize(3);
        // Compact Bashdown-shaped fixture: child cleanup followed by the
        // skill formulas. It exercises source group order and explicit live
        // owner-property reads separately from the shared scratch sheet.
        classes.rows[1] = {
            {-1, 0, 2, -1, -1},
            {173, 1, 1536, 172, 768},
            {174, 1, 512, 79, 256},
            {175, 1, 1280, 80, 256},
            {190, 1, 256, 172, 12},
            {174, 5, 190, 0, 0},
            {175, 5, 190, 0, 0},
            {183, 1, 24320, 19, 0},
            {183, 1, -666, 172, 1280}
        };
        for (std::int32_t id = 173; id <= 190; ++id)
            if (id != 177 && id != 191) classes.rows[2].push_back({id, 1, 0, 19, 0});
        bindings.character = 0x1122334455667788ull;
        bindings.rules = &rules;
        bindings.classes = &classes;
        bindings.state = &owner;
        bindings.shared_temp = &global_temp;
        view = d::property_view(rules, owner);
        bindings.owner_view = &view;
    }
};

dh2_script_value num(float x) { dh2_script_value v{}; v.type = DH2_SCRIPT_NUMBER; v.number = x; return v; }
dh2_script_value boolean(bool x) { dh2_script_value v{}; v.type = DH2_SCRIPT_BOOLEAN; v.boolean = x; return v; }
dh2_script_value identity(std::uintptr_t x) { dh2_script_value v{}; v.type = DH2_SCRIPT_IDENTITY; v.identity = x; return v; }

int invoke(F f, p::Bindings& bindings, const dh2_script_value* args, std::uint32_t count,
           dh2_script_value* output, std::uint32_t capacity, std::uint32_t& returned) {
    char error[128]{};
    return p::invoke(&bindings, bindings.character, f, args, count, output, capacity,
                     &returned, error, sizeof(error));
}

std::vector<std::uint8_t> read(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    check(bool(file), "source data file missing");
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void actual_bashdown_data(const std::filesystem::path& data_dir) {
    using namespace dh2::data;
    const auto classes_raw = read(data_dir / "character_classes_pyarray.bin");
    const auto classes_names = read(data_dir / "character_classes_pyarraynames.bin");
    const auto classes_schema = read(data_dir / "character_classes_pystructnames.bin");
    const auto characters_raw = read(data_dir / "character_properties_pyarray.bin");
    const auto characters_names = read(data_dir / "character_properties_pyarraynames.bin");
    const auto characters_schema = read(data_dir / "character_properties_pystructnames.bin");
    ClassTables classes; std::string error;
    check(load_classes({classes_raw.data(), classes_raw.size()}, {classes_names.data(), classes_names.size()},
                       {classes_schema.data(), classes_schema.size()}, classes, error), "actual class table did not load");
    CharacterTable characters;
    check(load_characters({characters_raw.data(), characters_raw.size()}, {characters_names.data(), characters_names.size()},
                          {characters_schema.data(), characters_schema.size()}, characters, error), "actual Character table did not load");
    PropertyRules rules; check(load_property_rules(characters, rules, error), "actual property rules did not load");
    const auto found = std::find(classes.names.begin(), classes.names.end(), "Skill_Warrior_BashDown");
    check(found != classes.names.end(), "Bashdown class row absent from actual table");
    const auto class_id = static_cast<std::int32_t>(found - classes.names.begin());

    PropertyState owner{}; owner.base = owner.saved = owner.gear = owner.resolved = rules.defaults;
    owner.resolved[19] = 10 * 256; owner.resolved[79] = 2 * 256; owner.resolved[80] = 3 * 256;
    owner.resolved[172] = 1 * 256;
    PropertySheet shared_temp{};
    p::Bindings bindings{0x123456789abcdef0ull, &rules, &classes, &owner, &shared_temp, false};
    auto view = property_view(rules, owner);
    bindings.owner_view = &view;
    std::uint32_t returned = 88;
    const auto clear = boolean(true);
    check(invoke(F::character_clear_props, bindings, &clear, 1, nullptr, 0, returned) == 0 &&
          shared_temp == owner.resolved, "actual ClearProps owner snapshot differs");
    const dh2_script_value set[] = {num(172), num(3 * 256), boolean(true)};
    check(invoke(F::character_set_prop, bindings, set, 3, nullptr, 0, returned) == 0 &&
          owner.resolved[172] == 3 * 256 && shared_temp[172] == 1 * 256,
          "actual Bashdown SetProp(true) fallthrough differs");
    const dh2_script_value apply[] = {num(static_cast<float>(class_id)), boolean(true)};
    check(invoke(F::character_apply_prop_class, bindings, apply, 2, nullptr, 0, returned) == 0 &&
          shared_temp[173] == 1536 + 3 * 768 && shared_temp[174] != 0 && shared_temp[175] != 0,
          "actual Bashdown temp class formulas differ");
    const dh2_script_value get[] = {num(173), boolean(true)};
    dh2_script_value output{};
    check(invoke(F::character_get_prop, bindings, get, 2, &output, 1, returned) == 0 &&
          returned == 1 && output.number == static_cast<float>(1536 + 3 * 768),
          "actual Bashdown mana cost did not read shared temp");
    const dh2_script_value get_owner_level[] = {num(19)};
    check(invoke(F::character_get_prop, bindings, get_owner_level, 1, &output, 1, returned) == 0 &&
          returned == 1 && output.number == static_cast<float>(10 * 256),
          "actual ordinary owner property did not use source GetProp path");
}
}

int main(int argc, char** argv) {
    try {
        check(argc == 1 || argc == 2, "optional pydata directory expected");
        Fixture f;
        unsigned checks = 0;
        std::uint32_t returned = 77;
        dh2_script_value output{};

        // Source SetTempProps order: ClearProps(true) snapshots the owner to
        // the one caller-owned shared temporary sheet; SetProp(..., true)
        // falls through to the owner's raw type-8 property setter.
        auto clear_true = boolean(true);
        check(invoke(F::character_clear_props, f.bindings, &clear_true, 1, nullptr, 0, returned) == 0 &&
              returned == 0 && f.global_temp == f.owner.resolved, "ClearProps(true) did not seed shared sheet from owner"); ++checks;
        auto clear_false = boolean(false);
        auto before_temp = f.global_temp;
        check(invoke(F::character_clear_props, f.bindings, &clear_false, 1, nullptr, 0, returned) == 0 &&
              returned == 0 && f.global_temp == before_temp, "ClearProps(false) changed shared sheet"); ++checks;

        dh2_script_value set_args[] = {num(172), num(3 * 256), boolean(true)};
        check(invoke(F::character_set_prop, f.bindings, set_args, 3, nullptr, 0, returned) == 0 &&
              returned == 0 && f.owner.resolved[172] == 3 * 256 && f.global_temp[172] == 1 * 256,
              "SetProp bool true did not fall through to owner or unexpectedly rewrote snapshot"); ++checks;

        dh2_script_value apply_args[] = {num(1), boolean(true)};
        const int apply_result = invoke(F::character_apply_prop_class, f.bindings, apply_args, 2, nullptr, 0, returned);
        check(apply_result == 0 &&
              returned == 0 && f.owner.resolved[173] == 777 && f.global_temp[172] == 256 &&
              f.global_temp[173] == 3840 && f.global_temp[174] == 1168 &&
              f.global_temp[175] == 2336 && f.global_temp[190] == 292 && f.global_temp[183] == 28160,
              "ApplyPropClass(true) did not apply class formulas to shared sheet using live owner source"); ++checks;

        dh2_script_value get_temp_args[] = {num(173), boolean(true)};
        output = num(-1);
        check(invoke(F::character_get_prop, f.bindings, get_temp_args, 2, &output, 1, returned) == 0 &&
              returned == 1 && output.type == DH2_SCRIPT_NUMBER && output.number == 3840,
              "GetProp(...,true) did not read shared class result"); ++checks;
        dh2_script_value get_owner_args[] = {num(173), boolean(false)};
        output = num(-1);
        check(invoke(F::character_get_prop, f.bindings, get_owner_args, 2, &output, 1, returned) == 0 &&
              returned == 1 && output.number == 777, "GetProp(...,false) did not read owner"); ++checks;

        // Unlike SetProp's type-8 write gate, source _GetProp reads any
        // schema-valid property through _GetProperty.
        dh2_script_value get_general_owner[] = {num(19)};
        output = num(-1);
        check(invoke(F::character_get_prop, f.bindings, get_general_owner, 1, &output, 1, returned) == 0 &&
              returned == 1 && output.number == 2560,
              "GetProp without temp selector rejected an ordinary property"); ++checks;
        dh2_script_value get_general_temp[] = {num(19), boolean(true)};
        output = num(-1);
        check(invoke(F::character_get_prop, f.bindings, get_general_temp, 2, &output, 1, returned) == 0 &&
              returned == 1 && output.number == 2560,
              "GetProp temp selector rejected an ordinary property"); ++checks;

        // The Bashdown attack uses the two-argument/no-selector owner path.
        dh2_script_value attack_set[] = {num(172), num(4 * 256)};
        check(invoke(F::character_set_prop, f.bindings, attack_set, 2, nullptr, 0, returned) == 0 &&
              f.owner.resolved[172] == 1024, "attack SetProp owner path failed"); ++checks;
        dh2_script_value attack_apply = num(1);
        check(invoke(F::character_apply_prop_class, f.bindings, &attack_apply, 1, nullptr, 0, returned) == 0 &&
              f.owner.resolved[173] == 4608 && f.owner.resolved[174] == 1216 &&
              f.owner.resolved[175] == 2432 && f.owner.resolved[190] == 304 && f.owner.resolved[183] == 29440,
              "attack ApplyPropClass owner path did not use current level and class outputs"); ++checks;

        // Both wrapper bindings share the same external scratch storage; no
        // per-VM temporary copy is created by this service.
        p::Bindings second = f.bindings; second.character = 0x8877665544332211ull;
        dh2_script_value read_shared[] = {num(173), boolean(true)};
        output = num(-1);
        check(invoke(F::character_get_prop, second, read_shared, 2, &output, 1, returned) == 0 &&
              output.number == 3840, "separate VM context did not observe the caller-shared scratch sheet"); ++checks;

        // Unsupported external sheet identities fail closed without touching
        // source state, shared scratch, or a getter's result field.
        auto owner_before = f.owner; auto temp_before = f.global_temp;
        dh2_script_value external_set[] = {num(172), num(7 * 256), identity(0x1234)};
        check(invoke(F::character_set_prop, f.bindings, external_set, 3, nullptr, 0, returned) == -1001 &&
              returned == 0 && f.owner.resolved == owner_before.resolved && f.global_temp == temp_before,
              "unresolved external SetProp identity did not fail without writes"); ++checks;
        dh2_script_value external_get[] = {num(173), identity(0x1234)};
        output = num(1234);
        check(invoke(F::character_get_prop, f.bindings, external_get, 2, &output, 1, returned) == -1001 &&
              returned == 0 && output.number == 1234, "unresolved external GetProp changed result"); ++checks;
        dh2_script_value external_apply[] = {num(1), identity(0x1234)};
        check(invoke(F::character_apply_prop_class, f.bindings, external_apply, 2, nullptr, 0, returned) == -1001 &&
              f.owner.resolved == owner_before.resolved && f.global_temp == temp_before,
              "unresolved external ApplyPropClass changed source sheets"); ++checks;

        auto other_id = f.bindings.character + 1;
        check(p::invoke(&f.bindings, other_id, F::character_clear_props, &clear_true, 1, nullptr, 0,
                        &returned, nullptr, 0) == -1001 && f.global_temp == temp_before,
              "stale Character identity mutated global scratch"); ++checks;
        f.bindings.busy = true;
        check(invoke(F::character_clear_props, f.bindings, &clear_true, 1, nullptr, 0, returned) == -1001 &&
              f.global_temp == temp_before, "reentrant callback mutated global scratch");
        f.bindings.busy = false; ++checks;

        dh2_script_value bad_class[] = {num(99), boolean(true)};
        check(invoke(F::character_apply_prop_class, f.bindings, bad_class, 2, nullptr, 0, returned) == -1001 &&
              f.owner.resolved == owner_before.resolved && f.global_temp == temp_before,
              "unsupported class ID changed source sheets"); ++checks;
        dh2_script_value wrong_property = num(36);
        dh2_script_value wrong_set[] = {wrong_property, num(1)};
        check(invoke(F::character_set_prop, f.bindings, wrong_set, 2, nullptr, 0, returned) == -1001 &&
              f.owner.resolved == owner_before.resolved, "non-skill SetProp was accepted"); ++checks;

        dh2_script_value too_many_args[] = {num(172), num(1), boolean(false), num(4)};
        check(invoke(F::character_set_prop, f.bindings, too_many_args, 4, nullptr, 0, returned) == -1001 &&
              f.owner.resolved == owner_before.resolved && f.global_temp == temp_before,
              "oversized callback argument span was accepted or mutated state"); ++checks;

        // C ABI contract: even failed supported calls return zero results.
        check(returned == 0, "failed callback left nonzero result count"); ++checks;

        // Class destinations use raw sheet writes, independently of property
        // type. Ordinary linear reads resolve through the borrowed live view,
        // including attached buff groups, before the destination write.
        f.rules.types[60] = f.rules.types[63] = 4;
        f.owner.base[60] = 256; f.owner.saved[60] = 512; f.owner.gear[60] = 768;
        d::PropertySheet buff{}; buff[60] = 1024;
        const std::int32_t* sheets[] = {buff.data()};
        const d::PropertyBuffGroup group{sheets, 1};
        f.view.groups = &group; f.view.group_count = 1;
        f.owner.resolved[60] = -17;
        f.classes.rows[0] = {{63, 1, 1792, 60, 76}};
        const auto ordinary = num(0);
        check(invoke(F::character_apply_prop_class, f.bindings, &ordinary, 1, nullptr, 0, returned) == 0 &&
              f.owner.resolved[60] == 2560 && f.owner.resolved[63] == 2552,
              "ordinary class did not resolve canonical base/saved/gear/buff source"); ++checks;
        f.owner.resolved[60] = 512;
        dh2_script_value general_temp[] = {num(0), boolean(true)};
        check(invoke(F::character_apply_prop_class, f.bindings, general_temp, 2, nullptr, 0, returned) == 0 &&
              f.global_temp[63] == 1944 && f.owner.resolved[60] == 512,
              "temporary class recomputed cached source or rejected type4 destination"); ++checks;
        f.classes.rows[0] = {{63, 9, 91, 0, 0}, {174, 1, 1, 224, 1}};
        check(invoke(F::character_apply_prop_class, f.bindings, general_temp, 2, nullptr, 0, returned) == -1001 &&
              f.global_temp[63] == 91, "temporary class kernel failure rolled back delivered prefix"); ++checks;
        check(invoke(F::character_apply_prop_class, f.bindings, &ordinary, 1, nullptr, 0, returned) == -1001 &&
              f.owner.resolved[63] == 91, "ordinary class kernel failure rolled back delivered prefix"); ++checks;
        f.classes.rows[0] = {{224, 3, 0, 0, 0}, {224, 42, 0, 0, 0}, {63, 9, 72, 0, 0}, {-1, 8, 0, 0, 0}, {224, 1, 0, 224, 0}};
        check(invoke(F::character_apply_prop_class, f.bindings, general_temp, 2, nullptr, 0, returned) == 0 &&
              f.global_temp[63] == 72, "source ignored formulas or type8 class termination changed"); ++checks;
        auto missing_view = f.bindings; missing_view.owner_view = nullptr;
        before_temp = f.global_temp; owner_before = f.owner;
        check(invoke(F::character_apply_prop_class, missing_view, &ordinary, 1, nullptr, 0, returned) == -1001 &&
              f.owner.resolved == owner_before.resolved, "ordinary class accepted absent canonical property view"); ++checks;
        auto detached = f.view; detached.resolved = f.global_temp.data();
        auto wrong_view = f.bindings; wrong_view.owner_view = &detached;
        returned = 77;
        check(invoke(F::character_apply_prop_class, wrong_view, general_temp, 2, nullptr, 0, returned) == -1001 &&
              returned == 77 && f.global_temp == before_temp, "detached property view changed controls"); ++checks;
        const auto before_view = f.view;
        check(invoke(F::character_get_prop, f.bindings, get_owner_args, 2,
                     reinterpret_cast<dh2_script_value*>(&f.view), 1, returned) == -1001 &&
              !std::memcmp(&before_view, &f.view, sizeof(f.view)), "result aliased borrowed property view"); ++checks;
        if (argc == 2) { actual_bashdown_data(std::filesystem::path(argv[1])); ++checks; }
        std::cout << "player_skill_property_services_v1 PASS checks=" << checks
                  << " supported_callbacks=4 shared_temp=caller_owned class_destinations=all_schema_fields"
                  << " external_identity_sheet=unresolved buff_owner=unavailable"
                  << " actual_bashdown_data=" << (argc == 2 ? "PASS" : "not_requested") << '\n';
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "player_skill_property_services_v1 FAIL: " << e.what() << '\n';
        return 1;
    }
}
