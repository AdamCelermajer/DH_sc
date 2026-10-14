#include "runtime_rogue_skill_owner_v1.hpp"

#include "../../combat_session.hpp"

#include <algorithm>
#include <cmath>
#include <string>

namespace dh::foundation::generic_skills {

namespace {
bool fail(std::string& error, const std::string& message) {
    error = message;
    return false;
}

bool field_index(const dh2::data::CharacterTable* table, const char* name, std::size_t& index, std::string& error) {
    if (!table) return fail(error, "Rogue skill owner requires the source CharacterProperties table");
    const auto found = std::find(table->fields.begin(), table->fields.end(), name);
    if (found == table->fields.end()) return fail(error, std::string("Original CharacterProperties field is unavailable: ") + name);
    index = static_cast<std::size_t>(found - table->fields.begin());
    if (index >= dh2::data::PropertySheet{}.size()) return fail(error, std::string("Property field exceeds the source sheet: ") + name);
    return true;
}

std::int32_t projectile_row(const dh2::android_ui::SourceProcessArraysV101& tables, const char* name) {
    const auto* group = tables.group("ProjectileTable");
    if (!group) return -1;
    for (std::size_t row = 0; row < group->names.size(); ++row)
        if (group->names[row] == name) return static_cast<std::int32_t>(row);
    return -1;
}

// The caster's heading as the forward vector (the same convention as the Archer and Mage launches).
std::array<float, 3> look_vector(const ActorState& actor) {
    const float heading = actor.transform.rotation[2];
    return {std::sin(heading), -std::cos(heading), 0.0f};
}

bool eligible(CombatSession& session, ActorId caster, ActorId target) {
    const auto* world = session.world();
    const auto* attacker = session.actor(caster);
    const auto* victim = session.actor(target);
    return world && attacker && victim && target != caster && world->eligible_target(*attacker, *victim);
}

constexpr const char* kKnifeSound = "sfx_skill_rogue_ass_throwing_knife";  // Lua PlaySound label (sounds.xml uid 139)
constexpr const char* kKnifeRow = "GrapplingHook";                         // ProjectileTable row (Lua PROJECTILE_ID)
// Arrival radius of the pull (not in the Lua; Cmd_HeadTo arrival is not recovered). The pulled actor stops this far from the
// caster centre, roughly a body-contact distance.
constexpr float kPullArriveRadius = 100.0f;
} // namespace

struct RuntimeRogueSkillOwnerV1::Cast {
    ActorId caster = invalid_actor_id;
    std::uint64_t generation = 0;
    std::string skill_name;
    std::uint32_t row_mask = 0;
    std::int32_t element = -1, category = -1;
    dh2::data::PropertySheet sheet{};  // the Use-time ApplyPropClass(CLASS) sheet (SetProp(SnS_Level) + class projection)
    double stun_ms = 0.0;              // Lua `duration` = FromFixed(GetProp(SnS_Duration, true))
    std::uint32_t next_event = 0;
};

RuntimeRogueSkillOwnerV1::RuntimeRogueSkillOwnerV1(RuntimeRogueSkillServicesV1 services)
    : services_(std::move(services)), pulls_(pull_services()) {}

void RuntimeRogueSkillOwnerV1::log(const std::string& line) const {
    if (services_.trace) services_.trace(line);
}

void RuntimeRogueSkillOwnerV1::reset() {
    pulls_.clear();
    pull_casters_.clear();
    pull_goals_.clear();
    knives_.clear();
    casts_.clear();
}

bool RuntimeRogueSkillOwnerV1::handles(const RuntimeSkillUseProfileV1& profile) const {
    return std::string(profile.skill_name) == "GrapplingHook";
}

ForcedHeadToServicesV1 RuntimeRogueSkillOwnerV1::pull_services() {
    ForcedHeadToServicesV1 services;
    services.position_of = [this](ActorId actor, std::array<float, 3>& position) {
        const auto* live = session_ ? session_->actor(actor) : nullptr;
        if (!live || !live->alive()) return false;
        position = live->transform.position;
        return true;
    };
    services.move = [this](ActorId actor, const std::array<float, 3>& position, std::string& error) {
        if (!services_.move_actor) return fail(error, "Rogue pull has no actor move service");
        return services_.move_actor(actor, position, error);
    };
    // Lua: the pull runs while the target is in the Stunned state (GetState() == AIStates.Stunned).
    services.may_continue = [this](ActorId actor) { return services_.is_stunned && services_.is_stunned(actor); };
    return services;
}

// ---------------------------------------------------------------------------------------------------------------------
// Pre: the generic OnPre query, LookAt and UseMana ran before this; the Lua OnPre has nothing else.
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeRogueSkillOwnerV1::pre(CombatSession&, const RuntimeSkillExternalContextV1& context, std::string& error) {
    error.clear();
    (void)context;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
// Use: OnSkill_ = PlaySound + SpawnSkillProjectile(GrapplingHook). The rest happens in the delayed body.
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeRogueSkillOwnerV1::use(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                   std::string& error) {
    error.clear();
    if (!context.profile || !context.formula_sheet)
        return fail(error, "Rogue skill Use requires its profile and ApplyPropClass formula sheet");
    if (!handles(*context.profile)) return fail(error, "Rogue skill owner does not handle " + std::string(context.profile->skill_name));
    auto cast = std::make_shared<Cast>();
    cast->caster = context.caster;
    cast->generation = context.generation;
    cast->skill_name = context.skill_name;
    cast->row_mask = context.row_mask;
    cast->element = context.element;
    cast->category = context.category;
    cast->sheet = *context.formula_sheet;
    std::size_t duration_index = 0;
    if (!field_index(context.characters, "SnS_Duration", duration_index, error)) return false;
    cast->stun_ms = static_cast<double>(cast->sheet[duration_index]) / 256.0;
    casts_[context.caster] = cast;

    std::string sound_error;
    if (services_.play_sound && !services_.play_sound(kKnifeSound, sound_error))
        log("Rogue skill diagnostic GrapplingHook sound: " + sound_error);
    else log(std::string("Rogue skill GrapplingHook sound label=") + kKnifeSound);
    return launch_knife(session, cast, error);
}

bool RuntimeRogueSkillOwnerV1::launch_knife(CombatSession& session, const std::shared_ptr<Cast>& cast, std::string& error) {
    auto* executor = services_.projectiles ? services_.projectiles() : nullptr;
    const auto tables = services_.projectile_tables ? services_.projectile_tables() : nullptr;
    if (!executor || !tables) return fail(error, "GrapplingHook requires the shared projectile executor and its tables");
    const auto row = projectile_row(*tables, kKnifeRow);
    if (row < 0) return fail(error, "GrapplingHook ProjectileTable row is missing");
    const auto* caster = session.actor(cast->caster);
    if (!caster) return fail(error, "GrapplingHook caster is absent");

    effects::RuntimeSessionSkillProjectileLaunchV1 request;
    request.owner = cast->caster;
    request.generation = cast->generation;
    request.occurrence = next_occurrence_++;
    request.row = row;
    request.origin = caster->transform.position;
    request.forward = look_vector(*caster);
    request.explicit_target = invalid_actor_id;  // SpawnSkillProjectile(PROJECTILE_ID): no SetProjectileTarget
    // Lua RegisterSkillEx has no delayed_check, so every collision is a hit (native default). The delayed body is below.
    CombatSession* owner_session = &session;
    request.delayed = [this, cast, owner_session](const effects::RuntimeSessionSkillProjectileEventV1& event,
                                                  std::string& callback_error) {
        return on_delayed(*owner_session, cast, event, callback_error);
    };
    std::uint64_t projectile_id = 0;
    if (!executor->launch_skill(tables, request, projectile_id, error)) return false;
    log("Rogue skill GrapplingHook projectile launched id=" + std::to_string(projectile_id) + " row=" + std::to_string(row) +
        " caster=" + std::to_string(cast->caster));
    knives_[projectile_id] = cast->caster;
    if (services_.projectile_launched && !services_.projectile_launched(projectile_id, true, error)) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
// Shared roll: SkillCombatRoll(target) with the Use-time formula sheet (the attacker side of F_ApplyResult).
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeRogueSkillOwnerV1::roll(CombatSession& session, const std::shared_ptr<Cast>& cast, ActorId target,
                                    DamageEvent& result, std::string& error) {
    CombatSessionSourceHit hit;
    hit.attacker = cast->caster;
    hit.target = target;
    hit.binding_lease = session.actor_binding_lease();
    hit.generation = cast->generation;
    hit.event_index = cast->next_event++;
    hit.mask = cast->row_mask | 0x08000000u;  // native SkillCombatRoll SkillAttack bit
    hit.source_id = cast->skill_name + "/knife";
    hit.marker_name = "do_skill";
    hit.category = cast->category;
    hit.element = cast->element;
    hit.direct_amount = 0;
    hit.attacker_formula_sheet = &cast->sheet;
    return session.apply_source_result(hit, result, error);
}

// ---------------------------------------------------------------------------------------------------------------------
// Delayed (OnDelayedSkill_ target): the authored order is roll, stun (not boss/miniboss), pull while Stunned.
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeRogueSkillOwnerV1::on_delayed(CombatSession& session, const std::shared_ptr<Cast>& cast,
                                          const effects::RuntimeSessionSkillProjectileEventV1& event, std::string& error) {
    error.clear();
    if (event.expired || event.peer == invalid_actor_id) return true;  // Lua: delayed(nil, projectile) does nothing
    if (!eligible(session, cast->caster, event.peer)) return true;
    DamageEvent result;
    if (!roll(session, cast, event.peer, result, error)) return false;
    log("Rogue skill GrapplingHook hit victim=" + std::to_string(event.peer) + " damage=" +
        std::to_string(result.health_removed) + " requested=" + std::to_string(result.requested_damage) +
        " died=" + std::to_string(result.target_died));

    // IsCharacter(): an actor with combat properties (monsters and the player). Objects get the roll only.
    const bool character = session.world() && session.world()->combat_properties(event.peer) != nullptr;
    if (!character) return true;
    const bool boss_or_mini = services_.is_boss_or_miniboss && services_.is_boss_or_miniboss(event.peer);
    if (!boss_or_mini) {
        // Lua SetStunState(duration, true). The status owner refuses bosses and minibosses itself; this gate is the Lua one.
        bool applied = false;
        if (!services_.set_stun) return fail(error, "GrapplingHook has no stun service");
        if (!services_.set_stun(event.peer, cast->stun_ms, applied, error)) return false;
        log("Rogue skill GrapplingHook stun victim=" + std::to_string(event.peer) + " ms=" + std::to_string(cast->stun_ms) +
            " applied=" + std::to_string(applied ? 1 : 0));
    }
    // Lua: if GetState() == Stunned and not boss/miniboss then HeadTo(GetPosition()): the caster's position at this moment.
    const auto* caster = session.actor(cast->caster);
    if (!caster) return fail(error, "GrapplingHook caster is absent for the pull");
    const bool stunned = services_.is_stunned && services_.is_stunned(event.peer);
    if (stunned && !boss_or_mini) {
        const float arrive = kPullArriveRadius;
        pulls_.start(event.peer, caster->transform.position, kPullSpeedPerSecond, arrive);
        pull_casters_[event.peer] = cast->caster;
        pull_goals_[event.peer] = caster->transform.position;
        log("Rogue skill GrapplingHook pull start victim=" + std::to_string(event.peer) + " goal=" +
            std::to_string(caster->transform.position[0]) + "," + std::to_string(caster->transform.position[1]) +
            " arrive=" + std::to_string(arrive) + " distanceToGoal=" + std::to_string(std::hypot(caster->transform.position[0] - session.actor(event.peer)->transform.position[0], caster->transform.position[1] - session.actor(event.peer)->transform.position[1])));
    }
    return true;
}

bool RuntimeRogueSkillOwnerV1::advance(CombatSession& session, double dt_ms, std::string& error) {
    error.clear();
    ++pull_frames_;
    // Knife flight (sampled): the executor position of each live knife, dropped when the executor no longer has it.
    if (auto* executor = services_.projectiles ? services_.projectiles() : nullptr) {
        for (auto it = knives_.begin(); it != knives_.end();) {
            std::array<float, 3> at{};
            if (!executor->projectile_position(it->first, at)) {
                log("Rogue skill GrapplingHook knife gone id=" + std::to_string(it->first));
                it = knives_.erase(it);
                continue;
            }
            if (pull_frames_ % 4 == 0) {
                const auto* caster = session.actor(it->second);
                const double from_caster = caster ? std::hypot(at[0] - caster->transform.position[0], at[1] - caster->transform.position[1]) : -1.0;
                log("Rogue skill GrapplingHook knife flight id=" + std::to_string(it->first) + " frame=" + std::to_string(pull_frames_) +
                    " at=" + std::to_string(at[0]) + "," + std::to_string(at[1]) + " fromCaster=" + std::to_string(from_caster));
            }
            ++it;
        }
    }
    // Pulls: the live commands, their end (arrival, refusal, death) and a sampled step trace against the fixed goal.
    const auto live_before = pulls_.actors();
    session_ = &session;
    const bool ok = pulls_.advance(dt_ms, error);
    session_ = nullptr;
    for (const auto victim : live_before) {
        const auto* live = session.actor(victim);
        const auto goal_it = pull_goals_.find(victim);
        const double distance = (live && goal_it != pull_goals_.end())
            ? std::hypot(live->transform.position[0] - goal_it->second[0], live->transform.position[1] - goal_it->second[1])
            : -1.0;
        if (!pulls_.active(victim)) {
            log("Rogue skill GrapplingHook pull end victim=" + std::to_string(victim) + " end=" + pulls_.last_end(victim) +
                " distanceToGoal=" + std::to_string(distance) + " frame=" + std::to_string(pull_frames_));
            pull_casters_.erase(victim);
            pull_goals_.erase(victim);
        } else if (pull_frames_ % 2 == 0) {
            log("Rogue skill GrapplingHook pull step victim=" + std::to_string(victim) + " frame=" + std::to_string(pull_frames_) +
                " distanceToGoal=" + std::to_string(distance));
        }
    }
    if (!ok) return false;
    return true;
}

} // namespace dh::foundation::generic_skills
