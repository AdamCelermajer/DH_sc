// Isolated GLES2 pixel fixture. Asset transport reads the actual byte/hash
// verified shader pack from disk; production SwfGpu/shader code is unchanged.
#include "swf_gpu.hpp"
#include <EGL/egl.h>
#include <android/asset_manager.h>
#include <array>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>
using dh2::android_ui::SwfGpu;using dh2::ui::SwfDraw;
namespace {std::string shader_path;unsigned checks{};struct File {FILE* handle;long length;};
void check(bool value,const char* message){++checks;if(!value)throw std::runtime_error(message);}
std::array<unsigned char,4> pixel(int x,int y){std::array<unsigned char,4> p{};glReadPixels(x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,p.data());return p;}
void draw(SwfGpu& gpu,SwfDraw command){std::string error;check(gpu.draw(command,error),error.c_str());}
SwfDraw begin(){SwfDraw d;d.kind=SwfDraw::begin;d.bounds[1]=d.bounds[3]=1280;d.viewport[2]=d.viewport[3]=64;return d;}
SwfDraw end(){SwfDraw d;d.kind=SwfDraw::end;return d;}
SwfDraw square(unsigned char r,unsigned char g,unsigned char b,unsigned char a){SwfDraw d;d.kind=SwfDraw::triangle_strip;d.fill.kind=dh2::ui::SwfFill::color;d.fill.rgba[0]=r;d.fill.rgba[1]=g;d.fill.rgba[2]=b;d.fill.rgba[3]=a;d.xy={320,320,960,320,320,960,960,960};return d;}
void background(){glBindFramebuffer(GL_FRAMEBUFFER,0);glViewport(0,0,64,64);glDisable(GL_SCISSOR_TEST);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glClearColor(.2f,.4f,.6f,1);glClear(GL_COLOR_BUFFER_BIT);}
}
extern "C" AAsset* AAssetManager_open(AAssetManager*,const char* name,int){if(std::strcmp(name,"shaders/shaders.pak"))return nullptr;auto* file=std::fopen(shader_path.c_str(),"rb");if(!file)return nullptr;std::fseek(file,0,SEEK_END);const auto n=std::ftell(file);std::rewind(file);return reinterpret_cast<AAsset*>(new File{file,n});}
extern "C" off64_t AAsset_getLength64(AAsset* p){return reinterpret_cast<File*>(p)->length;}
extern "C" int AAsset_read(AAsset* p,void* data,size_t count){return int(std::fread(data,1,count,reinterpret_cast<File*>(p)->handle));}
extern "C" void AAsset_close(AAsset* p){auto* file=reinterpret_cast<File*>(p);std::fclose(file->handle);delete file;}
int main(int argc,char** argv){try{
 check(argc==2,"actual shader pack required");shader_path=argv[1];auto display=eglGetDisplay(EGL_DEFAULT_DISPLAY);check(display!=EGL_NO_DISPLAY,"EGL display");check(eglInitialize(display,nullptr,nullptr),"EGL init");
 const EGLint config_attributes[]{EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_ALPHA_SIZE,8,EGL_STENCIL_SIZE,8,EGL_DEPTH_SIZE,24,EGL_NONE};
 EGLConfig config{};EGLint count{};check(eglChooseConfig(display,config_attributes,&config,1,&count)&&count==1,"EGL config");const EGLint surface_attributes[]{EGL_WIDTH,64,EGL_HEIGHT,64,EGL_NONE},context_attributes[]{EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};
 auto surface=eglCreatePbufferSurface(display,config,surface_attributes);auto context=eglCreateContext(display,config,EGL_NO_CONTEXT,context_attributes);check(surface!=EGL_NO_SURFACE&&context!=EGL_NO_CONTEXT&&eglMakeCurrent(display,surface,surface,context),"isolated GLES context");
 SwfGpu gpu;gpu.initialize(reinterpret_cast<AAssetManager*>(1));background();const auto original=pixel(32,32);const auto initial=gpu.stats_v36();
 for(unsigned i=0;i<100;++i){draw(gpu,begin());draw(gpu,end());}
 auto empty=gpu.stats_v36();check(empty.empty_displays==100&&empty.materializations==initial.materializations,"empty source displays materialized");check(pixel(32,32)==original,"empty display changed pixels");
 draw(gpu,begin());auto invisible=square(255,0,0,0);draw(gpu,invisible);draw(gpu,end());check(pixel(32,32)==original,"alpha-zero primitive changed pixels");check(gpu.stats_v36().materializations==0&&gpu.stats_v37().transparent_noops==1,"alpha-zero normal primitive materialized");
 background();draw(gpu,begin());draw(gpu,square(255,0,0,255));draw(gpu,end());auto red=pixel(32,32);check(red[0]>250&&red[1]<3&&red[2]<3,"actual cached primitive pixels");check(pixel(4,4)==original,"primitive changed outside region");const auto uploads=gpu.stats_v36().cache.uploads;
 for(unsigned i=0;i<10;++i){background();draw(gpu,begin());draw(gpu,square(255,0,0,255));draw(gpu,end());check(pixel(32,32)==red,"cached geometry changed pixels");}
 check(gpu.stats_v36().cache.uploads==uploads&&gpu.stats_v36().cache.hits>=10,"immutable geometry repeatedly uploaded");
 background();draw(gpu,begin());SwfDraw command;command.kind=SwfDraw::mask_begin;draw(gpu,command);auto mask=square(255,255,255,0);draw(gpu,mask);command.kind=SwfDraw::mask_end;draw(gpu,command);auto full=square(0,255,0,255);full.xy={0,0,1280,0,0,1280,1280,1280};draw(gpu,full);command.kind=SwfDraw::mask_disable;draw(gpu,command);draw(gpu,end());auto green=pixel(32,32);check(green[0]<3&&green[1]>250&&green[2]<3,"mask output");check(pixel(4,4)==original,"mask failed outside region");
 draw(gpu,begin());std::string error;check(!gpu.draw(begin(),error),"nested display accepted");check(gpu.draw(begin(),error)&&gpu.draw(end(),error),"nested failure did not restore logical scope");
 background();draw(gpu,begin());const float pane[]{320,960,320,960};check(gpu.scene_pane(pane,nullptr,[](void*,int w,int h,std::string&){if(w!=32||h!=32)return false;glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glClearColor(1,1,0,1);glClear(GL_COLOR_BUFFER_BIT);return true;},error),error.c_str());draw(gpu,end());auto yellow=pixel(32,32);check(yellow[0]>250&&yellow[1]>250&&yellow[2]<3,"scene pane lazy materialization");check(pixel(4,4)==original,"scene pane modified outside area");check(glGetError()==GL_NO_ERROR,"final GL error");

 // Deferred error remains observable at the next explicit boundary, with its
 // source command range, rather than being hidden or polled per primitive.
 background();draw(gpu,begin());draw(gpu,square(255,0,0,255));const auto queries=gpu.stats_v37().error_queries;
 for(unsigned i=0;i<10;++i)draw(gpu,square(255,0,0,255));
 check(gpu.stats_v37().error_queries==queries,"warm primitive issued GL error query");
 glEnable(0xdead);check(!gpu.draw(end(),error)&&error.find("commands [")!=std::string::npos,"deferred error range missing");
 draw(gpu,begin());draw(gpu,end());
 auto malformed=invisible;malformed.fill.kind=dh2::ui::SwfFill::bitmap;malformed.fill.texture={999999,1,1};
 draw(gpu,begin());check(!gpu.draw(malformed,error),"transparent invalid texture skipped validation");
 // Non-normal bitmap passes still submit even with exact zero alpha.
 const unsigned char texel[]{255,255,255,255};dh2::ui::SwfTexture bitmap;check(gpu.image(1,1,4,texel,4,bitmap,error),error.c_str());
 for(unsigned blend:{3u,4u,13u}){background();draw(gpu,begin());auto d=square(255,255,255,0);d.fill.kind=dh2::ui::SwfFill::bitmap;d.fill.texture=bitmap;d.fill.blend=blend;d.color_transform.value[6]=0;
 const auto materializations=gpu.stats_v36().materializations;draw(gpu,d);draw(gpu,end());check(gpu.stats_v36().materializations==materializations+1,"non-normal alpha-zero pass omitted");}
 // Actual context loss: retained CPU textures/cache survive, old GL names are
 // abandoned, and the original shader pack is compiled in the new context.
 const auto reuploads=gpu.stats_v36().cache.context_reuploads;const auto caps=gpu.stats_v37().capability_queries;
 check(eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT),"release context");check(eglDestroyContext(display,context),"destroy context");
 context=eglCreateContext(display,config,EGL_NO_CONTEXT,context_attributes);check(context!=EGL_NO_CONTEXT&&eglMakeCurrent(display,surface,surface,context),"recreated GLES context");gpu.initialize(reinterpret_cast<AAssetManager*>(1));
 background();draw(gpu,begin());draw(gpu,square(255,0,0,255));draw(gpu,end());check(pixel(32,32)==red,"context recreation changed pixels");
 check(gpu.stats_v36().cache.context_reuploads>reuploads&&gpu.stats_v37().capability_queries==caps+3,"context cache reset/reupload missing");check(glGetError()==GL_NO_ERROR,"post-context GL error");
 std::cout<<"PASS isolated actual GLES2/source shader pixels: empty100, alpha-zero suppressed, cached repeated geometry, transparent mask retained, nested failure, scene pane, deferred error range, non-normal passes, context recreation; checks="<<checks<<"\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
