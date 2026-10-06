#pragma once
#include "authored_gameplay_hud_v1.hpp"
namespace dh2::ui {
struct AuthoredStatusDiagnosticV27 {int frame{},frames{};bool visible{},advanced{};float cadence{},remainder{};};
// Actual selected HUD's itemname_text166 subtree, including original last-frame
// onAnimationEnd→NativeStopMessage. No manual queue expiry or movie-wide tick.
class AuthoredStatusTimelineV27 {
 std::uintptr_t movie_{};std::string path_;float remainder_{};
public:
 bool update(SwfMovie&,const AuthoredGameplayHudV1&,std::uint32_t actual_application_dt,
             bool actual_tick,AuthoredStatusDiagnosticV27&,std::string&);
 void release()noexcept{movie_=0;path_.clear();remainder_=0;}
};
}
