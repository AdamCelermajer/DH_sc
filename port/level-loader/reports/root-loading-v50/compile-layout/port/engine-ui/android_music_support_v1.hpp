#pragma once
#include <cstdint>
namespace dh2::ui {
// Whole original APK isSupportMM()I normal domain: both source static Build
// strings have been produced by the Java class initializer. No JNI fallback.
// 0 delivered, -1 missing/null Build string (source Java null would throw).
int android_music_support_v1(const char* actual_manufacturer,const char* actual_model,
 std::int32_t& original_jint) noexcept;
}
