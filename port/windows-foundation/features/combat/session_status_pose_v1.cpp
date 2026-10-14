#include "session_status_pose_v1.hpp"
#include "../skills_animation/skill_animation_program.hpp"

#include <algorithm>
#include <functional>

namespace dh::foundation::combat {

bool build_session_status_animation_bank_v1(const AssetCatalog& assets, const dh2::data::AnimationTables& animations,
    const dh2::data::Dictionary& clips, std::int32_t target_animation_table, std::int32_t stance,
    std::uint32_t stanced_animation_mask, const CharacterVisualConfig& visual, const std::string& role,
    SessionStatusAnimationBankV1& output, std::string& error) {
    error.clear();
    if (target_animation_table < 0 || std::size_t(target_animation_table) >= animations.characters.size()) {
        error = "Status animation bank requires a valid actor CharAnimTable row";
        return false;
    }
    if (stance < 0) {
        error = "Status animation bank requires an explicit nonnegative stance index";
        return false;
    }
    const auto& fields = animations.characters[std::size_t(target_animation_table)].fields;
    const auto source_root = [&](std::size_t field, std::uint32_t stanced_bit, std::int32_t& resolved) -> bool {
        if (fields[field].size() != 1) {
            error = "Status animation table scalar field is malformed";
            return false;
        }
        const auto base = fields[field][0];
        if (base < 0) {
            resolved = -1; // the original state setter stores a negative index: no clip
            return true;
        }
        const std::int64_t value = std::int64_t(base) + ((stanced_animation_mask & stanced_bit) ? stance : 0);
        if (value < 0 || std::size_t(value) >= animations.sequences.size()) {
            error = "Stanced status animation root is outside the source sequence table";
            return false;
        }
        resolved = static_cast<std::int32_t>(value);
        return true;
    };
    std::int32_t stun = -1, scare = -1;
    if (!source_root(32, 0x200u, stun) || !source_root(29, 0x100u, scare)) return false;
    if (stun < 0 && scare < 0) {
        error = "Status animation bank has no authored Stunned or Scared source root";
        return false;
    }
    std::vector<std::int32_t> roots;
    if (stun >= 0) roots.push_back(stun);
    if (scare >= 0 && scare != stun) roots.push_back(scare);
    skills_animation::SkillAnimationPrograms source_bank;
    if (!skills_animation::build_skill_animation_programs(assets, animations, clips, visual, roots, role, source_bank, error))
        return false;
    const auto rename_root = [&](std::int32_t sequence_id, const std::string& state) -> bool {
        if (sequence_id < 0) return true;
        auto found = std::find_if(source_bank.plan.sequences.begin(), source_bank.plan.sequences.end(),
            [&](const auto& sequence) { return sequence.id == sequence_id; });
        if (found == source_bank.plan.sequences.end()) return false;
        auto copy = *found;
        copy.state = state;
        auto existing = std::find_if(source_bank.plan.sequences.begin(), source_bank.plan.sequences.end(),
            [&](const auto& sequence) { return sequence.state == state; });
        if (existing == source_bank.plan.sequences.end()) source_bank.plan.sequences.push_back(std::move(copy));
        else *existing = std::move(copy);
        if (std::find(source_bank.plan.stateNames.begin(), source_bank.plan.stateNames.end(), state) ==
            source_bank.plan.stateNames.end())
            source_bank.plan.stateNames.push_back(state);
        return true;
    };
    SessionStatusAnimationBankV1 next;
    if (!rename_root(stun, next.stun_state) || !rename_root(scare, next.scare_state)) {
        error = "Status animation bank lost an authored root while naming the two variants";
        return false;
    }
    const std::function<std::string(std::int32_t, int)> describe = [&](std::int32_t id, int depth) -> std::string {
        if (id < 0 || std::size_t(id) >= animations.sequences.size()) return "?";
        const auto& sequence = animations.sequences[std::size_t(id)];
        std::string text = std::to_string(id) + ":t" + std::to_string(sequence.type) + "/l" + std::to_string(sequence.loop) + "[";
        for (std::size_t i = 0; i < sequence.steps.size(); ++i) {
            const auto& step = sequence.steps[i];
            if (i) text += ",";
            if (step.redir) text += depth < 3 ? describe(step.anim, depth + 1) : "..";
            else {
                const auto* clip = dh2::data::animation_clip(step, clips);
                text += clip ? *clip : "clip?";
            }
        }
        return text + "]";
    };
    next.stun_description = describe(stun, 0);
    next.scare_description = describe(scare, 0);
    if (stun >= 0) next.stun_looping = animations.sequences[std::size_t(stun)].loop != 0;
    if (scare >= 0) next.scare_looping = animations.sequences[std::size_t(scare)].loop != 0;
    // Program per pose from the source hierarchy (see the policy comment in combat_session.hpp): the first leaf under an infinite container is
    // the looping leaf; a single leaf before it (stun: pre_stunned) is the entry clip played once.
    const auto derive = [&](const std::string& state, SessionStatusPoseProgramV1& program) {
        program = {};
        const auto* sequence = source_bank.plan.sequence(state, 0);
        if (!sequence) return;
        for (const auto& phase : sequence->phases) {
            if (phase.sourcePath.empty() || phase.clipName.empty()) continue;
            std::int64_t repeats = sequence->loop;
            if (!phase.ancestors.empty()) {
                const auto found = source_bank.policies.find(phase.ancestors.back().animationId);
                if (found == source_bank.policies.end()) continue;
                repeats = found->second.loop;
            }
            if (repeats == 0) continue;
            program.has_loop = true;
            program.loop_leaf_path = phase.sourcePath;
            if (phase.sourcePath.size() >= 2 && phase.sourcePath[0] == 1) {
                program.has_entry = true;
                program.entry_group_path = {0};
            } else {
                program.loop_only = true;
            }
            return;
        }
        program.has_entry = true; // no infinite leaf: the whole authored sequence plays once and the pose holds its last frame
    };
    derive(next.stun_state, next.stun_program);
    derive(next.scare_state, next.scare_program);
    next.plan = std::move(source_bank.plan);
    next.policies = std::move(source_bank.policies);
    next.stun_sequence = stun;
    next.scare_sequence = scare;
    next.stance = stance;
    next.stanced_animation_mask = stanced_animation_mask;
    output = std::move(next);
    return true;
}

bool play_session_status_pose_v1(CombatSession& session, ActorId actor, bool stun, bool forced, std::uint64_t generation,
    const SessionStatusAnimationBankV1& bank, CombatSessionStateAnimationServices services, bool& started, std::string& error) {
    error.clear();
    started = false;
    const auto* target = session.actor(actor);
    if (!target || !target->alive()) {
        error = "Status pose actor is no longer live in the same Session";
        return false;
    }
    const auto sequence = stun ? bank.stun_sequence : bank.scare_sequence;
    const auto& state = stun ? bank.stun_state : bank.scare_state;
    if (sequence < 0) return true; // no clip in the row: nothing to play
    if (!bank.plan.sequence(state, 0)) {
        error = "Status animation bank lacks the source-selected authored sequence";
        return false;
    }
    OriginalAttackSelection selection;
    selection.state = state;
    selection.variant = 0;
    selection.actor_rate = 1.0;
    const auto& program = stun ? bank.stun_program : bank.scare_program;
    if (program.has_entry) selection.group_path = program.entry_group_path;
    if (!services.finished) services.finished = [](ActorId, std::string&) { return true; };
    if (!services.departed) services.departed = [](ActorId, std::int32_t, std::int32_t, std::string&) { return true; };
    if (!services.checkpoint) services.checkpoint = [](std::string&) { return true; };
    CombatSessionSourceSequencePolicy policy;
    policy.original_state = stun ? 9 : 8;
    policy.state_flags = stun ? 0x2202u : 0x2240u;
    policy.generation = generation;
    policy.source_direct_transition = forced;
    if (program.has_loop) {
        policy.loop_state = state;
        policy.loop_variant = 0;
        policy.loop_leaf_path = program.loop_leaf_path;
        policy.loop_only = program.loop_only;
        // Scared8: the original moves the actor with Cmd_HeadTowards (a random heading), not with the walk clip root motion; this port has no
        // such heading owner, so the scare pose is pose-only (no authored MoveGO drift). Stunned9 keeps the authored clip flags.
        policy.loop_root_motion = stun;
    }
    if (!session.play_actor_source_sequence(actor, bank.plan, bank.policies, selection, std::move(services), policy, error))
        return false;
    started = true;
    return true;
}

} // namespace dh::foundation::combat
