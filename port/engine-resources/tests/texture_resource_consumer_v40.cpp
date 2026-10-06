// Actual model helper, actual staged effect adapter and actual ZIP payload
// reader; fake driver owns only byte/name metadata and a four-byte sample.
#include "../admitted_cpu_bytes_v40.hpp"
#include "../retained_bytes_v39.hpp"
#include "../../engine-textures/general_fx_texture_image_v2.hpp"
#include "../../engine-textures/texture_binding_owner_v1.hpp"
#include "../../engine-textures/texture_unbind_v1.hpp"
#include "../../engine-textures/texture_mipmap_v1.hpp"
#include "../../engine-textures/texture_driver_fields_v1.hpp"
#include "../reports/texture-resource-v40/zip_asset_pack_v1.hpp"
#include "../reports/texture-resource-v40/original_cache_assets_v1.hpp"
#include <GLES2/gl2.h>
#include <android/log.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <new>
#include <stdexcept>
#include <utility>
using namespace dh2::resources;
#ifndef __ANDROID__
constexpr int AASSET_MODE_STREAMING=2;
#define ANDROID_LOG_WARN 5
#endif
static std::shared_ptr<ContextResourceBudgetV37> ledger;
static bool track_pixels,fail_decode,fail_cache;
static unsigned checks,encoded_allocations,decoded_allocations;
static void ck(bool x,const char* message){if(!x)throw std::runtime_error(message);++checks;}
__attribute__((noinline)) void* operator new(std::size_t n){
 // Probe only the two known payload allocation sizes/publication injection.
 // Budget diagnostics allocate other strings while holding the ledger lock.
 if(track_pixels&&ledger&&(n==82||n==64||fail_cache)){const auto s=ledger->snapshot();
  if(n==82&&s.pending.cpu_bytes>=82)++encoded_allocations;
  if(n==64&&s.pending.cpu_bytes==64){++decoded_allocations;if(fail_decode){fail_decode=false;throw std::bad_alloc();}}
  if(fail_cache&&s.pending.gpu_bytes&&s.pending.cpu_bytes==0&&s.live.cpu_bytes==146&&n>=64&&n<128){fail_cache=false;throw std::bad_alloc();}
 }
 if(auto* p=std::malloc(n?n:1))return p;
 throw std::bad_alloc();
}
__attribute__((noinline)) void operator delete(void* p)noexcept{std::free(p);}
__attribute__((noinline)) void operator delete(void* p,std::size_t)noexcept{std::free(p);}
__attribute__((noinline)) void* operator new[](std::size_t n){
 if(track_pixels&&ledger&&n==64){ck(ledger->snapshot().requested.cpu_bytes>=64,"Model decode allocation preceded admission");++decoded_allocations;if(fail_decode){fail_decode=false;throw std::bad_alloc();}}
 if(auto* p=std::malloc(n?n:1))return p;
 throw std::bad_alloc();
}
__attribute__((noinline)) void operator delete[](void* p)noexcept{std::free(p);}
__attribute__((noinline)) void operator delete[](void* p,std::size_t)noexcept{std::free(p);}
namespace dh2::android_resources {
std::shared_ptr<ContextResourceBudgetV37> budget_lease_v39(){return ledger;}
}
static dh2::assets::ZipAssetPackV1 fixture_pack;
static std::vector<std::uint8_t> tga;
static bool short_read;
static unsigned asset_opens,asset_closes,zip_admissions;
namespace dh2::android_ui {
bool OriginalCacheAssetsV1::read_admitted(const std::string& name,bool& found,std::vector<std::uint8_t>& out,const std::function<bool(std::uint32_t,std::string&)>& cb,std::string& error)const{
 return fixture_pack.read_admitted(name,found,out,[&](auto n,std::string& e){++zip_admissions;return cb(n,e);},error);
}
}
struct Asset {std::size_t cursor{};};
extern "C" {
AAsset* AAssetManager_open(AAssetManager*,const char* name,int mode){ck(!std::strcmp(name,"textures/tiny.tga")&&mode==AASSET_MODE_STREAMING,"Bundled texture read changed filename/mode");++asset_opens;return reinterpret_cast<AAsset*>(new Asset);}
off64_t AAsset_getLength64(AAsset*){return off64_t(tga.size());}
int AAsset_read(AAsset* raw,void* out,std::size_t n){auto& a=*reinterpret_cast<Asset*>(raw);if(short_read)return -1;n=std::min(n,tga.size()-a.cursor);std::memcpy(out,tga.data()+a.cursor,n);a.cursor+=n;return int(n);}
void AAsset_close(AAsset* raw){delete reinterpret_cast<Asset*>(raw);++asset_closes;}
#ifdef __ANDROID__
int __android_log_print(int,const char*,const char*,...){return 0;}
#endif
}
struct Texture {std::uint64_t bytes{};unsigned width{},height{};std::map<GLenum,GLint> parameters;std::array<unsigned char,4> sample{};};
struct Driver {
 std::map<GLuint,Texture> textures;
 GLuint next{1};unsigned active{},creates{},deletes{},images{},mips{},bindings{};
 std::array<GLuint,8> bound{};GLint alignment{4};GLenum error{};
 bool zero_name{},fail_image{},fail_mips{},loss_after_mips{};std::function<void()> at_error;
} driver;
extern "C" {
void glGetIntegerv(GLenum p,GLint* out){
 if(p==GL_MAX_TEXTURE_SIZE)*out=4096;
 else if(p==GL_MAX_TEXTURE_IMAGE_UNITS)*out=2;
 else if(p==GL_ACTIVE_TEXTURE)*out=GL_TEXTURE0+driver.active;
 else if(p==GL_UNPACK_ALIGNMENT)*out=driver.alignment;
 else if(p==GL_TEXTURE_BINDING_2D)*out=GLint(driver.bound.at(driver.active));
 else throw std::runtime_error("Unexpected driver query");
}
void glGetFloatv(GLenum,GLfloat* out){*out=1;}
const GLubyte* glGetString(GLenum){return reinterpret_cast<const GLubyte*>("");}
void glGenTextures(GLsizei n,GLuint* out){
 ck(ledger->snapshot().pending.objects[std::size_t(ResourceKindV37::texture)]>=std::uint64_t(n),"Texture GL allocation preceded admission");
 for(int i=0;i<n;++i){if(driver.zero_name){driver.zero_name=false;out[i]=0;continue;}out[i]=driver.next++;driver.textures.emplace(out[i],Texture{});++driver.creates;}
}
void glDeleteTextures(GLsizei n,const GLuint* names){for(int i=0;i<n;++i)if(names[i]){driver.textures.erase(names[i]);for(auto& bound:driver.bound)if(bound==names[i])bound=0;++driver.deletes;}}
void glActiveTexture(GLenum unit){driver.active=unit-GL_TEXTURE0;}
void glBindTexture(GLenum,GLuint name){driver.bound.at(driver.active)=name;++driver.bindings;}
void glTexParameteri(GLenum,GLenum p,GLint value){driver.textures.at(driver.bound.at(driver.active)).parameters[p]=value;}
void glTexParameterf(GLenum,GLenum,GLfloat){}
void glPixelStorei(GLenum,GLint value){driver.alignment=value;}
void glTexImage2D(GLenum,GLint,GLint,GLsizei w,GLsizei h,GLint,GLenum,GLenum,const void* pixels){
 const auto bytes=std::uint64_t(w)*h*4;
 ck(ledger->snapshot().pending.texture_gpu_bytes>=bytes,"Texture storage preceded GPU byte admission");
 auto& t=driver.textures.at(driver.bound.at(driver.active));t.bytes=bytes;t.width=w;t.height=h;std::memcpy(t.sample.data(),pixels,4);++driver.images;
 ck(t.sample==std::array<unsigned char,4>{41,29,17,255}||t.sample==std::array<unsigned char,4>{255,255,255,255},"Actual decoder/white texture pixel bytes changed");
 if(driver.fail_image){driver.fail_image=false;driver.error=GL_OUT_OF_MEMORY;}
}
void glCompressedTexImage2D(GLenum,GLint,GLenum,GLsizei,GLsizei,GLint,GLsizei,const void*){throw std::runtime_error("Actual RGBA path unexpectedly became compressed");}
void glGenerateMipmap(GLenum){
 auto& t=driver.textures.at(driver.bound.at(driver.active));std::uint64_t full=0;
 for(unsigned w=t.width,h=t.height;;w=std::max(1u,w/2),h=std::max(1u,h/2)){full+=std::uint64_t(w)*h*4;if(w==1&&h==1)break;}
 ck(ledger->snapshot().pending.texture_gpu_bytes>=full,"Mipmap generation preceded full-chain admission");t.bytes=full;++driver.mips;
 if(driver.fail_mips){driver.fail_mips=false;driver.error=GL_OUT_OF_MEMORY;}
}
GLenum glGetError(){if(driver.at_error&&(!driver.loss_after_mips||driver.mips)){auto callback=std::move(driver.at_error);driver.at_error={};callback();}return std::exchange(driver.error,GL_NO_ERROR);}
void glDeleteBuffers(GLsizei,const GLuint*){}
}
static void check(const char*){if(glGetError()!=GL_NO_ERROR)throw std::runtime_error("Injected GL OOM");}
struct Draw {GLuint vertices{},indices{};};
#include "../../android-native/app/src/main/cpp/renderer_model_texture_budget_v40.inc"
#include "../reports/texture-resource-v40/actual-effect-owner.inc"
std::map<std::string,std::shared_ptr<EffectGpuTextureV4>> effect_gpu_textures_v4;
dh2::textures::TextureDriverOptionsOwnerV1 effect_driver_options_v4;
#include "../reports/texture-resource-v40/renderer_effect_texture_v5.inc"
static void start(std::uint64_t cpu=4096,std::uint64_t gpu=4096){
 ck(model_texture_charges_v40.empty()&&effect_gpu_textures_v4.empty(),"Previous case retained texture owners");
 track_pixels=false;ResourceBudgetLimitsV37 limits;limits.cpu_bytes=cpu;limits.gpu_bytes=gpu;limits.texture_gpu_bytes=gpu;limits.individual_cpu_bytes=4096;limits.individual_gpu_bytes=4096;
 ledger=std::make_shared<ContextResourceBudgetV37>(limits);std::string e;ck(ledger->begin_context(e),"Context begin failed");driver={};effect_texture_grid_v5={};effect_texture_cache_v5={};effect_texture_units_v5=0;effect_driver_options_v4={};encoded_allocations=decoded_allocations=zip_admissions=0;track_pixels=true;
}
static void empty(){ck(driver.textures.empty()&&ledger->snapshot().occupied_records==0,"Case leaked actual names/accounting");}
static void failed_fx(dh2::android_ui::OriginalCacheAssetsV1& cache,const char* label){bool thrown=false;try{effect_texture_v4("tiny.tga",cache);}catch(const std::exception&){thrown=true;}ck(thrown,label);ck(effect_gpu_textures_v4.empty(),"Failed FX cache publication survived");empty();}
static void inject_context_loss(ResourceTokenV37& unrelated,GLuint& name,unsigned& bindings){
 driver.at_error=[&unrelated,&name,&bindings](){
  ledger->context_lost();driver.textures.clear();driver.bound={};driver.active=0;driver.next=1;std::string error;
  ck(ledger->begin_context(error),"Replacement context failed");
  ResourceReservationV37 admission;ck(ledger->reserve_create({ResourceKindV37::texture,ResourceScopeV37::actor,1,0},admission,error),"Unrelated context texture admission failed");
  glGenTextures(1,&name);driver.textures.at(name).bytes=1;glBindTexture(GL_TEXTURE_2D,name);
  ck(admission.commit(unrelated,error),"Unrelated context texture commit failed");bindings=driver.bindings;
 };
}
static void clear_unrelated(ResourceTokenV37& token,GLuint name){std::string error;glDeleteTextures(1,&name);ck(ledger->release(token,error),"Unrelated texture release failed");empty();}
int main(int argc,char** argv){try{
 ck(argc==2,"Tiny ZIP fixture required");
 auto archive=std::make_shared<std::vector<std::uint8_t>>();{std::ifstream f(argv[1],std::ios::binary);archive->assign(std::istreambuf_iterator<char>(f),{});}ck(archive->size()<4096,"Fixture archive exceeded tiny bound");
 dh2::assets::ZipBackingV1 backing{archive,archive->size(),[archive](auto at,void* p,std::size_t n,std::string&){if(at>archive->size()||n>archive->size()-at)return false;std::memcpy(p,archive->data()+at,n);return true;}};
 std::string error;ck(fixture_pack.mount(backing,"root/",error),error.c_str());bool present;
 ck(fixture_pack.read("data/3d/textures/tiny.tga",present,tga,error)&&present&&tga.size()==82,"Tiny actual ZIP reader payload failed");
 dh2::android_ui::OriginalCacheAssetsV1 cache;
 start(80);failed_fx(cache,"Encoded quota denial accepted");ck(encoded_allocations==0&&zip_admissions==1,"Encoded quota denial occurred after payload allocation");
 start(120);failed_fx(cache,"Decode quota denial accepted");ck(encoded_allocations==1&&decoded_allocations==0&&driver.creates==0,"Decode quota refusal allocated pixels/GL");
 start(4096,80);failed_fx(cache,"Full mip GPU quota denial accepted");ck(decoded_allocations==0&&driver.creates==0,"Full-chain GPU refusal allocated decode/GL");
 start();fail_decode=true;failed_fx(cache,"Injected FX pixel allocation failure accepted");ck(decoded_allocations==1,"FX decoder did not reach admitted original allocation");
 start();fail_cache=true;failed_fx(cache,"Injected FX map publication failure accepted");ck(!fail_cache&&driver.creates==0,"FX cache metadata was allocated after GL creation");
 start();driver.zero_name=true;failed_fx(cache,"Zero source FX texture name accepted");
 start();driver.fail_image=true;failed_fx(cache,"FX base OOM accepted");
 start();driver.fail_mips=true;failed_fx(cache,"FX mip-generation OOM accepted");
 start();ResourceTokenV37 unrelated;GLuint unrelated_name{};unsigned actor_bindings{};
 driver.loss_after_mips=true;inject_context_loss(unrelated,unrelated_name,actor_bindings);
 bool threw=false;try{effect_texture_v4("tiny.tga",cache);}catch(const std::exception&){threw=true;}
 ck(threw&&effect_gpu_textures_v4.empty()&&driver.textures.count(unrelated_name)&&ledger->snapshot().live.gpu_bytes==1&&ledger->snapshot().live.cpu_bytes==0,"FX context-loss cleanup lost unrelated name or CPU accounting");
 ck(driver.bindings==actor_bindings&&driver.bound[0]==unrelated_name,"Old FX state guard rebound recycled old-context names");clear_unrelated(unrelated,unrelated_name);
 start();auto fx=effect_texture_v4("tiny.tga",cache);
 ck(fx->owner().ready()&&driver.mips==1&&ledger->snapshot().live.gpu_bytes==84&&ledger->snapshot().live.cpu_bytes==146,"Source effect pixels/mips/accounting wrong");
 const auto& sample=driver.textures.at(fx->texture);ck(sample.parameters.at(GL_TEXTURE_MIN_FILTER)==GL_LINEAR_MIPMAP_NEAREST&&sample.parameters.at(GL_TEXTURE_MAG_FILTER)==GL_LINEAR&&sample.parameters.at(GL_TEXTURE_WRAP_S)==GL_REPEAT&&sample.parameters.at(GL_TEXTURE_WRAP_T)==GL_REPEAT,"Source effect sampler changed");
 const auto allocations=encoded_allocations+decoded_allocations;ck(effect_texture_v4("tiny.tga",cache)==fx&&encoded_allocations+decoded_allocations==allocations&&driver.creates==1,"Warm source cache duplicated memory/GL");
 ck(release_effect_texture_v5(*fx,false,false,error),error.c_str());effect_gpu_textures_v4.clear();ck(ledger->snapshot().live.cpu_bytes==146,"Retained external FX owner lost actual CPU charge early");fx.reset();empty();
 start();ck(effect_driver_options_v4.set_option(0x10,false,{},error),"Actual no-mip option failed");fx=effect_texture_v4("tiny.tga",cache);ck(driver.mips==0&&ledger->snapshot().live.gpu_bytes==64,"Source no-mip texture overrode policy");ck(release_effect_texture_v5(*fx,false,false,error),error.c_str());effect_gpu_textures_v4.clear();fx.reset();empty();
 // Model: actual AAsset reader, exact pixels and original repeat/linear policy.
 start();std::map<std::string,GLuint> names;std::vector<GLuint> owned;auto* assets=reinterpret_cast<AAssetManager*>(1);
 const auto model=upload(assets,"tiny.tga",names,owned);ck(ledger->snapshot().live.cpu_bytes==0&&ledger->snapshot().live.gpu_bytes==64&&asset_opens==asset_closes,"Model retained temporary bytes or leaked reader");
 ck(driver.textures.at(model).parameters.at(GL_TEXTURE_MIN_FILTER)==GL_LINEAR&&driver.textures.at(model).parameters.at(GL_TEXTURE_WRAP_S)==GL_REPEAT,"Model sampler changed");
 ck(upload(assets,"tiny.tga",names,owned)==model&&owned.size()==1,"Model warm cache duplicated ownership");std::vector<Draw> draws;release(draws,owned);names.clear();empty();
 start();short_read=true;bool thrown=false;try{upload(assets,"tiny.tga",names,owned);}catch(const std::exception&){thrown=true;}short_read=false;ck(thrown&&asset_opens==asset_closes,"Short model read accepted/leaked asset");empty();
 start();driver.fail_image=true;thrown=false;try{upload(assets,"tiny.tga",names,owned);}catch(const std::exception&){thrown=true;}ck(thrown&&names.empty()&&owned.empty(),"Model OOM published broken cache/owner");empty();
 start();driver.zero_name=true;thrown=false;try{upload(assets,"tiny.tga",names,owned);}catch(const std::exception&){thrown=true;}ck(thrown&&names.empty()&&owned.empty(),"Model zero name published");empty();
 start();fail_cache=true;thrown=false;try{upload(assets,"tiny.tga",names,owned);}catch(const std::exception&){thrown=true;}ck(thrown&&!fail_cache&&driver.creates==0&&names.empty(),"Model cache failure occurred after GL/published broken owner");empty();
 start(120);const auto prior_decodes=decoded_allocations;thrown=false;try{upload(assets,"tiny.tga",names,owned);}catch(const std::exception&){thrown=true;}ck(thrown&&decoded_allocations==prior_decodes&&driver.creates==0,"Model CPU quota refusal allocated decode/GL");empty();
 start(4096,63);thrown=false;try{upload(assets,"tiny.tga",names,owned);}catch(const std::exception&){thrown=true;}ck(thrown&&decoded_allocations==0&&driver.creates==0,"Model GPU refusal allocated decode/GL");empty();
 start();inject_context_loss(unrelated,unrelated_name,actor_bindings);thrown=false;try{upload(assets,"tiny.tga",names,owned);}catch(const std::exception&){thrown=true;}
 ck(thrown&&names.empty()&&owned.empty()&&model_texture_charges_v40.empty()&&driver.textures.count(unrelated_name)&&ledger->snapshot().live.cpu_bytes==0&&ledger->snapshot().live.gpu_bytes==1,"Model context-loss cleanup deleted unrelated recycled name");clear_unrelated(unrelated,unrelated_name);
 start();const auto equipped=upload(assets,"tiny.tga",names,owned,&cache,ResourceScopeV37::equipment);ck(driver.textures.count(equipped)&&ledger->snapshot().live_by_scope[std::size_t(ResourceScopeV37::equipment)].gpu_bytes==64,"Actual original-cache equipped texture scope wrong");release(draws,owned);names.clear();empty();
 start();const auto stored=upload(assets,"stored.tga",names,owned,&cache,ResourceScopeV37::loot);ck(driver.textures.count(stored)&&zip_admissions==1&&encoded_allocations==1,"Actual stored ZIP entry bypassed pre-payload admission");release(draws,owned);names.clear();empty();
 // Same reset composition as source: explicit lost-context loot cleanup first,
 // followed by remaining generic world/actor registry discard.
 start();upload(assets,"tiny.tga",names,owned);std::map<std::string,GLuint> loot_names;std::vector<GLuint> loot_owned;
 upload(assets,"",loot_names,loot_owned,nullptr,ResourceScopeV37::loot);
 ck(ledger->snapshot().live.gpu_bytes==68&&model_texture_charges_v40.size()==2,"Two real generic texture owners not accounted");
 ledger->context_lost();driver.textures.clear();const auto deletes=driver.deletes;
 for(auto name:loot_owned)release_model_texture_v40(name,true);
 loot_owned.clear();loot_names.clear();discard_model_textures_v40();owned.clear();names.clear();
 ck(driver.deletes==deletes,"Lost-context registry composition issued GL deletion");empty();
 track_pixels=false;
 std::cout<<"{\"status\":\"PASS\",\"checks\":"<<checks<<",\"actual_decoder_sampler_mip_unbind\":true,\"actual_ZIP_before_payload\":true,\"fake_GL_only\":true,\"final_records\":"<<ledger->snapshot().occupied_records<<"}\n";
 return 0;
}catch(const std::exception& e){track_pixels=false;std::cerr<<e.what()<<'\n';return 1;}}
