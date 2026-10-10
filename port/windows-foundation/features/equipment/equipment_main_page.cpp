#include "equipment_main_page.hpp"
#include <cmath>
#include <utility>

namespace dh::foundation::equipment_menu {

MainPage::MainPage(Presenter& equipment, inventory::MenuPresenter& inventory,
                   inventory::DetailsPresenter& details,
                   const inventory::MenuBindings& inventory_text,
                   const inventory::DetailBindings& details_text,
                   NativeEquipmentActions native_actions)
    : equipment_(equipment), inventory_(inventory), details_(details),
      inventory_text_(inventory_text), details_text_(details_text),
      native_actions_(std::move(native_actions)) {}

bool MainPage::content(character_menu::Tab tab, character_menu::Frame& frame,
                       std::string& error) {
    if (tab != character_menu::Tab::equipment) {
        error.clear();
        return true;
    }
    // The equipment presenter owns the nine source slots and their selected
    // item text. The inventory provider adds only category9 potions here.
    if (!equipment_.frame(frame, error) ||
        !inventory_.potions_content(inventory_text_, frame, error)) return false;
    if (details_.is_open() && !details_.frame(details_text_, frame, error)) return false;
    error.clear();
    return true;
}

std::function<bool(character_menu::Tab, character_menu::Frame&, std::string&)>
MainPage::content_callback(
    std::function<bool(character_menu::Tab, character_menu::Frame&, std::string&)> other_tabs) {
    return [this, other_tabs = std::move(other_tabs)](character_menu::Tab tab,
            character_menu::Frame& frame, std::string& error) {
        if (tab == character_menu::Tab::equipment) return content(tab, frame, error);
        if (other_tabs) return other_tabs(tab, frame, error);
        error.clear();
        return true;
    };
}

bool MainPage::release(float x, float y, MainPageCommand& command, std::string& error) {
    command = MainPageCommand::none;
    if (!std::isfinite(x) || !std::isfinite(y)) {
        error = "Invalid authored character menu release coordinates";
        return false;
    }

    if (details_.is_open()) {
        inventory::DetailAction action{};
        if (!details_.release(x, y, action, error)) return false;
        switch (action) {
        case inventory::DetailAction::equip:
            if (native_actions_.partial()) {
                error = "Canonical equipment action binding requires both equip and unequip";
                return false;
            }
            if (native_actions_.bound()) {
                const auto& id = equipment_.selected_instance();
                const auto slot = equipment_.selected_slot();
                if (id.empty() || slot >= 9) {
                    error = "Canonical equipment action requires the selected source item and slot";
                    return false;
                }
                if (!native_actions_.equip(id, slot, error)) return false;
                command = MainPageCommand::equipped;
                error.clear();
                return true;
            }
            if (!equipment_.equip_selected(error)) return false;
            command = MainPageCommand::equipped;
            return true;
        case inventory::DetailAction::unequip:
            if (native_actions_.partial()) {
                error = "Canonical equipment action binding requires both equip and unequip";
                return false;
            }
            if (native_actions_.bound()) {
                const auto slot = equipment_.selected_slot();
                if (slot >= 9) {
                    error = "Canonical equipment action requires a selected source slot";
                    return false;
                }
                if (!native_actions_.unequip(slot, error)) return false;
                command = MainPageCommand::unequipped;
                error.clear();
                return true;
            }
            if (!equipment_.unequip_selected_slot(error)) return false;
            command = MainPageCommand::unequipped;
            return true;
        case inventory::DetailAction::drop:
            command = MainPageCommand::request_drop;
            error.clear();
            return true;
        case inventory::DetailAction::auto_equip:
            command = MainPageCommand::request_auto_equip;
            error.clear();
            return true;
        case inventory::DetailAction::transmute:
            command = MainPageCommand::request_transmute;
            error.clear();
            return true;
        case inventory::DetailAction::select:
        case inventory::DetailAction::previous:
        case inventory::DetailAction::next:
            error.clear();
            return true;
        case inventory::DetailAction::none:
            break;
        }
    }

    const unsigned equipment_slot = equipment_.hit_test(x, y);
    if (equipment_slot < 9) return details_.open(equipment_slot, error);

    const auto inventory_slot = inventory_.hit_slot(x, y);
    if (inventory_slot < 0 || inventory_slot > 9) {
        error.clear();
        return true;
    }
    if (!inventory_.release_slot(x, y, error)) return false;
    return details_.open(static_cast<unsigned>(inventory_slot), error);
}

} // namespace dh::foundation::equipment_menu
