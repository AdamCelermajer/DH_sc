#include "runtime_archer_skill_owner_v1.hpp"

#include <algorithm>
#include <cmath>

namespace dh::foundation::generic_skills {
namespace {
constexpr float kPi = 3.1415927410125732421875f;

bool fail(std::string& error, const std::string& message) {
    error = message;
    return false;
}

double fixed(std::int32_t raw) { return static_cast<double>(raw) / 256.0; }

bool field_index(const dh2::data::CharacterTable* table, const char* name, std::size_t& index, std::string& error) {
    if (!table) return fail(error, "Archer skill owner requires the source CharacterProperties table");
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

// GameObject::GetTargetPosition (0x3935dc), the same choice runtime_skill_target_query_v1.cpp makes.
std::array<float, 3> target_position(const ActorState& actor) {
    if (actor.source_target_node180 && *actor.source_target_node180 != 0 && actor.source_target_position184)
        return *actor.source_target_position184;
    return actor.transform.position;
}

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

constexpr const char* kExplosionFx = "spell_dh2_prince_rogue_burst_shot";
constexpr const char* kExplosionSound = "sfx_impact_burst_shot";
constexpr double kExplosionDelayMs = 500.0;     // StartSkillTimerCB(500, ...)
constexpr float kDefaultExplosionRange = 200.0f; // RANGE_EXPL until OnSkillUpdate_ reads TempProp1
constexpr unsigned kMaxExplosionVictims = 3;    // MAX_COUNT
constexpr double kPinSwitchDelayMs = 750.0;     // SWITCH_TARGET_DELAY
constexpr float kReboundRange = 900.0f;         // Volley RANGE
} // namespace

struct RuntimeArcherSkillOwnerV1::Cast {
    ActorId caster = invalid_actor_id;
    std::uint64_t generation = 0;
    std::string skill_name;
    std::uint32_t rank = 0, row_mask = 0;
    std::int32_t element = -1, category = -1;
    dh2::data::PropertySheet sheet{};
    const dh2::data::CharacterTable* characters = nullptr;
    std::vector<ActorId> list;      // the OnPre target list (OnSkill_ pops it)
    std::size_t popped = 0;
    std::vector<ActorId> population_order;
    std::vector<SkillTargetSourceFactsV1> target_facts;
    std::optional<bool> objects_absent;
    std::uint32_t next_event = 0;
    // MultiHitArrow
    std::size_t pot_targets = 0;
    int hit = 0;
    // PinShot
    ActorId first_target = invalid_actor_id, second_target = invalid_actor_id;
    // Volley
    ActorId old_target = invalid_actor_id, new_target = invalid_actor_id;
    std::uint64_t rebound_projectile = 0;
    double chances = 0.0;
};

RuntimeArcherSkillOwnerV1::RuntimeArcherSkillOwnerV1(RuntimeArcherSkillServicesV1 services)
    : services_(std::move(services)) {}

void RuntimeArcherSkillOwnerV1::log(const std::string& line) const {
    if (services_.trace) services_.trace(line);
}

void RuntimeArcherSkillOwnerV1::reset() {
    timers_.clear();
    casts_.clear();
}

bool RuntimeArcherSkillOwnerV1::handles(const RuntimeSkillUseProfileV1& profile) const {
    const std::string name = profile.skill_name;
    return name == "ExplodingArrows" || name == "MultiHitArrow" || name == "PinShot" || name == "PoisonArrows" ||
           name == "Volley";
}

RuntimeArcherSkillOwnerV1::Cast& RuntimeArcherSkillOwnerV1::cast_for(const RuntimeSkillExternalContextV1& context) {
    auto& slot = casts_[context.caster];
    if (!slot || slot->generation != context.generation) {
        // Pre was not seen for this generation (a profile override or a restored cast): start from the Use-time list.
        slot = std::make_shared<Cast>();
        slot->caster = context.caster;
        slot->generation = context.generation;
        if (context.ordered_character_targets) slot->list = *context.ordered_character_targets;
        slot->pot_targets = slot->list.size();
    }
    auto& cast = *slot;
    cast.skill_name = context.skill_name;
    cast.rank = context.rank;
    cast.row_mask = context.row_mask;
    cast.element = context.element;
    cast.category = context.category;
    cast.characters = context.characters;
    if (context.formula_sheet) cast.sheet = *context.formula_sheet;
    cast.population_order = context.population_order ? *context.population_order : std::vector<ActorId>{};
    cast.target_facts = context.target_facts ? *context.target_facts : std::vector<SkillTargetSourceFactsV1>{};
    cast.objects_absent = context.non_character_attackable_objects_absent;
    return cast;
}

ActorId RuntimeArcherSkillOwnerV1::pop_target(Cast& cast, const RuntimeSkillExternalContextV1&) const {
    // PopTargetList removes the top of the Pre list; every OnSkill_ call (one per do_skill marker) pops once.
    if (cast.popped < cast.list.size()) return cast.list[cast.popped++];
    return invalid_actor_id;
}

// ---------------------------------------------------------------------------------------------------------------------
// Pre: the generic prefix already ran the target query, LookAt(top), UseMana and SetSkillCooldown. The Lua bodies keep
// their state in file-level locals: MultiHitArrow pot_targets/hit, Volley oldTarget/newTarget.
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeArcherSkillOwnerV1::pre(CombatSession&, const RuntimeSkillExternalContextV1& context, std::string& error) {
    error.clear();
    auto cast = std::make_shared<Cast>();
    cast->caster = context.caster;
    cast->generation = context.generation;
    cast->skill_name = context.skill_name;
    if (context.ordered_character_targets) cast->list = *context.ordered_character_targets;
    cast->pot_targets = cast->list.size();  // MultiHitArrow: pot_targets = GetTargetListSize()
    casts_[context.caster] = std::move(cast);
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
// shared helpers
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeArcherSkillOwnerV1::roll(CombatSession& session, const std::shared_ptr<Cast>& cast, ActorId target,
                                     DamageEvent& result, std::string& error) {
    CombatSessionSourceHit hit;
    hit.attacker = cast->caster;
    hit.target = target;
    hit.binding_lease = session.actor_binding_lease();
    hit.generation = cast->generation;
    hit.event_index = cast->next_event++;
    hit.mask = cast->row_mask | 0x08000000u;  // native SkillCombatRoll SkillAttack bit
    hit.source_id = cast->skill_name + "/arrow";
    hit.marker_name = "do_skill";
    hit.category = cast->category;
    hit.element = cast->element;
    hit.direct_amount = 0;
    hit.attacker_formula_sheet = &cast->sheet;
    return session.apply_source_result(hit, result, error);
}

// Lua target:SetKnockBackState(great): an explicit Push request through the host's same-Session Push sink. The pre-hit
// original state is the admission state, captured before the roll like the coordinator's own explicit knockback.
bool RuntimeArcherSkillOwnerV1::knock(CombatSession& session, const std::shared_ptr<Cast>& cast, ActorId target, bool great,
                                      std::string& error) {
    if (!services_.knock_back) return fail(error, "Archer skill has no knockback sink");
    RuntimeSkillKnockBackRequestV1 request;
    request.attacker = cast->caster;
    request.target = target;
    request.generation = cast->generation;
    request.event_index = cast->next_event;  // the roll that follows takes the same index (the Push retires per occurrence)
    request.source_id = cast->skill_name + "/arrow";
    request.target_state_before_hit = session.original_actor_state(target);
    request.great = great;
    return services_.knock_back(session, request, error);
}

bool RuntimeArcherSkillOwnerV1::look_at(CombatSession& session, ActorId caster, ActorId target, std::string& error) {
    SkillActorTargetQueryV1 query;
    query.status = SkillActorTargetQueryStatusV1::selected_character;
    query.caster = caster;
    query.selected = target;
    return look_at_bashdown_target_v1(session, query, error);
}

bool RuntimeArcherSkillOwnerV1::launch(CombatSession& session, const std::shared_ptr<Cast>& cast, const char* row_name,
                                       const std::array<float, 3>& origin, const std::array<float, 3>& forward,
                                       ActorId explicit_target,
                                       effects::RuntimeSessionSkillProjectileDelayedV1 delayed,
                                       std::uint64_t& projectile_id, std::string& error) {
    (void)session;
    auto* executor = services_.projectiles ? services_.projectiles() : nullptr;
    const auto tables = services_.projectile_tables ? services_.projectile_tables() : nullptr;
    if (!executor || !tables) return fail(error, "Archer skill requires the shared projectile executor and its tables");
    const auto row = projectile_row(*tables, row_name);
    if (row < 0) return fail(error, std::string("Archer ProjectileTable row is missing: ") + row_name);
    effects::RuntimeSessionSkillProjectileLaunchV1 request;
    request.owner = cast->caster;
    request.generation = cast->generation;
    request.occurrence = next_occurrence_++;
    request.row = row;
    request.origin = origin;
    request.forward = forward;
    request.explicit_target = explicit_target;
    request.delayed = std::move(delayed);
    if (!executor->launch_skill(tables, request, projectile_id, error)) return false;
    log(std::string("Archer skill projectile launched skill=") + cast->skill_name + " id=" + std::to_string(projectile_id) +
        " row=" + row_name + " (" + std::to_string(row) + ") forward=" + std::to_string(forward[0]) + "," +
        std::to_string(forward[1]));
    return !services_.projectile_launched || services_.projectile_launched(projectile_id, true, error);
}

bool RuntimeArcherSkillOwnerV1::launch_from_heading(CombatSession& session, const std::shared_ptr<Cast>& cast,
                                                    const char* row_name,
                                                    effects::RuntimeSessionSkillProjectileDelayedV1 delayed,
                                                    std::uint64_t& projectile_id, std::string& error) {
    // SpawnSkillProjectile(row): no target, the arrow flies along the owner's current heading from the owner.
    const auto* caster = session.actor(cast->caster);
    if (!caster) return fail(error, "Archer skill caster is absent");
    return launch(session, cast, row_name, caster->transform.position, look_vector(*caster), invalid_actor_id,
                  std::move(delayed), projectile_id, error);
}

// ---------------------------------------------------------------------------------------------------------------------
// Use
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeArcherSkillOwnerV1::use(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                    std::string& error) {
    error.clear();
    if (!context.profile || !context.formula_sheet)
        return fail(error, "Archer skill Use requires its profile and ApplyPropClass formula sheet");
    auto& raw = cast_for(context);
    const auto cast = casts_[context.caster];
    (void)raw;
    const std::string name = context.profile->skill_name;
    if (name == "ExplodingArrows") return use_exploding(session, cast, context, error);
    if (name == "MultiHitArrow") return use_multihit(session, cast, context, error);
    if (name == "PinShot") return use_pin_shot(session, cast, context, error);
    if (name == "PoisonArrows") return use_simple(session, cast, context, "ArrowPoison", error);
    if (name == "Volley") return use_volley(session, cast, context, error);
    return fail(error, "Archer skill owner does not handle " + name);
}

// ----- PoisonArrows ---------------------------------------------------------------------------------------------------
// OnSkill_: pop the target (if any), SpawnSkillProjectile(ArrowPoison). Delayed: SetProp(SnS_Level) + ApplyPropClass +
// SkillCombatRoll. The DOT is whatever the class sheet carries (the Lua adds nothing; its author note says it does not work).
bool RuntimeArcherSkillOwnerV1::use_simple(CombatSession& session, const std::shared_ptr<Cast>& cast,
                                           const RuntimeSkillExternalContextV1& context, const char* row,
                                           std::string& error) {
    pop_target(*cast, context);
    CombatSession* owner_session = &session;
    std::uint64_t id = 0;
    return launch_from_heading(session, cast, row, [this, cast, owner_session](const effects::RuntimeSessionSkillProjectileEventV1& event,
                                                                               std::string& callback_error) {
        if (event.expired || event.peer == invalid_actor_id) return true;
        if (!eligible(*owner_session, cast->caster, event.peer)) return true;
        DamageEvent result;
        if (!roll(*owner_session, cast, event.peer, result, callback_error)) return false;
        log(cast->skill_name + " arrow hit victim=" + std::to_string(event.peer) + " damage=" + std::to_string(result.health_removed) +
            " requested=" + std::to_string(result.requested_damage) + " died=" + std::to_string(result.target_died));
        return true;
    }, id, error);
}

// ----- MultiHitArrow --------------------------------------------------------------------------------------------------
// OnSkill_ runs once per do_skill marker (three): pop, SpawnSkillProjectile(ArrowDefault). Delayed: hit++; the third hit
// with exactly one potential target is a great knockback, hits 1-2 with exactly two potential targets a standard one;
// then SkillCombatRoll.
bool RuntimeArcherSkillOwnerV1::use_multihit(CombatSession& session, const std::shared_ptr<Cast>& cast,
                                             const RuntimeSkillExternalContextV1& context, std::string& error) {
    pop_target(*cast, context);
    CombatSession* owner_session = &session;
    std::uint64_t id = 0;
    return launch_from_heading(session, cast, "ArrowDefault", [this, cast, owner_session](const effects::RuntimeSessionSkillProjectileEventV1& event,
                                                                                         std::string& callback_error) {
        if (event.expired || event.peer == invalid_actor_id) return true;
        if (!eligible(*owner_session, cast->caster, event.peer)) return true;
        ++cast->hit;
        std::string kb_error;
        if (cast->hit == 3 && cast->pot_targets == 1) {
            if (services_.knock_back && !knock(*owner_session, cast, event.peer, true, kb_error))
                log("Archer skill diagnostic MultiHitArrow great knockback: " + kb_error);
            else log("Archer skill MultiHitArrow great knockback requested victim=" + std::to_string(event.peer));
        }
        if (cast->hit < 3 && cast->pot_targets == 2) {
            if (services_.knock_back && !knock(*owner_session, cast, event.peer, false, kb_error))
                log("Archer skill diagnostic MultiHitArrow standard knockback: " + kb_error);
            else log("Archer skill MultiHitArrow standard knockback requested victim=" + std::to_string(event.peer));
        }
        DamageEvent result;
        if (!roll(*owner_session, cast, event.peer, result, callback_error)) return false;
        log("MultiHitArrow arrow hit n=" + std::to_string(cast->hit) + " victim=" + std::to_string(event.peer) +
            " damage=" + std::to_string(result.health_removed) + " requested=" + std::to_string(result.requested_damage) +
            " died=" + std::to_string(result.target_died));
        return true;
    }, id, error);
}

// ----- ExplodingArrows ------------------------------------------------------------------------------------------------
bool RuntimeArcherSkillOwnerV1::use_exploding(CombatSession& session, const std::shared_ptr<Cast>& cast,
                                              const RuntimeSkillExternalContextV1& context, std::string& error) {
    pop_target(*cast, context);
    CombatSession* owner_session = &session;
    std::uint64_t id = 0;
    return launch_from_heading(session, cast, "ArrowExplode", [this, cast, owner_session](const effects::RuntimeSessionSkillProjectileEventV1& event,
                                                                                        std::string& callback_error) {
        if (event.expired || event.peer == invalid_actor_id) return true;
        if (!eligible(*owner_session, cast->caster, event.peer)) return true;
        DamageEvent result;
        if (!roll(*owner_session, cast, event.peer, result, callback_error)) return false;
        log("ExplodingArrows first hit victim=" + std::to_string(event.peer) + " damage=" + std::to_string(result.health_removed) +
            " requested=" + std::to_string(result.requested_damage) + " died=" + std::to_string(result.target_died));
        // local timer = StartSkillTimerCB(500, 0, DoExplosion, target): one shot on this owner's clock.
        const ActorId victim = event.peer;
        const auto timer = timers_.start(kExplosionDelayMs, false, [this, cast, victim](RuntimeSkillTimerIdV1, std::string& timer_error) {
            if (!timer_session_) return fail(timer_error, "ExplodingArrows DoExplosion fired outside the Archer owner clock");
            return explode(*timer_session_, cast, victim, timer_error);
        });
        if (timer == 0) return fail(callback_error, "ExplodingArrows explosion timer could not be started");
        return true;
    }, id, error);
}

bool RuntimeArcherSkillOwnerV1::explode(CombatSession& session, const std::shared_ptr<Cast>& cast, ActorId target,
                                        std::string& error) {
    // DoExplosion: ClosestFirst TargetListSearch(RANGE_EXPL, 360, target); SetProp(SnS_Level) + ApplyPropClass(CLASS_ID);
    // target:PlayFX(burst_shot); PlaySound(sfx_impact_burst_shot); up to MAX_COUNT victims each get SkillCombatRoll.
    const auto* origin_actor = session.actor(target);
    if (!origin_actor) return true;  // the target left the Session: nothing to centre the explosion on
    std::size_t temp1 = 0;
    float range = kDefaultExplosionRange;
    if (field_index(cast->characters, "TempProp1", temp1, error)) {
        const double configured = fixed(cast->sheet[temp1]);
        if (configured > 0.0) range = static_cast<float>(configured);
    } else error.clear();
    SkillTargetOriginV1 origin;
    origin.position = target_position(*origin_actor);
    origin.look = look_vector(*origin_actor);
    origin.radius = 0.0f;
    SkillActorTargetQueryV1 found;
    if (!query_source_character_targets_v1(session, cast->caster, cast->population_order, cast->target_facts,
            cast->objects_absent, range, kPi, SkillTargetSortV1::closest_first, found, error, nullptr, &origin)) return false;
    std::string presentation_error;
    if (services_.play_effect && !services_.play_effect(kExplosionFx, origin.position, presentation_error))
        log("Archer skill diagnostic ExplodingArrows FX: " + presentation_error);
    else log(std::string("Archer skill ExplodingArrows explosion FX ") + kExplosionFx);
    if (services_.play_sound && !services_.play_sound(kExplosionSound, presentation_error))
        log("Archer skill diagnostic ExplodingArrows sound: " + presentation_error);
    log("ExplodingArrows explosion range=" + std::to_string(range) + " targets=" + std::to_string(found.ordered_character_targets.size()));
    unsigned count = 0;
    for (const auto victim : found.ordered_character_targets) {
        if (eligible(session, cast->caster, victim)) {
            DamageEvent result;
            if (!roll(session, cast, victim, result, error)) return false;
            log("ExplodingArrows explosion hit victim=" + std::to_string(victim) + " damage=" + std::to_string(result.health_removed) +
                " requested=" + std::to_string(result.requested_damage) + " died=" + std::to_string(result.target_died));
        }
        if (++count == kMaxExplosionVictims) break;
    }
    return true;
}

// ----- PinShot --------------------------------------------------------------------------------------------------------
// OnSkill_: first arrow at the top target now; with more than one target and skill level > 4 a second PinShot timer
// (750 ms), with more than two and level > 9 a third (1500 ms). LookAt(target) runs at Use for each selected target, and
// every spawn uses the owner's heading at spawn time (the Lua does not pass the target), so a timed arrow flies along the
// heading of the LAST LookAt. Delayed: dedupe by target (first / second only), SkillCombatRoll, then SetStunState.
bool RuntimeArcherSkillOwnerV1::use_pin_shot(CombatSession& session, const std::shared_ptr<Cast>& cast,
                                             const RuntimeSkillExternalContextV1& context, std::string& error) {
    cast->first_target = cast->second_target = invalid_actor_id;
    std::size_t duration_index = 0;
    if (!field_index(cast->characters, "SnS_Duration", duration_index, error)) return false;
    const double duration_ms = fixed(cast->sheet[duration_index]);  // duration = FromFixed(GetProp(SnS_Duration, true)) (ms)
    CombatSession* owner_session = &session;
    const auto delayed = [this, cast, owner_session, duration_ms](const effects::RuntimeSessionSkillProjectileEventV1& event,
                                                                  std::string& callback_error) {
        if (event.expired || event.peer == invalid_actor_id) return true;
        bool can_attack = false;
        if (cast->first_target != invalid_actor_id) {
            if (event.peer != cast->first_target) {
                if (cast->second_target == invalid_actor_id) {
                    cast->second_target = event.peer;
                    can_attack = true;
                }  // else: the second target is already known, or a third target that the Lua never tracks (skipped)
            }
        } else {
            cast->first_target = event.peer;
            can_attack = true;
        }
        if (!can_attack) {
            log("PinShot skipped victim=" + std::to_string(event.peer) + " (already attacked or not first/second)");
            return true;
        }
        if (!eligible(*owner_session, cast->caster, event.peer)) return true;
        DamageEvent result;
        if (!roll(*owner_session, cast, event.peer, result, callback_error)) return false;
        std::string stun_error;
        const bool stunned = services_.set_stun && services_.set_stun(event.peer, duration_ms, stun_error);
        log("PinShot arrow hit victim=" + std::to_string(event.peer) + " damage=" + std::to_string(result.health_removed) +
            " requested=" + std::to_string(result.requested_damage) + " stunMs=" + std::to_string(duration_ms) +
            " stunApplied=" + std::to_string(stunned) + (stun_error.empty() ? "" : " stunDiagnostic=" + stun_error));
        return true;
    };
    const auto list_at = [&](std::size_t index) -> ActorId {
        return index < cast->list.size() ? cast->list[index] : invalid_actor_id;
    };
    const std::size_t count = cast->list.size();
    std::uint64_t id = 0;
    if (count > 0) {
        const ActorId target = list_at(0);
        std::string look_error;
        if (!look_at(session, cast->caster, target, look_error)) log("Archer skill diagnostic PinShot LookAt: " + look_error);
        cast->popped = 1;
        if (!launch_from_heading(session, cast, "ArrowPinShot", delayed, id, error)) return false;
    }
    const auto timed_shot = [this, cast, delayed](RuntimeSkillTimerIdV1, std::string& timer_error) {
        if (!timer_session_) return fail(timer_error, "PinShot timer fired outside the Archer owner clock");
        // PlayAnim(skill_dh2_prince_rogue_archer_pinning_shot): the host animation replay is not reproduced (gap).
        std::uint64_t timed_id = 0;
        return launch_from_heading(*timer_session_, cast, "ArrowPinShot", delayed, timed_id, timer_error);
    };
    if (count > 1 && cast->rank > 4) {
        std::string look_error;
        if (!look_at(session, cast->caster, list_at(1), look_error)) log("Archer skill diagnostic PinShot LookAt: " + look_error);
        if (timers_.start(kPinSwitchDelayMs, false, timed_shot) == 0) return fail(error, "PinShot second-shot timer could not be started");
        cast->popped = 2;
    }
    if (count > 2 && cast->rank > 9) {
        std::string look_error;
        if (!look_at(session, cast->caster, list_at(2), look_error)) log("Archer skill diagnostic PinShot LookAt: " + look_error);
        if (timers_.start(kPinSwitchDelayMs + kPinSwitchDelayMs, false, timed_shot) == 0)
            return fail(error, "PinShot third-shot timer could not be started");
    }
    (void)context;
    return true;
}

// ----- Volley ---------------------------------------------------------------------------------------------------------
bool RuntimeArcherSkillOwnerV1::use_volley(CombatSession& session, const std::shared_ptr<Cast>& cast,
                                           const RuntimeSkillExternalContextV1& context, std::string& error) {
    std::size_t temp1 = 0;
    if (!field_index(cast->characters, "TempProp1", temp1, error)) return false;
    cast->chances = fixed(cast->sheet[temp1]);  // OnSkillUpdate_: chances = FromFixed(GetProp(TempProp1, true))
    cast->old_target = cast->new_target = invalid_actor_id;
    cast->rebound_projectile = 0;
    CombatSession* owner_session = &session;
    const auto delayed = [this, cast, owner_session](const effects::RuntimeSessionSkillProjectileEventV1& event,
                                                     std::string& callback_error) {
        if (event.expired || event.peer == invalid_actor_id) return true;
        ActorId target = event.peer;
        // The rebound arrow is spawned ON the first victim: when it reports that victim again, the Lua swaps in newTarget
        // ("SetProjectileTarget doesn't seem to work").
        if (event.projectile_id == cast->rebound_projectile && cast->old_target != invalid_actor_id &&
            target == cast->old_target && cast->new_target != invalid_actor_id)
            target = cast->new_target;
        if (eligible(*owner_session, cast->caster, target)) {
            DamageEvent result;
            if (!roll(*owner_session, cast, target, result, callback_error)) return false;
            log("Volley arrow hit victim=" + std::to_string(target) + " rebound=" + std::to_string(event.projectile_id == cast->rebound_projectile) +
                " damage=" + std::to_string(result.health_removed) + " requested=" + std::to_string(result.requested_damage) +
                " died=" + std::to_string(result.target_died));
        }
        if (cast->old_target == invalid_actor_id) cast->old_target = target;
        return do_rebound(*owner_session, cast, target, callback_error);
    };
    std::uint64_t id = 0;
    const ActorId top = pop_target(*cast, context);
    if (top != invalid_actor_id) {
        std::string look_error;
        if (!look_at(session, cast->caster, top, look_error)) log("Archer skill diagnostic Volley LookAt: " + look_error);
    }
    return launch_from_heading(session, cast, "ArrowDefault", delayed, id, error);
}

bool RuntimeArcherSkillOwnerV1::do_rebound(CombatSession& session, const std::shared_ptr<Cast>& cast, ActorId origin,
                                           std::string& error) {
    // rebound_chance(): GetRand(0, 100) < chances.
    std::uint32_t draw = 0;
    if (!services_.random) return fail(error, "Volley rebound needs the shared Session random service");
    if (!services_.random(101, draw, error)) return false;
    if (!(static_cast<double>(draw) < cast->chances)) {
        log("Volley rebound roll=" + std::to_string(draw) + " chances=" + std::to_string(cast->chances) + " -> no rebound");
        return true;
    }
    const auto* origin_actor = session.actor(origin);
    if (!origin_actor) return true;
    SkillTargetOriginV1 search;
    search.position = target_position(*origin_actor);
    search.look = look_vector(*origin_actor);
    search.radius = 0.0f;
    search.exclude = origin;  // INFERENCE: the origin victim is not its own rebound candidate (the Lua list would hold it)
    SkillActorTargetQueryV1 found;
    if (!query_source_character_targets_v1(session, cast->caster, cast->population_order, cast->target_facts,
            cast->objects_absent, kReboundRange, kPi, SkillTargetSortV1::source_order, found, error, nullptr, &search)) return false;
    ActorId chosen = invalid_actor_id;
    for (const auto candidate : found.ordered_character_targets) {
        if (candidate != cast->old_target) {
            chosen = candidate;
            break;
        }
    }
    if (chosen == invalid_actor_id) {
        log("Volley rebound roll=" + std::to_string(draw) + " -> no target to attack");
        return true;
    }
    const auto* victim = session.actor(chosen);
    if (!victim) return true;
    // SpawnSkillProjectile(ArrowDefault, origin, target): the arrow is placed at the origin victim and heads for the target.
    const auto from = origin_actor->transform.position;
    const auto to = target_position(*victim);
    std::array<float, 3> forward{to[0] - from[0], to[1] - from[1], 0.0f};
    if (std::fabs(forward[0]) + std::fabs(forward[1]) < 1.0e-4f) forward = look_vector(*origin_actor);
    cast->new_target = chosen;
    CombatSession* owner_session = &session;
    std::uint64_t id = 0;
    const auto delayed = [this, cast, owner_session](const effects::RuntimeSessionSkillProjectileEventV1& event,
                                                     std::string& callback_error) {
        if (event.expired || event.peer == invalid_actor_id) return true;
        ActorId target = event.peer;
        if (event.projectile_id == cast->rebound_projectile && cast->old_target != invalid_actor_id &&
            target == cast->old_target && cast->new_target != invalid_actor_id)
            target = cast->new_target;
        if (eligible(*owner_session, cast->caster, target)) {
            DamageEvent result;
            if (!roll(*owner_session, cast, target, result, callback_error)) return false;
            log("Volley rebound arrow hit victim=" + std::to_string(target) + " damage=" + std::to_string(result.health_removed) +
                " requested=" + std::to_string(result.requested_damage) + " died=" + std::to_string(result.target_died));
        }
        if (cast->old_target == invalid_actor_id) cast->old_target = target;
        return do_rebound(*owner_session, cast, target, callback_error);
    };
    if (!launch(session, cast, "ArrowDefault", from, forward, chosen, delayed, id, error)) return false;
    cast->rebound_projectile = id;
    log("Volley rebound arrow launched id=" + std::to_string(id) + " from=" + std::to_string(origin) + " to=" + std::to_string(chosen));
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeArcherSkillOwnerV1::advance(CombatSession& session, double dt_ms, std::string& error) {
    error.clear();
    timer_session_ = &session;
    std::string timer_error;
    const bool ok = timers_.advance(dt_ms, timer_error);
    timer_session_ = nullptr;
    if (!ok) {
        error = timer_error.empty() ? "Archer skill timer failed" : timer_error;
        return false;
    }
    return true;
}

} // namespace dh::foundation::generic_skills
