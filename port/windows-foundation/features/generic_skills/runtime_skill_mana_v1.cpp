#include "runtime_skill_mana_v1.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <utility>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

std::int32_t to_fixed_rank(std::uint32_t rank) noexcept {
    // Original Lua ToFixed performs signed float-to-int conversion then a
    // wrapping 32-bit LSL8. Source saved skill levels are u16 values.
    const auto integral = static_cast<std::int32_t>(static_cast<float>(rank));
    const auto bits = static_cast<std::uint32_t>(integral) << 8;
    std::int32_t result{};
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}
} // namespace

bool evaluate_skill_mana_cost_v1(
    const dh2::data::ClassTables& classes,
    const dh2::data::PropertyRules& rules,
    const dh2::data::PropertySheet& current_resolved,
    const std::string& class_token, std::uint32_t rank,
    SkillManaCostV1& output, std::string& error) {
    if (rank > std::numeric_limits<std::uint16_t>::max()) {
        error = "Saved source skill rank exceeds the original u16 skill field";
        return false;
    }
    SkillTempPropertiesV1 temporary;
    if (!evaluate_skill_temp_properties_v1(classes, rules, current_resolved,
            class_token, rank, temporary, error)) return false;
    SkillManaCostV1 next;
    next.class_table_id = temporary.class_table_id;
    next.rank = temporary.rank;
    next.fixed_skill_level = temporary.fixed_skill_level;
    next.fixed_mana_cost = temporary.properties[173];
    output = next;
    error.clear();
    return true;
}

bool evaluate_skill_temp_properties_v1(
    const dh2::data::ClassTables& classes,
    const dh2::data::PropertyRules& rules,
    const dh2::data::PropertySheet& current_resolved,
    const std::string& class_token, std::uint32_t rank,
    SkillTempPropertiesV1& output, std::string& error) {
    error.clear();
    if (classes.names.size() != classes.rows.size() || classes.rows.empty())
        return fail(error, "Source ClassTables names/rows are unavailable or inconsistent");
    if (class_token.empty())
        return fail(error, "Source skill temp sheet requires the authored skill CLASS_ID token");
    // Saved ranks are u16, but NativeGetSkillDetails also calls OnSkillInfo
    // once at saved_rank+1. That second source call may legitimately be 65536.
    if (rank > static_cast<std::uint32_t>(std::numeric_limits<std::uint16_t>::max()) + 1u)
        return fail(error, "Source skill rank exceeds the NativeGetSkillDetails rank+1 domain");
    if (rules.defaults[172] != 0 || rules.defaults[173] != 0 ||
        rules.types[172] != 8 || rules.types[173] != 8)
        return fail(error, "Source property IDs 172/173 are not SnS_Level/SnS_ManaCost");

    const auto found = std::find(classes.names.begin(), classes.names.end(), class_token);
    if (found == classes.names.end())
        return fail(error, "Authored skill CLASS_ID token is absent from source ClassTables");
    const auto class_id = static_cast<std::int32_t>(found - classes.names.begin());

    // CalcManaCost delegates to SetTempProps. That routine resets the shared
    // temporary sheet to source defaults before setting level and evaluating
    // the authored class formulas. Formula type-1 buff reads receive the
    // actual same-character resolved-property snapshot.
    auto temporary = rules.defaults;
    const auto fixed_level = to_fixed_rank(rank);
    temporary[172] = fixed_level;
    if (!dh2::data::apply_class(classes, class_id, temporary, error,
                                &current_resolved)) {
        if (error.empty()) error = "Original skill property class evaluation failed";
        return false;
    }

    SkillTempPropertiesV1 next;
    next.class_table_id = class_id;
    next.rank = rank;
    next.fixed_skill_level = fixed_level;
    next.properties = temporary;
    output = next;
    error.clear();
    return true;
}

} // namespace dh::foundation::generic_skills
