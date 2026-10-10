#pragma once

#include <array>
#include <cstdint>
#include <string_view>

namespace dh::foundation::pause_ui {

// Source action meanings only. A caller must still perform the corresponding
// existing menu/native action; this projection neither opens nor closes menus.
enum class SourcePauseActionV1 : std::uint8_t {
    open_pause_page,
    resume_game,
    open_help,
    open_multiplayer,
    open_options,
    request_main_menu_confirmation,
    confirm_return_to_main_menu,
    cancel_confirmation,
    unhandled_source_control,
};

struct SourcePauseButtonV1 {
    std::string_view path;
    std::string_view label_symbol;
    SourcePauseActionV1 action{};
    std::uint16_t movie_character_id{};
    std::uint8_t display_depth{};
    std::int16_t source_x_twips{};
    std::int16_t source_y_twips{};
    bool multiplayer_state_dependent{};
};

struct SourcePauseUiV1 {
    std::string_view movie_asset;
    std::uint16_t gameplay_hud_character_id{};
    std::string_view gameplay_pause_button_path;
    std::uint16_t gameplay_pause_button_character_id{};
    std::uint16_t gameplay_pause_button_art_character_id{};
    std::uint16_t gameplay_pause_icon_character_id{};
    std::uint8_t gameplay_pause_button_depth{};
    std::int16_t gameplay_pause_button_x_twips{};
    std::int16_t gameplay_pause_button_y_twips{};
    std::string_view pause_page_path;
    std::uint16_t pause_page_character_id{};
    std::uint16_t pause_buttons_character_id{};
    std::string_view confirmation_page_path;
    std::uint16_t confirmation_page_character_id{};
    std::array<SourcePauseButtonV1, 7> buttons;
};

// Immutable references into the original dqhud_droid SWF and its authored
// ActionScript labels/routes. The MovieClip ids are references, not copies.
const SourcePauseUiV1& source_pause_ui_v1() noexcept;

} // namespace dh::foundation::pause_ui
