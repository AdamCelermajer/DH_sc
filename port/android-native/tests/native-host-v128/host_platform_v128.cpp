#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <zlib.h>
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fcntl.h>
#include <limits>
#include <memory>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

struct AAssetManager {std::filesystem::path root,cache;};
struct AAsset {int fd{-1};off64_t bytes{};};
namespace {
std::unique_ptr<AAssetManager> manager;
EGLDisplay display=EGL_NO_DISPLAY;EGLContext context=EGL_NO_CONTEXT;EGLSurface surface=EGL_NO_SURFACE;
int width{},height{};
std::string string(JNIEnv* env,jstring text){
 if(!text)return {};const char* p=env->GetStringUTFChars(text,nullptr);if(!p)return {};
 std::string out(p);env->ReleaseStringUTFChars(text,p);return out;
}
jstring failure(JNIEnv* env,const std::string& e){return env->NewStringUTF(e.c_str());}
void word(std::vector<unsigned char>& v,std::uint32_t n){for(int s=24;s>=0;s-=8)v.push_back(static_cast<unsigned char>(n>>s));}
void chunk(std::vector<unsigned char>& out,const char* type,const std::vector<unsigned char>& data){
 word(out,static_cast<std::uint32_t>(data.size()));const auto begin=out.size();
 out.insert(out.end(),type,type+4);out.insert(out.end(),data.begin(),data.end());
 word(out,static_cast<std::uint32_t>(crc32(0,out.data()+begin,static_cast<uInt>(4+data.size()))));
}
}
extern "C" AAsset* AAssetManager_open(AAssetManager* m,const char* name,int){
 if(!m||m!=manager.get()||!name)return nullptr;
 std::filesystem::path relative(name);if(relative.is_absolute())return nullptr;
 for(const auto& part:relative)if(part=="..")return nullptr;
 const auto path=relative=="dh2-original-cache.zip"?m->cache:m->root/relative;
 int fd=::open(path.c_str(),O_RDONLY);struct stat info{};
 if(fd<0)return nullptr;if(::fstat(fd,&info)||!S_ISREG(info.st_mode)||info.st_size<0){::close(fd);return nullptr;}
 return new AAsset{fd,info.st_size};
}
extern "C" int AAsset_read(AAsset* a,void* p,size_t n){
 if(!a||!p||n>std::size_t(std::numeric_limits<int>::max()))return -1;
 ssize_t count;do{count=::read(a->fd,p,n);}while(count<0&&errno==EINTR);return static_cast<int>(count);
}
extern "C" off64_t AAsset_getLength64(AAsset* a){return a?a->bytes:0;}
extern "C" int AAsset_openFileDescriptor64(AAsset* a,off64_t* offset,off64_t* bytes){
 if(!a||!offset||!bytes)return -1;*offset=0;*bytes=a->bytes;return ::dup(a->fd);
}
extern "C" void AAsset_close(AAsset* a){if(a){::close(a->fd);delete a;}}
extern "C" AAssetManager* AAssetManager_fromJava(JNIEnv* env,jobject assets){
 if(!env||!assets||!manager)return nullptr;auto type=env->GetObjectClass(assets);
 auto field=env->GetFieldID(type,"nativeHandle","J");env->DeleteLocalRef(type);
 if(!field||env->ExceptionCheck())return nullptr;
 const auto handle=env->GetLongField(assets,field);
 return handle==static_cast<jlong>(reinterpret_cast<std::uintptr_t>(manager.get()))?manager.get():nullptr;
}
extern "C" int __android_log_print(int priority,const char* tag,const char* format,...){
 std::fprintf(stderr,"[%d] %s: ",priority,tag?tag:"");va_list args;va_start(args,format);
 auto n=std::vfprintf(stderr,format,args);va_end(args);std::fputc('\n',stderr);return n;
}
extern "C" JNIEXPORT jlong JNICALL Java_com_example_dh2_HostRunner_configureAssets(JNIEnv* env,jclass,jstring root,jstring cache){
 if(manager)return 0;auto next=std::make_unique<AAssetManager>();next->root=string(env,root);next->cache=string(env,cache);
 if(!std::filesystem::is_directory(next->root)||!std::filesystem::is_regular_file(next->cache))return 0;
 manager=std::move(next);return static_cast<jlong>(reinterpret_cast<std::uintptr_t>(manager.get()));
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_HostRunner_createSurface(JNIEnv* env,jclass,jint w,jint h){
 if(context!=EGL_NO_CONTEXT||w<2||h<2||w>2048||h>2048)return failure(env,"Invalid/repeated host surface");
 display=eglGetDisplay(EGL_DEFAULT_DISPLAY);EGLint major{},minor{};
 if(display==EGL_NO_DISPLAY||!eglInitialize(display,&major,&minor)||!eglBindAPI(EGL_OPENGL_ES_API))return failure(env,"Host EGL initialization failed");
 EGLint attrs[]{EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,
  EGL_RED_SIZE,8,EGL_GREEN_SIZE,8,EGL_BLUE_SIZE,8,EGL_ALPHA_SIZE,8,EGL_DEPTH_SIZE,24,EGL_STENCIL_SIZE,8,EGL_NONE};
 EGLConfig config{};EGLint count{};
 if(!eglChooseConfig(display,attrs,&config,1,&count)||count!=1)return failure(env,"Host EGL config absent");
 EGLint pbuffer[]{EGL_WIDTH,w,EGL_HEIGHT,h,EGL_NONE};surface=eglCreatePbufferSurface(display,config,pbuffer);
 EGLint ctx[]{EGL_CONTEXT_CLIENT_VERSION,2,EGL_NONE};context=eglCreateContext(display,config,EGL_NO_CONTEXT,ctx);
 if(surface==EGL_NO_SURFACE||context==EGL_NO_CONTEXT||!eglMakeCurrent(display,surface,surface,context))return failure(env,"Host GLES2 surface/context failed");
 width=w;height=h;return nullptr;
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_HostRunner_capture(JNIEnv* env,jclass,jstring name){
 if(context==EGL_NO_CONTEXT)return failure(env,"No host GL capture context");
 std::vector<unsigned char> pixels(std::size_t(width)*height*4);glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());
 if(glGetError()!=GL_NO_ERROR)return failure(env,"Host glReadPixels failed");
 std::vector<unsigned char> scan;scan.reserve(pixels.size()+height);
 for(int y=height-1;y>=0;--y){scan.push_back(0);auto first=pixels.begin()+std::size_t(y)*width*4;scan.insert(scan.end(),first,first+width*4);}
 uLongf n=compressBound(scan.size());std::vector<unsigned char> compressed(n);
 if(compress2(compressed.data(),&n,scan.data(),scan.size(),3)!=Z_OK)return failure(env,"Host PNG compression failed");compressed.resize(n);
 std::vector<unsigned char> png{137,80,78,71,13,10,26,10},ihdr;word(ihdr,width);word(ihdr,height);ihdr.insert(ihdr.end(),{8,6,0,0,0});
 chunk(png,"IHDR",ihdr);chunk(png,"IDAT",compressed);chunk(png,"IEND",{});
 auto path=string(env,name);auto* file=std::fopen(path.c_str(),"wb");if(!file)return failure(env,"Host capture file open failed");
 auto written=std::fwrite(png.data(),1,png.size(),file);auto closed=std::fclose(file);
 return written==png.size()&&!closed?nullptr:failure(env,"Host capture file write failed");
}
