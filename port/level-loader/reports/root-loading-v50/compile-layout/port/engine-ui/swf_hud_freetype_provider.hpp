#pragma once
#include "swf_freetype_provider.hpp"
#include "hud_freetype_font.hpp"
namespace dh2::ui {
using HudBitmapProbe=void (*)(void*,const HudBitmapInfo32&);
bool swf_hud_face_metrics(gameswf::glyph_provider*,const std::string&,bool,bool,float&,float&,float&,std::string&);
class SwfHudFreetypeProvider {
public:
    explicit SwfHudFreetypeProvider(const SwfFontServices&,float source_scale=1.0f,HudBitmapProbe probe=nullptr);
    ~SwfHudFreetypeProvider();
    SwfHudFreetypeProvider(const SwfHudFreetypeProvider&)=delete;
    SwfHudFreetypeProvider& operator=(const SwfHudFreetypeProvider&)=delete;
    gameswf::glyph_provider* borrowed_provider() const;
    std::string error() const;
private:
    struct Impl;std::unique_ptr<Impl> impl_;
};
}
