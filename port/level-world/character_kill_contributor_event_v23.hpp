#pragma once
#include "character_ai_events.hpp"
#include "character_ais_kill_vm_v23.hpp"
#include <array>
#include <functional>
namespace dh2::character {
struct KillContributorServicesV23 {
 std::shared_ptr<void> actual_receiver;
 // Whole SAME FSM delivery only when source forced/locked/global-block gates
 // choose it. Caller refreshes those SAME fields before raise.
 std::function<bool(std::uint32_t,std::uintptr_t,const dh2_script_callback_scope*,std::string&)> state_event;
};
// Completion of recovered callable metadata only; borrows SAME AI state,
// ScriptOwner and FSM. No new AI flags, active AIS, target or life fields.
class CharacterKillContributorEventV23 {
 AIEventState64& ai_;ScriptOwner* scripts_{};ScriptOwnerV2* player_scripts_{};KillContributorServicesV23 services_;
 std::array<std::uintptr_t,51> keys_;const std::uintptr_t* previous_{};
 const dh2_script_callback_scope* scope_{};std::string error_;
 static int service(void*,AIEventState64*,const AIEventRequest40*,std::uint32_t*);
 void keys();
public:
 CharacterKillContributorEventV23(AIEventState64&,ScriptOwner&,KillContributorServicesV23);
 CharacterKillContributorEventV23(AIEventState64&,ScriptOwnerV2&,KillContributorServicesV23);
 ~CharacterKillContributorEventV23();
 CharacterKillContributorEventV23(const CharacterKillContributorEventV23&)=delete;
 bool raise(std::uintptr_t actual_killed,const dh2_script_callback_scope*,std::string&);
};
}
