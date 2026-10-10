#pragma once
#include "menu_avatar_preview_v1.hpp"
#include <memory>

namespace dh2::android_ui {
// Metadata loading and old persona previews may install their inspection
// provider only before the actual process has published its retained adapter.
// Replacing that adapter would change the slot in one owner while drawing the
// unchanged Character from another.
inline bool install_front_inspection_avatar_services_v1(
    ui::MenuAvatarPreviewServicesV1& current,
    const std::shared_ptr<void>& process_owner,
    const ui::MenuAvatarPreviewServicesV1& inspection) {
    if(process_owner)return false;
    current=inspection;
    return true;
}
}
