#include "event_passive_owner_v1.hpp"
#include "../../../game-data/ai.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

namespace dh::foundation::generic_skills {

namespace {
constexpr const char* kFrenzyClass = "Skill_Berserk_KillingFrenzy";
constexpr const char* kCarrionClass = "Skill_Necromancer_CarrionEater";
constexpr const char* kHolyWeaponClass = "Skill_Paladin_HolyWeapon";
constexpr const char* kShieldClass = "Skill_Paladin_GuardianShield";
constexpr unsigned kSnsPushChance = 183; // CharacterProperties.SnS_Push_Target_Chance (percent, raw x256)
constexpr unsigned kSnsStunChance = 184; // CharacterProperties.SnS_Stun_Target_Chance (percent, raw x256)
constexpr unsigned kSnsStunDuration = 185; // CharacterProperties.SnS_Stun_Duration (ms, raw x256)
constexpr unsigned kSnsDuration = 176;  // CharacterProperties.SnS_Duration (Conversion buff delay, raw x256 ms)
constexpr double kEnergyBalanceTickMs = 3000.0; // BUFF_DELAY: CreateBuff(class, 3000, false, rank) and the repeating timer
constexpr double kShieldStunTickMs = 500.0; // StartSkillTimerCB(500, true, check2, attacker): first tick applies the stun
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
    {"prince_paladin_holy_weapon", EventPassiveKindV1::holy_weapon},
    {"prince_illusionist_disciples", EventPassiveKindV1::disciples},
    {"prince_illusionist_petrify", EventPassiveKindV1::petrify},
    {"prince_illusionist_mirror", EventPassiveKindV1::mirror},
    {"prince_berseeker_killing_streak", EventPassiveKindV1::killing_streak},
    {"prince_berseeker_survival", EventPassiveKindV1::survival},
    {"prince_paladin_shield", EventPassiveKindV1::shield},
    {"prince_paladin_energy_balance", EventPassiveKindV1::energy_balance},
    {"prince_paladin_conversion", EventPassiveKindV1::conversion},
};

// prince_paladin_energy_balance.luac GetData(): the element row of GetEquippedFaeryElement(); any other value (no faery)
// uses the Lightning row. The port element numbers are FaeryTypes (Lightning 0, Earth 1, Water 2, Air 3, Fire 4); the Lua
// keys its table by the Elemental constants, whose numbers were not checked (phase-1 open question Q10).
const char* energy_balance_class_v1(std::int32_t element) {
    switch (element) {
    case 1: return "Skill_Paladin_EnergyBalance_Earth";
    case 2: return "Skill_Paladin_EnergyBalance_Water";
    case 3: return "Skill_Paladin_EnergyBalance_Air";
    case 4: return "Skill_Paladin_EnergyBalance_Fire";
    default: return "Skill_Paladin_EnergyBalance_Lightning";
    }
}
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
        // Conversion: OnTargetHit with attacker == self (the defender is the monster the player hit).
        if (rank(EventPassiveKindV1::conversion) > 0 && event.target != player_) conversion_hit(event.target);
    }
    if (event.target == player_ && event.attacker != player_ && rank(EventPassiveKindV1::holy_weapon) > 0)
        holy_hit(event.attacker);
    if (event.target == player_ && event.attacker != player_ && rank(EventPassiveKindV1::shield) > 0)
        shield_hit(event.attacker);
    // Mirror: the defender is the player (OnTargetHit with defender == self, attacker = the struck-by monster).
    if (event.target == player_ && event.attacker != player_ && rank(EventPassiveKindV1::mirror) > 0) mirror_hit(event);
    // Survival has no attacker guard in its Lua (INFERENCE: it therefore runs on the hits the player receives).
    if (event.target == player_ && rank(EventPassiveKindV1::survival) > 0) survival_hit();
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

void RuntimeEventPassiveOwnerV1::holy_hit(ActorId attacker) {
    // Lua: hp = attacker:GetHP(); hp = max(hp - regenHP_amount, 0) where regenHP_amount = TempProp1 of the caster's
    // Skill_Paladin_HolyWeapon row at the rank; attacker:SetProp(HP, hp); if hp == 0 then attacker:Kill().
    const std::uint32_t level = rank(EventPassiveKindV1::holy_weapon);
    auto* world = session_->world();
    const auto* live = world ? world->combat_properties(player_) : nullptr;
    auto* victim = session_->actor(attacker);
    if (!live || !victim) {
        error_ = "Holy Weapon requires the player's live sheet and the attacking actor";
        return;
    }
    const auto found = std::find(classes_->names.begin(), classes_->names.end(), std::string(kHolyWeaponClass));
    if (found == classes_->names.end()) {
        error_ = "Holy Weapon class absent from the ClassTables";
        return;
    }
    dh2::data::PropertySheet sheet = rules_->defaults;
    dh2::data::PropertySheet owner = live->sheets.resolved;
    sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
    std::string error;
    if (!dh2::data::apply_class(*classes_, std::int32_t(found - classes_->names.begin()), sheet, error, &owner)) {
        error_ = "Holy Weapon class evaluation: " + error;
        return;
    }
    const float amount = float(sheet[kTempProp1]) / 256.0f;
    const float before = victim->health;
    const float remaining = std::max(0.0f, before - amount);
    if (!world->set_current_health(attacker, remaining, error)) {
        error_ = "Holy Weapon HP write: " + error;
        return;
    }
    std::cout << "[holy-weapon] drain attacker=" << attacker << " hp " << before << " -> " << remaining
              << " (TempProp1 " << amount << " at rank " << level << ")\n";
    if (remaining <= 0.0f && !session_->script_kill_actor(attacker, error)) {
        error_ = "Holy Weapon Kill: " + error;
        return;
    }
    error_.clear();
}

void RuntimeEventPassiveOwnerV1::energy_advance(CombatSession& session, double dt_ms, std::string& error) {
    // OnSkillUpdate_ (rank change): RemoveBuff of the old buff and its timer, then CreateBuff + ApplyBuff at the rank and the
    // 3000 ms repeating timer (rank 0 stops). Timer tick (updateSelfBuff): CreateBuff + ApplyBuff of the element row at that
    // moment; the previous buff is not removed (it expires after its own 3000 ms).
    const std::uint32_t level = rank(EventPassiveKindV1::energy_balance);
    const bool rank_changed = level != energy_rank_seen_;
    if (!rank_changed) {
        if (!energy_active_) return;
        energy_timer_ms_ -= dt_ms;
        if (energy_timer_ms_ > 0.0) return;
    } else {
        energy_rank_seen_ = level;
        if (energy_active_) {
            if (!buffs_->remove_timed_buff(session, player_, energy_class_, error)) return;
            energy_active_ = false;
        }
        if (level == 0) return;
    }
    const char* wanted = energy_balance_class_v1(equipped_element_);
    TimedBuffSpecV1 spec;
    spec.class_name = wanted;
    spec.strength = level;
    spec.duration_ms = kEnergyBalanceTickMs;
    spec.permanent = false;
    spec.capacity = 1;
    spec.source = "energy balance";
    if (!buffs_->apply_timed_buff(session, player_, spec, error)) {
        error = "Energy Balance buff: " + error;
        return;
    }
    std::cout << "[energy-balance] class=" << wanted << " rank=" << level << " element=" << equipped_element_
              << (rank_changed ? " (rank change)" : " (timer)") << '\n';
    energy_active_ = true;
    energy_class_ = wanted;
    energy_timer_ms_ = kEnergyBalanceTickMs;
}

void RuntimeEventPassiveOwnerV1::conversion_hit(ActorId defender) {
    // prince_paladin_conversion.luac OnTargetHit_Skill: not boss / miniboss; a converted defender (memory slot by actor here,
    // by name in the Lua) loses its FactionBuff3 and is not converted again (skip); otherwise GetRand(0,100) below
    // TempProp1 (percent) converts it: CreateBuff(FactionBuff3, SnS_Duration, false, rank, persist FX) + ApplyBuff, Stop() on both.
    const std::uint32_t level = rank(EventPassiveKindV1::conversion);
    const auto* session = session_;
    if (!session) return;
    // Character::IsBoss / IsMiniBoss: AI row flags bit 2 / bit 1 (the same word the status owner reads).
    const auto* defender_props = session->world() ? session->world()->combat_properties(defender) : nullptr;
    const auto* ai_tables = session->original_ai_tables();
    const auto* defender_ai = defender_props && ai_tables ? dh2::data::ai_props(*ai_tables, defender_props->sheets.resolved[1]) : nullptr;
    if (defender_ai && (defender_ai->flags & 0x6u)) return;
    std::string error;
    bool skip = false;
    const auto slot = std::find(converted_.begin(), converted_.end(), defender);
    if (slot != converted_.end()) {
        if (!buffs_->remove_timed_buff(*session_, defender, "FactionBuff3", error)) {
            error_ = "Conversion break: " + error;
            return;
        }
        converted_.erase(slot);
        skip = true;
        std::cout << "[conversion] hit on converted monster=" << defender << " -> conversion removed\n";
    }
    const auto* victim = session_->actor(defender);
    const auto found = std::find(classes_->names.begin(), classes_->names.end(), std::string("Skill_Paladin_Conversion"));
    if (!victim || found == classes_->names.end()) return;
    dh2::data::PropertySheet sheet = rules_->defaults;
    const auto* live = session_->world() ? session_->world()->combat_properties(player_) : nullptr;
    dh2::data::PropertySheet owner = live ? live->sheets.resolved : rules_->defaults;
    sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
    if (!dh2::data::apply_class(*classes_, std::int32_t(found - classes_->names.begin()), sheet, error, &owner)) {
        error_ = "Conversion class evaluation: " + error;
        return;
    }
    const double chance = double(sheet[kTempProp1]) / 256.0;   // BUFF_CHANCE (percent)
    const double delay_ms = double(sheet[kSnsDuration]) / 256.0; // BUFF_DELAY = SnS_Duration
    const double roll = std::uniform_int_distribution<int>(0, 100)(rng_);  // GetRand(0,100)
    if (!(roll < chance) || skip || !victim->alive()) return;
    TimedBuffSpecV1 spec;
    spec.class_name = "FactionBuff3";
    spec.strength = level;
    spec.duration_ms = delay_ms;
    spec.permanent = false;
    spec.capacity = 1;
    spec.fx_set_name = "spell_dh2_prince_warrior_paladin_conversion_persist";
    spec.source = "conversion";
    if (!buffs_->apply_timed_buff(*session_, defender, spec, error)) {
        error_ = "Conversion buff: " + error;
        return;
    }
    if (converted_.size() < 6) converted_.push_back(defender);
    ++conversion_procs_;
    std::string stop_error;
    if (!session_->script_stop_actor(player_, stop_error) || !session_->script_stop_actor(defender, stop_error)) {
        error_ = "Conversion Stop: " + stop_error;
    }
    std::cout << "[conversion] converted monster=" << defender << " roll=" << roll << " chance=" << chance << " delay_ms="
              << delay_ms << " rank=" << level << " (PlaySound sfx_passive_skill_activate_1 not dispatched: presentation gap)\n";
}

void RuntimeEventPassiveOwnerV1::shield_hit(ActorId attacker) {
    // prince_paladin_shield.luac OnTargetHit_Skill: defender == self and attacker:TargetInMeleeRange(); two independent
    // GetRand(0,100) rolls against SnS_Push_Target_Chance (knockback) and SnS_Stun_Target_Chance (stun).
    const std::uint32_t level = rank(EventPassiveKindV1::shield);
    const auto* victim = session_->actor(attacker);
    const auto* player = session_->actor(player_);
    if (!victim || !player || !victim->alive()) return;
    // TargetInMeleeRange: INFERENCE, the same reach the Putrid owner uses (target radius of the attacker).
    const auto* world = session_->world();
    const float reach = world ? world->target_radius(*victim) : 0.0f;
    const float dx = victim->transform.position[0] - player->transform.position[0];
    const float dy = victim->transform.position[1] - player->transform.position[1];
    if (std::hypot(dx, dy) > reach) return;
    const auto index = std::find(classes_->names.begin(), classes_->names.end(), std::string(kShieldClass));
    if (index == classes_->names.end()) {
        error_ = "Guardian Shield class absent from the ClassTables";
        return;
    }
    dh2::data::PropertySheet sheet = rules_->defaults;
    const auto* live = session_->world() ? session_->world()->combat_properties(player_) : nullptr;
    dh2::data::PropertySheet owner = live ? live->sheets.resolved : rules_->defaults;
    sheet[kSnsLevel] = owner[kSnsLevel] = std::int32_t(level << 8);
    std::string error;
    if (!dh2::data::apply_class(*classes_, std::int32_t(index - classes_->names.begin()), sheet, error, &owner)) {
        error_ = "Guardian Shield class evaluation: " + error;
        return;
    }
    const int push_chance = static_cast<int>(std::floor(double(sheet[kSnsPushChance]) / 256.0));
    const int stun_chance = static_cast<int>(std::floor(double(sheet[kSnsStunChance]) / 256.0));
    const double stun_ms = sheet[kSnsStunDuration] > 0 ? double(sheet[kSnsStunDuration]) / 256.0 : 3000.0;
    // Lua: proc_chance < KNOCKBACK starts check() (SetKnockBackState(false): clears the flag, no displacement as shipped).
    const int push_roll = std::uniform_int_distribution<int>(0, 100)(rng_);
    if (push_roll < push_chance) {
        std::cout << "[shield] knockback roll passed attacker=" << attacker << " roll=" << push_roll << " chance=" << push_chance
                  << " (Lua SetKnockBackState(false): no displacement)\n";
    }
    // Lua: proc_chance2 < STUN starts check2(): SetStunState(3000, true) on the first 500 ms tick.
    const int stun_roll = std::uniform_int_distribution<int>(0, 100)(rng_);
    if (stun_roll < stun_chance) {
        shield_stuns_.push_back({attacker, kShieldStunTickMs, stun_ms});
        std::cout << "[shield] stun roll passed attacker=" << attacker << " roll=" << stun_roll << " chance=" << stun_chance
                  << " stun_ms=" << stun_ms << " tick_ms=" << kShieldStunTickMs << '\n';
    }
}

void RuntimeEventPassiveOwnerV1::shield_advance(CombatSession& session, double dt_ms) {
    for (auto it = shield_stuns_.begin(); it != shield_stuns_.end();) {
        it->left_ms -= dt_ms;
        if (it->left_ms > 0.0) {
            ++it;
            continue;
        }
        bool applied = false;
        std::string error;
        // Lua check2 tick: SetStunState(3000, true); the boss / missing-animation refusals come back as applied=false.
        if (!session.script_status_actor(it->actor, true, it->duration_ms, applied, error)) {
            error_ = "Guardian Shield stun: " + error;
        }
        std::cout << "[shield] stun tick attacker=" << it->actor << " applied=" << (applied ? 1 : 0) << " duration_ms="
                  << it->duration_ms << '\n';
        it = shield_stuns_.erase(it);
    }
}

bool RuntimeEventPassiveOwnerV1::advance(CombatSession& session, double dt_seconds, std::string& error) {
    error.clear();
    session_ = &session;
    shield_advance(session, dt_seconds * 1000.0);
    energy_advance(session, dt_seconds * 1000.0, error);
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
