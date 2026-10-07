#pragma once
#include "menu_manager_update_v58.hpp"
#include "hud_manager.hpp"
#include "hud_attack_control_v46.hpp"
#include "authored_joystick_v1.hpp"
#include "menu_manager_prefix_v62.hpp"
#include "../level-world/event_manager_owner_v12.hpp"
#include <array>
#include <vector>
namespace dh2::ui {
using MenuCurrentLevelV62=std::function<bool(MenuLevelBorrowV58&,std::string&)>;
//31f684 really queries twice on its positive branch. Do not freeze the first
// Level while delivering the second query or infer198 from a drawn scene.
bool menu_currently_in_game_view_v62(const MenuCurrentLevelV62&,bool&,std::string&);
//One actual process InfoHUD owner. C1 writes initialized0/timer-1/renderNULL,
//but does not write cached target0; that is first Update's source store.
class MenuInfoHudOwnerV62 {
 HudManagerState state_{nullptr,0,-1,0};
public:
 HudManagerState& fields()noexcept{return state_;}
 bool run(HudManagerEntry,const HudManagerServices&,std::string&);
 //Caller supplies genuine LoadMenu(3)431f3c store, not discovery/activation.
 void source_render_store_v62(std::uintptr_t render)noexcept{state_.render_fx=render;}
};
struct HudControlsServicesV62 {
 HudAttackServicesV46 prefix;
 AuthoredJoystickServicesV1 joystick;
 std::function<bool(std::uintptr_t,const float[2],float[3],bool&,std::string&)> screen_hit525884;
 //Position149c precedes its callback(false), then a fresh nullable149c query
//before SetActive(true). This leaf must preserve that entire source sequence.
 std::function<bool(std::uintptr_t,const float[3],std::string&)> click_effect149c;
 std::function<bool(std::uintptr_t,const float[3],std::string&)> cmd_head_to4054e4;
 std::function<bool(const events::EventBorrowV12&,std::int16_t&,std::int16_t&,std::uint8_t&,std::string&)> pointer_event_fields;
 std::function<bool(bool&,std::string&)> world_map_visible;
};
//Whole41a780, borrows SAME V46 held fields and SAME joystick fields. No second
//controls object or input dispatch. Missing positive leaves fail explicitly.
bool hud_controls_update_v62(HudAttackHeldFieldsV46&,AuthoredJoystickStateV1&,
 const HudControlsServicesV62&,std::string&);
struct MenuMapIconV62 {
 std::shared_ptr<void> weak_owner;
 std::function<bool(std::uintptr_t&,std::string&)> borrow;
 std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)> parent;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> remove;
};
//The18 vectors are the actual constructor-empty icon-family storage. Original
//AddIcon producers must append real weak receivers; clearing does not Find
//replacement icons by name or delete unrelated display-list objects.
class MenuMapIconsV62 {
 std::array<std::vector<MenuMapIconV62>,18> icons_;
 bool clearing_{};
public:
 using Parent=std::function<bool(std::uintptr_t,std::uintptr_t&,std::string&)>;
 using Remove=std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)>;
 bool source_append(std::uint32_t,MenuMapIconV62,std::string&);
 bool clear_all(const Parent&,const Remove&,std::string&);
};
struct MenuMapServicesV62 {
 std::shared_ptr<void> actual_manager;
 std::function<bool(std::string&)> debug_load;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(AuthoredMenuFieldsV1&,AuthoredCharacterStateV1&,MenuStackCharacterV1&,std::string&)> register_menu;
 std::function<bool(AuthoredMenuFieldsV1&,AuthoredCharacterStateV1&,const char*,AuthoredMenuWeakCharacterV59&,std::string&)> find_from;
};
class MenuMapOwnerV62 {
 AuthoredMenuFieldsV1 fields_;
 AuthoredCharacterStateV1 state_;
 MenuStackCharacterV1 projection_{};
 std::array<AuthoredMenuWeakCharacterV59,3> caches_f8_;
 //Source C1's two self-linked empty heads and exact float-word stores.
 struct Head{const void* first{this};const void* last{this};};
 [[maybe_unused]] Head c4_,cc_;
 [[maybe_unused]] std::array<std::uint32_t,9> words_d4_{{0xbf800000,0xbf800000,0xbf800000,0x3f800000,0x3f800000,0x3f800000,0,0,0}};
 [[maybe_unused]] std::uint32_t word1dc_{0x3f998000},word1e0_{0x3ecc0000},word1e8_{};
 [[maybe_unused]] std::uint8_t byte1e4_{};
 MenuMapIconsV62 icons_;
 bool attempted_{},constructed_{};std::string failure_;
public:
 MenuMapOwnerV62();
 bool construct(MenuBaseFSRegistryV62&,const MenuMapServicesV62&,std::string&);
 bool source_initialize_v67(const MenuMapServicesV62&,std::string&);
 AuthoredMenuFieldsV1& fields()noexcept{return fields_;}
 AuthoredCharacterStateV1& state()noexcept{return state_;}
 MenuMapIconsV62& icons()noexcept{return icons_;}
 bool constructed()const noexcept{return constructed_;}
 std::uint32_t source_map_camera1e8_v119()const noexcept{return word1e8_;}
 const std::string& failure()const noexcept{return failure_;}
};
}
