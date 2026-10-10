#include "runtime_skill_animation_bank_v1.hpp"

#include "../../../game-data/animation_tables.hpp"
#include "../../../game-data/skill_tables.hpp"
#include "../skills_animation/skill_animation_program.hpp"

#include <algorithm>
#include <cstring>
#include <set>
#include <utility>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

std::int32_t signed_word(std::uint32_t raw) noexcept {
    std::int32_t value{};
    std::memcpy(&value, &raw, sizeof(value));
    return value;
}

bool resolve_active_source_skill_list(
    const CharacterState& character, const dh2::data::CharacterTable& characters,
    const dh2::data::SkillTables::Borrow& skills, int& class_row,
    int& active_list, std::string& error) {
    const auto class_at = std::find(characters.names.begin(), characters.names.end(),
                                    character.class_id);
    if (class_at == characters.names.end())
        return fail(error, "Animation bank CharacterState class is absent from original CharacterTable");
    class_row = static_cast<int>(class_at - characters.names.begin());
    const auto tree_at = std::find(characters.fields.begin(), characters.fields.end(), "SkillTree");
    if (tree_at == characters.fields.end())
        return fail(error, "Original CharacterTable has no SkillTree field for animation-bank preload");
    const auto column = static_cast<std::size_t>(tree_at - characters.fields.begin());
    if (static_cast<std::size_t>(class_row) >= characters.rows.size() ||
        column >= characters.rows[static_cast<std::size_t>(class_row)].size())
        return fail(error, "Original CharacterTable class row has no SkillTree for animation-bank preload");
    const int authored_list = characters.rows[static_cast<std::size_t>(class_row)][column];
    if (authored_list < 0 || static_cast<std::size_t>(authored_list) >= skills.lists().size())
        return fail(error, "Original CharacterTable.SkillTree references an absent SkillList");

    auto matches_saved_rows = [&](const std::vector<std::int32_t>& list) {
        if (list.size() != character.skills.size()) return false;
        for (std::size_t i = 0; i < list.size(); ++i) {
            const auto table_id = list[i];
            if (table_id < 0 || static_cast<std::size_t>(table_id) >= skills.skill_names().size() ||
                character.skills[i].id != skills.skill_names()[static_cast<std::size_t>(table_id)])
                return false;
        }
        return true;
    };
    active_list = authored_list;
    bool saved_rows_match = false;
    if (!character.skills.empty()) {
        for (std::size_t i = 0; i < skills.lists().size(); ++i) {
            if (!matches_saved_rows(skills.lists()[i])) continue;
            active_list = static_cast<int>(i);
            saved_rows_match = true;
            if (active_list == authored_list) break;
        }
        if (character.source_skill_slots_known && !saved_rows_match)
            return fail(error, "Source-known saved skills do not match any original SkillList for animation-bank preload");
    }
    error.clear();
    return true;
}

bool same_plan_sequences(const OriginalCombatVisualPlan& base,
                         const skills_animation::SkillAnimationPrograms& bank,
                         std::string& error) {
    for (const auto& sequence : bank.plan.sequences) {
        if (std::find(base.stateNames.begin(), base.stateNames.end(), sequence.state) !=
            base.stateNames.end()) {
            error = "Source skill animation state collides with the preloaded actor plan";
            return false;
        }
        for (const auto& existing : base.sequences) {
            if (existing.id == sequence.id && existing.state != sequence.state) {
                error = "Source skill AnimTable root collides with a differently named actor sequence";
                return false;
            }
        }
    }
    error.clear();
    return true;
}
} // namespace

bool build_runtime_skill_animation_bank_v1(
    const RuntimeSkillAnimationBankRequestV1& request,
    const OriginalCombatVisualPlan& base_plan,
    RuntimeSkillAnimationBankV1& output, std::string& error) {
    error.clear();
    if (!request.character || !request.characters || !request.skills ||
        !request.assets || !request.animations || !request.animation_dictionary ||
        !request.same_actor_visual || request.actor_role.empty())
        return fail(error, "Skill animation bank requires the same character, original tables, asset catalog and actor visual");
    if (request.character->id.empty() || request.character->class_id.empty() ||
        !request.character->source_skill_slots_known)
        return fail(error, "Skill animation bank requires source-known saved hotbar assignments");

    RuntimeSkillAnimationBankV1 next;
    next.character_state_id = request.character->id;
    next.class_id = request.character->class_id;
    int preloaded_class_row = -1;
    int preloaded_active_list = -1;
    if (request.preload_current_class_roots &&
        !resolve_active_source_skill_list(*request.character, *request.characters,
                request.skills, preloaded_class_row, preloaded_active_list, error))
        return false;

    std::vector<std::int32_t> roots;
    std::set<std::int32_t> unique_roots;
    for (std::uint32_t slot = 0; slot < next.hotbar.size(); ++slot) {
        const SkillSlotBinding* binding = nullptr;
        for (const auto& candidate : request.character->skill_slots) {
            if (candidate.equipment_set != request.equipment_set || candidate.slot != slot) continue;
            if (binding) return fail(error, "Source skill animation bank found duplicate saved hotbar bindings");
            binding = &candidate;
        }
        if (!binding) continue;
        if (binding->saved_skill_row >= request.character->skills.size())
            return fail(error, "Source skill animation hotbar points outside the saved skill rows");

        // A known saved assignment may point at a current-class row that is
        // still rank zero during direct/pre-grant initialization. That row is
        // not an active learned hotbar cast yet, but its authentic class-list
        // root can still be preloaded below. Validate the mapping against the
        // exact active source list before omitting it from the learned-slot
        // projection; never synthesize a rank or carry this as bank.hotbar.
        const auto& saved = request.character->skills[binding->saved_skill_row];
        if (request.preload_current_class_roots && saved.rank == 0) {
            const auto& class_list = request.skills.lists()[
                static_cast<std::size_t>(preloaded_active_list)];
            const auto row = static_cast<std::size_t>(binding->saved_skill_row);
            if (row >= class_list.size() || class_list[row] < 0 ||
                static_cast<std::size_t>(class_list[row]) >= request.skills.skill_names().size() ||
                saved.id != request.skills.skill_names()[
                    static_cast<std::size_t>(class_list[row])])
                return fail(error, "Rank-zero saved hotbar row does not match its active original SkillList position");
            continue;
        }

        int position = -1;
        std::uint32_t saved_row = 0;
        if (!resolve_assigned_skill_position_v1(*request.character, request.skills,
                request.equipment_set, slot, position, saved_row, error)) return false;
        if (saved_row != binding->saved_skill_row)
            return fail(error, "Source hotbar row changed while resolving the preloaded animation bank");
        SkillVisualRequestV1 visual;
        if (!resolve_skill_visual_request_v1(*request.character, *request.characters,
                request.skills, position, visual, error)) return false;
        if (visual.saved_skill_row != static_cast<int>(saved_row) ||
            visual.animation_sequence_id < 0)
            return fail(error, "Saved hotbar row has no exact source SkillTable animation root");
        RuntimeSkillAnimationSlotV1 entry;
        entry.equipment_set = request.equipment_set;
        entry.source_slot = slot;
        entry.skill = std::move(visual);
        entry.selection_state = skills_animation::skill_sequence_state(
            entry.skill.animation_sequence_id);
        next.hotbar[slot] = std::move(entry);
        if (unique_roots.insert(next.hotbar[slot]->skill.animation_sequence_id).second)
            roots.push_back(next.hotbar[slot]->skill.animation_sequence_id);
    }

    if (request.preload_current_class_roots) {
        next.class_roots_preloaded = true;
        next.active_skill_list_id = preloaded_active_list;
        const auto& class_list = request.skills.lists()[
            static_cast<std::size_t>(preloaded_active_list)];
        for (std::size_t position = 0; position < class_list.size(); ++position) {
            RuntimeSkillAnimationClassRootV1 entry;
            entry.active_skill_list_id = preloaded_active_list;
            entry.class_skill_position = static_cast<int>(position);
            entry.skill_table_id = class_list[position];
            const auto table_id = class_list[position];
            if (table_id < 0 || static_cast<std::size_t>(table_id) >= request.skills.skills().size() ||
                static_cast<std::size_t>(table_id) >= request.skills.skill_names().size()) {
                entry.diagnostic = "source SkillList row has an invalid SkillTable id";
                next.class_skill_roots.push_back(std::move(entry));
                continue;
            }
            entry.source_skill_name = request.skills.skill_names()[static_cast<std::size_t>(table_id)];
            const auto& skill = request.skills.skills()[static_cast<std::size_t>(table_id)];
            entry.source_script = skill.script;
            entry.animation_sequence_id = signed_word(skill.scalar.words[1]);
            if (entry.animation_sequence_id < 0) {
                entry.diagnostic = "source SkillTable.Anim root is negative; no animation is authored";
                next.class_skill_roots.push_back(std::move(entry));
                continue;
            }
            entry.selection_state = skills_animation::skill_sequence_state(
                entry.animation_sequence_id);
            if (static_cast<std::size_t>(entry.animation_sequence_id) >= request.animations->sequences.size()) {
                entry.diagnostic = "source SkillTable.Anim root is outside AnimationTables";
                next.class_skill_roots.push_back(std::move(entry));
                continue;
            }
            if (unique_roots.count(entry.animation_sequence_id)) {
                entry.loaded = true;
                next.class_skill_roots.push_back(std::move(entry));
                continue;
            }

            auto candidate_roots = roots;
            candidate_roots.push_back(entry.animation_sequence_id);
            skills_animation::SkillAnimationPrograms candidate_programs;
            std::string candidate_error;
            if (!skills_animation::build_skill_animation_programs(*request.assets,
                    *request.animations, *request.animation_dictionary,
                    *request.same_actor_visual, candidate_roots, request.actor_role,
                    candidate_programs, candidate_error) ||
                !same_plan_sequences(base_plan, candidate_programs, candidate_error)) {
                entry.diagnostic = candidate_error.empty()
                    ? "source SkillTable.Anim assets could not be preloaded" : candidate_error;
                next.class_skill_roots.push_back(std::move(entry));
                continue;
            }
            roots = std::move(candidate_roots);
            unique_roots.insert(entry.animation_sequence_id);
            entry.loaded = true;
            next.class_skill_roots.push_back(std::move(entry));
        }
        if (preloaded_class_row < 0 || next.active_skill_list_id < 0)
            return fail(error, "Active source SkillList metadata is not available for animation preloading");
    }

    if (request.source_animation_table_id) {
        const auto id = *request.source_animation_table_id;
        if (id < 0 || static_cast<std::size_t>(id) >= request.animations->characters.size())
            return fail(error, "Same-actor source CharAnimTable id is outside AnimationTables");
        const auto& fields = request.animations->characters[static_cast<std::size_t>(id)].fields;
        constexpr std::size_t spells_field = 31;
        if (fields.size() <= spells_field || fields[spells_field].size() < next.faery_cast_slots.size())
            return fail(error, "Same-actor source CharAnimTable has fewer than five authored Spells roots");
        next.source_animation_table_id = id;
        for (std::size_t slot = 0; slot < next.faery_cast_slots.size(); ++slot) {
            auto& entry = next.faery_cast_slots[slot];
            entry.source_animation_table_id = id;
            entry.faery_slot = static_cast<std::int32_t>(slot);
            entry.animation_sequence_id = fields[spells_field][slot];
            if (entry.animation_sequence_id < 0) {
                entry.diagnostic = "source CharAnimTable Spells slot has no authored Cast root";
                continue;
            }
            if (static_cast<std::size_t>(entry.animation_sequence_id) >= request.animations->sequences.size()) {
                entry.diagnostic = "source CharAnimTable Spells root is outside AnimationTables";
                continue;
            }
            entry.selection_state = skills_animation::skill_sequence_state(
                entry.animation_sequence_id);
            if (unique_roots.count(entry.animation_sequence_id)) {
                entry.loaded = true;
                continue;
            }
            auto candidate_roots = roots;
            candidate_roots.push_back(entry.animation_sequence_id);
            skills_animation::SkillAnimationPrograms candidate_programs;
            std::string candidate_error;
            if (!skills_animation::build_skill_animation_programs(*request.assets,
                    *request.animations, *request.animation_dictionary,
                    *request.same_actor_visual, candidate_roots, request.actor_role,
                    candidate_programs, candidate_error) ||
                !same_plan_sequences(base_plan, candidate_programs, candidate_error)) {
                entry.diagnostic = candidate_error.empty()
                    ? "source CharAnimTable Spells root could not be preloaded" : candidate_error;
                continue;
            }
            roots = std::move(candidate_roots);
            unique_roots.insert(entry.animation_sequence_id);
            entry.loaded = true;
        }
        const auto& legacy_slot4 = next.faery_cast_slots[4];
        if (legacy_slot4.loaded) {
            next.active_faery_cast_sequence = legacy_slot4.animation_sequence_id;
            next.active_faery_cast_selection_state = legacy_slot4.selection_state;
        }
    }

    if (!roots.empty()) {
        if (!skills_animation::build_skill_animation_programs(*request.assets,
                *request.animations, *request.animation_dictionary,
                *request.same_actor_visual, roots, request.actor_role,
                next.programs, error)) return false;
        if (!same_plan_sequences(base_plan, next.programs, error)) return false;
    } else {
        next.programs.plan.config = *request.same_actor_visual;
        next.programs.plan.roleId = request.actor_role;
    }
    output = std::move(next);
    error.clear();
    return true;
}

bool merge_runtime_skill_animation_bank_v1(
    const RuntimeSkillAnimationBankV1& bank,
    OriginalCombatVisualPlan& visual_plan,
    OriginalSequencePolicies& sequence_policies,
    std::string& error) {
    error.clear();
    if (bank.programs.plan.roleId.empty())
        return fail(error, "Source skill animation bank has no compiled same-actor role");
    OriginalCombatVisualPlan next = visual_plan;
    OriginalSequencePolicies next_policies = sequence_policies;
    std::set<std::string> clip_names;
    for (const auto& clip : bank.programs.plan.config.clips) {
        if (!clip_names.insert(clip.first).second)
            return fail(error, "Compiled source skill bank contains duplicate clip aliases");
    }
    for (const auto& clip : visual_plan.config.clips) {
        const auto found = std::find_if(bank.programs.plan.config.clips.begin(),
            bank.programs.plan.config.clips.end(), [&](const auto& compiled) {
                return compiled.first == clip.first;
            });
        if (found == bank.programs.plan.config.clips.end() || found->second != clip.second)
            return fail(error, "Compiled source skill bank does not preserve the base actor clip map");
    }
    next.config = bank.programs.plan.config;
    next.clipRates.insert(bank.programs.plan.clipRates.begin(),
                          bank.programs.plan.clipRates.end());
    for (const auto& sequence : bank.programs.plan.sequences) {
        if (std::find(next.stateNames.begin(), next.stateNames.end(), sequence.state) !=
            next.stateNames.end())
            return fail(error, "Source skill animation state collides during actor bank merge");
        next.stateNames.push_back(sequence.state);
        next.sequences.push_back(sequence);
    }
    for (const auto& policy : bank.programs.policies) {
        const auto inserted = next_policies.emplace(policy.first, policy.second);
        if (!inserted.second &&
            (inserted.first->second.id != policy.second.id ||
             inserted.first->second.type != policy.second.type ||
             inserted.first->second.loop != policy.second.loop))
            return fail(error, "Source skill sequence policy conflicts with the existing actor bank");
    }
    visual_plan = std::move(next);
    sequence_policies = std::move(next_policies);
    error.clear();
    return true;
}

bool resolve_runtime_skill_animation_slot_v1(
    const CharacterState& character, const dh2::data::CharacterTable& characters,
    dh2::data::SkillTables::Borrow skills, std::uint32_t equipment_set,
    std::uint32_t source_slot, const RuntimeSkillAnimationBankV1& bank,
    RuntimeSkillAnimationSlotV1& output, std::string& error) {
    error.clear();
    if (character.id.empty() || character.id != bank.character_state_id ||
        character.class_id.empty() || character.class_id != bank.class_id)
        return fail(error, "Current skill assignment belongs to a different CharacterState/class than the preloaded bank");
    if (source_slot >= 3)
        return fail(error, "NativeHUDSkill source slot is outside the authored 0..2 range");
    int position = -1;
    std::uint32_t saved_row = 0;
    if (!resolve_assigned_skill_position_v1(character, skills, equipment_set,
            source_slot, position, saved_row, error)) return false;
    SkillVisualRequestV1 visual;
    if (!resolve_skill_visual_request_v1(character, characters, skills,
            position, visual, error)) return false;
    if (visual.saved_skill_row != static_cast<int>(saved_row))
        return fail(error, "Fresh saved assignment resolves to a different SkillList row");

    RuntimeSkillAnimationSlotV1 next;
    next.equipment_set = equipment_set;
    next.source_slot = source_slot;
    next.skill = std::move(visual);
    if (bank.class_roots_preloaded) {
        if (next.skill.active_skill_list_id != bank.active_skill_list_id)
            return fail(error, "Fresh assignment uses a different active source SkillList than the initialized visual bank");
        const auto root = std::find_if(bank.class_skill_roots.begin(),
            bank.class_skill_roots.end(), [&](const RuntimeSkillAnimationClassRootV1& entry) {
                return entry.active_skill_list_id == next.skill.active_skill_list_id &&
                    entry.class_skill_position == position &&
                    entry.skill_table_id == next.skill.skill_table_id;
            });
        if (root == bank.class_skill_roots.end())
            return fail(error, "Fresh assignment has no preloaded source class-list animation root");
        if (!root->loaded) {
            error = root->diagnostic.empty()
                ? "Assigned source class-list animation root was not loaded" : root->diagnostic;
            return false;
        }
        if (root->animation_sequence_id != next.skill.animation_sequence_id)
            return fail(error, "Fresh assignment animation root changed since the pre-init bank was compiled");
        next.selection_state = root->selection_state;
    } else {
        const auto& old_slot = bank.hotbar[source_slot];
        if (!old_slot || old_slot->skill.skill_table_id != next.skill.skill_table_id ||
            old_slot->skill.class_skill_position != position ||
            old_slot->skill.animation_sequence_id != next.skill.animation_sequence_id)
            return fail(error, "Fresh assignment does not match the preloaded saved-hotbar animation entry");
        next.selection_state = old_slot->selection_state;
    }
    output = std::move(next);
    error.clear();
    return true;
}

bool resolve_runtime_faery_animation_slot_v1(
    const CharacterState& character, std::int32_t difficulty,
    const RuntimeSkillAnimationBankV1& bank,
    RuntimeSkillFaeryAnimationSlotV1& output, std::string& error) {
    output = {};
    if (character.id.empty() || character.id != bank.character_state_id ||
        !character.source_faery_state_known || difficulty < 0 || difficulty >= 3)
        return fail(error, "Faery cast bank requires the same source-known CharacterState and difficulty");
    const auto slot = character.faery_by_difficulty[static_cast<std::size_t>(difficulty)].current_faery;
    if (slot < 0 || static_cast<std::size_t>(slot) >= bank.faery_cast_slots.size())
        return fail(error, "Current source Faery Save slot is outside the authored five slots");
    const auto& selected = bank.faery_cast_slots[static_cast<std::size_t>(slot)];
    if (!bank.source_animation_table_id || selected.source_animation_table_id !=
            *bank.source_animation_table_id || selected.faery_slot != slot ||
        !selected.loaded || selected.animation_sequence_id < 0 ||
        selected.selection_state != skills_animation::skill_sequence_state(
            selected.animation_sequence_id)) {
        error = selected.diagnostic.empty()
            ? "Current source Faery Cast root is not loaded in the same actor animation bank"
            : selected.diagnostic;
        return false;
    }
    output = selected;
    error.clear();
    return true;
}

} // namespace dh::foundation::generic_skills
