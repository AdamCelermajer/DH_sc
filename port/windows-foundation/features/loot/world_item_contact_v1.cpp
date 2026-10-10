#include "world_item_contact_v1.hpp"

#include <algorithm>

namespace dh::foundation::loot {

void WorldItemContactTrackerV1::reset() noexcept {
    character_ = 0;
    has_character_ = false;
    previous_contacts_.clear();
    stored_.clear();
}

std::vector<WorldItemContactTrackerV1::ItemId> WorldItemContactTrackerV1::advance(
    std::uint64_t character, bool moving, const std::vector<ItemId>& contacts,
    const std::function<bool(ItemId)>& available) {
    if (!has_character_ || character_ != character) {
        reset();
        character_ = character;
        has_character_ = true;
    }

    // GameObject::Update: an item stored by an earlier contact runs Interact on its next update.
    std::vector<ItemId> due;
    for (const auto item : stored_)
        if (available && available(item)) due.push_back(item);
    stored_.clear();

    // ItemObject::OnCollisionBegins: only a contact that BEGINS, on a looted-free item, while moving.
    for (const auto item : contacts) {
        const bool began = std::find(previous_contacts_.begin(), previous_contacts_.end(), item) == previous_contacts_.end();
        if (!began || !moving || !(available && available(item))) continue;
        if (std::find(stored_.begin(), stored_.end(), item) == stored_.end()) stored_.push_back(item);
    }
    previous_contacts_ = contacts;
    return due;
}

} // namespace dh::foundation::loot
