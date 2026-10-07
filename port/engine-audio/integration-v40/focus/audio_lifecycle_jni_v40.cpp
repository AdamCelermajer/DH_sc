#include "audio_lifecycle_gate_v40.hpp"
#include <jni.h>
extern "C" JNIEXPORT void JNICALL Java_com_example_dh2_NativeBridge_audioActivityV40(
 JNIEnv*,jclass,jlong owner,jlong sequence,jboolean resumed,jboolean window,jboolean focus,jboolean destroyed) {
    if(owner<=0||owner>0xffffffffLL||sequence<=0||sequence>0xffffffLL)return;
    dh2::audio::application_audio_gate_v40().publish_activity(std::uint32_t(owner),std::uint32_t(sequence),resumed,window,focus,destroyed);
}
extern "C" JNIEXPORT jboolean JNICALL Java_com_example_dh2_NativeBridge_audioSourceReadyV40(JNIEnv*,jclass) {
    return dh2::audio::application_audio_gate_v40().source_ready()?JNI_TRUE:JNI_FALSE;
}
