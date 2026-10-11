#include "../original_menu_viewport_v1.hpp"
#include "../viewport.hpp"

#include <array>
#include <cmath>
#include <stdexcept>

namespace {
unsigned checks = 0;
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
    ++checks;
}

struct Context {};
int invoke(void*, dh2::ui::ViewportState64*, const dh2::ui::ViewportRequest40* request,
           dh2::ui::ViewportResponse16* response) {
    if (request->operation != dh2::ui::ViewportOperation::orientation) return 0;
    response->values[0] = 0;
    return 1;
}
}

int main() {
    using dh2::ui::original_menu_viewport_v1::background_viewport;
    using dh2::ui::original_menu_viewport_v1::authored_stage_viewport;
    using dh2::ui::original_menu_viewport_v1::full_surface;
    using dh2::ui::original_menu_viewport_v1::surface_aspect;
    using dh2::ui::original_menu_viewport_v1::viewport_for_screen_state_v1;
    using dh2::ui::original_menu_viewport_v1::source_front_menu_assets_v1;

    const auto droid_assets = source_front_menu_assets_v1(854);
    check(std::string(droid_assets.main_movie) == "data/menus/dqmenus_droid.swf" &&
          std::string(droid_assets.shared_movie) == "data/menus/dqshared_droid.swf" &&
          droid_assets.width_twips == 9600 && droid_assets.height_twips == 6400 &&
          droid_assets.width_pixels == 480 && droid_assets.height_pixels == 320,
          "source 854px profile selects the Droid SWFs and their 480x320 stage");
    const auto standard_assets = source_front_menu_assets_v1(1280);
    check(std::string(standard_assets.main_movie) == "data/menus/dqmenus.swf" &&
          std::string(standard_assets.shared_movie) == "data/menus/dqshared.swf" &&
          standard_assets.width_twips == 20480 && standard_assets.height_twips == 15360 &&
          standard_assets.width_pixels == 1024 && standard_assets.height_pixels == 768,
          "non-854px profile selects standard SWFs and their 1024x768 stage");

    check(full_surface(2400, 1080) == std::array<std::int32_t, 4>{0, 0, 2400, 1080},
          "20:9 backdrop must use the entire live display bounds");
    check(background_viewport(2400, 1080) == full_surface(2400, 1080),
          "3D menu backdrop and SWF must share full display bounds");
    check(full_surface(1280, 549) == std::array<std::int32_t, 4>{0, 0, 1280, 549},
          "wide surface reference must retain its full-screen bounds");
    check(viewport_for_screen_state_v1("menu_CharacterMenu", 3200, 1440) ==
              full_surface(3200, 1440),
          "high-resolution Android menu must share the canonical physical surface frame");
    check(full_surface(0, 720) == std::array<std::int32_t, 4>{},
          "invalid surface must produce an empty rectangle");
    check(authored_stage_viewport(480, 320) == std::array<std::int32_t, 4>{0, 0, 480, 320},
          "authored 3:2 menu stage must use its full reference surface");
    check(authored_stage_viewport(1920, 1080) == std::array<std::int32_t, 4>{150, 0, 1620, 1080},
          "16:9 landscape must contain the authored stage with centered side gutters");
    check(authored_stage_viewport(2400, 1080) == std::array<std::int32_t, 4>{390, 0, 1620, 1080},
          "20:9 landscape must preserve menu proportions and center controls");
    check(authored_stage_viewport(1080, 2400) == std::array<std::int32_t, 4>{0, 840, 1080, 720},
          "portrait bounds must preserve the authored menu stage aspect");
    const auto wide_surface = full_surface(2400, 1080);
    const auto reference_frame = std::array<std::int32_t, 4>{0, 0, 2400, 1080};
    check(viewport_for_screen_state_v1("main", 2400, 1080) == reference_frame &&
          viewport_for_screen_state_v1("main", 2424, 1080) ==
              std::array<std::int32_t, 4>{0, 0, 2424, 1080},
          "front menu must preserve MenuFlash2DCamera's full-driver viewport");
    check(viewport_for_screen_state_v1("menu_CharacterMenu", 2400, 1080) == reference_frame &&
          viewport_for_screen_state_v1("menu_InventorySheetMain", 2400, 1080) == reference_frame &&
          viewport_for_screen_state_v1("menu_SkillTreeSheetNew", 2400, 1080) == reference_frame &&
          viewport_for_screen_state_v1("menu_FaerySheet", 2400, 1080) == reference_frame,
          "character, inventory, talents and faery screens must share the full-driver viewport");
    check(viewport_for_screen_state_v1("menu_Ingame", 2400, 1080) == reference_frame,
          "gameplay pause overlay must use the same full-driver viewport");
    check(viewport_for_screen_state_v1("hud", 2400, 1080) == wide_surface,
          "gameplay HUD must retain full-screen bounds beneath menu overlays");
    check(viewport_for_screen_state_v1("main", 1920, 1080) == full_surface(1920, 1080) &&
          viewport_for_screen_state_v1("main", 1080, 2400) == full_surface(1080, 2400),
          "front-menu viewport must refresh from changed landscape and portrait surfaces");
    check(authored_stage_viewport(0, 1080) == std::array<std::int32_t, 4>{},
          "invalid menu surface must produce an empty rectangle");
    check(std::fabs(surface_aspect(2400, 1080) - (2400.0f / 1080.0f)) < 1e-6f,
          "menu camera aspect must use the live display bounds");
    check(std::fabs(surface_aspect(0, 720) - 1.5f) < 1e-6f,
          "invalid camera bounds must fall back to authored 480x320 stage ratio");

    const auto viewport = viewport_for_screen_state_v1("main", 2400, 1080);
    dh2::ui::ViewportState64 state{
        {0.0f, 9600.0f, 0.0f, 6400.0f},
        {viewport[0], viewport[1], viewport[2], viewport[3]},
        {viewport[0], viewport[1], viewport[2], viewport[3]},
        1.0f, 0, 0};
    Context context;
    const dh2::ui::ViewportServices16 services{&context, invoke};
    float center[2]{viewport[0] + viewport[2] * 0.5f, viewport[1] + viewport[3] * 0.5f};
    check(dh2_ui_screen_to_logical(&state, center, &services) == 0 &&
          std::fabs(center[0] - 240.0f) < 1e-5f &&
          std::fabs(center[1] - 160.0f) < 1e-5f,
          "full-surface display center must map to source stage center");
    float left_edge[2]{static_cast<float>(viewport[0]), viewport[1] + viewport[3] * 0.5f};
    check(dh2_ui_screen_to_logical(&state, left_edge, &services) == 0 &&
          std::fabs(left_edge[0]) < 1e-5f,
          "left full-surface edge must reach the source stage edge");
    float right_edge[2]{static_cast<float>(viewport[0] + viewport[2]), viewport[1] + viewport[3] * 0.5f};
    check(dh2_ui_screen_to_logical(&state, right_edge, &services) == 0 &&
          std::fabs(right_edge[0] - 480.0f) < 1e-5f,
          "right full-surface edge must reach the source stage edge");
    float top_edge[2]{viewport[0] + viewport[2] * 0.5f, 0.0f};
    check(dh2_ui_screen_to_logical(&state, top_edge, &services) == 0 &&
          std::fabs(top_edge[1]) < 1e-5f,
          "top full-surface edge must reach the source stage edge");

    // Source mode 0 fills the driver viewport and maps each authored axis
    // independently: at 2400x1080, x uses 5 px/unit while y uses 3.375.
    float wide_authored_point[2]{500.0f, 337.5f};
    check(dh2_ui_screen_to_logical(&state, wide_authored_point, &services) == 0 &&
          std::fabs(wide_authored_point[0] - 100.0f) < 1e-5f &&
          std::fabs(wide_authored_point[1] - 100.0f) < 1e-5f,
          "20:9 source mode-0 projection must preserve its anisotropic full-screen mapping");

    // Mode 0 intentionally stretches the authored 480x320 stage to the full
    // surface. Confirm the production pixel→SWF transform remains normalized
    // across common 3:2, 16:9, ultrawide landscape, and portrait dimensions.
    constexpr std::array<std::array<std::int32_t, 2>, 4> surfaces{{
        {{960, 640}}, {{1920, 1080}}, {{2424, 1080}}, {{1080, 2400}}}};
    for (const auto& dimensions : surfaces) {
        const auto bounds = full_surface(dimensions[0], dimensions[1]);
        dh2::ui::ViewportState64 sized_state{
            {0.0f, 9600.0f, 0.0f, 6400.0f},
            {bounds[0], bounds[1], bounds[2], bounds[3]},
            {bounds[0], bounds[1], bounds[2], bounds[3]},
            1.0f, 0, 0};
        float point[2]{dimensions[0] * 0.25f, dimensions[1] * 0.75f};
        const bool mapped = dh2_ui_screen_to_logical(&sized_state, point, &services) == 0 &&
            std::fabs(point[0] - 120.0f) < 1e-4f &&
            std::fabs(point[1] - 240.0f) < 1e-4f;
        check(mapped, "full-surface hit mapping must preserve normalized coordinates at each aspect");
    }
    return checks == 28 ? 0 : 1;
}
