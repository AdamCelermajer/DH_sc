#pragma once
#include <atomic>
#include <cstdint>
namespace dh2::audio {
struct AudioDeviceClockV40 {
 std::int64_t position{},monotonic_ns{};
 std::uint64_t mixer_base_frame{},generation{};
 std::uint32_t rate{};
 bool ready{};
};
// The output control thread is the sole publisher. GL/gameplay reads only
// atomics, never AAudioStream pointers while another thread closes a stream.
class AudioClockV40 {
 std::atomic<std::uint32_t> sequence_{0},rate_{0};
 std::atomic<std::int64_t> position_{0},time_{0};
 std::atomic<std::uint64_t> base_{0},generation_{0};
 std::atomic<bool> ready_{false};
public:
 void publish(const AudioDeviceClockV40&) noexcept;
 void invalidate(std::uint64_t generation) noexcept;
 bool snapshot(AudioDeviceClockV40&)const noexcept;
 bool frame_at(std::int64_t actual_source_event_ns,std::uint64_t written_frame,
  std::uint64_t& frame)const noexcept;
};
}
