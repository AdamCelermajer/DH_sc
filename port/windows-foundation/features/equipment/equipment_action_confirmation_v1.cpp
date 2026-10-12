#include "equipment_action_confirmation_v1.hpp"
#include <algorithm>

namespace dh::foundation::equipment_menu::confirmation_v1 {

bool Pending::open(Action action, const InventoryItem& item, unsigned source_slot,
                   std::int32_t transmute_value, std::uint64_t source_visit_epoch,
                   std::string& error) {
    error.clear();
    if (visible()) { error = "An equipment action confirmation is already open"; return false; }
    if ((action != Action::transmute && action != Action::drop) || item.instance_id.empty() ||
        item.definition_id.empty() || item.quantity == 0 || source_slot > 9) {
        error = "Equipment confirmation requires an admitted current item action";
        return false;
    }
    request_ = {action, item.instance_id, item.definition_id, item.quantity,
                source_slot, transmute_value, source_visit_epoch, item};
    return true;
}

Release Pending::release(Button button, const CharacterState& character,
                         std::uint64_t current_source_visit_epoch,
                         const CanonicalCurrentCheck& current, std::string& error) {
    error.clear();
    if (!visible() || button == Button::other) return {};
    if (button == Button::no) {
        cancel();
        return {Outcome::cancelled, {}};
    }
    if (current_source_visit_epoch != request_.source_visit_epoch) {
        error = "Confirmed equipment action belongs to an expired source session/menu visit";
        cancel();
        return {Outcome::rejected, {}};
    }

    const auto found = std::find_if(character.inventory.begin(), character.inventory.end(),
        [&](const InventoryItem& item) { return item.instance_id == request_.instance_id; });
    if (found == character.inventory.end() || found->definition_id != request_.definition_id ||
        found->quantity != request_.quantity || found->quantity == 0) {
        error = "Confirmed equipment action item is stale or no longer owned";
        cancel();
        return {Outcome::rejected, {}};
    }
    if (std::any_of(character.equipment.begin(), character.equipment.end(), [&](const EquipmentBinding& binding) {
            return binding.item_instance_id == request_.instance_id;
        })) {
        error = "Confirmed equipment action item is equipped";
        cancel();
        return {Outcome::rejected, {}};
    }
    if (!current || !current(request_, *found, error)) {
        if (error.empty()) error = "Confirmed equipment action item is not current in the source inventory";
        cancel();
        return {Outcome::rejected, {}};
    }
    const auto confirmed = request_;
    cancel();
    return {Outcome::confirmed, confirmed};
}

} // namespace dh::foundation::equipment_menu::confirmation_v1
