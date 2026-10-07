#pragma once
#include "character_kill_live_v21.hpp"
#include "character_kill_contributor_event_v23.hpp"
#include "character_kill_level_events_v23.hpp"
namespace dh2::character {
struct CharacterKillProductionServicesV23 {
 std::shared_ptr<void> providers;
 // Same live callback capability, nullable outside a script callback.
 std::function<const dh2_script_callback_scope*()> scope;
 // Re-read the same AI active/forced/locked/global fields before event4.
 std::function<bool(std::uintptr_t,std::string&)> refresh_contributor;
 // Actual event2 selected AIS/FSM/death-animation delivery. No broadcast.
 std::function<bool(std::uintptr_t,std::uintptr_t,const dh2_script_callback_scope*,std::string&)> died;
 // Positive loot, XP, trophy, online, constants and other required services.
 // No default success is provided for any unbound source service.
 std::function<bool(KillActor56&,const KillRequest56&,KillResponse16&,std::string&)> remaining;
};
// Ordered composition around the existing whole Kill/CtrlKill owner. Actor
// fields/health/handles and the Level EventManager remain their sole owners.
class CharacterKillProductionV23 {
 CharacterKillLevelEventsV23 level_;
 CharacterKillProductionServicesV23 services_;
 std::map<std::uintptr_t,std::shared_ptr<CharacterKillContributorEventV23>> contributors_;
 CharacterKillLiveWorldV21 world_;
 bool route(KillActor56&,const KillRequest56&,KillResponse16&,std::string&);
public:
 CharacterKillProductionV23(skills::CharacterWorldRuntimeV1&,player::PlayerManagerOwnerV1&,
  KillWorld16&,KillLevelProviderV23,CharacterKillProductionServicesV23);
 bool add(CharacterKillLiveBorrowV21,std::shared_ptr<CharacterKillContributorEventV23>,std::string&);
 bool forget_after_unpublication(std::uintptr_t,std::string&);
 int command(std::uintptr_t controller,std::uintptr_t character,std::uintptr_t attacker,std::uint32_t force);
 const std::string& error()const noexcept{return world_.error();}
 const KillResult24* result(std::uintptr_t id)const noexcept{return world_.result(id);}
};
}
