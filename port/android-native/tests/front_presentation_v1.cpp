// Production Front functions are extracted verbatim by the Python runner.
// Android asset reads and GPU uploads/draws are host endpoints; filename,
// texture decoding, source viewport math and presentation policy are real.
#include "splash_layout_v1.hpp"
#include "viewport.hpp"
#include "textures.hpp"
#include "swf_texture.hpp"
#include "sha256.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

struct AAssetManager {std::filesystem::path directory;};
struct AAsset {std::vector<std::uint8_t> bytes;std::size_t at{};};
constexpr int AASSET_MODE_STREAMING=0,ANDROID_LOG_INFO=0;
int __android_log_print(int,const char*,const char*,...){return 0;}
std::vector<std::uint8_t> read_file(const std::filesystem::path& path){
 std::ifstream file(path,std::ios::binary);if(!file)throw std::runtime_error(path.string());
 return {std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>()};
}
AAsset* AAssetManager_open(AAssetManager* manager,const char* name,int){
 const auto path=manager->directory/name;if(!std::filesystem::is_regular_file(path))return nullptr;
 return new AAsset{read_file(path),0};
}
std::int64_t AAsset_getLength64(AAsset* value){return value->bytes.size();}
int AAsset_read(AAsset* value,void* out,std::size_t n){
 n=std::min(n,value->bytes.size()-value->at);std::memcpy(out,value->bytes.data()+value->at,n);value->at+=n;return int(n);
}
void AAsset_close(AAsset* value){delete value;}

namespace dh2::resources {
enum class ResourceScopeV37 {swf_front};
bool swf_bitmap_bytes_v39(int w,int h,unsigned channels,std::uint64_t& bytes,std::string&){bytes=std::uint64_t(w)*h*channels;return true;}
struct RetainedBytesV39 {
 std::vector<std::uint8_t> bytes;
 bool allocate(int,ResourceScopeV37,std::size_t n,std::string&){bytes.resize(n);return true;}
 std::uint8_t* data(){return bytes.data();}std::size_t size()const{return bytes.size();}
};
}
namespace dh2::ui {
struct SwfMatrix {float value[6]{1,0,0,0,1,0};};
struct SwfTexture {std::uintptr_t identity{};std::int32_t width{},height{};};
struct SwfFill {enum Kind {disabled,color,bitmap} kind{disabled};SwfTexture texture;SwfMatrix uv;};
struct SwfDraw {
 enum Kind {begin,end,triangles,triangle_strip,line_strip,bitmap_quad,mask_begin,mask_end,mask_disable,antialias} kind{};
 SwfMatrix matrix;SwfFill fill;std::vector<float> xy;
 float rect[4]{},uv_rect[4]{},bounds[4]{};std::int32_t viewport[4]{};
};
}
namespace dh2::android_ui {
struct Cache {
 std::filesystem::path directory;std::vector<std::string> requested;
 bool read(const std::string& uri,bool& present,std::vector<std::uint8_t>& out,std::string&){
  requested.push_back(uri);const auto file=directory/uri;present=std::filesystem::is_regular_file(file);
  if(present)out=read_file(file);return true;
 }
};
class OriginalUiAssets {
public:
 AAssetManager* manager_{};mutable Cache cache_;
 bool read(std::string_view,std::vector<std::uint8_t>&,std::string&)const;
};
namespace {
struct Entry {const char* key;const char* uri;const char* asset;const char* digest;std::uint32_t bytes;};
const Entry catalog[]={
#include "catalog_under_test.inc"
};
struct Close {void operator()(AAsset* p)const{if(p)AAsset_close(p);}};
constexpr const char* tag="host";
}
#include "assets_read_under_test.inc"
struct Gpu {
 std::uintptr_t next=1;std::map<std::uintptr_t,std::vector<std::uint8_t>> images;std::vector<ui::SwfDraw> draws;
 int resource_budget_lease_v39(){return 0;}
 bool image(int w,int h,unsigned,const std::uint8_t* pixels,std::size_t pitch,ui::SwfTexture& out,std::string&){
  out={next++,w,h};images[out.identity]={pixels,pixels+pitch*h};return true;
 }
 bool draw(const ui::SwfDraw& command,std::string&){draws.push_back(command);return true;}
 void abort(){}
};
class FrontUiSessionV87 {
public:
 struct Impl {
  OriginalUiAssets assets;Gpu gpu;std::map<std::string,ui::SwfTexture> exports;
  int driver_width{};ui::SwfTexture process_splash_v119;std::string process_splash_uri_v119,splash_source_uri_v1;
  std::uintptr_t splash_texture_identity_v1{};bool rendering_splash_clip_v1{},loading_bitmap_reported{};
  std::string font_failure,front_screen;unsigned bitmap_uploads{},strips{},lines{},masks{};
#include "front_texture_under_test.inc"
#include "front_draw_under_test.inc"
 };
 std::shared_ptr<Impl> impl_=std::make_shared<Impl>();
 bool load_process_splash_v119(std::int32_t,std::string&);
 bool render_process_splash_v119(int,int,std::string&);
 bool clear_process_splash_v119(std::string&);
};
#include "front_splash_under_test.inc"
struct ClipProbe {
 std::function<bool(std::string&)> display;
 bool display_source_stage_clip_v5(const char*,std::string& error){return display(error);}
};
bool display_front_clip(FrontUiSessionV87::Impl& self,const std::string& path,ClipProbe* movie,std::string& e){
 struct {std::string actual_clip_path;} q{path};
#include "front_splash_scope_under_test.inc"
 return true;
}
}

// Execute the existing scene viewport owner from renderer_front_exports_v87.
namespace model_renderer {
using GLuint=unsigned;
bool menu_background=true,menu_background_character_visible_v137=true;
std::array<int,4> viewport{};std::vector<std::array<int,4>> scene_draws;
bool active(){return true;}
void glViewport(int x,int y,int w,int h){viewport={x,y,w,h};}
void draw(int,int){scene_draws.push_back(viewport);}
#include "menu_viewport_under_test.inc"
}

// The source camera math used by the actual retained SWF owns its viewport.
int camera_driver(void* context,dh2::ui::ViewportState64* state,const dh2::ui::ViewportRequest40* request,dh2::ui::ViewportResponse16* response){
 using namespace dh2::ui;const auto& dimensions=*static_cast<const std::array<int,2>*>(context);
 switch(request->operation){
 case ViewportOperation::orientation:response->values[0]=0;return 1;
 case ViewportOperation::driver_dimensions:response->values[0]=dimensions[0];response->values[1]=dimensions[1];return 1;
 case ViewportOperation::camera_set_viewport:{ViewportServices16 services{context,camera_driver};return dh2_ui_set_viewport(state,request->values,&services)==0;}
 case ViewportOperation::camera_set_bounds:{ViewportServices16 services{context,camera_driver};return dh2_ui_set_bounds(state,request->values,0,&services)==0;}
 case ViewportOperation::publish_viewport:return 1;
 }
 return 0;
}

int main(int argc,char** argv){
 assert(argc==4);using namespace dh2;std::string error;AAssetManager manager{argv[1]};
 // Fixture rows contain widths/languages and the exact shipping TGA URI.
 std::ifstream cases(argv[3]);int width,language;std::string uri;
 while(cases>>width>>language>>uri){
  android_ui::FrontUiSessionV87 session;auto& self=*session.impl_;
  self.assets.manager_=&manager;self.assets.cache_.directory=argv[2];self.driver_width=width;
  // prepare_process_front_v119 selects this URI before importing dqmenus;
  // model the authored menu_splash bitmap delivery before GSInit loads its
  // startup quad, and require both phases to keep the same retained texture.
  self.splash_source_uri_v1=ui::splash_source_uri_v1(width,language);
  ui::SwfTexture touch_to_continue;assert(android_ui::FrontUiSessionV87::Impl::texture(
   &self,"menus/splash_final_droid.tga",0,0,touch_to_continue,error));
  assert(self.splash_texture_identity_v1==touch_to_continue.identity);
  assert(session.load_process_splash_v119(language,error));
  assert(self.splash_source_uri_v1==uri);const auto initial=self.process_splash_v119;
  assert(initial.identity==touch_to_continue.identity);
  if(uri!="data/3d/textures/splash_final_droid.tga")
   assert((self.assets.cache_.requested==std::vector<std::string>{uri}));
  else assert(self.assets.cache_.requested.empty());
  const auto source=read_file(std::filesystem::path(argv[2])/uri);textures::View view{};
  assert(dh2_texture_open(source.data(),source.size(),&view)==textures::Error::ok);
  std::vector<std::uint8_t> pixels(std::size_t(view.width)*view.height*4);
  assert(dh2_texture_decode(&view,pixels.data(),pixels.size())==textures::Error::ok);
  assert(self.gpu.images.at(initial.identity)==pixels);
  assert(session.render_process_splash_v119(2400,1080,error));
  assert(self.gpu.draws.size()==3);const auto& background=self.gpu.draws[1];
  assert(background.fill.texture.identity==initial.identity);
  assert(background.rect[1]==1280&&background.rect[3]==752);
  // Compare the actual source-pixel rectangle submitted by GSInit with the
  // authored menu_splash fill checked below. The two displays must sample the
  // same top-left 1280x752 pixels of this exact retained bitmap, not merely
  // happen to report the same texture URI/name.
  assert(background.uv_rect[0]==0.f&&background.uv_rect[2]==0.f);
  assert(std::abs(background.uv_rect[1]*initial.width-1280.f)<.001f);
  assert(std::abs(background.uv_rect[3]*initial.height-752.f)<.001f);
  assert(self.gpu.draws[0].viewport[2]==2400&&self.gpu.draws[0].viewport[3]==1080);
  assert(session.clear_process_splash_v119(error));
  // Different export names from droid/i9000/generic/JP/Kor movies still
  // resolve to the one selected source, including after GSInit releases it.
  for(const auto* name:{"splash_final.tga","menus/splash_final_droid.tga","menus/splash_final_i9000.tga","splash_final_jp.tga","splash_final_kor.tga"}){
   ui::SwfTexture touch;assert(android_ui::FrontUiSessionV87::Impl::texture(&self,name,0,0,touch,error));
   assert(touch.identity==initial.identity);assert(self.splash_texture_identity_v1==touch.identity);
  }
  assert(self.bitmap_uploads==1&&self.exports.size()==1);
 }
 // UV values are generated from the ORIGINAL droid/i9000 shape records by
 // the runner, not copied from the correction's output formula.
 std::ifstream uv_file(std::filesystem::path(argv[2])/"background-uv.txt");
 float input[6];for(auto& value:input)assert(bool(uv_file>>value));
 android_ui::FrontUiSessionV87::Impl self;self.splash_texture_identity_v1=7;self.rendering_splash_clip_v1=true;
 ui::SwfDraw command;command.kind=ui::SwfDraw::triangle_strip;command.fill.kind=ui::SwfFill::bitmap;
 command.fill.texture={7,2048,1024};std::copy_n(input,6,command.fill.uv.value);
 command.xy={-4838,-3284,4780,-3284,-4838,3141,4780,3141};
 // Breathing/motion belongs to this authored per-frame transform. The draw
 // policy may alter the fill but must retain every supplied transform/vertex.
 for(float scale:{.98f,1.f,1.02f}){
  command.matrix.value[0]=command.matrix.value[4]=scale;command.matrix.value[2]=125.f;
  assert(android_ui::FrontUiSessionV87::Impl::draw(&self,command,error));const auto& drawn=self.gpu.draws.back();
  assert(drawn.xy==command.xy);assert(std::memcmp(drawn.matrix.value,command.matrix.value,sizeof(command.matrix.value))==0);
  const auto& uv=drawn.fill.uv.value;
  assert(std::abs(uv[0]*-4838+uv[2])<.001f);assert(std::abs(uv[0]*4780+uv[2]-1280)<.001f);
  assert(std::abs(uv[4]*-3284+uv[5])<.001f);assert(std::abs(uv[4]*3141+uv[5]-752)<.001f);
  assert(std::abs((uv[0]*-4838+uv[2])-0.f)<.001f);
  assert(std::abs((uv[0]*4780+uv[2])-1280.f)<.001f);
  assert(std::abs((uv[4]*-3284+uv[5])-0.f)<.001f);
  assert(std::abs((uv[4]*3141+uv[5])-752.f)<.001f);
 }
 self.rendering_splash_clip_v1=false;
 assert(android_ui::FrontUiSessionV87::Impl::draw(&self,command,error));
 assert(std::memcmp(self.gpu.draws.back().fill.uv.value,input,sizeof(input))==0);
 self.rendering_splash_clip_v1=true;command.fill.texture.identity=8;
 assert(android_ui::FrontUiSessionV87::Impl::draw(&self,command,error));
 assert(std::memcmp(self.gpu.draws.back().fill.uv.value,input,sizeof(input))==0);
 command.fill.texture.identity=7;self.rendering_splash_clip_v1=false;
 android_ui::ClipProbe probe{[&](std::string& e){return android_ui::FrontUiSessionV87::Impl::draw(&self,command,e);}};
 assert(android_ui::display_front_clip(self,"_root.menu_splash",&probe,error));
 assert(!self.rendering_splash_clip_v1);
 assert(std::memcmp(self.gpu.draws.back().fill.uv.value,input,sizeof(input))!=0);
 assert(android_ui::display_front_clip(self,"_root.menu_MainMenu",&probe,error));
 assert(std::memcmp(self.gpu.draws.back().fill.uv.value,input,sizeof(input))==0);
 probe.display=[](std::string&){return false;};
 assert(!android_ui::display_front_clip(self,"_root.menu_splash",&probe,error));
 assert(!self.rendering_splash_clip_v1); // source-scope cleanup on failure
 self.rendering_splash_clip_v1=true;
 command.fill.texture.identity=7;command.fill.uv.value[0]=.05f; // other sprite in same atlas
 assert(android_ui::FrontUiSessionV87::Impl::draw(&self,command,error));
 assert(self.gpu.draws.back().fill.uv.value[0]==.05f);
 for(const auto dimensions:{std::array<int,2>{854,480},{800,480},{1920,1080},{2400,1080},{1080,1920}}){
  const auto w=dimensions[0],h=dimensions[1];model_renderer::viewport={123,0,480,320};
  model_renderer::draw_menu_background(w,h,false);
  const auto scene_w=std::min(w,h*3/2),scene_h=std::min(h,w*2/3);
  const auto scene_x=(w-scene_w)/2,scene_y=(h-scene_h)/2;
  assert((model_renderer::scene_draws.back()==std::array<int,4>{scene_x,scene_y,scene_w,scene_h}));
  assert((model_renderer::viewport==std::array<int,4>{0,0,w,h}));
  assert(model_renderer::menu_background_character_visible_v137); // admission restored
  for(const auto authored:{std::array<float,4>{0,9600,0,6400},{0,20480,0,15360}}){
   ui::ViewportState64 state{};std::copy(authored.begin(),authored.end(),state.movie_rect);
   state.viewport[2]=state.bounds[2]=480;state.viewport[3]=state.bounds[3]=320;
   ui::FlashCamera40 camera{};ui::ViewportServices16 driver{const_cast<std::array<int,2>*>(&dimensions),camera_driver};
   assert(dh2_ui_flash_camera_update(&camera,&state,&driver)==0);
   assert(state.viewport[0]==0&&state.viewport[1]==0&&state.viewport[2]==w&&state.viewport[3]==h);
   assert(state.bounds[0]==0&&state.bounds[1]==0&&state.bounds[2]==w&&state.bounds[3]==h);
   float rectangle[4];assert(dh2_ui_display_rectangle(&state,rectangle,&driver)==0);
   assert(std::equal(std::begin(rectangle),std::end(rectangle),authored.begin()));
  }
 }
 std::puts("PASS production Front texture identity, authored splash UV/animation, centered 3:2 menu scene and full-surface SWF viewport");
}
