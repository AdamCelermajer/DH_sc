#pragma once
#include "script_command_receivers_v59.hpp"
#include "../game-data/loot_tables_v2.hpp"
namespace dh2::loader {
struct ScriptExecutionControlServicesV96 {
 std::weak_ptr<void> application;
 std::weak_ptr<ScriptManagerOwnerV52> manager;
 std::function<bool(std::string&)> debug_load;
 std::function<bool(const char*,bool&,std::string&)> debug_switch;
 std::function<bool(std::uint32_t&,std::string&)> application_dt;
 // Actual Random BSS channel/counter borrowed fresh at reached selection.
 std::function<bool(data::LootRandom8V2*&,std::shared_ptr<void>&,std::string&)> random;
};
// Compose into the SAME original behavior before C1, or bind_execution_v96
// on already parsed actual receivers. Existing Init and other Execute bodies
// remain intact. This contributes complete ExecScript/Wait/Empty bodies.
ScriptCommandBehaviorV59 script_execution_control_v96(
 ScriptCommandBehaviorV59 existing,ScriptExecutionControlServicesV96);
// Attach behavior to the actual parsed receivers without reconstructing them.
bool bind_parsed_script_execution_v96(ScriptManagerOwnerV52&,
 ScriptCommandBehaviorV59,std::string&);
}
