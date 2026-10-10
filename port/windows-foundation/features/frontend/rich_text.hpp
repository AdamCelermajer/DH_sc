#pragma once
#include "art/original_art.hpp"
#include "../../hud_glyphs.hpp"
#include "../../overlay_renderer.hpp"
#include "../../renderer.hpp"
namespace dh::foundation::frontend {
class FrontendText {
public:
 bool load(const std::filesystem::path&,std::string&);
 bool rebuild(const art::ScreenArt&,int,int,Renderer&,std::string&);
 void draw(OverlayRenderer&)const;
 void clear(Renderer&);
private:
 HudGlyphFont font_;
 std::vector<OverlaySprite> sprites_;
 std::vector<std::uint32_t> textures_;
};
}
