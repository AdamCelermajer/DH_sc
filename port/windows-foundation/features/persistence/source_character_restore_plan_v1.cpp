#include "death_checkpoint_snapshot_v1.hpp"

namespace dh::foundation::persistence::death_flow {

bool source_checkpoint_object_eligible_v1(SourceCheckpointObjectFamilyV1 family,
    bool skip_checkpoint, bool& eligible, std::string& error) {
    switch (family) {
    case SourceCheckpointObjectFamilyV1::character:
    case SourceCheckpointObjectFamilyV1::container:
    case SourceCheckpointObjectFamilyV1::door:
    case SourceCheckpointObjectFamilyV1::module:
    case SourceCheckpointObjectFamilyV1::native_trigger:
        eligible=!skip_checkpoint;error.clear();return true;
    case SourceCheckpointObjectFamilyV1::object_base:
    case SourceCheckpointObjectFamilyV1::area_zone:
        eligible=false;error.clear();return true;
    case SourceCheckpointObjectFamilyV1::unknown:
        error="Source checkpoint family is unknown; admission cannot be inferred";return false;
    }
    error="Source checkpoint family value is invalid";return false;
}

bool source_character_restore_plan_v1(
    std::uint32_t saved_state, bool saved_dead,
    bool already_limbus, bool already_pre_spawn,
    std::optional<bool> can_respawn, std::optional<bool> physical_body_present,
    SourceCharacterRestorePlanV1& output, std::string& error) {
    SourceCharacterRestorePlanV1 plan;
    plan.saved_state=saved_state;
    plan.saved_dead=saved_dead;
    // Character::Deserialize stores the serialized Character+5193 value
    // before evaluating the state-specific branch below.
    plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::restore_serialized_dead_bit);
    // Character::Deserialize branches on (1u << savedCSstate) only through
    // state 17. Larger/corrupt values take the ordinary saved-dead/alive path;
    // never shift by an unbounded serialized value.
    const std::uint32_t state_bit=saved_state<=17 ? (std::uint32_t(1u)<<saved_state) : 0u;
    if ((state_bit & 0x1005u)!=0u) {
        plan.resulting_limbus=true;
        if (!already_limbus) {
            plan.requires_can_respawn=true;
            plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::can_respawn_gate);
            if (!can_respawn) { error="Character::CanRespawn source group permission is unavailable"; return false; }
            plan.can_respawn_allowed=*can_respawn;
            plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::set_limbus_if_not_already);
        }
        // Deserialize writes +5193=true for this mask after restoring the
        // saved field; it does not zero HP here. Model the explicit Limbus
        // exception separately from ordinary corpses.
        plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::set_source_dead_true);
        plan.resulting_dead=true;
    } else if ((state_bit & 0x20002u)!=0u) {
        if (!already_pre_spawn) plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::set_pre_spawn_if_not_already);
        // The saved Character+5193 dead value is retained on this branch.
        plan.resulting_dead=saved_dead;
    } else if (saved_dead) {
        plan.resulting_limbus=true;
        plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::set_source_dead_true);
        plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::set_hp_zero);
        plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::set_ai_dead);
        plan.requires_can_respawn=true;
        plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::can_respawn_gate);
        if (!can_respawn) { error="Character::CanRespawn source group permission is unavailable"; return false; }
        plan.can_respawn_allowed=*can_respawn;
        plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::set_limbus_if_not_already);
        plan.resulting_dead=true;
    } else {
        if (!physical_body_present) {
            error="Character::Deserialize requires current physical-body presence for the saved-alive branch";
            return false;
        }
        if (!*physical_body_present)
            plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::initialize_physical_if_missing);
        plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::set_idle_zero);
        plan.resulting_dead=false;
    }
    // Common tail of all branches, clearing the saved target.
    plan.ordered_operations.push_back(SourceCharacterRestoreOpV1::clear_target);
    output=std::move(plan);
    error.clear();
    return true;
}

} // namespace dh::foundation::persistence::death_flow
