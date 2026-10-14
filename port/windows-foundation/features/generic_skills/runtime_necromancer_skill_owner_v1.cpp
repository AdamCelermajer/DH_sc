#include "runtime_necromancer_skill_owner_v1.hpp"

#include "runtime_skill_target_query_v1.hpp"

#include <algorithm>
#include <cmath>
#include <optional>

namespace dh::foundation::generic_skills {
namespace {
constexpr float kPi = 3.1415927410125732421875f;

// Lua constants of the three scripts (see the header and the profile rows).
constexpr float kFireSearchRange = 280.0f;   // DamageTick TargetListSearch(RANGE, ANGLE 360)
constexpr double kFireTickMs = 1000.0;       // TICK_DELAY
constexpr int kFireMaxTicks = 2;             // MAX_TICKS: rolls at tick_count 0, 1, 2
constexpr const char* kFireSkullRainFx = "spell_dh2_prince_necromancer_fireskullrain";
constexpr double kPestilencePollMs = 500.0;  // UpdateAttackedTargets StartSkillTimerCB(500, true)
constexpr float kPestilenceBlastRange = 200.0f;
constexpr const char* kPestilenceFx = "spell_dh2_prince_necromancer_pestilence";
constexpr const char* kPestilenceSound = "sfx_skill_mage_doppelganger_statue_explode";
constexpr const char* kRiseProjectile = "TeleportTool";
constexpr unsigned kSnsDurationRawIndex = 176;  // CharacterProperties.SnS_Duration (Rise bufftime, x256 ms)

bool fail(std::string& error, const std::string& message) {
    error = message;
    return false;
}

double fixed(std::int32_t raw) { return static_cast<double>(raw) / 256.0; }

bool field_index(const dh2::data::CharacterTable* table, const char* name, std::size_t& index, std::string& error) {
    if (!table) return fail(error, "Necromancer skill owner requires the source CharacterProperties table");
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

// GameObject::GetTargetPosition, the choice runtime_skill_target_query_v1.cpp makes.
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
} // namespace

struct RuntimeNecromancerSkillOwnerV1::CastSnapshot {
    ActorId caster = invalid_actor_id;
    std::uint64_t generation = 0;
    std::uint32_t row_mask = 0;
    std::int32_t element = -1, category = -1;
    // The Use-time ApplyPropClass(CLASS_ID) sheet after SetProp(SnS_Level): the F_SkillAttack formula sheet of every roll.
    dh2::data::PropertySheet sheet{};
    std::string skill_name;
    std::vector<ActorId> population_order;
    std::vector<SkillTargetSourceFactsV1> target_facts;
    std::optional<bool> objects_absent;
    std::uint32_t next_event = 0;
};

struct RuntimeNecromancerSkillOwnerV1::FireRain {
    std::shared_ptr<CastSnapshot> cast;
    ActorId caster = invalid_actor_id;
    RuntimeSkillTimerIdV1 timer = 0;  // StartSkillTimerCB id (0 = Lua nil)
    int tick_count = 0;
};

struct RuntimeNecromancerSkillOwnerV1::Victim {
    ActorId id = invalid_actor_id;
    std::shared_ptr<CastSnapshot> cast;
    RuntimeSkillTimerIdV1 removal_timer = 0;  // StartSkillTimerCB(SnS_Dot_Duration + 1000, false, RemoveTarget)
    std::array<float, 3> last_position{};     // used when the victim is no longer in the Session at explosion time
};

RuntimeNecromancerSkillOwnerV1::RuntimeNecromancerSkillOwnerV1(RuntimeNecromancerSkillServicesV1 services)
    : services_(std::move(services)) {}

void RuntimeNecromancerSkillOwnerV1::log(const std::string& line) const {
    if (services_.trace) services_.trace(line);
}

void RuntimeNecromancerSkillOwnerV1::reset() {
    timers_.clear();
    fire_rains_.clear();
    infected_.clear();
    death_watch_.clear();
    rise_landing_.clear();
    rise_slots_ = {};  // the world is being reset: the summons go with it (no despawn call)
    rise_spawn_counter_ = 0;
    pestilence_poll_timer_ = 0;
}

bool RuntimeNecromancerSkillOwnerV1::handles(const RuntimeSkillUseProfileV1& profile) const {
    const std::string name = profile.skill_name;
    return name == "FireSkullRain" || name == "Pestilence" || name == "Rise";
}

bool RuntimeNecromancerSkillOwnerV1::roll(CombatSession& session, CastSnapshot& cast, ActorId target, std::string& error) {
    CombatSessionSourceHit hit;
    hit.attacker = cast.caster;
    hit.target = target;
    hit.binding_lease = session.actor_binding_lease();
    hit.generation = cast.generation;
    hit.event_index = cast.next_event++;
    hit.mask = cast.row_mask | 0x08000000u;  // native SkillCombatRoll SkillAttack bit
    hit.source_id = cast.skill_name;
    hit.marker_name = "do_skill";
    hit.category = cast.category;
    hit.element = cast.element;
    hit.direct_amount = 0;
    hit.attacker_formula_sheet = &cast.sheet;
    DamageEvent result;
    if (!session.apply_source_result(hit, result, error)) return false;
    log("Necromancer skill " + cast.skill_name + " roll target=" + std::to_string(target) +
        " requested=" + std::to_string(result.requested_damage) + " damage=" + std::to_string(result.health_removed) +
        " died=" + std::to_string(result.target_died));
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
// Pre
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeNecromancerSkillOwnerV1::pre(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                         std::string& error) {
    error.clear();
    if (!context.profile) return fail(error, "Necromancer Pre requires its profile");
    const std::string name = context.profile->skill_name;
    if (name == "Rise") {
        // prince_necromancer_rise.luac OnPreSkill_: UseMana (done by the coordinator) then DoRise(count). The landing
        // position is reset (x = nil) and the front TeleportTool projectile is launched.
        rise_landing_.erase(context.caster);
        return launch_rise_projectile(session, context, error);
    }
    // FireSkullRain: OnPre is UseMana + SetSkillCooldown only. Pestilence: OnPre is the search + LookAt + UseMana, all in
    // the coordinator's generic prefix.
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------
// Use
// ---------------------------------------------------------------------------------------------------------------------
bool RuntimeNecromancerSkillOwnerV1::use(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                         std::string& error) {
    error.clear();
    if (!context.profile || !context.formula_sheet)
        return fail(error, "Necromancer skill Use requires its profile and ApplyPropClass formula sheet");
    const std::string name = context.profile->skill_name;
    if (name == "FireSkullRain") return use_fire_skull_rain(session, context, error);
    if (name == "Pestilence") return use_pestilence(session, context, error);
    if (name == "Rise") return use_rise(session, context, error);
    return fail(error, "Necromancer skill owner does not handle " + name);
}

namespace {
std::shared_ptr<RuntimeNecromancerSkillOwnerV1::CastSnapshot> make_snapshot(const RuntimeSkillExternalContextV1& context) {
    auto cast = std::make_shared<RuntimeNecromancerSkillOwnerV1::CastSnapshot>();
    cast->caster = context.caster;
    cast->generation = context.generation;
    cast->row_mask = context.row_mask;
    cast->element = context.element;
    cast->category = context.category;
    cast->sheet = *context.formula_sheet;
    cast->skill_name = context.skill_name;
    cast->population_order = context.population_order ? *context.population_order : std::vector<ActorId>{};
    cast->target_facts = context.target_facts ? *context.target_facts : std::vector<SkillTargetSourceFactsV1>{};
    cast->objects_absent = context.non_character_attackable_objects_absent;
    return cast;
}
} // namespace

// ----- FireSkullRain -------------------------------------------------------------------------------------------------
bool RuntimeNecromancerSkillOwnerV1::fire_skull_rain_tick(CombatSession& session, const std::shared_ptr<FireRain>& rain,
                                                          std::string& error) {
    CastSnapshot& cast = *rain->cast;
    // DamageTick: SetTargetListCharacterFilter(Enemy), object filter NONE, NoSort, TargetListSearch(280, 360) from the
    // caster (no origin argument in the Lua call).
    SkillActorTargetQueryV1 found;
    if (!query_source_character_targets_v1(session, cast.caster, cast.population_order, cast.target_facts,
            cast.objects_absent, kFireSearchRange, kPi, SkillTargetSortV1::source_order, found, error)) return false;
    const bool empty = found.ordered_character_targets.empty();
    if (rain->tick_count < 1) {
        // PlaySkullRainAnim: the METEOR FX on the first target, or at caster + (160, 160, 0) when the search is empty.
        std::string fx_error;
        if (!empty) {
            const auto* first = session.actor(found.ordered_character_targets.front());
            const auto at = first ? target_position(*first) : std::array<float, 3>{};
            if (services_.play_effect && !services_.play_effect(kFireSkullRainFx, at, fx_error))
                log("Necromancer skill diagnostic FireSkullRain FX: " + fx_error);
        } else if (const auto* caster = session.actor(cast.caster)) {
            const auto& p = caster->transform.position;
            const std::array<float, 3> at{p[0] + 160.0f, p[1] + 160.0f, p[2]};
            if (services_.play_effect && !services_.play_effect(kFireSkullRainFx, at, fx_error))
                log("Necromancer skill diagnostic FireSkullRain FX: " + fx_error);
        }
    }
    const auto stop = [&]() {
        timers_.stop(rain->timer);
        rain->timer = 0;
        rain->tick_count = 0;
        fire_rains_.erase(rain->caster);
    };
    if (empty) {
        log("Necromancer skill FireSkullRain tick empty: loop stopped");
        stop();
        return true;
    }
    // SetProp(SnS_Level) + ApplyPropClass(CLASS_ID): the Use-time sheet, rolled for every popped target.
    for (const auto target : found.ordered_character_targets) {
        if (!eligible(session, cast.caster, target)) continue;
        if (!roll(session, cast, target, error)) return false;
    }
    if (rain->tick_count >= kFireMaxTicks) {
        log("Necromancer skill FireSkullRain third roll done: loop stopped");
        stop();
        return true;
    }
    ++rain->tick_count;
    return true;
}

bool RuntimeNecromancerSkillOwnerV1::use_fire_skull_rain(CombatSession&, const RuntimeSkillExternalContextV1& context,
                                                         std::string& error) {
    // OnSkill_: `if (timer_id == nil) then timer_id = StartSkillTimerCB(TICK_DELAY, true, DamageTick)`. A running loop is
    // not restarted by a second Use.
    auto found = fire_rains_.find(context.caster);
    if (found != fire_rains_.end() && found->second->timer != 0 && timers_.active(found->second->timer)) return true;
    auto rain = std::make_shared<FireRain>();
    rain->cast = make_snapshot(context);
    rain->caster = context.caster;
    fire_rains_[context.caster] = rain;
    rain->timer = timers_.start(kFireTickMs, true,
        [this, rain](RuntimeSkillTimerIdV1, std::string& timer_error) -> bool {
            if (!timer_session_) return fail(timer_error, "FireSkullRain tick requires the Session of advance()");
            return fire_skull_rain_tick(*timer_session_, rain, timer_error);
        }, kOwnerFire);
    if (rain->timer == 0) {
        fire_rains_.erase(context.caster);
        return fail(error, "FireSkullRain loop timer was not started");
    }
    log("Necromancer skill FireSkullRain loop started (1000 ms, max 3 rolls)");
    return true;
}

// ----- Pestilence ----------------------------------------------------------------------------------------------------
bool RuntimeNecromancerSkillOwnerV1::use_pestilence(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                                    std::string& error) {
    auto cast = make_snapshot(context);
    // timeofdot = FromFixed(SnS_Dot_Duration) (the class sheet; the Lua reads it in OnSkillUpdate_ with the same class).
    std::size_t dot_index = 0;
    if (!field_index(context.characters, "SnS_Dot_Duration", dot_index, error)) return false;
    const double dot_ms = fixed(cast->sheet[dot_index]);
    const auto* caster = session.actor(context.caster);
    if (!caster) return fail(error, "Pestilence Use requires the caster actor");
    // The Lua loop pops the OnPre list (no new search). Characters that are not the player and not already infected are
    // infected: SkillCombatRoll, then a one-shot removal timer. The Pre list holds Characters only (see the report).
    const auto targets = context.ordered_character_targets ? *context.ordered_character_targets : std::vector<ActorId>{};
    for (const auto target : targets) {
        const auto* traits = session.world() ? session.world()->traits(target) : nullptr;
        if (traits && traits->is_player) continue;
        if (infected_.count(target)) continue;  // Lua: name match, "He was infected, do not add it to the list"
        if (!eligible(session, context.caster, target)) continue;
        if (!roll(session, *cast, target, error)) return false;
        auto victim = std::make_shared<Victim>();
        victim->id = target;
        victim->cast = cast;
        if (const auto* state = session.actor(target)) victim->last_position = state->transform.position;
        victim->removal_timer = timers_.start(dot_ms + 1000.0, false,
            [this, target](RuntimeSkillTimerIdV1, std::string&) -> bool {
                // RemoveTarget: the victim leaves the list without an explosion.
                infected_.erase(target);
                death_watch_.untrack(target);
                return true;
            }, kOwnerPestilence);
        infected_[target] = victim;
        death_watch_.track(target);
        log("Necromancer skill Pestilence infected target=" + std::to_string(target) +
            " removeAfterMs=" + std::to_string(dot_ms + 1000.0));
    }
    if (!infected_.empty() && pestilence_poll_timer_ == 0) {
        // UpdateAttackedTargets: the 500 ms death watch. Started with the first infection (the Lua starts it at learn time;
        // the phase differs, see the report).
        pestilence_poll_timer_ = timers_.start(kPestilencePollMs, true,
            [this](RuntimeSkillTimerIdV1, std::string& timer_error) -> bool {
                if (!timer_session_) return fail(timer_error, "Pestilence poll requires the Session of advance()");
                return pestilence_poll(*timer_session_, timer_error);
            }, kOwnerPestilence);
    }
    return true;
}

bool RuntimeNecromancerSkillOwnerV1::pestilence_poll(CombatSession& session, std::string& error) {
    // IsDead() over the infected list. A dead victim explodes and is removed. Explosions can kill further victims, so the
    // poll repeats until no new death is reported (inference: the Lua pairs() loop reaches those in the same pass).
    for (;;) {
        const auto dead = death_watch_.poll([&](ActorId actor) { return actor_is_dead_in_session(session, actor); });
        if (dead.empty()) break;
        for (const auto id : dead) {
            const auto found = infected_.find(id);
            if (found == infected_.end()) continue;
            const auto victim = found->second;
            infected_.erase(found);
            timers_.stop(victim->removal_timer);
            victim->removal_timer = 0;
            log("Necromancer skill Pestilence victim dead=" + std::to_string(id) + " -> explosion");
            if (!pestilence_explode(session, *victim, error)) return false;
        }
    }
    if (infected_.empty() && pestilence_poll_timer_ != 0) {
        timers_.stop(pestilence_poll_timer_);
        pestilence_poll_timer_ = 0;
    }
    error.clear();
    return true;
}

bool RuntimeNecromancerSkillOwnerV1::pestilence_explode(CombatSession& session, const Victim& victim, std::string& error) {
    // DamageNearbyTargets(victim): ApplyPropClass(CLASS_ID) (the stored Use sheet), TargetListSearch(200, 360, victim)
    // with Enemy/AttackableOnly, ClosestFirst; PlayFX on the victim, PlaySound, then one roll per popped target.
    CastSnapshot& cast = *victim.cast;
    const auto* state = session.actor(victim.id);
    const auto at = state ? target_position(*state) : victim.last_position;
    SkillTargetOriginV1 origin;
    origin.position = at;
    origin.look = {0.0f, -1.0f, 0.0f};
    origin.radius = state && session.world() ? session.world()->target_radius(*state) : 0.0f;
    origin.exclude = victim.id;
    SkillActorTargetQueryV1 found;
    if (!query_source_character_targets_v1(session, cast.caster, cast.population_order, cast.target_facts,
            cast.objects_absent, kPestilenceBlastRange, kPi, SkillTargetSortV1::closest_first, found, error, nullptr,
            &origin)) return false;
    std::string fx_error;
    if (services_.play_effect && !services_.play_effect(kPestilenceFx, at, fx_error))
        log("Necromancer skill diagnostic Pestilence FX: " + fx_error);
    std::string sound_error;
    if (services_.play_sound && !services_.play_sound(kPestilenceSound, sound_error))
        log("Necromancer skill diagnostic Pestilence sound: " + sound_error);
    log("Necromancer skill Pestilence explosion victims=" + std::to_string(found.ordered_character_targets.size()));
    for (const auto target : found.ordered_character_targets) {
        if (!eligible(session, cast.caster, target)) continue;
        if (!roll(session, cast, target, error)) return false;
    }
    return true;
}

// ----- Rise ----------------------------------------------------------------------------------------------------------
bool RuntimeNecromancerSkillOwnerV1::launch_rise_projectile(CombatSession& session, const RuntimeSkillExternalContextV1& context,
                                                            std::string& error) {
    auto* executor = services_.projectiles ? services_.projectiles() : nullptr;
    const auto tables = services_.projectile_tables ? services_.projectile_tables() : nullptr;
    if (!executor || !tables) return fail(error, "Rise requires the shared projectile executor and its tables");
    const auto row = projectile_row(*tables, kRiseProjectile);
    if (row < 0) return fail(error, "Rise ProjectileTable row TeleportTool is missing");
    const auto* caster = session.actor(context.caster);
    if (!caster) return fail(error, "Rise caster is absent");
    effects::RuntimeSessionSkillProjectileLaunchV1 request;
    request.owner = context.caster;
    request.generation = context.generation;
    request.occurrence = next_occurrence_++;
    request.row = row;
    request.origin = caster->transform.position;
    request.forward = look_vector(*caster);  // SpawnSkillProjectile(TeleportTool, 0.0): the front direction
    request.explicit_target = invalid_actor_id;
    // delayed_check: the Lua accepts only a contact with a non-Character target (a door or decor object). The port's
    // projectile contacts name ActorIds only, and every ActorId is a Character, so no contact is accepted here; the
    // projectile then ends on expiry (distance / lifetime), which is the landing the Lua records.
    request.check = [](const effects::RuntimeSessionSkillProjectileEventV1&, bool& accept, std::string& check_error) {
        accept = false;
        check_error.clear();
        return true;
    };
    const auto caster_id = context.caster;
    request.delayed = [this, caster_id](const effects::RuntimeSessionSkillProjectileEventV1& event, std::string& callback_error) {
        // OnDelayedSkill_(target, projectile): x, y, z = projectile:GetPosition(). Runs on expiry (peer nil) as in the Lua.
        rise_landing_[caster_id] = event.position;
        callback_error.clear();
        log("Necromancer skill Rise landing expired=" + std::string(event.expired ? "1" : "0") +
            " at=" + std::to_string(event.position[0]) + "," + std::to_string(event.position[1]) + "," +
            std::to_string(event.position[2]));
        return true;
    };
    std::uint64_t projectile_id = 0;
    if (!executor->launch_skill(tables, request, projectile_id, error)) return false;
    log("Necromancer skill Rise TeleportTool launched id=" + std::to_string(projectile_id));
    // wants_contacts = false: the check above rejects every contact, so the TeleportTool projectile needs no contact sensor.
    // The staged package has no authored AABB for this row, and a sensor registration for it failed the whole Pre.
    if (services_.projectile_launched && !services_.projectile_launched(projectile_id, false, error)) return false;
    return true;
}

// v21.8 w4-rise: Rise summon. OnSkill_ (prince_necromancer_rise.luac): count = checkslot(); with a landing position
// spawn(count) = Summon(RisenZombie, false, x, y, z) + LOCK + PlayFX(RISE_FX) + SetLevel, then a one-shot timer of
// SnS_Duration (killed(): Kill + UNLOCK). checkslot(): an empty slot; else a slot whose summon is dead (freed); else -1,
// and ForceKillAndSpawnNew kills the slot with the oldest spawn counter, stops its timer and spawns into it.
bool RuntimeNecromancerSkillOwnerV1::use_rise(CombatSession&, const RuntimeSkillExternalContextV1& context, std::string& error) {
    const auto landing = rise_landing_.find(context.caster);
    if (landing == rise_landing_.end()) {
        log("Necromancer skill Rise Use: no landing position (Lua x == nil), nothing to summon");
        return true;
    }
    if (!services_.summon || !services_.despawn || !services_.is_dead) {
        error = "Rise summon services are not wired";
        return false;
    }
    std::size_t slot = rise_slots_.size();
    for (std::size_t i = 0; i < rise_slots_.size() && slot == rise_slots_.size(); ++i)
        if (rise_slots_[i].actor == invalid_actor_id) slot = i;
    for (std::size_t i = 0; i < rise_slots_.size() && slot == rise_slots_.size(); ++i)
        if (services_.is_dead(rise_slots_[i].actor)) {
            if (!rise_release(i, error)) return false;
            slot = i;
        }
    if (slot == rise_slots_.size()) {  // ForceKillAndSpawnNew: the oldest summon
        slot = 0;
        for (std::size_t i = 1; i < rise_slots_.size(); ++i)
            if (rise_slots_[i].order < rise_slots_[slot].order) slot = i;
        if (!rise_release(slot, error)) return false;
    }
    ActorId risen = invalid_actor_id;
    std::string summon_error;
    if (!services_.summon("RisenZombie", landing->second, risen, summon_error)) {
        error = "Rise summon refused: " + summon_error;
        return false;
    }
    // SetLevel(plvl) and the spawn-pool level are the host's (the summon request carries the player level).
    // Presentation: PlayFX(RISE_FX) on the risen at its landing point.
    if (services_.play_effect) {
        std::string fx_error;
        if (!services_.play_effect("spell_dh2_prince_necromancer_rise", landing->second, fx_error))
            log("Necromancer skill Rise FX: " + fx_error);
    }
    // bufftime = FromFixed(SnS_Duration) of the Use-time sheet (CharacterProperties.SnS_Duration = 176, x256 ms), 10000 when 0.
    double bufftime_ms = double(context.formula_sheet ? (*context.formula_sheet)[kSnsDurationRawIndex] : 0) / 256.0;
    if (bufftime_ms <= 0.0) bufftime_ms = 10000.0;
    rise_slots_[slot].actor = risen;
    rise_slots_[slot].order = ++rise_spawn_counter_;
    rise_slots_[slot].timer = timers_.start(bufftime_ms, false,
        [this, slot](RuntimeSkillTimerIdV1, std::string& timer_error) -> bool {
            rise_slots_[slot].timer = 0;  // the one-shot timer has fired: killed(count)
            return rise_release(slot, timer_error);
        }, kOwnerRise);
    log("Necromancer skill Rise summoned RisenZombie actor=" + std::to_string(risen) + " slot=" + std::to_string(slot + 1) +
        " at=" + std::to_string(landing->second[0]) + "," + std::to_string(landing->second[1]) + "," +
        std::to_string(landing->second[2]) + " lifetimeMs=" + std::to_string(bufftime_ms));
    return true;
}

// killed() / KillAllSummoned(): stop the slot's timer, Kill + UNLOCK the summon when it is alive, free the slot.
bool RuntimeNecromancerSkillOwnerV1::rise_release(std::size_t slot, std::string& error) {
    RiseSlot& s = rise_slots_[slot];
    if (s.timer != 0) {
        timers_.stop(s.timer);
        s.timer = 0;
    }
    if (s.actor == invalid_actor_id) return true;
    const ActorId actor = s.actor;
    s.actor = invalid_actor_id;
    s.order = 0;
    if (services_.is_dead && services_.is_dead(actor)) return true;  // dead summons are already gone
    if (services_.despawn && !services_.despawn(actor, error)) return false;
    log("Necromancer skill Rise released actor=" + std::to_string(actor) + " slot=" + std::to_string(slot + 1));
    return true;
}

bool RuntimeNecromancerSkillOwnerV1::rise_landing_known(ActorId caster) const {
    return rise_landing_.count(caster) != 0;
}

void RuntimeNecromancerSkillOwnerV1::cleanup(CombatSession&, ActorId actor) {
    // OnSkillCleanUp: KillAllSummoned (Kill + UNLOCK of every live risen, timers stopped) and the counters.
    for (std::size_t slot = 0; slot < rise_slots_.size(); ++slot) {
        std::string error;
        if (!rise_release(slot, error)) log("Necromancer skill Rise cleanup: " + error);
    }
    rise_spawn_counter_ = 0;
    rise_landing_.erase(actor);
}

// ----- clock ----------------------------------------------------------------------------------------------------------
bool RuntimeNecromancerSkillOwnerV1::advance(CombatSession& session, double dt_ms, std::string& error) {
    error.clear();
    timer_session_ = &session;
    const bool ok = timers_.advance(dt_ms, error);
    timer_session_ = nullptr;
    return ok;
}

} // namespace dh::foundation::generic_skills
