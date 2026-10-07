#pragma once
#include "canonical_level_context_v1.hpp"
#include <functional>
#include <utility>
namespace dh2::loader {
// Reached leaves of original Level.Update3f8350..3f8838. Each provider is an
// actual service owner/body, not a readiness flag or replacement actor loop.
struct LevelGameplayServicesV66 {
 std::shared_ptr<void> actual_application,actual_object_manager,provider;
 std::function<bool(std::string&)> prepare_debug_hud;
 std::function<bool(std::string&)> debug_load;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(bool&,std::string&)> in_game_view;
 std::function<bool(std::int32_t,std::int32_t,bool,std::string&)> start_script;
 std::function<bool(std::uintptr_t&,std::string&)> local_character;
 std::function<bool(std::uintptr_t,bool,std::string&)> save_update;
 std::function<bool(std::string&)> player_manager_update;
 std::function<bool(std::uintptr_t&,std::string&)> player_manager714;
 std::function<bool(std::string&)> execute_all_scripts,game_events_update,set_ambient,
  physical_update,handle_ai_groups,inc_ai_queue,update_zoom,update_fx;
 std::function<bool(double,std::string&)> update_spawn_groups;
 std::function<bool(float,std::string&)> update_objects;
 // Source field12c==actual override global updates that extra receiver first;
 // the regular128 camera pair then executes unconditionally.
 std::function<bool(std::uintptr_t&,std::string&)> camera_override_global;
 std::function<bool(std::string&)> camera_override_update,camera_update,camera_node_update_false,
  update_camera_fog,update_render_flags;
 // This is the exact3f8774..3f87cc suffix on SAME Level144 and actual sound
 // manager32/31: source PlayMusic+SetInSafeZoneMusic+ambient Play. It must
 // perform its own reached byte144=1 store only after successful source calls.
 std::function<bool(std::string&)> start_level_sound;
 std::function<bool(std::string&)> update_listener,update_dynamic_fog;
 // IsAutoLoadingLevels positive branch alone owns source .data wait timer,
 // counters, random selection, Save/Level transition. Never emulate it here.
 std::function<bool(std::string&)> auto_level_loading;
};
class LevelGameplayUpdateV66 final {
 std::shared_ptr<CanonicalLevelContextV1> level_;LevelGameplayServicesV66 services_;
 bool busy_{},failed_{};std::string error_,required_;
 bool reject(const char*);
 template<class F,class...A>bool call(const char* name,const F& fn,A&&...args){
  if(!fn||!fn(std::forward<A>(args)...,error_)||failed_)return reject(name);return true;
 }
 bool switch_value(const char*,bool&);
 bool body();
public:
 LevelGameplayUpdateV66(std::shared_ptr<CanonicalLevelContextV1> level,LevelGameplayServicesV66 s):
  level_(std::move(level)),services_(std::move(s)){}
 bool update(const std::shared_ptr<CanonicalLevelContextV1>&,bool force,std::string&);
 const std::string& required_service()const noexcept{return required_;}
};
}
