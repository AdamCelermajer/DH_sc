#include "../gfnt_text_backend_v1.hpp"
#include <cstring>
extern "C" int gfnt_text_geometry_v1_oracle(std::uint32_t* out,const std::uint32_t* words,
 std::int32_t size,std::int32_t width,std::int32_t height){
 dh2::ui::GfntRaster raster;std::memcpy(&raster.source.metrics,words,20);
 dh2::ui::text_v1::Glyph glyph;std::string error;
 if(!dh2::ui::gfnt_text_geometry_v1(glyph,raster,size,width,height,error))return -1;
 const float fields[5]{glyph.x0,glyph.x1,glyph.y0,glyph.y1,glyph.advance};std::memcpy(out,fields,20);return 1;
}
