#include "runtime_equipment_text_v1.hpp"

#include "../inventory/source_item_descriptors.hpp"
#include "../../../engine-ui/character_menu_potions_v4.hpp"
#include "../../../engine-ui/item_text_owner_v5.hpp"

#include <algorithm>
#include <cstring>
#include <limits>
#include <set>

namespace dh::foundation::equipment_menu {
namespace {
std::int32_t signed_word(std::uint32_t bits) {
    std::int32_t value{};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}
std::int32_t wrapped_multiply(std::int32_t a, std::int32_t b) {
    return signed_word(std::uint32_t(a) * std::uint32_t(b));
}
std::int32_t source_transmute_value(std::int32_t value, std::int32_t raw_bonus,
                                    std::int32_t multiplier) {
    const auto bonus = signed_word(std::uint32_t(raw_bonus) + 256u);
    auto amount = wrapped_multiply(multiplier,
        wrapped_multiply(signed_word(std::uint32_t(value) << 8), bonus) >> 8) >> 16;
    return amount < 1 ? 1 : amount;
}
}

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

    bool transmute_value(const InventoryItem& selected, std::int32_t raw_property_197,
                         std::int32_t multiplier,
                         RuntimeEquipmentTransmuteValuePacketV1& output,
                         std::string& error) const {
        output = {};
        if (!profile) {
            error = "Runtime equipment selected CharacterState lease is unavailable";
            return false;
        }
        const auto current = std::find_if(profile->inventory.begin(), profile->inventory.end(),
            [&](const InventoryItem& item) { return item.instance_id == selected.instance_id; });
        if (selected.instance_id.empty() || current == profile->inventory.end() ||
            current->definition_id != selected.definition_id ||
            current->quantity != selected.quantity) {
            error = "Transmute amount request is stale or differs from the currently owned item";
            return false;
        }
        const dh2::data::Item* row{};
        if (!supported(selected, row, error)) return false;
        if (!text_owner || !text) {
            error = "Runtime equipment source HudText adapter is unavailable for transmute formatting";
            return false;
        }

        const auto source_id = dh2::data::item_id(*table, selected.definition_id);
        const auto product = std::uint32_t(row->record.words[27]) *
                             std::uint32_t(row->record.words[28]);
        std::int32_t source_value{};
        std::memcpy(&source_value, &product, sizeof(source_value));
        dh2::data::ItemInstanceV1 source_item;
        source_item.id = source_id;
        source_item.value = source_value;
        const auto amount = source_transmute_value(source_value, raw_property_197, multiplier);

        struct FormatContext {
            const State* owner{};
            dh2::data::ItemInstanceV1 item;
            std::string localized;
        } context;
        context.owner = this;
        context.item = source_item;
        dh2::ui::HudTextServicesV1 formatter{&context,
            [](void* opaque, const dh2::ui::HudTextRequestV1& request,
               dh2::ui::HudTextResponseV1& response, std::string& message) {
                auto& c = *static_cast<FormatContext*>(opaque);
                using Operation = dh2::data::ItemTextOperationV5;
                dh2::data::ItemTextRequestV5 source{};
                dh2::data::ItemTextResponseV5 result;
                if (request.operation == dh2::ui::hud_text_constant_v1) {
                    source.operation = Operation::constant;
                    source.group = request.group;
                    source.key = request.key;
                } else if (request.operation == dh2::ui::hud_text_integer_string_v1) {
                    source.operation = Operation::integer_string;
                    source.value = request.value;
                } else if (request.operation == dh2::ui::hud_text_pack_v1) {
                    response.value = c.owner->text->pack();
                    return true;
                } else {
                    message = "Transmute formatter requested an unsupported original HudText operation";
                    return false;
                }
                auto services = c.owner->text_owner->services();
                std::string ignored;
                if (!services.invoke(services.context, c.item, source, result, ignored, message))
                    return false;
                if (request.operation == dh2::ui::hud_text_constant_v1) {
                    response.value = result.value;
                } else {
                    c.localized = std::move(result.text);
                    response.text = c.localized.c_str();
                }
                return true;
            }};
        dh2::data::ItemTextArgumentV5 argument{};
        argument.number = static_cast<float>(amount);
        argument.integer = amount;
        std::string formatted;
        bool changed{};
        if (!dh2::ui::item_text_varargs_v5("^d", &argument, 1, formatter,
                                           formatted, changed, error)) return false;

        RuntimeEquipmentTransmuteValuePacketV1 next;
        next.instance_id = selected.instance_id;
        next.definition_id = selected.definition_id;
        next.source_item_value = source_value;
        next.raw_property_197 = raw_property_197;
        next.source_multiplier = multiplier;
        next.transmute_value = amount;
        next.formatted_value = std::move(formatted);
        output = std::move(next);
        error.clear();
        return true;
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

bool RuntimeEquipmentTextProviderV1::transmute_value(
        const InventoryItem& selected, std::int32_t raw_property_197,
        std::int32_t source_multiplier,
        RuntimeEquipmentTransmuteValuePacketV1& output, std::string& error) const {
    output = {};
    if (!state_) {
        error = "Runtime equipment text provider is not bound";
        return false;
    }
    return state_->transmute_value(selected, raw_property_197, source_multiplier,
                                   output, error);
}

} // namespace dh::foundation::equipment_menu
