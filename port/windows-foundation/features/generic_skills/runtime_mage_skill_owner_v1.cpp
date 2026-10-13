#include "runtime_mage_skill_owner_v1.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <sstream>

namespace dh::foundation::generic_skills {
namespace {
constexpr float kPi = 3.1415927410125732421875f;

bool fail(std::string& error, const std::string& message) {
    error = message;
    return false;
}

double fixed(std::int32_t raw) { return static_cast<double>(raw) / 256.0; }
// Lua ToFixed: the value in 24.8 fixed point (truncated).
std::int32_t to_fixed(double value) { return static_cast<std::int32_t>(value * 256.0); }

bool field_index(const dh2::data::CharacterTable* table, const char* name, std::size_t& index, std::string& error) {
    if (!table) return fail(error, "Mage skill owner requires the source CharacterProperties table");
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

float planar_distance(const std::array<float, 3>& a, const std::array<float, 3>& b) {
    return std::hypot(a[0] - b[0], a[1] - b[1]);
}

bool eligible(CombatSession& session, ActorId caster, ActorId target) {
    const auto* world = session.world();
    const auto* attacker = session.actor(caster);
    const auto* victim = session.actor(target);
    return world && attacker && victim && target != caster && world->eligible_target(*attacker, *victim);
}

constexpr const char* kChainSound = "sfx_skill_mage_thunder_braid";
constexpr const char* kChainSoundAdd1 = "sfx_skill_mage_thunder_braid_add_1";
constexpr const char* kChainSoundAdd2 = "sfx_skill_mage_thunder_braid_add_2";
constexpr const char* kStoneEscapeFx = "spell_dh2_prince_mage_stone_escape_magefx";
constexpr const char* kStatueIdleFx = "skill_dh2_prince_mage_stone_escape_idle";
constexpr const char* kStatueDeathFx = "skill_dh2_prince_mage_stone_escape_death";
constexpr double kStatueDelayMs = 2200.0;   // TNT_DELAY
constexpr float kStoneMoveRange = 650.0f;   // MOVE_RANGE
constexpr float kStoneBlastRange = 650.0f;  // RANGE (explosion radius)
constexpr double kTornadoTickMs = 500.0;    // TICK_DELAY
constexpr float kChainSearchRange = 500.0f;
} // namespace

struct RuntimeMageSkillOwnerV1::RollContext {
    ActorId caster = invalid_actor_id;
    std::uint64_t generation = 0;
    std::uint32_t row_mask = 0;
    std::int32_t element = -1, category = -1;
    dh2::data::PropertySheet sheet{};
    std::string source_id;
    std::uint32_t next_event = 0;
};

struct RuntimeMageSkillOwnerV1::ChainState {
    RollContext roll_context;
    dh2::data::PropertySheet base_sheet{};
    std::size_t min_index = 0, max_index = 0;
    double num_jumps = 0.0, penalty = 0.0;
    unsigned victim_count = 0;
    std::vector<ActorId> victims;
    std::vector<ActorId> population_order;
    std::vector<SkillTargetSourceFactsV1> target_facts;
    std::optional<bool> objects_absent;
    std::int32_t row_target = -1, row_base = -1;
};

struct RuntimeMageSkillOwnerV1::Tornado {
    RollContext victim_context, area_context;
    std::uint64_t projectile_id = 0;
    std::array<float, 3> start{};
    RuntimeSkillTimerIdV1 tick_timer = 0;  // TICK_DELAY loop on the owner's timer service
    float area_range = 0.0f;
    bool area_has_direct_damage = false;
    bool finished = false;
    struct Victim {
        std::array<float, 3> offset{};  // victim position relative to the tornado when it was caught
        bool rolled = false;            // SkillCombatRoll(Skill_Cyclone) done (inference: stunned from here on)
    };
    std::map<ActorId, Victim> victims;
    std::vector<ActorId> population_order;
    std::vector<SkillTargetSourceFactsV1> target_facts;
    std::optional<bool> objects_absent;
};

struct RuntimeMageSkillOwnerV1::Statue {
    RollContext roll_context;
    std::array<float, 3> position{};
    RuntimeSkillTimerIdV1 timer = 0;  // TNT_DELAY one-shot on the owner's timer service
    std::vector<ActorId> population_order;
    std::vector<SkillTargetSourceFactsV1> target_facts;
    std::optional<bool> objects_absent;
};

RuntimeMageSkillOwnerV1::RuntimeMageSkillOwnerV1(RuntimeMageSkillServicesV1 services)
    : services_(std::move(services)) {}

void RuntimeMageSkillOwnerV1::log(const std::string& line) const {
    if (services_.trace) services_.trace(line);
}

void RuntimeMageSkillOwnerV1::reset() {
    timers_.clear();
    tornados_.clear();
    statues_.clear();
    stone_escape_origin_.clear();
}

bool RuntimeMageSkillOwnerV1::handles(const RuntimeSkillUseProfileV1& profile) const {
    const std::string name = profile.skill_name;
    return name == "ChainLightning" || name == "Cyclone" || name == "StoneEscape";
}

bool RuntimeMageSkillOwnerV1::roll(CombatSession& session, RollContext& context, ActorId target,
                                   DamageEvent& result, std::string& error) {
    CombatSessionSourceHit hit;
    hit.attacker = context.caster;
    hit.target = target;
    hit.binding_lease = session.actor_binding_lease();
    hit.generation = context.generation;
    hit.event_index = context.next_event++;
    hit.mask = context.row_mask | 0x08000000u;  // native SkillCombatRoll SkillAttack bit
    hit.source_id = context.source_id;
    hit.marker_name = "do_skill";
    hit.category = context.category;
    hit.element = context.element;
    hit.direct_amount = 0;
    hit.attacker_formula_sheet = &context.sheet;
    return session.apply_source_result(hit, result, error);
}

// ---------------------------------------------------------------------------------------------------------------------
// Pre
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeMageSkillOwnerV1::pre(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                  std::string& error) {
    error.clear();
    if (context.profile && std::string(context.profile->skill_name) == "StoneEscape") {
        // OnPreSkill_: oldx,oldy,oldz = GetPosition(); PlayFX(TELEPORT_FX, oldx, oldy, oldz).
        // Gap: the Lua then spawns the TeleportTool projectile (invisible_bolt, speed 50, max distance 650, StayOnFloor)
        // and reads its final position at Use. Here the destination is a floor-clamped straight trace of MOVE_RANGE at Use.
        const auto* caster = session.actor(context.caster);
        if (!caster) return fail(error, "StoneEscape Pre requires the caster actor");
        const auto at = caster->transform.position;
        stone_escape_origin_[context.caster] = at;
        std::string fx_error;
        if (services_.play_effect && !services_.play_effect(kStoneEscapeFx, at, fx_error))
            log("Mage skill diagnostic StoneEscape Pre FX: " + fx_error);
        else log("Mage skill StoneEscape Pre FX name=" + std::string(kStoneEscapeFx));
    }
    // ChainLightning Pre resets its victim table (the owner builds a fresh state per Use) and Cyclone Pre has no body
    // beyond the generic LookAt/UseMana prefix.
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
// Use
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeMageSkillOwnerV1::use(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                  std::string& error) {
    error.clear();
    if (!context.profile || !context.formula_sheet)
        return fail(error, "Mage skill Use requires its profile and ApplyPropClass formula sheet");
    const std::string name = context.profile->skill_name;
    if (name == "ChainLightning") return use_chain(session, context, error);
    if (name == "Cyclone") return use_cyclone(session, context, error);
    if (name == "StoneEscape") return use_stone_escape(session, context, error);
    return fail(error, "Mage skill owner does not handle " + name);
}

namespace {
template<class R>
void fill_roll(R& roll, const RuntimeSkillExternalContextV1& context, const char* source_suffix) {
    roll.caster = context.caster;
    roll.generation = context.generation;
    roll.row_mask = context.row_mask;
    roll.element = context.element;
    roll.category = context.category;
    roll.sheet = *context.formula_sheet;
    roll.source_id = context.skill_name + source_suffix;
    roll.next_event = 0;
}
} // namespace

// ----- ChainLightning ------------------------------------------------------------------------------------------------
bool RuntimeMageSkillOwnerV1::launch_chain_projectile(CombatSession& session, const std::shared_ptr<ChainState>& state,
                                                      std::int32_t row, const std::array<float, 3>& origin,
                                                      ActorId target, std::string& error) {
    auto* executor = services_.projectiles ? services_.projectiles() : nullptr;
    const auto tables = services_.projectile_tables ? services_.projectile_tables() : nullptr;
    if (!executor || !tables) return fail(error, "ChainLightning requires the shared projectile executor and its tables");
    effects::RuntimeSessionSkillProjectileLaunchV1 request;
    request.owner = state->roll_context.caster;
    request.generation = state->roll_context.generation;
    request.occurrence = next_occurrence_++;
    request.row = row;
    request.origin = origin;
    if (const auto* aimed = session.actor(target)) {
        const auto to = target_position(*aimed);
        request.forward = {to[0] - origin[0], to[1] - origin[1], 0.0f};
    }
    if (std::fabs(request.forward[0]) + std::fabs(request.forward[1]) < 1.0e-4f) {
        const auto* caster = session.actor(request.owner);
        if (!caster) return fail(error, "ChainLightning caster is absent");
        request.forward = look_vector(*caster);
    }
    request.explicit_target = target;
    // Lua RegisterSkillEx has delayed (OnDelayedSkill_) and a delayed_check that is not defined in this script, so every
    // collision is a hit (native default).
    CombatSession* owner_session = &session;
    request.delayed = [this, state, owner_session](const effects::RuntimeSessionSkillProjectileEventV1& event,
                                                   std::string& callback_error) {
        if (event.peer == invalid_actor_id) return true;  // Lua: delayed(nil, projectile) does nothing
        return chain_hit(*owner_session, state, event.peer, callback_error);
    };
    std::uint64_t projectile_id = 0;
    if (!executor->launch_skill(tables, request, projectile_id, error)) return false;
    log("Mage skill ChainLightning projectile launched id=" + std::to_string(projectile_id) + " row=" + std::to_string(row) +
        " target=" + std::to_string(target));
    if (services_.projectile_launched && !services_.projectile_launched(projectile_id, true, error)) return false;
    return true;
}

bool RuntimeMageSkillOwnerV1::use_chain(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                        std::string& error) {
    auto state = std::make_shared<ChainState>();
    fill_roll(state->roll_context, context, "/chain");
    state->base_sheet = *context.formula_sheet;
    std::size_t temp1 = 0, temp2 = 0;
    if (!field_index(context.characters, "SnS_MinDamage", state->min_index, error) ||
        !field_index(context.characters, "SnS_MaxDamage", state->max_index, error) ||
        !field_index(context.characters, "TempProp1", temp1, error) ||
        !field_index(context.characters, "TempProp2", temp2, error)) return false;
    // OnSkillUpdate_: num_jumps = FromFixed(TempProp1), penalty = FromFixed(TempProp2) / 100.
    state->num_jumps = fixed((*context.formula_sheet)[temp1]);
    state->penalty = fixed((*context.formula_sheet)[temp2]) / 100.0;
    state->population_order = context.population_order ? *context.population_order : std::vector<ActorId>{};
    state->target_facts = context.target_facts ? *context.target_facts : std::vector<SkillTargetSourceFactsV1>{};
    state->objects_absent = context.non_character_attackable_objects_absent;
    const auto tables = services_.projectile_tables ? services_.projectile_tables() : nullptr;
    if (!tables) return fail(error, "ChainLightning requires the projectile tables");
    state->row_target = projectile_row(*tables, "SimplifiedChainLightning");
    state->row_base = projectile_row(*tables, "SimplifiedChainLightning_base");
    if (state->row_target < 0 || state->row_base < 0)
        return fail(error, "ChainLightning ProjectileTable rows are missing");
    const auto* caster = session.actor(context.caster);
    if (!caster) return fail(error, "ChainLightning caster is absent");
    log("Mage skill ChainLightning Use jumps=" + std::to_string(state->num_jumps) + " penalty=" + std::to_string(state->penalty) +
        " preTargets=" + std::to_string(context.ordered_character_targets ? context.ordered_character_targets->size() : 0));
    std::string sound_error;
    if (services_.play_sound && !services_.play_sound(kChainSound, sound_error))
        log("Mage skill diagnostic ChainLightning sound: " + sound_error);
    // OnSkill_: pop the Pre list until the first Character, SpawnSkillProjectile(PROJECTILE_ID, GetSelf(), myTarget) +
    // SetProjectileTarget; with no target SpawnSkillProjectile(PROJECTILE_ID2) (the untargeted base row).
    ActorId first = invalid_actor_id;
    if (context.ordered_character_targets && !context.ordered_character_targets->empty())
        first = context.ordered_character_targets->front();
    const auto origin = caster->transform.position;
    if (first != invalid_actor_id)
        return launch_chain_projectile(session, state, state->row_target, origin, first, error);
    return launch_chain_projectile(session, state, state->row_base, origin, invalid_actor_id, error);
}

bool RuntimeMageSkillOwnerV1::chain_hit(CombatSession& session, const std::shared_ptr<ChainState>& state,
                                        ActorId target, std::string& error) {
    // OnDelayedSkill_(attackTarget): check_victims(name) and victim_count < num_jumps.
    // Deviation: victims are compared by actor identity (the Lua compares GetName()).
    const bool known = std::find(state->victims.begin(), state->victims.end(), target) != state->victims.end();
    if (known || !(static_cast<double>(state->victim_count) < state->num_jumps)) return true;
    if (!eligible(session, state->roll_context.caster, target)) return true;  // dead/untargetable: nothing to roll
    state->victims.push_back(target);  // Gap: attackTarget:LOCK() (AI freeze) is not reproduced
    // ApplyPropClass(CLASS_ID) restores the class sheet before the scaled min/max are written.
    state->roll_context.sheet = state->base_sheet;
    const double base_min = fixed(state->roll_context.sheet[state->min_index]);
    const double base_max = fixed(state->roll_context.sheet[state->max_index]);
    ++state->victim_count;
    const double scale = 1.0 - (static_cast<double>(state->victim_count) - 1.0) * state->penalty;
    state->roll_context.sheet[state->min_index] = to_fixed(base_min * scale);
    state->roll_context.sheet[state->max_index] = to_fixed(base_max * scale);
    DamageEvent result;
    if (!roll(session, state->roll_context, target, result, error)) return false;
    log("Mage skill ChainLightning hit victim=" + std::to_string(target) + " n=" + std::to_string(state->victim_count) +
        " scale=" + std::to_string(scale) + " damage=" + std::to_string(result.health_removed) +
        " requested=" + std::to_string(result.requested_damage) + " died=" + std::to_string(result.target_died));
    return chain_search(session, state, target, error);
}

bool RuntimeMageSkillOwnerV1::chain_search(CombatSession& session, const std::shared_ptr<ChainState>& state,
                                           ActorId from, std::string& error) {
    // SearchVictim(startingTarget): victim_count > num_jumps ends the chain.
    if (static_cast<double>(state->victim_count) > state->num_jumps) return true;
    const auto* origin_actor = session.actor(from);
    const auto* world = session.world();
    if (!origin_actor || !world) return fail(error, "ChainLightning search origin is absent");
    SkillTargetOriginV1 origin;
    origin.position = target_position(*origin_actor);
    origin.look = look_vector(*origin_actor);
    origin.radius = world->target_radius(*origin_actor);
    origin.exclude = from;
    // Enemy + AttackableOnly filters, ClosestFirst, TargetListSearch(500, 360, startingTarget).
    SkillActorTargetQueryV1 found;
    if (!query_source_character_targets_v1(session, state->roll_context.caster, state->population_order,
            state->target_facts, state->objects_absent, kChainSearchRange, 2.0f * kPi, SkillTargetSortV1::closest_first,
            found, error, nullptr, &origin)) return false;
    if (found.ordered_character_targets.empty()) return true;
    ActorId next = invalid_actor_id;
    for (const auto candidate : found.ordered_character_targets) {
        if (std::find(state->victims.begin(), state->victims.end(), candidate) == state->victims.end()) {
            next = candidate;
            break;
        }
    }
    std::string sound_error;
    const char* sound = state->victim_count > 1 ? kChainSoundAdd2 : kChainSoundAdd1;
    if (services_.play_sound && !services_.play_sound(sound, sound_error))
        log("Mage skill diagnostic ChainLightning sound: " + sound_error);
    if (next != invalid_actor_id && static_cast<double>(state->victim_count) < state->num_jumps) {
        log("Mage skill ChainLightning jump from=" + std::to_string(from) + " to=" + std::to_string(next));
        return launch_chain_projectile(session, state, state->row_target, origin.position, next, error);
    }
    return true;
}

// ----- Cyclone ------------------------------------------------------------------------------------------------------
bool RuntimeMageSkillOwnerV1::use_cyclone(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                          std::string& error) {
    auto* executor = services_.projectiles ? services_.projectiles() : nullptr;
    const auto tables = services_.projectile_tables ? services_.projectile_tables() : nullptr;
    if (!executor || !tables) return fail(error, "Cyclone requires the shared projectile executor and its tables");
    const auto row = projectile_row(*tables, "Tornado");
    if (row < 0) return fail(error, "Cyclone ProjectileTable row Tornado is missing");
    const auto* caster = session.actor(context.caster);
    if (!caster) return fail(error, "Cyclone caster is absent");
    auto tornado = std::make_shared<Tornado>();
    fill_roll(tornado->victim_context, context, "/cyclone");
    fill_roll(tornado->area_context, context, "/cyclone-area");
    if (!context.class_projection ||
        !context.class_projection("Skill_Cyclone_Proximity", tornado->area_context.sheet, error)) {
        if (error.empty()) error = "Cyclone Proximity class projection is unavailable";
        return false;
    }
    std::size_t temp1 = 0, min_damage = 0, max_damage = 0;
    if (!field_index(context.characters, "TempProp1", temp1, error) ||
        !field_index(context.characters, "SnS_MinDamage", min_damage, error) ||
        !field_index(context.characters, "SnS_MaxDamage", max_damage, error)) return false;
    // OnSkillUpdate_: AREA_RANGE = FromFixed(TempProp1) of the Skill_Cyclone projection.
    tornado->area_range = static_cast<float>(fixed((*context.formula_sheet)[temp1]));
    tornado->area_has_direct_damage = tornado->area_context.sheet[min_damage] != 0 || tornado->area_context.sheet[max_damage] != 0;
    tornado->population_order = context.population_order ? *context.population_order : std::vector<ActorId>{};
    tornado->target_facts = context.target_facts ? *context.target_facts : std::vector<SkillTargetSourceFactsV1>{};
    tornado->objects_absent = context.non_character_attackable_objects_absent;
    tornado->start = caster->transform.position;

    effects::RuntimeSessionSkillProjectileLaunchV1 request;
    request.owner = context.caster;
    request.generation = context.generation;
    request.occurrence = next_occurrence_++;
    request.row = row;
    request.origin = caster->transform.position;
    request.forward = look_vector(*caster);
    CombatSession* owner_session = &session;
    // OnDelayedSkillCheck_: AddVictim(target, projectile) and return false (the tornado never "hits").
    std::weak_ptr<Tornado> weak = tornado;
    request.check = [weak, owner_session](const effects::RuntimeSessionSkillProjectileEventV1& event, bool& accept,
                                          std::string&) {
        accept = false;
        const auto owner = weak.lock();
        if (!owner || event.peer == invalid_actor_id) return true;
        if (owner->victims.count(event.peer)) return true;
        const auto* victim = owner_session->actor(event.peer);
        if (!victim) return true;
        Tornado::Victim record;
        for (unsigned axis = 0; axis < 3; ++axis) record.offset[axis] = victim->transform.position[axis] - event.position[axis];
        owner->victims.emplace(event.peer, record);  // Gap: victim:LOCK() is not reproduced
        return true;
    };
    // OnDelayedSkill_(nil, projectile): the tornado expired -> CleanUp.
    request.delayed = [weak](const effects::RuntimeSessionSkillProjectileEventV1& event, std::string&) {
        if (!event.expired) return true;
        if (const auto owner = weak.lock()) owner->finished = true;
        return true;
    };
    std::uint64_t projectile_id = 0;
    if (!executor->launch_skill(tables, request, projectile_id, error)) return false;
    tornado->projectile_id = projectile_id;
    // Lua StartSkillTimerCB(TICK_DELAY, true, HandleTick): a repeating tick on this owner's clock until the tornado ends.
    std::weak_ptr<Tornado> weak_tick = tornado;
    tornado->tick_timer = timers_.start(kTornadoTickMs, true, [this, weak_tick](RuntimeSkillTimerIdV1, std::string& timer_error) {
        const auto owner = weak_tick.lock();
        if (!owner || owner->finished) return true;
        if (!timer_session_) return fail(timer_error, "Cyclone tick fired outside the Mage owner clock");
        return tick_tornado(*timer_session_, *owner, timer_error);
    });
    if (tornado->tick_timer == 0) return fail(error, "Cyclone tick timer could not be started");
    tornados_.push_back(tornado);
    log("Mage skill Cyclone tornado id=" + std::to_string(projectile_id) + " areaRange=" + std::to_string(tornado->area_range) +
        " proximityDirectDamage=" + std::to_string(tornado->area_has_direct_damage));
    return !services_.projectile_launched || services_.projectile_launched(projectile_id, true, error);
}

bool RuntimeMageSkillOwnerV1::tick_tornado(CombatSession& session, Tornado& tornado, std::string& error) {
    auto* executor = services_.projectiles ? services_.projectiles() : nullptr;
    std::array<float, 3> position{};
    if (!executor || !executor->projectile_position(tornado.projectile_id, position)) {
        tornado.finished = true;
        return true;
    }
    // HandleTick, victims: a victim that is not yet stunned receives SkillCombatRoll(Skill_Cyclone) once; a stunned one
    // is moved with the tornado (HeadTo(vx+dx, vy+dy)). Inference: the stun tail of F_ApplyResult is not reproduced, so a
    // rolled victim counts as stunned (the authored class has SnS_Stun_Target_Chance 100%).
    for (auto& entry : tornado.victims) {
        if (entry.second.rolled) continue;
        if (!eligible(session, tornado.victim_context.caster, entry.first)) continue;
        DamageEvent result;
        if (!roll(session, tornado.victim_context, entry.first, result, error)) return false;
        entry.second.rolled = true;
        log("Mage skill Cyclone victim rolled actor=" + std::to_string(entry.first) + " damage=" +
            std::to_string(result.health_removed) + " requested=" + std::to_string(result.requested_damage));
    }
    // Collateral damage: enemies within AREA_RANGE of the tornado that are not victims (Skill_Cyclone_Proximity).
    // Gap: the class carries only slow tails (no direct damage), which this port does not apply; the roll is skipped
    // when the projected class has no SnS_Min/MaxDamage.
    if (tornado.area_has_direct_damage) {
        SkillTargetOriginV1 origin;
        origin.position = position;
        origin.look = {0.0f, -1.0f, 0.0f};
        origin.radius = 0.0f;
        SkillActorTargetQueryV1 found;
        if (!query_source_character_targets_v1(session, tornado.victim_context.caster, tornado.population_order,
                tornado.target_facts, tornado.objects_absent, tornado.area_range, 2.0f * kPi, SkillTargetSortV1::source_order,
                found, error, nullptr, &origin)) return false;
        for (const auto target : found.ordered_character_targets) {
            if (tornado.victims.count(target) || !eligible(session, tornado.area_context.caster, target)) continue;
            DamageEvent result;
            if (!roll(session, tornado.area_context, target, result, error)) return false;
        }
    }
    return true;
}

// ----- StoneEscape --------------------------------------------------------------------------------------------------
bool RuntimeMageSkillOwnerV1::use_stone_escape(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                               std::string& error) {
    const auto* caster = session.actor(context.caster);
    if (!caster) return fail(error, "StoneEscape caster is absent");
    const auto origin_found = stone_escape_origin_.find(context.caster);
    const auto old_position = origin_found != stone_escape_origin_.end() ? origin_found->second : caster->transform.position;
    stone_escape_origin_.erase(context.caster);
    // Teleport: WarpTo(x, y, z) where (x, y, z) is the TeleportTool projectile end point (MOVE_RANGE 650 along the
    // facing, stopping before a door/decor). Inference: a floor-clamped straight trace stands in for that projectile.
    const auto direction = look_vector(*caster);
    std::array<float, 3> destination{old_position[0] + direction[0] * kStoneMoveRange,
                                     old_position[1] + direction[1] * kStoneMoveRange, old_position[2]};
    if (services_.teleport_path && !services_.teleport_path(context.caster, old_position, direction, kStoneMoveRange,
                                                            destination, error)) {
        if (error.empty()) error = "StoneEscape teleport path query failed";
        return false;
    }
    if (!services_.move_actor || !services_.move_actor(context.caster, destination, true, error)) {
        if (error.empty()) error = "StoneEscape WarpTo is unavailable";
        return false;
    }
    std::string fx_error;
    if (services_.play_effect && !services_.play_effect(kStoneEscapeFx, destination, fx_error))
        log("Mage skill diagnostic StoneEscape destination FX: " + fx_error);
    log("Mage skill StoneEscape warp from=" + std::to_string(old_position[0]) + "," + std::to_string(old_position[1]) +
        " to=" + std::to_string(destination[0]) + "," + std::to_string(destination[1]) +
        " distance=" + std::to_string(planar_distance(old_position, destination)));
    // Summon(CharacterTable Dummy) at the old position: gap, the Dummy is an invisible_man actor whose
    // idle/death clips play skill_dh2_prince_mage_stone_escape_idle/_death. Inference: those two FX sets stand in for the
    // statue and its death at the old position; no Dummy actor is spawned.
    auto statue = std::make_shared<Statue>();
    fill_roll(statue->roll_context, context, "/statue");
    statue->position = old_position;
    statue->population_order = context.population_order ? *context.population_order : std::vector<ActorId>{};
    statue->target_facts = context.target_facts ? *context.target_facts : std::vector<SkillTargetSourceFactsV1>{};
    statue->objects_absent = context.non_character_attackable_objects_absent;
    if (services_.play_effect && !services_.play_effect(kStatueIdleFx, old_position, fx_error))
        log("Mage skill diagnostic StoneEscape statue FX: " + fx_error);
    // Lua StartSkillTimerCB(TNT_DELAY, false, DoExplosion, statue): a one-shot on this owner's clock.
    std::weak_ptr<Statue> weak_statue = statue;
    statue->timer = timers_.start(kStatueDelayMs, false, [this, weak_statue](RuntimeSkillTimerIdV1, std::string& timer_error) {
        const auto owner = weak_statue.lock();
        if (!owner) return true;
        if (!timer_session_) return fail(timer_error, "StoneEscape statue timer fired outside the Mage owner clock");
        // The one-shot is spent before DoExplosion runs (the owner leaves the list, as the Lua timer entry does).
        statues_.erase(std::remove(statues_.begin(), statues_.end(), owner), statues_.end());
        return explode_statue(*timer_session_, *owner, timer_error);
    });
    if (statue->timer == 0) return fail(error, "StoneEscape statue timer could not be started");
    statues_.push_back(statue);
    return true;
}

bool RuntimeMageSkillOwnerV1::explode_statue(CombatSession& session, Statue& statue, std::string& error) {
    // DoExplosion: Enemy + AttackableOnly, NoSort, TargetListSearch(RANGE 650, ANGLE 360, statueDummy); every target gets
    // SkillCombatRoll with ApplyPropClass(CLASS_ID); then the statue is killed (its death clip plays the death FX).
    SkillTargetOriginV1 origin;
    origin.position = statue.position;
    origin.look = {0.0f, -1.0f, 0.0f};
    origin.radius = 0.0f;
    SkillActorTargetQueryV1 found;
    if (!query_source_character_targets_v1(session, statue.roll_context.caster, statue.population_order,
            statue.target_facts, statue.objects_absent, kStoneBlastRange, 2.0f * kPi, SkillTargetSortV1::source_order,
            found, error, nullptr, &origin)) return false;
    log("Mage skill StoneEscape explosion targets=" + std::to_string(found.ordered_character_targets.size()));
    for (const auto target : found.ordered_character_targets) {
        if (!eligible(session, statue.roll_context.caster, target)) continue;
        DamageEvent result;
        if (!roll(session, statue.roll_context, target, result, error)) return false;
        log("Mage skill StoneEscape blast hit target=" + std::to_string(target) + " damage=" + std::to_string(result.health_removed) +
            " requested=" + std::to_string(result.requested_damage) + " died=" + std::to_string(result.target_died));
    }
    std::string fx_error;
    if (services_.stop_effect) services_.stop_effect(kStatueIdleFx, fx_error);
    if (services_.play_effect && !services_.play_effect(kStatueDeathFx, statue.position, fx_error))
        log("Mage skill diagnostic StoneEscape death FX: " + fx_error);
    return true;
}

// ----- clock --------------------------------------------------------------------------------------------------------
bool RuntimeMageSkillOwnerV1::advance(CombatSession& session, double dt_ms, std::string& error) {
    error.clear();
    // The statue explosions and the tornado ticks are timers on the shared service, fired in due-time order. The
    // Session is bound only for the duration of this call (the timer callbacks read it from timer_session_).
    timer_session_ = &session;
    std::string timer_error;
    const bool timers_ok = timers_.advance(dt_ms, timer_error);
    timer_session_ = nullptr;
    if (!timers_ok) {
        error = timer_error.empty() ? "Mage skill timer failed" : timer_error;
        return false;
    }
    auto* executor = services_.projectiles ? services_.projectiles() : nullptr;
    // Cleanup and carry after the timers. Ordering change against the former ad hoc clock (documented in the report):
    // a victim rolled by this frame's tick is carried in this frame, not from the next one.
    for (auto it = tornados_.begin(); it != tornados_.end();) {
        auto& tornado = **it;
        std::array<float, 3> position{};
        const bool alive = executor && executor->projectile_position(tornado.projectile_id, position);
        if (!alive || tornado.finished) {
            log("Mage skill Cyclone cleanup id=" + std::to_string(tornado.projectile_id) + " victims=" + std::to_string(tornado.victims.size()));
            timers_.stop(tornado.tick_timer);
            it = tornados_.erase(it);
            continue;
        }
        // Carry rolled (stunned) victims with the tornado every frame (inference for HeadTo(vx+dx, vy+dy) per tick).
        for (auto& entry : tornado.victims) {
            if (!entry.second.rolled) continue;
            const auto* victim = session.actor(entry.first);
            if (!victim || !victim->alive()) continue;
            const std::array<float, 3> wanted{position[0] + entry.second.offset[0], position[1] + entry.second.offset[1],
                                              victim->transform.position[2]};
            std::string move_error;
            if (services_.move_actor && !services_.move_actor(entry.first, wanted, false, move_error))
                log("Mage skill diagnostic Cyclone carry: " + move_error);
        }
        ++it;
    }
    return true;
}

} // namespace dh::foundation::generic_skills
