#pragma once
#include "text_render_owner_v2.hpp"
#include "edit_text_display_v1.hpp"

namespace gameswf { struct font; struct player;struct bitmap_info; }
namespace dh2::ui {
struct SwfEditTextFieldV1;
struct SwfTextPolicyV1 {
    // Exact default source root/player constructors; later native setters must
    // update these same owned fields. Reached cache/buffer providers remain
    // required instead of borrowing stock GameSWF scheduling or atlas state.
    bool buffering{},flushing{},auto_preload{},render_cache{},filter_engine{};
    std::function<bool(text_v1::State&,std::string&)> preload;
    // Reached non-default renderer features use the genuine backend. The
    // bridge never substitutes stock masks/caches for these source methods.
    std::function<bool(const edit_text_display_v1::Command&,std::string&)> renderer_feature;
    std::function<bool(bool&,std::string&)> cache_validate;
};
// One per movie/player. The source metrics and glyph raster use the same
// TextRenderOwnerV2 face. The returned facade services retain this platform,
// the original providers and their caller-owned lifetime through teardown.
class SwfTextFontPlatformV1 {
    struct Impl;std::shared_ptr<Impl> impl_;
    explicit SwfTextFontPlatformV1(std::shared_ptr<Impl>);
public:
    SwfTextFontPlatformV1(SwfFontServices,SwfServices,
                         std::shared_ptr<void> platform_owner,
                         TextFontBackendsV2,float source_provider_scale);
    ~SwfTextFontPlatformV1();
    SwfTextFontPlatformV1(const SwfTextFontPlatformV1&)=delete;
    SwfTextFontPlatformV1& operator=(const SwfTextFontPlatformV1&)=delete;
    // Explicit source capability, rather than inferring metrics from the old
    // glyph_provider interface. Caller installs before graph construction.
    SwfServices services() const;
    std::shared_ptr<text_v1::Font> font(gameswf::font*,std::string&);
    // Borrow only within the exact facade Scope. The descriptor/image owns the
    // source metadata; the platform retains the materialized core object.
    gameswf::font* core_font(const std::shared_ptr<text_v1::Font>&,gameswf::player*,std::string&);
    gameswf::bitmap_info* core_bitmap(const text_v1::Glyph&,std::string&);
    static std::shared_ptr<SwfTextFontPlatformV1> for_player(gameswf::player*);
    static bool set_source_text_buffering_v98(const SwfAsLease&,bool,std::string&);
    // Inspect the actual retained adapter and its source startup provider.
    // A font wrapper must not cause a second input/history owner to be bound.
    static bool owns_source_startup(const SwfServices&) noexcept;
    text_v1::Services layout_services(text_v1::Services) const;
    text_display_v2::Services display_services(text_display_v2::Services) const;
    float provider_scale() const;
    std::string error() const;
    SwfTextPolicyV1& policy();
    bool diagnostic_available() const;
    bool enqueue(std::weak_ptr<SwfEditTextFieldV1>,std::string&);
    bool flush_buffered_text(std::string&);
    // Clear the per-player source glyph/font projections after the caller has
    // blanked edit fields in this player's actual root.
    bool source_reset_fonts_v119(std::string&);
};
}
