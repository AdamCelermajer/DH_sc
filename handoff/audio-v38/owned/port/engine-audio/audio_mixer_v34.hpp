#pragma once
#include "audio_sample_v34.hpp"
#include "audio_native_envelope_v34.hpp"
#include <atomic>
#include <array>
#include <cstdint>
namespace dh2::audio {
// Source PriorityBankBehavior: 0 steal oldest, 1 lower priority only,
// 2 lower priority or oldest equal, 3 refuse when full.
struct AudioBankV34 {std::int32_t id{},minimum_priority{},max_playbacks{},behavior{3};};
enum class AudioCommandKindV34:std::uint8_t {play,stop,pause_voice,resume_voice,gains,volume,pause_all,resume_all,stop_all,native_state};
struct AudioCommandV34 {
 AudioCommandKindV34 kind{};std::uint64_t token{},start_frame{};
 const AudioSampleV34* sample{};std::int32_t bank{},priority{},volume_group{},native_state{-1};
 float left{1},right{1},pitch{1};std::uint32_t fade_frames{};bool loop{};
};
enum class AudioReceiptKindV34:std::uint8_t {started,completed,stopped,stolen,rejected,malformed,control_required};
struct AudioReceiptV34 {std::uint64_t token{},frame{};AudioReceiptKindV34 kind{};};
template<class T,unsigned N>class AudioSpscV34 {
 std::array<T,N> values_{};std::atomic<unsigned> read_{0},write_{0};
public:
 bool push(const T&v)noexcept{const unsigned w=write_.load(std::memory_order_relaxed),next=(w+1)%N;if(next==read_.load(std::memory_order_acquire))return false;values_[w]=v;write_.store(next,std::memory_order_release);return true;}
 const T* front()const noexcept{const unsigned r=read_.load(std::memory_order_relaxed);return r==write_.load(std::memory_order_acquire)?nullptr:&values_[r];}
 void pop()noexcept{read_.store((read_.load(std::memory_order_relaxed)+1)%N,std::memory_order_release);}
 bool take(T&out)noexcept{const auto*p=front();if(!p)return false;out=*p;pop();return true;}
};
class AudioMixerV34 {
 struct Voice {
  AudioCommandV34 command{};AudioSampleCursorV34 cursor,old_cursor;
  double position{};float gain{1},fade_target{1},fade_step{};
  std::uint32_t fade_remaining{};std::int32_t element{-1},repeats{};
  bool active{},paused{},retiring{},stop_after_fade{},started{};AudioReceiptKindV34 retired_kind{};
  bool native_transition{};double old_position{};
  AudioNativeEnvelopeV34 current_envelope,old_envelope;
 };
 std::array<Voice,64> voices_{};std::array<AudioBankV34,16> banks_{};unsigned bank_count_{};
 AudioSpscV34<AudioCommandV34,512> commands_;AudioSpscV34<AudioReceiptV34,1024> receipts_;
 std::array<float,3> group_volume_{1,1,1};bool paused_{};unsigned rate_{48000};
 std::uint64_t frame_{};std::atomic<std::uint64_t> observed_frame_{0};std::atomic<unsigned> observed_voices_{0};
 bool finish(Voice&,AudioReceiptKindV34)noexcept;
 bool bind_element(Voice&,bool next)noexcept;
 bool apply(const AudioCommandV34&)noexcept;
public:
 // Configure while output is stopped. IDs are original soundpack bank UIDs.
 bool configure_banks(const AudioBankV34*,unsigned) noexcept;
 bool set_rate(unsigned) noexcept;
 bool post(const AudioCommandV34&c) noexcept{return commands_.push(c);}
 bool receipt(AudioReceiptV34&r) noexcept{return receipts_.take(r);}
 // Single audio consumer, no locks, IO, allocations or shared_ptr destruction.
 void render(float* stereo,unsigned frames) noexcept;
 void advance_silence(unsigned frames) noexcept{frame_+=frames;observed_frame_.store(frame_,std::memory_order_release);}
 std::uint64_t output_frame()const noexcept{return observed_frame_.load(std::memory_order_acquire);}
 unsigned active_voices()const noexcept{return observed_voices_.load(std::memory_order_acquire);}
};
}
