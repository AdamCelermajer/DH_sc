#pragma once
#include <jni.h>
#include <cstdint>
#include <string>
#include <functional>
#include <vector>
namespace dh2::android_ui {
bool bind_android_intro_movie_v119(JNIEnv*,jobject,jobject,std::string&);
using IntroCacheReadV119=std::function<bool(const std::string&,bool&,std::vector<std::uint8_t>&,std::uint32_t,std::string&)>;
bool bind_android_intro_cache_v119(IntroCacheReadV119,std::string&);
bool start_android_intro_movie_v119(std::int32_t,std::string&);
bool intro_finished_v119(bool&,std::string&);
bool platform_language_v119(std::uint32_t&,std::string&);
void close_android_intro_movie_v119(JNIEnv*);
}
