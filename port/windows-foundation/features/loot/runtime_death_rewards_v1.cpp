#include "runtime_death_rewards_v1.hpp"

#include "../../../level-world/player_progression_v1.hpp"
#include "../../../game-data/properties.hpp"
#include "../../../game-data/loot_table_selection_v8.hpp"
#include "../../../game-data/loot_item_selection_v8.hpp"
#include "../../../game-data/loot_power_creation_v7.hpp"
#include "../../combat_session.hpp"
#include "../../playable_actor_world.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>

namespace dh::foundation::loot {
namespace {
std::int32_t asr8(std::int32_t x) noexcept {
    const auto bits = std::uint32_t(x);
    std::int32_t result;
    const auto shifted = (bits >> 8) | ((bits >> 31) ? 0xff000000u : 0u);
    std::memcpy(&result, &shifted, sizeof(result));
    return result;
}
std::int32_t wrap(std::uint32_t bits) noexcept {
    std::int32_t result;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}
float design_float(const dh2::data::DesignSettingsProjection176& design,
                   std::size_t offset) noexcept {
    float value{};
    std::memcpy(&value, &design.words[offset / 4], sizeof(value));
    return value;
}
bool fail(std::string& error, const char* text) {
    error = text;
    return false;
}
bool same_state(const std::shared_ptr<CharacterState>& a,
                const std::shared_ptr<CharacterState>& b) noexcept {
    return a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a);
}
bool select_death_items(const dh2::data::LootTablesV2::Borrow& tables,
                        const dh2::data::LootPowerResourcesV7::Borrow& powers,
                        dh2::data::LootRandom8V2& random,
                        dh2::data::LootEntryServicesV8 services,
                        std::int32_t table,
                        std::vector<dh2::data::LootItemInfoV8>& items,
                        std::string& error) {
    dh2::data::LootTableSelectionV8 table_selection(tables, powers, random, services);
    std::vector<const dh2::data::LootEntry32V2*> selected;
    if (!table_selection.select(table, selected, error)) return false;
    dh2::data::LootItemSelectionV8 item_selection(tables, random, services);
    return item_selection.expand(selected, false, items, error);
}
bool sync_character(CharacterState& character,
                    const dh2::data::PropertyState& properties,
                    std::string& error) {
    const auto level = asr8(properties.resolved[19]);
    const auto experience = asr8(properties.resolved[33]);
    const auto stat_points = asr8(properties.resolved[148]);
    const auto skill_points = asr8(properties.resolved[157]);
    if (level < 1 || experience < 0 || stat_points < 0 || skill_points < 0) {
        error = "Source progression produced a negative/unrepresentable CharacterState field";
        return false;
    }
    character.stats.level = static_cast<std::uint32_t>(level);
    character.experience = static_cast<std::uint64_t>(experience);
    character.source_stat_points = static_cast<std::uint32_t>(stat_points);
    character.source_skill_points = static_cast<std::uint32_t>(skill_points);
    // Vitals always project from the exact source candidate being published.
    // Before ordinary XP that candidate is synchronized from live ActorState;
    // on level-up the source recalculation/refill updates it first.
    character.stats.health = std::max(0.0f, ::dh::foundation::original_signed256(properties.resolved[36]));
    character.stats.max_health = std::max(0.0f, ::dh::foundation::original_signed256(properties.resolved[38]));
    character.stats.resource = std::max(0.0f, ::dh::foundation::original_signed256(properties.resolved[41]));
    character.stats.max_resource = std::max(0.0f, ::dh::foundation::original_signed256(properties.resolved[43]));
    return true;
}

bool source_q8(float value, std::int32_t& output, std::string& error) {
    if (!std::isfinite(value) || value < 0.0f) {
        error = "Live ActorState vital is negative or nonfinite";
        return false;
    }
    const double raw = std::round(static_cast<double>(value) * 256.0);
    if (!std::isfinite(raw) || raw < 0.0 || raw > double(INT32_MAX)) {
        error = "Live ActorState vital is outside source signed Q8 range";
        return false;
    }
    output = static_cast<std::int32_t>(raw);
    return true;
}

bool synchronize_live_vital(dh2::data::PropertyState& properties,
                            dh2::data::PropertyView& view,
                            unsigned current_id, unsigned maximum_id,
                            float current_value, float maximum_value,
                            std::string& error) {
    std::int32_t current_raw{}, actor_max_raw{};
    if (!source_q8(current_value, current_raw, error) ||
        !source_q8(maximum_value, actor_max_raw, error)) return false;
    const auto source_max_raw = properties.resolved[maximum_id];
    if (source_max_raw < 0 || actor_max_raw != source_max_raw || current_raw > source_max_raw) {
        error = "Live ActorState current/max vital differs from the same source property maximum";
        return false;
    }
    const auto delta64 = std::int64_t(current_raw) - std::int64_t(properties.resolved[current_id]);
    if (delta64 < INT32_MIN || delta64 > INT32_MAX) {
        error = "Live ActorState vital delta exceeds source signed Q8 range";
        return false;
    }
    if (dh2_property_add(&view, static_cast<std::int32_t>(current_id),
                         static_cast<std::int32_t>(delta64)) ||
        properties.resolved[current_id] != current_raw) {
        error = "Live ActorState vital could not be applied to its source property";
        return false;
    }
    return true;
}

bool synchronize_live_vitals(const ActorState& actor,
                             dh2::data::PropertyState& properties,
                             const dh2::data::PropertyRules& rules,
                             std::string& error) {
    auto view = dh2::data::property_view(rules, properties);
    return synchronize_live_vital(properties, view, 36, 38,
                                  actor.health, actor.max_health, error) &&
           synchronize_live_vital(properties, view, 41, 43,
                                  actor.resource, actor.max_resource, error);
}

bool level_up(::dh::foundation::PlayableActorWorld& world, ActorState& actor,
              OriginalCombatProperties& candidate,
              const OriginalPropertyDatabase& database,
              const dh2::data::PropertyRules& rules,
              CharacterState& character, std::int32_t max_level,
              std::string& error) {
    auto& props = candidate.sheets;
    auto view = dh2::data::property_view(rules, props);
    const auto before_level = asr8(props.resolved[19]);
    if (before_level >= max_level) return true;
    if (actor.definition_id.empty())
        return fail(error, "Source LevelUp requires the actual CharacterTable row name");
    const auto found = std::find(database.characters.names.begin(), database.characters.names.end(),
                                 actor.definition_id);
    if (found == database.characters.names.end())
        return fail(error, "Source LevelUp CharacterTable row is absent");
    if (dh2_property_add(&view, 19, 256) || dh2_property_set_int(&view, 33, 0))
        return fail(error, "Source LevelUp property prefix failed");
    props.base = database.characters.rows[std::size_t(found - database.characters.names.begin())];
    view = dh2::data::property_view(rules, props);
    std::vector<dh2::data::ClassRow> rows;
    rows.reserve(database.classes.rows.size());
    for (const auto& row : database.classes.rows)
        rows.push_back({row.data(), static_cast<std::uint32_t>(row.size())});
    if (rows.empty() || dh2_class_recalc_base(rows.data(), static_cast<std::uint32_t>(rows.size()),
                                              props.base.data(), &view))
        return fail(error, "Source LevelUp class recalculation failed");
    for (const auto current : {36, 41}) {
        const auto maximum = current == 36 ? 38 : 43;
        const auto delta = wrap(std::uint32_t(props.resolved[maximum]) -
                                std::uint32_t(props.resolved[current]));
        if (delta > 0 && dh2_property_add(&view, current, delta))
            return fail(error, "Source LevelUp full-vital property mutation failed");
    }
    const auto carry = asr8(wrap(std::uint32_t(props.resolved[33]) -
                                 std::uint32_t(props.resolved[34])));
    auto bounded_carry = carry;
    if (bounded_carry > asr8(props.resolved[34])) bounded_carry = asr8(props.resolved[34]) - 1;
    if (bounded_carry > 0 && dh2_property_add(&view, 33,
                                               wrap(std::uint32_t(bounded_carry) << 8)))
        return fail(error, "Source LevelUp XP carry mutation failed");
    if (props.resolved[33] > props.resolved[34] &&
        dh2_property_set(&view, 33, props.resolved[34]))
        return fail(error, "Source LevelUp XP post-clamp failed");
    std::string property_error;
    if (!dh2::data::recalc_properties(rules, props, property_error)) {
        error = "Source LevelUp final property resolve failed: " + property_error;
        return false;
    }
    if (!world.update_combat_properties(actor.id, candidate,
                                         *world.traits(actor.id), error)) return false;
    if (!sync_character(character, candidate.sheets, error)) return false;
    actor.health = character.stats.health;
    actor.max_health = character.stats.max_health;
    actor.resource = character.stats.resource;
    actor.max_resource = character.stats.max_resource;
    return true;
}

bool award_player_xp(::dh::foundation::PlayableActorWorld& world, ActorState& player,
                     std::shared_ptr<CharacterState> character,
                     const OriginalCombatProperties& original,
                     std::int32_t raw, const RuntimeDeathRewardServicesV1& services,
                     const dh2::data::PropertyRules& rules,
                     std::string& error) {
    if (!character) return fail(error, "Player XP recipient has no canonical shared CharacterState");
    OriginalCombatProperties candidate = original;
    auto& sheet = candidate.sheets;
    const auto source_level = asr8(sheet.resolved[19]);
    const auto source_xp = asr8(sheet.resolved[33]);
    if (source_level < 1 || source_xp < 0 || character->stats.level != std::uint32_t(source_level) ||
        character->experience != std::uint64_t(source_xp))
        return fail(error, "CharacterState XP/level differs from the same live source property sheet");
    if (source_level >= services.max_level) return true;
    if (!synchronize_live_vitals(player, sheet, rules, error)) return false;
    auto view = dh2::data::property_view(rules, sheet);
    bool one_kill_level_up = services.one_kill_level_up;
    if (services.query_one_kill_level_up &&
        !services.query_one_kill_level_up(services.context, one_kill_level_up, error))
        return false;
    if (one_kill_level_up)
        raw = wrap(std::uint32_t(sheet.resolved[34]) - std::uint32_t(sheet.resolved[33]));
    if (services.unlocked_difficulty < services.current_difficulty) raw = 256;
    const auto added = dh2::character::progression_modified_xp_v1(raw, sheet.resolved[201]);
    if (dh2_property_add(&view, 33, added)) return fail(error, "Source XP property Add failed");
    if (sheet.resolved[33] >= sheet.resolved[34]) {
        if (!level_up(world, player, candidate, *services.properties, rules,
                      *character, services.max_level, error)) return false;
        // Source LevelUp presentation runs only for a real level gain (the
        // cap branch returns before any state or FX change).
        if (character->stats.level > std::uint32_t(source_level) && services.on_level_up &&
            !services.on_level_up(services.context, player.id,
                                  std::int32_t(character->stats.level), error)) return false;
    } else {
        if (!world.update_combat_properties(player.id, candidate, *world.traits(player.id), error))
            return false;
        if (!sync_character(*character, candidate.sheets, error)) return false;
    }
    player.health = character->stats.health;
    player.max_health = character->stats.max_health;
    player.resource = character->stats.resource;
    player.max_resource = character->stats.max_resource;
    return true;
}

bool distribute_xp(dh::foundation::PlayableActorWorld& world, ActorState& victim,
                   ActorState* killer, const RuntimeDeathRewardServicesV1& services,
                   RuntimeDeathRewardOutcomeV1& outcome, std::string& error) {
    if (!services.properties || !services.xp_design)
        return fail(error, "Missing same-session original progression authorities");
    const auto* victim_props = world.combat_properties(victim.id);
    if (!victim_props) return fail(error, "Victim has no original resolved property sheet");
    std::vector<ActorState*> players;
    for (const auto& item : world.actors()) {
        const auto* traits = world.traits(item.first);
        if (traits && traits->is_player) players.push_back(world.find_actor(item.first));
    }
    if (players.size() > 4) return fail(error, "Source XP PlayerManager limit exceeds four actors");
    const auto victim_level = asr8(victim_props->sheets.resolved[19]);
    const auto base = std::max(0, asr8(victim_props->sheets.resolved[35]));
    const auto* origin = killer ? killer : &victim;
    float shares[4]{};
    std::uint32_t qualifying = 0;
    for (std::size_t i = 0; i < players.size(); ++i) {
        const auto* props = world.combat_properties(players[i]->id);
        if (!props) return fail(error, "XP recipient has no original resolved property sheet");
        const auto level = asr8(props->sheets.resolved[19]);
        shares[i] = dh2::character::progression_scaled_xp_v1(
            float(base), level, victim_level, *services.xp_design);
        if (shares[i] >= 0) {
            const auto dx = players[i]->transform.position[0] - origin->transform.position[0];
            const auto dy = players[i]->transform.position[1] - origin->transform.position[1];
            volatile float dx2 = dx * dx, dy2 = dy * dy;
            const float distance = std::sqrt(dx2 + dy2);
            if (players[i] == killer || design_float(*services.xp_design, 0xa0) >= distance)
                ++qualifying;
            else shares[i] = 0;
        }
    }
    if (!qualifying) return true;
    const float deduction = float(qualifying - 1) * design_float(*services.xp_design, 0xa4);
    dh2::data::PropertyRules rules;
    if (!dh2::data::load_property_rules(services.properties->characters, rules, error)) return false;
    for (std::size_t i = 0; i < players.size(); ++i) {
        const float scaled = ((100.f - deduction) * shares[i]) / 100.f;
        if (scaled < 0) continue;
        RuntimeDeathActorV1 resolved;
        if (!services.resolve_character(services.context, players[i]->id, resolved, error)) return false;
        if (!resolved.binding_lifecycle || !resolved.character) 
            return fail(error, "XP recipient requires actual actor lifetime and shared CharacterState");
        const auto* props = world.combat_properties(players[i]->id);
        if (!props) return fail(error, "XP recipient property owner expired");
        const auto raw = dh2::character::progression_award_raw_v1(scaled);
        if (!award_player_xp(world, *players[i], std::move(resolved.character), *props,
                             raw, services, rules, error)) return false;
        ++outcome.xp_recipients;
    }
    return true;
}
}

bool RuntimeDeathRewardsV1::consume(dh::foundation::CombatSession& session,
    const RuntimeDeathRewardServicesV1& services,
    std::vector<RuntimeDeathRewardOutcomeV1>& outcomes, std::string& error) {
    return consume_events(session, session.events(), services, outcomes, error);
}

bool RuntimeDeathRewardsV1::consume_events(dh::foundation::CombatSession& session,
    const std::vector<dh::foundation::DamageEvent>& events,
    const RuntimeDeathRewardServicesV1& services,
    std::vector<RuntimeDeathRewardOutcomeV1>& outcomes, std::string& error) {
    auto* world = session.world();
    if (!world) { outcomes.clear(); return fail(error, "CombatSession has no live actor world"); }
    return consume_events(*world, events, services, outcomes, error);
}

bool RuntimeDeathRewardsV1::consume_events(dh::foundation::PlayableActorWorld& world,
    const std::vector<dh::foundation::DamageEvent>& events,
    const RuntimeDeathRewardServicesV1& services,
    std::vector<RuntimeDeathRewardOutcomeV1>& outcomes, std::string& error) {
    outcomes.clear(); error.clear();
    const bool suppressed = services.admission.rewards_suppressed;
    if (!services.resolve_character || (!suppressed &&
        (!services.spawn_world_item || !services.xp_design || !services.properties ||
         services.current_difficulty < 0 || services.unlocked_difficulty < 0 ||
         services.max_level <= 0)))
        return fail(error, "Incomplete original death reward data providers");
    for (const auto& event : events) {
        if (!event.applied || !event.target_died) continue;
        auto* victim = world.find_actor(event.target);
        if (!victim || victim->alive()) return fail(error, "Death event victim is absent or no longer dead");
        const auto* victim_traits = world.traits(event.target);
        if (!victim_traits)
            return fail(error, "Death event victim has no source player classification");
        // Character::Kill has a separate player-death branch: unless forced,
        // it records the death and returns before DropLoot or DistributeXP.
        // The session adapter consumes generic DamageEvents, so preserve that
        // admission rule before resolving any reward owners or touching RNG.
        if (victim_traits->is_player) continue;
        RuntimeDeathActorV1 victim_binding;
        if (!services.resolve_character(services.context, event.target, victim_binding, error)) return false;
        if (!victim_binding.binding_lifecycle)
            return fail(error, "Death event has no source actor binding-lifecycle token");
        auto& receipt = receipts_[event.target];
        if (receipt.lifecycle && receipt.lifecycle != victim_binding.binding_lifecycle)
            receipt = {};
        if (receipt.lifecycle && !same_state(receipt.state_lease, victim_binding.character))
            return fail(error, "ActorId lifetime token reused with a different canonical CharacterState");
        RuntimeDeathRewardOutcomeV1 outcome; outcome.victim = event.target;
        if (receipt.state != RuntimeDeathRewardStateV1::none) {
            outcome.state = receipt.state;
            outcome.rewards_suppressed = receipt.rewards_suppressed;
            outcomes.push_back(outcome);
            if (receipt.state == RuntimeDeathRewardStateV1::failed)
                return fail(error, "Death reward lifetime already failed; source prefix cannot be retried");
            if (receipt.state == RuntimeDeathRewardStateV1::attempted)
                return fail(error, "Death reward lifetime is already being dispatched");
            continue;
        }
        receipt.lifecycle = victim_binding.binding_lifecycle;
        receipt.state_identity = victim_binding.character.get();
        receipt.state_lease = victim_binding.character;
        receipt.state = RuntimeDeathRewardStateV1::attempted;
        outcome.state = receipt.state;
        if (suppressed) {
            // The source Level gate suppresses both DropLoot and AddExperience.
            // Commit the once-only receipt before any killer/property/RNG work
            // so a later cleared gate cannot retroactively pay this death.
            receipt.state = RuntimeDeathRewardStateV1::completed;
            receipt.rewards_suppressed = true;
            outcome.state = receipt.state;
            outcome.rewards_suppressed = true;
            outcomes.push_back(outcome);
            continue;
        }
        ActorState* killer = event.attacker == invalid_actor_id ? nullptr : world.find_actor(event.attacker);
        std::int32_t killer_value_bonus256 = 0;
        bool killer_value_bonus_available = true;
        if (killer) {
            const auto* killer_properties = world.combat_properties(killer->id);
            if (!killer_properties) {
                killer_value_bonus_available = false;
                outcome.diagnostic = RuntimeDeathRewardDiagnosticV1::
                    killer_properties_unavailable_for_gold_value;
                outcome.gold_bonus_source = RuntimeGoldValueBonusSourceV1::unavailable;
            } else {
                // Character::DropLootTable resolves its killer again and
                // passes resolved word195 as LootCreationV8::value_bonus256.
                killer_value_bonus256 = killer_properties->sheets.resolved[195];
                outcome.gold_bonus_source = RuntimeGoldValueBonusSourceV1::
                    same_world_killer_property195;
            }
        } else {
            // CharacterLootDropV8::character(NULL/deleted) clears the borrow,
            // leaving its initialized value/power bonus pair at 0/0.
            outcome.gold_bonus_source = RuntimeGoldValueBonusSourceV1::
                absent_killer_zero_baseline;
        }
        const auto* victim_props = world.combat_properties(event.target);
        if (!victim_props) error = "Victim has no original resolved property sheet";
        else {
            const auto table = victim_props->sheets.resolved[9];
            if (table >= 0) {
                    if (!services.loot_tables || !services.loot_powers || !services.gameplay_rng ||
                    std::size_t(table) >= services.loot_tables.loots().size())
                    error = "Victim loot property does not name a loaded original Loot row";
                else {
                    std::vector<dh2::data::LootItemInfoV8> selected;
                    if (select_death_items(services.loot_tables, services.loot_powers, *services.gameplay_rng,
                                           services.loot_entry, table, selected, error)) {
                        outcome.selected_items = selected.size();
                        for (const auto& item : selected) {
                            RuntimeWorldItemRecordV1 record{event.target, event.attacker, table,
                                item.id, item.quantity, item.item, item.entry};
                            if (item.item && item.item->record.words[22] == 13) {
                                if (!killer_value_bonus_available) {
                                    // CalcLootItemValue consumes one range draw even
                                    // when its actual AddLoot bonus is unavailable.
                                    // Keep later source RNG consumers aligned, but
                                    // do not manufacture a value or publish this gold.
                                    std::int32_t discarded{};
                                    const auto low = item.item->record.words[27];
                                    const auto high = item.item->record.words[28];
                                    const auto range = std::uint32_t(high) + 1u - std::uint32_t(low);
                                    std::int32_t signed_range{};
                                    const auto range_bits = range;
                                    std::memcpy(&signed_range, &range_bits, sizeof(signed_range));
                                    if (dh2_loot_v2_random(services.gameplay_rng, signed_range,
                                                           &discarded)) {
                                        error = "Source GoldStack value RNG range is invalid";
                                        break;
                                    }
                                    ++outcome.unsupported_gold_items;
                                    continue;
                                }
                                std::int32_t value{};
                                if (dh2_loot_item_value_v7(&value, services.gameplay_rng,
                                        &item.item->record, nullptr, 0,
                                        killer_value_bonus256)) {
                                    error = "Source GoldStack value calculation failed";
                                    break;
                                }
                                record.resolved_gold_value = value;
                            }
                            if (!item.item || !item.entry ||
                                !services.spawn_world_item(services.context, record, *victim,
                                                           killer, error)) break;
                            ++outcome.spawned_items;
                        }
                    }
                }
            }
            if (error.empty() && distribute_xp(world, *victim, killer, services, outcome, error)) {
                receipt.state = RuntimeDeathRewardStateV1::completed;
                outcome.state = receipt.state;
                outcomes.push_back(outcome);
                continue;
            }
        }
        receipt.state = RuntimeDeathRewardStateV1::failed;
        outcome.state = receipt.state;
        outcomes.push_back(outcome);
        if (error.empty()) error = "Source death reward dispatch failed";
        return false;
    }
    return true;
}

} // namespace dh::foundation::loot
