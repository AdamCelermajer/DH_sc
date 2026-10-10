#pragma once

#include "runtime_player_combo_chain_v1.hpp"

namespace dh::foundation::combat {

// A profile-bound pair of source attack banks. The moving/static choice stays
// with the original predecessor-state caller; this value only prepares both
// exact stance-selected AnimTable graphs and their visual aliases.
struct RuntimePlayerProfileAttackBankPlanV1 {
    std::string source_profile_id;
    std::int32_t animation_table{-1};
    std::int32_t stance{-1};
    std::int32_t moving_base_sequence_id{-1};
    std::int32_t moving_sequence_id{-1};
    std::int32_t static_base_sequence_id{-1};
    std::int32_t static_sequence_id{-1};
    bool moving_stance_applied{};
    bool static_stance_applied{};
    OriginalAttackSelection moving_selection;
    OriginalAttackSelection static_selection;
    OriginalSequencePolicies sequence_policies;
    OriginalCombatVisualPlan bank;
    bool requires_original_rng_start{};
};

// Builds both player Attack/AttackStatic source graphs for one actual profile
// and one already-resolved equipment stance. `same_profile_visual` must be the
// loaded profile's own source visual plan; it supplies the model, skin and
// existing aliases. This creates no command, attack result, RNG choice, marker
// routing, state transition or equipment mutation.
bool plan_runtime_player_profile_attack_bank_v1(
    const AssetCatalog& assets,
    const ActorProfile& source_profile,
    const equipment_menu::RuntimePlayerLocomotionV1& equipped,
    std::int32_t source_stanced_list_mask,
    const dh2::data::AnimationTables& animations,
    const dh2::data::Dictionary& clip_dictionary,
    const OriginalCombatVisualPlan& same_profile_visual,
    const std::string& role,
    RuntimePlayerProfileAttackBankPlanV1& output,
    std::string& error);

} // namespace dh::foundation::combat
