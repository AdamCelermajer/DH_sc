#include "status_message_v1.hpp"

#include <cmath>

namespace dh::foundation::hud_panels {
namespace {
struct StatusFrame { float alpha, scale_y, ty, tx; }; // twips for ty/tx

#include "status_message_timeline_v1.inc"

using Matrix = std::array<float, 6>; // a b c d tx ty (twips)

Matrix multiply(const Matrix& p, const Matrix& c) noexcept { // p * c (child first)
    return {p[0] * c[0] + p[2] * c[1], p[1] * c[0] + p[3] * c[1],
            p[0] * c[2] + p[2] * c[3], p[1] * c[2] + p[3] * c[3],
            p[0] * c[4] + p[2] * c[5] + p[4], p[1] * c[4] + p[3] * c[5] + p[5]};
}

// DefineFont3 font 7 (Fontin SmallCaps) raw ascent / em, from the HUD font table (main.cpp field.font_metrics 911/1024).
constexpr float font_ascent_em = 911.f / 1024.f;
} // namespace

int StatusMessageQueueV1::scroll_frame() noexcept { return status_scroll_frame; }
int StatusMessageQueueV1::frame_count() noexcept { return int(sizeof(status_frames) / sizeof(status_frames[0])); }
float StatusMessageQueueV1::seconds_per_frame() noexcept { return 1.f / status_frames_per_second; }

void StatusMessageQueueV1::enqueue(std::string text, std::uint32_t rgb) {
    queue_.push_back({std::move(text), rgb & 0xFFFFFFu});
    if (queue_.size() == 1) { frame_ = status_scroll_frame; remainder_ms_ = 0; }
}

void StatusMessageQueueV1::clear() noexcept {
    queue_.clear();
    frame_ = 0;
    remainder_ms_ = 0;
}

void StatusMessageQueueV1::advance(std::uint32_t dt_ms) noexcept {
    if (queue_.empty()) return;
    const double cadence_ms = 1000.0 * double(seconds_per_frame());
    remainder_ms_ += double(dt_ms);
    if (remainder_ms_ < cadence_ms) return;
    remainder_ms_ = std::fmod(remainder_ms_ - cadence_ms, cadence_ms);
    if (++frame_ < frame_count() - 1) return;
    // Last authored frame: onAnimationEnd -> NativeStopMessage pops this message and starts the next one.
    queue_.pop_front();
    frame_ = status_scroll_frame;
    remainder_ms_ = 0;
}

bool StatusMessageQueueV1::view(StatusMessageViewV1& out) const {
    out = {};
    if (queue_.empty()) return false;
    const auto& message = queue_.front();
    const auto& frame = status_frames[frame_];
    const Matrix root{status_root_matrix[0], status_root_matrix[1], status_root_matrix[2],
                      status_root_matrix[3], status_root_matrix[4], status_root_matrix[5]};
    const Matrix container{1.f, 0.f, 0.f, frame.scale_y, frame.tx, frame.ty};
    const Matrix info{status_info_matrix[0], status_info_matrix[1], status_info_matrix[2],
                      status_info_matrix[3], status_info_matrix[4], status_info_matrix[5]};
    const auto chain = multiply(multiply(root, container), info);
    // Field-local point: horizontal centre of the authored box; first baseline = text area top (box ymin + the
    // 2 px / 40 twip gutter = local 0) + font ascent.
    const float local_x = (status_field_bounds_twips[0] + status_field_bounds_twips[1]) * .5f;
    const float local_y = font_ascent_em * status_font_height_twips;
    out.text = &message.text;
    out.rgb = message.rgb;
    out.alpha = frame.alpha;
    out.centre_x = (chain[0] * local_x + chain[2] * local_y + chain[4]) / 20.f;
    out.baseline_y = (chain[1] * local_x + chain[3] * local_y + chain[5]) / 20.f;
    out.y_scale = chain[3];
    out.font_px = status_font_height_twips / 20.f;
    out.frame = frame_;
    return true;
}

} // namespace dh::foundation::hud_panels
