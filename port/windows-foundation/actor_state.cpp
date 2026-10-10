#include "actor_state.hpp"

#include <algorithm>
#include <cmath>
#include <unordered_set>

namespace dh::foundation {
namespace {
bool valid_text(const std::string& value) {
    if (value.empty() || value.size() > character_text_limit) return false;
    for (unsigned char ch : value) if (ch < 0x20 || ch == 0x7f) return false;
    return true;
}
bool nonnegative(float value) { return std::isfinite(value) && value >= 0.0f; }
}

bool ActorState::alive() const noexcept {
    return std::isfinite(health) && health > 0.0f;
}

bool validate_actor_state(const ActorState& actor, std::string& error) {
    const auto fail = [&](const char* message) { error = message; return false; };
    if (actor.id == invalid_actor_id) return fail("actor has no world ID");
    if (!valid_text(actor.definition_id)) return fail("actor definition ID is invalid");
    if (!actor.class_id.empty() && !valid_text(actor.class_id)) return fail("actor class ID is invalid");
    if (actor.faction_id < -1) return fail("actor faction ID is invalid");
    for (float value : actor.transform.position)
        if (!std::isfinite(value)) return fail("actor position is nonfinite");
    for (float value : actor.transform.rotation)
        if (!std::isfinite(value)) return fail("actor rotation is nonfinite");
    for (float value : actor.transform.scale)
        if (!std::isfinite(value) || value <= 0.0f) return fail("actor scale must be finite and positive");
    if (!nonnegative(actor.health) || !nonnegative(actor.max_health) || actor.max_health <= 0.0f ||
        actor.health > actor.max_health) return fail("actor health range is invalid");
    if (!nonnegative(actor.resource) || !nonnegative(actor.max_resource) ||
        actor.resource > actor.max_resource) return fail("actor resource range is invalid");
    if (!nonnegative(actor.action_elapsed_seconds)) return fail("actor action elapsed time is invalid");
    switch (actor.action) {
    case CharacterAction::idle: case CharacterAction::moving:
    case CharacterAction::attacking: case CharacterAction::casting:
    case CharacterAction::hurt: case CharacterAction::dead: break;
    default: return fail("actor action is invalid");
    }
    if ((actor.health == 0.0f) != (actor.action == CharacterAction::dead))
        return fail("actor health and dead action disagree");
    if (actor.target_id == actor.id) return fail("actor cannot target itself");
    if (!actor.alive() && actor.target_id != invalid_actor_id)
        return fail("dead actor retains a target");
    if (actor.equipment.size() > character_collection_limit ||
        actor.attack_ids.size() > character_collection_limit)
        return fail("actor reference collection exceeds limit");
    std::unordered_set<std::string> slots;
    for (const auto& binding : actor.equipment) {
        if (!valid_text(binding.slot) || !valid_text(binding.definition_id) ||
            (binding.item_instance_id && !valid_text(*binding.item_instance_id)))
            return fail("actor equipment reference is invalid");
        if (!slots.insert(binding.slot).second) return fail("actor equipment slot is duplicated");
    }
    std::unordered_set<std::string> attacks;
    for (const auto& attack : actor.attack_ids) {
        if (!valid_text(attack)) return fail("actor attack ID is invalid");
        if (!attacks.insert(attack).second) return fail("actor attack ID is duplicated");
    }
    if (actor.persistent_character_id && !valid_text(*actor.persistent_character_id))
        return fail("actor persistent character reference is invalid");
    error.clear();
    return true;
}

void reset_actor_action(ActorState& actor, CharacterAction action) noexcept {
    actor.action = actor.alive() ? action : CharacterAction::dead;
    actor.action_elapsed_seconds = 0.0f;
    if (actor.action == CharacterAction::dead) actor.target_id = invalid_actor_id;
}

float apply_actor_damage_health_prefix(ActorState& actor, float amount) noexcept {
    if (!actor.alive() || !std::isfinite(amount) || amount <= 0.0f) return 0.0f;
    const float removed = std::min(actor.health, amount);
    actor.health -= removed;
    return removed;
}
float apply_actor_damage(ActorState& actor,float amount)noexcept{
    const float removed=apply_actor_damage_health_prefix(actor,amount);
    if(removed>0&&!actor.alive())reset_actor_action(actor,CharacterAction::dead);
    return removed;
}

} // namespace dh::foundation
