#pragma once

#include "equipment_main_page.hpp"
#include "../actor_frame/source_character_owner_factory.hpp"
#include "../inventory/source_character_inventory_binding.hpp"

#include <memory>

namespace dh::foundation::equipment_menu {

struct NativeEquipmentVitals {
    std::int32_t hp{}, max_hp{}, mp{}, max_mp{};
};

// Root projection refresh after a native Gear attempt. Called on both success
// and failure because the original source path can leave a reached mutation
// prefix before a later property/skin/vitals continuation rejects.
using NativeEquipmentProjectionRefresh = std::function<bool(
    const features::SourceCharacterOwnerAliases&, std::string&)>;

class NativeGearActionBinding final :
    public std::enable_shared_from_this<NativeGearActionBinding> {
    std::shared_ptr<void> owner_token_;
    CharacterState& projection_;
    const features::SourceCharacterOwnerFactory& factory_;
    std::uintptr_t identity_{};
    inventory::SourceCharacterInventoryBinding& inventory_binding_;
    NativeEquipmentProjectionRefresh refresh_projection_;
    bool graph(features::SourceCharacterOwnerAliases&, std::string&) const;
    bool current_vitals(const features::SourceCharacterOwnerAliases&,
                        NativeEquipmentVitals&, std::string&) const;
    bool finish_attempt(const features::SourceCharacterOwnerAliases&,
                        bool action_succeeded, const std::string& action_error,
                        std::string&) const;
    bool equip(const std::string&, unsigned, std::string&);
    bool unequip(unsigned, std::string&);
public:
    NativeGearActionBinding(std::shared_ptr<void> canonical_owner,
        CharacterState& current_projection,
        const features::SourceCharacterOwnerFactory&, std::uintptr_t character_identity,
        inventory::SourceCharacterInventoryBinding&,
        NativeEquipmentProjectionRefresh);

    // Initial bind after the root has projected the completed source owner.
    // The retained SourceCharacterInventoryBinding owns pointer/ID validation.
    // Refresh it after source item changes; rebuild it/binding after owner
    // replacement or GEAR restore.
    bool bind_projection(std::string& error);
    bool ready(std::string& error) const;
    bool current_vitals(NativeEquipmentVitals&, std::string& error) const;
    NativeEquipmentActions callbacks();
};

} // namespace dh::foundation::equipment_menu
