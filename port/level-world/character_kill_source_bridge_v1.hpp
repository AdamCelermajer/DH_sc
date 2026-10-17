#pragma once

#include "player_kill_continuation_v1.hpp"

#include <cstdint>
#include <string>

namespace dh2::character_kill_source_bridge_v1 {

// These are ingress reasons for the same canonical Character::Ctrl_Kill.
// Death animation completion is deliberately not a kill ingress.
enum class Source : std::uint32_t {
    player_melee=1,player_skill=2,npc_attack=3,death_visual_completion=4
};

struct Request {
    std::uintptr_t character=0,killer=0;
    std::uint32_t forced=0;
    Source source=Source::player_melee;
};

struct Result {
    Source source=Source::player_melee;
    player_kill_continuation_v1::Result ctrl{};
};

using Status=player_kill_continuation_v1::Status;

// Non-owning ingress adapter. A caller constructs one bridge per canonical
// Character and passes its one retained CtrlCaller to melee, skill and NPC
// damage paths. The CtrlCaller keeps the fresh outer IsDead -> full Kill ->
// RaiseEvent(2,killer) latch; its Kill provider must call the matching inner
// Character::Kill/character_kill_death_tail_v1::Runtime. Repeated ingress
// calls therefore observe that same source latch and cannot replay loot, XP,
// objective events or event 2. Visual completion remains a separate owner.
class Runtime {
    std::uintptr_t character_;
    player_kill_continuation_v1::CtrlCaller* caller_;
    bool busy_=false;
public:
    Runtime(std::uintptr_t character,player_kill_continuation_v1::CtrlCaller&);
    Runtime(const Runtime&)=delete;
    Runtime& operator=(const Runtime&)=delete;
    Status dispatch(const Request&,Result*,std::string& error);
};

} // namespace dh2::character_kill_source_bridge_v1
