#pragma once

#include "player_skill_progression_v1.hpp"
#include "../game-data/player_saved_skill_slots_v1.hpp"

namespace dh2::player_initial_skill_grants_v1 {

using Status = player_skill_progression_v1::Status;

struct FullRecalculation {
    void* context{};
    int (*invoke)(void*, data::PropertyView*, bool full, std::string&){};
};

// Composition of the existing source callers and saved-slot writer. No new
// PlayerSavegame, PropertyState, inventory, profile, skill table, VM or FSM is
// allocated. current_savegame is Character+14e8's live source pointer slot;
// old/new owners stay alive through synchronous callbacks. Null is a normal
// source pointer for the outer _InitSkillsSlots wrappers.
struct Bindings {
    std::uintptr_t character{};
    data::PlayerSavegameV1* const* current_savegame{};
    const data::PropertyRules* rules{};
    data::PropertyState* properties{};
    // The existing runtime's actual view, including live buff group storage.
    // Its six sheets must point to rules/properties above. The adapter never
    // reconstructs a group-free view. Providers may refresh group descriptors
    // in this same view while retaining all borrowed storage.
    data::PropertyView* view{};
    const data::SkillTables* tables{};
    data::FreshInventoryOwnedV4* inventory{};
    data::SavedSkillUpdateServicesV1 slot_updates{};
    FullRecalculation full_recalculation{};
    // Reached design skill_limit, AI_UpdateAllSkills and Debug load/query
    // remain mandatory actual services. Full PROPS_Recalc(true) uses the typed
    // callback above and receives the exact same view with buff groups. This
    // adapter handles
    // the existing save/slot/inventory getters and source property157 AddInt
    // and potion byte stores itself. current_savegame in this Services struct
    // is unused; the binding's one live pointer slot is authoritative.
    player_skill_progression_v1::Services effects{};
};

struct Result {
    player_skill_progression_v1::InitSlotsResult slots{};
    player_skill_progression_v1::Result increment{};
    std::uint32_t slot_assignments_attempted{}, equipment_swaps{},
                  increment_attempts{}, property_adds{}, potion_stores{},
                  effect_calls{};
    Status dependency_status{Status::complete};
};

// Delegates _InitSkillsSlots0x3b3a90 and IncSkill0x3bcc58 to progression_v1 and
// source map-zero writes to BoundSlotsV1. Initial points, level, list and saved
// levels must come from the actual profile/class/InitPost producers. The
// source may normally decline the increment; this never sets level1 itself,
// creates points, assigns a second saved map or publishes Player readiness.
// Source effects and nested error prefixes survive provider failure. Bindings
// and Result must be disjoint from all borrowed storage. One owning thread;
// callbacks retain this inventory/property/save graph and cannot destroy it.
// Full profile SG_Load, InitPost and campaign routing remain external callers.
Status initialize(const Bindings*, Result*, std::string& error) noexcept;

} // namespace dh2::player_initial_skill_grants_v1
