#pragma once
#include <android/asset_manager.h>
#include <vector>
#include <string>
namespace model_renderer {struct PlayerGameplayBinding;}
namespace dh2::android_ui {
// Width, height, then Android ARGB pixels. Empty output means actual absence.
std::vector<int> menu_icon_pixels(AAssetManager*,const std::string&,std::string&);
// Resolves actual authored EquipmentSlots constants, then the identical SWF
// frame label. This is a slot icon, never a fabricated blank-item fallback.
bool equipment_slot_icon_name(const model_renderer::PlayerGameplayBinding&,
 int slot,std::string& name,std::string& error);
}
