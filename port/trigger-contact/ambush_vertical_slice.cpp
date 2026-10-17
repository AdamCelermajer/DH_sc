#include "ambush_vertical_slice.hpp"

#include <cstring>

namespace dh2_trigger_contact {
namespace {

bool copy_bounded(char *output, uint32_t capacity, const char *input) {
    if (output == nullptr || capacity == 0 || input == nullptr) return false;
    uint32_t length = 0;
    while (length < capacity && input[length] != '\0') ++length;
    if (length == capacity) return false;
    std::memcpy(output, input, length + 1U);
    return true;
}

bool is_spawn_event(const dh2_script_runtime::Event &event) {
    using namespace dh2_script_runtime;
    return event.command_id == 30 &&
        (event.type == EVENT_CHARACTER_SPAWN_STATE_REQUESTED ||
         event.type == EVENT_OBJECT_LOOKUP_MISS);
}

}  // namespace

void init_projection_state(ProjectionState *state) {
    if (state != nullptr) std::memset(state, 0, sizeof(*state));
}

bool build_ambush_runtime_seeds(const dh2::actors::Registry *registry,
                                dh2_script_runtime::ObjectSeed *seeds,
                                uint32_t capacity,
                                uint32_t *out_count) {
    using namespace dh2_script_runtime;
    if (out_count != nullptr) *out_count = 0;
    if (registry == nullptr || out_count == nullptr ||
        (registry->actor_count != 0 && registry->actors == nullptr) ||
        registry->actor_count > capacity ||
        (registry->actor_count != 0 && seeds == nullptr) ||
        registry->actor_count > MAX_OBJECTS) return false;

    for (uint32_t i = 0; i < registry->actor_count; ++i) {
        ObjectSeed &seed = seeds[i];
        std::memset(&seed, 0, sizeof(seed));
        if (!copy_bounded(seed.name, sizeof(seed.name), registry->actors[i].name) ||
            !copy_bounded(seed.gametype, sizeof(seed.gametype), "Character") ||
            !copy_bounded(seed.ai_state, sizeof(seed.ai_state),
                          registry->actors[i].has_ai_state
                              ? registry->actors[i].ai_state : "")) {
            return false;
        }
        seed.auto_spawn = registry->actors[i].auto_spawn;
    }
    *out_count = registry->actor_count;
    return true;
}

ProjectionStatus advance_ambush_and_project(
    dh2_script_runtime::Runtime *runtime,
    dh2::actors::Registry *registry,
    ProjectionState *state,
    uint32_t delta_ms) {
    using namespace dh2::actors;
    using namespace dh2_script_runtime;

    if (runtime == nullptr || registry == nullptr || state == nullptr ||
        state->event_cursor > runtime->event_count ||
        state->request_count > MAX_AMBUSH_PROJECTED_REQUESTS) {
        return PROJECTION_INVALID_ARGUMENT;
    }
    if (advance(runtime, delta_ms) != ERROR_OK)
        return PROJECTION_SCRIPT_ERROR;

    const Event *pending[MAX_AMBUSH_PROJECTED_REQUESTS];
    uint32_t pending_count = 0;
    for (uint32_t i = state->event_cursor; i < runtime->event_count; ++i) {
        const Event &event = runtime->events[i];
        if (!is_spawn_event(event)) continue;
        if (pending_count == MAX_AMBUSH_PROJECTED_REQUESTS ||
            state->request_count + pending_count >= MAX_AMBUSH_PROJECTED_REQUESTS) {
            return PROJECTION_TOO_MANY_REQUESTS;
        }
        if (event.detail[0] == '\0') return PROJECTION_SOURCE_REGISTRY_MISMATCH;

        const ActorInstance *actor = find(registry, event.detail);
        if (event.type == EVENT_CHARACTER_SPAWN_STATE_REQUESTED && actor == nullptr)
            return PROJECTION_SOURCE_REGISTRY_MISMATCH;
        if (event.type == EVENT_OBJECT_LOOKUP_MISS && actor != nullptr)
            return PROJECTION_SOURCE_REGISTRY_MISMATCH;
        pending[pending_count++] = &event;
    }

    for (uint32_t i = 0; i < pending_count; ++i) {
        const Event &event = *pending[i];
        const SpawnResult result = request_spawn(registry, event.detail);
        const bool expected = event.type == EVENT_CHARACTER_SPAWN_STATE_REQUESTED
            ? result == SpawnResult::requested || result == SpawnResult::already_requested
            : result == SpawnResult::lookup_miss;
        if (!expected) return PROJECTION_REGISTRY_ERROR;

        ProjectedSpawnRequest &record = state->requests[state->request_count++];
        if (!copy_bounded(record.actor_name, sizeof(record.actor_name), event.detail))
            return PROJECTION_SOURCE_REGISTRY_MISMATCH;
        record.result = result;
        record.script_event_index = static_cast<uint32_t>(&event - runtime->events);
    }
    state->event_cursor = runtime->event_count;
    return PROJECTION_OK;
}

const char *projection_status_name(ProjectionStatus status) {
    switch (status) {
        case PROJECTION_OK: return "ok";
        case PROJECTION_INVALID_ARGUMENT: return "invalid argument";
        case PROJECTION_SCRIPT_ERROR: return "script runtime error";
        case PROJECTION_TOO_MANY_REQUESTS: return "too many spawn requests";
        case PROJECTION_SOURCE_REGISTRY_MISMATCH: return "source registry mismatch";
        case PROJECTION_REGISTRY_ERROR: return "actor registry error";
    }
    return "unknown";
}

}  // namespace dh2_trigger_contact
