#include "crypt_spawn_trigger.hpp"

#include <cmath>
#include <cstring>

namespace dh2_crypt_spawn_trigger {

const SourceFacts GHOST_AMBUSH_01 = {
    "_prim_TriggerZone_GhostAmbush01",
    "GhostAmbush01",
    "crypt_straight_c_ns_01.mgp",
    {-1405.23f, 300.262f, 608.062f},
    {5.06089f, 1.43277f, 1.0f},
    {200.0f, 200.0f, 200.0f},
    1,
    0,
    0,
    0,
    0,
    0,
    0
};

namespace {

bool valid_world_position(const dh2_zone_contact::Vec3 *position) {
    return position != NULL && std::isfinite(position->x) &&
        std::isfinite(position->y) && std::isfinite(position->z);
}

bool valid_source(const SourceFacts *source) {
    return source != NULL && source->trigger_name != NULL &&
        source->trigger_name[0] != '\0' && source->script_name != NULL &&
        source->script_name[0] != '\0' && source->activation_limit >= -1 &&
        source->configured_delay_ms == 0 && !source->has_script_move_out &&
        !source->has_script_all_player &&
        !source->has_script_all_player_move_out &&
        !source->has_one_player_effect && !source->has_associated_door;
}

bool runtime_matches_source(const dh2_script_runtime::Runtime *runtime,
                            const SourceFacts *source) {
    if (runtime == NULL || source == NULL || runtime->common_table == NULL ||
        runtime->level_table == NULL || runtime->trigger_name[0] == '\0' ||
        std::strcmp(runtime->trigger_name, source->trigger_name) != 0 ||
        runtime->trigger_count != source->activation_limit) {
        return false;
    }
    const int32_t expected_script_id = dh2_script_resolve_id(
        runtime->common_table, runtime->level_table,
        reinterpret_cast<const uint8_t *>(source->script_name),
        static_cast<uint32_t>(std::strlen(source->script_name)), 1);
    return expected_script_id >= 0 && runtime->trigger_script_id == expected_script_id;
}

}  // namespace

bool make_world_bounds(const SourceFacts *source,
                       const dh2_zone_contact::Vec3 *owner_world_position,
                       const dh2_zone_contact::Vec3 *owner_scale,
                       Aabb *world_bounds) {
    if (!valid_source(source) || !valid_world_position(owner_world_position) ||
        owner_scale == NULL || world_bounds == NULL) {
        return false;
    }

    dh2_zone_contact::Aabb relative = {};
    if (!dh2_zone_contact::make_zone_local_aabb(
            &source->inherited_zone_dimensions, owner_scale, &relative)) {
        return false;
    }
    const Aabb absolute = {
        owner_world_position->x + relative.min.x,
        owner_world_position->y + relative.min.y,
        owner_world_position->z + relative.min.z,
        owner_world_position->x + relative.max.x,
        owner_world_position->y + relative.max.y,
        owner_world_position->z + relative.max.z
    };
    if (!std::isfinite(absolute.min_x) || !std::isfinite(absolute.min_y) ||
        !std::isfinite(absolute.min_z) || !std::isfinite(absolute.max_x) ||
        !std::isfinite(absolute.max_y) || !std::isfinite(absolute.max_z) ||
        absolute.min_x > absolute.max_x || absolute.min_y > absolute.max_y ||
        absolute.min_z > absolute.max_z) {
        return false;
    }
    *world_bounds = absolute;
    return true;
}

void init_state(State *state) {
    dh2_trigger_contact::init_state(state);
}

Status update(dh2_script_runtime::Runtime *runtime,
              State *state,
              const Frame *frame) {
    if (runtime == NULL || state == NULL || frame == NULL ||
        !valid_source(&GHOST_AMBUSH_01) ||
        !runtime_matches_source(runtime, &GHOST_AMBUSH_01) ||
        (frame->player_count != 0 && frame->players == NULL)) {
        return dh2_trigger_contact::STATUS_INVALID_ARGUMENT;
    }

    Aabb world_bounds = {};
    if (!make_world_bounds(&GHOST_AMBUSH_01,
                           &frame->owner_world_position,
                           &frame->owner_scale, &world_bounds)) {
        return dh2_trigger_contact::STATUS_INVALID_ARGUMENT;
    }

    dh2_trigger_contact::Frame contact_frame = {};
    contact_frame.trigger_bounds = world_bounds;
    contact_frame.players = frame->players;
    contact_frame.player_count = frame->player_count;
    contact_frame.delay_timer_ms = frame->delay_timer_ms;
    contact_frame.local_player_marked_scripted =
        frame->local_player_marked_scripted;
    contact_frame.enabled = frame->enabled;
    contact_frame.online = frame->online;
    /* The authored Crypt record has no associated-door binding. */
    contact_frame.has_associated_door = 0;
    return dh2_trigger_contact::update(runtime, state, &contact_frame);
}

Status update(dh2_script_runtime::Runtime *runtime,
              State *state,
              const SourceFacts *source,
              const Frame *frame) {
    if (runtime == NULL || state == NULL || frame == NULL ||
        !valid_source(source) || runtime->common_table == NULL ||
        runtime->level_table == NULL ||
        (frame->player_count != 0 && frame->players == NULL)) {
        return dh2_trigger_contact::STATUS_INVALID_ARGUMENT;
    }
    const auto script_id = dh2_script_resolve_id(
        runtime->common_table, runtime->level_table,
        reinterpret_cast<const uint8_t *>(source->script_name),
        static_cast<uint32_t>(std::strlen(source->script_name)), 0);
    if (script_id < 0) return dh2_trigger_contact::STATUS_INVALID_ARGUMENT;

    Aabb world_bounds = {};
    if (!make_world_bounds(source, &frame->owner_world_position,
                           &frame->owner_scale, &world_bounds)) {
        return dh2_trigger_contact::STATUS_INVALID_ARGUMENT;
    }
    dh2_trigger_contact::Frame contact_frame = {};
    contact_frame.trigger_bounds = world_bounds;
    contact_frame.players = frame->players;
    contact_frame.player_count = frame->player_count;
    contact_frame.delay_timer_ms = frame->delay_timer_ms;
    contact_frame.local_player_marked_scripted =
        frame->local_player_marked_scripted;
    contact_frame.enabled = frame->enabled;
    contact_frame.online = frame->online;
    contact_frame.has_associated_door = 0;
    return dh2_trigger_contact::update_for(runtime, state, &contact_frame,
        source->trigger_name, script_id, source->activation_limit);
}

}  // namespace dh2_crypt_spawn_trigger
