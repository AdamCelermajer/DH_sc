#pragma once

#include <cstdint>
#include <string_view>

namespace dh2::android_ui {

enum class FrontLoadingRenderPhaseV1 { front_menu, source_loading, gameplay };

constexpr FrontLoadingRenderPhaseV1 front_loading_render_phase_v1(
    bool source_campaign_active, bool source_scene_active) noexcept {
    if (!source_campaign_active) return FrontLoadingRenderPhaseV1::front_menu;
    return source_scene_active ? FrontLoadingRenderPhaseV1::gameplay
                               : FrontLoadingRenderPhaseV1::source_loading;
}

constexpr bool front_movie_slot_visible_v1(FrontLoadingRenderPhaseV1 phase,
                                            std::uint32_t slot) noexcept {
    if (phase == FrontLoadingRenderPhaseV1::source_loading) return slot == 0;
    return slot == 0 || slot == 2;
}

constexpr bool front_loading_clip_visible_v1(FrontLoadingRenderPhaseV1 phase,
                                               std::uint32_t slot,
                                               std::string_view path) noexcept {
    if (phase != FrontLoadingRenderPhaseV1::source_loading)
        return front_movie_slot_visible_v1(phase, slot);
    return slot == 0 && path.find("menu_Loading") != std::string_view::npos;
}

} // namespace dh2::android_ui
