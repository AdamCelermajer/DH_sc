#include "original_combat_visual_plan.hpp"
#include "content_paths.hpp"

#include <functional>
#include <set>
#include <stdexcept>
#include <utility>

namespace dh::foundation {
const OriginalCombatSequencePlan* OriginalCombatVisualPlan::sequence(const std::string& state,
                                                                    std::size_t explicitVariant) const noexcept {
    for (const auto& candidate : sequences)
        if (candidate.state == state && candidate.variant == explicitVariant) return &candidate;
    return nullptr;
}
const OriginalCombatPhase* OriginalCombatVisualPlan::phase(const std::string& state,
                                                          std::size_t explicitVariant,
                                                          const std::vector<std::size_t>& explicitLeafPath) const noexcept {
    const auto* selected = sequence(state, explicitVariant);
    if (!selected) return nullptr;
    for (const auto& candidate : selected->phases)
        if (candidate.sourcePath == explicitLeafPath) return &candidate;
    return nullptr;
}
bool build_original_combat_visual_plan(const AssetCatalog& assets,
                                       const OriginalMeleeBindings& bindings,
                                       const std::string& profileId,
                                       const ActorCustomization& customization,
                                       const std::string& callerRoleId,
                                       OriginalCombatVisualPlan& output,
                                       std::string& error) {
    try {
        if (callerRoleId.empty()) throw std::runtime_error("Combat visual plan requires a caller role ID");
        const auto* actor = bindings.find_actor(profileId);
        if (!actor) throw std::runtime_error("Original combat actor binding not found: " + profileId);
        OriginalCombatVisualPlan next;
        next.profileId = profileId; next.roleId = callerRoleId;
        next.config.model_path = resolve_content_path(assets, normalize_content_uri(actor->model)).lexically_relative(assets.root()).generic_string();
        if (!actor->templateClip.empty())
            next.config.template_clip_path = resolve_content_path(assets, normalize_content_uri(actor->templateClip), actor->model).lexically_relative(assets.root()).generic_string();
        next.config.skin_id_contains = customization.skin_id_contains;
        next.config.controller_ids=customization.controller_ids;
        next.config.use_authored_modular_defaults=customization.use_authored_modular_defaults;
        next.config.expected_controller_count = customization.expected_controller_count;
        next.config.include_static_instances = customization.include_static_instances;
        next.config.allow_missing_animation_targets = customization.allow_missing_animation_targets;
        std::set<std::string> clipNames;
        for (const auto& state : actor->states) {
            next.stateNames.push_back(state.first);
            for (std::size_t variant = 0; variant < state.second.size(); ++variant) {
                const auto& source = state.second[variant];
                OriginalCombatSequencePlan plan;
                plan.state=state.first; plan.variant=variant; plan.id=source.id;
                plan.name=source.name; plan.loop=source.loop; plan.type=source.type; plan.properties=source.properties;
                std::function<void(const OriginalMeleeStep&, std::vector<std::size_t>, std::vector<std::int64_t>, std::vector<OriginalCombatRedirect>)> append;
                append = [&](const OriginalMeleeStep& step, std::vector<std::size_t> path,
                             std::vector<std::int64_t> indices, std::vector<OriginalCombatRedirect> ancestors) {
                    indices.push_back(step.index);
                    if (!step.children.empty()) {
                        ancestors.push_back({step.index, step.animationId, step.redirect, step.speed, step.blendOut, step.moveGO, step.properties});
                        for (std::size_t child=0; child<step.children.size(); ++child) {
                            auto childPath=path; childPath.push_back(child);
                            append(step.children[child],std::move(childPath),indices,ancestors);
                        }
                        return;
                    }
                    OriginalCombatPhase phase;
                    phase.sourcePath=std::move(path); phase.sourceIndices=std::move(indices); phase.ancestors=std::move(ancestors);
                    phase.animationId=step.animationId; phase.redirect=step.redirect; phase.blendOut=step.blendOut;
                    phase.moveGO=step.moveGO; phase.speed=step.speed; phase.sourceUri=step.uri; phase.properties=step.properties;
                    if (!step.uri.empty()) {
                        phase.resolvedPath=resolve_content_path(assets,normalize_content_uri(step.uri),actor->model).lexically_relative(assets.root()).generic_string();
                        phase.clipName=callerRoleId+"/"+state.first+"/sequence-"+std::to_string(variant)+"/phase-"+std::to_string(plan.phases.size());
                        if (!clipNames.insert(phase.clipName).second) throw std::runtime_error("Combat visual clip alias collision");
                        next.config.clips.emplace_back(phase.clipName,phase.resolvedPath);
                        next.clipRates.emplace(phase.clipName,phase.speed);
                        if (next.config.clips.size()>4096) throw std::runtime_error("Combat visual bank exceeds 4096 clips");
                    }
                    plan.phases.push_back(std::move(phase));
                };
                for (std::size_t step=0; step<source.steps.size(); ++step) append(source.steps[step],{step},{},{});
                next.sequences.push_back(std::move(plan));
            }
        }
        output=std::move(next); error.clear(); return true;
    } catch (const std::exception& exception) { error=exception.what(); return false; }
}

} // namespace dh::foundation
