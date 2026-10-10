#pragma once

#include "equipment_menu.hpp"
#include "../inventory/inventory_menu.hpp"
#include "../inventory/inventory_details.hpp"
#include <functional>

namespace dh::foundation::equipment_menu {

enum class MainPageCommand {
    none,
    equipped,
    unequipped,
    request_drop,
    // Details btn_AutoEquip: NativeInvAutoEquipSlot(selected slot).
    request_auto_equip,
    request_transmute,
    // Main sheet btn_GAMEPLAYMENUS_AUTOEQUIP_ALL: NativeInvAutoEquipSlot(-1).
    request_auto_equip_all
};

// One-line log description of the active equipment bindings: "slot1=Longsword01/sword slot0=...".
std::string describe_equipment(const CharacterState&);

// Optional root binding to the already-published canonical Character/Gear.
// `equip` receives the selected projection ID and source slot so the root can
// resolve that ID against the same native FreshInventoryOwnedV4 and call the
// same PlayerEquipmentRenderOwnerV1. Each callback must finish any required
// same-owner projection refresh before returning success. No native owner or
// item identity is retained by MainPage.
struct NativeEquipmentActions {
    std::function<bool(const std::string& instance_id, unsigned source_slot,
                       std::string& error)> equip;
    std::function<bool(unsigned source_slot, std::string& error)> unequip;
    bool bound() const noexcept { return bool(equip) && bool(unequip); }
    bool partial() const noexcept { return bool(equip) != bool(unequip); }
};

// Composes the original equipment MAINPAGE over the caller's one inventory,
// equipment-selection and Details owners. It owns no CharacterState, ItemTable,
// PropertyState, adapter, inventory item identity, or text/localization owner.
// The caller must keep all presenters and shared text services alive longer
// than this binding.
class MainPage {
    Presenter& equipment_;
    inventory::MenuPresenter& inventory_;
    inventory::DetailsPresenter& details_;
    const inventory::MenuBindings& inventory_text_;
    const inventory::DetailBindings& details_text_;
    NativeEquipmentActions native_actions_;
public:
    MainPage(Presenter&, inventory::MenuPresenter&, inventory::DetailsPresenter&,
             const inventory::MenuBindings&, const inventory::DetailBindings&,
             NativeEquipmentActions = {});

    // Suitable for character_menu::Bindings.content. Other tabs pass through.
    bool content(character_menu::Tab, character_menu::Frame&, std::string& error);
    std::function<bool(character_menu::Tab, character_menu::Frame&, std::string&)> content_callback(
        std::function<bool(character_menu::Tab, character_menu::Frame&, std::string&)> other_tabs = {});

    // Coordinates are already converted to authored 480x320 menu space.
    // Equipment and valuables hits open the existing DetailsPresenter; row and
    // arrow hits update its existing equipment Presenter selection. Equip and
    // unequip execute through that presenter's existing EquipmentAdapter.
    bool release(float authored_x, float authored_y, MainPageCommand&, std::string& error);
    void leave_page() noexcept { details_.close(); }
};

} // namespace dh::foundation::equipment_menu
