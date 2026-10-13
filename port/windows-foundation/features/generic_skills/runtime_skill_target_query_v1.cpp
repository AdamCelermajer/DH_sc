#include "runtime_skill_target_query_v1.hpp"

#include "../../playable_actor_world.hpp"
#include "../../../level-world/character_path_commands.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
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
    SkillActorTargetQueryV1& output, std::string& error,
    const SkillObjectTargetArmV1* objects,
    const SkillTargetOriginV1* search_origin) {
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
    float source_radius = world->target_radius(*caster);
    if (search_origin) source_radius = search_origin->radius;
    if (!std::isfinite(heading) || !std::isfinite(source_radius) || source_radius < 0.0f)
        return fail(error, "Source facing or same-world radius is invalid");
    float look[3] = {std::sin(heading), -std::cos(heading), 0.0f};
    if (search_origin) for (unsigned axis = 0; axis < 3; ++axis) look[axis] = search_origin->look[axis];
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
        if (id == caster_id || (search_origin && id == search_origin->exclude)) continue;
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
        const float* origin = search_origin ? search_origin->position.data() : source_target_position(*caster);
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

    if (objects) {
        if (!objects->facts || objects->authored_order.size() != world->objects().size())
            return fail(error, "Source object query requires complete same-world typed registry facts/order");
        std::set<ObjectId> seen_objects;
        for (const auto id : objects->authored_order) {
            const auto* object = world->find_object(id);
            if (!object || !seen_objects.insert(id).second)
                return fail(error, "Source object query contains stale or duplicate identity");
            SkillObjectTargetFactsV1 facts;
            if (!objects->facts(*object, caster_id, facts, error)) return false;
            // GetInteractionType is a pure typed admission fact. A proven
            // non8 can never enter AttackableOnly regardless of earlier gates;
            // do not demand unavailable zone data for this excluded domain.
            if (facts.interaction_type && *facts.interaction_type != 8) continue;
            if (!facts.visible) return fail(error, "Reached object visible field is unknown");
            if (!*facts.visible) continue;
            if (!facts.zonable) return fail(error, "Reached object IsZonable result is unknown");
            if (*facts.zonable) {
                if (!facts.zoned) return fail(error, "Reached object zoned field is unknown");
                if (*facts.zoned) {
                    if (!facts.in_zone) return fail(error, "Reached object in-zone field is unknown");
                    if (!*facts.in_zone) continue;
                }
            }
            if (!facts.interactive) return fail(error, "Reached object IsInteractive result is unknown");
            if (!*facts.interactive) continue;
            if (!facts.interaction_type) return fail(error, "Reached object interaction type is unknown");
            if (*facts.interaction_type != 8) continue;
            if (!facts.interaction_radius || !std::isfinite(*facts.interaction_radius) || *facts.interaction_radius < 0)
                return fail(error, "Reached object interaction radius is unknown/invalid");
            const float* origin = search_origin ? search_origin->position.data() : source_target_position(*caster);
            const auto& point = object->transform.position;
            const float delta[3] = {source_sub(point[0], origin[0]), source_sub(point[1], origin[1]), source_sub(point[2], origin[2])};
            const float distance = source_sub(source_sub(source_length(delta), *facts.interaction_radius), source_radius);
            if (distance > range) continue;
            const float angle = source_angle(delta, look);
            if (cone_radians < 3.1415927410125732421875f && cone_radians < angle) continue;
            source_push(heap, {id, distance, angle, 0u, static_cast<std::uint32_t>(sort)});
        }
    }

    if (objects && !objects->registry_order.empty()) {
        // Rebuild in genuine mixed enrollment order so NoSort and heap ties
        // retain the same intrusive-registry insertion semantics.
        std::map<ObjectId,Candidate> admitted;
        for(const auto& candidate:heap)admitted.emplace(candidate.id,candidate);
        heap.clear();std::set<ObjectId> registry_seen;
        for(const auto id:objects->registry_order) {
            if(!id||!registry_seen.insert(id).second)return fail(error,"Source mixed registry order contains invalid/duplicate identity");
            const auto found=admitted.find(id);
            if(found!=admitted.end()){source_push(heap,found->second);admitted.erase(found);}
        }
        if(!admitted.empty())return fail(error,"Source mixed registry order omits an admitted target");
    }
    SkillActorTargetQueryV1 next;
    next.caster = caster_id;
    next.source_character_order_preserved = sort == SkillTargetSortV1::source_order;
    Candidate selected_candidate{};
    while (!heap.empty()) {
        const auto current = source_pop(heap);
        if (next.ordered_targets.empty()) selected_candidate = current;
        next.ordered_targets.push_back(current.id);
        if (current.flags & 1u) next.ordered_character_targets.push_back(current.id);
        else next.ordered_object_targets.push_back(current.id);
    }
    if (next.ordered_targets.empty()) {
        next.status = objects || (non_character_attackable_objects_absent && *non_character_attackable_objects_absent)
            ? SkillActorTargetQueryStatusV1::no_actor_target
            : SkillActorTargetQueryStatusV1::non_character_target_unknown;
        output = std::move(next);
        error.clear();
        return true;
    }
    next.status = selected_candidate.flags & 1u ? SkillActorTargetQueryStatusV1::selected_character
                                               : SkillActorTargetQueryStatusV1::selected_object;
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
    if ((query.status != SkillActorTargetQueryStatusV1::selected_character &&
         query.status != SkillActorTargetQueryStatusV1::selected_object) ||
        query.caster == invalid_actor_id || query.selected == invalid_actor_id)
        return fail(error, "BashDown LookAt requires a selected character target from its source query");
    auto* world = session.world();
    auto* caster = session.actor(query.caster);
    const auto* target = session.actor(query.selected);
    const auto* object = world ? world->find_object(query.selected) : nullptr;
    if (!world || !caster || (!target && !object) || caster != world->find_actor(query.caster) ||
        (target && target != world->find_actor(query.selected)))
        return fail(error, "BashDown LookAt actors no longer belong to the same Session world");
    const float* target_point = target ? source_target_position(*target) : object->transform.position.data();
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

// ---- v21.8 ES-API actor-side TargetList arms ----

void SkillTargetListBackupsV1::backup(const std::string& name, std::vector<ActorId> ordered_targets) {
    lists_[name] = std::move(ordered_targets);
}

std::vector<ActorId> SkillTargetListBackupsV1::get(const std::string& name) const {
    const auto found = lists_.find(name);
    return found == lists_.end() ? std::vector<ActorId>{} : found->second;
}

std::array<float, 3> relative_forward_search_origin_v1(const std::array<float, 3>& caster_position,
                                                       const std::array<float, 3>& look, float forward) {
    return {caster_position[0] + forward * look[0],
            caster_position[1] + forward * look[1],
            caster_position[2] + forward * look[2]};
}

namespace {
// Shared range/cone/sort tail of the actor-side arms. Candidates are visited in the caller's order; `admit` is the
// arm-specific population predicate (Player population or the stored backup list). Same distance and angle
// arithmetic and heap as the Enemy path above (interaction radius of the candidate, radius of the caster).
bool run_actor_arm_search_v1(
    const PlayableActorWorld& world, ActorId caster_id,
    const std::vector<ActorId>& candidates,
    const std::function<bool(ActorId)>& admit,
    float range, float cone_radians, SkillTargetSortV1 sort,
    const std::optional<std::array<float, 3>>& origin,
    SkillActorTargetQueryV1& output, std::string& error) {
    output = {};
    error.clear();
    if (!std::isfinite(range) || range < 0.0f || !std::isfinite(cone_radians) ||
        cone_radians < 0.0f || static_cast<unsigned>(sort) > 2u)
        return fail(error, "Actor TargetListSearch range/cone/sort is invalid");
    const auto* caster = world.find_actor(caster_id);
    const auto* caster_properties = world.combat_properties(caster_id);
    if (!caster || !caster_properties)
        return fail(error, "Actor TargetListSearch requires a bound caster with its property sheet");
    if (candidates.size() > 65536) return fail(error, "Actor TargetListSearch population exceeds TargetSearch bounds");
    const float heading = caster->transform.rotation[2];
    const float source_radius = world.target_radius(*caster);
    if (!std::isfinite(heading) || !std::isfinite(source_radius) || source_radius < 0.0f)
        return fail(error, "Actor TargetListSearch source facing or radius is invalid");
    const float look[3] = {std::sin(heading), -std::cos(heading), 0.0f};
    const float* origin_point = origin ? origin->data() : source_target_position(*caster);

    std::set<ActorId> seen;
    std::vector<Candidate> heap;
    heap.reserve(candidates.size());
    for (const auto id : candidates) {
        if (id == invalid_actor_id || !seen.insert(id).second)
            return fail(error, "Actor TargetListSearch candidate identity is invalid or duplicated");
        if (id == caster_id) continue; // the searcher is never its own target
        const auto* candidate = world.find_actor(id);
        if (!candidate || !candidate->alive()) continue;
        if (!admit(id)) continue;
        const auto* candidate_properties = world.combat_properties(id);
        if (!candidate_properties) return fail(error, "Actor TargetListSearch candidate property sheet is unavailable");
        const auto* candidate_ai = dh2::data::ai_props(world.factions(), candidate_properties->sheets.resolved[1]);
        if (!candidate_ai)
            return fail(error, "Original Character interaction-radius AI row/fallback8 is unavailable");
        const float* center = source_target_position(*candidate);
        const float delta[3] = {source_sub(center[0], origin_point[0]),
                                source_sub(center[1], origin_point[1]),
                                source_sub(center[2], origin_point[2])};
        const float distance = source_sub(source_sub(source_length(delta), candidate_ai->interact_radius), source_radius);
        if (distance > range) continue;
        const float angle = source_angle(delta, look);
        if (cone_radians < 3.1415927410125732421875f && cone_radians < angle) continue;
        source_push(heap, {id, distance, angle, 1u, static_cast<std::uint32_t>(sort)});
    }

    output.caster = caster_id;
    output.source_character_order_preserved = sort == SkillTargetSortV1::source_order;
    Candidate selected_candidate{};
    while (!heap.empty()) {
        const auto current = source_pop(heap);
        if (output.ordered_character_targets.empty()) selected_candidate = current;
        output.ordered_character_targets.push_back(current.id);
    }
    if (output.ordered_character_targets.empty()) {
        output.status = SkillActorTargetQueryStatusV1::no_actor_target;
        return true;
    }
    output.status = SkillActorTargetQueryStatusV1::selected_character;
    output.selected = selected_candidate.id;
    output.selected_distance = selected_candidate.distance;
    output.selected_angle = selected_candidate.angle;
    return true;
}
} // namespace

bool query_player_character_targets_v1(
    const PlayableActorWorld& world, ActorId caster,
    const std::vector<ActorId>& authored_actor_order,
    float range, float cone_radians, SkillTargetSortV1 sort,
    const std::optional<std::array<float, 3>>& origin,
    SkillActorTargetQueryV1& output, std::string& error) {
    const auto admit = [&world](ActorId id) {
        const auto* traits = world.traits(id);
        return traits && traits->is_player;
    };
    return run_actor_arm_search_v1(world, caster, authored_actor_order, admit,
                                   range, cone_radians, sort, origin, output, error);
}

bool query_backup_character_targets_v1(
    const PlayableActorWorld& world, ActorId caster,
    const std::vector<ActorId>& backup_order,
    float range, float cone_radians, SkillTargetSortV1 sort,
    const std::optional<std::array<float, 3>>& origin,
    SkillActorTargetQueryV1& output, std::string& error) {
    const auto admit = [](ActorId) { return true; };
    return run_actor_arm_search_v1(world, caster, backup_order, admit,
                                   range, cone_radians, sort, origin, output, error);
}

} // namespace dh::foundation::generic_skills
