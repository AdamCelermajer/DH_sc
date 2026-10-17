#pragma once
#include <cmath>
#include <cstdint>

namespace dh2::item_world_touch_v1 {

enum class ContactEvent { begin, persist, end, other };

// ItemObject::OnCollisionBegins performs the one-shot touch decision. It has
// no item-level persist callback; OnCollisionEnds re-arms a later entry.
inline bool queue_interact(ContactEvent event, bool is_player, bool eligible,
                           bool& contact_active) noexcept {
    if (event == ContactEvent::end) {
        contact_active = false;
        return false;
    }
    if (event != ContactEvent::begin || !is_player || contact_active) return false;
    contact_active = true;
    return eligible;
}

// CharAI::AI_IsInInteractionRange order for an ordinary target: 3D distance,
// subtract owner melee and target interaction radii, then compare against the
// owner's AI InteractRadius. ItemObject inherits GameObject's interaction
// type -1, so it uses the ordinary threshold path (type 8 is a distinct case).
inline bool in_interaction_range(const float owner[3],const float target[3],
 float owner_melee_radius,float target_interaction_radius,float interact_radius,
 std::uint32_t interaction_type) noexcept {
 volatile float x=owner[0]-target[0];
 volatile float y=owner[1]-target[1];
 volatile float z=owner[2]-target[2];
 volatile float xx=x*x,yy=y*y,xy=xx+yy,zz=z*z,squared=xy+zz;
 volatile float distance=std::sqrt(float(squared));
 volatile float after_owner=distance-owner_melee_radius;
 volatile float remaining=after_owner-target_interaction_radius;
 return remaining<=(interaction_type==8?0.f:interact_radius);
}

// ItemObject::Interact accepts a null receiver or the exact resolved
// Character supplied to ItemObject::InitAgain/ItemManager::Spawn.
inline bool receiver_allows_interact(std::uintptr_t receiver_identity,
 std::uintptr_t character_identity) noexcept {
 return character_identity!=0&&
        (receiver_identity==0||receiver_identity==character_identity);
}

} // namespace dh2::item_world_touch_v1
