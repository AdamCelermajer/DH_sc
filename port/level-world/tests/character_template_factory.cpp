#include "../character_template_factory.hpp"

#include <cstdio>
#include <string>
#include <vector>

namespace {
using namespace dh2::character::template_factory;

bool check(bool condition, const char* message) {
    if (condition) return true;
    std::fprintf(stderr, "FAIL: %s\n", message);
    return false;
}

Catalog crypt_catalog() {
    Catalog catalog;
    catalog.templates.push_back({"GothicusCrypt_Ghosts", {35, 35, 35, 35, 37}});
    catalog.characters.resize(448);
    catalog.characters[35] = {"Crypt_Ghost", 18};
    catalog.characters[37] = {"Crypt_Ghost_RE", 18};
    catalog.class_names.resize(260);
    catalog.class_names[18] = "BaseMonster";
    return catalog;
}

Source ambusher(const char* name) {
    return {name, "MonsterCommonType1", "Charater_Templates",
            "GothicusCrypt_Ghosts", ""};
}

}  // namespace

int main() {
    bool ok = true;
    const Catalog catalog = crypt_catalog();
    const Source source = ambusher("_prim_tmp_ambusher01");

    const auto pending = resolve(source, catalog);
    ok &= check(pending.status == Status::selection_required,
                "template resolution without RNG index must require selection");
    ok &= check(pending.authored_alternatives ==
                    std::vector<std::int32_t>({35, 35, 35, 35, 37}),
                "all five weighted slots, including duplicates, must be preserved");
    ok &= check(pending.property_id == -1 && pending.class_id == -1,
                "selection-required result must not partially resolve a property");

    const char* expected_names[] = {
        "Crypt_Ghost", "Crypt_Ghost", "Crypt_Ghost", "Crypt_Ghost", "Crypt_Ghost_RE"
    };
    const std::int32_t expected_ids[] = {35, 35, 35, 35, 37};
    for (std::int32_t slot = 0; slot < 5; ++slot) {
        const auto result = resolve(source, catalog, slot);
        ok &= check(result.status == Status::resolved, "selected slot must resolve");
        ok &= check(result.alternative_index == slot, "selected slot index must be retained");
        ok &= check(result.property_id == expected_ids[slot], "selected property ID changed");
        ok &= check(result.property_name == expected_names[slot], "selected property name changed");
        ok &= check(result.class_id == 18 && result.class_name == "BaseMonster",
                    "both ghost rows must project to BaseMonster class ID 18");
        ok &= check(result.class_formula_selected,
                    "a nonnegative source ClassID must be surfaced for downstream class logic");
        ok &= check(result.authored_alternatives == pending.authored_alternatives,
                    "successful projection must retain the authored alternative list");
    }

    ok &= check(resolve(source, catalog, 5).status == Status::alternative_out_of_range,
                "alternative bound must reject one-past-end index");
    ok &= check(resolve(source, catalog, -2).status == Status::invalid_source,
                "negative non-sentinel selection must be rejected");

    Source explicit_property{"explicit_ghost", "MonsterCommonType1", "", "", "Crypt_Ghost_RE"};
    const auto explicit_result = resolve(explicit_property, catalog);
    ok &= check(explicit_result.status == Status::resolved && explicit_result.explicit_property,
                "exact explicit property link must resolve despite editor template metadata");
    ok &= check(explicit_result.property_id == 37 &&
                    explicit_result.property_name == "Crypt_Ghost_RE" &&
                    explicit_result.class_id == 18,
                "explicit link must project the matching property and class");
    ok &= check(resolve(explicit_property, catalog, 0).status == Status::invalid_source,
                "explicit property path must not consume a template selection index");
    explicit_property.explicit_property_name = "missing-property";
    ok &= check(resolve(explicit_property, catalog).status == Status::property_not_found,
                "missing exact property name must not be reported as a bad numeric ID");
    explicit_property.explicit_property_name = "Crypt_Ghost";
    Catalog duplicate_property = catalog;
    duplicate_property.characters[40] = duplicate_property.characters[35];
    ok &= check(resolve(explicit_property, duplicate_property).status == Status::ambiguous_property,
                "duplicate exact property names must be rejected atomically");

    Source conflicting = source;
    conflicting.explicit_property_name = "Crypt_Ghost";
    ok &= check(resolve(conflicting, catalog).status == Status::conflicting_property_sources,
                "ambiguous simultaneous explicit/template routes must fail closed");
    Source wrong_data_class = source;
    wrong_data_class.template_data_class = "Character_Templates";
    ok &= check(resolve(wrong_data_class, catalog).status == Status::unsupported_template_data_class,
                "runtime pydata class comparison must be exact");
    Source missing_template = source;
    missing_template.template_name = "missing";
    ok &= check(resolve(missing_template, catalog).status == Status::template_not_found,
                "missing runtime template key must not fall back to editor template");

    Catalog ambiguous_template = catalog;
    ambiguous_template.templates.push_back(ambiguous_template.templates.front());
    ok &= check(resolve(source, ambiguous_template).status == Status::ambiguous_template,
                "duplicate runtime template keys must be rejected atomically");
    Catalog empty_template = catalog;
    empty_template.templates.front().property_ids.clear();
    ok &= check(resolve(source, empty_template).status == Status::template_has_no_alternatives,
                "empty authored variant list must be reported");
    Catalog bad_property = catalog;
    bad_property.templates.front().property_ids[4] = 448;
    ok &= check(resolve(source, bad_property, 4).status == Status::property_id_out_of_range,
                "invalid CharacterTable ID must be rejected");
    Catalog bad_class = catalog;
    bad_class.characters[37].class_id = 260;
    ok &= check(resolve(source, bad_class, 4).status == Status::invalid_class_id,
                "invalid ClassID must be rejected");
    Catalog no_class = catalog;
    no_class.characters[37].class_id = -1;
    const auto no_class_result = resolve(source, no_class, 4);
    ok &= check(no_class_result.status == Status::resolved && no_class_result.class_id == -1 &&
                    !no_class_result.class_formula_selected && no_class_result.class_name.empty(),
                "source ClassID -1 must remain the no-class-formula case");

    if (!ok) return 1;
    std::puts("character template factory host checks passed");
    return 0;
}
