// Stand-ins for the Android JNI bridge used by ALicenseCheck. The Windows port has no
// JVM, so every call returns null or leaves its output empty. The original's JNIEnv
// function table is not modelled; each helper names the JNI call it replaces.
#pragma once

#include <cstddef>

struct _JNIEnv;
struct _jclass;
struct _jmethodID;
struct _jobject;

using JNIEnv = _JNIEnv;
using jclass = _jclass*;
using jmethodID = _jmethodID*;
using jobject = _jobject*;

// NVThreadGetCurrentJNIEnv(): the JNI env of the calling thread.
inline JNIEnv* NVThreadGetCurrentJNIEnv() { return nullptr; }

// JNIEnv::NewGlobalRef / FindClass / GetStaticMethodID: class and method lookups.
inline jclass JniNewGlobalRef(jclass cls) { return cls; }
inline jclass JniFindClass(const char* /*name*/) { return nullptr; }
inline jmethodID JniGetStaticMethodID(jclass /*cls*/, const char* /*name*/, const char* /*sig*/) {
    return nullptr;
}

// _JNIEnv::CallStaticObjectMethod with a byte[] result: the byte array is copied into
// `out` (at most `max` bytes). Here no array is produced, so `out` stays zeroed.
// The overload with `arg` passes one NewStringUTF argument (ALicenseCheck::CallJNIFuncChar).
inline void JniCopyStaticByteArray(jclass /*cls*/, jmethodID /*method*/, char* /*out*/,
                                   int /*max*/) {}
inline void JniCopyStaticByteArray(jclass /*cls*/, jmethodID /*method*/, const char* /*arg*/,
                                   char* /*out*/, int /*max*/) {}

// _JNIEnv::CallStaticVoidMethod (no arguments).
inline void JniCallStaticVoidMethod(jclass /*cls*/, jmethodID /*method*/) {}

// The telephony lookup in ALicenseCheck::getIMEI: TELEPHONY_SERVICE via
// getSystemService on the activity, then getDeviceId() as a string. Returns an empty
// string here.
inline void JniTelephonyDeviceId(char* out, std::size_t max) {
    if (max > 0) {
        out[0] = '\0';
    }
}
