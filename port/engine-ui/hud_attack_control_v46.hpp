#pragma once
#include <cstdint>
#include <string>
namespace dh2::ui {
// One source HUDControls component, embedded once in the retained HUD owner.
// C1 41af4c /41af78..80: byte9=0, pending7c/80=-1, byte84=0.
struct HudAttackHeldFieldsV46 {
 std::uint8_t held9{},consumed84{};
 std::int32_t pending_x7c{-1},pending_y80{-1};
};
struct HudAttackActorBorrowV46 {
 std::uintptr_t character{},controller378{};
 const std::uintptr_t* object_of_interest14a4{};
 std::uint8_t* click413{};
};
struct HudAttackServicesV46 {
 void* context{};
 bool(*input_blocked30)(void*,std::uint8_t&,std::string&){};
 bool(*store_controller_blocked)(void*,std::uint8_t,std::string&){};
 // Genuine Application current-Level NULL is allowed; nonNULL requires SAME198.
 bool(*current_level)(void*,std::uintptr_t&,const std::uint8_t*&,std::string&){};
 bool(*cached_root658)(void*,std::uintptr_t&,std::string&){};
 bool(*cached_chars8)(void*,std::uint8_t&,std::string&){};
 bool(*init_cached_chars)(void*,std::string&){};
 bool(*local_player)(void*,std::int32_t,bool,HudAttackActorBorrowV46&,std::string&){};
 bool(*use_ooi)(void*,std::uintptr_t,std::uintptr_t,std::string&){};
 bool(*attack)(void*,std::uintptr_t,std::uintptr_t,std::string&){};
};
// Call only for the actual cached attack receiver +88, after original outer
// customization routing. Source4 press,6 release,7 release-outside/cancel.
bool hud_attack_event_v46(HudAttackHeldFieldsV46&,std::uint32_t source_event,bool& consumed,std::string&);
// Whole source held-control prefix of Update41a780, ending at41a840 before
// the independently owned joystick/world-touch continuation. Source controller
// gates run inside real Cmd methods; do not pre-clear on skill state6.
bool hud_attack_update_v46(HudAttackHeldFieldsV46&,std::uint8_t& same_joystick_active_a,
 const HudAttackServicesV46&,std::string&);
// Separately exposed original 41a818..840/9d8..9e4 branch for oracle replay.
bool hud_attack_dispatch_v46(HudAttackHeldFieldsV46&,const HudAttackActorBorrowV46&,
 const HudAttackServicesV46&,std::string&);
}
