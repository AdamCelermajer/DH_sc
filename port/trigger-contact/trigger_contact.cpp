#include "trigger_contact.hpp"
#include "../zone-contact-runtime/zone_geometry.hpp"

#include <cmath>
#include <string.h>

namespace dh2_trigger_contact {

const SourceMetadata INFECTED_VILLAGE_AMBUSH = {
    "005_infectedvillage",
    "_prim_TriggerZone_ambush",
    "Ambush",
    {-3448.5f, 3000.0f, 0.0f},
    {5993.8f, -1889.17f, 1313.39f},
    {2545.3f, 1110.83f, 1313.39f},
    {0.0f, 0.0f, 0.0f},
    {1.92682f, 1.92682f, 1.0f},
    {200.0f, 200.0f, 200.0f},
    1,
    0,
    1
};

namespace {

static bool valid(const Aabb *box) {
    return box != NULL && box->min_x <= box->max_x &&
        box->min_y <= box->max_y && box->min_z <= box->max_z;
}

static bool overlaps_axis(float min_left, float max_left,
                          float min_right, float max_right) {
    return min_left <= max_right && max_left >= min_right;
}

static bool door_blocks(const Frame *frame) {
    return frame->has_associated_door &&
        (frame->associated_door_state == 1 ||
         frame->associated_door_state == 3);
}

}  // namespace

void init_state(State *state) {
    if (state != NULL) memset(state, 0, sizeof(*state));
}

bool overlaps_closed(const Aabb *left, const Aabb *right) {
    return valid(left) && valid(right) &&
        overlaps_axis(left->min_x, left->max_x, right->min_x, right->max_x) &&
        overlaps_axis(left->min_y, left->max_y, right->min_y, right->max_y) &&
        overlaps_axis(left->min_z, left->max_z, right->min_z, right->max_z);
}

bool make_approximate_trigger_bounds(Aabb *bounds) {
    const SourceMetadata *source = &INFECTED_VILLAGE_AMBUSH;
    if (bounds == NULL || !source->zone_dimensions_are_inherited) return false;
    const dh2_zone_contact::Vec3 dimensions = {
        source->zone_dimensions[0], source->zone_dimensions[1],
        source->zone_dimensions[2]
    };
    const dh2_zone_contact::Vec3 scale = {
        source->scale[0], source->scale[1], source->scale[2]
    };
    dh2_zone_contact::Aabb local_bounds = {};
    if (!dh2_zone_contact::make_zone_local_aabb(
            &dimensions, &scale, &local_bounds)) return false;

    const float *center = source->world_position;
    if (!std::isfinite(center[0]) || !std::isfinite(center[1]) ||
        !std::isfinite(center[2])) return false;
    const Aabb translated = {
        center[0] + local_bounds.min.x,
        center[1] + local_bounds.min.y,
        center[2] + local_bounds.min.z,
        center[0] + local_bounds.max.x,
        center[1] + local_bounds.max.y,
        center[2] + local_bounds.max.z
    };
    if (!valid(&translated) || !std::isfinite(translated.min_x) ||
        !std::isfinite(translated.min_y) || !std::isfinite(translated.min_z) ||
        !std::isfinite(translated.max_x) || !std::isfinite(translated.max_y) ||
        !std::isfinite(translated.max_z)) return false;
    *bounds = translated;
    return true;
}

namespace {
Status update_impl(dh2_script_runtime::Runtime *runtime,
                   State *state,
                   const Frame *frame,
                   const char *trigger_name,
                   int32_t trigger_script_id,
                   int32_t trigger_count,
                   bool source_specific) {
    using namespace dh2_script_runtime;

    if (runtime == NULL || state == NULL || frame == NULL ||
        !valid(&frame->trigger_bounds) ||
        (frame->player_count != 0 && frame->players == NULL)) {
        return STATUS_INVALID_ARGUMENT;
    }
    if (runtime->error != ERROR_OK) return STATUS_RUNTIME_ERROR;

    /* TriggerZone::Update returns before sampling contacts for these gates;
     * preserve the edge latch on each early return. */
    if (frame->local_player_marked_scripted) {
        return STATUS_BLOCKED_SCRIPTED_PLAYER;
    }
    if (door_blocks(frame)) return STATUS_BLOCKED_DOOR;
    if (!frame->enabled) return STATUS_BLOCKED_DISABLED;
    const auto activation_count = source_specific
        ? state->trigger_activations : runtime->trigger_activations;
    const auto activation_limit = source_specific
        ? trigger_count : runtime->trigger_count;
    if (activation_limit >= 0 &&
        activation_count >= static_cast<uint32_t>(activation_limit)) {
        return STATUS_BLOCKED_ACTIVATION_COUNT;
    }
    if (frame->delay_timer_ms >= 1) return STATUS_BLOCKED_DELAY;

    /* TriggerZone's online-only virtual branch is not modeled by this
     * one-player offline slice. Reject it explicitly rather than guessing. */
    if (frame->online) return STATUS_UNSUPPORTED_ONLINE;

    /* The recovered direct GameObject::MeetCondition implementation returns
     * true unconditionally. No additional condition system is inferred here. */
    uint32_t touching_players = 0;
    for (uint32_t i = 0; i < frame->player_count; ++i) {
        const PlayerAabb *player = &frame->players[i];
        if (player->has_character && !valid(&player->bounds)) {
            return STATUS_INVALID_ARGUMENT;
        }
        if (player->has_character &&
            overlaps_closed(&frame->trigger_bounds, &player->bounds)) {
            ++touching_players;
        }
    }

    /* TriggerZone samples GameObject::GetNumPlayerTouching, which calls
     * GameObject::IsTouching and compares absolute AABBs. Zone::IsInside is a
     * separate function and is not part of this path. Infected Village leaves
     * script_all_player empty, so the ordinary `script` policy is represented
     * by at least one touching Character. */
    if (touching_players == 0) {
        state->qualifying_contact = 0;
        return STATUS_NO_CONTACT;
    }
    if (state->qualifying_contact) return STATUS_SUSTAINED_CONTACT;

    state->qualifying_contact = 1;
    const uint32_t events_before = runtime->event_count;
    if (source_specific) {
        if (!enter_trigger_for(runtime, trigger_script_id, trigger_name,
                trigger_count, &state->trigger_activations,
                &state->trigger_fired)) return STATUS_RUNTIME_ERROR;
    } else if (!enter_trigger(runtime)) {
        return STATUS_RUNTIME_ERROR;
    }

    for (uint32_t i = events_before; i < runtime->event_count; ++i) {
        if (runtime->events[i].type == EVENT_TRIGGER_SCRIPT_ALREADY_RUNNING) {
            return STATUS_SCRIPT_ALREADY_RUNNING;
        }
        if (runtime->events[i].type == EVENT_TRIGGER_STARTED) {
            return STATUS_ACTIVATED;
        }
    }
    return STATUS_RUNTIME_ERROR;
}
}  // namespace

Status update(dh2_script_runtime::Runtime *runtime,
              State *state,
              const Frame *frame) {
    return update_impl(runtime, state, frame, NULL, -1, 0, false);
}

Status update_for(dh2_script_runtime::Runtime *runtime,
                  State *state,
                  const Frame *frame,
                  const char *trigger_name,
                  int32_t trigger_script_id,
                  int32_t trigger_count) {
    if (!trigger_name || trigger_script_id < 0) return STATUS_INVALID_ARGUMENT;
    return update_impl(runtime, state, frame, trigger_name,
                       trigger_script_id, trigger_count, true);
}

const char *status_name(Status status) {
    switch (status) {
        case STATUS_NO_CONTACT: return "no contact";
        case STATUS_ACTIVATED: return "activated";
        case STATUS_SUSTAINED_CONTACT: return "sustained contact";
        case STATUS_SCRIPT_ALREADY_RUNNING: return "script already running";
        case STATUS_BLOCKED_SCRIPTED_PLAYER: return "blocked: scripted player";
        case STATUS_BLOCKED_DISABLED: return "blocked: disabled";
        case STATUS_BLOCKED_ACTIVATION_COUNT: return "blocked: activation count";
        case STATUS_BLOCKED_DELAY: return "blocked: delay";
        case STATUS_BLOCKED_DOOR: return "blocked: door";
        case STATUS_UNSUPPORTED_ONLINE: return "unsupported: online path";
        case STATUS_INVALID_ARGUMENT: return "invalid argument";
        case STATUS_RUNTIME_ERROR: return "runtime error";
    }
    return "unknown status";
}

}  // namespace dh2_trigger_contact
