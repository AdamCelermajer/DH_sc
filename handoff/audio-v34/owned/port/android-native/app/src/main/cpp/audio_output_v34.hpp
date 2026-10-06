#pragma once
#include "../../../../../engine-audio/audio_mixer_v34.hpp"
#include <aaudio/AAudio.h>
#include <atomic>
#include <string>
namespace dh2::audio {
// AAudio is resolved at runtime so the existing API23 target can still load.
// An API26+ device supplies the real output; absence remains a required leaf.
struct AAudioApiV34 {
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
// Driver lifetime belongs to the control thread; only render runs in callback.
class AndroidAudioOutputV34 {
 AudioMixerV34&mixer_;AAudioStream*stream_{};AAudioApiV34 api_;
 std::atomic<bool>audible_{false},disconnected_{false};bool resumed_{},focused_{};
 std::uint64_t stream_base_frame_{};
 static aaudio_data_callback_result_t data(AAudioStream*,void*,void*,std::int32_t);
 static void error(AAudioStream*,void*,aaudio_result_t);
public:
 explicit AndroidAudioOutputV34(AudioMixerV34&m):mixer_(m){}
 ~AndroidAudioOutputV34();
 bool open(std::string&);void close()noexcept;
 bool lifecycle(bool actual_resumed,bool actual_focused,std::string&);
 bool recover(std::string&);
 bool frame_at_monotonic_ns(std::int64_t event_ns,std::uint64_t&frame)const noexcept;
 bool opened()const noexcept{return stream_!=nullptr;}
 bool audible_requested()const noexcept{return audible_.load(std::memory_order_acquire);}
};
}
