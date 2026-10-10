#include "runtime_skill_session_training_v1.hpp"

#include "../../playable_actor_world.hpp"
#include "../../../level-world/player_initial_grants_v2.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

namespace dh::foundation::generic_skills {
namespace {
using namespace dh2::player;

struct StagedTrainingV1 {
    const CharacterState* identity = nullptr;
    CharacterState* character = nullptr;
    OriginalCombatProperties* combat = nullptr;
    const OriginalPropertyDatabase* database = nullptr;
    const dh2::data::PropertyRules* rules = nullptr;
    dh2::data::SkillTables::Borrow skills;
    CharacterDesignSkillCapsV1 caps;
    std::vector<dh2::data::ClassRow> class_rows;
    std::uint8_t potion_capacity = 0;
    bool potion_capacity_written = false;
    std::string error;
};

std::int32_t signed_word(std::uint32_t bits) noexcept {
    std::int32_t value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

int invoke_staged(void* raw, const InitialGrantRequest32V2* request,
                  InitialGrantResponse8V2* response) noexcept {
    if (!raw || !request || !response) return -1;
    auto& staged = *static_cast<StagedTrainingV1*>(raw);
    *response = {};
    try {
        if (reinterpret_cast<std::uintptr_t>(staged.identity) != request->owner ||
            !staged.character || !staged.combat || !staged.database || !staged.rules) {
            staged.error = "Staged IncSkill provider lost its same-state/session owners";
            return -1;
        }
        const auto row = request->arguments[0];
        switch (request->operation) {
        case has_savegame:
            response->value = staged.character->source_skill_slots_known ? 1 : 0;
            return 0;
        case has_saved_rows:
            response->value = !staged.character->skills.empty() ? 1 : 0;
            return 0;
        case property_integer: {
            const auto property = request->arguments[0];
            if (property < 0 || property >= 224) {
                staged.error = "IncSkill property read is outside the source 224-word sheet";
                return -1;
            }
            response->value = staged.combat->sheets.resolved[static_cast<std::size_t>(property)] >> 8;
            return 0;
        }
        case skill_available: {
            if (!staged.skills || row < 0 ||
                static_cast<std::size_t>(row) >= staged.skills.skills().size()) {
                staged.error = "IncSkill source SkillTable row is unavailable";
                return -1;
            }
            const auto level = staged.combat->sheets.resolved[19] >> 8;
            const auto required = staged.skills.skills()[static_cast<std::size_t>(row)].scalar.words[8];
            response->value = level >= 0 && static_cast<std::uint32_t>(level) >= required;
            return 0;
        }
        case skill_limit: {
            const auto difficulty = request->arguments[0];
            if (difficulty < 0 || difficulty >= static_cast<std::int32_t>(staged.caps.max_skill_level.size()) ||
                staged.caps.max_skill_level[static_cast<std::size_t>(difficulty)] >
                    static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max())) {
                staged.error = "IncSkill source CharacterDesign cap is outside the supported integer range";
                return -1;
            }
            response->value = static_cast<std::int32_t>(
                staged.caps.max_skill_level[static_cast<std::size_t>(difficulty)]);
            return 0;
        }
        case difficulty_unlocked:
            response->value = static_cast<std::int32_t>(staged.caps.unlocked_difficulty);
            return 0;
        case saved_level_read:
            if (row < 0 || static_cast<std::size_t>(row) >= staged.character->skills.size() ||
                staged.character->skills[static_cast<std::size_t>(row)].rank > 65535u) {
                staged.error = "IncSkill saved rank is absent or exceeds its source uint16 row";
                return -1;
            }
            response->value = static_cast<std::int32_t>(
                staged.character->skills[static_cast<std::size_t>(row)].rank);
            return 0;
        case can_increment: {
            if (row < 0 || static_cast<std::size_t>(row) >= staged.character->skills.size() ||
                !staged.skills || static_cast<std::size_t>(row) >= staged.skills.skills().size()) {
                staged.error = "IncSkill CanIncrement row is outside same saved/source skill rows";
                return -1;
            }
            const auto level = staged.combat->sheets.resolved[19] >> 8;
            const auto required = staged.skills.skills()[static_cast<std::size_t>(row)].scalar.words[8];
            const auto difference_bits = static_cast<std::uint32_t>(level) - required;
            const auto difference = signed_word(difference_bits);
            response->value = static_cast<std::int32_t>(
                staged.character->skills[static_cast<std::size_t>(row)].rank) <= difference;
            return 0;
        }
        case property_add: {
            const auto property = request->arguments[0];
            const auto delta_bits = static_cast<std::uint32_t>(request->arguments[1]) << 8;
            const auto delta = signed_word(delta_bits);
            auto view = dh2::data::property_view(*staged.rules, staged.combat->sheets);
            if (dh2_property_add(&view, property, delta)) {
                staged.error = "IncSkill source property_add failed on staged property sheet";
                return -1;
            }
            return 0;
        }
        case saved_level_increment: {
            if (row < 0 || static_cast<std::size_t>(row) >= staged.character->skills.size()) {
                staged.error = "IncSkill saved-rank write is outside staged CharacterState rows";
                return -1;
            }
            auto& rank = staged.character->skills[static_cast<std::size_t>(row)].rank;
            rank = static_cast<std::uint16_t>(rank + 1u);
            return 0;
        }
        case update_all_skills:
            // The modern host has no per-Session derived PlayerSkills cache:
            // menus and casts resolve saved ranks from this shared state on
            // every query, and the class roots are already preloaded. The
            // source update has no additional modern-host cache to rebuild.
            return 0;
        case properties_recalculate: {
            auto view = dh2::data::property_view(*staged.rules, staged.combat->sheets);
            if (staged.class_rows.empty() ||
                dh2_class_recalc_base(staged.class_rows.data(),
                    static_cast<std::uint32_t>(staged.class_rows.size()),
                    staged.combat->sheets.base.data(), &view)) {
                staged.error = "IncSkill source class-sheet recalculation failed";
                return -1;
            }
            return 0;
        }
        case potion_capacity_store:
            staged.potion_capacity = static_cast<std::uint8_t>(request->arguments[0]);
            staged.potion_capacity_written = true;
            return 0;
        case debug_load:
        case debug_query:
            // These are the source's optional character-stats trace branch;
            // they have no gameplay effect and no modern host debug-file owner.
            response->value = 0;
            return 0;
        default:
            staged.error = "IncSkill requested an operation outside the recovered modern provider";
            return -1;
        }
    } catch (...) {
        try { staged.error = "IncSkill staged provider failed"; } catch (...) {}
        return -1;
    }
}

bool validate_same_owners(const CharacterState& state, CombatSession& session,
                          const OriginalPropertyDatabase& database,
                          dh2::data::SkillTables::Borrow skills,
                          const CharacterDesignSkillCapsV1& caps, int position,
                          const OriginalCombatProperties*& current,
                          PlayableActorWorld*& world,
                          std::string& error) {
    if (!skills) return fail(error, "Session skill training requires original SkillTables");
    if (!caps.known || caps.unlocked_difficulty >= caps.max_skill_level.size())
        return fail(error, "Session skill training requires actual unlocked CharacterDesign caps");
    world = session.world();
    if (!world || session.player_id() == invalid_actor_id)
        return fail(error, "Session skill training has no current player World owner");
    const auto* actor = world->find_actor(session.player_id());
    const auto* traits = world->traits(session.player_id());
    current = world->combat_properties(session.player_id());
    if (!actor || !traits || !traits->is_player || !current)
        return fail(error, "Session skill training requires its actual controlled player property owner");
    if (!actor->persistent_character_id || *actor->persistent_character_id != state.id ||
        actor->definition_id != state.class_id)
        return fail(error, "Session actor class/persistent identity differs from same CharacterState");
    if (!state.source_points_known || !state.source_skill_slots_known)
        return fail(error, "Session training requires source-known saved rows and Skill_Points");
    if (current->sheets.resolved[19] < 0 ||
        static_cast<std::uint32_t>(current->sheets.resolved[19] >> 8) != state.stats.level)
        return fail(error, "Session source level differs from same CharacterState");
    const auto point_raw = current->sheets.resolved[157];
    if (point_raw < 0 || (point_raw & 255) != 0 ||
        static_cast<std::uint32_t>(point_raw >> 8) != state.source_skill_points)
        return fail(error, "Session resolved property157 differs from same CharacterState points");
    const auto character = std::find(database.characters.names.begin(), database.characters.names.end(), state.class_id);
    const auto skill_tree = std::find(database.characters.fields.begin(), database.characters.fields.end(), "SkillTree");
    if (character == database.characters.names.end() || skill_tree == database.characters.fields.end())
        return fail(error, "Session training cannot resolve exact CharacterTable SkillTree");
    const auto character_row = static_cast<std::size_t>(character - database.characters.names.begin());
    const auto tree_column = static_cast<std::size_t>(skill_tree - database.characters.fields.begin());
    if (character_row >= database.characters.rows.size() || tree_column >= database.characters.rows[character_row].size())
        return fail(error, "Session training CharacterTable SkillTree row is absent");
    const auto source_list = database.characters.rows[character_row][tree_column];
    if (source_list < 0 || static_cast<std::size_t>(source_list) >= skills.lists().size() ||
        static_cast<std::size_t>(position) >= skills.lists()[static_cast<std::size_t>(source_list)].size())
        return fail(error, "Session training class position is outside its source SkillList");
    const auto& source_rows = skills.lists()[static_cast<std::size_t>(source_list)];
    if (state.skills.size() != source_rows.size())
        return fail(error, "Session training saved rows do not match original class SkillList size");
    for (std::size_t i = 0; i < source_rows.size(); ++i) {
        const auto skill_id = source_rows[i];
        if (skill_id < 0 || static_cast<std::size_t>(skill_id) >= skills.skill_names().size() ||
            state.skills[i].id != skills.skill_names()[static_cast<std::size_t>(skill_id)])
            return fail(error, "Session training saved row order differs from original class SkillList");
    }
    // SkillTree (property28) is an OID/index, not a Q8 quantity.
    if (current->sheets.resolved[28] != source_list)
        return fail(error, "Session property28 SkillTree differs from the same source CharacterTable list");
    SkillProgressionV1 progression;
    if (!evaluate_skill_progression_v1(state, database.characters, skills, caps,
                                       position, progression, error)) return false;
    if (progression.skill_list_id != source_list || progression.skill_table_id != source_rows[position] ||
        progression.saved_skill_row != position || !progression.rank_known || !progression.training_known)
        return fail(error, "Session source progression does not resolve one exact saved SkillList row");
    error.clear();
    return true;
}

} // namespace

bool train_skill_in_session_v1(
    CharacterState& same_state, CombatSession& same_session,
    const OriginalPropertyDatabase& source_properties,
    dh2::data::SkillTables::Borrow source_skills,
    const CharacterDesignSkillCapsV1& source_caps,
    int class_skill_position, SessionSkillTrainingCommitV1& output,
    std::string& error) {
    output = {};
    const OriginalCombatProperties* live = nullptr;
    PlayableActorWorld* world = nullptr;
    if (!validate_same_owners(same_state, same_session, source_properties, source_skills,
                              source_caps, class_skill_position, live, world, error)) return false;

    try {
        CharacterState staged_character = same_state;
        OriginalCombatProperties staged_combat = *live;
        dh2::data::PropertyRules rules;
        if (!dh2::data::load_property_rules(source_properties.characters, rules, error)) return false;
        StagedTrainingV1 staged;
        staged.identity = &same_state;
        staged.character = &staged_character;
        staged.combat = &staged_combat;
        staged.database = &source_properties;
        staged.rules = &rules;
        staged.skills = std::move(source_skills);
        staged.caps = source_caps;
        staged.class_rows.reserve(source_properties.classes.rows.size());
        for (const auto& row : source_properties.classes.rows)
            staged.class_rows.push_back({row.data(), static_cast<std::uint32_t>(row.size())});

        SkillProgressionV1 progression;
        if (!evaluate_skill_progression_v1(same_state, source_properties.characters, staged.skills,
                                           source_caps, class_skill_position, progression, error)) return false;
        if (!progression.can_train)
            return fail(error, "Original source skill training rejected current point/rank/level/cap gates");
        const auto previous_rank = staged_character.skills[static_cast<std::size_t>(progression.saved_skill_row)].rank;

        InitialGrantServices16V2 services{&staged, invoke_staged};
        std::int32_t accepted = 0;
        const auto rc = dh2_player_increment_skill_v2(&accepted,
            reinterpret_cast<std::uintptr_t>(&same_state), progression.saved_skill_row, 0, &services);
        if (rc || accepted != 1)
            return fail(error, staged.error.empty() ? "Original IncSkill kernel failed on staged owners" : staged.error.c_str());
        if (!staged.potion_capacity_written)
            return fail(error, "Original IncSkill did not produce its source potion-capacity side effect");
        const auto points_raw = staged_combat.sheets.resolved[157];
        if (points_raw < 0 || (points_raw & 255) != 0)
            return fail(error, "Staged source Skill_Points is not a nonnegative integral Q8 value");
        const auto expected_points = same_state.source_skill_points - 1u;
        const auto next_points = static_cast<std::uint32_t>(points_raw >> 8);
        const auto next_rank = staged_character.skills[static_cast<std::size_t>(progression.saved_skill_row)].rank;
        if (next_points != expected_points || next_rank != previous_rank + 1u)
            return fail(error, "Staged source IncSkill effects differ from one point and one saved rank");
        staged_character.source_skill_points = next_points;

        const auto* live_traits = world->traits(same_session.player_id());
        if (!live_traits)
            return fail(error, "Session player traits expired before source training publication");
        if (!world->update_combat_properties(same_session.player_id(), std::move(staged_combat),
                                             *live_traits, error)) return false;
        static_assert(noexcept(std::swap(std::declval<CharacterState&>(),
                                         std::declval<CharacterState&>())),
                      "Training publication requires a no-throw same-state swap");
        using std::swap;
        swap(same_state, staged_character);

        SessionSkillTrainingCommitV1 committed;
        committed.class_skill_position = class_skill_position;
        committed.saved_skill_row = progression.saved_skill_row;
        committed.previous_rank = previous_rank;
        committed.current_rank = next_rank;
        committed.remaining_points = next_points;
        committed.potion_capacity = staged.potion_capacity;
        output = committed;
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    } catch (...) {
        error = "Same-Session source skill training failed";
        return false;
    }
}

bool probe_skill_training_in_session_v1(
    const CharacterState& same_state, CombatSession& same_session,
    const OriginalPropertyDatabase& source_properties,
    dh2::data::SkillTables::Borrow source_skills,
    const CharacterDesignSkillCapsV1& source_caps,
    int class_skill_position, bool& accepted, std::string& error) {
    accepted = false;
    const OriginalCombatProperties* live = nullptr;
    PlayableActorWorld* world = nullptr;
    if (!validate_same_owners(same_state, same_session, source_properties, source_skills,
                              source_caps, class_skill_position, live, world, error)) return false;
    try {
        CharacterState staged_character = same_state;
        OriginalCombatProperties staged_combat = *live;
        dh2::data::PropertyRules rules;
        if (!dh2::data::load_property_rules(source_properties.characters, rules, error)) return false;
        StagedTrainingV1 staged;
        staged.identity = &same_state;
        staged.character = &staged_character;
        staged.combat = &staged_combat;
        staged.database = &source_properties;
        staged.rules = &rules;
        staged.skills = std::move(source_skills);
        staged.caps = source_caps;
        staged.class_rows.reserve(source_properties.classes.rows.size());
        for (const auto& row : source_properties.classes.rows)
            staged.class_rows.push_back({row.data(), static_cast<std::uint32_t>(row.size())});
        const auto* before_world = world->combat_properties(same_session.player_id());
        if (!before_world) return fail(error, "Session property sheet expired before IncSkill probe");
        const auto before_raw = before_world->sheets.resolved;
        InitialGrantServices16V2 services{&staged, invoke_staged};
        std::int32_t result = 0;
        const auto rc = dh2_player_increment_skill_v2(&result,
            reinterpret_cast<std::uintptr_t>(&same_state), class_skill_position, 1, &services);
        if (rc) return fail(error, staged.error.empty() ? "Original IncSkill probe failed on staged owners" : staged.error.c_str());
        if (staged_combat.sheets.resolved != before_raw || staged_character.skills.size() != same_state.skills.size())
            return fail(error, "Original IncSkill test probe unexpectedly changed staged source state");
        accepted = result != 0;
        error.clear();
        return true;
    } catch (const std::exception& exception) {
        error = exception.what();
        return false;
    } catch (...) {
        error = "Same-Session source skill probe failed";
        return false;
    }
}

} // namespace dh::foundation::generic_skills
