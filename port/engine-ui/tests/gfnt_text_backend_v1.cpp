#include "../gfnt_text_backend_v1.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
using namespace dh2::ui;
struct Resource {std::string file;int width{},height{};unsigned resolves{},reads{},publish{};};
void check(bool value,const char* text){if(!value)throw std::runtime_error(text);}
bool resolve(void* p,const text_v1::Font& font,std::string& uri,bool& found,std::string&){auto& r=*static_cast<Resource*>(p);++r.resolves;found=font.name=="Symbol";uri=found?r.file:"data/Fontin SmallCaps.ttf";return true;}
bool read(void* p,const char* name,std::vector<std::uint8_t>& out,std::string&){++static_cast<Resource*>(p)->reads;std::ifstream f(name,std::ios::binary);if(!f)return false;out={std::istreambuf_iterator<char>(f),{}};return true;}
int main(int argc,char** argv){try{
 check(argc==2,"actual SCT_Font_3.fnt required");auto resource=std::make_shared<Resource>();resource->file=argv[1];
 GfntTextServicesV1 services{resource.get(),&resource->width,&resource->height,resolve,read,nullptr};
 GfntTextBackendV1 owner(services,resource);text_v1::Font symbol;symbol.name="Symbol";TextBitmapFaceV2 face;std::string error;
 check(owner.face(symbol,face,error),error.c_str());check(face.owner&&face.height==51.2f,"actual source ctor scale missing");
 text_v1::Font alias=symbol;alias.bold=true;TextBitmapFaceV2 second;check(owner.face(alias,second,error)&&second.owner==face.owner,"actual filename aliases did not reuse face");check(resource->reads==1,"actual GFNT re-read on alias");
 text_v1::Glyph glyph;glyph.advance=512;glyph.x0=123;
 unsigned checks=0;for(unsigned code=32;code<32+8451;++code){check(owner.glyph(face,std::uint16_t(code),19,glyph,error),error.c_str());check(!glyph.image&&glyph.advance==512&&glyph.x0==123,"source zero-cache branch published fake glyph");++checks;}
 text_v1::Font ttf;ttf.name="Fontin SmallCaps";TextBitmapFaceV2 miss;check(owner.face(ttf,miss,error)&&!miss.owner,"source TTF selected as GFNT");
 resource->width=resource->height=512;check(!owner.glyph(face,65,19,glyph,error)&&!error.empty(),"enabled cache missing image provider silently succeeded");
 GfntFont raw;auto bytes=std::vector<std::uint8_t>{};check(read(resource.get(),argv[1],bytes,error)&&raw.load(bytes.data(),bytes.size(),error),error.c_str());GfntRaster raster;check(raw.borrow().raster(raster,65,19,error)==1,error.c_str());
 text_v1::Glyph geometry;check(gfnt_text_geometry_v1(geometry,raster,19,32,32,error),error.c_str());check(geometry.x1==.5f&&geometry.y1==19.f/32.f&&geometry.advance==raster.advance_twips,"original wrapper geometry mismatch");
 check(!gfnt_text_geometry_v1(geometry,raster,19,16,32,error),"source cache padding guard missing");
 std::cout<<"{\"validation\":\"PASS\",\"actual_slots\":"<<checks<<",\"same_resource_alias\":true,\"source_MenuManager_cache_zero\":true,\"enabled_cache_provider_failure_explicit\":true,\"resolver_fixture\":true}\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
