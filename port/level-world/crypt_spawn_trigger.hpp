#ifndef DH2_LEVEL_WORLD_CRYPT_SPAWN_TRIGGER_HPP
#define DH2_LEVEL_WORLD_CRYPT_SPAWN_TRIGGER_HPP

#include "../script-runtime/script_runtime.hpp"
#include "../trigger-contact/trigger_contact.hpp"
#include "../zone-contact-runtime/zone_geometry.hpp"

#include <stdint.h>

namespace dh2_crypt_spawn_trigger {

/* Authored values from crypt_straight_c_ns_01.mgp. The parent module's final
 * transform is intentionally supplied by the caller: generated Crypt layouts
 * can place and rotate this module differently from x07_crypt_backup.mlx. */
struct SourceFacts {
    const char *trigger_name;
    const char *script_name;
    const char *module_gameplay_file;
    dh2_zone_contact::Vec3 authored_local_position;
    dh2_zone_contact::Vec3 authored_object_scale;
    dh2_zone_contact::Vec3 inherited_zone_dimensions;
    int32_t activation_limit;
    int32_t configured_delay_ms;
    uint8_t has_script_move_out;
    uint8_t has_script_all_player;
    uint8_t has_script_all_player_move_out;
    uint8_t has_one_player_effect;
    uint8_t has_associated_door;
};

extern const SourceFacts GHOST_AMBUSH_01;

using Aabb = dh2_trigger_contact::Aabb;
using PlayerAabb = dh2_trigger_contact::PlayerAabb;
using State = dh2_trigger_contact::State;

/* The owner position and scale are the resolved GameObject values after the
 * level/module loader has placed it. The original Zone/SetRelativeAABB/
 * UpdateAbsoluteAABB path first forms +/- (dimensions * scale / 2), then adds
 * owner position componentwise. It does not rotate those box endpoints. */
bool make_world_bounds(const SourceFacts *source,
                       const dh2_zone_contact::Vec3 *owner_world_position,
                       const dh2_zone_contact::Vec3 *owner_scale,
                       Aabb *world_bounds);

struct Frame {
    dh2_zone_contact::Vec3 owner_world_position;
    dh2_zone_contact::Vec3 owner_scale;
    const PlayerAabb *players;
    uint32_t player_count;
    int32_t delay_timer_ms;
    uint8_t local_player_marked_scripted;
    uint8_t enabled;
    uint8_t online;
};

using Status = dh2_trigger_contact::Status;

void init_state(State *state);

/* Bounded to this authored ordinary-script, one-shot trigger. Contact follows
 * TriggerZone::Update -> GetNumPlayerTouching -> GameObject::IsTouching, so
 * PlayerAabb is the already-updated absolute bounds of a non-null Character.
 * This route does not call Zone::IsInside and therefore must not impose that
 * separate predicate's PhysicalObject guard. */
Status update(dh2_script_runtime::Runtime *runtime,
              State *state,
              const Frame *frame);
Status update(dh2_script_runtime::Runtime *runtime,
              State *state,
              const SourceFacts *source,
              const Frame *frame);

}  // namespace dh2_crypt_spawn_trigger

#endif
