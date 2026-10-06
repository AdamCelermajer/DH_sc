#include "../effect_render_pass_v4.hpp"
#include "../shader_sources.hpp"
#include "../../scene-materials/scene.hpp"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include "../../android-native/app/src/main/cpp/renderer_authored_effect_draw_v5.inc"
#include "../../android-native/app/src/main/cpp/renderer_authored_effect_program_connection_v5.inc"
static unsigned checks;
static void check(bool b,const std::string& e){++checks;if(!b)throw std::runtime_error(e);}
static std::vector<std::uint8_t> bytes(const char* p){std::ifstream f(p,std::ios::binary);check(bool(f),p);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{check(argc==4,"shader pack/BashDown/Charge required");const auto shaders=bytes(argv[1]);
 EGLDisplay d=eglGetDisplay(EGL_DEFAULT_DISPLAY);check(eglInitialize(d,nullptr,nullptr),"EGL initialize");
 const EGLint choose[]{EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_ALPHA_SIZE,8,EGL_DEPTH_SIZE,24,EGL_NONE};EGLConfig cfg;EGLint n;
 check(eglChooseConfig(d,choose,&cfg,1,&n)&&n==1,"actual GLES config");const EGLint dim[]{EGL_WIDTH,32,EGL_HEIGHT,32,EGL_NONE},ver[]{EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};
 for(int cycle=0;cycle<2;++cycle){auto surface=eglCreatePbufferSurface(d,cfg,dim);auto ctx=eglCreateContext(d,cfg,EGL_NO_CONTEXT,ver);check(eglMakeCurrent(d,surface,surface,ctx),"same current fixture context");
  const float v[]{-1,-1,0,0,0,0,0,0,0,1,-1,0,0,0,0,0,1,0,1,1,0,0,0,0,0,1,1,-1,1,0,0,0,0,0,0,1};const std::uint16_t ids[]{0,1,2,0,2,3};const std::uint8_t white[]{255,255,255,255};GLuint vb,ib,tex;
  glGenBuffers(1,&vb);glBindBuffer(GL_ARRAY_BUFFER,vb);glBufferData(GL_ARRAY_BUFFER,sizeof(v),v,GL_STATIC_DRAW);glGenBuffers(1,&ib);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ib);glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(ids),ids,GL_STATIC_DRAW);glGenTextures(1,&tex);glBindTexture(GL_TEXTURE_2D,tex);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,white);
  const float identity[]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},color[]{.8f,.4f,.1f,1};
  for(int a=2;a<4;++a){const auto raw=bytes(argv[a]);dh2::resources::BresView image{};check(dh2_bres_open(&image,raw.data(),raw.size())==dh2::resources::BresError::ok,"actual BDAE");dh2::scene::Scene scene;std::string error;check(dh2::scene::load(image,scene,error),error);
   for(const auto& material:scene.materials){EffectProgramConnectionV5 program;const std::string config;program.initialize(image,material.id.c_str(),shaders,0x10,&config);
    dh2::scene::EffectRenderPassV4 pass;check(dh2::scene::effect_render_pass_v4(image,material.id.c_str(),"default",pass,error),error);std::uint32_t flags;std::memcpy(&flags,pass.pass.data()+4,4);check(flags&0x10000,"actual supported transparent mesh pass");
    EffectDrawBorrowV5 draw{};draw.vertices=vb;draw.indices=ib;draw.texture=tex;draw.count=6;draw.stride=9*sizeof(float);draw.color_offset=3*sizeof(float);draw.uv_offset=7*sizeof(float);draw.world_view_projection=identity;draw.diffuse_color=color;draw.source_texture_ready=true;
    for(bool missing:{false,true}){glViewport(0,0,32,32);glDepthMask(GL_TRUE);glClearDepthf(1);glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);glVertexAttrib4f(1,.2f,.3f,.4f,.5f);glDisableVertexAttribArray(1);draw.source_color_missing=missing;program.draw({draw},false,true,0x10,&config);glFinish();check(glGetError()==GL_NO_ERROR,"actual shader draw");std::array<GLfloat,4> restored;glGetVertexAttribfv(1,GL_CURRENT_VERTEX_ATTRIB,restored.data());check(restored==std::array<float,4>{.2f,.3f,.4f,.5f},"source constant attribute restored");check(!glIsEnabled(GL_BLEND)&&!glIsEnabled(GL_DEPTH_TEST),"borrowed GL state restored");std::array<std::uint8_t,4> pixel;glReadPixels(16,16,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel.data());check(glGetError()==GL_NO_ERROR,"read actual framebuffer");check(missing?pixel[0]>=200&&pixel[1]>=99&&pixel[2]>=23:pixel[0]==0&&pixel[1]==0&&pixel[2]==0,"missing Color0 source white vs present black stream");}
    program.release();
   }
  }
  glDeleteBuffers(1,&vb);glDeleteBuffers(1,&ib);glDeleteTextures(1,&tex);eglMakeCurrent(d,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);eglDestroyContext(d,ctx);eglDestroySurface(d,surface);
 }eglTerminate(d);std::cout<<"{\"validation\":\"PASS\",\"checks\":"<<checks<<",\"geometry_texture_fixture\":true,\"live_submission\":false}"<<std::endl;return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
