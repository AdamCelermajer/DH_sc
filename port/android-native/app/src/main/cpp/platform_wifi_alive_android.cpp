#include "platform_wifi_alive.hpp"

#include <jni.h>

#include <mutex>

namespace {
JavaVM* java_vm = nullptr;
jclass bridge_class = nullptr;
jmethodID wifi_query = nullptr;
std::mutex bridge_mutex;

JNIEnv* current_env(bool& attached) {
    attached = false;
    if (!java_vm) return nullptr;
    JNIEnv* env = nullptr;
    const jint status = java_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    if (status == JNI_OK) return env;
    if (status != JNI_EDETACHED ||
        java_vm->AttachCurrentThread(&env, nullptr) != JNI_OK) return nullptr;
    attached = true;
    return env;
}
}

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    std::lock_guard<std::mutex> lock(bridge_mutex);
    java_vm = vm;
    JNIEnv* env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        java_vm = nullptr;
        return JNI_ERR;
    }
    jclass local = env->FindClass("com/example/dh2/NativeBridge");
    if (!local || env->ExceptionCheck()) {
        env->ExceptionClear();
        java_vm = nullptr;
        return JNI_ERR;
    }
    bridge_class = static_cast<jclass>(env->NewGlobalRef(local));
    env->DeleteLocalRef(local);
    if (!bridge_class) {
        java_vm = nullptr;
        return JNI_ERR;
    }
    wifi_query = env->GetStaticMethodID(bridge_class, "platformWifiAlive", "()Z");
    if (!wifi_query || env->ExceptionCheck()) {
        env->ExceptionClear();
        env->DeleteGlobalRef(bridge_class);
        bridge_class = nullptr;
        java_vm = nullptr;
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void*) {
    std::lock_guard<std::mutex> lock(bridge_mutex);
    JNIEnv* env = nullptr;
    if (bridge_class && vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK)
        env->DeleteGlobalRef(bridge_class);
    bridge_class = nullptr;
    wifi_query = nullptr;
    java_vm = nullptr;
}

namespace dh2::platform {

bool source_wifi_alive(bool& alive, std::string& error) {
    alive = false;
    error.clear();
    bool attached = false;
    JNIEnv* env = current_env(attached);
    if (!env) {
        error = "Android Java VM is unavailable for the Wi-Fi query";
        return false;
    }
    jclass cls = nullptr;
    jmethodID method = nullptr;
    {
        std::lock_guard<std::mutex> lock(bridge_mutex);
        cls = bridge_class;
        method = wifi_query;
    }
    if (!cls || !method) {
        error = "Android Wi-Fi query bridge is not initialized";
    } else {
        const jboolean value = env->CallStaticBooleanMethod(cls, method);
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
            error = "Android ConnectivityManager Wi-Fi query failed";
        } else {
            alive = value == JNI_TRUE;
        }
    }
    if (attached) java_vm->DetachCurrentThread();
    return error.empty();
}

} // namespace dh2::platform
