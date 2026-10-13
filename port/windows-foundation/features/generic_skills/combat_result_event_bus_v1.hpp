#pragma once
// v21.8 es-passive: player-side OnTargetHit / OnTargetMissed listeners (Lua StealAIEvent).
//
// Original (IDA offline export, libDungeonHunter2.so pseudocode-all.c):
//   AISDefault::InitVCB 0x3dc7d8: flag word (AI+184) gets 0x800 when the AI VFTable has "OnTargetHit" and 0x1000 when it has
//     "OnTargetMissed" (LuaScript::IsInVFTable). AISPlayer::InitVCB 0x3dd884 adds 0x400 for "OnKill".
//   AISDefault::OnCombatResults(attacker, target, result) 0x3dc9a8: if (result[24] & 3) != 0 it calls the Lua
//     "OnTargetMissed" when 0x1000 is set; otherwise it calls "OnTargetHit" when 0x800 is set. Arguments: (attacker, target).
//   Commons _commons.luac StealAIEvent: listeners share one proxy per event name in the VFTable and run in registration
//     order, each with its own skill id (SKILL_ID is set per listener).
//   CharAI::OnCombatResults 0x3d0da4 forwards to the AI object (vtable +180). Port model of the receiver fan-out (attacker
//     receiver first, then target receiver): character_combat_results_ai_v1.hpp. The IDA caller of 0x3d0da4 is NOT located
//     (no direct xref, no 4-argument virtual call found in the export); the receiver order comes from the port model.
//
// Port: a bus keyed by receiver actor. The CombatSession hit observer (set_combat_result_observer) feeds dispatch() once a
// source hit is applied. Listeners decide their own role (attacker == self, target == self), exactly as the Lua does.
#include "../../actor_state.hpp"

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace dh::foundation::generic_skills {

inline constexpr std::uint32_t kCombatCallbackTargetHitV1 = 0x800u;     // AI flag: OnTargetHit present
inline constexpr std::uint32_t kCombatCallbackTargetMissedV1 = 0x1000u; // AI flag: OnTargetMissed present

struct CombatResultEventV1 {
    ActorId attacker = invalid_actor_id;
    ActorId target = invalid_actor_id;
    std::uint32_t outcomes = 0; // result bits; (outcomes & 3) != 0 selects the miss callback
    bool target_died = false;   // the hit killed the target (Lua defender:IsDead() after the hit)
};

class CombatResultEventBusV1 {
public:
    using Listener = std::function<void(const CombatResultEventV1&, ActorId receiver)>;

    // Registers a listener on the receiver's AI for one callback (kCombatCallbackTargetHitV1 or ...MissedV1). `key`
    // labels the owning skill. Returns a token for unsubscribe. Returns 0 for an invalid receiver or callback.
    std::uint64_t subscribe(ActorId receiver, std::uint32_t callback_flag, std::string key, Listener listener);
    bool unsubscribe(std::uint64_t token);
    // Removes every listener of one skill key on one receiver (skill removal / reset).
    std::size_t unsubscribe_key(ActorId receiver, const std::string& key);

    // The receiver's AI flag word for this bus: 0x800 when it has an OnTargetHit listener, 0x1000 for OnTargetMissed.
    std::uint32_t flags(ActorId receiver) const;
    std::size_t listener_count(ActorId receiver) const;

    // Original fan-out: the attacker receiver is served first, then the target receiver. Each receiver runs only the
    // callback its own flag word selects for these outcomes, in subscription order. Returns the listeners invoked.
    std::size_t dispatch(const CombatResultEventV1& event);

private:
    struct Entry {
        std::uint64_t token = 0;
        ActorId receiver = invalid_actor_id;
        std::uint32_t callback_flag = 0;
        std::string key;
        Listener listener;
    };
    std::vector<Entry> entries_;
    std::uint64_t next_token_ = 1;
};

} // namespace dh::foundation::generic_skills
