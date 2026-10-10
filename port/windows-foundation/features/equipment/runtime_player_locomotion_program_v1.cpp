#include "runtime_player_locomotion_program_v1.hpp"

#include "../../content_paths.hpp"

#include <set>
#include <stdexcept>

namespace dh::foundation::equipment_menu {
namespace {
using namespace dh2::data;

const RuntimePlayerLocomotionStateV1& require_state(
        const RuntimePlayerLocomotionV1& bank, const std::string& name) {
    for (const auto& state : bank.states) if (state.name == name) return state;
    throw std::runtime_error("Resolved locomotion bank is missing state " + name);
}

const RuntimePlayerLocomotionSequenceV1& require_projected_sequence(
        const RuntimePlayerLocomotionStateV1& state, std::int32_t id) {
    for (const auto& sequence : state.reachable_sequences)
        if (sequence.sequence_id == id) return sequence;
    throw std::runtime_error("Resolved locomotion bank is missing reachable sequence " +
                             std::to_string(id));
}

void add_policy(RuntimePlayerLocomotionProgramV1& out,
        const AnimationTables& tables, std::int32_t id) {
    if (id < 0 || static_cast<std::size_t>(id) >= tables.sequences.size() ||
        static_cast<std::size_t>(id) >= tables.sequence_names.size())
        throw std::runtime_error("Locomotion sequence reference is outside the actual AnimTable");
    const auto& sequence = tables.sequences[static_cast<std::size_t>(id)];
    OriginalSequencePolicy policy{id, sequence.type, sequence.loop,
                                  tables.sequence_names[static_cast<std::size_t>(id)]};
    const auto inserted = out.policies.emplace(id, policy);
    if (!inserted.second &&
        (inserted.first->second.type != policy.type || inserted.first->second.loop != policy.loop ||
         inserted.first->second.name != policy.name))
        throw std::runtime_error("Shared locomotion sequence has inconsistent source policy");
}

std::string path_name(const std::vector<std::size_t>& path) {
    std::string result;
    for (const auto part : path) {
        if (!result.empty()) result += '-';
        result += std::to_string(part);
    }
    return result;
}

} // namespace

bool build_runtime_player_locomotion_program_v1(
        const AssetCatalog& assets, const RuntimePlayerLocomotionV1& locomotion,
        const AnimationTables& animations, const Dictionary& clip_dictionary,
        const CharacterVisualConfig& same_visual, const std::string& role,
        RuntimePlayerLocomotionProgramV1& output, std::string& error) {
    try {
        if (role.empty() || locomotion.states.empty())
            throw std::runtime_error("Locomotion program requires a source role and resolved state bank");

        RuntimePlayerLocomotionProgramV1 next;
        next.plan.config = same_visual;
        next.plan.roleId = role;
        std::set<std::string> state_names, clip_aliases;
        for (const auto& existing : same_visual.clips) {
            if (existing.first.empty() || !clip_aliases.insert(existing.first).second)
                throw std::runtime_error("Same visual contains an empty or duplicate named clip alias");
        }
        std::size_t total_clips = same_visual.clips.size();

        for (const auto& projected_state : locomotion.states) {
            if (projected_state.name.empty() || !state_names.insert(projected_state.name).second)
                throw std::runtime_error("Locomotion state names are empty or duplicated");
            if (projected_state.selected_sequence < 0 ||
                projected_state.base_sequence < 0 || projected_state.reachable_sequences.empty())
                throw std::runtime_error("Locomotion state has no selected source sequence graph");

            const auto& state_bank = require_state(locomotion, projected_state.name);
            const auto root_id = projected_state.selected_sequence;
            const auto& root_projection = require_projected_sequence(state_bank, root_id);
            if (root_id >= static_cast<std::int32_t>(animations.sequences.size()) ||
                root_id >= static_cast<std::int32_t>(animations.sequence_names.size()))
                throw std::runtime_error("Selected locomotion root is outside the actual AnimTable");

            OriginalCombatSequencePlan plan_sequence;
            plan_sequence.state = projected_state.name;
            plan_sequence.name = animations.sequence_names[static_cast<std::size_t>(root_id)];
            plan_sequence.variant = 0;
            plan_sequence.id = root_id;
            plan_sequence.loop = animations.sequences[static_cast<std::size_t>(root_id)].loop;
            plan_sequence.type = animations.sequences[static_cast<std::size_t>(root_id)].type;
            next.plan.stateNames.push_back(projected_state.name);
            add_policy(next, animations, root_id);

            std::set<std::int32_t> active;
            std::function<void(std::int32_t, std::vector<std::size_t>,
                               std::vector<OriginalCombatRedirect>)> append;
            append = [&](std::int32_t sequence_id, std::vector<std::size_t> prefix,
                         std::vector<OriginalCombatRedirect> ancestors) {
                if (prefix.size() >= 3 || !active.insert(sequence_id).second)
                    throw std::runtime_error("Locomotion redirect exceeds source depth or cycles");
                const auto& projection = require_projected_sequence(state_bank, sequence_id);
                if (sequence_id < 0 || static_cast<std::size_t>(sequence_id) >= animations.sequences.size() ||
                    static_cast<std::size_t>(sequence_id) >= animations.sequence_names.size())
                    throw std::runtime_error("Reachable locomotion sequence is outside actual AnimTable");
                const auto& source_sequence = animations.sequences[static_cast<std::size_t>(sequence_id)];
                const auto& source_name = animations.sequence_names[static_cast<std::size_t>(sequence_id)];
                if (projection.alias != source_name || projection.loop != source_sequence.loop ||
                    projection.type != source_sequence.type || projection.steps.size() != source_sequence.steps.size())
                    throw std::runtime_error("Resolved locomotion projection diverges from actual typed AnimTable");
                add_policy(next, animations, sequence_id);

                for (std::size_t i = 0; i < source_sequence.steps.size(); ++i) {
                    const auto& step = source_sequence.steps[i];
                    const auto& projected_step = projection.steps[i];
                    if (projected_step.animation_id != step.anim || projected_step.redirect != step.redir ||
                        projected_step.blend_out != step.blend_out || projected_step.speed != step.speed ||
                        projected_step.move_go != step.move_go)
                        throw std::runtime_error("Resolved locomotion step diverges from actual source phase");
                    auto path = prefix;
                    path.push_back(i);
                    if (!next.steps.emplace(std::make_pair(projected_state.name, path), step).second)
                        throw std::runtime_error("Duplicate source locomotion step path");

                    if (step.redir == 1) {
                        const auto child_id = step.anim;
                        if (child_id < 0 || static_cast<std::size_t>(child_id) >= animations.sequences.size() ||
                            static_cast<std::size_t>(child_id) >= animations.sequence_names.size() ||
                            projected_step.redirected_sequence_alias !=
                                animations.sequence_names[static_cast<std::size_t>(child_id)])
                            throw std::runtime_error("Locomotion redirect alias does not match actual AnimTable");
                        auto child_ancestors = ancestors;
                        child_ancestors.push_back({static_cast<std::int64_t>(i), step.anim, step.redir,
                                                   step.speed, step.blend_out, step.move_go, {}});
                        append(child_id, std::move(path), std::move(child_ancestors));
                        continue;
                    }

                    if (step.redir != 0 || step.anim < 0)
                        throw std::runtime_error("Symbolic/nonvisual locomotion phase has no supported clip binding");
                    const auto* source_uri = animation_clip(step, clip_dictionary);
                    if (!source_uri || projected_step.clip_uri != *source_uri ||
                        projected_step.clip_alias != clip_dictionary.names[static_cast<std::size_t>(step.anim)])
                        throw std::runtime_error("Locomotion clip alias/URI differs from actual clip dictionary");
                    const auto resolved = resolve_content_path(assets, normalize_content_uri(*source_uri),
                                                               same_visual.model_path)
                                              .lexically_relative(assets.root()).generic_string();
                    const auto named_alias = role + "/" + projected_state.name + "/seq-" +
                                             std::to_string(sequence_id) + "/step-" + path_name(path);
                    if (!clip_aliases.insert(named_alias).second)
                        throw std::runtime_error("Locomotion named clip alias collides with existing visual bank");
                    if (++total_clips > 4096)
                        throw std::runtime_error("Locomotion named clip bank exceeds source visual bound");

                    OriginalCombatPhase phase;
                    phase.sourcePath = path;
                    for (const auto index : path) phase.sourceIndices.push_back(static_cast<std::int64_t>(index));
                    phase.ancestors = ancestors;
                    phase.animationId = step.anim;
                    phase.redirect = step.redir;
                    phase.blendOut = step.blend_out;
                    phase.moveGO = step.move_go;
                    phase.speed = step.speed;
                    phase.sourceUri = *source_uri;
                    phase.resolvedPath = resolved;
                    phase.clipName = named_alias;
                    plan_sequence.phases.push_back(std::move(phase));
                    next.plan.config.clips.emplace_back(named_alias, resolved);
                    next.plan.clipRates.emplace(named_alias, step.speed);
                    next.named_clips.push_back({projected_state.name, path, sequence_id, step.anim,
                        projected_step.clip_alias, *source_uri, named_alias, resolved,
                        step.speed, step.blend_out, step.move_go});
                }
                active.erase(sequence_id);
            };
            append(root_id, {}, {});
            if (plan_sequence.phases.empty())
                throw std::runtime_error("Selected locomotion source graph contains no direct clip leaves");
            next.plan.sequences.push_back(std::move(plan_sequence));
            (void)root_projection;
        }

        output = std::move(next);
        error.clear();
        return true;
    } catch (const std::exception& e) {
        error = e.what();
        return false;
    }
}

} // namespace dh::foundation::equipment_menu
