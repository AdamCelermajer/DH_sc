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
constexpr EventPassiveScriptV1 kEventPassives[] = {
    {"prince_berseeker_killing_frenzy", EventPassiveKindV1::killing_frenzy},
    {"prince_necromancer_carrioneater", EventPassiveKindV1::carrion_eater},
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
    if (receiver != player_ || event.attacker != player_ || !session_) return;
    if (rank(EventPassiveKindV1::killing_frenzy) > 0) frenzy_hit();
    if (event.target_died && rank(EventPassiveKindV1::carrion_eater) > 0) carrion_kill();
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
