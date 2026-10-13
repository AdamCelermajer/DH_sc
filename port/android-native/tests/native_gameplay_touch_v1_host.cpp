#include "../app/src/main/cpp/native_gameplay_touch_v1.hpp"

#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {
using namespace dh2::native::gameplay_touch_v1;

unsigned checks = 0;
void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
    ++checks;
}

struct Recorder {
    std::vector<std::pair<int, int>> pans;
    std::vector<std::pair<float, float>> pinches;
    std::vector<unsigned> cancelled;
    CameraServices services() {
        return {this,
                [](void* raw, std::int32_t dx, std::int32_t dy) {
                    static_cast<Recorder*>(raw)->pans.emplace_back(dx, dy);
                },
                [](void* raw, float previous, float current) {
                    static_cast<Recorder*>(raw)->pinches.emplace_back(previous, current);
                },
                [](void* raw, std::uint32_t slot, float, float) {
                    static_cast<Recorder*>(raw)->cancelled.push_back(slot);
                }};
    }
};

// Stick on finger 0 and attack on finger 1 are both consumed by the HUD, so
// ZoomHandler's map stays empty and neither finger pans or pinches the camera.
void hud_fingers_never_reach_the_camera() {
    Recorder recorder;
    const auto services = recorder.services();
    CameraFingerMap map;
    map.down(0, 100, 600, false, services);   // joystick press, consumed by the HUD
    map.down(1, 1100, 560, false, services);  // attack press, consumed by the HUD
    check(map.count() == 0 && !map.tracks(0) && !map.tracks(1), "HUD fingers must not enter the camera map");
    map.move(0, 160, 560, services);
    map.move(1, 1090, 560, services);
    check(recorder.pans.empty() && recorder.pinches.empty() && recorder.cancelled.empty(),
          "HUD fingers must not pan, pinch or cancel the other finger");
    map.up(1, services);
    map.up(0, services);
    check(recorder.pans.empty() && recorder.pinches.empty() && recorder.cancelled.empty(),
          "lifting HUD fingers must not touch the camera");
}

// A free finger pans while the stick finger keeps driving the HUD.
void free_finger_pans_beside_a_held_stick() {
    Recorder recorder;
    const auto services = recorder.services();
    CameraFingerMap map;
    map.down(0, 100, 600, false, services);  // stick
    map.down(1, 700, 300, true, services);   // free finger on the world
    check(map.count() == 1 && map.tracks(1) && !map.pinching(), "one free finger is a pan, not a pinch");
    check(recorder.cancelled.empty(), "a lone free finger must not cancel the stick's cursor");
    map.move(1, 730, 290, services);
    check(recorder.pans.size() == 1 && recorder.pans[0].first == 30 && recorder.pans[0].second == -10,
          "pan delta is the finger's drawable-pixel displacement");
    map.move(0, 120, 600, services);  // the stick finger is not tracked by the camera
    check(recorder.pans.size() == 1, "moving the HUD finger must not pan the camera");
}

void sub_pixel_pan_keeps_its_remainder() {
    Recorder recorder;
    const auto services = recorder.services();
    CameraFingerMap map;
    map.down(2, 10, 10, true, services);
    map.move(2, 10.4f, 10, services);
    map.move(2, 10.8f, 10, services);
    check(recorder.pans.size() == 1 && recorder.pans[0].first == 1, "fractional pan accumulates to a whole pixel");
    map.move(2, 10.8f, 10, services);
    check(recorder.pans.size() == 1, "an unchanged position is not a pan");
}

void two_free_fingers_pinch_and_hand_back_to_pan() {
    Recorder recorder;
    const auto services = recorder.services();
    CameraFingerMap map;
    map.down(0, 300, 300, true, services);
    map.down(1, 500, 300, true, services);
    check(map.pinching() && map.count() == 2, "two free fingers pinch");
    check(recorder.cancelled.size() == 2 && recorder.cancelled[0] == 0 && recorder.cancelled[1] == 1,
          "a pinch takes both fingers from their SWF cursors exactly once");
    map.move(1, 560, 300, services);
    check(recorder.pinches.size() == 1 && std::fabs(recorder.pinches[0].first - 200.f) < 1e-3f &&
              std::fabs(recorder.pinches[0].second - 260.f) < 1e-3f,
          "pinch reports drawable-pixel distances");
    map.up(0, services);
    check(!map.pinching() && map.tracks(1), "lifting one pinch finger resumes a one-finger pan");
    const auto pans = recorder.pans.size();
    map.move(1, 570, 310, services);
    check(recorder.pans.size() == pans + 1 && recorder.pans.back().first == 10 && recorder.pans.back().second == 10,
          "the remaining finger pans from where it is, with no jump");
}

// Source ZoomHandler pinches between the first and last map entries.
void three_fingers_pinch_between_the_extremes() {
    Recorder recorder;
    const auto services = recorder.services();
    CameraFingerMap map;
    map.down(0, 100, 100, true, services);
    map.down(3, 400, 100, true, services);
    map.down(1, 250, 400, true, services);
    check(map.pinching() && map.count() == 3, "three free fingers still pinch");
    map.move(1, 250, 450, services);  // the middle finger does not drive the pinch
    check(recorder.pinches.empty(), "the middle finger of three does not change the pinch distance");
    map.move(3, 500, 100, services);
    check(recorder.pinches.size() == 1 && std::fabs(recorder.pinches[0].second - 400.f) < 1e-3f,
          "the outer fingers measure the pinch");
}

void stale_and_out_of_range_fingers_are_ignored() {
    Recorder recorder;
    const auto services = recorder.services();
    CameraFingerMap map;
    map.move(1, 1, 1, services);
    map.up(2, services);
    map.down(4, 1, 1, true, services);
    check(map.count() == 0 && recorder.pans.empty(), "unknown fingers and slots above 3 are consumed silently");
    map.down(0, 5, 5, true, services);
    map.down(0, 50, 50, true, services);  // duplicate press replaces the old entry
    check(map.count() == 1, "a repeated press keeps one map entry");
    map.reset();
    check(map.count() == 0 && !map.pinching(), "reset empties the map");
}
}  // namespace

int main() {
    hud_fingers_never_reach_the_camera();
    free_finger_pans_beside_a_held_stick();
    sub_pixel_pan_keeps_its_remainder();
    two_free_fingers_pinch_and_hand_back_to_pan();
    three_fingers_pinch_between_the_extremes();
    stale_and_out_of_range_fingers_are_ignored();
    std::printf("PASS: gameplay multi-touch camera finger map (%u assertions)\n", checks);
    return 0;
}
