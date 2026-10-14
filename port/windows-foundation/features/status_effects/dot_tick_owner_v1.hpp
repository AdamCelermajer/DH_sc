#pragma once
// v21.8 w2-assassin: damage-over-time owner (the original PROPS_AddDot + CharProperties::HandleDots pair).
//
// Original (IDA, libDungeonHunter2.so/pseudocode-all.c):
//  - CF_CalcDotDamage 0x3b09c4 (normal branch, a4 == 0): when the ATTACKER's live sheet has Dot_Duration (125) > 0, the hit
//    result carries duration = 125 and tick = Dot_Min (123) + GetRandom(Dot_Max (124) - Dot_Min). Called from CF__CalcDamage
//    (line ~130317) for every calculated hit, so Assassin PoisonSting's self buff (Dot_* cells) makes all of the caster's hits
//    carry a DoT.
//  - Character::F_ApplyResult 0x3b10b4 tail (lines ~129868-129883): for a NON-player target with duration (word +8 >> 8) > 0 and
//    tick (word +12) > 0: CharProperties::PROPS_AddDot(target, duration_ms, tick, element).
//  - PROPS_AddDot 0x3e2720: names the buff dot_<element> (normal = "dot_normal"), PROPS_AddBuff capacity 1 (a new DoT replaces the
//    old one of the same element), then writes the tick value to property element + 127 (Dot_Tick_Damage_Normal = 126 for
//    element -1). The property > 0 is the "DoT active" state; the buff expiry clears it.
//  - CharProperties::HandleDots 0x3df3f0 is run by CharAI command 52, driven by a repeating timer whose period is the
//    CharacterDesign constant DoT_Tick (1000 ms, design_pycst.bin). For each element slot with property > 0 it runs
//    F_DotAttack(tick damage, element) + F_ApplyResult.
//
// Port scope: the normal element (-1) only. Elemental DoTs (fire/water/...: CF_CalcDotDamage a4 = 1, props 178-181) and the
// Lua-side DOT removal are not used by the admitted skills and are not implemented here.
//
// Pure value logic (no session, no RNG): the combat session queues entries from applied hits and applies the ticks it returns.
#include <algorithm>
#include <cstdint>
#include <vector>

#include "../../actor_state.hpp"

namespace dh::foundation::status_effects {

// CharacterProperties raw indices (x256 fixed point) read by CF_CalcDotDamage.
inline constexpr int kDotMinRawIndex = 123;      // Dot_Min
inline constexpr int kDotMaxRawIndex = 124;      // Dot_Max
inline constexpr int kDotDurationRawIndex = 125; // Dot_Duration (ms, x256)
inline constexpr std::int32_t kDotNormalElement = -1;
// CharacterDesign DoT_Tick (ms) from original-cache/data/pydata/design_pycst.bin.
inline constexpr double kDotTickMs = 1000.0;

struct DotTickV1 {
    ActorId attacker = invalid_actor_id;
    ActorId target = invalid_actor_id;
    std::int32_t element = kDotNormalElement;
    double tick_hp = 0.0; // damage of this tick, HP units (raw x256 / 256)
};

class DotTickOwnerV1 {
public:
    // PROPS_AddDot: capacity 1 per element; a new DoT replaces the old one (its duration and tick are the new ones).
    // Non-positive duration or tick is ignored (F_ApplyResult only calls PROPS_AddDot for positive values).
    void add(ActorId attacker, ActorId target, std::int32_t element, double duration_ms, double tick_hp) {
        if (!(duration_ms > 0.0) || !(tick_hp > 0.0) || target == invalid_actor_id) return;
        auto it = std::find_if(entries_.begin(), entries_.end(), [&](const Entry& e) {
            return e.target == target && e.element == element;
        });
        if (it == entries_.end()) {
            entries_.push_back(Entry{attacker, target, element, duration_ms, tick_hp});
        } else {
            *it = Entry{attacker, target, element, duration_ms, tick_hp};
        }
    }

    // One call per frame: returns the ticks due in this frame (HandleDots on each DOT-timer boundary crossed while the DoT was
    // still active), then ages every DoT and drops the expired ones (buff expiry clears the property).
    std::vector<DotTickV1> advance(double dt_ms) {
        std::vector<DotTickV1> due;
        if (!(dt_ms >= 0.0) || entries_.empty()) {
            tick_clock_ms_ = 0.0;
            return due;
        }
        tick_clock_ms_ += dt_ms;
        while (tick_clock_ms_ >= kDotTickMs) {
            tick_clock_ms_ -= kDotTickMs;
            for (const auto& e : entries_) {
                if (e.remaining_ms > 0.0) due.push_back(DotTickV1{e.attacker, e.target, e.element, e.tick_hp});
            }
        }
        for (auto& e : entries_) e.remaining_ms -= dt_ms;
        entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
                                      [](const Entry& e) { return e.remaining_ms <= 0.0; }),
                       entries_.end());
        return due;
    }

    void forget_target(ActorId target) {
        entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
                                      [&](const Entry& e) { return e.target == target; }),
                       entries_.end());
    }

    bool empty() const noexcept { return entries_.empty(); }
    std::size_t size() const noexcept { return entries_.size(); }

private:
    struct Entry {
        ActorId attacker;
        ActorId target;
        std::int32_t element;
        double remaining_ms;
        double tick_hp;
    };
    std::vector<Entry> entries_;
    double tick_clock_ms_ = 0.0;
};

} // namespace dh::foundation::status_effects
