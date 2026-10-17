#ifndef DH2_AMBUSH_VERTICAL_SLICE_HPP
#define DH2_AMBUSH_VERTICAL_SLICE_HPP

#include "../actor-runtime/actor_registry.hpp"
#include "../script-runtime/script_runtime.hpp"
#include "trigger_contact.hpp"

#include <stdint.h>

namespace dh2_trigger_contact {

constexpr uint32_t MAX_AMBUSH_PROJECTED_REQUESTS = 5;

struct ProjectedSpawnRequest {
    char actor_name[dh2::actors::MAX_ACTOR_NAME];
    dh2::actors::SpawnResult result;
    uint32_t script_event_index;
};

/* Cursor and request journal owned by this adapter. The journal records only
 * scheduler SpawnCharacter events projected into the existing actor registry.
 * It does not allocate actors or submit render objects. */
struct ProjectionState {
    uint32_t event_cursor;
    uint32_t request_count;
    ProjectedSpawnRequest requests[MAX_AMBUSH_PROJECTED_REQUESTS];
};

enum ProjectionStatus {
    PROJECTION_OK = 0,
    PROJECTION_INVALID_ARGUMENT,
    PROJECTION_SCRIPT_ERROR,
    PROJECTION_TOO_MANY_REQUESTS,
    PROJECTION_SOURCE_REGISTRY_MISMATCH,
    PROJECTION_REGISTRY_ERROR
};

void init_projection_state(ProjectionState *state);

/* Build script-runtime seeds from imported Character rows. The scheduler's
 * SpawnCharacter command operates on preloaded source records; this helper
 * copies names/state only and does not instantiate native Character objects. */
bool build_ambush_runtime_seeds(const dh2::actors::Registry *registry,
                                dh2_script_runtime::ObjectSeed *seeds,
                                uint32_t capacity,
                                uint32_t *out_count);

/* Advance the already initialized script scheduler, then project only its new
 * SpawnCharacter success/miss events through actor-runtime::request_spawn. */
ProjectionStatus advance_ambush_and_project(
    dh2_script_runtime::Runtime *runtime,
    dh2::actors::Registry *registry,
    ProjectionState *state,
    uint32_t delta_ms);

const char *projection_status_name(ProjectionStatus status);

}  // namespace dh2_trigger_contact

#endif
