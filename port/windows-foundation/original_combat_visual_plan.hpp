#pragma once

#include "actor_profiles.hpp"
#include "original_melee_bindings.hpp"

namespace dh::foundation {

struct OriginalCombatRedirect {
    std::int64_t index = 0, animationId = -1, redirect = 0;
    double speed = 0;
    std::int64_t blendOut = 0, moveGO = 0;
    OriginalBindingProperties properties;
};
struct OriginalCombatPhase {
    // Hierarchy positions, not flattened phase ordinals. Explicit selection must
    // supply this complete path; sourceIndices separately retain exported indices.
    std::vector<std::size_t> sourcePath;
    std::vector<std::int64_t> sourceIndices;
    std::vector<OriginalCombatRedirect> ancestors;
    std::int64_t animationId = -1, redirect = 0, blendOut = 0, moveGO = 0;
    double speed = 0;
    std::string sourceUri, resolvedPath, clipName;
    OriginalBindingProperties properties;
    bool has_visual() const noexcept { return !clipName.empty(); }
};
struct OriginalCombatSequencePlan {
    std::string state, name;
    std::size_t variant = 0;
    std::int64_t id = -1, loop = 0, type = 0;
    OriginalBindingProperties properties;
    // Ordered pre/strike/recovery leaves are never treated as random variants.
    // Nonvisual/symbolic terminal steps remain phases with an empty clipName.
    std::vector<OriginalCombatPhase> phases;
};
struct OriginalCombatVisualPlan {
    std::string profileId, roleId;
    CharacterVisualConfig config;
    // Includes authored empty states; no sequence is invented for them.
    std::vector<std::string> stateNames;
    std::vector<OriginalCombatSequencePlan> sequences;
    // Leaf source rate only. Redirect ancestor rates remain on each phase;
    // composing them requires the original sequence playback policy.
    std::map<std::string, double> clipRates;
    const OriginalCombatSequencePlan* sequence(const std::string& state,
                                              std::size_t explicitVariant) const noexcept;
    const OriginalCombatPhase* phase(const std::string& state,
                                    std::size_t explicitVariant,
                                    const std::vector<std::size_t>& explicitLeafPath) const noexcept;
};

// Caller supplies actor ID, role namespace and customization. No stance, state,
// sequence variant, leaf, controller, or missing-target policy is chosen here.
// Builds a complete named visual bank and source sequence metadata atomically.
// Selecting only a strike leaf is a diagnostic operation, not a complete attack.
bool build_original_combat_visual_plan(const AssetCatalog& assets,
                                       const OriginalMeleeBindings& bindings,
                                       const std::string& profileId,
                                       const ActorCustomization& customization,
                                       const std::string& callerRoleId,
                                       OriginalCombatVisualPlan& output,
                                       std::string& error);

} // namespace dh::foundation
