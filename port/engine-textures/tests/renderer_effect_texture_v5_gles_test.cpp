#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <android/log.h>
#include <array>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <exception>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "blood_texture_image_v1.hpp"
#include "texture_driver_fields_v1.hpp"
#include "texture_mipmap_v1.hpp"
#include "texture_unbind_v1.hpp"
#include "original_cache_assets_v1.hpp"
#include "../../scene-materials/particle_scene_v1.hpp"
#include "../../../.local-inputs/fire_atlas_native_fixture_v5.inc"
#include "../../../.local-inputs/fire_bdae_native_fixture_v5.inc"
// ONLY cache transport is a fixture boundary. Bytes are the pinned actual
// packaged atlas, not generated pixels, and every texture/driver call is GLES.
bool dh2::android_ui::OriginalCacheAssetsV1::read(const std::string& name,bool& found,std::vector<std::uint8_t>& bytes,std::string& error)const{
 found=name=="data/3d/textures/FX_smoke_03.tga";
 if(!found){error="Actual packaged atlas fixture has no requested member";return false;}
 bytes.assign(std::begin(actual_fire_atlas),std::end(actual_fire_atlas));return true;
}
void check(const char* where){const auto e=glGetError();if(e)throw std::runtime_error(std::string(where)+": GLerror"+std::to_string(e));}
struct AdapterV5 {
 struct EffectGpuTextureV4{GLuint texture{};dh2::textures::BloodTextureImageOwnerV1 image;dh2::textures::TextureParameterOwnerV1& owner(){return image.parameters();}};
 std::map<std::string,std::shared_ptr<EffectGpuTextureV4>> effect_gpu_textures_v4;
 dh2::textures::TextureDriverOptionsOwnerV1 effect_driver_options_v4;
 #include "renderer_effect_texture_v5.inc"
};
struct Context {
 EGLDisplay display{EGL_NO_DISPLAY};EGLSurface surface{EGL_NO_SURFACE};EGLContext context{EGL_NO_CONTEXT};
 Context(){display=eglGetDisplay(EGL_DEFAULT_DISPLAY);if(display==EGL_NO_DISPLAY||!eglInitialize(display,nullptr,nullptr))throw std::runtime_error("Actual EGL display unavailable");
  const EGLint attrs[]{EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_ALPHA_SIZE,8,EGL_NONE};EGLConfig config{};EGLint count=0;
  if(!eglChooseConfig(display,attrs,&config,1,&count)||count!=1)throw std::runtime_error("Actual EGL RGBA8 pbuffer unavailable");
  const EGLint dimensions[]{EGL_WIDTH,512,EGL_HEIGHT,512,EGL_NONE},version[]{EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};
  surface=eglCreatePbufferSurface(display,config,dimensions);context=eglCreateContext(display,config,EGL_NO_CONTEXT,version);
  if(surface==EGL_NO_SURFACE||context==EGL_NO_CONTEXT||!eglMakeCurrent(display,surface,surface,context))throw std::runtime_error("Actual GLES2 context unavailable");
 }
 ~Context(){eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);eglDestroyContext(display,context);eglDestroySurface(display,surface);eglTerminate(display);}
};
GLuint shader(GLenum kind,const char* source){const auto value=glCreateShader(kind);glShaderSource(value,1,&source,nullptr);glCompileShader(value);GLint ready=0;glGetShaderiv(value,GL_COMPILE_STATUS,&ready);if(!ready)throw std::runtime_error("Actual sampler test shader failed");return value;}
unsigned draw_actual_atlas(GLuint texture,unsigned side,unsigned base_side){
 const char* vs="attribute vec2 p; varying vec2 uv; void main(){uv=(p+1.0)*0.5;gl_Position=vec4(p,0.0,1.0);}";
 const char* fs="precision highp float; varying vec2 uv; uniform sampler2D tex; void main(){gl_FragColor=texture2D(tex,uv);}";
 const auto v=shader(GL_VERTEX_SHADER,vs),f=shader(GL_FRAGMENT_SHADER,fs),program=glCreateProgram();glAttachShader(program,v);glAttachShader(program,f);glBindAttribLocation(program,0,"p");glLinkProgram(program);GLint ready=0;glGetProgramiv(program,GL_LINK_STATUS,&ready);if(!ready)throw std::runtime_error("Actual sampler test link failed");
 glUseProgram(program);glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_2D,texture);glUniform1i(glGetUniformLocation(program,"tex"),0);
 constexpr GLfloat quad[]{-1,-1,1,-1,-1,1,1,1};glBindBuffer(GL_ARRAY_BUFFER,0);glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,quad);glEnableVertexAttribArray(0);glViewport(0,0,side,side);glDisable(GL_BLEND);glDisable(GL_DEPTH_TEST);glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT);glDrawArrays(GL_TRIANGLE_STRIP,0,4);
 std::vector<std::uint8_t> pixels(side*side*4);glReadPixels(0,0,side,side,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());check("Actual atlas GLES draw/readback");
 unsigned nonzero=0,differing=0;for(std::size_t i=0;i<pixels.size();i+=4){if(pixels[i]||pixels[i+1]||pixels[i+2])++nonzero;if(std::memcmp(pixels.data(),pixels.data()+i,4))++differing;}
 if(nonzero<side||differing<side)throw std::runtime_error("Actual atlas draw is blank or constant");
 // Base-resolution readback must match actual source decode, within GLES
 // interpolation/rounding error. Compressed GPU implementations may differ
 // by rounding; this is not a synthetic white/colored fixture acceptance.
 if(side==base_side){dh2::textures::View image{};
  if(dh2_texture_open(actual_fire_atlas,sizeof(actual_fire_atlas),&image)!=dh2::textures::Error::ok)throw std::runtime_error("Actual FIRE atlas decode header");
  std::vector<std::uint8_t> decoded(image.width*image.height*4);
  if(dh2_texture_decode(&image,decoded.data(),decoded.size())!=dh2::textures::Error::ok)throw std::runtime_error("Actual FIRE atlas PVRTC decode");
  unsigned mismatch=0;for(std::size_t i=0;i<pixels.size();++i)if(std::abs(int(pixels[i])-int(decoded[i]))>3)++mismatch;
  if(mismatch>pixels.size()/100)throw std::runtime_error("Actual atlas readback differs from source PVRTC decode");
 }
 glDisableVertexAttribArray(0);glUseProgram(0);glDeleteProgram(program);glDeleteShader(v);glDeleteShader(f);return differing;
}
int main(){try{unsigned checks=0;AdapterV5 adapter;dh2::android_ui::OriginalCacheAssetsV1 cache;
 dh2::resources::BresView bres{};dh2::scene::Scene scene;std::string parsed_error;
 if(dh2_bres_open(&bres,actual_fire_bdae,sizeof(actual_fire_bdae))!=dh2::resources::BresError::ok||!dh2::scene::load_particle_scene_v1(bres,scene,parsed_error))throw std::runtime_error("Actual FIRE material parse: "+parsed_error);
 if(scene.materials.empty())throw std::runtime_error("Actual FIRE material missing");
 const auto material_name=scene.materials.front().diffuse;if(material_name!="FX_smoke_03.tga")throw std::runtime_error("Actual FIRE material texture changed: "+material_name);
 for(unsigned generation=0;generation<3;++generation){Context context;const auto* renderer=glGetString(GL_RENDERER);std::printf("GLES context %u renderer %s\n",generation,renderer?reinterpret_cast<const char*>(renderer):"NULL");
  GLint units=0;glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&units);units=std::min(units,8);assert(units>0);
  std::array<GLuint,8> sentinels{};glGenTextures(units,sentinels.data());
  for(GLint i=0;i<units;++i){glActiveTexture(GL_TEXTURE0+i);glBindTexture(GL_TEXTURE_2D,sentinels[i]);}
  glActiveTexture(GL_TEXTURE0+units-1);glPixelStorei(GL_UNPACK_ALIGNMENT,8);
  for(unsigned iteration=0;iteration<3;++iteration){
   auto texture=adapter.effect_texture_v4(material_name,cache);assert(texture->texture&&texture->owner().ready());assert(!adapter.effect_texture_grid_v5.texture_changes84());++checks;
   GLint current=0;glGetIntegerv(GL_ACTIVE_TEXTURE,&current);assert(current==GL_TEXTURE0+units-1);glGetIntegerv(GL_UNPACK_ALIGNMENT,&current);assert(current==8);
   for(GLint i=0;i<units;++i){glActiveTexture(GL_TEXTURE0+i);glGetIntegerv(GL_TEXTURE_BINDING_2D,&current);assert(GLuint(current)==sentinels[i]);assert(!adapter.effect_texture_grid_v5.slot(0,i)->identity);}++checks;
   glActiveTexture(GL_TEXTURE0+units-1);
   {AdapterV5::EffectTextureStateGuardV5 restore(adapter.effect_texture_grid_v5,adapter.effect_texture_cache_v5,adapter.effect_texture_units_v5);
    const auto width=texture->image.descriptor().width;assert(width==texture->image.descriptor().height&&width<=512);
    const auto full=draw_actual_atlas(texture->texture,width,width),small=draw_actual_atlas(texture->texture,width/4,width);std::printf("Actual FIRE atlas draw %u/%u varied=%u mipdraw=%u format=%u size=%u\n",generation,iteration,full,small,texture->image.descriptor().format,width);checks+=2;}
   glGetIntegerv(GL_ACTIVE_TEXTURE,&current);assert(current==GL_TEXTURE0+units-1);glGetIntegerv(GL_UNPACK_ALIGNMENT,&current);assert(current==8);
   for(GLint i=0;i<units;++i){glActiveTexture(GL_TEXTURE0+i);glGetIntegerv(GL_TEXTURE_BINDING_2D,&current);assert(GLuint(current)==sentinels[i]);assert(!adapter.effect_texture_grid_v5.slot(0,i)->identity);}
   glActiveTexture(GL_TEXTURE0+units-1);++checks;
   std::string error;if(!adapter.release_effect_texture_v5(*texture,false,false,error))throw std::runtime_error(error);assert(!texture->texture&&!texture->owner().ready());adapter.effect_gpu_textures_v4.clear();++checks;check("Actual source texture cleanup");
  }
  auto lost=adapter.effect_texture_v4(material_name,cache);assert(lost->owner().ready());
  eglMakeCurrent(context.display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);std::string error;
  if(!adapter.release_effect_texture_v5(*lost,true,false,error))throw std::runtime_error(error);assert(!lost->owner().ready()&&!lost->texture);adapter.effect_gpu_textures_v4.clear();++checks;
 }
 std::printf("PASS %u actual packaged atlas GLES V5 lifecycle checks\n",checks);return 0;
 }catch(const std::exception& e){std::fprintf(stderr,"FAIL %s\n",e.what());return 1;}}
