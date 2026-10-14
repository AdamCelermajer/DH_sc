#include "runtime_necromancer_passive_owner_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation::generic_skills {

namespace {
constexpr const char* kWrathClass = "Skill_Necromancer_Wrath";
constexpr const char* kPutridClass = "Skill_Necromancer_Putrid";
constexpr const char* kWrathSound = "sfx_passive_skill_activate_3";
constexpr double kWrathTimerMs = 3000.0;   // TIMER_DELAY
constexpr double kWrathThresholdPct = 40.0; // PCT_VALUE
constexpr unsigned kSnsLevel = 172;         // CharacterProperties.SnS_Level
constexpr unsigned kTempProp1 = 190;        // CharacterProperties.TempProp1 (Putrid chance, percent x256)
constexpr unsigned kTempProp2 = 191;        // CharacterProperties.TempProp2 (Putrid DoT duration, ms x256)
constexpr std::uint64_t kOwnerWrath = 1;    // timer owner key of the Wrath loop

struct NecromancerPassiveScriptV1 {
    const char* script;
    NecromancerPassiveKindV1 kind;
};
constexpr NecromancerPassiveScriptV1 kNecromancerPassives[] = {
    {"prince_necromancer_wrath", NecromancerPassiveKindV1::wrath},
    {"prince_necromancer_putrid", NecromancerPassiveKindV1::putrid},
};
} // namespace

NecromancerPassiveKindV1 necromancer_passive_kind_for_script_v1(const std::string& script) {
    for (const auto& row : kNecromancerPassives)
        if (script == row.script) return row.kind;
    return NecromancerPassiveKindV1::none;
}

RuntimeNecromancerPassiveOwnerV1::RuntimeNecromancerPassiveOwnerV1(TimedBuffSinkV1& buffs, const dh2::data::ClassTables& classes,
                                                                   const dh2::data::PropertyRules& rules,
                                                                   CombatResultEventBusV1& bus, ActorId player,
                                                                   PlaySoundFn play_sound)
    : buffs_(&buffs), classes_(&classes), rules_(&rules), bus_(&bus), player_(player), play_sound_(std::move(play_sound)) {
    token_ = bus_->subscribe(player_, kCombatCallbackTargetHitV1, "necromancer_passives",
                             [this](const CombatResultEventV1& event, ActorId receiver) { on_hit(event, receiver); });
}

RuntimeNecromancerPassiveOwnerV1::~RuntimeNecromancerPassiveOwnerV1() {
    if (token_ != 0) bus_->unsubscribe(token_);
}

void RuntimeNecromancerPassiveOwnerV1::set_rank(NecromancerPassiveKindV1 kind, std::uint32_t rank) {
    if (rank == 0) {
        ranks_.erase(kind);
        if (kind == NecromancerPassiveKindV1::wrath && wrath_timer_ != 0) {
            timers_.stop(wrath_timer_);
            wrath_timer_ = 0;
        }
        return;
    }
    // The host syncs ranks every frame: an unchanged rank must not restart the Wrath loop.
    const auto previous = ranks_.find(kind);
    if (previous != ranks_.end() && previous->second == rank) return;
    ranks_[kind] = rank;
    // OnSkillUpdate_: StopTimerCB(timer) then StartSkillTimerCB(3000, true, canApplyBuff) on a rank change.
    if (kind == NecromancerPassiveKindV1::wrath) {
        if (wrath_timer_ != 0) timers_.stop(wrath_timer_);
        wrath_timer_ = timers_.start(kWrathTimerMs, true,
            [this](RuntimeSkillTimerIdV1, std::string& timer_error) -> bool {
                if (!session_) return true;  // no Session bound yet: nothing to read
                return wrath_tick(*session_, timer_error);
            }, kOwnerWrath);
    }
}

std::uint32_t RuntimeNecromancerPassiveOwnerV1::rank(NecromancerPassiveKindV1 kind) const {
    const auto found = ranks_.find(kind);
    return found == ranks_.end() ? 0u : found->second;
}

void RuntimeNecromancerPassiveOwnerV1::bind(CombatSession& session) { session_ = &session; }

bool RuntimeNecromancerPassiveOwnerV1::advance(CombatSession& session, double dt_ms, std::string& error) {
    error.clear();
    session_ = &session;
    return timers_.advance(dt_ms, error);
}

bool RuntimeNecromancerPassiveOwnerV1::class_sheet(std::uint32_t level, const char* class_name, dh2::data::PropertySheet& out,
                                                   std::string& error) {
    const auto found = std::find(classes_->names.begin(), classes_->names.end(), std::string(class_name));
    if (found == classes_->names.end()) {
        error = std::string("class absent from the ClassTables: ") + class_name;
        return false;
    }
    out = rules_->defaults;
    out[kSnsLevel] = std::int32_t(level << 8);
    return dh2::data::apply_class(*classes_, std::int32_t(found - classes_->names.begin()), out, error, nullptr);
}

// ----- Wrath -------------------------------------------------------------------------------------------------------
bool RuntimeNecromancerPassiveOwnerV1::wrath_tick(CombatSession& session, std::string& error) {
    // canApplyBuff: GetHP() percent against 40, buff handle nil-check.
    const auto* state = session.actor(player_);
    if (!state || state->max_health <= 0.0f) {
        error = "Wrath requires the player's HP";
        return false;
    }
    const double hp_pct = 100.0 * static_cast<double>(state->health) / static_cast<double>(state->max_health);
    const std::uint32_t level = rank(NecromancerPassiveKindV1::wrath);
    if (hp_pct < kWrathThresholdPct && !wrath_active_) {
        TimedBuffSpecV1 spec;
        spec.class_name = kWrathClass;
        spec.strength = level;             // CreateBuff(BUFF_ID, 0, false, skill_level, BUFF_FX)
        spec.duration_ms = 0.0;
        spec.permanent = true;
        spec.capacity = 1;
        spec.source = "Wrath";
        if (!buffs_->apply_timed_buff(session, player_, spec, error)) return false;
        wrath_active_ = true;
        if (play_sound_) {
            std::string sound_error;
            if (!play_sound_(kWrathSound, sound_error)) error_ = "Wrath sound: " + sound_error;
        }
    } else if (hp_pct >= kWrathThresholdPct && wrath_active_) {
        if (!buffs_->remove_timed_buff(session, player_, kWrathClass, error)) return false;
        wrath_active_ = false;
    }
    return true;
}

// ----- Putrid ------------------------------------------------------------------------------------------------------
void RuntimeNecromancerPassiveOwnerV1::on_hit(const CombatResultEventV1& event, ActorId receiver) {
    // The Lua listener checks its own role: defender == self and an attacker in melee range.
    if (receiver != player_ || event.target != player_ || event.attacker == player_) return;
    if (rank(NecromancerPassiveKindV1::putrid) == 0 || !session_) return;
    putrid_hit(event);
}

void RuntimeNecromancerPassiveOwnerV1::putrid_hit(const CombatResultEventV1& event) {
    const std::uint32_t level = rank(NecromancerPassiveKindV1::putrid);
    const auto* attacker = session_->actor(event.attacker);
    const auto* player = session_->actor(player_);
    if (!attacker || !player || !attacker->alive()) return;
    // TargetInMeleeRange: INFERENCE. The native gate (Character::_TargetInMeleeRange 0x3b7020) is not read here; the port
    // uses the attacker's target radius as the reach, which is the same radius the Session uses for melee searches.
    const auto* world = session_->world();
    const float reach = world ? world->target_radius(*attacker) : 0.0f;
    const float dx = attacker->transform.position[0] - player->transform.position[0];
    const float dy = attacker->transform.position[1] - player->transform.position[1];
    if (std::hypot(dx, dy) > reach) return;
    dh2::data::PropertySheet sheet{};
    std::string error;
    if (!class_sheet(level, kPutridClass, sheet, error)) {
        error_ = "Putrid class sheet: " + error;
        return;
    }
    // Lua: DOT = FromFixed(TempProp1) (percent); duration = FromFixed(TempProp2) (ms).
    const int chance = static_cast<int>(std::floor(static_cast<double>(sheet[kTempProp1]) / 256.0));
    const double duration_ms = static_cast<double>(sheet[kTempProp2]) / 256.0;
    const int roll = std::uniform_int_distribution<int>(0, 100)(rng_);  // GetRand(0,100)
    if (roll >= chance) return;
    ++putrid_procs_;
    TimedBuffSpecV1 spec;
    spec.class_name = kPutridClass;
    spec.strength = level;
    spec.duration_ms = duration_ms;
    spec.permanent = false;
    spec.capacity = 1;
    spec.source = "Putrid Touch";
    if (!buffs_->apply_timed_buff(*session_, event.attacker, spec, error)) {
        error_ = "Putrid buff: " + error;
        return;
    }
    ++putrid_applied_;
}

// ----- sync --------------------------------------------------------------------------------------------------------
bool sync_necromancer_passive_ranks_v1(RuntimeNecromancerPassiveOwnerV1& owner, const CharacterState& state,
                                       const dh2::data::SkillTables::Borrow& tables, std::string& error) {
    error.clear();
    if (!tables) {
        error = "Necromancer passive sync requires the SkillTable snapshot";
        return false;
    }
    const auto& records = tables.skills();
    std::map<NecromancerPassiveKindV1, std::uint32_t> learned;
    for (const auto& progress : state.skills) {
        const auto index = tables.skill_index(progress.id.c_str());
        if (index < 0 || static_cast<std::size_t>(index) >= records.size()) continue;
        const auto kind = necromancer_passive_kind_for_script_v1(records[static_cast<std::size_t>(index)].script);
        if (kind != NecromancerPassiveKindV1::none) learned[kind] = progress.rank;
    }
    for (const auto kind : {NecromancerPassiveKindV1::wrath, NecromancerPassiveKindV1::putrid}) {
        const auto found = learned.find(kind);
        owner.set_rank(kind, found == learned.end() ? 0u : found->second);
    }
    return true;
}

} // namespace dh::foundation::generic_skills
