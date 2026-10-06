#pragma once
#include "authored_gameplay_hud_v1.hpp"
namespace dh2::ui {
struct AuthoredProgressionHudDiagnosticV23 {int frame{},frames{};bool visible{},advanced{};float cadence{},remainder{};};
// Advance actual authored level-up portrait timeline162 only. Native availability
// and XP-bar lookup remain the existing SAME HUD/Property providers' authority.
class AuthoredProgressionHudV23 {
 std::uintptr_t movie_{};std::string path_;float remainder_{};
public:
 bool update(SwfMovie&,const AuthoredGameplayHudV1&,std::uint32_t application_dt,
  bool application_tick,AuthoredProgressionHudDiagnosticV23&,std::string&);
 void release()noexcept{movie_=0;path_.clear();remainder_=0;}
};
}
