#include "native_gear_actions.hpp"

#include "../../../level-world/canonical_character_candidate_v60.hpp"
#include "../../../level-world/player_equipment_render_owner_v1.hpp"

#include <utility>

namespace dh::foundation::equipment_menu {
namespace {
bool same_property_cells(const dh2::data::PropertyView* a,
                         const dh2::data::PropertyView* b) noexcept {
    return a && b && a->base == b->base && a->saved == b->saved &&
           a->gear == b->gear && a->resolved == b->resolved;
}
}

NativeGearActionBinding::NativeGearActionBinding(
    std::shared_ptr<void> owner, CharacterState& projection,
    const features::SourceCharacterOwnerFactory& factory,
    std::uintptr_t identity,
    inventory::SourceCharacterInventoryBinding& inventory_binding,
    NativeEquipmentProjectionRefresh refresh)
    : owner_token_(std::move(owner)), projection_(projection),
      factory_(factory), identity_(identity), inventory_binding_(inventory_binding),
      refresh_projection_(std::move(refresh)) {}

bool NativeGearActionBinding::graph(
    features::SourceCharacterOwnerAliases& aliases,
    std::string& error) const {
    if (!owner_token_) {
        error = "Native equipment binding requires the canonical selected-character owner token";
        return false;
    }
    if (!factory_.borrow_completed_character(identity_, aliases, error)) return false;
    if (!aliases.validate(error)) return false;
    auto record = aliases.lifetime;
    if (!record || record.get() != owner_token_.get() ||
        !record->init_complete || record->failed || !record->equipment ||
        record->prepared_equipment_v60 != record->equipment.get()) {
        error = "Equipment page requires the same completed canonical V60 record and Gear";
        return false;
    }
    bool is_player = false;
    if (!record->is_player(is_player, error) || !is_player) {
        if (error.empty()) error = "Native equipment page requires the canonical player Character";
        return false;
    }
    auto& gear = *record->equipment;
    const auto* inventory_owner = gear.inventory();
    const auto& properties = gear.properties();
    const auto* gear_view = gear.property_view();
    if (!gear.ready() || !gear.prepared_v60() ||
        inventory_owner != aliases.inventory37c ||
        !properties || properties.get() != aliases.properties ||
        !same_property_cells(gear_view, aliases.property_view) ||
        !gear.item_text_owner_v1()) {
        error = "Native Gear, constructor inventory, current sheets, and typed source inventory binding must be the same completed owner graph";
        return false;
    }
    error.clear();
    return true;
}

bool NativeGearActionBinding::current_vitals(
    const features::SourceCharacterOwnerAliases& aliases,
    NativeEquipmentVitals& out, std::string& error) const {
    if (!aliases.properties || !aliases.property_view || !aliases.lifetime ||
        !aliases.lifetime->equipment ||
        aliases.lifetime->equipment->properties().get() != aliases.properties ||
        !same_property_cells(aliases.lifetime->equipment->property_view(),
                             aliases.property_view)) {
        error = "Current HP/MP must borrow the same Gear/Character PropertyState cells";
        return false;
    }
    NativeEquipmentVitals next;
    // These are the source resolved PropertyState cells read by Gear's native
    // vitals clamp; they are intentionally kept as integer source values.
    next.hp = aliases.properties->resolved[36];
    next.max_hp = aliases.properties->resolved[38];
    next.mp = aliases.properties->resolved[41];
    next.max_mp = aliases.properties->resolved[43];
    out = next;
    error.clear();
    return true;
}

bool NativeGearActionBinding::current_vitals(
    NativeEquipmentVitals& out, std::string& error) const {
    features::SourceCharacterOwnerAliases aliases;
    return graph(aliases, error) && current_vitals(aliases, out, error);
}

bool NativeGearActionBinding::bind_projection(std::string& error) {
    features::SourceCharacterOwnerAliases aliases;
    if (!graph(aliases, error)) return false;
    return inventory_binding_.bind_initial(projection_, error);
}

bool NativeGearActionBinding::ready(std::string& error) const {
    features::SourceCharacterOwnerAliases aliases;
    if (!graph(aliases, error)) return false;
    if (!inventory_binding_.projection_bound()) {
        error = "Canonical inventory projection is not bound to the completed source Gear owner";
        return false;
    }
    NativeEquipmentVitals current;
    return current_vitals(aliases, current, error);
}

bool NativeGearActionBinding::finish_attempt(
    const features::SourceCharacterOwnerAliases& aliases, bool action_succeeded,
    const std::string& action_error, std::string& error) const {
    std::string refresh_error;
    bool refreshed = refresh_projection_ && refresh_projection_(aliases, refresh_error);
    if (!refresh_projection_ && refresh_error.empty())
        refresh_error = "Same-owner equipment projection refresh callback unavailable";
    if (refreshed)
        refreshed = inventory_binding_.refresh_after_native_mutation(projection_, refresh_error);
    if (!action_succeeded) {
        error = action_error.empty() ? "Native Gear action rejected" : action_error;
        if (!refreshed) error += "; read-model refresh failed: " + refresh_error;
        return false;
    }
    if (!refreshed) {
        error = "Native Gear committed, but same-owner read-model refresh failed: " + refresh_error;
        return false;
    }
    error.clear();
    return true;
}

bool NativeGearActionBinding::equip(const std::string& id, unsigned slot,
                                    std::string& error) {
    if (id.empty() || slot >= 9) {
        error = "Native equip requires a selected stable instance ID and source slot 0..8";
        return false;
    }
    features::SourceCharacterOwnerAliases aliases;
    if (!graph(aliases, error)) return false;
    if (!inventory_binding_.projection_bound()) {
        error = "Native equip requires a current completed source inventory projection";
        return false;
    }
    NativeEquipmentVitals before;
    if (!current_vitals(aliases, before, error)) return false;

    inventory::SourceInstanceLease instance;
    auto resolve = inventory_binding_.resolver_callback();
    if (!resolve || !resolve(id, instance, error)) return false;
    const auto* inventory_owner = aliases.inventory37c;
    auto& gear = *aliases.lifetime->equipment;
    if (!instance.owner || !instance.item || !instance.powers ||
        instance.text_owner != gear.item_text_owner_v1() ||
        !instance.descriptors_current || !instance.owns ||
        !instance.owns(instance.item) || !inventory_owner ||
        instance.source_index >= inventory_owner->items().size() ||
        !inventory_owner->items()[instance.source_index] ||
        inventory_owner->items()[instance.source_index]->item.get() != instance.item) {
        error = "Selected stable ID no longer resolves to the same actual native inventory item";
        return false;
    }
    inventory_binding_.invalidate_before_native_mutation();
    std::string action_error;
    const bool succeeded = gear.equip(slot, instance.source_index, action_error);
    return finish_attempt(aliases, succeeded, action_error, error);
}

bool NativeGearActionBinding::unequip(unsigned slot, std::string& error) {
    if (slot >= 9) {
        error = "Native unequip requires source slot 0..8";
        return false;
    }
    features::SourceCharacterOwnerAliases aliases;
    if (!graph(aliases, error)) return false;
    if (!inventory_binding_.projection_bound()) {
        error = "Native unequip requires a current completed source inventory projection";
        return false;
    }
    NativeEquipmentVitals before;
    if (!current_vitals(aliases, before, error)) return false;

    auto& gear = *aliases.lifetime->equipment;
    inventory_binding_.invalidate_before_native_mutation();
    std::string action_error;
    const bool succeeded = gear.unequip(slot, action_error);
    return finish_attempt(aliases, succeeded, action_error, error);
}

NativeEquipmentActions NativeGearActionBinding::callbacks() {
    auto self = shared_from_this();
    NativeEquipmentActions result;
    result.equip = [self](const std::string& id, unsigned slot,
                          std::string& error) {
        return self->equip(id, slot, error);
    };
    result.unequip = [self](unsigned slot, std::string& error) {
        return self->unequip(slot, error);
    };
    return result;
}

} // namespace dh::foundation::equipment_menu
