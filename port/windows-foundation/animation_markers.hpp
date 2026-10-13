#pragma once
#include "../engine-animation/events.hpp"
#include <cstdint>
#include <string>
#include <vector>
namespace dh::foundation {
// Names are authored identifiers, not combat decisions. Preserve repeated names
// and group order; index is the stable occurrence identity inside this clip.
struct AnimationMarker {
    std::string name;
    // Effective first integer sampling time vs original truncated lookup time.
    // e.g. authored frame8 is reported266ms but dispatch begins267ms.
    std::int32_t time_ms{}, authored_time_ms{};
    std::uint32_t entry{}, group{}, index{};
};
struct MarkerCursor {
    std::uint64_t elapsed_ms{}, generation{};
    bool started{};
};
struct MarkerOccurrence {
    AnimationMarker marker;
    std::uint64_t generation{}, cycle{}, elapsed_ms{}, lag_ms{};
};
class AnimationMarkers {
public:
    bool load(const dh2::animation::EventView&, std::int32_t start_ms,
              std::int32_t end_ms, std::string& error);
    bool load(const std::uint8_t* bres, std::size_t size, std::int32_t start_ms,
              std::int32_t end_ms, std::string& error);
    // B155: a clip without any key (exporter sentinel range, e.g. swampking_hurt_01.bdae) has no markers and a zero-length
    // range at 0. This is the only zero-length track admitted; load() still rejects end <= start.
    bool load_no_keys(const dh2::animation::EventView&, std::string& error);
    // True when the BRES root (words +28/+32) carries the exporter's no-key sentinel range INT32_MAX..INT32_MIN.
    static bool has_no_key_sentinel(const std::uint8_t* bres, std::size_t size);
    const std::vector<AnimationMarker>& markers() const { return markers_; }
    std::int32_t start_ms() const { return start_; }
    std::int32_t end_ms() const { return end_; }
    // Cursor is scoped to one clip/action. Actor identity and clip identity belong
    // to the caller; use (generation,cycle,marker.index) as its delivery token.
    // Explicit reentry/restart creates a fresh generation. Restored cursors can
    // resume without restart so already-delivered occurrences are not repeated.
    static bool restart(MarkerCursor&, std::string& error);
    // Commit cursor before returning a detached batch. Consumers may interrupt,
    // restart or change clips while dispatching without mutating this batch.
    // Initial start markers fire once, including advance(0); thereafter (old,new].
    // At a loop boundary, outgoing end markers precede incoming start markers.
    // Failure leaves cursor and output unchanged. Bounded to 1M emissions/call.
    bool advance(MarkerCursor&, std::uint64_t delta_ms, bool loop,
                 std::vector<MarkerOccurrence>& output, std::string& error) const;
private:
    std::vector<AnimationMarker> markers_;
    std::int32_t start_{}, end_{};
    bool loaded_{};
};
}
