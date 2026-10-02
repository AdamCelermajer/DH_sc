#include "../lua-runtime/runtime.h"
#include <jni.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

/* Activity-owned, main-thread-only session. Java clears the handle on destruction.
 * Original engine objects and callback dispatch are not wired to this runtime. */
extern "C" JNIEXPORT jlong JNICALL
Java_local_dh2_sourceviewer_MainActivity_createScriptSession(JNIEnv*,jclass) {
    return static_cast<jlong>(reinterpret_cast<std::uintptr_t>(dh2_lua_create(8*1024*1024)));
}
extern "C" JNIEXPORT void JNICALL
Java_local_dh2_sourceviewer_MainActivity_destroyScriptSession(JNIEnv*,jclass,jlong handle) {
    dh2_lua_destroy(reinterpret_cast<dh2_lua*>(static_cast<std::uintptr_t>(handle)));
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_executeScript(JNIEnv* env,jclass,jlong handle,jbyteArray source) {
    auto* runtime=reinterpret_cast<dh2_lua*>(static_cast<std::uintptr_t>(handle));
    if(!runtime || !source)return env->NewStringUTF("Script rejected: session unavailable");
    const auto length=env->GetArrayLength(source);
    if(length<0 || length>1024*1024)return env->NewStringUTF("Script rejected: exceeds 1 MiB limit");
    auto* bytes=static_cast<char*>(std::malloc(length?static_cast<std::size_t>(length):1));
    if(!bytes)return env->NewStringUTF("Script rejected: out of memory");
    if(length)env->GetByteArrayRegion(source,0,length,reinterpret_cast<jbyte*>(bytes));
    if(env->ExceptionCheck()) { std::free(bytes);return nullptr; }
    char error[512]{};const int status=dh2_lua_execute(runtime,bytes,length,1000,error,sizeof(error));
    std::free(bytes);
    if(!status)return env->NewStringUTF("Script loaded. Game objects are not connected yet.");
    /* Arbitrary source can raise arbitrary bytes. Return ASCII-safe diagnostics,
     * avoiding passing unvalidated bytes to JNI's modified-UTF-8 decoder. */
    char message[560]="Script rejected: ";const std::size_t start=std::strlen(message);
    for(std::size_t i=0;error[i] && i<sizeof(error)-1 && start+i<sizeof(message)-1;++i) {
        const auto byte=static_cast<unsigned char>(error[i]);
        message[start+i]=byte>=32 && byte<127?static_cast<char>(byte):' ';
    }
    return env->NewStringUTF(message);
}
