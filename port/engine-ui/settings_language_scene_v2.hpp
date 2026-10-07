#pragma once
#include "settings_language_scene_v1.hpp"
// Same original traversal, with required localization byte only at its actual
// type14 reached branch. V1 remains unchanged for frozen callers.
extern "C" int dh2_settings_v2_refresh_language_scene(dh2::ui::SettingsLanguageScene24V1*,const dh2::ui::SettingsSceneServices16V1*) noexcept;
