#include "runtime_archer_passive_owner_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation::generic_skills {

namespace {
constexpr const char* kPrecisionClass = "Skill_Archer_Precision";
constexpr const char* kSideStepClass = "Skill_Archer_SideStep";
constexpr double kPrecisionCheckMs = 1000.0;  // StartSkillTimerCB(1000, true, checkbuff)
constexpr double kSideStepCheckMs = 500.0;    // StartSkillTimerCB(500, true, check, attacker)
constexpr unsigned kSnsLevel = 172;           // CharacterProperties.SnS_Level
constexpr unsigned kTempProp1 = 190;          // CharacterProperties.TempProp1 (SideStep KNOCKBACK percent x256)
constexpr std::int32_t kStateAttack = 5;      // AIStates Attack
constexpr std::int32_t kStateKnockedBack = 10;  // AIStates KnockedBack

struct ArcherPassiveScriptV1 {
    const char* script;
    ArcherPassiveKindV1 kind;
};
constexpr ArcherPassiveScriptV1 kArcherPassives[] = {
    {"prince_archer_precision", ArcherPassiveKindV1::precision},
    {"prince_archer_sidestep", ArcherPassiveKindV1::side_step},
};
} // namespace

ArcherPassiveKindV1 archer_passive_kind_for_script_v1(const std::string& script) {
    for (const auto& row : kArcherPassives)
        if (script == row.script) return row.kind;
    return ArcherPassiveKindV1::none;
}

RuntimeArcherPassiveOwnerV1::RuntimeArcherPassiveOwnerV1(TimedBuffSinkV1& buffs, const dh2::data::ClassTables& classes,
                                                         const dh2::data::PropertyRules& rules, CombatResultEventBusV1& bus,
                                                         ActorId player, RuntimeArcherPassiveServicesV1 services)
    : buffs_(&buffs), classes_(&classes), rules_(&rules), bus_(&bus), player_(player), services_(std::move(services)) {
    token_ = bus_->subscribe(player_, kCombatCallbackTargetHitV1, "archer_passives",
                             [this](const CombatResultEventV1& event, ActorId receiver) { on_hit(event, receiver); });
}

RuntimeArcherPassiveOwnerV1::~RuntimeArcherPassiveOwnerV1() {
    if (token_ != 0) bus_->unsubscribe(token_);
}

void RuntimeArcherPassiveOwnerV1::log(const std::string& line) const {
    if (services_.trace) services_.trace(line);
}

std::uint32_t RuntimeArcherPassiveOwnerV1::rank(ArcherPassiveKindV1 kind) const {
    const auto found = ranks_.find(kind);
    return found == ranks_.end() ? 0u : found->second;
}

void RuntimeArcherPassiveOwnerV1::bind(CombatSession& session) { session_ = &session; }

bool RuntimeArcherPassiveOwnerV1::class_sheet(std::uint32_t level, const char* class_name, dh2::data::PropertySheet& out,
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

// ----- Precision ---------------------------------------------------------------------------------------------------
bool RuntimeArcherPassiveOwnerV1::precision_create(std::string& error) {
    if (!session_) return true;
    TimedBuffSpecV1 spec;
    spec.class_name = kPrecisionClass;
    spec.strength = rank(ArcherPassiveKindV1::precision);  // CreateBuff(CLASS_ID, 0, false, skill_level)
    spec.duration_ms = 0.0;
    spec.permanent = true;
    spec.capacity = 1;
    spec.source = "Precision";
    if (!buffs_->apply_timed_buff(*session_, player_, spec, error)) return false;
    precision_buff_ = true;
    precision_timer_ = timers_.start(kPrecisionCheckMs, true, [this](RuntimeSkillTimerIdV1 timer, std::string& timer_error) -> bool {
        // checkbuff: no bow and a buff -> RemoveBuff + StopTimerCB.
        if (!services_.has_bow || services_.has_bow() || !precision_buff_) return true;
        if (!precision_remove(timer_error)) return false;
        timers_.stop(timer);
        precision_timer_ = 0;
        return true;
    });
    log("Archer passive Precision buff applied rank=" + std::to_string(spec.strength));
    return true;
}

bool RuntimeArcherPassiveOwnerV1::precision_remove(std::string& error) {
    if (!precision_buff_) return true;
    precision_buff_ = false;
    if (precision_timer_ != 0) {
        timers_.stop(precision_timer_);
        precision_timer_ = 0;
    }
    if (!session_) return true;
    log("Archer passive Precision buff removed");
    return buffs_->remove_timed_buff(*session_, player_, kPrecisionClass, error);
}

void RuntimeArcherPassiveOwnerV1::precision_hit() {
    if (rank(ArcherPassiveKindV1::precision) == 0 || !session_) return;
    std::string error;
    const bool bow = services_.has_bow && services_.has_bow();
    if (bow) {
        if (!precision_buff_ && !precision_create(error)) error_ = "Precision: " + error;
    } else if (precision_buff_ && !precision_remove(error)) {
        error_ = "Precision: " + error;
    }
}

// ----- SideStep ----------------------------------------------------------------------------------------------------
bool RuntimeArcherPassiveOwnerV1::side_step_knockback(ActorId attacker, std::string& error) {
    if (!services_.knock_back) {
        error = "SideStep has no knockback sink";
        return false;
    }
    RuntimeSkillKnockBackRequestV1 request;
    request.attacker = player_;
    request.target = attacker;
    request.generation = (std::uint64_t(1) << 48) + (++next_generation_);  // a passive occurrence, distinct from skill casts
    request.event_index = 0;
    request.source_id = "SideStep/deflection";
    request.target_state_before_hit = session_->original_actor_state(attacker);
    request.great = true;  // SetKnockBackState(true)
    return services_.knock_back(*session_, request, error);
}

bool RuntimeArcherPassiveOwnerV1::side_step_check(RuntimeSkillTimerIdV1 timer, ActorId attacker, std::string& error) {
    error.clear();
    if (!session_) return true;
    ++side_step_checks_;
    std::string kb_error;
    const bool kb = side_step_knockback(attacker, kb_error);
    if (kb) ++side_step_knockbacks_;
    const auto state = session_->original_actor_state(attacker);
    log("Archer passive SideStep check attacker=" + std::to_string(attacker) + " knockback=" + (kb ? "requested" : "refused") +
        (kb ? std::string() : " (" + kb_error + ")") + " stateAfter=" + std::to_string(state));
    if (state != kStateAttack) {
        timers_.stop(timer);
        std::string stop_error;
        const auto* actor = session_->actor(attacker);
        if (actor && actor->alive() && !session_->script_stop_actor(attacker, stop_error))
            log("Archer passive SideStep Stop diagnostic: " + stop_error);
    }
    if (state == kStateKnockedBack) timers_.stop(timer);
    return true;
}

void RuntimeArcherPassiveOwnerV1::side_step_hit(const CombatResultEventV1& event) {
    const std::uint32_t level = rank(ArcherPassiveKindV1::side_step);
    if (level == 0 || !session_) return;
    const auto* attacker = session_->actor(event.attacker);
    const auto* player = session_->actor(player_);
    if (!attacker || !player || !attacker->alive()) return;
    // attacker:TargetInMeleeRange() = CharAI::AI_IsInMeleeRange(attacker AI, its current target) (IDA 0x3d6188): the 3D distance
    // between the two target positions is below the sum of both melee radii. The attacker just hit the player, so the player is
    // its target; the port's melee reach (PlayableActorWorld::melee_reach) is that radius; positions are the actor positions
    // (INFERENCE: GetTargetPosition node offsets are not applied).
    const auto* world = session_->world();
    const float reach = world ? world->melee_reach(attacker->id) + world->melee_reach(player->id) : 0.0f;
    const float dx = attacker->transform.position[0] - player->transform.position[0];
    const float dy = attacker->transform.position[1] - player->transform.position[1];
    const float dz = attacker->transform.position[2] - player->transform.position[2];
    const float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (!(distance < reach)) {
        log("Archer passive SideStep hit ignored: attacker " + std::to_string(event.attacker) + " is outside the melee reach (distance=" +
            std::to_string(distance) + " reach=" + std::to_string(reach) + ")");
        return;
    }
    dh2::data::PropertySheet sheet{};
    std::string error;
    if (!class_sheet(level, kSideStepClass, sheet, error)) {
        error_ = "SideStep class sheet: " + error;
        return;
    }
    const double knockback = std::floor(static_cast<double>(sheet[kTempProp1]) / 256.0);  // FromFixed(TempProp1)
    std::uint32_t roll = 0;
    if (!services_.random || !services_.random(101, roll, error)) {  // GetRand(0, 100)
        error_ = "SideStep roll: " + error;
        return;
    }
    log("Archer passive SideStep hit attacker=" + std::to_string(event.attacker) + " roll=" + std::to_string(roll) +
        " chance=" + std::to_string(knockback));
    if (static_cast<double>(roll) >= knockback) return;
    ++side_step_procs_;
    const ActorId attacker_id = event.attacker;
    timers_.start(kSideStepCheckMs, true, [this, attacker_id](RuntimeSkillTimerIdV1 timer, std::string& timer_error) -> bool {
        return side_step_check(timer, attacker_id, timer_error);
    });
}

// ----- bus + rank --------------------------------------------------------------------------------------------------
void RuntimeArcherPassiveOwnerV1::on_hit(const CombatResultEventV1& event, ActorId receiver) {
    if (receiver != player_) return;
    if (event.attacker == player_) precision_hit();
    else if (event.target == player_) side_step_hit(event);
}

bool RuntimeArcherPassiveOwnerV1::set_rank(ArcherPassiveKindV1 kind, std::uint32_t rank_value, std::string& error) {
    error.clear();
    const auto previous = ranks_.find(kind);
    const std::uint32_t before = previous == ranks_.end() ? 0u : previous->second;
    if (before == rank_value) return true;  // the host syncs every frame: an unchanged rank is not an OnSkillUpdate_
    if (rank_value == 0) ranks_.erase(kind);
    else ranks_[kind] = rank_value;
    if (kind != ArcherPassiveKindV1::precision) return true;  // SideStep only caches the level (KNOCKBACK is read at hit time)
    // OnSkillUpdate_: RemoveBuff + StopTimerCB, then with a bow and a non-zero rank CreateBuff + ApplyBuff + timer.
    if (!precision_remove(error)) return false;
    if (rank_value != 0 && services_.has_bow && services_.has_bow()) return precision_create(error);
    return true;
}

bool RuntimeArcherPassiveOwnerV1::advance(CombatSession& session, double dt_ms, std::string& error) {
    error.clear();
    session_ = &session;
    // INFERENCE: the original re-runs every skill's OnSkillUpdate_ when the character's equipment changes (the saved Archer starts
    // with the bow already in hand, so there is no rank change to trigger it). A bow appearing in the main hand therefore runs
    // OnSkillUpdate_ for Precision; losing it is left to the Lua's own 1000 ms checkbuff.
    const bool bow = services_.has_bow && services_.has_bow();
    if (bow && !bow_seen_ && rank(ArcherPassiveKindV1::precision) != 0 && !precision_buff_) {
        if (!precision_create(error)) return false;
    }
    bow_seen_ = bow;
    return timers_.advance(dt_ms, error);
}

bool sync_archer_passive_ranks_v1(RuntimeArcherPassiveOwnerV1& owner, const CharacterState& state,
                                  const dh2::data::SkillTables::Borrow& tables, std::string& error) {
    error.clear();
    if (!tables) {
        error = "Archer passive sync requires the SkillTable snapshot";
        return false;
    }
    const auto& records = tables.skills();
    std::map<ArcherPassiveKindV1, std::uint32_t> learned;
    for (const auto& progress : state.skills) {
        const auto index = tables.skill_index(progress.id.c_str());
        if (index < 0 || static_cast<std::size_t>(index) >= records.size()) continue;
        const auto kind = archer_passive_kind_for_script_v1(records[static_cast<std::size_t>(index)].script);
        if (kind != ArcherPassiveKindV1::none) learned[kind] = progress.rank;
    }
    for (const auto kind : {ArcherPassiveKindV1::precision, ArcherPassiveKindV1::side_step}) {
        const auto found = learned.find(kind);
        if (!owner.set_rank(kind, found == learned.end() ? 0u : found->second, error)) return false;
    }
    return true;
}

} // namespace dh::foundation::generic_skills
