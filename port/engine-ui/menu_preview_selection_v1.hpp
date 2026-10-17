#pragma once

#include <cstdint>
#include <string>

namespace dh2::ui {

// Source MenuMainMenu::ChangeCharacterToDisplay(slot, force) operates on the
// owning MainMenu's mSavegameSlot field. The caller passes that stable field
// directly; this helper never creates a parallel selection or derives a slot
// from PlayerInfo, a visible row, or a save-manager cursor.
struct MenuPreviewSelectionServicesV1 {
    void* context{};
    bool (*destroy_character)(void*, std::string&){};
    bool (*setup_character)(void*, std::int32_t published_slot,
                            std::string&){};
    bool (*create_avatar_camera)(void*, std::string&){};
};

enum class MenuPreviewSelectionStatusV1 : std::uint8_t {
    complete,
    invalid_argument,
    destroy_failed,
    setup_failed,
    camera_failed,
};

struct MenuPreviewSelectionResultV1 {
    MenuPreviewSelectionStatusV1 status =
        MenuPreviewSelectionStatusV1::invalid_argument;
    std::int32_t old_slot = -1;
    bool changed{};
    bool destroy_attempted{};
    bool slot_stored{};
    bool setup_attempted{};
    bool camera_attempted{};
};

// Source order: equal slot + !force returns; if old slot != -1, destroy the
// old avatar; publish the new slot; SetupCharacter reads that published slot;
// then create the avatar camera (which itself can early-return when its pane
// camera already exists). Failures preserve the source prefix already reached.
MenuPreviewSelectionStatusV1 change_menu_preview_slot_v1(
    std::int32_t* main_menu_savegame_slot, std::int32_t requested_slot,
    bool force, const MenuPreviewSelectionServicesV1&,
    MenuPreviewSelectionResultV1*, std::string& error) noexcept;

} // namespace dh2::ui
