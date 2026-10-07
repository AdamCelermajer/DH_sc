// Actual Mesa/EGL ES pixel comparison workload. Disk AAsset transport supplies
// the unmodified hash-verified shader pack; every GLES call reaches the driver.
#include "swf_gpu.hpp"
#include "../asset-payloads/sha256.hpp"
#include <EGL/egl.h>
#include <dlfcn.h>
#include <sys/resource.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <cerrno>
#include <cstddef>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using dh2::android_ui::SwfGpu;using dh2::ui::SwfDraw;
namespace {std::string shader_path;unsigned checks{};std::uint64_t error_queries{};struct File{FILE* handle;long length;};
void check(bool value,const char* text){++checks;if(!value)throw std::runtime_error(text);}
void limits(){
 // Enforced before libEGL creates a context. One address space, bounded CPU;
 // Mesa threads share the same cap. No subprocesses, fork or vfork allowed.
 const rlimit address{1024*1024*1024,1024*1024*1024},cpu{10,10},files{16*1024*1024,16*1024*1024},core{0,0};
 check(!setrlimit(RLIMIT_AS,&address)&&!setrlimit(RLIMIT_CPU,&cpu)&&!setrlimit(RLIMIT_FSIZE,&files)&&!setrlimit(RLIMIT_CORE,&core),"hard resource limits");
 rlimit actual{};check(!getrlimit(RLIMIT_AS,&actual)&&actual.rlim_max==address.rlim_max,"address limit readback");
 // glibc falls back to clone when clone3 returns ENOSYS; only CLONE_THREAD
 // is permitted. Ordinary process spawning is refused inside the fixture.
 const sock_filter code[]{
 BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,arch)),BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,AUDIT_ARCH_X86_64,1,0),BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_KILL_PROCESS),
 BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,nr)),
 BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_fork,0,1),BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|EPERM),
 BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_vfork,0,1),BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|EPERM),
 BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_clone3,0,1),BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|ENOSYS),
 BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K,SYS_clone,0,4),BPF_STMT(BPF_LD|BPF_W|BPF_ABS,offsetof(seccomp_data,args[0])),
 BPF_JUMP(BPF_JMP|BPF_JSET|BPF_K,CLONE_THREAD,1,0),BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ERRNO|EPERM),BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW),
 BPF_STMT(BPF_RET|BPF_K,SECCOMP_RET_ALLOW)};
 const sock_fprog program{sizeof(code)/sizeof(code[0]),const_cast<sock_filter*>(code)};
 check(!prctl(PR_SET_NO_NEW_PRIVS,1,0,0,0)&&!prctl(PR_SET_SECCOMP,SECCOMP_MODE_FILTER,&program),"thread-only seccomp");alarm(20);
}
void draw(SwfGpu& gpu,SwfDraw d){std::string error;check(gpu.draw(d,error),error.c_str());}
SwfDraw begin(int w=64,int h=64){SwfDraw d;d.kind=SwfDraw::begin;d.bounds[1]=d.bounds[3]=1280;d.viewport[2]=w;d.viewport[3]=h;return d;}
SwfDraw end(){SwfDraw d;d.kind=SwfDraw::end;return d;}
SwfDraw square(unsigned char r,unsigned char g,unsigned char b,unsigned char a){SwfDraw d;d.kind=SwfDraw::triangle_strip;d.fill.kind=dh2::ui::SwfFill::color;d.fill.rgba[0]=r;d.fill.rgba[1]=g;d.fill.rgba[2]=b;d.fill.rgba[3]=a;d.xy={320,320,960,320,320,960,960,960};return d;}
void background(){glBindFramebuffer(GL_FRAMEBUFFER,0);glViewport(0,0,320,180);glDisable(GL_SCISSOR_TEST);glDisable(GL_STENCIL_TEST);glDisable(GL_DEPTH_TEST);glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glClearColor(.2f,.4f,.6f,1);glClear(GL_COLOR_BUFFER_BIT);}
std::string digest(const std::vector<unsigned char>& data){dh2::assets::Sha256Digest hash{};check(dh2::assets::sha256(data.data(),data.size(),hash),"pixel hash");std::string out;for(auto b:hash){char text[3];std::snprintf(text,3,"%02x",b);out+=text;}return out;}
void capture(const char* name){std::vector<unsigned char> pixels(64*64*4);glPixelStorei(GL_PACK_ALIGNMENT,1);glReadPixels(0,0,64,64,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());check(glGetError()==GL_NO_ERROR,"readback error");std::cout<<"PIXEL "<<name<<" "<<digest(pixels)<<"\n";}
void failed(SwfGpu& gpu,SwfDraw d,const char* label){std::string error;check(!gpu.draw(d,error),label);std::cout<<"ERROR "<<label<<" "<<error<<"\n";}
}
extern "C" AAsset* AAssetManager_open(AAssetManager*,const char* name,int){if(std::strcmp(name,"shaders/shaders.pak"))return nullptr;auto* file=std::fopen(shader_path.c_str(),"rb");if(!file)return nullptr;std::fseek(file,0,SEEK_END);const auto n=std::ftell(file);std::rewind(file);return reinterpret_cast<AAsset*>(new File{file,n});}
extern "C" off64_t AAsset_getLength64(AAsset* p){return reinterpret_cast<File*>(p)->length;}
extern "C" int AAsset_read(AAsset* p,void* data,size_t count){return int(std::fread(data,1,count,reinterpret_cast<File*>(p)->handle));}
extern "C" void AAsset_close(AAsset* p){auto* file=reinterpret_cast<File*>(p);std::fclose(file->handle);delete file;}
// Only instrument the real error query; the actual GL error and driver behavior
// remain intact. All source drawing/shader/texture calls reach libGL dispatch.
extern "C" GLenum glGetError(){using Query=GLenum(*)();static const auto query=reinterpret_cast<Query>(dlsym(RTLD_NEXT,"glGetError"));if(!query)throw std::runtime_error("real GL dispatch missing");++error_queries;return query();}
int main(int argc,char** argv){try{
 limits();check(argc==2,"actual shader pack required");shader_path=argv[1];auto display=eglGetDisplay(EGL_DEFAULT_DISPLAY);check(display!=EGL_NO_DISPLAY&&eglInitialize(display,nullptr,nullptr),"surfaceless EGL init");check(eglBindAPI(EGL_OPENGL_ES_API),"actual GLES API");
 const EGLint ca[]{EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_ALPHA_SIZE,8,EGL_STENCIL_SIZE,8,EGL_DEPTH_SIZE,24,EGL_NONE};
 EGLConfig config{};EGLint count{};check(eglChooseConfig(display,ca,&config,1,&count)&&count==1,"pbuffer config");const EGLint sa[]{EGL_WIDTH,320,EGL_HEIGHT,180,EGL_NONE},ctx[]{EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};
 auto surface=eglCreatePbufferSurface(display,config,sa);auto context=eglCreateContext(display,config,EGL_NO_CONTEXT,ctx);check(surface!=EGL_NO_SURFACE&&context!=EGL_NO_CONTEXT&&eglMakeCurrent(display,surface,surface,context),"pbuffer context");
 const std::string renderer=reinterpret_cast<const char*>(glGetString(GL_RENDERER)),version=reinterpret_cast<const char*>(glGetString(GL_VERSION));check(renderer.find("llvmpipe")!=std::string::npos&&version.find("OpenGL ES")!=std::string::npos,"software actual GLES required");std::cout<<"DRIVER "<<renderer<<" | "<<version<<"\n";
 SwfGpu gpu;gpu.initialize(reinterpret_cast<AAssetManager*>(1));background();capture("baseline");
 draw(gpu,begin());draw(gpu,end());capture("empty");draw(gpu,begin());draw(gpu,square(255,0,0,0));draw(gpu,end());capture("normal-zero");
 background();draw(gpu,begin());draw(gpu,square(255,0,0,255));draw(gpu,end());capture("visible");
 background();draw(gpu,begin());auto tint=square(255,0,0,0);tint.color_transform.value[7]=128;draw(gpu,tint);draw(gpu,end());capture("zero-raised-by-cxform");
 background();draw(gpu,begin());auto line=square(255,0,0,0);line.kind=SwfDraw::line_strip;line.line=line.fill;line.line_width=40;draw(gpu,line);draw(gpu,end());capture("line-zero");
 background();draw(gpu,begin());SwfDraw command;command.kind=SwfDraw::mask_begin;draw(gpu,command);draw(gpu,square(255,255,255,0));command.kind=SwfDraw::mask_end;draw(gpu,command);auto full=square(0,255,0,255);full.xy={0,0,1280,0,0,1280,1280,1280};draw(gpu,full);
 std::string error;bool hit{};const float bounds[]{20,40,20,40};check(gpu.stencil(bounds,1,hit,error)&&hit,"actual transparent mask stencil");command.kind=SwfDraw::mask_disable;draw(gpu,command);draw(gpu,end());capture("transparent-mask");
 background();draw(gpu,begin());command.kind=SwfDraw::mask_begin;draw(gpu,command);draw(gpu,square(255,255,255,255));command.kind=SwfDraw::mask_end;draw(gpu,command);command.kind=SwfDraw::mask_begin;draw(gpu,command);auto inner=square(255,255,255,255);inner.xy={480,480,800,480,480,800,800,800};draw(gpu,inner);command.kind=SwfDraw::mask_end;draw(gpu,command);draw(gpu,full);command.kind=SwfDraw::mask_disable;draw(gpu,command);draw(gpu,square(255,0,0,96));draw(gpu,command);draw(gpu,end());capture("nested-mask-pop");
 const unsigned char texel[]{128,200,64,160};dh2::ui::SwfTexture texture;check(gpu.image(1,1,4,texel,4,texture,error),error.c_str());
 for(unsigned blend:{0u,1u,3u,4u,13u,15u,16u})for(unsigned alpha:{0u,128u,255u}){
 background();draw(gpu,begin());auto d=square(255,255,255,255);d.fill.kind=dh2::ui::SwfFill::bitmap;d.fill.texture=texture;d.fill.blend=blend;d.color_transform.value[6]=float(alpha)/255;draw(gpu,d);draw(gpu,end());const auto name="blend"+std::to_string(blend)+"-alpha"+std::to_string(alpha);capture(name.c_str());}
 background();draw(gpu,begin());const float pane[]{320,960,320,960};check(gpu.scene_pane(pane,nullptr,[](void*,int w,int h,std::string&){if(w!=32||h!=32)return false;glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);glClearColor(1,1,0,1);glClear(GL_COLOR_BUFFER_BIT);return true;},error),error.c_str());draw(gpu,end());capture("scene-pane");
 draw(gpu,begin());draw(gpu,square(255,0,0,255));glEnable(0xdead);failed(gpu,end(),"display-error");check(glGetError()==GL_NO_ERROR,"error recovered");
 draw(gpu,begin());auto invalid=square(255,0,0,0);invalid.fill.kind=dh2::ui::SwfFill::bitmap;invalid.fill.texture={999999,1,1};failed(gpu,invalid,"invalid-transparent-texture");
 check(eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT)&&eglDestroyContext(display,context),"context destroy");context=eglCreateContext(display,config,EGL_NO_CONTEXT,ctx);check(context!=EGL_NO_CONTEXT&&eglMakeCurrent(display,surface,surface,context),"context recreate");gpu.initialize(reinterpret_cast<AAssetManager*>(1));background();draw(gpu,begin());draw(gpu,square(255,0,0,255));draw(gpu,end());capture("context-visible");
 // Stable synthetic HUD submission benchmark, not app FPS: eight visible
 // draws, a separate four-command alpha-zero hurt movie and an empty movie.
 auto workload=[&](){background();draw(gpu,begin(320,180));for(unsigned i=0;i<8;++i){auto d=square(255,0,0,128);d.matrix.value[2]=float(i*30);draw(gpu,d);}draw(gpu,end());draw(gpu,begin(320,180));for(unsigned i=0;i<4;++i)draw(gpu,square(255,0,0,0));draw(gpu,end());draw(gpu,begin(320,180));draw(gpu,end());glFinish();};
 for(unsigned i=0;i<20;++i)workload();const auto queries_before=error_queries;std::vector<double> times;
 for(unsigned batch=0;batch<3;++batch){const auto start=std::chrono::steady_clock::now();for(unsigned i=0;i<100;++i)workload();times.push_back(std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-start).count()/100);}
 std::cout<<"BENCH mean_ms "<<(times[0]+times[1]+times[2])/3<<" batches "<<times[0]<<","<<times[1]<<","<<times[2]<<" error_queries "<<error_queries-queries_before<<" frames 300\n";
#ifdef DH2_SUBMISSION_V37
 const auto stats=gpu.stats_v37();check(stats.capability_queries==6,"capabilities did not reset once per context");std::cout<<"STATS noops "<<stats.transparent_noops<<" error_queries "<<stats.error_queries<<" capability_queries "<<stats.capability_queries<<" commands "<<stats.commands<<"\n";
#endif
 rusage usage{};check(!getrusage(RUSAGE_SELF,&usage),"resource receipt");std::cout<<"RESOURCE peak_rss_KiB "<<usage.ru_maxrss<<" hard_AS_MiB 1024 hard_CPU_sec 10 alarm_sec 20 no_fork 1\n";check(glGetError()==GL_NO_ERROR,"final GL error");std::cout<<"PASS checks "<<checks<<"\n";eglMakeCurrent(display,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);eglDestroyContext(display,context);eglDestroySurface(display,surface);eglTerminate(display);return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
