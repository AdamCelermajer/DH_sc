#include <jni.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <time.h>
#include <cstdint>
#include <sstream>
#include <string_view>
namespace {
bool token(const char* raw,std::string_view wanted){
 if(!raw){return false;}std::size_t size=0;while(size<65536&&raw[size]){++size;}
 if(size==65536){return false;}std::string_view text(raw,size);std::size_t at=0;
 while(at<text.size()){const auto end=text.find(' ',at);const auto length=end==text.npos?text.size()-at:end-at;
  if(text.substr(at,length)==wanted){return true;}if(end==text.npos){break;}at=end+1;}
 return false;
}
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_FrameBoundaryV42_discoverPresentCapabilities(JNIEnv* env,jclass){
 try{
 const auto context=eglGetCurrentContext(),display=eglGetCurrentDisplay();const auto surface=eglGetCurrentSurface(EGL_DRAW);
 if(context==EGL_NO_CONTEXT||display==EGL_NO_DISPLAY){return env->NewStringUTF("{\"available\":false,\"reason\":\"no current EGL context\"}");}
 const bool advertised=token(reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)),"GL_EXT_disjoint_timer_query");
 const auto query=reinterpret_cast<PFNGLGETQUERYIVEXTPROC>(eglGetProcAddress("glGetQueryivEXT"));
 const bool timer_functions=query&&eglGetProcAddress("glGenQueriesEXT")&&eglGetProcAddress("glDeleteQueriesEXT")&&eglGetProcAddress("glBeginQueryEXT")&&eglGetProcAddress("glEndQueryEXT")&&eglGetProcAddress("glGetQueryObjectuivEXT")&&eglGetProcAddress("glGetQueryObjectui64vEXT");
 GLint counter_bits=0,disjoint=0;if(advertised&&timer_functions){query(GL_TIME_ELAPSED_EXT,GL_QUERY_COUNTER_BITS_EXT,&counter_bits);glGetIntegerv(GL_GPU_DISJOINT_EXT,&disjoint);}
 const bool timestamps=token(eglQueryString(display,EGL_EXTENSIONS),"EGL_ANDROID_get_frame_timestamps");
 const auto supported=reinterpret_cast<PFNEGLGETFRAMETIMESTAMPSUPPORTEDANDROIDPROC>(eglGetProcAddress("eglGetFrameTimestampSupportedANDROID"));
 const bool frame_functions=supported&&eglGetProcAddress("eglGetNextFrameIdANDROID")&&eglGetProcAddress("eglGetFrameTimestampsANDROID");
 const bool present_supported=timestamps&&frame_functions&&surface!=EGL_NO_SURFACE&&supported(display,surface,EGL_DISPLAY_PRESENT_TIME_ANDROID)==EGL_TRUE;
 std::ostringstream out;out<<"{\"schema\":\"dh2-present-capabilities-v42\",\"tid\":"<<syscall(SYS_gettid)<<",\"timer_extension_advertised\":"<<(advertised?"true":"false")<<",\"timer_functions\":"<<(timer_functions?"true":"false")<<",\"elapsed_counter_bits\":"<<counter_bits<<",\"gpu_disjoint_at_discovery\":"<<disjoint<<",\"frame_timestamp_extension_advertised\":"<<(timestamps?"true":"false")<<",\"frame_timestamp_functions\":"<<(frame_functions?"true":"false")<<",\"display_present_supported\":"<<(present_supported?"true":"false")<<",\"gpu_or_present_samples_collected\":false}";
 return env->NewStringUTF(out.str().c_str());
 }catch(...){return nullptr;}
 // No glGetError/eglGetError consumption, timer allocation or synchronization.
}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_FrameBoundaryV42_sampleGuestThreadCounters(JNIEnv* env,jclass){
 try{
 rusage usage{};timespec cpu{},wall{};const bool valid=getrusage(RUSAGE_THREAD,&usage)==0&&clock_gettime(CLOCK_THREAD_CPUTIME_ID,&cpu)==0&&clock_gettime(CLOCK_MONOTONIC,&wall)==0;
 if(!valid){return env->NewStringUTF("{\"available\":false}");}
 std::ostringstream out;out<<"{\"schema\":\"dh2-guest-thread-v42\",\"available\":true,\"tid\":"<<syscall(SYS_gettid)<<",\"wall_ns\":"<<std::uint64_t(wall.tv_sec)*1000000000ull+wall.tv_nsec<<",\"thread_cpu_ns\":"<<std::uint64_t(cpu.tv_sec)*1000000000ull+cpu.tv_nsec<<",\"minor_faults\":"<<usage.ru_minflt<<",\"major_faults\":"<<usage.ru_majflt<<",\"voluntary_switches\":"<<usage.ru_nvcsw<<",\"involuntary_switches\":"<<usage.ru_nivcsw<<",\"host_qemu_paging_measured\":false}";
 return env->NewStringUTF(out.str().c_str());
 }catch(...){return nullptr;}
}
