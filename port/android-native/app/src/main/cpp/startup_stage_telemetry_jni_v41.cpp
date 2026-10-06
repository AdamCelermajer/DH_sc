#include "startup_stage_telemetry_v41.hpp"
#include <jni.h>
using namespace dh2::startup_v41;
extern "C" JNIEXPORT jlong JNICALL Java_com_example_dh2_StartupStagesV41_begin(JNIEnv*,jclass,jint request,jlong origin){if(request<0||origin<0){return 0;}return jlong(global().begin(Request(request),std::uint64_t(origin)));}
extern "C" JNIEXPORT jlong JNICALL Java_com_example_dh2_StartupStagesV41_start(JNIEnv*,jclass,jlong generation,jint stage,jlong bytes){if(generation<=0||stage<0||bytes<0){return 0;}return jlong(global().start(std::uint64_t(generation),Stage(stage),std::uint64_t(bytes)));}
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_StartupStagesV41_end(JNIEnv*,jclass,jlong ticket,jboolean success,jlong bytes){if(ticket>0&&bytes>=0){global().end(std::uint64_t(ticket),success==JNI_TRUE,std::uint64_t(bytes));}}
extern "C" JNIEXPORT jstring JNICALL Java_com_example_dh2_StartupStagesV41_finish(JNIEnv* env,jclass,jlong generation,jint outcome,jboolean events){
 try{if(generation<=0||outcome<0||!global().finish(std::uint64_t(generation),Outcome(outcome))){return nullptr;}const auto report=json(global().snapshot(),events==JNI_TRUE);return env->NewStringUTF(report.c_str());}
 catch(...){return nullptr;} // Measurement serialization must not unwind through JNI.
}
