#include "general_fx_texture_image_v2.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
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
static unsigned state_queries_v34;
static void state_int_v34(GLenum p,GLint* v){++state_queries_v34;glGetIntegerv(p,v);}
static void state_bool_v34(GLenum p,GLboolean* v){++state_queries_v34;glGetBooleanv(p,v);}
static GLboolean state_enabled_v34(GLenum p){++state_queries_v34;return glIsEnabled(p);}
static void state_attr_int_v34(GLuint i,GLenum p,GLint* v){++state_queries_v34;glGetVertexAttribiv(i,p,v);}
static void state_attr_float_v34(GLuint i,GLenum p,GLfloat* v){++state_queries_v34;glGetVertexAttribfv(i,p,v);}
static void state_attr_pointer_v34(GLuint i,GLenum p,void** v){++state_queries_v34;glGetVertexAttribPointerv(i,p,v);}
#define glGetIntegerv state_int_v34
#define glGetBooleanv state_bool_v34
#define glIsEnabled state_enabled_v34
#define glGetVertexAttribiv state_attr_int_v34
#define glGetVertexAttribfv state_attr_float_v34
#define glGetVertexAttribPointerv state_attr_pointer_v34
#include "../../android-native/app/src/main/cpp/renderer_authored_effect_draw_v5.inc"
#undef glGetIntegerv
#undef glGetBooleanv
#undef glIsEnabled
#undef glGetVertexAttribiv
#undef glGetVertexAttribfv
#undef glGetVertexAttribPointerv
#include "../../android-native/app/src/main/cpp/renderer_authored_effect_program_connection_v5.inc"
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

#pragma GCC diagnostic pop

struct ExternalStateV34 {
 std::array<GLint,14> words{};std::array<GLboolean,6> caps{};GLboolean depth{};std::array<GLfloat,4> color{};
 static ExternalStateV34 capture(){ExternalStateV34 s;const GLenum names[]{GL_CURRENT_PROGRAM,GL_ACTIVE_TEXTURE,GL_TEXTURE_BINDING_2D,GL_ARRAY_BUFFER_BINDING,GL_ELEMENT_ARRAY_BUFFER_BINDING,GL_BLEND_SRC_RGB,GL_BLEND_DST_RGB,GL_BLEND_SRC_ALPHA,GL_BLEND_DST_ALPHA,GL_BLEND_EQUATION_RGB,GL_BLEND_EQUATION_ALPHA,GL_DEPTH_FUNC,GL_CULL_FACE_MODE,GL_FRONT_FACE};
  for(unsigned i=0;i<14;++i)glGetIntegerv(names[i],&s.words[i]);const GLenum caps[]{GL_BLEND,GL_DEPTH_TEST,GL_CULL_FACE,GL_STENCIL_TEST,GL_SAMPLE_COVERAGE,GL_POLYGON_OFFSET_FILL};for(unsigned i=0;i<6;++i)s.caps[i]=glIsEnabled(caps[i]);glGetBooleanv(GL_DEPTH_WRITEMASK,&s.depth);glGetVertexAttribfv(1,GL_CURRENT_VERTEX_ATTRIB,s.color.data());return s;
 }
 bool operator==(const ExternalStateV34& x)const{return words==x.words&&caps==x.caps&&depth==x.depth&&color==x.color;}
};
int main(int argc,char** argv){try{
 require(argc==2,"actual shaders.pak path");std::ifstream file(argv[1],std::ios::binary);require(bool(file),"actual shader file");std::vector<std::uint8_t> shaders{std::istreambuf_iterator<char>(file),{}};
 unsigned old_queries{},batch_queries{};
 for(unsigned cycle=0;cycle<2;++cycle){Context context;AdapterV5 textures;android_ui::OriginalCacheAssetsV1 assets;auto texture=textures.effect_texture_v4("FX_smoke_03.tga",assets);require(texture->owner().ready(),"actual atlas");
  MaterialFixture material;std::string config;EffectProgramConnectionV5 program;program.initialize(material.bres,"_1_-_Defaultjh",shaders,0x10,&config);std::string reflection_error;std::vector<scene::ShaderUniformReflectionV4> reflected;require(reflect_effect_program_v4(program.native_program(),reflected,reflection_error),reflection_error);require(!reflected.empty(),"actual linked reflected uniforms");
  const float vertices[]{-1,-1,0,1,1,1,1,0,0,1,-1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,-1,1,0,1,1,1,1,0,1};const std::uint16_t indices[]{0,1,2,0,2,3};GLuint vbo{},ibo{};glGenBuffers(1,&vbo);glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);glGenBuffers(1,&ibo);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
  float wvp[]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};EffectDrawBorrowV5 draw;draw.vertices=vbo;draw.indices=ibo;draw.texture=texture->texture;draw.count=6;draw.stride=9*sizeof(float);draw.color_offset=3*sizeof(float);draw.uv_offset=7*sizeof(float);draw.world_view_projection=wvp;draw.source_texture_ready=true;draw.sampler_bias=texture->owner().lod_bias();
  auto render=[&](bool batch){glViewport(0,0,512,512);glDepthMask(GL_TRUE);glClearDepthf(1);glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);glDepthMask(GL_FALSE);glDisable(GL_BLEND);glEnable(GL_DEPTH_TEST);glDepthFunc(GL_GEQUAL);glCullFace(GL_FRONT);glFrontFace(GL_CW);glDisable(GL_CULL_FACE);glVertexAttrib4f(1,.2f,.3f,.4f,.5f);glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,0);glUseProgram(0);
   const auto before=ExternalStateV34::capture();state_queries_v34=0;
   auto one=[&](unsigned i,const EffectDrawProgramV5::BatchScope* scope){float color[]{i%2?.2f:.8f,i%2?.7f:.4f,.1f,1.f};draw.diffuse_color=color;draw.source_color_missing=i%3==0;
    if(scope)program.draw_one_in_batch(*scope,material.pass,draw,false,true,0x10,&config);else program.draw_pass(material.pass,{draw},false,true,0x10,&config);
   };
   if(batch){EffectDrawProgramV5::BatchScope scope;for(unsigned i=0;i<8;++i)one(i,&scope);}else for(unsigned i=0;i<8;++i)one(i,nullptr);
   const auto queried=state_queries_v34;if(batch)batch_queries=queried;else old_queries=queried;
   require(ExternalStateV34::capture()==before,"complete batch restores actual external GL state");check("actual effect batch state");std::vector<std::uint8_t> pixels(512*512*4);glReadPixels(0,0,512,512,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());check("actual batch FIRE pixels");unsigned nonzero=0;for(unsigned i=0;i<pixels.size();i+=4)nonzero+=pixels[i]||pixels[i+1]||pixels[i+2];require(nonzero>512,"actual atlas nonblank");return pixels;
  };
  const auto legacy=render(false),batch=render(true);require(legacy==batch,"same source-ordered passes/geometry/texture/colors produce bit-identical pixels");require(old_queries==8*batch_queries&&batch_queries>0,"one actual state snapshot per eight-effect batch");
  // A live geometry mutation still reaches the same persistent GPU handles.
  float moved[sizeof(vertices)/sizeof(float)];std::copy(std::begin(vertices),std::end(vertices),moved);for(unsigned i=0;i<4;++i)moved[i*9]+=0.35f;glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(moved),moved);const auto changed=render(true);require(changed!=batch,"changing live geometry is never frozen by buffer reuse");
  const auto gl_program=program.native_program();glDeleteBuffers(1,&vbo);glDeleteBuffers(1,&ibo);program.release();require(glIsProgram(gl_program)==GL_FALSE,"actual program released in owning context");std::string error;require(textures.release_effect_texture_v5(*texture,false,false,error),error);textures.effect_gpu_textures_v4.clear();check("actual batch cleanup");
 }
 std::cout<<"PASS effect batchV34 checks="<<checks<<" draws=8 old_state_queries="<<old_queries<<" batch_state_queries="<<batch_queries<<" contexts=2; actual FIRE BRES/shader/atlas/GLES, declared geometry/current-color fixtures, no live FPS claim\n";return 0;
 }catch(const std::exception& e){std::cerr<<"FAIL "<<e.what()<<'\n';return 1;}}
