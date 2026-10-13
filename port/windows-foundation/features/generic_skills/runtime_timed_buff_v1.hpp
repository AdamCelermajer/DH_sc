#pragma once
// v21.8 skills-a: reusable timed buff / temporary property owner for CombatSession actors.
//
// Original model (recovered notes: port/level-world/character_buffs.cpp, character_skill_buff_bindings_v3.cpp and
// reference/character-skill-gameplay-v3/NOTES.md; Lua prince_rogue_quickness.luac):
//   CreateBuff(classId, duration_ms, capacity, strength, fxOid) -> an instance owning a free property sheet
//     (defaults, SnS_Level word172 = strength<<8) filed under its class id (BuffOwner map, ascending class id,
//     instances in insertion order) with one expiry timer of duration_ms and an optional looping AnimatedFX.
//   ApplyBuff(classId, instance) = ApplyPropClass(class, externalSheet): the class evaluates into the instance
//     sheet (linear sources read the owner's resolved sheet, where SnS_Level was just set) and the owner recalculates.
//   Recalculation resolves each property from base, saved, gear and then the buff groups with the native
//     per-type rule (type&4 properties accumulate every buff sheet).
//   RemoveBuff / timer expiry destroys the instance, releases the FX and recalculates.
//
// This owner keeps that model in the windows-foundation property domain, data driven by the ClassTable row only:
// the buff sheets are overlaid on the live CombatSession actor sheet (PlayableActorWorld::update_combat_properties).
// It is small on purpose and reusable for any "class row + duration" buff (passives and Faery buffs can use the same
// apply/remove calls later). Gaps are listed in coordination/v21.8/skills-a-report.md.
#include "../../combat_session.hpp"
#include "../../../game-data/class_tables.hpp"
#include "../../../game-data/properties.hpp"

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace dh::foundation::generic_skills {

struct TimedBuffSpecV1 {
    std::string class_name;       // ClassTable row evaluated into the buff sheet (Skill_Rogue_Quickness_buff)
    std::uint32_t strength = 0;   // CreateBuff strength = SnS_Level of the sheet (skill rank)
    double duration_ms = 0.0;     // expiry timer; must be positive unless permanent
    // Permanent buff: no expiry timer (original CreateBuff with duration 0 -> PROPS_AddBuff starts no TMR). Ends only by
    // RemoveBuff. Used by the Celest faery passive (faerie_celest.luac CheckFaeries); skills keep positive durations.
    bool permanent = false;
    std::string fx_set_name;      // optional EffectsTables set (AnimatedEffectTable name); presentation only
    std::string source;           // label for logs
};

// What the cast coordinator needs from a buff owner. Keeping it abstract means the coordinator links without the
// owner implementation (several test runners link the coordinator alone).
class TimedBuffSinkV1 {
public:
    virtual ~TimedBuffSinkV1() = default;
    // Source Lua OnPreSkill_: RemoveBuff(BUFF_ID, previous). No-op when no such buff is active.
    virtual bool remove_timed_buff(CombatSession&, ActorId, const std::string& class_name, std::string& error) = 0;
    // Source Lua OnSkill_: CreateBuff + ApplyBuff. Replaces an active buff of the same class.
    virtual bool apply_timed_buff(CombatSession&, ActorId, const TimedBuffSpecV1&, std::string& error) = 0;
};

// v21.8 slow (original Character::F_ApplyResult 0x3b10b4, the byte+25 bit 0 branch -> CharProperties::PROPS_DebuffSlow):
// outcome bit 0x100 on a non-boss defender adds the ClassTable row Debuff_Slow with capacity 1, strength 0 and duration
// = attacker property 189 (SnS_Slow_Duration) >> 8 when the skill mask has 0x10000, else property 146 >> 8.
// PROPS_DebuffSlow does nothing for a zero duration. The original's AUTO_DEBUFF_SLOW FX is not requested here.
// Returns nullopt when the outcome does not slow this defender.
std::optional<TimedBuffSpecV1> debuff_slow_spec_v1(std::uint32_t source_outcomes, std::uint32_t source_mask,
                                                   const dh2::data::PropertySheet& attacker_resolved,
                                                   bool defender_is_boss);
// Same rule from the already-selected raw x256 duration (DamageEvent::source_slow_duration_raw: the property 189/146 value of the sheet the hit was rolled with).
std::optional<TimedBuffSpecV1> debuff_slow_spec_from_duration_v1(std::uint32_t source_outcomes, std::int32_t duration_raw, bool defender_is_boss);

struct TimedBuffInstanceV1 {
    std::int32_t class_id = -1;
    std::string class_name, fx_set_name, source;
    std::uint32_t strength = 0;
    double total_ms = 0.0, remaining_ms = 0.0;
    dh2::data::PropertySheet sheet{};
    std::uintptr_t fx_identity = 0;  // opaque FX instance returned by the FX service (0 = none)
};

// Pure property-domain core (no session): resolves every cell touched by the buff sheets (plus `previous_touched`,
// cells a former buff changed) with the native rule over base/saved/gear and the ordered buff groups. Only cells
// with a buff-visible type (type&7) are considered. Returns the cells recomputed in `touched` and whether the
// resolved sheet changed. `buffs` must be ordered like the native owner (ascending class id, insertion order).
bool resolve_timed_buffs_v1(const dh2::data::PropertyRules&, dh2::data::PropertyState&,
                            const std::vector<const TimedBuffInstanceV1*>& buffs,
                            const std::set<unsigned>& previous_touched, std::set<unsigned>& touched,
                            bool& changed, std::string& error);

struct TimedBuffEventV1 {
    enum class Kind : std::uint8_t { applied, replaced, removed, expired, reasserted, dropped } kind = Kind::applied;
    ActorId actor = invalid_actor_id;
    std::string class_name, source;
    std::uint32_t strength = 0;
    double duration_ms = 0.0;
    std::vector<std::pair<unsigned, std::pair<std::int32_t, std::int32_t>>> changed_cells; // cell, before, after (raw)
    std::string detail;
};

struct TimedBuffFxServicesV1 {
    // Creates the persistent buff FX on the actor; returns an opaque identity. Failure is presentation-only.
    std::function<bool(const std::string& set_name, ActorId, std::uintptr_t& identity, std::string& error)> play;
    std::function<bool(std::uintptr_t identity, std::string& error)> drop;
};

struct TimedBuffBindingsV1 {
    const dh2::data::ClassTables* classes = nullptr;
    const dh2::data::PropertyRules* rules = nullptr;
    TimedBuffFxServicesV1 fx;
};

class RuntimeTimedBuffOwnerV1 final : public TimedBuffSinkV1 {
public:
    explicit RuntimeTimedBuffOwnerV1(TimedBuffBindingsV1 bindings) : bindings_(std::move(bindings)) {}

    bool remove_timed_buff(CombatSession&, ActorId, const std::string& class_name, std::string& error) override;
    bool apply_timed_buff(CombatSession&, ActorId, const TimedBuffSpecV1&, std::string& error) override;

    // Call once per rendered frame after CombatSession::update (outside the update): ages every buff by dt,
    // expires finished ones, drops buffs of dead or removed actors and re-asserts the overlay when an external
    // writer (equipment rebuild, stat change) rebuilt the actor sheet without it.
    bool advance(CombatSession&, double dt_seconds, std::string& error);

    // True once after any change of an actor's buff-visible cells. Hosts re-derive rate-bound state (locomotion
    // clip rates) when this is set.
    bool take_sheet_dirty(ActorId);
    std::vector<TimedBuffEventV1> take_events();

    std::size_t active_count(ActorId) const;
    bool active(ActorId, const std::string& class_name) const;
    double remaining_ms(ActorId, const std::string& class_name) const;

private:
    struct ActorBuffs {
        std::vector<std::unique_ptr<TimedBuffInstanceV1>> instances; // insertion order
        std::set<unsigned> touched;
    };
    std::vector<const TimedBuffInstanceV1*> ordered(const ActorBuffs&) const;
    // Overlays the actor's current buffs on its live sheet; publishes through the world when a cell changed.
    bool publish(CombatSession&, ActorId, ActorBuffs&, TimedBuffEventV1::Kind kind,
                 const std::string& class_name, const std::string& source, std::uint32_t strength, double duration_ms,
                 bool& changed, std::string& error);
    void release_fx(TimedBuffInstanceV1&);
    bool bind_lease(CombatSession&);

    TimedBuffBindingsV1 bindings_;
    std::weak_ptr<const void> lease_;
    bool lease_bound_ = false;
    std::map<ActorId, ActorBuffs> actors_;
    std::set<ActorId> dirty_;
    std::vector<TimedBuffEventV1> events_;
};

} // namespace dh::foundation::generic_skills
