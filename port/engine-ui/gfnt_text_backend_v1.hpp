#pragma once
#include "gfnt.hpp"
#include "text_render_owner_v2.hpp"
#include <map>
#include <tuple>
namespace dh2::ui {
struct GfntTextServicesV1 {
 void* context{};
 // SAME RenderFX InitializationParameters+14/+18. MenuManager constructor
 // writes both zero; those are NOT the separate FreeType512 dimensions.
 const std::int32_t* bitmap_cache_width{};
 const std::int32_t* bitmap_cache_height{};
 // Same original FontResolve/cache. found=false is a delivered non-GFNT miss.
 bool(*resolve)(void*,const text_v1::Font&,std::string& uri,bool& found,std::string&){};
 bool(*read)(void*,const char*,std::vector<std::uint8_t>&,std::string&){};
 // Registers the actual source cache image with the retained TextRenderOwner.
 // The source cache rounds width+1,height+1 to minimum16/multiples16.
 // It clears the whole rounded region and copies original RGBA cell rows.
 // This MUST return a real registered image, not a fake void identity.
 bool(*publish)(void*,const std::shared_ptr<void>& face,std::uint16_t code,
  std::int32_t size,const GfntRaster&,std::int32_t region_width,
  std::int32_t region_height,std::shared_ptr<void>& image,std::string&){};
};
class GfntTextBackendV1 {
 struct Face;
 GfntTextServicesV1 services_;std::shared_ptr<void> lifetime_;
 std::map<std::tuple<std::string,bool,bool>,std::shared_ptr<Face>> faces_;
 std::map<std::string,std::weak_ptr<Face>> resources_;
public:
 GfntTextBackendV1(GfntTextServicesV1,std::shared_ptr<void> actual_resource_owner);
 bool face(const text_v1::Font&,TextBitmapFaceV2&,std::string&);
 bool glyph(const TextBitmapFaceV2&,std::uint16_t,std::int32_t,text_v1::Glyph&,std::string&);
 TextFontBackendsV2 backends();
};
// Original bitmap_font_entity::get_char_image7c59dc wrapper after GFNT raster.
// Does not create a cache/image or substitute geometry for a missing provider.
bool gfnt_text_geometry_v1(text_v1::Glyph&,const GfntRaster&,std::int32_t size,
 std::int32_t region_width,std::int32_t region_height,std::string&);
}
