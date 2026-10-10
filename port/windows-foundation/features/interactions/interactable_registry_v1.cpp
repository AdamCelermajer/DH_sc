#include "interactable_registry_v1.hpp"

#include <algorithm>

namespace dh::foundation {

bool InteractableRegistryV1::upsert(InteractableEntryV1 entry, std::string& error) {
    error.clear();
    if (entry.id == invalid_actor_id) {
        error = "Interactable requires an object id";
        return false;
    }
    if (!entry.provider.interaction_type) {
        error = "Interactable provider has no interaction_type function";
        return false;
    }
    const auto id = entry.id;
    entries_[id] = std::move(entry);
    return true;
}

void InteractableRegistryV1::erase(ActorId id) noexcept { entries_.erase(id); }

void InteractableRegistryV1::clear() noexcept { entries_.clear(); }

std::vector<ObjectOfInterestCandidateV1> InteractableRegistryV1::candidates(ActorId viewer) const {
    std::vector<ObjectOfInterestCandidateV1> out;
    out.reserve(entries_.size());
    for (const auto& pair : entries_) {
        const auto& e = pair.second;
        if (e.id == viewer) continue;
        ObjectOfInterestCandidateV1 c;
        c.id = e.id;
        c.is_character = e.is_character;
        c.position = e.position;
        c.radius = e.radius;
        c.eligible = e.eligible;
        c.interaction_type = e.provider.interaction_type(viewer);
        c.type1_targets_owner = e.provider.type1_targets_viewer ? e.provider.type1_targets_viewer(viewer) : false;
        out.push_back(c);
    }
    // Deterministic input order for the owner's stable sort (ties by id).
    std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.id < b.id; });
    return out;
}

} // namespace dh::foundation
