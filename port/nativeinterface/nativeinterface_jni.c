/* JNI signatures are recovered exactly from NativeInterface.smali. */
#include "nativeinterface.h"
#include <jni.h>
#include <stdlib.h>
#include <string.h>

JNIEXPORT jboolean JNICALL
Java_com_samsung_zirconia_NativeInterface_checkLicenseFile(
        JNIEnv *env, jclass type, jstring path, jstring first, jstring second) {
    (void)type;
    if (!path || !first || !second) return JNI_FALSE;
    const char *p = (*env)->GetStringUTFChars(env, path, NULL);
    if (!p) return JNI_FALSE;
    const char *a = (*env)->GetStringUTFChars(env, first, NULL);
    if (!a) { (*env)->ReleaseStringUTFChars(env, path, p); return JNI_FALSE; }
    const char *b = (*env)->GetStringUTFChars(env, second, NULL);
    int result = b ? dh2_check_license_with_globals(p, a, b) : 0;
    if (b) (*env)->ReleaseStringUTFChars(env, second, b);
    (*env)->ReleaseStringUTFChars(env, first, a);
    (*env)->ReleaseStringUTFChars(env, path, p);
    return result ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_samsung_zirconia_NativeInterface_checkLicenseFile2(
        JNIEnv *env, jclass type, jstring path, jstring second) {
    (void)env; (void)type; (void)path; (void)second;
    /* Exact original ELF instructions at 0x920: 01 20 70 47
     * (Thumb movs r0,#1; bx lr), symbol size 4. */
    return JNI_TRUE;
}

JNIEXPORT jstring JNICALL
Java_com_samsung_zirconia_NativeInterface_doPassphraseTest(
        JNIEnv *env, jclass type, jstring first, jstring second) {
    (void)type;
    if (!first || !second) return NULL;
    const char *a = (*env)->GetStringUTFChars(env, first, NULL);
    if (!a) return NULL;
    const char *b = (*env)->GetStringUTFChars(env, second, NULL);
    if (!b) { (*env)->ReleaseStringUTFChars(env, first, a); return NULL; }
    const char *fragment;
    size_t length = dh2_passphrase(a, b, &fragment);
    char *output = (char *)calloc(length + 1, 1);
    jstring result = NULL;
    if (output) {
        strncpy(output, fragment, length);
        result = (*env)->NewStringUTF(env, output);
        memset(output, 0, length + 1);
        free(output);
    } else {
        jclass error = (*env)->FindClass(env, "java/lang/OutOfMemoryError");
        if (error) (*env)->ThrowNew(env, error, "passphrase allocation failed");
    }
    (*env)->ReleaseStringUTFChars(env, second, b);
    (*env)->ReleaseStringUTFChars(env, first, a);
    return result;
}

JNIEXPORT jboolean JNICALL
Java_com_samsung_zirconia_NativeInterface_storeLicenseKey(
        JNIEnv *env, jclass type, jstring path, jbyteArray key, jstring metadata) {
    (void)type;
    if (!path || !key || !metadata) return JNI_FALSE;
    jsize length = (*env)->GetArrayLength(env, key);
    if (length < 20) return JNI_FALSE;
    const char *p = (*env)->GetStringUTFChars(env, path, NULL);
    if (!p) return JNI_FALSE;
    jbyte *bytes = (*env)->GetByteArrayElements(env, key, NULL);
    if (!bytes) { (*env)->ReleaseStringUTFChars(env, path, p); return JNI_FALSE; }
    const char *m = (*env)->GetStringUTFChars(env, metadata, NULL);
    int result = m ? dh2_store_license_key(p, (const uint8_t *)bytes, (size_t)length, m) : 0;
    if (m) (*env)->ReleaseStringUTFChars(env, metadata, m);
    (*env)->ReleaseByteArrayElements(env, key, bytes, JNI_ABORT);
    (*env)->ReleaseStringUTFChars(env, path, p);
    return result ? JNI_TRUE : JNI_FALSE;
}
