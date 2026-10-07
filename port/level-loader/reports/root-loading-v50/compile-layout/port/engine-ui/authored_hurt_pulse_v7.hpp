#pragma once
#include "swf_movie.hpp"
namespace dh2::ui {
struct AuthoredHurtPulseDiagnosticV7 {
 int outer_frame{}, pulse_frame{}, pulse_frames{};
 float pulse_alpha{}, health_alpha{}, frame_seconds{}, remainder{};
 bool advanced{};
};
// Native targeted-advance adapter: same source Sprite::advance body and actual
// movie cadence, restricted to the verified action-free HurtCorners subtree.
// Outer health selection is exclusively PlayerStatusHud's authority.
class AuthoredHurtPulseV7 {
public:
 bool update(SwfMovie&,std::uint32_t application_dt,bool application_tick,
             AuthoredHurtPulseDiagnosticV7&,std::string&);
 void release() noexcept { player_=0; remainder_=0; }
private:
 std::uintptr_t player_{}; float remainder_{};
};
}
