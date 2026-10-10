#pragma once

#include "runtime_player_locomotion_v1.hpp"
#include "../../original_attack_sequence.hpp"

namespace dh::foundation::equipment_menu {

struct RuntimePlayerLocomotionClipV1 {
    std::string state;
    std::vector<std::size_t> source_path;
    std::int32_t sequence_id{-1}, animation_id{-1};
    std::string source_alias, source_uri, named_alias, resolved_path;
    double speed{1.0};
    std::int32_t blend_out{}, move_go{};
};

struct RuntimePlayerLocomotionProgramV1 {
    OriginalCombatVisualPlan plan;
    OriginalSequencePolicies policies;
    // Keeps every actual AnimationStep for source FX/camera/audio/etc consumers.
    std::map<std::pair<std::string, std::vector<std::size_t>>, dh2::data::AnimationStep> steps;
    std::vector<RuntimePlayerLocomotionClipV1> named_clips;
};

// Converts a previously resolved equipment/stance projection into the existing
// visual-plan and typed sequence-policy formats. The caller supplies the exact
// same AnimTable graph, dictionary, AssetCatalog and character visual config.
// No sequence selection or playback is performed here.
bool build_runtime_player_locomotion_program_v1(
    const AssetCatalog& assets,
    const RuntimePlayerLocomotionV1& locomotion,
    const dh2::data::AnimationTables& animations,
    const dh2::data::Dictionary& clip_dictionary,
    const CharacterVisualConfig& same_visual,
    const std::string& role,
    RuntimePlayerLocomotionProgramV1& output,
    std::string& error);

} // namespace dh::foundation::equipment_menu
