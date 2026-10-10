#pragma once

namespace dh2::android_ui {

// A name-entry dialog owns its complete authored SWF backdrop. The retained
// MainMenu may remain underneath it in MenuManager's active render array, but
// that must not select the swamp scene behind the name form. SelectClass is
// different: its authored 3D class scene replaces MainMenu's scene.
enum class FrontSceneSelectionV1 { authored_swf, menu_swamp, class_selection };

constexpr FrontSceneSelectionV1 front_scene_selection_v1(
    bool main_movie_visible, bool class_preview, bool name_entry) noexcept {
    if (class_preview) return FrontSceneSelectionV1::class_selection;
    if (name_entry || !main_movie_visible) return FrontSceneSelectionV1::authored_swf;
    return FrontSceneSelectionV1::menu_swamp;
}

} // namespace dh2::android_ui
