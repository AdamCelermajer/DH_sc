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
static jstring importData(JNIEnv* env,jlong handle,jbyteArray source,int kind) {
    const char* label=kind==5?"Quests":kind==4?"Constants":kind==3?"Powers":kind==2?"Items":kind==1?"Classes":"Properties";
    auto* runtime=reinterpret_cast<dh2_lua*>(static_cast<std::uintptr_t>(handle));
    char message[560]{};
    if(!runtime || !source) { std::snprintf(message,sizeof(message),"%s rejected: session unavailable",label);return env->NewStringUTF(message); }
    const auto length=env->GetArrayLength(source);
    if(length<0 || length>4*1024*1024) { std::snprintf(message,sizeof(message),"%s rejected: exceeds 4 MiB limit",label);return env->NewStringUTF(message); }
    auto* bytes=static_cast<unsigned char*>(std::malloc(length?static_cast<std::size_t>(length):1));
    if(!bytes) { std::snprintf(message,sizeof(message),"%s rejected: out of memory",label);return env->NewStringUTF(message); }
    if(length)env->GetByteArrayRegion(source,0,length,reinterpret_cast<jbyte*>(bytes));
    if(env->ExceptionCheck()) { std::free(bytes);return nullptr; }
    char error[512]{};
    const int status=kind==5?dh2_lua_import_quests(runtime,bytes,length,error,sizeof(error)):
                     kind==4?dh2_lua_import_constants(runtime,bytes,length,error,sizeof(error)):
                     kind==3?dh2_lua_import_item_powers(runtime,bytes,length,error,sizeof(error)):
                     kind==2?dh2_lua_import_loot_tables(runtime,bytes,length,error,sizeof(error)):
                     kind==1?dh2_lua_import_character_classes(runtime,bytes,length,error,sizeof(error)):
                             dh2_lua_import_character_properties(runtime,bytes,length,error,sizeof(error));
    std::free(bytes);
    if(!status)return env->NewStringUTF(kind==5?"Quests loaded. Quest records are ready for scripts.":
                                      kind==4?"Constants loaded. Script combat calculations are ready; gameplay is unfinished.":
                                      kind==3?"Powers loaded. New script property objects can calculate gear stats; gameplay is unfinished.":
                                      kind==2?"Items loaded. New script property objects can test equipment bonuses; gameplay is unfinished.":
                                      kind==1?"Classes loaded. New script property objects can apply class rules; gameplay is unfinished.":
                                             "Properties loaded. Script property objects are ready; gameplay is unfinished.");
    std::snprintf(message,sizeof(message),"%s rejected: ",label);const std::size_t start=std::strlen(message);
    for(std::size_t i=0;error[i] && i<sizeof(error)-1 && start+i<sizeof(message)-1;++i) {
        const auto byte=static_cast<unsigned char>(error[i]);
        message[start+i]=byte>=32 && byte<127?static_cast<char>(byte):' ';
    }
    return env->NewStringUTF(message);
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_importProperties(JNIEnv* env,jclass,jlong handle,jbyteArray source) {
    return importData(env,handle,source,0);
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_importClasses(JNIEnv* env,jclass,jlong handle,jbyteArray source) {
    return importData(env,handle,source,1);
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_importItems(JNIEnv* env,jclass,jlong handle,jbyteArray source) {
    return importData(env,handle,source,2);
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_importPowers(JNIEnv* env,jclass,jlong handle,jbyteArray source) {
    return importData(env,handle,source,3);
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_importConstants(JNIEnv* env,jclass,jlong handle,jbyteArray source) {
    return importData(env,handle,source,4);
}
extern "C" JNIEXPORT jstring JNICALL
Java_local_dh2_sourceviewer_MainActivity_importQuests(JNIEnv* env,jclass,jlong handle,jbyteArray source) {
    return importData(env,handle,source,5);
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
