#pragma once
#include <cstdint>
#include <string>
namespace gameswf {struct fn_call;}
namespace dh2::ui {
struct MenuAvatarPreviewStateV1 {std::int32_t slot=-1;};
struct MenuAvatarPreviewServicesV1 {
    void* context{};
    bool (*destroy_character)(void*,std::string&){};
    // Canonical CreatePlayer + existing-profile Character::SG_Load(4), then
    // genuine node transform/visibility. No menu metadata-only substitute.
    bool (*setup_character)(void*,std::int32_t,std::string&){};
    bool (*create_avatar_camera)(void*,std::string&){};
};
// Original42c0d0: same slot is a no-op unless forced. Changed/forced clears
// previous avatar (except slot=-1), publishes slot, setup, then camera. On a
// provider failure reached side effects and published slot remain retained.
bool change_menu_avatar_preview_v1(MenuAvatarPreviewStateV1&,std::int32_t,bool,
    const MenuAvatarPreviewServicesV1&,std::string&);
// NativeSetSaveSlotIDToMainMenu43d220, distinct from gameplay assignment.
bool swf_menu_avatar_preview_v1(const gameswf::fn_call&,MenuAvatarPreviewStateV1&,
    const MenuAvatarPreviewServicesV1&,std::string&);
}
