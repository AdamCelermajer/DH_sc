#pragma once

#include "../equipment/runtime_player_locomotion_v1.hpp"
#include "../../original_attack_sequence.hpp"

namespace dh::foundation::combat {

// Source state-entry choice for one delivered player attack command. Equipment
// and stance must come from the same live ItemTable/stance resolver used by
// RuntimePlayerLocomotionV1; this plan does not infer weapons from visuals.
struct RuntimePlayerComboChainPlanV1 {
    bool command_delivered{};
    bool stance_applied{};
    bool requires_original_rng_start{};
    std::int32_t animation_table{-1};
    std::int32_t stance{};
    std::int32_t predecessor_state{-1};
    std::int32_t base_sequence_id{-1};
    std::int32_t selected_sequence_id{-1};
    std::string state;
    OriginalCombatVisualPlan visual;
    OriginalSequencePolicies sequence_policies;
    OriginalAttackSelection selection;
};

// Reconstructs CSAttack::OnFocus's player field and stance-mask selection from
// the original AnimTable. `command_delivered` is already admitted upstream;
// this function does not synthesize input edges or controller admission.
// Source moving attack is selected only when predecessor_state == 4 (Move).
// It builds a complete source visual sequence graph from the selected root and
// leaves combo progression, RNG, markers and results to existing owners.
bool plan_runtime_player_combo_chain_v1(
    const AssetCatalog& assets,
    const equipment_menu::RuntimePlayerLocomotionV1& equipped,
    std::int32_t source_stanced_list_mask,
    std::int32_t predecessor_state,
    bool command_delivered,
    const dh2::data::AnimationTables& animations,
    const dh2::data::Dictionary& clips,
    const std::string& source_profile_id,
    const CharacterVisualConfig& same_character_visual,
    const std::string& role,
    RuntimePlayerComboChainPlanV1& output,
    std::string& error);

} // namespace dh::foundation::combat
