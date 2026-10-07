#pragma once
#include "character_script_commands.hpp"
#include "character_controller_commands.hpp"
#include "character_path_commands.hpp"
#include "character_heading_owner_v1.hpp"
#include "actor_runtime.hpp"
#include "floors.hpp"
#include "navigation_producers.hpp"
namespace dh2::character {
struct NpcScriptCommandBorrowV1 {
 std::uintptr_t identity{};ControllerCommandState32* controller{};
 actor::RuntimeState* runtime{};TargetState48* target{};
 const float* actual_position{};const float* source_vec3_k{};
 const std::uint8_t* static84{};const std::uint32_t* path_limit26c{};
 CharacterHeadingOwnerV1* heading{};floors::World* floors{};
};
struct NpcScriptCommandServicesV1 {
 void* context{};
 int(*remote)(void*,bool&){};
 int(*position)(void*,std::uintptr_t,const float*&){};
 int(*look_vector)(void*,const float*&){};
 // Full SAME actual v2Controller Cmd_Attack, including source gates/network.
 int(*attack)(void*,std::uintptr_t,const dh2_script_callback_scope*){};
 int(*number)(void*,const dh2_script_value*,float*){};
 // Whole original FindPath Debug/profile + SearchGraph Debug/GetModule.
 int(*path_policy)(void*,std::uint32_t*){};
};
class NpcScriptCommandsV1 {
 NpcScriptCommandBorrowV1 b_;NpcScriptCommandServicesV1 s_;
 ScriptCommandState48 state_{};ScriptCommandBindings40 bindings_{};
 std::vector<navigation::PathSegment> segments_;
 std::vector<std::uint32_t> route_ids_;
 std::string error_;navigation::RouteResult route_result_{};
 bool pf_ready_{},pf_constructed_{};
 static int service(void*,ScriptCommandState48*,const ScriptCommandRequest40*,const float**);
 static int number(void*,const dh2_script_value*,float*);
 static int find_path(void*,const PathToRequest32*,std::uint32_t*);
 int path_to(const float*);
 int deliver(const ScriptCommandRequest40&,const float**);
 bool permitted()const noexcept;
public:
 NpcScriptCommandsV1(NpcScriptCommandBorrowV1,NpcScriptCommandServicesV1);
 ScriptCommandBindings40& bindings()noexcept{return bindings_;}
 // Must run before EVERY Lua command (including HasPath) and after mutation.
 bool refresh();
 // Host lifetime boundary, not an original gameplay command. Call before the
 // old floor/registry is retired; retires the route without dereferencing it.
 bool suspend_navigation();
 // After old registry retirement/reset, adopt the actual replacement floor.
 // PF stays unavailable until initialize_pf supplies fresh genuine producers.
 bool rebind_floor(floors::World*);
 const navigation::RouteResult& last_route()const noexcept{return route_result_;}
 const std::string& error()const noexcept{return error_;}
 floors::World* floor_borrow()const noexcept{return b_.floors;}
 bool navigation_ready()const noexcept{return pf_ready_;}
 // Genuine source PF initialization/update over same runtime PFObject. Caller
 // supplies actual bounds/body producer and shared registry. InitObject's
 // source bool is SAME static84, not the separate flying capability bit.
 bool initialize_pf(navigation::ObstacleRegistry&,const navigation::ProducerFields& actual_fields);
};
}
