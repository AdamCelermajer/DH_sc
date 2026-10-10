#pragma once
#include "source_instance_resolver.hpp"
#include "../actor_frame/source_character_owner_factory.hpp"
#include "../../../level-world/source_item_resources_v88.hpp"

namespace dh::foundation::inventory {

// The root owns the typed association between a completed V60 record and the
// SourceItemResourcesV88 that was used to prepare its Gear. Never recover it
// by downcasting PlayerEquipmentRenderInputsV1::services_lease_v62.
using SourceItemResourcesBorrow = std::function<bool(
    const dh2::world::CanonicalCharacterCandidateRecordV60&,
    std::shared_ptr<const dh2::character::SourceItemResourcesV88>&,
    std::string&)>;

// Builds the graph callback from the actual completed-character factory loan,
// the same ready V60 Gear and its existing ItemTextOwner getter, plus the
// explicitly associated shared SourceItemResources presentation owner.
SourceInventoryGraphBorrow source_character_inventory_graph_borrow(
    const dh::foundation::features::SourceCharacterOwnerFactory&,
    std::uintptr_t character_identity, SourceItemResourcesBorrow);

// Small lifecycle wrapper for a single published V60 owner. Call invalidate
// before every native inventory mutation; refresh only after the projected
// rows match the resulting native order. A GEAR restore replaces item
// identities, so it invalidates the old resolver and creates a new one.
class SourceCharacterInventoryBinding {
    SourceInventoryGraphBorrow borrow_;
    std::unique_ptr<SourceInventoryInstanceResolver> resolver_;
public:
    SourceCharacterInventoryBinding(
        const dh::foundation::features::SourceCharacterOwnerFactory&,
        std::uintptr_t character_identity, SourceItemResourcesBorrow);
    bool bind_initial(const CharacterState&, std::string& error);
    void invalidate_before_native_mutation() noexcept;
    bool refresh_after_native_mutation(const CharacterState&, std::string& error);
    bool rebind_after_gear_restore(const CharacterState&, std::string& error);
    bool projection_bound() const noexcept;
    std::function<bool(const std::string&,SourceInstanceLease&,std::string&)>
        resolver_callback() const;
};

} // namespace dh::foundation::inventory
