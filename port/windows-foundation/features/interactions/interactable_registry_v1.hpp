#pragma once

// P16 CONTEXT: interaction-type provider registry. Any world object that can be an object of interest
// (chests, barrels, NPCs, items, triggers, ...) registers here with the same contract as the source
// GetInteractionType (vt+144(object, viewer)). The OOI owner reads the registry; it never knows the class.
//
// Classes own their registration (containers agent: chests/barrels; NPC agent: friendly characters; triggers
// with their data-driven type). A provider returns -1 for "not interactive" (source default).

#include "../../actor_state.hpp"
#include "../combat/object_of_interest_owner_v1.hpp"

#include <array>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace dh::foundation {

struct InteractableProviderV1 {
    // Required. Source vt+144(object, viewer). -1 = not an action target.
    std::function<int(ActorId viewer)> interaction_type;
    // Optional. Source candidate+956 == viewer (the type-1 accept clause).
    std::function<bool(ActorId viewer)> type1_targets_viewer;
};

struct InteractableEntryV1 {
    ActorId id = invalid_actor_id;
    // Characters are queued before objects (source flag bit). Only characters use the character path.
    bool is_character = false;
    std::array<float, 3> position{};
    float radius = 0.0f;   // source vt+148 target radius
    bool eligible = true;  // object is currently usable (e.g. a chest not yet opened)
    InteractableProviderV1 provider;
};

class InteractableRegistryV1 {
public:
    // Registers or replaces an entry. Rejects a missing provider.
    bool upsert(InteractableEntryV1 entry, std::string& error);
    void erase(ActorId id) noexcept;
    void clear() noexcept;
    std::size_t size() const noexcept { return entries_.size(); }
    // Candidates for the OOI owner. Types come from the providers for this viewer.
    std::vector<ObjectOfInterestCandidateV1> candidates(ActorId viewer) const;

private:
    std::unordered_map<ActorId, InteractableEntryV1> entries_;
};

} // namespace dh::foundation
