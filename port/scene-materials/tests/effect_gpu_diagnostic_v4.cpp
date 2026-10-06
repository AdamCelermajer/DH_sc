#include "../effect_render_pass_v4.hpp"
#include "../shader_sources.hpp"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <array>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <iterator>
#include <cassert>
#include "C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/effect-gpu-diagnostic-v4/draw.inc"
#include "../../android-native/app/src/main/cpp/renderer_effect_program_connection_v4.inc"
std::vector<std::uint8_t> bytes(const char* path){std::ifstream f(path,std::ios::binary);assert(f);return {std::istreambuf_iterator<char>(f),{}};}
int main(int argc,char** argv){try{
 assert(argc==5);EGLDisplay display=eglGetDisplay(EGL_DEFAULT_DISPLAY);assert(display!=EGL_NO_DISPLAY);assert(eglInitialize(display,nullptr,nullptr));
 const EGLint choice[]{EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_ALPHA_SIZE,8,EGL_NONE};
 EGLConfig config;EGLint count=0;assert(eglChooseConfig(display,choice,&config,1,&count)&&count==1);
 const EGLint dimensions[]{EGL_WIDTH,32,EGL_HEIGHT,32,EGL_NONE},version[]{EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};
 EGLSurface surface=eglCreatePbufferSurface(display,config,dimensions);EGLContext context=eglCreateContext(display,config,EGL_NO_CONTEXT,version);assert(surface!=EGL_NO_SURFACE&&context!=EGL_NO_CONTEXT&&eglMakeCurrent(display,surface,surface,context));
 const auto shaders=bytes(argv[1]);
 // Declared GPU presentation fixture: actual shader/material pass, a retained
 // fullscreen quad, one real texture and genuine GL state. Authored particle
 // simulation and the source texture owner have independent original tests.
 const float vertices[]{-1,-1,0,1,1,1,1,0,0, 1,-1,0,1,1,1,1,1,0, 1,1,0,1,1,1,1,1,1, -1,1,0,1,1,1,1,0,1};
 const std::uint16_t indices[]{0,2,1,0,3,2};const std::uint8_t white[]{255,255,255,255};
 GLuint vbo=0,ibo=0,texture=0;glGenBuffers(1,&vbo);glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);glGenBuffers(1,&ibo);glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);glGenTextures(1,&texture);glBindTexture(GL_TEXTURE_2D,texture);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,GL_UNSIGNED_BYTE,white);
 const float matrix[]{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},color[]{.8f,.4f,.1f,1};
 for(int i=2;i<5;++i){const auto resource=bytes(argv[i]);dh2::resources::BresView image{};assert(dh2_bres_open(&image,resource.data(),resource.size())==dh2::resources::BresError::ok);
  const char* material=i==2?"_1_-_Defaultjh":"fx_particles_alpha";
  EffectProgramConnectionV4 program;const std::string actual_empty_config;
  program.initialize(image,material,shaders,0x10,&actual_empty_config);
  EffectDrawBorrowV4 part;part.vertices=vbo;part.indices=ibo;part.texture=texture;part.count=6;part.stride=9*sizeof(float);part.color_offset=3*sizeof(float);part.uv_offset=7*sizeof(float);part.world_view_projection=matrix;part.diffuse_color=color;part.source_texture_ready=true;
  for(int mode=0;mode<6;++mode)for(int winding=0;winding<2;++winding){effect_gpu_diagnostic_mode_v4=mode;
  const std::uint16_t ccw[]{0,1,2,0,2,3};glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibo);glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),winding?ccw:indices,GL_STATIC_DRAW);
  std::cerr<<"FIXTURE mode "<<mode<<" winding "<<winding<<'\n';
  glViewport(0,0,32,32);glDisable(GL_BLEND);glDisable(GL_DEPTH_TEST);glDepthMask(GL_TRUE);glClearColor(0,0,0,0);glClear(GL_COLOR_BUFFER_BIT);if(mode==5){glClearDepthf(1);glClear(GL_DEPTH_BUFFER_BIT);}
  glActiveTexture(GL_TEXTURE1);
  program.draw({part},false,true,0x10,&actual_empty_config);glFinish();assert(glGetError()==GL_NO_ERROR);
  GLint active=0;glGetIntegerv(GL_ACTIVE_TEXTURE,&active);assert(active==GL_TEXTURE1);assert(glIsEnabled(GL_BLEND)==GL_FALSE&&glIsEnabled(GL_DEPTH_TEST)==GL_FALSE);
  std::array<std::uint8_t,4> pixel{};glReadPixels(16,16,1,1,GL_RGBA,GL_UNSIGNED_BYTE,pixel.data());assert(glGetError()==GL_NO_ERROR);
  std::cerr<<"GPU pixel "<<material<<' '<<unsigned(pixel[0])<<' '<<unsigned(pixel[1])<<' '<<unsigned(pixel[2])<<' '<<unsigned(pixel[3])<<'\n';
  std::vector<std::uint8_t> frame(32*32*4);glReadPixels(0,0,32,32,GL_RGBA,GL_UNSIGNED_BYTE,frame.data());unsigned visible=0;for(std::size_t q=0;q<frame.size();q+=4)visible+=frame[q]||frame[q+1]||frame[q+2]||frame[q+3];std::cerr<<"MODE "<<mode<<" nonzero pixels "<<visible<<'\n';
  }
  bool missing=false;try{program.draw({part},false,false,0x10,&actual_empty_config);}catch(const std::runtime_error&){missing=true;}assert(missing);
  std::cout<<"actual effect "<<material<<" pixels "<<"diagnostic complete"<<" state restored\n";program.release();
 }
 glDeleteBuffers(1,&vbo);glDeleteBuffers(1,&ibo);glDeleteTextures(1,&texture);eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);eglDestroyContext(display,context);eglDestroySurface(display,surface);eglTerminate(display);
 std::cout<<"DIAGNOSTIC complete actual FIRE/blood shader compilation and controlled state modes; evaluate pixel results separately\n";
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
