#pragma once

// P16 CONTEXT: walk-over pickup of ground items (replaces the E-key pickup adaptation).
//
// Source (CONTEXT-report.md section 1.6):
// - POItem::onCollisionBegins 0x4702a8 -> ItemObject::OnCollisionBegins 0x3ec048: on a contact that BEGINS, if the
//   item is not looted and GetInteractionType == -1 and CharStateMachine::SM_IsMoving(character), then
//   item+740 = character. A contact while standing still stores nothing.
// - GameObject::Update 0x38cbe8 consumes item+740 on the item's next update: vt+152 = ItemObject::Interact
//   (with the original gates, which the caller applies through interact_world_item_v1), then clears it.
//
// This tracker models that sequence per character: a stored pickup is returned on the frame after its contact
// began, and only one stored pickup per item. Contact detection (the AABB sensor) belongs to the caller.

#include <cstdint>
#include <functional>
#include <vector>

namespace dh::foundation::loot {

class WorldItemContactTrackerV1 {
public:
    using ItemId = std::uint64_t;

    // One frame. `contacts` = item ids whose sensor overlaps the character this frame. `moving` = SM_IsMoving.
    // `available(item)` = item still on the ground (not looted / not removed).
    // Returns the items to run ItemObject::Interact on now (stored by an earlier frame's contact).
    std::vector<ItemId> advance(std::uint64_t character, bool moving, const std::vector<ItemId>& contacts,
                                const std::function<bool(ItemId)>& available);

    void reset() noexcept;

private:
    std::uint64_t character_ = 0;
    bool has_character_ = false;
    std::vector<ItemId> previous_contacts_;
    std::vector<ItemId> stored_;  // item+740 stored for the next update
};

} // namespace dh::foundation::loot
