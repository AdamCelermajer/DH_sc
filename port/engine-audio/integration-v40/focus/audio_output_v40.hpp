#pragma once
#include "../../audio_mixer_v34.hpp"
#include <aaudio/AAudio.h>
#include <atomic>
#include <string>
#include <thread>
#include "audio_lifecycle_gate_v40.hpp"
namespace dh2::audio {
// AAudio is resolved at runtime so the existing API23 target can still load.
// An API26+ device supplies the real output; absence remains a required leaf.
struct AAudioApiV40 {
 void*library{};
 aaudio_result_t(*create)(AAudioStreamBuilder**){};
 aaudio_result_t(*delete_builder)(AAudioStreamBuilder*){};
 void(*direction)(AAudioStreamBuilder*,aaudio_direction_t){};
 void(*format)(AAudioStreamBuilder*,aaudio_format_t){};
 void(*channels)(AAudioStreamBuilder*,std::int32_t){};
 void(*performance)(AAudioStreamBuilder*,aaudio_performance_mode_t){};
 void(*sharing)(AAudioStreamBuilder*,aaudio_sharing_mode_t){};
 void(*data_callback)(AAudioStreamBuilder*,AAudioStream_dataCallback,void*){};
 void(*error_callback)(AAudioStreamBuilder*,AAudioStream_errorCallback,void*){};
 aaudio_result_t(*open)(AAudioStreamBuilder*,AAudioStream**){};
 aaudio_format_t(*get_format)(AAudioStream*){};
 std::int32_t(*get_channels)(AAudioStream*){};
 std::int32_t(*rate)(AAudioStream*){};
 std::int32_t(*burst)(AAudioStream*){};
 aaudio_result_t(*buffer)(AAudioStream*,std::int32_t){};
 aaudio_result_t(*start)(AAudioStream*){};
 aaudio_result_t(*pause)(AAudioStream*){};
 aaudio_result_t(*stop)(AAudioStream*){};
 aaudio_result_t(*close)(AAudioStream*){};
 aaudio_result_t(*timestamp)(AAudioStream*,clockid_t,std::int64_t*,std::int64_t*){};
 bool load(std::string&);void unload()noexcept;
};
struct AudioDeviceClockSnapshotV40 {
 std::int64_t position{},monotonic_ns{};
 std::int32_t rate{};
 std::uint64_t base_frame{},generation{};
};
// Construct/use/destroy on one dedicated control thread. close() success is
// required before releasing this owner or its borrowed mixer/gate.
class AndroidAudioOutputV40 {
 AudioMixerV34& mixer_;AudioLifecycleGateV40& gate_;
 AAudioStream* stream_{};AAudioApiV40 api_;
 std::atomic<bool> audible_{false},disconnected_{false};
 bool running_{};
 bool close_failed_{};
 std::string close_error_;
 std::uint64_t stream_base_frame_{},generation_{};
 std::atomic<std::uint32_t> stream_source_epoch_{0};
 std::thread::id control_{std::this_thread::get_id()};
 static aaudio_data_callback_result_t data(AAudioStream*,void*,void*,std::int32_t);
 static void error(AAudioStream*,void*,aaudio_result_t);
 bool on_control(std::string&)const;
 bool open(std::string&);
public:
 AndroidAudioOutputV40(AudioMixerV34& mixer,AudioLifecycleGateV40& gate):mixer_(mixer),gate_(gate){}
 ~AndroidAudioOutputV40();
 bool apply(std::string&);
 bool recover(std::string&);
 bool close(std::string&);
 bool snapshot(AudioDeviceClockSnapshotV40&)const noexcept;
 bool opened()const noexcept{return stream_!=nullptr;}
 bool audible_requested()const noexcept{return audible_.load(std::memory_order_acquire)&&gate_.permitted_for(stream_source_epoch_.load(std::memory_order_acquire));}
};
}
