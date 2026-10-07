#pragma once
#include "character_kill.hpp"
#include "../game-data/combat_application.hpp"
#include <string>
namespace dh2::character {
// Exact source metadata/projection is supplied by its existing owner. This
// owner does not invent OID/template/master/quest flags or manufacture a Level.
// Both property graph and canonical life survive all synchronous callbacks.
class CombatCtrlKillOwnerV1 {
 KillActor56& actor_;data::CombatActorState& life_;KillWorld16& world_;
 KillServices16 services_;bool delivering_{};std::uint8_t published_dead_{};
 static int invoke(void*,KillActor56*,const KillRequest56*,KillResponse16*);
 void publish();
public:
 CombatCtrlKillOwnerV1(KillActor56&,data::CombatActorState&,KillWorld16&,KillServices16);
 CombatCtrlKillOwnerV1(const CombatCtrlKillOwnerV1&)=delete;
 CombatCtrlKillOwnerV1& operator=(const CombatCtrlKillOwnerV1&)=delete;
 bool matches(const data::PropertyView*,const data::CombatActorState*)const noexcept;
 int ctrl_kill(KillResult24&,std::uintptr_t actual_attacker,std::uint32_t force,std::string&);
};
struct CombatCtrlKillResultV1 {
 data::MonsterApplication application{};KillResult24 kill{};
 bool kill_reached{},leech_reached{},complete{};
};
// Versioned successor to the old direct-owner offline application core. Same
// bounded source domain: offline single-player, direct HP, default debug facts,
// no gold-damage mask/critical skill. It adds WHOLE required Ctrl_Kill delivery
// at the health Kill point, not after an artificial dead prefix. Original
// status requests/FX/text/audio/player-achievement/AI tails remain distinct.
// 1 core complete,-1 malformed atomic,-2 required wholeKill failure. Prefix
// output is committed on failure; HP0 is not evidence of a completed Kill.
int combat_apply_ctrl_kill_v1(CombatCtrlKillResultV1&,
 const data::MonsterApplicationRequest&,std::uintptr_t actual_attacker,
 CombatCtrlKillOwnerV1&,bool player_defender,bool actual_player_idle,std::string&);
}
