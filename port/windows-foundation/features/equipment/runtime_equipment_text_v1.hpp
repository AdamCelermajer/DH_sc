#pragma once

#include "runtime_equipment_page_v1.hpp"
#include "../character_menu/menu_text.hpp"
#include "../inventory/source_item_descriptors.hpp"
#include "../../../game-data/items.hpp"
#include <functional>

namespace dh::foundation::equipment_menu {

// Source-caller proof for the narrow generic inventory scope: these actual
// source definitions are known authored unpowered base items. The provider
// rejects every other definition rather than pretending generic rows expose
// native generated powers or ItemInstance valuation.
struct RuntimeEquipmentBareDefinitionPolicyV1 {
    std::vector<std::string> supported_unpowered_definition_ids;
    // Reuse the caller's same authentic ItemText wrapper/cache epoch. Its
    // ItemTable metadata identity is checked before installing callbacks.
    std::shared_ptr<dh2::ui::ItemTextOwnerV5> existing_text_owner;
};

// Resolves each live menu row against its current canonical item/effects/text
// owner. `verified_bare` is an explicit classification: it may be true only
// when the source inventory metadata says this instance has no generated
// instance payload. If false, `lease` must own the matching ItemInstanceV1,
// ItemPresentationOwnerV5, and ItemTextOwnerV5 for the same current revision.
// A failed or stale resolution is an error and must never downgrade to the
// bare-definition formatter.
struct RuntimeEquipmentInstanceResolutionV1 {
    bool verified_bare{};
    inventory::SourceInstanceLease lease;
};
using RuntimeEquipmentInstanceResolverV1 = std::function<bool(
    const InventoryItem&, RuntimeEquipmentInstanceResolutionV1&, std::string&)>;

// Source NativeInvGetItemDetails' fourth value/string pair for a currently
// selected canonical or approved bare item. The caller supplies property197 from the same
// live player CombatSession. The caller also passes the real CharacterDesign
// multiplier from its source constants owner because MenuLocalization does not
// own design_pycst. This provider supplies the actual saved ItemInstance value
// or approved bare ItemTable value, source fixed-point transmute result, and original
// localized formatting. Generated/powered ItemInstances require their
// canonical source instance provider through the required resolver.
struct RuntimeEquipmentTransmuteValuePacketV1 {
    std::string instance_id;
    std::string definition_id;
    std::int32_t source_item_value{};
    std::int32_t raw_property_197{};
    std::int32_t source_multiplier{};
    std::int32_t transmute_value{};
    std::string formatted_value;
};

// Builds the default original-text callbacks over the caller's exact ItemTable,
// CharacterState, MenuLocalization and its borrowed HudText environment. The
// localization owner and source tables must outlive callbacks copied into the
// equipment binding/page. Existing custom callbacks are preserved.
class RuntimeEquipmentTextProviderV1 {
    struct State;
    std::shared_ptr<State> state_;

public:
    bool bind(const dh2::data::ItemTable&,
              const dh2::data::CharacterTable& source_characters,
              character_menu::MenuLocalization&,
              const CharacterState& same_character_state,
              RuntimeEquipmentBareDefinitionPolicyV1,
              RuntimeEquipmentInstanceResolverV1,
              RuntimeEquipmentOptionsV1&,
              RuntimeEquipmentPageBindingsV1&,
              std::string& error);

    // These values must come from the same live player/context as the bound
    // CharacterState: property197 from CombatSession resolved sheets and
    // TransmuteMultiplier from the caller's real design_pycst owner.
    bool transmute_value(const InventoryItem&, std::int32_t raw_property_197,
                         std::int32_t source_multiplier,
                         RuntimeEquipmentTransmuteValuePacketV1&,
                         std::string& error) const;
};

} // namespace dh::foundation::equipment_menu

