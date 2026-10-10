#pragma once

#include "../../actor_profiles.hpp"
#include "../../original_attack_sequence.hpp"
#include "../../../game-data/animation_tables.hpp"

namespace dh::foundation::combat {

// Exact incoming hit/death source roots for one actor profile. This is clip and
// source-step data only; an incoming receiver/state owner decides when to use it.
struct RuntimePlayerIncomingAnimationBankV1 {
    std::string profile_id;
    std::int32_t animation_table = -1;
    std::int32_t injury_override_sequence_id = -1;
    std::int32_t death_override_sequence_id = -1;
    OriginalCombatVisualPlan visual;
    OriginalSequencePolicies sequence_policies;
    std::map<std::pair<std::string, std::vector<std::size_t>>,
             dh2::data::AnimationStep> steps;
};

// Resolves CharAnimTable.Injured and CharAnimTable.Died from the supplied
// profile's exact animationTable. Injured choice is always caller-selected:
// this helper neither samples nor advances source RNG. Died is the ordinary
// SM_SetDeadState branch and does not apply an equipment stance.
bool build_runtime_player_incoming_animation_bank_v1(
    const AssetCatalog&, const ActorProfile&, const CharacterVisualConfig& same_actor_visual,
    const dh2::data::AnimationTables&, const dh2::data::Dictionary&,
    const std::string& role, RuntimePlayerIncomingAnimationBankV1&, std::string& error);

bool make_runtime_player_injury_selection_v1(
    const RuntimePlayerIncomingAnimationBankV1&, std::size_t caller_selected_step,
    OriginalAttackSelection&, std::string& error);

bool make_runtime_player_death_selection_v1(
    const RuntimePlayerIncomingAnimationBankV1&, OriginalAttackSelection&,
    std::string& error);

} // namespace dh::foundation::combat
