#include "source_pause_ui_render_v1.hpp"

#include <algorithm>
#include <cmath>
#include <string>

namespace dh::foundation::pause_ui {

const frontend::art::ScreenArt& source_pause_ui_art_v1(SourcePauseSurfaceV1,
                                                        bool) noexcept;

namespace {
using frontend::art::HitRegion;

bool has_suffix(std::string_view value, std::string_view suffix) noexcept {
    return value.size() >= suffix.size() &&
           value.compare(value.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool source_hit_contains(const HitRegion& region, float x, float y) noexcept {
    if (!std::isfinite(x) || !std::isfinite(y)) return false;
    const auto& vertices = region.triangles;
    for (std::size_t i = 0; i + 2 < vertices.size(); i += 3) {
        const auto& a = vertices[i];
        const auto& b = vertices[i + 1];
        const auto& c = vertices[i + 2];
        const float area = (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
        if (std::abs(area) < 1e-6f) continue;
        const float e0 = (b.x-a.x)*(y-a.y) - (b.y-a.y)*(x-a.x);
        const float e1 = (c.x-b.x)*(y-b.y) - (c.y-b.y)*(x-b.x);
        const float e2 = (a.x-c.x)*(y-c.y) - (a.y-c.y)*(x-c.x);
        if ((e0 >= -1e-5f && e1 >= -1e-5f && e2 >= -1e-5f) ||
            (e0 <= 1e-5f && e1 <= 1e-5f && e2 <= 1e-5f)) {
            return true;
        }
    }
    return false;
}

SourcePauseActionV1 action_for_path(std::string_view path) noexcept {
    if (has_suffix(path, "btn_mainmenu")) return SourcePauseActionV1::open_pause_page;
    if (has_suffix(path, "btn_MENU_CONTINUE")) return SourcePauseActionV1::resume_game;
    if (has_suffix(path, "btn_MENU_HELP")) return SourcePauseActionV1::open_help;
    if (has_suffix(path, "btn_Multiplayer")) return SourcePauseActionV1::open_multiplayer;
    if (has_suffix(path, "btn_MENU_OPTIONS")) return SourcePauseActionV1::open_options;
    if (has_suffix(path, "btn_MENU_MAIN_MENU")) return SourcePauseActionV1::request_main_menu_confirmation;
    if (has_suffix(path, "btn_yes")) return SourcePauseActionV1::confirm_return_to_main_menu;
    if (has_suffix(path, "btn_no")) return SourcePauseActionV1::cancel_confirmation;
    return SourcePauseActionV1::unhandled_source_control;
}

void set_markup_text(std::string& markup, std::string_view value) {
    const auto font = markup.find("<font");
    const auto begin = font == std::string::npos ? std::string::npos : markup.find('>', font);
    const auto end = begin == std::string::npos ? std::string::npos : markup.find("</font>", begin + 1);
    if (begin != std::string::npos && end != std::string::npos) {
        markup.replace(begin + 1, end - begin - 1, value);
    } else {
        markup.assign(value);
    }
}

void bind_english_source_labels(frontend::art::ScreenArt& art) {
    for (auto& field : art.text_fields) {
        for (const auto& binding : source_pause_ui_text_bindings_v1()) {
            if (has_suffix(field.path, binding.path_suffix)) {
                set_markup_text(field.initial_text, binding.english_fallback);
                break;
            }
        }
    }
}
} // namespace

SourcePauseUiFrameV1 source_pause_ui_frame_v1(SourcePauseSurfaceV1 surface,
                                               bool multiplayer) {
    SourcePauseUiFrameV1 frame;
    frame.surface = surface;
    frame.multiplayer = multiplayer;
    frame.art = source_pause_ui_art_v1(surface, multiplayer);
    bind_english_source_labels(frame.art);
    return frame;
}

SourcePauseUiHitV1 source_pause_ui_hit_test_v1(const SourcePauseUiFrameV1& frame,
                                                float source_x,
                                                float source_y) noexcept {
    for (auto it = frame.art.hit_regions.rbegin(); it != frame.art.hit_regions.rend(); ++it) {
        if (!source_hit_contains(*it, source_x, source_y)) continue;
        return {true, action_for_path(it->button_path), it->button_path};
    }
    return {};
}

SourcePauseRouteV1 source_pause_ui_route_v1(const SourcePauseUiFrameV1& frame,
                                             const SourcePauseUiHitV1& hit) noexcept {
    if (!hit.hit) return {};
    switch (hit.action) {
        case SourcePauseActionV1::open_pause_page:
            if (frame.surface == SourcePauseSurfaceV1::hud_pause_button)
                return {SourcePauseRouteKindV1::open_pause_page, true, {}};
            break;
        case SourcePauseActionV1::resume_game:
            if (frame.surface == SourcePauseSurfaceV1::pause_page)
                return {SourcePauseRouteKindV1::resume_game, true, {}};
            break;
        case SourcePauseActionV1::request_main_menu_confirmation:
            if (frame.surface == SourcePauseSurfaceV1::pause_page)
                return {SourcePauseRouteKindV1::open_main_menu_confirmation, true, {}};
            break;
        case SourcePauseActionV1::confirm_return_to_main_menu:
            if (frame.surface == SourcePauseSurfaceV1::confirmation)
                return {SourcePauseRouteKindV1::return_to_main_menu, true, {}};
            break;
        case SourcePauseActionV1::cancel_confirmation:
            if (frame.surface == SourcePauseSurfaceV1::confirmation)
                return {SourcePauseRouteKindV1::cancel_confirmation, true, {}};
            break;
        case SourcePauseActionV1::open_help:
        case SourcePauseActionV1::open_multiplayer:
        case SourcePauseActionV1::open_options:
            return {SourcePauseRouteKindV1::unsupported_action, false,
                    "Authored Help, Multiplayer, or Options route requires its existing menu owner."};
        case SourcePauseActionV1::unhandled_source_control:
            return {SourcePauseRouteKindV1::unsupported_action, false,
                    "Authored source control has no pause integration route."};
    }
    return {SourcePauseRouteKindV1::unsupported_action, false,
            "Authored control is not actionable on this pause surface."};
}

const std::array<SourcePauseTextBindingV1, 8>& source_pause_ui_text_bindings_v1() noexcept {
    static constexpr std::array<SourcePauseTextBindingV1, 8> bindings{{
        {"btn_MENU_CONTINUE/text", "MENU_CONTINUE", "Continue"},
        {"btn_MENU_HELP/text", "MENU_HELP", "Help"},
        {"btn_Multiplayer/text", "MENU_MULTIPLAYER", "Multiplayer"},
        {"btn_MENU_OPTIONS/text", "MENU_OPTIONS", "Options"},
        {"btn_MENU_MAIN_MENU/text", "MENU_MAIN_MENU", "Main Menu"},
        {"WarningBox/confirm_msg/text", "GLOBAL_ASK_FOR_MAINMENU", "Return to main menu?"},
        {"WarningBox/btn_yes/text", "GAMEPLAYMENUS_ACCEPT", "Yes"},
        {"WarningBox/btn_no/text", "GAMEPLAYMENUS_REFUSE", "No"},
    }};
    return bindings;
}

const char* source_pause_ui_texture_file_v1(std::uint32_t source_bitmap_id) noexcept {
    return source_bitmap_id == 1 ? "data/3d/textures/MenusGraphics_droid.tga" : "";
}

const char* source_pause_ui_font_file_v1(std::uint32_t source_font_id) noexcept {
    return source_font_id == 7 || source_font_id == 512 ? "data/Fontin SmallCaps.ttf" : "";
}

} // namespace dh::foundation::pause_ui
