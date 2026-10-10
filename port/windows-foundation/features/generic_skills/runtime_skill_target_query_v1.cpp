#include "runtime_skill_target_query_v1.hpp"

#include "../../playable_actor_world.hpp"
#include "../../../level-world/character_path_commands.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <map>
#include <set>

namespace dh::foundation::generic_skills {
namespace {
struct Candidate {
    ActorId id = invalid_actor_id;
    float distance = 0.0f;
    float angle = 0.0f;
    std::uint32_t flags = 1; // Source TargetSearch marks Character rows with bit0.
    std::uint32_t sort = 2;
};

float source_mul(float a, float b) { volatile float result = a * b; return result; }
float source_add(float a, float b) { volatile float result = a + b; return result; }
float source_sub(float a, float b) { volatile float result = a - b; return result; }
float source_div(float a, float b) { volatile float result = a / b; return result; }
float source_length(const float value[3]) {
    return std::sqrt(source_add(source_add(source_mul(value[0], value[0]),
                                            source_mul(value[1], value[1])),
                                source_mul(value[2], value[2])));
}
const float* source_target_position(const ActorState& actor) noexcept {
    // GameObject::GetTargetPosition (0x3935dc) chooses the raw transform
    // pointer when node+0x180 is null, before it ever looks at the cached
    // target position+0x184. A constructor-zeroed cache is not a target point.
    if (actor.source_target_node180 && *actor.source_target_node180 != 0 &&
        actor.source_target_position184)
        return actor.source_target_position184->data();
    return actor.transform.position.data();
}
float source_angle(const float delta[3], const float look[3]) {
    const float dot = source_add(source_add(source_mul(delta[0], look[0]),
                                             source_mul(delta[1], look[1])),
                                 source_mul(delta[2], look[2]));
    float result = std::acos(source_div(dot,
        source_mul(source_length(delta), source_length(look))));
    // The recovered source clears only the sign bit. This intentionally
    // preserves unordered NaN angle behavior for coincident targets.
    std::uint32_t bits = 0;
    std::memcpy(&bits, &result, sizeof(bits));
    bits &= 0x7fffffffu;
    std::memcpy(&result, &bits, sizeof(result));
    return result;
}

// Exact target_search.cpp heap comparator/push/pop behavior for sort=2.
bool source_less(const Candidate& a, const Candidate& b) {
    if (a.sort == 0) return false;
    if ((a.flags & 1u) != (b.flags & 1u)) return (b.flags & 1u) != 0;
    return a.sort == 1 ? a.distance > b.distance : a.angle > b.angle;
}
void source_push(std::vector<Candidate>& heap, Candidate value) {
    auto hole = heap.size();
    heap.push_back(value);
    while (hole) {
        const auto parent = (hole - 1) / 2;
        if (!source_less(heap[parent], value)) break;
        heap[hole] = heap[parent];
        hole = parent;
    }
    heap[hole] = value;
}
Candidate source_pop(std::vector<Candidate>& heap) {
    const auto result = heap.front();
    const auto n = heap.size() - 1;
    if (!n) { heap.pop_back(); return result; }
    auto value = heap.back();
    heap.pop_back();
    std::size_t hole = 0, right = 2;
    while (right < heap.size()) {
        const auto selected = source_less(heap[right], heap[right - 1]) ? right - 1 : right;
        heap[hole] = heap[selected];
        hole = selected;
        right = (selected + 1) * 2;
    }
    if (right == heap.size()) {
        heap[hole] = heap[right - 1];
        hole = right - 1;
    }
    while (hole) {
        const auto parent = (hole - 1) / 2;
        if (!source_less(heap[parent], value)) break;
        heap[hole] = heap[parent];
        hole = parent;
    }
    heap[hole] = value;
    return result;
}

bool fail(std::string& error, const char* text) {
    error = text;
    return false;
}
} // namespace

bool query_bashdown_character_targets_v1(
    const CombatSession& session, ActorId caster_id,
    const std::vector<ActorId>& authored_actor_order,
    const std::vector<SkillTargetSourceFactsV1>& source_facts,
    std::optional<bool> non_character_attackable_objects_absent,
    SkillActorTargetQueryV1& output, std::string& error) {
    output = {};
    error.clear();
    const auto* world = session.world();
    const auto* caster = session.actor(caster_id);
    const auto* caster_properties = world ? world->combat_properties(caster_id) : nullptr;
    if (!world || !caster || caster != world->find_actor(caster_id) || !caster_properties)
        return fail(error, "BashDown query requires the same live Session actor and source property owner");
    if (authored_actor_order.size() > 65536 || source_facts.size() > 65536)
        return fail(error, "BashDown source population order exceeds TargetSearch bounds");
    const float heading = caster->transform.rotation[2];
    const float source_radius = world->target_radius(*caster);
    if (!std::isfinite(heading) || !std::isfinite(source_radius) || source_radius < 0.0f)
        return fail(error, "BashDown source facing or same-world radius is invalid");
    const float look[3] = {std::sin(heading), -std::cos(heading), 0.0f};
    const float look_length = source_length(look);
    if (!std::isfinite(look_length) || look_length <= 0.0f)
        return fail(error, "BashDown source-facing vector is unavailable");

    std::map<ActorId, const SkillTargetSourceFactsV1*> facts_by_actor;
    for (const auto& facts : source_facts) {
        if (facts.actor == invalid_actor_id ||
            !facts_by_actor.emplace(facts.actor, &facts).second)
            return fail(error, "BashDown source actor facts contain invalid or duplicate identity");
    }
    std::set<ActorId> seen;
    std::vector<Candidate> heap;
    heap.reserve(authored_actor_order.size());
    for (const auto id : authored_actor_order) {
        if (id == invalid_actor_id || !seen.insert(id).second)
            return fail(error, "BashDown source population order contains invalid or duplicate actor identity");
        if (id == caster_id) continue; // Source Search excludes the owner object.
        const auto* candidate = session.actor(id);
        if (!candidate || candidate != world->find_actor(id))
            return fail(error, "BashDown population actor is absent from this Session world");
        const auto fact_it = facts_by_actor.find(id);
        if (fact_it == facts_by_actor.end())
            return fail(error, "Reached BashDown visible/zone/interactive query facts are missing for a source actor");
        const auto& facts = *fact_it->second;
        if (!facts.visible)
            return fail(error, "Reached source GameObject visible field is unknown");
        if (!*facts.visible) continue;
        if (!facts.zonable)
            return fail(error, "Reached source IsZonable result is unknown");
        if (*facts.zonable) {
            if (!facts.zoned)
                return fail(error, "Reached source zoned field is unknown");
            if (*facts.zoned) {
                if (!facts.in_zone)
                    return fail(error, "Reached source in-zone field is unknown");
                if (!*facts.in_zone) continue;
            }
        }
        if (!facts.interactive)
            return fail(error, "Reached source IsInteractive result is unknown");
        if (!*facts.interactive) continue;

        const auto* candidate_properties = world->combat_properties(id);
        if (!candidate_properties)
            return fail(error, "BashDown candidate source property owner is unavailable");
        // Character TargetSearch compares Character+1314 against Character+1310.
        // Shared target-facing V2 proves these alias resolved properties 199
        // Sneak_Detection and 198 Special_Sneak respectively.
        if (caster_properties->sheets.resolved[199] <
            candidate_properties->sheets.resolved[198]) continue;
        if (!world->eligible_target(*caster, *candidate)) continue; // alive/Enemy/targetable
        // CharacterWorldTargetOwnerV1::search_service resolves Character
        // interaction_radius from ai_props(AI, Character.resolved[1]).
        // This is the same source AI table and property sheet retained by the
        // generic Session world. ai_props preserves the original row8 fallback.
        const auto* candidate_ai = dh2::data::ai_props(
            world->factions(), candidate_properties->sheets.resolved[1]);
        if (!candidate_ai)
            return fail(error, "Original Character interaction-radius AI row/fallback8 is unavailable");
        const float target_radius = candidate_ai->interact_radius;
        const float* center = source_target_position(*candidate);
        const float* origin = source_target_position(*caster);
        const float delta[3] = {
            source_sub(center[0], origin[0]),
            source_sub(center[1], origin[1]),
            source_sub(center[2], origin[2])};
        const float distance = source_sub(source_sub(source_length(delta), target_radius), source_radius);
        if (distance > 160.0f) continue;
        const float angle = source_angle(delta, look);
        source_push(heap, {id, distance, angle, 1u, 2u});
    }

    SkillActorTargetQueryV1 next;
    next.caster = caster_id;
    next.source_character_order_preserved = true;
    Candidate selected_candidate{};
    while (!heap.empty()) {
        const auto current = source_pop(heap);
        if (next.ordered_character_targets.empty()) selected_candidate = current;
        next.ordered_character_targets.push_back(current.id);
    }
    if (next.ordered_character_targets.empty()) {
        next.status = non_character_attackable_objects_absent && *non_character_attackable_objects_absent
            ? SkillActorTargetQueryStatusV1::no_actor_target
            : SkillActorTargetQueryStatusV1::non_character_target_unknown;
        output = std::move(next);
        error.clear();
        return true;
    }
    next.status = SkillActorTargetQueryStatusV1::selected_character;
    next.selected = selected_candidate.id;
    next.selected_distance = selected_candidate.distance;
    next.selected_angle = selected_candidate.angle;
    output = std::move(next);
    error.clear();
    return true;
}

bool query_source_character_targets_v1(
    const CombatSession& session, ActorId caster_id,
    const std::vector<ActorId>& authored_actor_order,
    const std::vector<SkillTargetSourceFactsV1>& source_facts,
    std::optional<bool> non_character_attackable_objects_absent,
    float range, float cone_radians, SkillTargetSortV1 sort,
    SkillActorTargetQueryV1& output, std::string& error) {
    output = {};
    error.clear();
    if (!std::isfinite(range) || range < 0.0f || !std::isfinite(cone_radians) ||
        cone_radians < 0.0f || static_cast<unsigned>(sort) > 2u)
        return fail(error, "Source Character TargetListSearch range/cone/sort is invalid");
    const auto* world = session.world();
    const auto* caster = session.actor(caster_id);
    const auto* caster_properties = world ? world->combat_properties(caster_id) : nullptr;
    if (!world || !caster || caster != world->find_actor(caster_id) || !caster_properties)
        return fail(error, "Source target query requires the same live Session actor and source property owner");
    if (authored_actor_order.size() > 65536 || source_facts.size() > 65536)
        return fail(error, "Source population order exceeds TargetSearch bounds");
    const float heading = caster->transform.rotation[2];
    const float source_radius = world->target_radius(*caster);
    if (!std::isfinite(heading) || !std::isfinite(source_radius) || source_radius < 0.0f)
        return fail(error, "Source facing or same-world radius is invalid");
    const float look[3] = {std::sin(heading), -std::cos(heading), 0.0f};
    const float look_length = source_length(look);
    if (!std::isfinite(look_length) || look_length <= 0.0f)
        return fail(error, "Source-facing vector is unavailable");

    std::map<ActorId, const SkillTargetSourceFactsV1*> facts_by_actor;
    for (const auto& facts : source_facts) {
        if (facts.actor == invalid_actor_id ||
            !facts_by_actor.emplace(facts.actor, &facts).second)
            return fail(error, "Source actor facts contain invalid or duplicate identity");
    }
    std::set<ActorId> seen;
    std::vector<Candidate> heap;
    heap.reserve(authored_actor_order.size());
    for (const auto id : authored_actor_order) {
        if (id == invalid_actor_id || !seen.insert(id).second)
            return fail(error, "Source population order contains invalid or duplicate actor identity");
        if (id == caster_id) continue;
        const auto* candidate = session.actor(id);
        if (!candidate || candidate != world->find_actor(id))
            return fail(error, "Population actor is absent from this Session world");
        const auto fact_it = facts_by_actor.find(id);
        if (fact_it == facts_by_actor.end())
            return fail(error, "Reached source visibility/zone/interaction facts are missing");
        const auto& facts = *fact_it->second;
        if (!facts.visible) return fail(error, "Reached source GameObject visible field is unknown");
        if (!*facts.visible) continue;
        if (!facts.zonable) return fail(error, "Reached source IsZonable result is unknown");
        if (*facts.zonable) {
            if (!facts.zoned) return fail(error, "Reached source zoned field is unknown");
            if (*facts.zoned) {
                if (!facts.in_zone) return fail(error, "Reached source in-zone field is unknown");
                if (!*facts.in_zone) continue;
            }
        }
        if (!facts.interactive) return fail(error, "Reached source IsInteractive result is unknown");
        if (!*facts.interactive) continue;
        const auto* candidate_properties = world->combat_properties(id);
        if (!candidate_properties)
            return fail(error, "Candidate source property owner is unavailable");
        if (caster_properties->sheets.resolved[199] <
            candidate_properties->sheets.resolved[198]) continue;
        if (!world->eligible_target(*caster, *candidate)) continue;
        const auto* candidate_ai = dh2::data::ai_props(
            world->factions(), candidate_properties->sheets.resolved[1]);
        if (!candidate_ai)
            return fail(error, "Original Character interaction-radius AI row/fallback8 is unavailable");
        const float* center = source_target_position(*candidate);
        const float* origin = source_target_position(*caster);
        const float delta[3] = {source_sub(center[0], origin[0]),
                                source_sub(center[1], origin[1]),
                                source_sub(center[2], origin[2])};
        const float distance = source_sub(source_sub(source_length(delta),
            candidate_ai->interact_radius), source_radius);
        if (distance > range) continue;
        const float angle = source_angle(delta, look);
        if (cone_radians < 3.1415927410125732421875f && cone_radians < angle) continue;
        source_push(heap, {id, distance, angle, 1u, static_cast<std::uint32_t>(sort)});
    }

    SkillActorTargetQueryV1 next;
    next.caster = caster_id;
    next.source_character_order_preserved = sort == SkillTargetSortV1::source_order;
    Candidate selected_candidate{};
    while (!heap.empty()) {
        const auto current = source_pop(heap);
        if (next.ordered_character_targets.empty()) selected_candidate = current;
        next.ordered_character_targets.push_back(current.id);
    }
    if (next.ordered_character_targets.empty()) {
        next.status = non_character_attackable_objects_absent && *non_character_attackable_objects_absent
            ? SkillActorTargetQueryStatusV1::no_actor_target
            : SkillActorTargetQueryStatusV1::non_character_target_unknown;
        output = std::move(next);
        error.clear();
        return true;
    }
    next.status = SkillActorTargetQueryStatusV1::selected_character;
    next.selected = selected_candidate.id;
    next.selected_distance = selected_candidate.distance;
    next.selected_angle = selected_candidate.angle;
    output = std::move(next);
    error.clear();
    return true;
}

bool look_at_bashdown_target_v1(CombatSession& session,
                                SkillActorTargetQueryV1& query,
                                std::string& error) {
    error.clear();
    if (query.status != SkillActorTargetQueryStatusV1::selected_character ||
        query.caster == invalid_actor_id || query.selected == invalid_actor_id)
        return fail(error, "BashDown LookAt requires a selected character target from its source query");
    auto* world = session.world();
    auto* caster = session.actor(query.caster);
    const auto* target = session.actor(query.selected);
    if (!world || !caster || !target || caster != world->find_actor(query.caster) ||
        target != world->find_actor(query.selected))
        return fail(error, "BashDown LookAt actors no longer belong to the same Session world");
    const float* target_point = source_target_position(*target);
    dh2::character::LookAtState16 look{{caster->transform.position[0], caster->transform.position[1],
                                       caster->transform.position[2]}, caster->transform.rotation[2]};
    if (dh2_character_look_at_point(&look, target_point))
        return fail(error, "Original GameObject LookAt(Point) kernel rejected the same-world target");
    caster->transform.rotation[2] = look.heading_angle;
    query.look_at_heading = look.heading_angle;
    query.look_at_heading_known = true;
    error.clear();
    return true;
}

} // namespace dh::foundation::generic_skills
