#ifndef DH2_TRIGGER_CONTACT_HPP
#define DH2_TRIGGER_CONTACT_HPP

#include "../script-runtime/script_runtime.hpp"

#include <stdint.h>

namespace dh2_trigger_contact {

struct SourceMetadata {
    const char *level_name;
    const char *trigger_name;
    const char *script_name;
    float module_position[3];
    float local_position[3];
    float world_position[3];
    float rotation[3];
    float scale[3];
    float zone_dimensions[3];
    int32_t activation_count;
    int32_t delay_ms;
    uint8_t zone_dimensions_are_inherited;
};

/* Placement facts are from 005_infectedvillage.mlx and infected01.mgp. The
 * inherited dimensions come from Zone's native default. TriggerZone contact
 * follows GameObject::GetNumPlayerTouching -> GameObject::IsTouching and uses
 * absolute AABB overlap; it does not call Zone::IsInside. For this zero-rotation
 * fixture, InitPost's relative box followed by SetRelativeAABB and
 * UpdateAbsoluteAABB gives the translated world bounds used by that path. */
extern const SourceMetadata INFECTED_VILLAGE_AMBUSH;

struct Aabb {
    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;
};

/* Builds the TriggerZone's world AABB for the historical Infected Village
 * fixture from its inherited dimensions, scale, and resolved center. This is
 * the same relative-box then absolute-box translation used by the recovered
 * Zone/GameObject path. Rotation is zero in this fixture. The helper name is
 * retained for source/API compatibility; it does not model Zone::IsInside,
 * which is a separate API and is not used by TriggerZone contact. */
bool make_approximate_trigger_bounds(Aabb *bounds);

struct PlayerAabb {
    Aabb bounds;
    uint8_t has_character;
};

struct Frame {
    /* Absolute TriggerZone bounds and absolute Character bounds, matching
     * GameObject::IsTouching. For the historical fixture helper, see
     * make_approximate_trigger_bounds. */
    Aabb trigger_bounds;
    const PlayerAabb *players;
    uint32_t player_count;
    int32_t delay_timer_ms;
    int32_t associated_door_state;
    uint8_t local_player_marked_scripted;
    uint8_t enabled;
    uint8_t online;
    uint8_t has_associated_door;
};

struct State {
    uint8_t qualifying_contact;
    uint8_t trigger_fired;
    uint32_t trigger_activations;
};

enum Status {
    STATUS_NO_CONTACT = 0,
    STATUS_ACTIVATED,
    STATUS_SUSTAINED_CONTACT,
    STATUS_SCRIPT_ALREADY_RUNNING,
    STATUS_BLOCKED_SCRIPTED_PLAYER,
    STATUS_BLOCKED_DISABLED,
    STATUS_BLOCKED_ACTIVATION_COUNT,
    STATUS_BLOCKED_DELAY,
    STATUS_BLOCKED_DOOR,
    STATUS_UNSUPPORTED_ONLINE,
    STATUS_INVALID_ARGUMENT,
    STATUS_RUNTIME_ERROR
};

void init_state(State *state);

/* Uses closed AABBs supplied by the caller. The offline adapter applies only
 * the recovered TriggerZone gates supported by this API and then delegates a
 * rising contact edge to script-runtime::enter_trigger(). */
bool overlaps_closed(const Aabb *left, const Aabb *right);
Status update(dh2_script_runtime::Runtime *runtime,
              State *state,
              const Frame *frame);
Status update_for(dh2_script_runtime::Runtime *runtime,
                  State *state,
                  const Frame *frame,
                  const char *trigger_name,
                  int32_t trigger_script_id,
                  int32_t trigger_count);
const char *status_name(Status status);

}  // namespace dh2_trigger_contact

#endif
