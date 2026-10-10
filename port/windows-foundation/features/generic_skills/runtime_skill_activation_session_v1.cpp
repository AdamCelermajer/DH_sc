#include "runtime_skill_activation_v1.hpp"

#include <string>
#include <utility>

namespace dh::foundation::generic_skills {
namespace {
bool fail(std::string& error, const char* message) {
    error = message;
    return false;
}

std::string source_sequence_state(std::int32_t sequence_id) {
    return "source-skill-sequence-" + std::to_string(sequence_id);
}
} // namespace

bool play_skill_visual_request_v1(
    const SkillVisualRequestV1& request, ActorId actor, CombatSession& session,
    const OriginalCombatVisualPlan& plan, const OriginalSequencePolicies& policies,
    const OriginalAttackSelection& selection,
    CombatSessionStateAnimationServices services, std::string& error) {
    if (actor == invalid_actor_id)
        return fail(error, "Skill visual request requires the same valid CombatSession actor");
    if (request.character_state_id.empty() || request.class_id.empty() ||
        request.class_skill_position < 0 || request.active_skill_list_id < 0 ||
        request.skill_table_id < 0 || request.saved_skill_row < 0 || request.saved_rank == 0 ||
        request.animation_sequence_id < 0 || request.source_skill_name.empty())
        return fail(error, "Skill visual request is incomplete or no longer source-resolved");

    const auto expected_state = source_sequence_state(request.animation_sequence_id);
    if (selection.state != expected_state)
        return fail(error, "Skill sequence selection does not name the resolved SkillTable Anim root");
    const auto* sequence = plan.sequence(selection.state, selection.variant);
    if (!sequence || sequence->id != request.animation_sequence_id)
        return fail(error, "Preloaded same-actor animation plan does not contain the resolved SkillTable Anim root");
    const auto policy = policies.find(request.animation_sequence_id);
    if (policy == policies.end() || policy->second.id != request.animation_sequence_id ||
        policy->second.type != sequence->type || policy->second.loop != sequence->loop)
        return fail(error, "Original skill sequence type/loop policy is missing or mismatched");

    // Preserve caller source selection and marker/completion routing. In
    // particular, don't manufacture a type2/random choice here.
    return session.play_actor_source_sequence(actor, plan, policies, selection,
                                               std::move(services), error);
}

bool request_skill_cast_animation_v1(
    const SkillVisualRequestV1& request, const SkillCastAdmissionFactV1& admission,
    ActorId actor, CombatSession& session, const OriginalCombatVisualPlan& plan,
    const OriginalSequencePolicies& policies, const OriginalAttackSelection& selection,
    CombatSessionStateAnimationServices services, SkillCastAnimationOutcomeV1& outcome,
    std::string& error) {
    if (!resolve_skill_cast_admission_v1(admission, outcome, error)) return false;
    if (outcome != SkillCastAnimationOutcomeV1::source_admission_accepted) return true;
    if (!validate_skill_cast_admission_binding_v1(request, admission, actor, error)) return false;
    if (!play_skill_visual_request_v1(request, actor, session, plan, policies,
                                      selection, std::move(services), error)) return false;
    outcome = SkillCastAnimationOutcomeV1::playback_started;
    return true;
}

} // namespace dh::foundation::generic_skills
