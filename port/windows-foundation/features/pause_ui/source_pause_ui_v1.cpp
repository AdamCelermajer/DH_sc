#include "source_pause_ui_v1.hpp"

namespace dh::foundation::pause_ui {

const SourcePauseUiV1& source_pause_ui_v1() noexcept {
    static constexpr SourcePauseUiV1 value{
        "data/menus/dqhud_droid.swf",
        470,
        "_root.menu_HUD_0.HUDelements.btn_mainmenu",
        85,
        77,
        41,
        11,
        670,
        1878,
        "_root.menu_Ingame",
        756,
        755,
        "_root.menu_hud_confirm",
        699,
        {{
            {"_root.menu_Ingame.buttons.btn_MENU_CONTINUE", "MENU_CONTINUE",
             SourcePauseActionV1::resume_game, 531, 13, 90, -67, false},
            {"_root.menu_Ingame.buttons.btn_MENU_HELP", "MENU_HELP",
             SourcePauseActionV1::open_help, 531, 7, 90, 819, false},
            {"_root.menu_Ingame.buttons.btn_Multiplayer", "MENU_MULTIPLAYER",
             SourcePauseActionV1::open_multiplayer, 754, 31, -7289, 1114, true},
            {"_root.menu_Ingame.buttons.btn_MENU_OPTIONS", "MENU_OPTIONS",
             SourcePauseActionV1::open_options, 531, 1, 90, 1767, false},
            {"_root.menu_Ingame.buttons.btn_MENU_MAIN_MENU", "MENU_MAIN_MENU",
             SourcePauseActionV1::request_main_menu_confirmation, 531, 19, 90, 2653, false},
            {"_root.menu_hud_confirm.WarningBox.btn_yes", "GAMEPLAYMENUS_ACCEPT",
             SourcePauseActionV1::confirm_return_to_main_menu, 697, 10, 270, 1185, false},
            {"_root.menu_hud_confirm.WarningBox.btn_no", "GAMEPLAYMENUS_REFUSE",
             SourcePauseActionV1::cancel_confirmation, 697, 16, 2782, 1185, false},
        }}
    };
    return value;
}

} // namespace dh::foundation::pause_ui
