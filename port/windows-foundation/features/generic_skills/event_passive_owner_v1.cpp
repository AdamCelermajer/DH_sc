#include "event_passive_owner_v1.hpp"

#include <algorithm>
#include <cstring>

namespace dh::foundation::generic_skills {

namespace {
constexpr const char* kFrenzyClass = "Skill_Berserk_KillingFrenzy";
constexpr const char* kCarrionClass = "Skill_Necromancer_CarrionEater";
constexpr const char* kContaminateClass = "Skill_Assassin_Contaminate";
constexpr const char* kSlowDebuffClass = "Debuff_Slow";
constexpr const char* kSlowDebuffFx = "AUTO_DEBUFF_SLOW";
constexpr const char* kStreakClass = "Skill_Berserk_KillingStreak";
constexpr const char* kStreakDummyClass = "Skill_Berserk_KillingStreak_dummy";
constexpr const char* kStreakDummyFx = "active_buff_fx"; // prince_berseeker_killing_streak.luac BUFF_FX
constexpr const char* kSurvivalClass = "Skill_Berserk_Survival";
constexpr const char* kSurvivalFx = "skill_dh2_prince_warrior_berseker_survival"; // prince_berseeker_survival.luac BUFF_FX
constexpr float kSurvivalPercentV1 = 35.0f;   // PCT_VALUE
constexpr double kSurvivalTimerMs = 8000.0;   // BUFF_TIMER (buff duration and the flag timer)
constexpr unsigned kSnsLevel = 172;  // CharacterProperties.SnS_Level
constexpr unsigned kSnsCooldown = 177; // CharacterProperties.SnS_Cooldown (Killing Streak bufftime, raw x256 ms)
constexpr unsigned kTempProp1 = 190; // CharacterProperties.TempProp1 (CarrionEater heal, raw x256 HP)
constexpr unsigned kHp = 36;         // CharacterProperties.HP (raw x256)
constexpr double kFrenzyResetMs = 1500.0;
constexpr double kFrenzyMaxMs = 5000.0;
constexpr std::uint32_t kFrenzyMaxCounter = 5;

struct EventPassiveScriptV1 {
    const char* script;
    EventPassiveKindV1 kind;
};
constexpr const char* kDisciplesClass = "Skill_Illusionist_Disciples";
constexpr const char* kPetrifyClass = "Skill_Illusionist_Petrify";
constexpr const char* kMirrorClass = "Skill_Illusionist_Mirror";
constexpr unsigned kTempProp2 = 191;          // CharacterProperties.TempProp2 (Disciples chance %, Mirror stun ms)
constexpr unsigned kDamageMinMainHand = 79;   // CharacterProperties.Damage_Min_Main_Hand (Petrify threshold, raw x256)
constexpr double kDisciplesDurationMs = 10000.0;          // Lua CreateBuff(CLASS_ID, 10000, false, L, FX)
constexpr float kPetrifyBossThreshold = 30.0f;            // Lua MIN_BOSS_DAMAGE_VALUE
constexpr float kPetrifyMonsterThreshold = 10.0f;         // Lua MIN_MONSTER_DAMAGE_VALUE
constexpr std::uint32_t kPetrifyCapacity = 5;             // Lua AMOUNT_OF_BUFFS_PER_MONSTER
constexpr const char* kDisciplesFx = "spell_dh2_prince_mage_illusionist_hypnotic_gaze_persist";

constexpr EventPassiveScriptV1 kEventPassives[] = {
    {"prince_berseeker_killing_frenzy", EventPassiveKindV1::killing_frenzy},
    {"prince_necromancer_carrioneater", EventPassiveKindV1::carrion_eater},
    {"prince_illusionist_disciples", EventPassiveKindV1::disciples},
    {"prince_illusionist_petrify", EventPassiveKindV1::petrify},
    {"prince_illusionist_mirror", EventPassiveKindV1::mirror},
    {"prince_berseeker_killing_streak", EventPassiveKindV1::killing_streak},
    {"prince_berseeker_survival", EventPassiveKindV1::survival},
    {"prince_assassin_contaminate", EventPassiveKindV1::contaminate},
    {"prince_assassin_blindness", EventPassiveKindV1::blindness_break},
};
} // namespace

EventPassiveKindV1 event_passive_kind_for_script_v1(std::string_view script) {
    for (const auto& row : kEventPassives)
        if (script == row.script) return row.kind;
    return EventPassiveKindV1::none;
}

RuntimeEventPassiveOwnerV1::RuntimeEventPassiveOwnerV1(TimedBuffSinkV1& buffs, const dh2::data::ClassTables& classes,
                                                       const dh2::data::PropertyRules& rules,
                                                       CombatResultEventBusV1& bus, ActorId player)
    : buffs_(&buffs), classes_(&classes), rules_(&rules), bus_(&bus), player_(player) {
    token_ = bus_->subscribe(player_, kCombatCallbackTargetHitV1, "event_passives",
                             [this](const CombatResultEventV1& event, ActorId receiver) { on_hit(event, receiver); });
}

RuntimeEventPassiveOwnerV1::~RuntimeEventPassiveOwnerV1() {
    if (token_ != 0) bus_->unsubscribe(token_);
}

void RuntimeEventPassiveOwnerV1::set_rank(EventPassiveKindV1 kind, std::uint32_t rank) {
    if (rank == 0) ranks_.erase(kind);
    else ranks_[kind] = rank;
}

std::uint32_t RuntimeEventPassiveOwnerV1::rank(EventPassiveKindV1 kind) const {
    const auto found = ranks_.find(kind);
    return found == ranks_.end() ? 0u : found->second;
}

void RuntimeEventPassiveOwnerV1::bind(CombatSession& session) { session_ = &session; }

void RuntimeEventPassiveOwnerV1::on_hit(const CombatResultEventV1& event, ActorId receiver) {
    // The Lua listener runs on every OnTargetHit and checks its own role; the bus delivers both participants.
    if (receiver != player_ || !session_) return;
    if (event.attacker == player_) {
        if (rank(EventPassiveKindV1::killing_frenzy) > 0) frenzy_hit();
        if (event.target_died && rank(EventPassiveKindV1::carrion_eater) > 0) carrion_kill();
        if (event.target != player_ && rank(EventPassiveKindV1::disciples) > 0) disciples_hit(event);
        if (event.target != player_ && rank(EventPassiveKindV1::petrify) > 0) petrify_hit(event);
        if (event.target_died && rank(EventPassiveKindV1::killing_streak) > 0) streak_kill();
    }
    // Mirror: the defender is the player (OnTargetHit with defender == self, attacker = the struck-by monster).
    if (event.target == player_ && event.attacker != player_ && rank(EventPassiveKindV1::mirror) > 0) mirror_hit(event);
    // Survival has no attacker guard in its Lua (INFERENCE: it therefore runs on the hits the player receives).
    if (event.target == player_ && rank(EventPassiveKindV1::survival) > 0) survival_hit();
    // Contaminate: OnTargetHit with defender == self (the caster is hit by an attacker in melee range).
    if (event.target == player_ && event.attacker != player_ && rank(EventPassiveKindV1::contaminate) > 0)
        contaminate_hit(event.attacker);
    // Blindness: OnTargetHit with attacker == self removes the buff (RemoveBuff + SetIsTargetable(true) through the owner).
    if (event.attacker == player_ && rank(EventPassiveKindV1::blindness_break) > 0) blindness_break_hit();
}

// prince_assassin_blindness.luac OnTargetHit_Skill: if slvl > 0 and attacker:GetID() == GetID(): RemoveBuff(BUFF_ID), buff = nil,
// SetIsTargetable(true). A removal with no live Blindness instance is a no-op in the owner (native PROPS_DelBuff).
void RuntimeEventPassiveOwnerV1::blindness_break_hit() {
    std::string error;
    if (!buffs_->remove_timed_buff(*session_, player_, "Skill_Assassin_Blindness", error))
        error_ = "Blindness break: " + error;
}

// prince_assassin_contaminate.luac OnTargetHit(attacker, defender): if defender == self and attacker:TargetInMeleeRange():
// proc_chance = GetRand(0,100); if proc_chance < SLOW (FromFixed(TempProp1) = 8 + 2 x rank percent) then
// attacker:CreateBuff(Debuff_Slow, duration, false, slvl, AUTO_DEBUFF_SLOW) + ApplyBuff, duration = FromFixed(TempProp2).
// The Lua's enemy_proc roll (GetRand(0, Slow_Resist_Chance)) is computed but never used by the authored script.
void RuntimeEventPassiveOwnerV1::contaminate_hit(ActorId attacker) {
    auto* world = session_->world();
    if (!world || !world->original_melee_in_range(attacker, player_)) return;
    const std::uint32_t level = rank(EventPassiveKindV1::contaminate);
    const auto found = std::find(classes_->names.begin(), classes_->names.end(), std::string(kContaminateClass));
    const auto* live = world->combat_properties(player_);
    if (found == classes_->names.end() || !live) {
        error_ = "Contaminate class or the player's live sheet is absent";
        return;
    }
    dh2::data::PropertySheet sheet = rules_->defaults;
    dh2::data::PropertySheet owner = live->sheets.resolved;
    sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
    std::string error;
    if (!dh2::data::apply_class(*classes_, std::int32_t(found - classes_->names.begin()), sheet, error, &owner)) {
        error_ = "Contaminate class evaluation: " + error;
        return;
    }
    const float slow_chance = float(sheet[kTempProp1]) / 256.0f;
    const float duration_ms = float(sheet[kTempProp2]) / 256.0f;
    std::uint32_t roll = 0;
    if (!world->random_uniform(101u, roll, error)) {
        error_ = "Contaminate proc roll: " + error;
        return;
    }
    if (!(float(roll) < slow_chance)) return;
    TimedBuffSpecV1 spec;
    spec.class_name = kSlowDebuffClass;
    spec.strength = level;
    spec.duration_ms = duration_ms;
    spec.capacity = 1;            // CreateBuff(..., false, ...): one instance per class
    spec.fx_set_name = kSlowDebuffFx;
    spec.source = "contaminate";
    if (!buffs_->apply_timed_buff(*session_, attacker, spec, error)) error_ = "Contaminate Debuff_Slow: " + error;
}

bool RuntimeEventPassiveOwnerV1::projected_class_cell(const char* class_name, std::uint32_t level, unsigned cell, float& value) {
    const auto found = std::find(classes_->names.begin(), classes_->names.end(), std::string(class_name));
    if (found == classes_->names.end()) {
        error_ = std::string(class_name) + " class absent from the ClassTables";
        return false;
    }
    const auto* live = session_->world() ? session_->world()->combat_properties(player_) : nullptr;
    if (!live) {
        error_ = std::string(class_name) + " requires the player's live sheet";
        return false;
    }
    dh2::data::PropertySheet sheet = rules_->defaults;
    dh2::data::PropertySheet owner = live->sheets.resolved;
    sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
    std::string error;
    if (!dh2::data::apply_class(*classes_, std::int32_t(found - classes_->names.begin()), sheet, error, &owner)) {
        error_ = std::string(class_name) + " class evaluation: " + error;
        return false;
    }
    value = float(sheet[cell]) / 256.0f;
    return true;
}

void RuntimeEventPassiveOwnerV1::disciples_hit(const CombatResultEventV1& event) {
    // Lua OnTargetHit: skill_level > 0, attacker == self, defender HP > 0 and a Character. chance1 = GetRand(1, 100) is drawn
    // for every such hit; `chance1 < attack_chance` (TempProp2 at the rank) and not boss/miniboss gives the 10 s faction flip.
    const std::uint32_t level = rank(EventPassiveKindV1::disciples);
    if (event.target_died) return;
    float attack_chance = 0.0f;
    if (!projected_class_cell(kDisciplesClass, level, kTempProp2, attack_chance)) return;
    std::uniform_int_distribution<int> draw(1, 100);
    const int chance = draw(rng_);
    const auto flags = target_buff_flags_v1(*session_, event.target);
    if (!(float(chance) < attack_chance) || flags.is_boss || flags.is_miniboss) return;
    TimedBuffSpecV1 spec;
    spec.class_name = kDisciplesClass;
    spec.strength = level;
    spec.duration_ms = kDisciplesDurationMs;
    spec.capacity = 1;
    spec.fx_set_name = kDisciplesFx;
    spec.source = "disciples";
    std::string error;
    if (!buffs_->apply_timed_buff(*session_, event.target, spec, error)) error_ = "Disciples buff: " + error;
}

void RuntimeEventPassiveOwnerV1::petrify_hit(const CombatResultEventV1& event) {
    // Lua OnTargetHit: skill_level > 0 and attacker == self. The struck monster's current Damage_Min_Main_Hand (x256 raw)
    // must exceed 30 (boss/miniboss) or 10; then CreateBuff(CLASS_ID, 0, 5, L) + ApplyBuff on the target (permanent).
    const std::uint32_t level = rank(EventPassiveKindV1::petrify);
    const auto* world = session_->world();
    const auto* live = world ? world->combat_properties(event.target) : nullptr;
    if (!live) return;
    const float damage_rating = float(live->sheets.resolved[kDamageMinMainHand]) / 256.0f;
    const auto flags = target_buff_flags_v1(*session_, event.target);
    const float threshold = (flags.is_boss || flags.is_miniboss) ? kPetrifyBossThreshold : kPetrifyMonsterThreshold;
    if (!(damage_rating > threshold)) return;
    TimedBuffSpecV1 spec;
    spec.class_name = kPetrifyClass;
    spec.strength = level;
    spec.duration_ms = 0.0;
    spec.permanent = true;
    spec.capacity = kPetrifyCapacity;
    spec.source = "petrify";
    std::string error;
    if (!buffs_->apply_timed_buff(*session_, event.target, spec, error)) error_ = "Petrify buff: " + error;
}

void RuntimeEventPassiveOwnerV1::mirror_hit(const CombatResultEventV1& event) {
    // Lua OnTargetHit: skill_level > 0, defender == self and attacker:TargetInMeleeRange(); GetRand(0, 100) < STUN
    // (TempProp1 at the rank) -> attacker:SetStunState(DURATION = TempProp2, true).
    const std::uint32_t level = rank(EventPassiveKindV1::mirror);
    const auto* world = session_->world();
    if (!world || !world->original_melee_in_range(event.attacker, player_)) return;
    float stun_chance = 0.0f, stun_ms = 0.0f;
    if (!projected_class_cell(kMirrorClass, level, 190u, stun_chance) || !projected_class_cell(kMirrorClass, level, kTempProp2, stun_ms)) return;
    std::uniform_int_distribution<int> draw(0, 100);
    const int proc = draw(rng_);
    if (!(float(proc) < stun_chance)) return;
    if (!stun_target_) {
        error_ = "Mirror stun has no ES-STATUS hook bound";
        return;
    }
    std::string error;
    if (!stun_target_(*session_, event.attacker, double(stun_ms), error)) error_ = "Mirror stun: " + error;
}

void RuntimeEventPassiveOwnerV1::survival_hit() {
    // prince_berseeker_survival.luac OnTargetHit_Skill. HP% < 35 with no flag: SetProp(SnS_Level, L), CreateBuff(BUFF_ID,
    // 8000, false, L, BUFF_FX), ApplyBuff, 8000 ms flag timer. HP% >= 35 with the buff: RemoveBuff, flag cleared.
    const std::uint32_t level = rank(EventPassiveKindV1::survival);
    const auto* world = session_->world();
    float health = 0.0f, maximum = 0.0f;
    if (!world || !world->current_health(player_, health, maximum) || !(maximum > 0.0f)) {
        error_ = "Survival requires the player's live HP sheet";
        return;
    }
    const float percent = health / maximum * 100.0f;
    std::string error;
    if (percent < kSurvivalPercentV1 && !survival_flag_) {
        TimedBuffSpecV1 spec;
        spec.class_name = kSurvivalClass;
        spec.strength = level;
        spec.duration_ms = kSurvivalTimerMs;   // CreateBuff(BUFF_ID, BUFF_TIMER, ...): the buff itself expires with the timer
        spec.capacity = 1;
        spec.fx_set_name = kSurvivalFx;
        spec.source = "survival";
        if (!buffs_->apply_timed_buff(*session_, player_, spec, error)) {
            error_ = "Survival buff: " + error;
            return;
        }
        survival_buff_ = true;
        survival_flag_ = true;
        survival_flag_ms_ = kSurvivalTimerMs;
    } else if (percent >= kSurvivalPercentV1 && survival_buff_) {
        if (!buffs_->remove_timed_buff(*session_, player_, kSurvivalClass, error)) {
            error_ = "Survival removal: " + error;
            return;
        }
        survival_buff_ = false;
        survival_flag_ = false;
        survival_flag_ms_ = 0.0;
    }
}

void RuntimeEventPassiveOwnerV1::streak_kill() {
    // Lua: buff = CreateBuff(CLASS_ID, bufftime, true, L) and buff2 = CreateBuff(DUMMY_ID, bufftime, false, L, BUFF_FX), both
    // applied with no removal first. The body class has capacity 128 (true), so every kill adds an instance (stacking) for
    // bufftime = SnS_Cooldown of the class at the rank; the dummy has capacity 1 and is replaced by the newer kill.
    const std::uint32_t level = rank(EventPassiveKindV1::killing_streak);
    const auto* live = session_->world() ? session_->world()->combat_properties(player_) : nullptr;
    if (!live) {
        error_ = "Killing Streak requires the player's live sheet";
        return;
    }
    const auto found = std::find(classes_->names.begin(), classes_->names.end(), std::string(kStreakClass));
    if (found == classes_->names.end()) {
        error_ = "Killing Streak class absent from the ClassTables";
        return;
    }
    dh2::data::PropertySheet sheet = rules_->defaults;
    dh2::data::PropertySheet owner = live->sheets.resolved;
    sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
    std::string error;
    if (!dh2::data::apply_class(*classes_, std::int32_t(found - classes_->names.begin()), sheet, error, &owner)) {
        error_ = "Killing Streak class evaluation: " + error;
        return;
    }
    const double bufftime_ms = double(sheet[kSnsCooldown]) / 256.0;
    if (!(bufftime_ms > 0.0)) {
        error_ = "Killing Streak bufftime projected to zero";
        return;
    }
    TimedBuffSpecV1 body;
    body.class_name = kStreakClass;
    body.strength = level;
    body.duration_ms = bufftime_ms;
    body.capacity = 128;
    body.source = "killing streak";
    TimedBuffSpecV1 dummy;
    dummy.class_name = kStreakDummyClass;
    dummy.strength = level;
    dummy.duration_ms = bufftime_ms;
    dummy.capacity = 1;
    dummy.fx_set_name = kStreakDummyFx;
    dummy.source = "killing streak dummy";
    std::string apply_error;
    if (!buffs_->apply_timed_buff(*session_, player_, body, apply_error) ||
        !buffs_->apply_timed_buff(*session_, player_, dummy, apply_error)) {
        error_ = "Killing Streak buff: " + apply_error;
    }
}

bool RuntimeEventPassiveOwnerV1::frenzy_set_buff(std::uint32_t strength) {
    std::string error;
    TimedBuffSpecV1 spec;
    spec.class_name = kFrenzyClass;
    spec.strength = strength;
    spec.duration_ms = 0.0;   // CreateBuff(BUFF_ID, 0, ...): permanent, the reset timer removes it
    spec.permanent = true;
    spec.capacity = 1;
    spec.source = "killing frenzy";
    if (!buffs_->remove_timed_buff(*session_, player_, kFrenzyClass, error) ||
        !buffs_->apply_timed_buff(*session_, player_, spec, error)) {
        error_ = "Killing Frenzy buff: " + error;
        frenzy_active_ = false;
        return false;
    }
    frenzy_active_ = true;
    return true;
}

void RuntimeEventPassiveOwnerV1::frenzy_hit() {
    const std::uint32_t level = rank(EventPassiveKindV1::killing_frenzy);
    // Lua: `if CURR == 0 or not buff`: CURR = 1 and a new buff; `CURR < 5 and buff`: CURR + 1 and rebuild; else the
    // maximum is rebuilt with a 5000 ms timer. Every branch restarts the timer.
    if (frenzy_counter_ == 0 || !frenzy_active_) {
        frenzy_counter_ = 1;
        if (frenzy_set_buff(1u * level)) frenzy_timer_ms_ = kFrenzyResetMs;
    } else if (frenzy_counter_ < kFrenzyMaxCounter) {
        ++frenzy_counter_;
        if (frenzy_set_buff(frenzy_counter_ * level)) frenzy_timer_ms_ = kFrenzyResetMs;
    } else {
        if (frenzy_set_buff(kFrenzyMaxCounter * level)) frenzy_timer_ms_ = kFrenzyMaxMs;
    }
}

void RuntimeEventPassiveOwnerV1::carrion_kill() {
    // Lua: SetProp(SnS_Level, rank); ApplyPropClass(CLASS); heal TempProp1 with HP clamped to max.
    const std::uint32_t level = rank(EventPassiveKindV1::carrion_eater);
    auto* world = session_->world();
    const auto* live = world ? world->combat_properties(player_) : nullptr;
    auto* state = session_->actor(player_);
    if (!live || !state) {
        error_ = "Carrion Eater requires the player's live sheet";
        return;
    }
    const auto found = std::find(classes_->names.begin(), classes_->names.end(), std::string(kCarrionClass));
    if (found == classes_->names.end()) {
        error_ = "Carrion Eater class absent from the ClassTables";
        return;
    }
    dh2::data::PropertySheet sheet = rules_->defaults;
    dh2::data::PropertySheet owner = live->sheets.resolved;
    sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
    std::string error;
    if (!dh2::data::apply_class(*classes_, std::int32_t(found - classes_->names.begin()), sheet, error, &owner)) {
        error_ = "Carrion Eater class evaluation: " + error;
        return;
    }
    const float heal = float(sheet[kTempProp1]) / 256.0f;
    // HP is a saved-based property: a direct resolved[] write is recomputed away by the next resolve and made the
    // death-reward vital sync fail ("Live ActorState vital could not be applied"). SetProp(HP) (set_current_health)
    // moves the saved cell with the resolved cell, as the other source HP writers do.
    // The live ActorState HP is the session truth (combat damage reaches it first; the property sheet is synchronised
    // from it by the death-reward path), so the heal is computed from the actor and then published through SetProp.
    const float before = state->health;
    const float healed = std::min(state->max_health, before + heal);
    if (!world->set_current_health(player_, healed, error)) {
        error_ = "Carrion Eater HP write: " + error;
        return;
    }
    carrion_heals_.push_back({before, heal, healed, level});
}

bool RuntimeEventPassiveOwnerV1::advance(CombatSession& session, double dt_seconds, std::string& error) {
    error.clear();
    session_ = &session;
    // Survival flag (Lua StopBuffTimer): clears only the flag; the buff keeps its own duration.
    if (survival_flag_) {
        survival_flag_ms_ -= dt_seconds * 1000.0;
        if (survival_flag_ms_ <= 0.0) {
            survival_flag_ = false;
            survival_flag_ms_ = 0.0;
        }
    }
    if (!frenzy_active_ || frenzy_timer_ms_ <= 0.0) return true;
    frenzy_timer_ms_ -= dt_seconds * 1000.0;
    if (frenzy_timer_ms_ > 0.0) return true;
    // Lua resetBuff: RemoveBuff and buff = nil; the counter is kept (the next hit without a buff restarts at 1).
    std::string remove_error;
    if (!buffs_->remove_timed_buff(session, player_, kFrenzyClass, remove_error)) {
        error = "Killing Frenzy reset: " + remove_error;
        frenzy_active_ = false;
        frenzy_timer_ms_ = 0.0;
        return false;
    }
    frenzy_active_ = false;
    frenzy_timer_ms_ = 0.0;
    return true;
}

bool sync_event_passive_ranks_v1(RuntimeEventPassiveOwnerV1& owner, const CharacterState& state,
                                 const dh2::data::SkillTables::Borrow& tables, std::string& error) {
    error.clear();
    if (!tables) {
        error = "Event passive sync requires the SkillTable snapshot";
        return false;
    }
    const auto& records = tables.skills();
    for (const auto& progress : state.skills) {
        const auto index = tables.skill_index(progress.id.c_str());
        if (index < 0 || static_cast<std::size_t>(index) >= records.size()) continue;
        const auto kind = event_passive_kind_for_script_v1(records[static_cast<std::size_t>(index)].script);
        if (kind == EventPassiveKindV1::none) continue;
        owner.set_rank(kind, progress.rank);
    }
    return true;
}

} // namespace dh::foundation::generic_skills
