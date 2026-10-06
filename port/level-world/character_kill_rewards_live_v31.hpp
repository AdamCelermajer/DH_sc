#pragma once
#include "world_loot_gameplay_v23.hpp"
#include "character_progression_world_v23.hpp"
#include "character_kill_production_v23.hpp"
namespace dh2::character {
// Same source owners are retained by the application's provider lease; this
// connector allocates no pool, Gear, RNG, player map or progression ledger.
class CharacterKillRewardsLiveV31 {
 WorldLootGameplayV23& loot_;CharacterProgressionWorldV23& progression_;
 std::shared_ptr<void> providers_;
 std::function<bool(KillActor56&,const KillRequest56&,KillResponse16&,std::string&)> remaining_;
public:
 CharacterKillRewardsLiveV31(WorldLootGameplayV23& loot,CharacterProgressionWorldV23& progression,
  std::shared_ptr<void> providers,
  std::function<bool(KillActor56&,const KillRequest56&,KillResponse16&,std::string&)> remaining)
  :loot_(loot),progression_(progression),providers_(std::move(providers)),remaining_(std::move(remaining)){}
 bool route(KillActor56&,const KillRequest56&,KillResponse16&,std::string&);
 // Caller retains this connector before installing the whole production
 // service table. This does not publish Hit8 or assert deeper source readiness.
 bool attach(CharacterKillProductionServicesV23&,std::string&);
};
}
