#pragma once
#include "character_menu_queries_owner_v1.hpp"
#include "hud_startup_callbacks.hpp"
namespace dh2::ui {
// Application callback receiver for original shared/character startup. Borrow
// the existing settings/application/audio/device owner; never a second option
// map. Other callbacks remain explicitly owned by caller's dispatcher.
class AuthoredCharacterApplicationV1 {
 std::shared_ptr<void> owner_;
 HudStartupState48& state_;
 HudStartupServices16 services_;
 struct Invocation;
 static int service(void*,HudStartupState48*,const HudStartupRequest40*,HudStartupResponse16*);
public:
 AuthoredCharacterApplicationV1(std::shared_ptr<void>,HudStartupState48&,HudStartupServices16);
 bool dispatch(const char*,CharacterMenuCallV1&,std::string&);
 static bool owns(const char*)noexcept;
};
}
