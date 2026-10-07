#pragma once
#include "character_skill_info_v1.hpp"
#include "character_property_bindings.hpp"
#include "../engine-ui/hud_text_v1.hpp"
#include "../engine-ui/hud_initialization_v1.hpp"
#include <deque>
#include <memory>
namespace dh2::character {
// One invocation's original VarArgs and returned string lifetimes. The caller
// retains this frame through the complete NativeGetSkillDetails call. A nested
// invocation uses a distinct frame; no global scratch or shared argument clock.
class CharacterHudSkillTextV1 {
 struct Arguments {std::vector<ui::HudTextVariantV1> values;};
 std::vector<std::unique_ptr<Arguments>> arguments_;
 std::deque<std::string> strings_;
 ui::HudTextV1& text_;const ui::HudTextEnvironmentV1& environment_;
 void* context_{};
 // Live actor resolution is mandatory for skill_info/property; implementations
 // must retain all returned state/provider/sheet backing across synchronous Lua.
 bool(*actor_)(void*,std::uintptr_t,skills::State40*&,const skills::SkillInfoServicesV1*&,PropertySheet16&){};
 public:
 CharacterHudSkillTextV1(ui::HudTextV1&,const ui::HudTextEnvironmentV1&,void*,decltype(actor_));
 CharacterHudSkillTextV1(const CharacterHudSkillTextV1&)=delete;
 CharacterHudSkillTextV1& operator=(const CharacterHudSkillTextV1&)=delete;
 // 1 delivered,0 required failure,-1 outside this owned domain. Handles are
 // checked by exact ownership before dereference. No player/savegame/AS fallback.
 int query(const ui::HudInitRequest64&,ui::HudInitResponse32&,std::string&);
};
}
