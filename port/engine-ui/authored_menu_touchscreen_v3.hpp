#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <string>
namespace dh2::ui {
struct AuthoredMenuQueuedTouchV3 {
 std::uint32_t event{};std::int16_t x{},y{};std::int32_t id{};
};
// Actual TouchScreenBase constructor-owned scalar/queue projection. Derived
// platform input and nonempty ProcessEvents must compose their real providers.
// Android's direct GameSWF mouse transport does not pretend to be that queue.
class AuthoredMenuTouchScreenV3 {
 std::int16_t width_,height_;
 float scale_{1.f};std::int32_t orientation_{};
 std::array<std::uint8_t,8> active28_{};
 std::array<AuthoredMenuQueuedTouchV3,16> events_{};
 std::uint32_t head1a0_{},tail1a4_{};
public:
 AuthoredMenuTouchScreenV3(std::int16_t width,std::int16_t height):width_(width),height_(height){}
 std::array<std::uint8_t,8>& active_source_bytes()noexcept{return active28_;}
 std::array<AuthoredMenuQueuedTouchV3,16>& source_events()noexcept{return events_;}
 std::uint32_t& source_head()noexcept{return head1a0_;}
 std::uint32_t& source_tail()noexcept{return tail1a4_;}
 std::int32_t& orientation()noexcept{return orientation_;}
 float& scale()noexcept{return scale_;}
 std::int16_t width()const noexcept{return width_;}
 std::int16_t height()const noexcept{return height_;}
 bool empty()const noexcept{return head1a0_==tail1a4_;}
 // Application.ResetTouch snapshots getTouchIDList before delivering each
 // virtual release. IDs are actual source slot indices, in ascending order.
 bool reset(const std::function<bool(std::int16_t,std::int16_t,std::int32_t,std::string&)>& release,std::string&);
 bool process(const std::function<bool(AuthoredMenuTouchScreenV3&,std::string&)>& whole_nonempty,std::string&);
};
}
