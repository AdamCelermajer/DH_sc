#pragma once
#include <array>
#include <chrono>
#include <cstdint>
#include <algorithm>
#include <android/log.h>
namespace dh2::perf {
enum class Phase:unsigned {frame,actor,camera,fx_prepare,equipment,world,actors,fx_draw,hud_prepare,hud,combat_text,front,ui_geometry,ui_submit,ui_update,count};
inline constexpr const char* names[]{"frame","actor","camera","fx_prepare","equipment","world","actors","fx_draw","hud_prepare","hud","combat_text","front","ui_geometry","ui_submit","ui_update"};
struct State {std::array<std::uint64_t,unsigned(Phase::count)> ns{};std::array<double,120> frame_ms{};unsigned frames{};std::uint64_t draws{},culled{},uploads{};};
inline thread_local State state;
using Clock=std::chrono::steady_clock;
class Scope {
 Phase phase_;Clock::time_point start_=Clock::now();
public:
 explicit Scope(Phase phase):phase_(phase){}
 ~Scope(){state.ns[unsigned(phase_)]+=std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now()-start_).count();}
};
class Frame {
 Clock::time_point start_=Clock::now();
public:
 ~Frame(){
  const auto ns=std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now()-start_).count();
  state.ns[0]+=ns;state.frame_ms[state.frames++]=double(ns)*1e-6;
  if(state.frames!=state.frame_ms.size())return;
  auto sorted=state.frame_ms;std::sort(sorted.begin(),sorted.end());
  __android_log_print(ANDROID_LOG_INFO,"DH2Perf","CPU120 median %.3f p95 %.3f p99 %.3f draws %.1f culled %.1f upload_KiB %.1f",sorted[60],sorted[114],sorted[118],double(state.draws)/120.,double(state.culled)/120.,double(state.uploads)/122880.);
  for(unsigned i=0;i<unsigned(Phase::count);++i)if(state.ns[i])__android_log_print(ANDROID_LOG_INFO,"DH2Perf","phase %s mean_ms %.3f",names[i],double(state.ns[i])/120e6);
  state={};
 }
};
}
