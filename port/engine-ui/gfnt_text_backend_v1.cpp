#include "gfnt_text_backend_v1.hpp"
#include <cstring>
#include <stdexcept>
namespace dh2::ui {
namespace {
std::uint32_t be(const std::uint8_t* p){return(std::uint32_t(p[0])<<24)|(std::uint32_t(p[1])<<16)|(std::uint32_t(p[2])<<8)|p[3];}
float div(float a,float b){volatile float v=a/b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
std::int32_t rounded(std::uint32_t n){return std::int32_t((n+15u)&~15u);}
}
struct GfntTextBackendV1::Face {
 GfntFont owner;GfntFont::Borrow font;float height{};
 std::shared_ptr<void> resource_owner;
 std::map<std::uint32_t,text_v1::Glyph> glyphs;
};
GfntTextBackendV1::GfntTextBackendV1(GfntTextServicesV1 services,std::shared_ptr<void> owner):services_(services),lifetime_(std::move(owner)){
 if(!lifetime_||!services_.resolve||!services_.read||!services_.bitmap_cache_width||!services_.bitmap_cache_height)
  throw std::invalid_argument("GFNT backend requires actual resolver/resource/cache image owners");
}
bool GfntTextBackendV1::face(const text_v1::Font& descriptor,TextBitmapFaceV2& out,std::string& error){
 error.clear();const auto key=std::make_tuple(descriptor.name,descriptor.bold,descriptor.italic);
 auto entry=faces_.find(key);
 if(entry==faces_.end()){
  std::string uri;bool found{};
  if(!services_.resolve(services_.context,descriptor,uri,found,error))return false;
  if(!found){faces_.emplace(key,nullptr);out={};return true;}
  if(uri.empty()){error="Delivered GFNT resource has no source filename";return false;}
  auto next=resources_[uri].lock();
  if(!next){
   std::vector<std::uint8_t> bytes;
   if(!services_.read(services_.context,uri.c_str(),bytes,error))return false;
   next=std::make_shared<Face>();
   if(!next->owner.load(bytes.data(),bytes.size(),error))return false;
   const float base=float(be(bytes.data()+28));
   next->height=mul(base,div(1024.f,mul(base,20.f)));
   next->font=next->owner.borrow();next->resource_owner=lifetime_;
   resources_[uri]=next;
  }
  entry=faces_.emplace(key,std::move(next)).first;
 }
 if(!entry->second){out={};return true;}
 out={entry->second,entry->second->height};return true;
}
bool gfnt_text_geometry_v1(text_v1::Glyph& out,const GfntRaster& raster,std::int32_t size,
 std::int32_t width,std::int32_t height,std::string& error){
 if(size<=0||size>65535||width<16||height<16||(width&15)||(height&15)||
  std::uint32_t(width)<raster.source.metrics.width+1u||std::uint32_t(height)<raster.source.metrics.height+1u){error="Malformed source GFNT cache region/size";return false;}
 const auto& m=raster.source.metrics;text_v1::Glyph next{};
 next.advance=mul(float(m.advance),20.f);next.height=std::int16_t(std::uint16_t(size));next.index=-1;next.type=0;
 next.x1=div(float(m.width),float(width));next.y1=div(float(m.height),float(height));
 const std::uint32_t negative=0u-m.bearing_x;std::int32_t signed_negative{};std::memcpy(&signed_negative,&negative,4);
 next.x0=mul(div(float(signed_negative),float(m.width)),-next.x1);
 next.y0=mul(div(float(m.baseline),float(m.height)),next.y1);
 out=std::move(next);return true;
}
bool GfntTextBackendV1::glyph(const TextBitmapFaceV2& face,std::uint16_t code,
 std::int32_t size,text_v1::Glyph& out,std::string& error){
 error.clear();if(!face.owner){error="GFNT glyph requires retained selected face";return false;}
 std::shared_ptr<Face> selected;
 for(const auto& item:faces_)if(item.second&&item.second==face.owner){selected=item.second;break;}
 if(!selected){error="GFNT face belongs to another source backend";return false;}
 if(size<=0||size>65535){error="GFNT glyph size outside original16bit cache domain";return false;}
 const auto key=std::uint32_t(code)|(std::uint32_t(size)<<16);
 auto existing=selected->glyphs.find(key);if(existing!=selected->glyphs.end()){out=existing->second;return true;}
 GfntRaster raster;const int status=selected->font.raster(raster,code,size,error);
 if(status<0)return false;
 if(!status){out.image.reset();return true;}
 // Original7c5a98..7c5bf8: the source constructor's disabled cache returns
 // a genuine bitmap-image miss after the raw GFNT query. No glyph geometry
 // or advance is published; the real font owner proceeds to its FT/embedded
 // continuation. Never turn FreeType512 into a manufactured bitmap atlas.
 if(*services_.bitmap_cache_width<=0||*services_.bitmap_cache_height<=0){out.image.reset();return true;}
 if(!services_.publish){error="Required enabled source GFNT cache/image owner";return false;}
 const auto width=rounded(raster.source.metrics.width+1u),height=rounded(raster.source.metrics.height+1u);
 text_v1::Glyph next;
 if(!gfnt_text_geometry_v1(next,raster,size,width,height,error))return false;
 if(!services_.publish(services_.context,selected,code,size,raster,width,height,next.image,error))return false;
 if(!next.image){error="GFNT source cache did not deliver registered image";return false;}
 selected->glyphs.emplace(key,next);out=std::move(next);return true;
}
TextFontBackendsV2 GfntTextBackendV1::backends(){
 TextFontBackendsV2 result;
 result.bitmap_face=[this](const auto& f,auto& out,std::string& e){return face(f,out,e);};
 result.bitmap_glyph=[this](const auto& f,auto code,auto size,auto& out,std::string& e){return glyph(f,code,size,out,e);};
 return result;
}
}
