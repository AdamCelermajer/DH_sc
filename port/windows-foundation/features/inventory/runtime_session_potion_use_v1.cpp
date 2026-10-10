#include "runtime_session_potion_use_v1.hpp"

#include "../../combat_session.hpp"
#include "../../playable_actor_world.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <type_traits>

namespace dh::foundation::inventory {
namespace {
bool q8(float value, std::int32_t& output) {
    const double raw = std::round(static_cast<double>(value) * 256.0);
    if (!std::isfinite(raw) || raw < INT32_MIN || raw > INT32_MAX) return false;
    output = static_cast<std::int32_t>(raw);
    return true;
}

std::int16_t signed_low16(std::uint32_t value) {
    const std::uint16_t low = static_cast<std::uint16_t>(value);
    std::int16_t result{};
    std::memcpy(&result, &low, sizeof result);
    return result;
}

bool synchronize_live_vital(dh2::data::PropertyState& state,
                            const dh2::data::PropertyRules& rules,
                            unsigned current_id, unsigned maximum_id,
                            float live, float live_max, std::string& error) {
    std::int32_t current_raw{}, max_raw{};
    if (!q8(live, current_raw) || !q8(live_max, max_raw) || current_raw < 0 ||
        max_raw <= 0 || state.resolved[maximum_id] != max_raw || current_raw > max_raw) {
        error = "Live actor vital does not match the source property maximum";
        return false;
    }
    const std::int64_t difference = std::int64_t(current_raw) - state.resolved[current_id];
    if (difference < INT32_MIN || difference > INT32_MAX) {
        error = "Live actor vital delta exceeds source signed range";
        return false;
    }
    auto view = dh2::data::property_view(rules, state);
    if (dh2_property_add(&view, current_id, static_cast<std::int32_t>(difference)) ||
        state.resolved[current_id] != current_raw) {
        error = "Source PropertyAdd could not synchronize live actor vital";
        return false;
    }
    error.clear();
    return true;
}

bool source_property_add(dh2::data::PropertyState& state,
                         const dh2::data::PropertyRules& rules,
                         unsigned property, std::int32_t amount) {
    auto view = dh2::data::property_view(rules, state);
    return dh2_property_add(&view, property, amount) == 0;
}

bool publish_prefix(dh::foundation::CombatSession& session, ActorId player,
                    dh::foundation::CharacterState& character,
                    dh::foundation::CharacterState&& candidate,
                    dh2::data::PropertyState&& source,
                    const dh2::data::PropertyRules& rules,
                    std::string& error) {
    auto* world = session.world();
    auto* actor = world ? world->find_actor(player) : nullptr;
    const auto* traits = world ? world->traits(player) : nullptr;
    if (!world || !actor || !traits) {
        error = "Same CombatSession player disappeared before potion publication";
        return false;
    }
    // The caller stages inventory before entering this publication point.
    // Vitals always reflect the source sheet prefix.
    candidate.stats.health = original_signed256(source.resolved[36]);
    candidate.stats.max_health = original_signed256(source.resolved[38]);
    candidate.stats.resource = original_signed256(source.resolved[41]);
    candidate.stats.max_resource = original_signed256(source.resolved[43]);
    const auto valid = validate_character_state(candidate);
    if (!valid.ok()) {
        error = valid.errors.front();
        return false;
    }
    const auto* current_properties = world->combat_properties(player);
    if (!current_properties) {
        error = "Same CombatSession source properties disappeared before potion publication";
        return false;
    }
    OriginalCombatProperties next = *current_properties;
    next.sheets = std::move(source);
    if (!world->update_combat_properties(player, std::move(next), *traits, error)) return false;
    actor = world->find_actor(player);
    if (!actor) {
        error = "Same CombatSession actor disappeared during potion publication";
        return false;
    }
    actor->health = candidate.stats.health;
    actor->max_health = candidate.stats.max_health;
    actor->resource = candidate.stats.resource;
    actor->max_resource = candidate.stats.max_resource;
    static_assert(std::is_nothrow_move_assignable<dh::foundation::CharacterState>::value,
                  "Potion publication needs a no-throw CharacterState move commit");
    character = std::move(candidate);
    (void)rules;
    error.clear();
    return true;
}
}

bool RuntimeSessionPotionUseV1::dispatch(
    dh::foundation::CombatSession& session, ActorId expected_player,
    dh::foundation::CharacterState& character,
    const dh::foundation::platform_input::ButtonEdges& input,
    const RuntimePotionUseServicesV1& services,
    RuntimePotionUseReceiptV1& output, std::string& error) const {
    error.clear();
    RuntimePotionUseReceiptV1 receipt;
    if (!input.pressed) {
        receipt.result = RuntimePotionUseResultV1::not_pressed;
        output = std::move(receipt);
        return true;
    }
    receipt.player = expected_player;
    auto* world = session.world();
    if (!world || expected_player == invalid_actor_id || session.player_id() != expected_player) {
        error = "Potion command requires the same live CombatSession player";
        return false;
    }
    auto* actor = world->find_actor(expected_player);
    const auto* traits = world->traits(expected_player);
    const auto* live = world->combat_properties(expected_player);
    if (!actor || !traits || !live || !traits->is_player ||
        !actor->persistent_character_id || *actor->persistent_character_id != character.id) {
        error = "Potion command requires the same player CharacterState and source binding";
        return false;
    }
    if (!actor->alive()) {
        receipt.result = RuntimePotionUseResultV1::player_inactive;
        output = std::move(receipt);
        return true;
    }
    if (!services.item_table || !services.property_rules) {
        error = "Potion command requires actual ItemTable and PropertyRules";
        return false;
    }
    const auto potion_id = dh2::data::item_id(*services.item_table, "Potion0");
    const auto* potion_data = dh2::data::item(*services.item_table, potion_id);
    if (!potion_data || potion_id < 0 || dh2::data::item_type(*potion_data) != 14) {
        error = "Actual Potion0 ItemTable row/type14 is unavailable";
        return false;
    }

    auto potion = character.inventory.end();
    for (auto it = character.inventory.begin(); it != character.inventory.end(); ++it) {
        const auto* row = dh2::data::item(*services.item_table,
            dh2::data::item_id(*services.item_table, it->definition_id));
        if (row && dh2::data::item_type(*row) == 14) {
            if (it->definition_id != "Potion0" || potion != character.inventory.end()) {
                error = "Source potion owner is ambiguous in CharacterState inventory";
                return false;
            }
            potion = it;
        }
    }
    if (potion == character.inventory.end()) {
        receipt.result = RuntimePotionUseResultV1::no_potion;
        output = std::move(receipt);
        return true;
    }
    const auto count = signed_low16(potion->quantity);
    if (count <= 0) {
        receipt.result = RuntimePotionUseResultV1::no_potion;
        receipt.potion_instance = potion->instance_id;
        receipt.quantity_before = count;
        output = std::move(receipt);
        return true;
    }
    const auto item_instance = potion->instance_id;
    for (const auto& binding : character.equipment) {
        if (binding.item_instance_id == item_instance) {
            error = "Source potion inventory instance cannot be equipped";
            return false;
        }
    }

    auto source = *live;
    if (!synchronize_live_vital(source.sheets, *services.property_rules,
                                36, 38, actor->health, actor->max_health, error) ||
        !synchronize_live_vital(source.sheets, *services.property_rules,
                                41, 43, actor->resource, actor->max_resource, error)) return false;
    receipt.potion_instance = item_instance;
    receipt.quantity_before = count;
    receipt.hp_before = source.sheets.resolved[36];
    receipt.mp_before = source.sheets.resolved[41];
    receipt.potion_count_property_before = source.sheets.resolved[219];
    const auto hp_max = source.sheets.resolved[38];
    const auto mp_max = source.sheets.resolved[43];
    if (hp_max <= 0 || mp_max <= 0) {
        error = "Source HP/MP maximum is unavailable for Potion0 use";
        return false;
    }
    const bool hp_below = float(source.sheets.resolved[36]) / float(hp_max) < 1.0f;
    const bool mp_below = float(source.sheets.resolved[41]) / float(mp_max) < 1.0f;
    if (!hp_below && !mp_below) {
        receipt.result = RuntimePotionUseResultV1::vitals_full;
        receipt.hp_after = receipt.hp_before;
        receipt.mp_after = receipt.mp_before;
        receipt.quantity_after = count;
        receipt.potion_count_property_after = receipt.potion_count_property_before;
        output = std::move(receipt);
        return true;
    }

    auto candidate = character;
    auto candidate_potion = std::find_if(candidate.inventory.begin(), candidate.inventory.end(),
        [&](const auto& item) { return item.instance_id == item_instance; });
    if (candidate_potion == candidate.inventory.end()) {
        error = "Same CharacterState potion instance changed during use";
        return false;
    }
    // RemoveOnePotion reads the signed16 count. SetPotionQty stores the new
    // value as uint16; zero clears the potion pointer and destroys the instance.
    if (count > 1) {
        candidate_potion->quantity = static_cast<std::uint32_t>(count - 1);
        receipt.quantity_after = count - 1;
    } else {
        candidate.inventory.erase(candidate_potion);
        receipt.quantity_after = 0;
    }
    receipt.consumed = true;

    if (!source_property_add(source.sheets, *services.property_rules, 219, 256)) {
        receipt.result = RuntimePotionUseResultV1::consumed_prefix_failure;
        receipt.potion_count_property_after = source.sheets.resolved[219];
        receipt.hp_after = source.sheets.resolved[36];
        receipt.mp_after = source.sheets.resolved[41];
        if (!publish_prefix(session, expected_player, character, std::move(candidate), std::move(source.sheets),
                            *services.property_rules, error)) return false;
        error = "Potion consumed; source Achievement_Potions_Count PropertyAdd failed";
        output = std::move(receipt);
        return false;
    }
    receipt.potion_count_property_after = source.sheets.resolved[219];
    if ((source.sheets.resolved[219] >> 8) > 99) {
        receipt.result = RuntimePotionUseResultV1::consumed_prefix_failure;
        receipt.hp_after = source.sheets.resolved[36];
        receipt.mp_after = source.sheets.resolved[41];
        if (!publish_prefix(session, expected_player, character, std::move(candidate), std::move(source.sheets),
                            *services.property_rules, error)) return false;
        error = "Potion consumed; original trophy producer is unavailable after source count99";
        output = std::move(receipt);
        return false;
    }

    auto property_view = dh2::data::property_view(*services.property_rules, source.sheets);
    if (dh2::character::skills::dh2_character_skill_regen_v6(
            &property_view, 0, -1, services.source_debug) != 0) {
        receipt.result = RuntimePotionUseResultV1::consumed_prefix_failure;
        receipt.hp_after = source.sheets.resolved[36];
        receipt.mp_after = source.sheets.resolved[41];
        if (!publish_prefix(session, expected_player, character, std::move(candidate), std::move(source.sheets),
                            *services.property_rules, error)) return false;
        error = "Potion consumed; original RegenHP failed after source prefix";
        output = std::move(receipt);
        return false;
    }
    receipt.health_regenerated = source.sheets.resolved[36] != receipt.hp_before;
    if (dh2::character::skills::dh2_character_skill_regen_v6(
            &property_view, 1, -1, services.source_debug) != 0) {
        receipt.result = RuntimePotionUseResultV1::consumed_prefix_failure;
        receipt.hp_after = source.sheets.resolved[36];
        receipt.mp_after = source.sheets.resolved[41];
        if (!publish_prefix(session, expected_player, character, std::move(candidate), std::move(source.sheets),
                            *services.property_rules, error)) return false;
        error = "Potion consumed; original RegenMP failed after source HP prefix";
        output = std::move(receipt);
        return false;
    }
    receipt.mana_regenerated = source.sheets.resolved[41] != receipt.mp_before;
    receipt.hp_after = source.sheets.resolved[36];
    receipt.mp_after = source.sheets.resolved[41];
    receipt.result = RuntimePotionUseResultV1::used;
    if (!publish_prefix(session, expected_player, character, std::move(candidate), std::move(source.sheets),
                        *services.property_rules, error)) return false;
    output = std::move(receipt);
    error.clear();
    return true;
}

} // namespace dh::foundation::inventory
