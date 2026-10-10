#include "stat_training_v1.hpp"

#include "../../../game-data/properties.hpp"
#include "../../../game-data/class_tables.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dh::foundation::character_menu {
namespace {

bool fail(std::string& error, std::string message) {
    error = std::move(message);
    return false;
}

// Property ids of the source CharacterProperties sheet used here.
constexpr unsigned level_property = 19;
constexpr unsigned stat_points_property = 148;
constexpr unsigned skill_points_property = 157;
constexpr unsigned first_stat_property = 149; // Strength, then Dexterity, Endurance, Energy
constexpr unsigned health_property = 36, health_max_property = 38;
constexpr unsigned resource_property = 41, resource_max_property = 43;

std::int32_t q8_integer(std::int32_t raw) noexcept { return raw >> 8; }

// Whole-point Q8 value as a float for CharacterState, with the same rule as
// the existing sync_character projection (negative sentinels clamp to zero).
float vital_value(std::int32_t raw) noexcept {
    return std::max(0.0f, static_cast<float>(raw) / 256.0f);
}

} // namespace

bool train_stat_in_session_v1(CharacterState& same_state, CombatSession& same_session,
    const OriginalPropertyDatabase& source_properties, std::uint32_t stat,
    const StatTrainingPersistV1& persist, StatTrainingCommitV1& output, std::string& error) {
    output = {};
    if (stat > 3) return fail(error, "Source stat training index must be Strength/Dexterity/Endurance/Energy (0..3)");
    if (!persist) return fail(error, "Source stat training requires an actual persistence owner");
    if (!same_state.source_points_known)
        return fail(error, "Source stat training requires CharacterState points to be known");
    auto* world = same_session.world();
    if (!world) return fail(error, "Source stat training requires the live combat world");
    const auto player = same_session.player_id();
    const OriginalCombatProperties* live = world->combat_properties(player);
    const PlayableActorTraits* traits = world->traits(player);
    ActorState* actor = same_session.actor(player);
    if (!live || !traits || !actor) return fail(error, "Source stat training player has no live property sheet");

    const auto& resolved = live->sheets.resolved;
    if (resolved[level_property] < 0 || q8_integer(resolved[level_property]) != static_cast<std::int32_t>(same_state.stats.level))
        return fail(error, "Session source level differs from same CharacterState");
    for (const auto property : {stat_points_property, skill_points_property}) {
        if (resolved[property] < 0 || (resolved[property] & 255) != 0)
            return fail(error, "Session source points are not nonnegative integral Q8 values");
    }
    if (static_cast<std::uint32_t>(q8_integer(resolved[stat_points_property])) != same_state.source_stat_points ||
        static_cast<std::uint32_t>(q8_integer(resolved[skill_points_property])) != same_state.source_skill_points)
        return fail(error, "Session resolved points differ from same CharacterState points");
    // Refused without any mutation: the original button is disabled at zero.
    if (q8_integer(resolved[stat_points_property]) <= 0)
        return fail(error, "Source stat training refused: no Stat_Points remain");

    const auto row = std::find(source_properties.characters.names.begin(),
                               source_properties.characters.names.end(), same_state.class_id);
    if (row == source_properties.characters.names.end())
        return fail(error, "Source stat training CharacterTable row is absent");
    const auto row_index = static_cast<std::size_t>(row - source_properties.characters.names.begin());
    if (row_index >= source_properties.characters.rows.size())
        return fail(error, "Source stat training CharacterTable row is outside the table");
    std::vector<dh2::data::ClassRow> class_rows;
    class_rows.reserve(source_properties.classes.rows.size());
    for (const auto& class_row : source_properties.classes.rows)
        class_rows.push_back({class_row.data(), static_cast<std::uint32_t>(class_row.size())});
    dh2::data::PropertyRules rules;
    if (!dh2::data::load_property_rules(source_properties.characters, rules, error)) return false;

    try {
        // Stage on copies so every failure leaves the Session and CharacterState untouched.
        OriginalCombatProperties staged = *live;
        const auto before = staged.sheets.resolved;
        {
            auto view = dh2::data::property_view(rules, staged.sheets);
            if (dh2_property_add(&view, stat_points_property, -256) ||
                dh2_property_add(&view, first_stat_property + stat, 256))
                return fail(error, "Source stat training property update failed");
        }
        // Source ResetBaseProperties/LoadBaseProperties: base comes from the
        // authored row; the current base level is kept (profile projection rule).
        const auto base_level = staged.sheets.base[level_property];
        staged.sheets.base = source_properties.characters.rows[row_index];
        staged.sheets.base[level_property] = base_level;
        {
            auto view = dh2::data::property_view(rules, staged.sheets);
            if (class_rows.empty() ||
                dh2_class_recalc_base(class_rows.data(), static_cast<std::uint32_t>(class_rows.size()),
                                      staged.sheets.base.data(), &view))
                return fail(error, "Source stat training class recalculation failed");
        }
        const auto& after = staged.sheets.resolved;
        if (after[stat_points_property] != before[stat_points_property] - 256 ||
            after[first_stat_property + stat] != before[first_stat_property + stat] + 256)
            return fail(error, "Staged source stat effects differ from one point on the chosen stat");

        // Live actor vitals: the sheet's maxima are published; the current
        // values stay at the actor's live values clamped to the new maxima.
        const float max_health = vital_value(after[health_max_property]);
        const float max_resource = vital_value(after[resource_max_property]);
        const float health = std::min(actor->health, max_health);
        const float resource = std::min(actor->resource, max_resource);

        CharacterState staged_character = same_state;
        staged_character.source_stat_points = static_cast<std::uint32_t>(q8_integer(after[stat_points_property]));
        staged_character.source_points_known = true;
        staged_character.source_endurance_energy_known = true;
        staged_character.stats.strength = vital_value(after[149]);
        staged_character.stats.dexterity = vital_value(after[150]);
        staged_character.stats.endurance = vital_value(after[151]);
        staged_character.stats.energy = vital_value(after[152]);
        staged_character.stats.health = health;
        staged_character.stats.max_health = max_health;
        staged_character.stats.resource = resource;
        staged_character.stats.max_resource = max_resource;

        // Persist first: a failed save leaves the live Session unchanged.
        if (!persist(staged_character, error)) {
            if (error.empty()) error = "Source stat training persistence failed";
            return false;
        }
        if (!world->update_combat_properties(player, std::move(staged), *traits, error)) return false;
        actor->max_health = max_health;
        actor->max_resource = max_resource;
        actor->health = health;
        actor->resource = resource;
        using std::swap;
        swap(same_state, staged_character);

        output.stat = stat;
        output.previous_points = static_cast<std::uint32_t>(q8_integer(before[stat_points_property]));
        output.remaining_points = same_state.source_stat_points;
        output.previous_value = q8_integer(before[first_stat_property + stat]);
        output.current_value = q8_integer(after[first_stat_property + stat]);
        output.previous_max_health = q8_integer(before[health_max_property]);
        output.current_max_health = q8_integer(after[health_max_property]);
        output.previous_max_resource = q8_integer(before[resource_max_property]);
        output.current_max_resource = q8_integer(after[resource_max_property]);
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    } catch (...) {
        error = "Source stat training failed";
        return false;
    }
}

int stats_training_button_at_v1(float authored_x, float authored_y) noexcept {
    // Measured from the Stats page frame (menu_CharacterSheetNew, Points left 3):
    // each wooden + button box, in authored space. The vertical extents match
    // the source btn_train_* deactivated overlay bounds (Strength 174.5, Dexterity
    // 219.9, Endurance 263.4, Energy 309.9), so the hit box is the drawn button.
    struct Region { int stat; float left, top, right, bottom; };
    static constexpr Region regions[] = {
        {0, 3.0f, 134.0f, 63.0f, 174.5f}, // Strength
        {1, 3.0f, 180.0f, 63.0f, 219.9f}, // Dexterity
        {2, 3.0f, 224.0f, 63.0f, 263.4f}, // Endurance
        {3, 3.0f, 270.0f, 63.0f, 309.9f}, // Energy
    };
    for (const auto& region : regions) {
        if (authored_x >= region.left && authored_x <= region.right &&
            authored_y >= region.top && authored_y <= region.bottom)
            return region.stat;
    }
    return -1;
}

bool register_stat_training_v1(SourceCompositionV1& composition,
    std::shared_ptr<void> owner, CharacterState& character,
    std::function<CombatSession*()> session, const OriginalPropertyDatabase& source_properties,
    StatTrainingPersistV1 persist, std::shared_ptr<StatTrainingVisitV1> visit,
    std::string& error) {
    if (!visit || !session || !persist) {
        error = "Stats training requires the visit gate, Session query and persistence owner";
        return false;
    }
    return composition.register_stat_training_callbacks(
        std::move(owner),
        [](float x, float y) { return stats_training_button_at_v1(x, y); },
        [&character, session = std::move(session), &source_properties,
         persist = std::move(persist), visit](std::uint32_t stat, std::string& message) {
            // Original guard: a second spend in the same visit does nothing.
            if (visit->added_this_turn) {
                message = "Stat point already assigned in this menu visit";
                return false;
            }
            CombatSession* live = session();
            if (!live) {
                message = "Stats training requires the current Session";
                return false;
            }
            StatTrainingCommitV1 commit;
            if (!train_stat_in_session_v1(character, *live, source_properties, stat, persist, commit, message))
                return false;
            visit->added_this_turn = true;
            std::cout << "Source stat training stat=" << commit.stat << " points="
                      << commit.previous_points << "->" << commit.remaining_points << " value="
                      << commit.previous_value << "->" << commit.current_value
                      << " maxHP=" << commit.previous_max_health << "->" << commit.current_max_health
                      << " maxMP=" << commit.previous_max_resource << "->" << commit.current_max_resource << "\n";
            message.clear();
            return true;
        },
        error);
}

} // namespace dh::foundation::character_menu
