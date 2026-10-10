#pragma once
// TEMPORARY AUDIOSTALL probe (not for release): timestamps around audio calls.
#include <chrono>
#include <cstdio>
namespace dh2::audio::probe {
inline double ms_now(){return std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now().time_since_epoch()).count();}
}
