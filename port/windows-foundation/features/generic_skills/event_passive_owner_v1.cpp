#include "event_passive_owner_v1.hpp"

#include <algorithm>
#include <cstring>

namespace dh::foundation::generic_skills {

namespace {
constexpr const char* kFrenzyClass = "Skill_Berserk_KillingFrenzy";
constexpr const char* kCarrionClass = "Skill_Necromancer_CarrionEater";
constexpr unsigned kSnsLevel = 172;  // CharacterProperties.SnS_Level
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
    }
    // Mirror: the defender is the player (OnTargetHit with defender == self, attacker = the struck-by monster).
    if (event.target == player_ && event.attacker != player_ && rank(EventPassiveKindV1::mirror) > 0) mirror_hit(event);
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
    const float healed = std::min(state->max_health, state->health + heal);
    // Write HP to the actor and to the property sheet (the source writes HP through SetProp).
    auto seeded = *live;
    seeded.sheets.resolved[kHp] = std::int32_t(healed * 256.0f);
    if (!world->update_combat_properties(player_, seeded, *world->traits(player_), error)) {
        error_ = "Carrion Eater HP write: " + error;
        return;
    }
    session_->actor(player_)->health = healed;
}

bool RuntimeEventPassiveOwnerV1::advance(CombatSession& session, double dt_seconds, std::string& error) {
    error.clear();
    session_ = &session;
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
