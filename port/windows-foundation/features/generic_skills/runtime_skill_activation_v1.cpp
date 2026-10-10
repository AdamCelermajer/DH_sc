#include "runtime_skill_activation_v1.hpp"

#include "runtime_skill_progression_v1.hpp"

#include <cstring>
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

} // namespace

bool resolve_skill_visual_request_v1(
    const CharacterState& state, const dh2::data::CharacterTable& characters,
    dh2::data::SkillTables::Borrow tables, int position,
    SkillVisualRequestV1& output, std::string& error) {
    CharacterDesignSkillCapsV1 caps;
    SkillProgressionV1 progression;
    if (!evaluate_skill_progression_v1(state, characters, tables, caps,
                                       position, progression, error)) return false;
    if (!progression.rank_known || progression.saved_skill_row < 0)
        return fail(error, "Skill visual request requires the exact saved CharacterState rank");
    if (progression.rank == 0)
        return fail(error, "Original skill visual request requires a learned source skill");
    if (!progression.available)
        return fail(error, "Original SkillTable character-level requirement rejected the skill request");

    // evaluate_skill_progression_v1 resolved the active list from the saved
    // source rows where available; repeat only the bounds-safe lookup needed
    // to retain the exact dictionary and script tokens in this request.
    if (!tables) return fail(error, "Skill visual request lost its pinned SkillTables borrow");
    if (progression.skill_table_id < 0 ||
        static_cast<std::size_t>(progression.skill_table_id) >= tables.skills().size() ||
        static_cast<std::size_t>(progression.skill_table_id) >= tables.skill_names().size())
        return fail(error, "Resolved original SkillTable row is outside the pinned source table");
    const auto& skill = tables.skills()[static_cast<std::size_t>(progression.skill_table_id)];
    const auto sequence_id = signed_word(skill.scalar.words[1]);
    if (sequence_id < 0)
        return fail(error, "Original SkillTable has no supported authored Anim sequence root");

    SkillVisualRequestV1 next;
    next.character_state_id = state.id;
    next.class_id = state.class_id;
    next.class_skill_position = position;
    next.active_skill_list_id = progression.skill_list_id;
    next.skill_table_id = progression.skill_table_id;
    next.saved_skill_row = progression.saved_skill_row;
    next.saved_rank = progression.rank;
    next.animation_sequence_id = sequence_id;
    next.source_skill_name = tables.skill_names()[static_cast<std::size_t>(progression.skill_table_id)];
    next.source_script = skill.script;
    output = std::move(next);
    error.clear();
    return true;
}

bool resolve_skill_cast_admission_v1(
    const SkillCastAdmissionFactV1& admission, SkillCastAnimationOutcomeV1& outcome,
    std::string& error) {
    error.clear();
    switch (admission.status) {
    case SkillCastAdmissionStatusV1::unknown:
        outcome = SkillCastAnimationOutcomeV1::source_admission_unknown;
        return true;
    case SkillCastAdmissionStatusV1::rejected:
        outcome = SkillCastAnimationOutcomeV1::source_rejected;
        return true;
    case SkillCastAdmissionStatusV1::accepted:
        if (admission.source.empty())
            return fail(error, "Accepted skill cast requires a named source admission provider");
        outcome = SkillCastAnimationOutcomeV1::source_admission_accepted;
        return true;
    }
    return fail(error, "Skill cast admission status is not a recognized source fact");
}

bool validate_skill_cast_admission_binding_v1(
    const SkillVisualRequestV1& request, const SkillCastAdmissionFactV1& admission,
    ActorId actor, std::string& error) {
    if (admission.status != SkillCastAdmissionStatusV1::accepted || admission.source.empty())
        return fail(error, "Skill admission binding requires an accepted named source fact");
    if (actor == invalid_actor_id || admission.actor != actor ||
        request.character_state_id.empty() || request.class_id.empty() ||
        admission.character_state_id != request.character_state_id ||
        admission.class_id != request.class_id ||
        admission.active_skill_list_id != request.active_skill_list_id ||
        admission.class_skill_position != request.class_skill_position ||
        admission.skill_table_id != request.skill_table_id ||
        admission.saved_rank != request.saved_rank)
        return fail(error, "Accepted source admission does not match this CharacterState, skill row, rank and session actor");
    error.clear();
    return true;
}

} // namespace dh::foundation::generic_skills
