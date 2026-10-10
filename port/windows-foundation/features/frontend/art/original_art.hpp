#pragma once
#include "../../../hud_geometry.hpp"
#include <filesystem>
#include <string_view>
namespace dh::foundation::frontend::flow { struct PresentationState; }

namespace dh::foundation::frontend::art {
enum class Screen { main_menu, select_class, enter_name, start_game };
struct TextField {
    std::string path;
    std::uint32_t character_id{}, font_id{};
    float source_height{};
    std::array<float,4> bounds{};
    std::array<std::uint8_t,4> rgba{};
    unsigned align{};
    std::array<float,6> matrix{};
    std::array<float,4> local_bounds{};
    std::array<float,3> margins{};
    float leading{};
    std::string initial_text;
    std::array<float,3> font_metrics{}; // ascent/descent/leading, normalized1024em
    bool word_wrap{},multiline{};
};
struct HitRegion {
    std::string button_path;
    std::vector<HudGeometryVertex> triangles;
};
struct RenderRegion {
    std::string path;
    std::array<float,4> bounds{}; // xmin,xmax,ymin,ymax in normalized stage pixels.
};
struct ScreenArt {
    std::vector<HudGeometryBatch> batches;
    std::vector<TextField> text_fields;
    std::vector<std::uint32_t> bitmap_ids; // Parallel to batches: 8=menus, 1=splash.
    std::vector<HitRegion> hit_regions;
    std::vector<std::array<float,4>> batch_colors; // Parallel; bitmap0 uses white texture.
    std::vector<RenderRegion> render_regions;
};
const ScreenArt& original_art(Screen) noexcept;
// Resolves only exact source symbol components/btn_SYMBOL names in field.path,
// then falls back to authored InitialText. Dynamic profile fields stay owned
// by the caller. Translation bytes come from original menu/gameplaymenus.
std::string source_english_label(const TextField&);
const char* source_font_file(std::uint32_t font_id) noexcept;
const char* source_texture_file(std::uint32_t bitmap_id) noexcept;
// Applies source labels, visibility and actual text bindings to retained source
// timelines. Shapes and hits use the identical current placement graph.
bool compose(Screen,const flow::PresentationState&,ScreenArt&,std::string& error);
std::string normalize_source_path(std::string_view);
inline std::string normalized_source_path(std::string_view path) { return normalize_source_path(path); }
std::string source_english_symbol(std::string_view);
bool project(const ScreenArt&,int width,int height,HudGeometry&,std::string& error);
bool source_point(float screen_x,float screen_y,int width,int height,std::array<float,2>&) noexcept;
bool class_scene_bounds(const ScreenArt&,std::array<float,4>&) noexcept;
// Bitmap8 selects the main original atlas; bitmap1 selects splash/keyboard art.
inline constexpr const char* atlas_relative_path = "data/3d/textures/MenusGraphics_droid.tga";
inline constexpr const char* splash_relative_path = "data/3d/textures/splash_final_droid.tga";
inline constexpr const char* generic_atlas_relative_path = "data/3d/textures/MenusGraphics.tga";
// Bitmap19 is extracted exactly from retained generic SWF bitmap62.
inline constexpr const char* generic_button_asset = "port/windows-foundation/features/frontend/art/assets/generic_bitmap_62.tga";
// A pure authored-stage projection for the static diagnostic; source AS reflow
// belongs to the original CUI runtime and must not be inferred here.
bool geometry(Screen, int width, int height, HudGeometry&, std::string& error);
bool load(const std::filesystem::path& cache_root, std::string& error);
bool contains(const HitRegion&, float authored_x, float authored_y) noexcept;
}
