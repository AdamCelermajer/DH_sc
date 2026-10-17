#include "character_ai_set_target.hpp"

#include <cstddef>
#include <cstdint>

namespace dh2::character::set_target {
namespace {

bool aligned(const void* pointer, std::uintptr_t alignment) {
    return pointer &&
           (reinterpret_cast<std::uintptr_t>(pointer) & (alignment - 1U)) == 0;
}

bool overlaps(const void* first, std::size_t first_size,
              const void* second, std::size_t second_size) {
    const auto a = reinterpret_cast<std::uintptr_t>(first);
    const auto b = reinterpret_cast<std::uintptr_t>(second);
    if (a > UINTPTR_MAX - first_size || b > UINTPTR_MAX - second_size) return true;
    return first_size && second_size && a < b + second_size && b < a + first_size;
}

bool valid_state(const State* state) {
    return aligned(state, alignof(State)) && state->identity &&
           aligned(state->owner, alignof(OwnerFacts)) && state->owner->identity &&
           state->reserved == 0 && state->owner->reserved == 0;
}

bool valid_services(const Services* services) {
    return aligned(services, alignof(Services)) && services->invoke;
}

bool invoke(const State* state, const OwnerFacts* owner, const Services* services,
            Operation operation, std::uint32_t key, std::uintptr_t target,
            Response& response) {
    response = {};
    const Request request{static_cast<std::uint32_t>(operation), key,
                          state->identity, owner ? owner->identity : 0, target};
    return services->invoke(services->context, &request, &response) == 0;
}

bool debug_switch(const State* state, const Services* services, DebugKey key,
                  std::uint32_t& enabled) {
    Response ignored{};
    if (!invoke(state, nullptr, services, debug_switches_load, 0, 0, ignored)) {
        return false;
    }
    Response result{};
    if (!invoke(state, nullptr, services, debug_switch_lookup,
                static_cast<std::uint32_t>(key), 0, result)) {
        return false;
    }
    enabled = result.word;
    return true;
}

// Character::GetCharAIId is called by the source after storing a nonnull
// target. Its result is intentionally discarded by AI_SetTarget.
std::int32_t get_char_ai_id(const OwnerFacts* owner, std::int32_t count) {
    const auto id = owner->character_ai_id;
    if (id >= 0 && id < count) return id;
    return 8;  // CharacterProperties::Basic fallback
}

int update_target_snapshots(State* state, const Services* services) {
    if (!state->target) return complete;

    const OwnerFacts* owner = state->owner;
    if (!aligned(owner, alignof(OwnerFacts)) || !owner->identity) {
        return invalid_argument;
    }
    const std::int32_t ignored_ai_id = get_char_ai_id(owner, services->ai_property_count);
    (void)ignored_ai_id;

    // These are fresh AI+0x40 and AI+0x44 reads after GetCharAIId.
    const auto current_target = state->target;
    if (current_target != state->last_target) {
        state->sticky = 0;
        state->last_target = current_target;
    }

    Response dead{};
    if (!invoke(state, nullptr, services, target_is_dead, 0,
                current_target, dead)) {
        return source_service_failed;
    }
    // The native virtual returns bool. Preserve its low-byte XOR behavior.
    state->alive_snapshot = static_cast<std::uint8_t>(dead.word ^ 1U);

    // Source reloads AI+0x40 after IsDead; AI_IsInSight handles a null
    // argument by consulting the live target itself.
    const auto sight_argument = state->target;
    const OwnerFacts* sight_owner = state->owner;
    if (!aligned(sight_owner, alignof(OwnerFacts)) || !sight_owner->identity) {
        return invalid_argument;
    }
    Response sight{};
    if (!invoke(state, sight_owner, services, ai_is_in_sight, 0,
                sight_argument, sight)) {
        return source_service_failed;
    }
    state->sight_snapshot = static_cast<std::uint8_t>(sight.word);
    return complete;
}

}  // namespace

extern "C" int dh2_character_ai_set_target(State* state, std::uintptr_t target,
                                             std::uint8_t force,
                                             const Services* services) {
    if (!valid_state(state) || !valid_services(services) || force > 1 ||
        overlaps(state, sizeof(*state), state->owner, sizeof(*state->owner)) ||
        overlaps(state, sizeof(*state), services, sizeof(*services)) ||
        overlaps(state->owner, sizeof(*state->owner), services, sizeof(*services))) {
        return invalid_argument;
    }

    // AI_SetTarget first stores the requested target, even on force and null.
    state->requested_target = target;
    if (force) {
        state->target = target;
        return complete;
    }

    // The owner is loaded before the debug-manager calls. Keep this owner for
    // the source marker write even if a later service reenters and replaces it.
    const auto previous_target = state->target;
    if (previous_target != target) {
        OwnerFacts* owner_before_debug = state->owner;
        if (!aligned(owner_before_debug, alignof(OwnerFacts)) ||
            !owner_before_debug->identity) {
            return invalid_argument;
        }
        owner_before_debug->target_change_marker_14d0 = 0;
    }

    std::uint32_t trace_changes = 0;
    if (!debug_switch(state, services, trace_target_changes, trace_changes)) {
        return source_service_failed;
    }

    // The source compares the *fresh* target after DebugSwitches::GetSwitch.
    // Its ordinary path writes the new pointer before any target predicates.
    if (trace_changes && state->target != target && (state->target || target)) {
        // Trace detail lookup is diagnostic only; the original ignores its
        // value. Every changed transition with either pointer nonnull reaches
        // this query, including nonnull -> null.
        std::uint32_t ignored_detail_switch = 0;
        if (!debug_switch(state, services, trace_target_details,
                          ignored_detail_switch)) {
            return source_service_failed;
        }
    }

    state->target = target;
    return update_target_snapshots(state, services);
}

}  // namespace dh2::character::set_target
