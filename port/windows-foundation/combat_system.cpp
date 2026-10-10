#include "combat_system.hpp"
#include "animation_markers.hpp"
#include <algorithm>
#include <cmath>

namespace dh::foundation {

bool validate_attack_definition(const AttackDefinition& definition, std::string& error) {
    error.clear();
    if (definition.id.empty() || definition.animation_clip_id.empty()) {
        error = "Attack requires definition and animation clip IDs"; return false;
    }
    if (!std::isfinite(definition.minimum_range) || !std::isfinite(definition.maximum_range)
        || definition.minimum_range < 0 || definition.maximum_range <= 0
        || definition.minimum_range > definition.maximum_range) {
        error = "Attack range is invalid or unbound"; return false;
    }
    if (definition.geometry != AttackGeometry::melee_radius && definition.geometry != AttackGeometry::ranged_band) {
        error = "Attack geometry is unsupported"; return false;
    }
    if (definition.geometry == AttackGeometry::melee_radius && definition.minimum_range != 0) {
        error = "Melee radius geometry has no minimum range"; return false;
    }
    if (!std::isfinite(definition.cooldown_seconds) || definition.cooldown_seconds < 0) {
        error = "Attack cooldown is invalid"; return false;
    }
    if (definition.cooldown_timing != CooldownTiming::attack_start
        && definition.cooldown_timing != CooldownTiming::attack_departure) {
        error = "Attack cooldown timing is unsupported"; return false;
    }
    if (definition.damage_markers.empty() || definition.damage_markers.size() > character_collection_limit) {
        error = "Attack requires bounded authored damage marker bindings"; return false;
    }
    std::set<std::string> names;
    for (const auto& binding : definition.damage_markers) {
        if (binding.marker_name.empty() || binding.damage_source_id.empty()
            || !names.insert(binding.marker_name).second) {
            error = "Attack damage marker bindings must be named, resolved, and unique"; return false;
        }
    }
    return true;
}

bool attack_target_in_range(const AttackDefinition& definition, const ActorState& attacker,
                            const ActorState& target, float target_radius) noexcept {
    double distance_squared = 0;
    for (std::size_t axis = 0; axis < 3; ++axis) {
        const double a = attacker.transform.position[axis], b = target.transform.position[axis];
        if (!std::isfinite(a) || !std::isfinite(b)) return false;
        const double delta = a - b;
        distance_squared += delta * delta;
    }
    if (!std::isfinite(definition.maximum_range) || definition.maximum_range <= 0) return false;
    if (definition.geometry == AttackGeometry::melee_radius) {
        if (!std::isfinite(target_radius) || target_radius < 0) return false;
        const double reach = double(definition.maximum_range) + target_radius;
        // Recovered dh2_attack_melee_distance: 3D squared distance, strict boundary.
        return distance_squared < reach * reach;
    }
    if (definition.geometry != AttackGeometry::ranged_band
        || !std::isfinite(definition.minimum_range) || definition.minimum_range < 0
        || definition.minimum_range > definition.maximum_range) return false;
    // Recovered dh2_attack_ranged_distance: inclusive lower and upper boundaries.
    const double low = definition.minimum_range, high = definition.maximum_range;
    return low * low <= distance_squared && distance_squared <= high * high;
}

bool CombatSystem::valid_pair(ActorId attacker, ActorId target, ActorState*& owner, ActorState*& victim) {
    owner = world_.find_actor(attacker); victim = world_.find_actor(target);
    return attacker != invalid_actor_id && target != invalid_actor_id && attacker != target
        && owner && victim && owner->id == attacker && victim->id == target
        && owner->alive() && victim->alive() && world_.eligible_target(*owner, *victim);
}

bool CombatSystem::validate_begin(ActorId attacker, ActorId target, const AttackDefinition& definition,
                         std::uint64_t generation, std::string& error) {
    error.clear();
    if (!validate_attack_definition(definition, error)) return false;
    if (!generation) { error = "Attack requires a selected animation generation"; return false; }
    ActorState *owner = nullptr, *victim = nullptr;
    if(target==invalid_actor_id&&definition.geometry==AttackGeometry::melee_radius){
        owner=world_.find_actor(attacker);
        if(attacker==invalid_actor_id||!owner||owner->id!=attacker||!owner->alive()){
            error="Attack owner is invalid";return false;
        }
    }else if (!valid_pair(attacker, target, owner, victim)) { error = "Attack actors or target policy are invalid"; return false; }
    if (std::find(owner->attack_ids.begin(), owner->attack_ids.end(), definition.id) == owner->attack_ids.end()) {
        error = "Attack is not bound to this actor"; return false;
    }
    if (owner->action != CharacterAction::idle && owner->action != CharacterAction::moving) {
        error = "Actor action does not permit starting an attack"; return false;
    }
    const auto existing = actors_.find(attacker);
    if (existing != actors_.end() && (existing->second.attack || existing->second.cooldown > 0)) {
        error = "Actor attack is active or cooling down"; return false;
    }
    if (victim&&!attack_target_in_range(definition, *owner, *victim, world_.target_radius(*victim))) {
        error = "Attack target is out of range"; return false;
    }
    return true;
}

bool CombatSystem::begin(ActorId attacker, ActorId target, const AttackDefinition& definition,
                         std::uint64_t generation, std::string& error) {
    if(!validate_begin(attacker,target,definition,generation,error))return false;
    auto* owner=world_.find_actor(attacker);
    AttackRun run;
    run.target = target; run.definition = definition; run.generation = generation;
    auto& record = actors_[attacker];
    record.attack = std::move(run);
    record.cooldown = definition.cooldown_timing == CooldownTiming::attack_start ? definition.cooldown_seconds : 0;
    reset_actor_action(*owner, CharacterAction::attacking);
    owner->target_id = target;
    return true;
}

bool CombatSystem::consume_marker(ActorId attacker, const MarkerOccurrence& occurrence,
                                 DamageEvent& result, std::string& error) {
    return consume_marker(attacker,occurrence,result,error,false);
}
bool CombatSystem::consume_marker(ActorId attacker,const MarkerOccurrence& occurrence,
    DamageEvent& result,std::string& error,bool defer_death_state){
    result = {}; error.clear();
    cleanup_invalid();
    auto record = actors_.find(attacker);
    if (record == actors_.end() || !record->second.attack) return true;
    auto& run = *record->second.attack;
    if (occurrence.generation != run.generation) return true;
    // Attack clips are single-shot. Looping replay cannot turn one attack into
    // repeated damage; a new attack requires a new begin and animation generation.
    if (occurrence.cycle != 0) return true;
    const auto binding = std::find_if(run.definition.damage_markers.begin(), run.definition.damage_markers.end(),
        [&](const DamageMarkerBinding& item) { return item.marker_name == occurrence.marker.name; });
    if (binding == run.definition.damage_markers.end()) return true;
    const auto key = std::make_tuple(occurrence.generation, occurrence.cycle,
                                    std::size_t(occurrence.marker.group), std::size_t(occurrence.marker.index));
    if (run.delivered.count(key)) return true;
    if(run.target==invalid_actor_id){
        // The ordinary null-target swing owns its animation until authored End.
        // Consume this event without target lookup, formula/RNG or damage.
        run.delivered.insert(key);return true;
    }
    ActorState *owner = nullptr, *victim = nullptr;
    if (!valid_pair(attacker, run.target, owner, victim)) { interrupt(attacker); return true; }
    // Consume misses as well: delivering this same authored event after a target
    // returns to range must not retroactively apply a hit.
    if (!attack_target_in_range(run.definition, *owner, *victim, world_.target_radius(*victim))) {
        run.delivered.insert(key); return true;
    }
    float damage = 0;
    std::optional<std::uint32_t> outcomes;
    std::optional<std::uint32_t> source_mask;
    if (!world_.resolve_damage_with_outcomes(binding->damage_source_id, *owner, *victim,
            binding->marker_name, damage, outcomes, source_mask, error)) {
        if (error.empty()) error = "Authored attack damage provider is unavailable";
        return false;
    }
    if (!std::isfinite(damage) || damage < 0) { error = "Damage provider returned an invalid amount"; return false; }
    run.delivered.insert(key);
    result.attacker = attacker; result.target = run.target;
    result.attack_id = run.definition.id; result.marker_name = occurrence.marker.name;
    result.source_id = binding->damage_source_id;
    result.requested_damage = damage;
    result.source_outcomes = outcomes;
    result.source_mask = source_mask;
    result.health_removed = defer_death_state?apply_actor_damage_health_prefix(*victim,damage):apply_actor_damage(*victim, damage);
    result.applied = true; result.target_died = !victim->alive();
    if (result.target_died&&!defer_death_state) cleanup_invalid();
    return true;
}

bool CombatSystem::apply_calculated_hit(ActorId attacker, ActorId target,
    const std::string& source_id, const std::string& marker_name, float amount,
    std::optional<std::uint32_t> outcomes, std::optional<std::uint32_t> source_mask,
    DamageEvent& result, std::string& error) {
    return apply_calculated_hit(attacker,target,source_id,marker_name,amount,outcomes,source_mask,result,error,false);
}
bool CombatSystem::apply_calculated_hit(ActorId attacker,ActorId target,
    const std::string& source_id,const std::string& marker_name,float amount,
    std::optional<std::uint32_t> outcomes,std::optional<std::uint32_t> source_mask,
    DamageEvent& result,std::string& error,bool defer_death_state){
    result = {}; error.clear();
    if (source_id.empty() || marker_name.empty()) {
        error = "Calculated hit requires source and occurrence identifiers";
        return false;
    }
    if (!std::isfinite(amount) || amount < 0) {
        error = "Calculated hit amount must be finite and nonnegative";
        return false;
    }
    ActorState* owner = world_.find_actor(attacker);
    ActorState* victim = world_.find_actor(target);
    if (attacker == invalid_actor_id || target == invalid_actor_id || attacker == target
        || !owner || !victim || owner->id != attacker || victim->id != target
        || !owner->alive() || !victim->alive()) {
        error = "Calculated hit actor records are not live in this world";
        return false;
    }
    result.attacker = attacker;
    result.target = target;
    result.source_id = source_id;
    result.marker_name = marker_name;
    result.requested_damage = amount;
    result.source_outcomes = outcomes;
    result.source_mask = source_mask;
    result.health_removed = defer_death_state?apply_actor_damage_health_prefix(*victim,amount):apply_actor_damage(*victim, amount);
    result.applied = true;
    result.target_died = !victim->alive();
    return true;
}

bool CombatSystem::finish(ActorId attacker, std::uint64_t generation) {
    const auto record = actors_.find(attacker);
    if (record == actors_.end() || !record->second.attack || record->second.attack->generation != generation) return false;
    return interrupt(attacker);
}

bool CombatSystem::interrupt(ActorId attacker) {
    const auto record = actors_.find(attacker);
    if (record == actors_.end() || !record->second.attack) return false;
    if (record->second.attack->definition.cooldown_timing == CooldownTiming::attack_departure)
        record->second.cooldown = record->second.attack->definition.cooldown_seconds;
    record->second.attack.reset();
    if (auto* owner = world_.find_actor(attacker)) {
        owner->target_id = invalid_actor_id;
        if (owner->action == CharacterAction::attacking) reset_actor_action(*owner, owner->alive() ? CharacterAction::idle : CharacterAction::dead);
    }
    return true;
}

void CombatSystem::cleanup_invalid() {
    for (auto it = actors_.begin(); it != actors_.end();) {
        auto* owner = world_.find_actor(it->first);
        if (!owner || !owner->alive() || owner->id != it->first) {
            interrupt(it->first);
            it = actors_.erase(it); continue;
        }
        if (it->second.attack) {
            if (!active_attack_valid(it->first)) interrupt(it->first);
        }
        if (!it->second.attack && it->second.cooldown <= 0) it = actors_.erase(it);
        else ++it;
    }
}

bool CombatSystem::update(double seconds, std::string& error) {
    error.clear();
    if (!std::isfinite(seconds) || seconds < 0) { error = "Combat elapsed time must be finite and nonnegative"; return false; }
    cleanup_invalid();
    for (auto& entry : actors_) entry.second.cooldown = std::max(0.0, entry.second.cooldown - seconds);
    cleanup_invalid();
    return true;
}

void CombatSystem::clear() {
    for (const auto& entry : actors_) interrupt(entry.first);
    actors_.clear();
}
bool CombatSystem::attacking(ActorId attacker) const noexcept {
    const auto record = actors_.find(attacker);
    return record != actors_.end() && bool(record->second.attack);
}
bool CombatSystem::active_target_valid(ActorId attacker){
    const auto record=actors_.find(attacker);
    if(record==actors_.end()||!record->second.attack)return false;
    ActorState* owner=nullptr;ActorState* target=nullptr;
    return valid_pair(attacker,record->second.attack->target,owner,target)&&
        owner->action==CharacterAction::attacking&&owner->target_id==record->second.attack->target;
}
bool CombatSystem::active_attack_valid(ActorId attacker){
    const auto record=actors_.find(attacker);
    if(record==actors_.end()||!record->second.attack)return false;
    const auto& run=*record->second.attack;
    if(run.target!=invalid_actor_id)return active_target_valid(attacker);
    const auto* owner=world_.find_actor(attacker);
    return run.definition.geometry==AttackGeometry::melee_radius&&attacker!=invalid_actor_id&&
        owner&&owner->id==attacker&&owner->alive()&&
        owner->action==CharacterAction::attacking&&owner->target_id==invalid_actor_id;
}
double CombatSystem::cooldown_remaining(ActorId attacker) const noexcept {
    const auto record = actors_.find(attacker);
    return record == actors_.end() ? 0.0 : record->second.cooldown;
}

} // namespace dh::foundation
