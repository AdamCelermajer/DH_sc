#pragma once

#include "runtime_equipment_page_v1.hpp"
#include "../character_menu/menu_text.hpp"
#include "../../../game-data/items.hpp"

namespace dh::foundation::equipment_menu {

// Source-caller proof for the narrow generic inventory scope: these actual
// source definitions are known authored unpowered base items. The provider
// rejects every other definition rather than pretending generic rows expose
// native generated powers or ItemInstance valuation.
struct RuntimeEquipmentBareDefinitionPolicyV1 {
    std::vector<std::string> supported_unpowered_definition_ids;
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
              RuntimeEquipmentOptionsV1&,
              RuntimeEquipmentPageBindingsV1&,
              std::string& error);
};

} // namespace dh::foundation::equipment_menu
