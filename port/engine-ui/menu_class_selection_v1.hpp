#pragma once

#include <array>
#include <cstdint>

namespace dh2::ui {

struct MenuClassChoiceV1 {
    const char* profile_character;
    const char* title_symbol;
    const char* description_symbol;
};

// menu_SelectClass's CurrentClass callback value becomes the source class
// argument to NativeCreateSaveSlot. Keep it beside the selector's localized
// labels so an index cannot display one class and persist another.
inline constexpr std::array<MenuClassChoiceV1, 3> menu_class_choices_v1{{
    {"KnightPlayerBase", "MENU_CLASS_00", "MENU_KNIGHT_DESC"},
    {"RoguePlayerBase", "MENU_CLASS_01", "MENU_ROGUE_DESC"},
    {"MagePlayerBase", "MENU_CLASS_02", "MENU_MAGE_DESC"},
}};

inline constexpr const MenuClassChoiceV1* menu_class_choice_v1(int index) noexcept {
    return index >= 0 && index < static_cast<int>(menu_class_choices_v1.size())
        ? &menu_class_choices_v1[static_cast<unsigned>(index)] : nullptr;
}

// MenuCharacterSelect::OnEvent (0x4281b8) reacts to RenderFX event kind 2
// only, matches the actual cached btn_right/btn_left Character identities,
// and clamps the source selection to the three authored classes. Keep the
// Android adapter on the same identity-based rule; names and coordinates are
// not substitutes for the event's Character pointer.
inline constexpr bool menu_class_selection_click_v1(
    std::uint32_t event_kind, std::uintptr_t event_character,
    std::uintptr_t left_button, std::uintptr_t right_button,
    bool input_enabled, int current_index, int& next_index) noexcept {
    next_index = current_index;
    if (event_kind != 2 || !input_enabled || !event_character ||
        !left_button || !right_button || current_index < 0 || current_index > 2)
        return false;
    if (event_character == right_button && current_index < 2) {
        next_index = current_index + 1;
        return true;
    }
    if (event_character == left_button && current_index > 0) {
        next_index = current_index - 1;
        return true;
    }
    return false;
}

} // namespace dh2::ui
