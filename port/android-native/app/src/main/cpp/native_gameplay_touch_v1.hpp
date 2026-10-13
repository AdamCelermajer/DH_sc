#pragma once

#include <array>
#include <cmath>
#include <cstdint>

// Gameplay touch routing shared by the Android MotionEvent bridge and the
// desktop finger path. It reconstructs two source owners:
//
//  * MenuManager::onEvent (0x430ad4) hands every finger index 0..3 to the
//    menus (including the HUD joystick/attack buttons) first. Fingers above 3
//    are consumed without dispatch, and fingers 1..3 are consumed while the
//    skill-drag lock written by NativeSetMultitouch (MenuManager+0x110) is set.
//  * ZoomHandler::onEvent (0x382bdc) is attached to the same touch events
//    afterwards. It only ever sees a finger whose press the menus did not
//    consume, keeps those fingers in a map, pans the camera while the map holds
//    at most one finger and pinch-zooms from its first and last entries once it
//    holds two or more.
//
// A finger which the HUD owns (stick, attack, skill buttons...) therefore never
// reaches the camera, and a camera finger never cancels a HUD finger.
namespace dh2::native::gameplay_touch_v1 {

inline constexpr std::uint32_t kFingerSlots = 4;

struct CameraServices {
    void* context = nullptr;
    // Whole drawable pixels of pan; the fractional remainder stays in the map.
    void (*pan)(void* context, std::int32_t delta_x, std::int32_t delta_y) = nullptr;
    // Drawable-pixel distance between the two pinch fingers, before and after.
    void (*pinch)(void* context, float previous, float current) = nullptr;
    // A pinch takes the fingers away from the SWF cursor that saw their press
    // (the earlier Android gesture cancelled it once at pinch start).
    void (*cancel_ui)(void* context, std::uint32_t slot, float x, float y) = nullptr;
};

class CameraFingerMap {
public:
    // Press of finger `slot`. `camera_free` is false when the menus consumed
    // it, in which case ZoomHandler never learns about the finger.
    void down(std::uint32_t slot, float x, float y, bool camera_free,
              const CameraServices& services) {
        if (slot >= kFingerSlots) return;
        const bool had_finger = fingers_[slot].active;
        fingers_[slot].active = false;
        if (had_finger) rebuild(services, false);
        if (!camera_free) return;
        fingers_[slot] = {true, x, y};
        rebuild(services, true);
    }

    void move(std::uint32_t slot, float x, float y, const CameraServices& services) {
        if (slot >= kFingerSlots || !fingers_[slot].active) return;
        auto& finger = fingers_[slot];
        const float previous_x = finger.x, previous_y = finger.y;
        finger.x = x;
        finger.y = y;
        if (!(x == previous_x && y == previous_y)) apply(slot, previous_x, previous_y, services);
    }

    void up(std::uint32_t slot, const CameraServices& services) {
        if (slot >= kFingerSlots || !fingers_[slot].active) return;
        fingers_[slot].active = false;
        rebuild(services, false);
    }

    void reset() noexcept {
        for (auto& finger : fingers_) finger = {};
        mode_ = Mode::none;
        remainder_x_ = remainder_y_ = 0.0f;
        previous_distance_ = 0.0f;
        first_ = second_ = kFingerSlots;
    }

    std::uint32_t count() const noexcept {
        std::uint32_t total = 0;
        for (const auto& finger : fingers_) total += finger.active ? 1u : 0u;
        return total;
    }
    bool tracks(std::uint32_t slot) const noexcept {
        return slot < kFingerSlots && fingers_[slot].active;
    }
    bool pinching() const noexcept { return mode_ == Mode::pinch; }

private:
    enum class Mode : std::uint8_t { none, pan, pinch };
    struct Finger {
        bool active = false;
        float x = 0.0f, y = 0.0f;
    };

    static float distance(const Finger& a, const Finger& b) noexcept {
        return std::hypot(a.x - b.x, a.y - b.y);
    }

    // Recompute the gesture after the finger set changed: one finger pans from
    // its current position, two or more pinch between the lowest and highest
    // slot (the first and last entries of ZoomHandler's ordered map).
    void rebuild(const CameraServices& services, bool added) {
        std::uint32_t lowest = kFingerSlots, highest = kFingerSlots, total = 0;
        for (std::uint32_t slot = 0; slot < kFingerSlots; ++slot) {
            if (!fingers_[slot].active) continue;
            if (lowest == kFingerSlots) lowest = slot;
            highest = slot;
            ++total;
        }
        remainder_x_ = remainder_y_ = 0.0f;
        if (total == 0) {
            mode_ = Mode::none;
            first_ = second_ = kFingerSlots;
            previous_distance_ = 0.0f;
            return;
        }
        if (total == 1) {
            mode_ = Mode::pan;
            first_ = lowest;
            second_ = kFingerSlots;
            previous_distance_ = 0.0f;
            return;
        }
        const bool new_pinch = mode_ != Mode::pinch || first_ != lowest || second_ != highest;
        mode_ = Mode::pinch;
        first_ = lowest;
        second_ = highest;
        previous_distance_ = distance(fingers_[first_], fingers_[second_]);
        if (added && new_pinch && services.cancel_ui) {
            services.cancel_ui(services.context, first_, fingers_[first_].x, fingers_[first_].y);
            services.cancel_ui(services.context, second_, fingers_[second_].x, fingers_[second_].y);
        }
    }

    void apply(std::uint32_t slot, float previous_x, float previous_y,
               const CameraServices& services) {
        if (mode_ == Mode::pan && slot == first_) {
            remainder_x_ += fingers_[slot].x - previous_x;
            remainder_y_ += fingers_[slot].y - previous_y;
            const auto delta_x = static_cast<std::int32_t>(std::lround(remainder_x_));
            const auto delta_y = static_cast<std::int32_t>(std::lround(remainder_y_));
            remainder_x_ -= static_cast<float>(delta_x);
            remainder_y_ -= static_cast<float>(delta_y);
            if ((delta_x || delta_y) && services.pan) services.pan(services.context, delta_x, delta_y);
        } else if (mode_ == Mode::pinch && (slot == first_ || slot == second_)) {
            const float current = distance(fingers_[first_], fingers_[second_]);
            const float previous = previous_distance_;
            previous_distance_ = current;
            if (previous > 0.0f && services.pinch) services.pinch(services.context, previous, current);
        }
    }

    std::array<Finger, kFingerSlots> fingers_{};
    Mode mode_ = Mode::none;
    float remainder_x_ = 0.0f, remainder_y_ = 0.0f;
    float previous_distance_ = 0.0f;
    std::uint32_t first_ = kFingerSlots, second_ = kFingerSlots;
};

}  // namespace dh2::native::gameplay_touch_v1
