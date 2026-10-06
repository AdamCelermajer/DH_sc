// Reuse actual pinned BRES/atlas transport and EGL/texture adapter, without
// running or repeating the independent 57-check texture regression.
#define main texture_fixture_main
#include "../../engine-textures/tests/renderer_effect_texture_v5_gles_test.cpp"
#undef main
#include "../effect_render_pass_v4.hpp"
#include "../shader_sources.hpp"
#include "../shader_program_collection_v4.hpp"
#include "../effect_material_directory_v4.hpp"
#include "../material_matrix_v4.hpp"
#include "../transparent_sort_v2.hpp"
#include "../../android-native/app/src/main/cpp/renderer_effect_draw_v4.inc"
#include "../../android-native/app/src/main/cpp/renderer_effect_program_connection_v4.inc"
#include "../../android-native/app/src/main/cpp/renderer_effect_material_directory_v4.inc"
#include <fstream>
#include <iostream>
using namespace dh2;
static unsigned checks;
static void require(bool value,const std::string& text){++checks;if(!value)throw std::runtime_error(text);}
static std::uint32_t word(const resources::BresView& b,std::uint32_t p){require(p<=b.size&&b.size-p>=4,"BRES range");std::uint32_t v;std::memcpy(&v,b.bytes+p,4);return v;}
static math::Matrix4f actual_matrix(const resources::BresView& b){for(unsigned i=0;i<dh2_bres_library_count(&b,resources::Library::effect);++i){auto r=dh2_bres_library_item(&b,resources::Library::effect,i);auto at=std::uint32_t(r-b.bytes);for(unsigned j=0;j<word(b,at+16);++j){auto p=word(b,at+20)+24*j;const auto name=word(b,p);require(name<b.size,"BRES name");if(std::strcmp(reinterpret_cast<const char*>(b.bytes+name),"diffuse-sampler-matrix"))continue;require(word(b,p+4)==10&&word(b,p+12)==1,"actual matrix type");float values[16];auto base=word(b,p+20);for(unsigned k=0;k<16;++k){auto v=word(b,base+4*k);std::memcpy(values+k,&v,4);}math::Matrix4f matrix;scene::material_matrix_from_bres_v4(matrix,values);return matrix;}}throw std::runtime_error("actual FIRE matrix absent");}
struct MaterialFixture {
 std::vector<std::uint8_t> bytes{std::begin(actual_fire_bdae),std::end(actual_fire_bdae)};
 resources::BresView bres{};scene::EffectRenderPassV4 pass;
 math::Matrix4f matrix{};scene::EffectMaterialDirectoryV4 directory;
 // Declared current-value mutation fixture. Actual authored color sampling
 // has separate gold/real-resource tests; this tests uniform isolation.
 float color[4]{.8f,.4f,.1f,1};
 MaterialFixture(){std::string e;require(dh2_bres_open(&bres,bytes.data(),bytes.size())==resources::BresError::ok,"actual retained FIRE");require(scene::effect_render_pass_v4(bres,"_1_-_Defaultjh","default",pass,e),e);matrix=actual_matrix(bres);}
};
int main(int argc,char** argv){try{
 require(argc==2,"actual shaders.pak path");std::ifstream file(argv[1],std::ios::binary);require(bool(file),"shader file");std::vector<std::uint8_t> shaders{std::istreambuf_iterator<char>(file),{}};
 for(unsigned cycle=0;cycle<3;++cycle){Context context;AdapterV5 texture_adapter;android_ui::OriginalCacheAssetsV1 cache;
  auto texture=texture_adapter.effect_texture_v4("FX_smoke_03.tga",cache);require(texture->owner().ready(),"actual atlas ready");
  MaterialFixture a,b;require(a.bres.bytes!=b.bres.bytes,"distinct retained BRES receivers");
  scene::ShaderProgramCollectionV4 collection;std::shared_ptr<scene::ShaderProgramRecordV4> pa,pb;std::vector<std::shared_ptr<EffectProgramConnectionV4>> programs;unsigned creates=0;std::string e,config;
  auto create=[&](void*,std::uint16_t,const std::string&,std::shared_ptr<void>& owner,std::vector<scene::ShaderUniformReflectionV4>& reflected,std::string& why){auto p=std::make_shared<EffectProgramConnectionV4>();p->initialize(a.bres,"_1_-_Defaultjh",shaders,0x10,&config);if(!reflect_effect_program_v4(p->native_program(),reflected,why)){p->release();return false;}++creates;programs.push_back(p);owner=p;return true;};
  // Typed C callback transport carries the actual GL creation closure.
  auto thunk=[](void* p,std::uint16_t id,const std::string& key,std::shared_ptr<void>& owner,std::vector<scene::ShaderUniformReflectionV4>& r,std::string& why){return (*static_cast<decltype(create)*>(p))(nullptr,id,key,owner,r,why);};
  require(collection.get_or_create(scene::effect_program_cache_name_v4(a.pass),&create,thunk,pa,e),e);
  require(collection.get_or_create(scene::effect_program_cache_name_v4(b.pass),&create,thunk,pb,e),e);
  require(creates==1&&pa==pb&&pa->identity()==pb->identity()&&pa->collection_id==0,"same source key owns one linked program");
  bool color=false,sampler=false,matrix=false,global=false;for(unsigned i=0;i<pa->parameters.size();++i){const auto& u=pa->parameters[i];require(u.count==1&&u.location>=0,"actual reflected count/location");if(u.semantic==6){require(u.type==8,"diffuse float4");color=true;}if(u.semantic==2){require(u.type==12&&u.sub_id==0,"sampler normalized index");sampler=true;}if(u.semantic==3){require(u.type==11,"matrix4 reflection");matrix=true;}if(u.semantic==39){require(i<pa->global_count,"WVP global prefix");global=true;}}
  require(color&&sampler&&matrix&&global,"actual ProfileCOMMON reflection directory");
  auto refresh=[&](MaterialFixture& m){float bias=texture->owner().lod_bias();EffectMaterialProfileContextV4 values;values.borrowed={m.color,&m.matrix,reinterpret_cast<std::uintptr_t>(texture.get()),&bias};require(m.directory.refresh(pa,m.pass,&values,EffectMaterialProfileContextV4::read,e),e);};
  refresh(a);refresh(b);bool equal=false;require(scene::material_equal_v4(equal,*a.directory.view(),*b.directory.view(),e)==0&&equal,"equal actual program/current values");
  b.color[0]=.15f;b.color[1]=.7f;refresh(b);require(scene::material_equal_v4(equal,*a.directory.view(),*b.directory.view(),e)==0&&!equal,"different material values on shared program");
  // Missing value must fail atomically and leave previous immutable snapshot.
  auto previous=b.directory.view();EffectMaterialProfileContextV4 missing;require(!b.directory.refresh(pb,b.pass,&missing,EffectMaterialProfileContextV4::read,e)&&b.directory.view()==previous,"missing producer never publishes partial directory");
  scene::TransparentQueueServicesV1 services;unsigned eq_calls=0,less_calls=0;
  auto view=[&](std::uintptr_t p){return p==reinterpret_cast<std::uintptr_t>(&a)?a.directory.view():b.directory.view();};
  services.material_equal_3537b0=[&](auto x,auto y,bool& v,std::string& why){++eq_calls;return scene::material_equal_v4(v,*view(x),*view(y),why)==0;};
  services.material_less_3537e4=[&](auto x,auto y,bool& v,std::string& why){++less_calls;return scene::material_less_v4(v,*view(x),*view(y),why)==0;};
  services.node_suborder_20=[](auto,auto,std::int32_t& v,std::string&){v=0;return true;};
  std::vector<scene::TransparentEntryV1> queue;for(unsigned i=0;i<8;++i){scene::TransparentEntryV1 entry;entry.node=i+1;entry.material=reinterpret_cast<std::uintptr_t>(i%2?&a:&b);entry.distance=7;entry.priority=0;queue.push_back(entry);}require(scene::transparent_sort_v2(queue,services,e),e);require(eq_calls&&less_calls,"equal-distance source queue reaches material compare");for(unsigned i=1;i<queue.size();++i){bool backwards=false;require(scene::transparent_less_v1(queue[i],queue[i-1],services,backwards,e)&&!backwards,"source queue material order");}
  const float vertices[]{-1,-1,0,1,1,1,1,0,0,1,-1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,-1,1,0,1,1,1,1,0,1};const std::uint16_t indices[]{0,1,2,0,2,3};GLuint vbo{},ibo{};glGenBuffers(1,&vbo);glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);glGenBuffers(1,&ibo);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
  float wvp[]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};EffectDrawBorrowV4 draw;draw.vertices=vbo;draw.indices=ibo;draw.texture=texture->texture;draw.count=6;draw.stride=9*sizeof(float);draw.color_offset=3*sizeof(float);draw.uv_offset=7*sizeof(float);draw.world_view_projection=wvp;draw.source_texture_ready=true;draw.sampler_bias=texture->owner().lod_bias();
  auto program=std::static_pointer_cast<EffectProgramConnectionV4>(pa->same_program);
  auto render=[&](MaterialFixture& material){glViewport(0,0,512,512);glDepthMask(GL_TRUE);glClearDepthf(1);glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);draw.diffuse_color=material.color;program->draw_pass(material.pass,{draw},false,true,0x10,&config);std::vector<std::uint8_t> pixels(512*512*4);glReadPixels(0,0,512,512,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());check("actual shared FIRE draw");unsigned nonzero=0;for(unsigned i=0;i<pixels.size();i+=4)nonzero+=pixels[i]||pixels[i+1]||pixels[i+2];require(nonzero>512,"actual FIRE atlas draw nonblank");return pixels;};
  auto pixels_a=render(a),pixels_b=render(b);require(pixels_a!=pixels_b,"shared program draws isolated current material colors");require(render(a)==pixels_a,"drawing B never contaminates A uniforms");
  const auto id=program->native_program();pb.reset();require(glIsProgram(id)==GL_TRUE,"dropping one material preserves shared GL program");
  glDeleteBuffers(1,&vbo);glDeleteBuffers(1,&ibo);require(texture_adapter.release_effect_texture_v5(*texture,false,false,e),e);texture_adapter.effect_gpu_textures_v4.clear();
  if(cycle==2){eglMakeCurrent(context.display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);collection={};programs.clear();pa.reset();program.reset();}else{program->release();require(glIsProgram(id)==GL_FALSE,"one owning live-context release deletes shared program");collection={};programs.clear();pa.reset();program.reset();check("live context cleanup");}
  std::cout<<"context "<<cycle<<" shared=1 uniforms="<<(color+sampler+matrix+global)<<" queue comparisons="<<eq_calls<<'/'<<less_calls<<" actual atlas pixels PASS\n";
 }
 std::cout<<"PASS "<<checks<<" real GLES shared material/program checks; declared quad/current-color inputs, not whole gameFX acceptance\n";return 0;
 }catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
