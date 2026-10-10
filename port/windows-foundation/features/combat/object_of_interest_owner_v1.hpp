#pragma once

// Object-of-interest (OOI) owner and rendered target-marker policy (B004/B029, extended by P16 CONTEXT).
//
// Source: Character::UpdateObjectOfInterest (0x3abb9c). The OOI lives at Character+0x14a4, the 500 ms
// refresh timer at +0x14aa, and the cached candidate interaction type at +0x14a8 (read by the HUD action
// icon). Candidates come from one TargetList query: radius CharacterDesign.OOI_Distance (200) measured as
// centre distance minus candidate radius minus owner melee radius, full 2*pi arc, sorted by
// TargetSorter::_sortFrontal: Characters first (flag bit), then ascending angle from the owner's look vector.
// Distance is not used for ranking. See coordination/claude-preview16/CONTEXT-report.md section 1.1.
//
// This owner never writes combat state. Combat target, last/preferred target, sticky target and the
// rendered marker stay separate values.

#include "../../actor_state.hpp"
#include "auto_target_marker_v1.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <vector>

namespace dh::foundation {

// Character+0x14aa reset value in UpdateObjectOfInterest.
inline constexpr float object_of_interest_refresh_ms_v1 = 500.0f;
// CharacterDesign.OOI_Distance (port/script-runtime/reference/design-bindings/cache-constant-inventory.json).
inline constexpr float object_of_interest_distance_v1 = 200.0f;

// Source interaction-type codes (Character/GameObject::GetInteractionType, vt+144).
inline constexpr int interaction_type_none_v1 = -1;        // no action (items, summoned, GameObject default)
inline constexpr int interaction_type_trigger_v1 = 1;      // data-driven TriggerObject accept rule
inline constexpr int interaction_type_openable_v1 = 0;     // OpenableContainer (chest)
inline constexpr int interaction_type_npc_talk_v1 = 3;     // friendly Character (talk)
inline constexpr int interaction_type_attack_v1 = 8;       // enemy/monster Character, DestructibleContainer

struct ObjectOfInterestCandidateV1 {
    ActorId id = invalid_actor_id;
    // Characters are queued before non-character objects (source TargetInfo flag bit 0).
    bool is_character = false;
    std::array<float, 3> position{};
    // Source vt+148 target radius and the owner's melee radius are subtracted from the centre distance.
    float radius = 0.0f;
    // Source vt+144(candidate, owner). Non-character candidates with -1 must be filtered by the caller.
    int interaction_type = interaction_type_none_v1;
    // Source candidate+956 == owner: the type-1 accept clause.
    bool type1_targets_owner = false;
    // Characters: enemy/friend/merchant gate (source mask 89). Objects: any non-(-1) type is already admitted.
    bool eligible = false;
};

struct ObjectOfInterestOwnerStateV1 {
    ActorId owner = invalid_actor_id;
    std::array<float, 3> position{};
    // Owner look vector (GetLookAtVec). Need not be normalised.
    std::array<float, 3> look{0.0f, 1.0f, 0.0f};
    // Owner AI melee radius (MeleeRadius in TargetList::Search). Zero when unknown.
    float melee_radius = 0.0f;
};

class ObjectOfInterestOwnerV1 {
public:
    // Advances the refresh timer by dt_seconds. On expiry the timer restarts at 500 ms, the OOI and the cached
    // type are cleared, and the source candidate loop is re-run over `candidates`. A change of owner resets
    // the state so the first update refreshes immediately.
    void update(const ObjectOfInterestOwnerStateV1& owner, double dt_seconds,
                const std::vector<ObjectOfInterestCandidateV1>& candidates) noexcept;
    void reset() noexcept;
    ActorId owner() const noexcept { return owner_; }
    ActorId object() const noexcept { return object_; }
    // Cached candidate type (Character+0x14a8). -1 when there is no OOI.
    int interaction_type() const noexcept { return type_; }
    // True for the refresh where the OOI changed (Character+0x14ad).
    bool changed() const noexcept { return changed_; }

private:
    ActorId owner_ = invalid_actor_id;
    ActorId object_ = invalid_actor_id;
    int type_ = interaction_type_none_v1;
    bool changed_ = false;
    float timer_ms_ = 0.0f;
};

// Pure source candidate loop (UpdateObjectOfInterest 126139-126178). `ordered` must be in
// _sortFrontal order (see order_object_of_interest_candidates_v1). Returns the OOI and cached type.
struct ObjectOfInterestSelectionV1 {
    ActorId object = invalid_actor_id;
    int interaction_type = interaction_type_none_v1;
};
ObjectOfInterestSelectionV1 select_object_of_interest_v1(ActorId owner,
                                                         const std::vector<ObjectOfInterestCandidateV1>& ordered) noexcept;

// Builds the queue of candidates that the source TargetList would hold: eligible, in range, and in
// _sortFrontal order (characters first, then ascending angle to the owner's look vector).
std::vector<ObjectOfInterestCandidateV1> order_object_of_interest_candidates_v1(
    const ObjectOfInterestOwnerStateV1& owner, const std::vector<ObjectOfInterestCandidateV1>& candidates);

// Marker precedence: last target (only when its projection is known, non-null and not the player),
// else the OOI. The chosen actor must pass the render-time eligibility gate, otherwise the marker is
// hidden (invalid_actor_id). A rejected last target does not fall back to the OOI.
ActorId resolve_rendered_target_marker_v1(ActorId player, bool last_known, ActorId last_target,
                                          ActorId object_of_interest,
                                          const std::function<bool(ActorId)>& eligible_now) noexcept;

} // namespace dh::foundation
