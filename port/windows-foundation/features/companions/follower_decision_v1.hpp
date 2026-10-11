#pragma once
// B077: data-driven follower/companion decisions, platform- and engine-free.
//
// Source evidence (libDungeonHunter2.so, see coordination/claude-preview161/B077-report.md):
//  * CharAI::_UpdateMaster 0x3cc5a4 raises the master events from three predicates: AI_IsInSight (view radius,
//    edge events 20 OutOfSight / 21 InSight), then, while in sight, close range (24), ranged range (23),
//    out of range (22) for ranged characters, or melee range (25) / out of range (22) for melee ones.
//  * For a friendly master the "close"/"melee" predicate is AI_IsInInteractionRange 0x3d4f98:
//    (distance - own melee radius - master body radius) <= AI row InteractRadius.
//  * follower.luac / attacking_follower.luac: out of range -> MoveTo(master) unless already moving, in any range
//    event -> Stop, out of sight -> WarpBehind(master) = master position + normalize(-lookAt) * 100
//    (Character::_WarpBehind 0x3b8d98). rene.luac: far/catching-up state machine with a haste buff, a 200 ms
//    CaughtUp timer and a WarpTo(master) fallback when there is no path.
// The script name of the actor's AI row selects the behaviour; nothing here names a map or an NPC.
#include <string_view>

namespace dh::foundation::companions {

enum class FollowerScriptV1 { none, follower, attacking_follower, rene };

// AiTables row `Script` value.
FollowerScriptV1 follower_script_v1(std::string_view ai_script) noexcept;

struct FollowerRadiiV1 {
    float view_radius = 0;      // AiProps.view_radius (AI_IsInSight)
    float interact_radius = 0;  // AiProps.interact_radius (AI_IsInInteractionRange)
    float own_reach = 0;        // AiProps.melee_radius (+ weapon reach when the actor has one)
    float master_radius = 0;    // master body radius (GameObject vtable +148)
};

struct FollowerMemoryV1 {
    bool prev_sight = false;    // CharAI +85; starts false, so a follower that never saw its master never warps
    bool moving = false;        // the actor is currently walking toward the master (script GetState()==Move)
    bool far = false;           // rene g_is_far
    bool catching_up = false;   // rene g_is_catching_up
    double caught_timer_ms = -1;// rene g_catchup_timer; < 0 means no timer
};

struct FollowerInputV1 {
    float distance = 0;         // 3D distance between the two target positions
    bool master_alive = true;
    double dt_ms = 0;
    FollowerRadiiV1 radii;
};

enum class FollowerActionV1 {
    none,
    approach,        // MoveTo(master): continue/start the route and walk
    stop,            // Stop(): leave the route, idle
    warp_behind,     // WarpBehind(master)
    warp_to_master   // WarpTo(master) (rene no-path fallback; decided by the controller)
};

struct FollowerDecisionV1 {
    FollowerActionV1 action = FollowerActionV1::none;
    bool apply_haste = false;   // rene CatchUp: ApplyBuff(Buff_Rene_Haste) + EnableCollisions(false)
    bool release_haste = false; // rene CaughtUp: RemoveBuff + EnableCollisions(true) (+ stop)
};

// True when the actor is within the interaction range of its master.
bool follower_close_v1(const FollowerInputV1&) noexcept;
// True while the master is inside the AI view radius.
bool follower_in_sight_v1(const FollowerInputV1&) noexcept;

// One AI turn. Updates `memory` and returns what the host must execute.
FollowerDecisionV1 follower_step_v1(FollowerScriptV1, FollowerMemoryV1&, const FollowerInputV1&) noexcept;

// Position behind the master: master + normalize(-lookAt) * 100 on the ground plane (z unchanged).
// `heading` is the master's yaw in radians (look-at = (cos, sin)).
void follower_warp_behind_position_v1(const float master_position[3], float master_heading, float out[3]) noexcept;

} // namespace dh::foundation::companions
