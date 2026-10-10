#pragma once
#include "script_execution_control_v96.hpp"
#include <functional>
namespace dh2::player {struct PlayerInfoFieldsV1;}
namespace dh2::android_ui {
struct SourceScriptActorLeavesV96 {
 std::shared_ptr<void> owner; //Independent services; App/World captures weak.
 std::function<bool(std::uintptr_t,bool,std::string&)> set_idle;
 std::function<bool(std::uintptr_t,std::uintptr_t,std::string&)> controller_look_at;
 std::function<bool(std::uintptr_t,const float*,bool,std::string&)> generic_set_position;
 std::function<bool(std::uintptr_t,std::string&)> force_update_position;
 std::function<bool(std::uintptr_t,bool,std::string&)> render_visible;
 std::function<bool(std::uintptr_t,std::string&)> reload_skills;
 std::function<bool(std::uintptr_t,std::int32_t,std::int32_t,std::int32_t,std::string&)> play_animation;
 std::function<bool(std::uintptr_t,std::int32_t,bool&,std::string&)> animation_blocking;
 std::function<bool(std::uintptr_t,std::uintptr_t,bool,bool,bool,std::uint8_t&,bool&,std::string&)> move_actor;
 std::function<bool(std::uintptr_t,std::uint8_t,bool&,std::string&)> move_blocking;
 std::function<bool(std::uintptr_t,std::string&)> disable_zoning,stop_actor;
 std::function<bool(std::uintptr_t,bool,std::string&)> mark_scripted,put_idle;
};
struct SourceScriptUiLeavesV96 {
 // Actual UI/process provider independent of App-owned ScriptManager. All
 // callbacks must capture App/World weakly; no new message/control globals.
 std::shared_ptr<void> owner;
 std::function<bool(const char*,std::string&)> publish_current_name;
 std::function<bool(bool,std::int32_t,std::int32_t,std::string&)> send_script_message;
 std::function<bool(std::string&)> flush_dialogs,flush_achievements,flush_status,flush_online;
 std::function<bool(bool&,std::string&)> dialog_active;
 //Actual DialogMsg C1(0,Data10,Datac,Data8) then same context0 queue,
 //EnqueueMessage(...,0,true). Provider must perform localization/art/AS start.
 std::function<bool(std::int32_t,std::int32_t,std::int32_t,std::int32_t,
  std::int32_t,bool,std::string&)> enqueue_dialog;
 std::function<bool(std::uint8_t,std::string&)> store_controller_global;
 std::function<bool(bool&,std::string&)> force_cutscene_ui; // actual MenuBase::s_igmOpened
 std::function<bool(bool,std::string&)> menu_f4_virtual38;
 std::function<bool(const char*,std::string&)> hud_callback_noargs;
 std::function<bool(std::uint8_t,std::string&)> store_display_hud; // actual AnimController::s_scalingEnabled
 std::function<bool(player::PlayerInfoFieldsV1&,bool,std::string&)> player_set_in_cutscene;
 std::function<bool(player::PlayerInfoFieldsV1&,std::string&)> reset_dead_local_player;
};
// Optional typed decoration point over the native binder's fully composed
// behavior. The manager and scheduler are the exact owners this binder will
// use for parsed receiver binding and execution; existing callers may omit it.
using SourceScriptBehaviorDecoratorV96 = std::function<bool(
 const std::shared_ptr<loader::ScriptManagerOwnerV52>&,
 const loader::ScriptSchedulerServicesV96&,
 loader::ScriptCommandBehaviorV59&,std::string&)>;
// Binds the actual Application V52 manager, genuine PM70 guards, Debug,
// GetOnline/GetDt/process Random plus existing parsed V59 command owners.
// Root supplies its actual UI/process leaves at its owned runtime boundary.
// No parse/C1/Init/StartScript is run by this enrollment.
bool bind_source_campaign_script_execution_v96(const std::shared_ptr<void>& actual_world,
 SourceScriptUiLeavesV96,std::string&,SourceScriptActorLeavesV96={},
 SourceScriptBehaviorDecoratorV96={});
}
