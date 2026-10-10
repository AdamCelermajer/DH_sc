#include "runtime_equipment_text_v1.hpp"

#include "../inventory/source_item_descriptors.hpp"
#include "../../../engine-ui/character_menu_potions_v4.hpp"
#include "../../../engine-ui/item_text_owner_v5.hpp"

#include <cstring>
#include <limits>
#include <set>

namespace dh::foundation::equipment_menu {

struct RuntimeEquipmentTextProviderV1::State {
    const dh2::data::ItemTable* table{};
    const CharacterState* profile{};
    character_menu::MenuLocalization* localization{};
    dh2::ui::HudTextV1* text{};
    dh2::ui::HudTextEnvironmentV1 environment;
    std::unique_ptr<dh2::ui::ItemTextOwnerV5> text_owner;
    std::set<std::string> supported_unpowered;
    std::string potion_format;

    bool supported(const InventoryItem& inventory, const dh2::data::Item*& row,
                   std::string& error) const {
        if (!table || !supported_unpowered.count(inventory.definition_id)) {
            error = "Runtime equipment bare-definition policy does not support this powered/generated or unknown item";
            return false;
        }
        const auto id = dh2::data::item_id(*table, inventory.definition_id);
        row = dh2::data::item(*table, id);
        if (!row || dh2::data::item_type(*row) == 13) {
            error = "Runtime equipment bare text does not support missing or gold source item definitions";
            return false;
        }
        if (!inventory.quantity || inventory.quantity > 32767) {
            error = "Runtime equipment bare item quantity is outside the original signed16 domain";
            return false;
        }
        error.clear();
        return true;
    }

    bool same_source_row(const InventoryItem& inventory, const dh2::data::Item& supplied,
                         const dh2::data::Item*& row, std::string& error) const {
        if (!supported(inventory, row, error)) return false;
        if (std::memcmp(&row->record, &supplied.record, sizeof(row->record)) != 0) {
            error = "Runtime equipment item row differs from the provider's original ItemTable";
            return false;
        }
        return true;
    }

    bool descriptors(const InventoryItem& inventory, const dh2::data::Item* supplied,
                     inventory::SourceDescriptors& output, std::string& error) const {
        const dh2::data::Item* row{};
        if (supplied) {
            if (!same_source_row(inventory, *supplied, row, error)) return false;
        } else if (!supported(inventory, row, error)) {
            return false;
        }
        if (!text_owner) {
            error = "Runtime equipment source HudText adapter is unavailable";
            return false;
        }
        return inventory::source_bare_item_descriptors(inventory, *table,
                                                       text_owner->services(), output, error);
    }

    bool name(const InventoryItem& inventory, const dh2::data::Item* supplied,
              std::string& output, std::string& error) const {
        inventory::SourceDescriptors value;
        if (!descriptors(inventory, supplied, value, error)) return false;
        output = std::move(value.name);
        error.clear();
        return true;
    }

    bool symbol(const std::string& key, std::string& output, std::string& error) const {
        if (!localization || !profile) {
            error = "Runtime equipment source MenuLocalization/profile lease is unavailable";
            return false;
        }
        return localization->symbol(key, profile, output, error);
    }

    bool details(const std::string& field, const InventoryItem& inventory,
                 std::string& output, std::string& error) const {
        inventory::SourceDescriptors value;
        if (!descriptors(inventory, nullptr, value, error)) return false;
        if (field.find("ItemInfo1") != std::string::npos) output = std::move(value.stats);
        else if (field.find("ItemReq") != std::string::npos) output = std::move(value.requirements);
        else if (field.find("ItemInfo2") != std::string::npos ||
                 field.find("ItemInfo3") != std::string::npos ||
                 field.find("ItemInfo4") != std::string::npos ||
                 field.find("ItemInfo5") != std::string::npos) {
            // These are blank for a certified bare definition. A generated
            // instance needs the source-owned power descriptor provider.
            output.clear();
        } else {
            error = "Runtime equipment source details field is outside the bare item contract";
            return false;
        }
        error.clear();
        return true;
    }

    bool potions(std::uint32_t count, std::string& output, std::string& error) const {
        if (!text || potion_format.empty()) {
            error = "Runtime equipment actual GAMEPLAYMENUS_POTIONS format is unavailable";
            return false;
        }
        if (count > static_cast<std::uint32_t>(std::numeric_limits<std::int32_t>::max())) {
            error = "Source potion count exceeds the original signed integer formatter domain";
            return false;
        }
        return dh2::ui::character_menu_potion_integer_text_v4(
            *text, environment, potion_format.c_str(), static_cast<std::int32_t>(count),
            output, error);
    }
};

bool RuntimeEquipmentTextProviderV1::bind(
        const dh2::data::ItemTable& table,
        const dh2::data::CharacterTable& source_characters,
        character_menu::MenuLocalization& localization,
        const CharacterState& same_character_state,
        RuntimeEquipmentBareDefinitionPolicyV1 policy,
        RuntimeEquipmentOptionsV1& options,
        RuntimeEquipmentPageBindingsV1& page,
        std::string& error) {
    if (state_) {
        error = "Runtime equipment default text provider can only be bound once";
        return false;
    }
    if (table.identifiers.size() != table.rows.size() ||
        policy.supported_unpowered_definition_ids.empty()) {
        error = "Runtime equipment text requires an actual ItemTable and explicit unpowered-definition policy";
        return false;
    }
    auto next = std::make_shared<State>();
    next->table = &table;
    next->profile = &same_character_state;
    next->localization = &localization;
    for (auto& definition : policy.supported_unpowered_definition_ids) {
        if (definition.empty() || !next->supported_unpowered.insert(definition).second) {
            error = "Runtime equipment unpowered-definition policy has an empty or duplicate ID";
            return false;
        }
        const auto id = dh2::data::item_id(table, definition);
        const auto* row = dh2::data::item(table, id);
        if (!row || dh2::data::item_type(*row) == 13) {
            error = "Runtime equipment unpowered-definition policy names a missing or gold source item";
            return false;
        }
    }
    if (!localization.bind_profile(&same_character_state, error)) return false;
    dh2::ui::HudTextV1* text{};
    if (!localization.borrow_text(text, next->environment, error) || !text) {
        if (error.empty()) error = "Runtime equipment could not borrow the same menu HudText owner";
        return false;
    }
    next->text = text;
    next->text_owner = std::make_unique<dh2::ui::ItemTextOwnerV5>(
        table, source_characters, *text, next->environment);
    bool source_null = false;
    if (!dh2::ui::character_menu_potion_localized_format_v4(
            *text, next->environment, next->potion_format, source_null, error)) return false;
    if (source_null || next->potion_format.empty()) {
        error = "Original GAMEPLAYMENUS_POTIONS localization is source-null or empty";
        return false;
    }

    if (!options.menu.item_name) {
        options.menu.item_name = [next](const InventoryItem& item,
                const dh2::data::Item& row, std::string& output, std::string& message) {
            return next->name(item, &row, output, message);
        };
    }
    if (!options.menu.empty_name) {
        options.menu.empty_name = [next](std::string& output, std::string& message) {
            return next->symbol("GLOBAL_EMPTY", output, message);
        };
    }
    if (!page.inventory_text.item_name) {
        page.inventory_text.item_name = [next](const inventory::Row& row,
                std::string& output, std::string& message) {
            InventoryItem item;
            item.instance_id = row.instance_id;
            item.definition_id = row.definition_id;
            item.quantity = row.quantity;
            return next->name(item, nullptr, output, message);
        };
    }
    if (!page.inventory_text.symbol) {
        page.inventory_text.symbol = [next](const std::string& key,
                std::string& output, std::string& message) {
            return next->symbol(key, output, message);
        };
    }
    if (!page.inventory_text.potions) {
        page.inventory_text.potions = [next](std::uint32_t count,
                std::string& output, std::string& message) {
            return next->potions(count, output, message);
        };
    }
    if (!page.details_text.item_name) {
        page.details_text.item_name = [next](const InventoryItem& item,
                const dh2::data::Item& row, std::string& output, std::string& message) {
            return next->name(item, &row, output, message);
        };
    }
    if (!page.details_text.symbol) {
        page.details_text.symbol = [next](const std::string& key,
                std::string& output, std::string& message) {
            return next->symbol(key, output, message);
        };
    }
    if (!page.details_text.item_details) {
        page.details_text.item_details = [next](const std::string& field,
                const InventoryItem& item, std::string& output, std::string& message) {
            return next->details(field, item, output, message);
        };
    }
    state_ = std::move(next);
    error.clear();
    return true;
}

} // namespace dh::foundation::equipment_menu
