#include "hotty_effects_v1.hpp"

#include <algorithm>
#include <limits>

namespace dh::foundation::faery_menu {
namespace {
bool find_set(const dh2::data::EffectsTables::Borrow& tables,
              const char* name, std::int32_t& id, std::string& error) {
    if (!tables) { error = "Hotty FX requires the original EffectsTables borrow"; return false; }
    const auto& names = tables.set_names();
    const auto& sets = tables.sets();
    if (names.size() != sets.size()) {
        error = "Hotty FX source set names and rows differ";
        return false;
    }
    const auto found = std::find(names.begin(), names.end(), name);
    if (found == names.end()) {
        error = std::string("Required original Hotty AnimatedEffectTable row is missing: ") + name;
        return false;
    }
    const auto index = static_cast<std::size_t>(found - names.begin());
    if (index > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()) ||
        sets[index].steps.empty()) {
        error = std::string("Original Hotty FX row has no playable source steps: ") + name;
        return false;
    }
    id = static_cast<std::int32_t>(index);
    error.clear();
    return true;
}
}

bool resolve_faery_effect_ids_v1(dh2::data::EffectsTables::Borrow tables,
                                 const FaeryAbilityV1& ability,
                                 HottyEffectIdsV1& output,
                                 std::string& error) {
    output = {};
    HottyEffectIdsV1 next;
    const std::string pre(ability.player_pre_fx), target(ability.target_fx);
    if (!find_set(tables, pre.c_str(), next.player_pre, error) ||
        !find_set(tables, target.c_str(), next.character_target, error))
        return false;
    next.player_pre_name = ability.player_pre_fx;
    next.character_target_name = ability.target_fx;
    output = next;
    error.clear();
    return true;
}

bool resolve_hotty_effect_ids_v1(dh2::data::EffectsTables::Borrow tables,
                                 HottyEffectIdsV1& output,
                                 std::string& error) {
    const auto* hotty = find_faery_ability_by_script_v1("faerie_hotty");
    if (!hotty) {
        output = {};
        error = "Hotty is missing from the faery ability registry";
        return false;
    }
    return resolve_faery_effect_ids_v1(tables, *hotty, output, error);
}
} // namespace dh::foundation::faery_menu
