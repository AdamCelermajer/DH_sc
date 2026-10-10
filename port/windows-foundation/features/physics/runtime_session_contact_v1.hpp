#pragma once

#include "../../combat_session.hpp"
#include "../../../level-world/character_script_collision.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <string>

namespace dh::foundation::physics {

enum class RuntimeSessionContactEventV1 : std::uint32_t {
    begin = 0,
    persist = 1,
    end = 2,
    result = 3
};

// Live source-owned fields which CombatSession does not manufacture. The
// provider must read the actual Character/CharAI/AIS/controller owners for this
// exact Session actor. Controller identity is a coherence witness only: source
// collision events are terminal CharAI first-switch events and do not inherit
// the generic controller locked/global-blocked suppression.
struct RuntimeSessionContactSnapshotV1 {
    std::uintptr_t active_ais{};
    std::uintptr_t character{};
    std::uintptr_t char_ai{};
    std::uintptr_t controller{};
    ActorId controller_owner = invalid_actor_id;
    ActorId preferred_target = invalid_actor_id;
    std::uint32_t movement_state{};
    std::uint32_t paused{};
    std::uint32_t application_frame{};
    std::uint32_t dt_ms{};
    std::array<std::uintptr_t, 4> collision_methods{}; // +bc,+c0,+c4,+c8
};

struct RuntimeSessionContactServicesV1 {
    // Required for each reached nonempty, non-Result POCharacter contact. Must
    // borrow one coherent live source projection; no cached clock/controller.
    std::function<bool(ActorId, RuntimeSessionContactSnapshotV1&, std::string&)> snapshot;
    // Exact POCharacter Debug::Load / isTracingPlayersCollision prefix. The
    // owner IsPlayer branch is sourced from this Session's actual traits.
    std::function<bool(std::string&)> debug_load;
    std::function<bool(const char*, bool&, std::string&)> debug_switch;
    // Whole source Character::CancelSneaking continuation. Missing service is
    // a hard error only if the source branch reaches it.
    std::function<bool(ActorId, std::string&)> cancel_sneaking;
    // Whole same-CharAI AI_SetTarget continuation, including the source flags.
    // It must publish its accepted target through this Session's ActorState.
    std::function<bool(ActorId, ActorId, std::uint32_t, std::string&)> set_target;
};

struct RuntimeSessionContactAISFieldsV1 {
    // These are precisely AISDefault +bc/+c0. Paused/movement/target/controller
    // are borrowed per callback and are deliberately not shadow-owned here.
    std::uint32_t collision_ms{};
    std::uint32_t last_collision_frame{};
};

// Source POCharacter contact continuation on an existing CombatSession.
// Actor/target/health/FSM/controller/property/position remain Session or
// caller-owned. The owner retains only body-registration IDs, source AIS
// collision counter/frame projections, and the original world collision count.
class RuntimeSessionContactOwnerV1 {
public:
    RuntimeSessionContactOwnerV1(CombatSession&, RuntimeSessionContactServicesV1);

    bool bind_body(ActorId, std::string& error);
    bool unbind_body(ActorId, std::string& error);

    // `peer` must be resolved from the live PlayableActorBodies registry by the
    // caller. This API accepts only stable ActorIds and never casts a physics
    // context or accepts an unregistered Session actor as a physical peer.
    bool physical_event(ActorId owner, RuntimeSessionContactEventV1 event,
                        ActorId peer, std::uint32_t source_side,
                        std::string& error);

    // AISDefault::OnUpdate's source >199 collision-time gate. The reached
    // pause/timer/Stop continuation runs before the canonical counter resets;
    // if it fails, any delivered prefix is retained and collision_ms remains.
    using CollisionPauseContinuation = std::function<bool(ActorId,std::string&)>;
    bool process_collision_pause_threshold(ActorId,
                        const CollisionPauseContinuation&, bool& triggered,
                        std::string& error);

    std::uint32_t world_collision_count() const noexcept { return collisions_; }
    const RuntimeSessionContactAISFieldsV1* ais_fields(ActorId) const noexcept;
    bool owns_binding() const noexcept;

private:
    CombatSession& session_;
    std::weak_ptr<const CombatSessionLifetime> lifetime_;
    std::weak_ptr<const void> lease_;
    RuntimeSessionContactServicesV1 services_;
    std::map<ActorId, RuntimeSessionContactAISFieldsV1> ais_fields_;
    std::uint32_t collisions_{};

    bool validate_actor(ActorId, std::string&) const;
};

} // namespace dh::foundation::physics
